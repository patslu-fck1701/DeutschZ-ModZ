enum DZES_Phase { CANDIDATE, QUEUED, SCHEDULED, RESERVED, STARTING, ACTIVE, COMPLETED, ABANDONED, TIMEOUT, CLEANUP, CLEANUP_DONE, SLOT_RELEASE }

class DZES_State
{
 int Version = 2;
 string EventId;
 string ProviderId;
 string StartedUTC;
 int Phase = DZES_Phase.CANDIDATE;
 vector Position;
 ref array<string> Participants = new array<string>;
 ref array<string> RewardCommitted = new array<string>;
 ref array<string> SpawnedTypes = new array<string>;
 string RecoveryPolicy;
 string NextEvent;
 int NextEventAt;
 int NextEventAtUTCSeconds;
 string NextEventAtUTC;
 int ScheduledProviderGeneration;
 string LastEvent;
 int KothSkipped;
 int CourierSkipped;
 int RavenSkipped;
 int ConvoySkipped;
}

class DZES_Status
{
 string ActiveEvent;
 string ActiveEventId;
 string ActivePhase;
 string NextEvent;
 int NextEventAt;
 string NextEventAtUTC;
 int SecondsUntilNextEvent;
 int QueueCount;
 int CooldownRemaining;
 bool RestartBlock;
 int SecondsUntilRestart;
 string SelectionReason;
 int SelectionPriority;
}

class DZES_Settings
{
 int Version = 2;
 int MinMajorEventGapSeconds = 900;
 int MaxMajorEventGapSeconds = 1800;
 int RestartBlockSeconds = 1800;
 int EventSafetyReserveSeconds = 120;
 int ProviderCollectionSeconds = 30;
 int KothPreferenceBonus = 5;
 int WaitingBonusPerMinute = 2;
 int StarvationBonusPerSkippedRound = 15;
 int MaxSkippedRounds = 8;
 int RestoreRetryIntervalSeconds = 15;
 int RestoreTimeoutSeconds = 120;
 int SchedulerWatchdogTimeoutSeconds = 300;
 bool PreventImmediateRepeat = true;
}

// Stable provider-facing API. Event mods do not need access to scheduler internals.
class DZES_ProviderAPI
{
 static bool RequestLease(string providerId, int durationSeconds, int priority, string reason)
 {
  return DZES_Scheduler.TryAcquire(providerId, durationSeconds, priority, reason);
 }

 static bool Start(string providerId, vector position = "0 0 0")
 {
  return DZES_Scheduler.Transition(providerId, DZES_Phase.STARTING, position);
 }

 static bool Activate(string providerId, vector position = "0 0 0")
 {
  return DZES_Scheduler.Transition(providerId, DZES_Phase.ACTIVE, position);
 }

 static bool Complete(string providerId, vector position = "0 0 0")
 {
  return DZES_Scheduler.Transition(providerId, DZES_Phase.COMPLETED, position);
 }

 static bool BeginCleanup(string providerId, vector position = "0 0 0")
 {
  return DZES_Scheduler.Transition(providerId, DZES_Phase.CLEANUP, position);
 }

 static bool FinishCleanup(string providerId)
 {
  return DZES_Scheduler.FinishCleanup(providerId);
 }

 static void Heartbeat(string providerId)
 {
  DZES_Scheduler.Renew(providerId);
 }

 static void AddParticipant(string providerId, string uid)
 {
  DZES_Scheduler.Participant(providerId, uid);
 }

 static bool CommitParticipantReward(string providerId, string uid)
 {
  return DZES_Scheduler.CommitReward(providerId, uid);
 }
}

class DZES_Scheduler
{
 protected static ref DZES_State s_State;
 protected static ref DZES_Settings s_Settings;
 protected static int s_Heartbeat;
 protected static int s_LastHeartbeatLog;
 protected static int s_LastRestartBlockLog;
 protected static int s_CleanupSince;
 protected static int s_NextAllowedAt;
 protected static int s_PlanningOpenAt;
 protected static int s_RestoreStartedAt;
 protected static int s_LastRestoreRetryLog;
 protected static int s_LastGrantAt;
 protected static bool s_StartupFastSchedule;
 protected static bool s_Ready;
 protected static bool s_StorageFailed;
 protected static ref array<string> s_Queue = new array<string>;
 protected static ref map<string, int> s_LastRequest = new map<string, int>;
 protected static ref map<string, int> s_Priority = new map<string, int>;
 protected static ref map<string, int> s_QueuedSince = new map<string, int>;
 protected static ref map<string, string> s_SelectionReason = new map<string, string>;
 protected static ref map<string, string> s_StoryPhases = new map<string, string>;
 protected static ref map<string, int> s_ProviderGeneration = new map<string, int>;
 protected static ref map<string, int> s_ProviderFirstSeen = new map<string, int>;
 protected static int s_NextProviderGeneration = 1;

 // Runtime safety:
 // 3 min without heartbeat -> request cleanup.
 // If the provider still does not confirm cleanup for another 10 min,
 // archive + force-release the global slot and apply a cooldown.
 static const int HEARTBEAT_TIMEOUT_MS = 180000;
 static const int GRANT_START_TIMEOUT_MS = 30000;
 static const int CLEANUP_GRACE_MS = 600000;
 static const int STARTUP_GRACE_MS = 15000;
 static const int STARTUP_EVENT_MIN_SECONDS = 30;
 static const int STARTUP_EVENT_MAX_SECONDS = 120;
 static const int HEARTBEAT_LOG_INTERVAL_MS = 300000;
 static const int QUEUE_STALE_MS = 180000;
 static const int SCHEDULE_START_TIMEOUT_MS = 60000;

 static const string ROOT = "$profile:DeutschZ-System/deutschz_eventscheduler";

 static void Initialize()
 {
  if (s_Ready || !GetGame() || !GetGame().IsServer()) return;

  MakeDirectory("$profile:DeutschZ-System");
  MakeDirectory(ROOT);
  MakeDirectory(ROOT + "/history");

  s_Settings = new DZES_Settings;
  string settingsError;
  if (FileExist(ROOT + "/settings.json"))
  {
   if (!JsonFileLoader<DZES_Settings>.LoadFile(ROOT + "/settings.json", s_Settings, settingsError) || !s_Settings || (s_Settings.Version != 1 && s_Settings.Version != 2))
   {
    s_StorageFailed = true;
    s_Ready = true;
    Print("[EventSchedulerZ] BLOCKED invalid settings: " + settingsError);
    return;
   }
  }
  else if (!JsonFileLoader<DZES_Settings>.SaveFile(ROOT + "/settings.json", s_Settings, settingsError))
  {
   s_StorageFailed = true;
   s_Ready = true;
   Print("[EventSchedulerZ] BLOCKED settings create failed: " + settingsError);
   return;
  }
  if (s_Settings.Version == 1)
  {
   s_Settings.Version = 2;
   s_Settings.ProviderCollectionSeconds = 30;
   s_Settings.KothPreferenceBonus = 5;
   s_Settings.WaitingBonusPerMinute = 2;
   s_Settings.StarvationBonusPerSkippedRound = 15;
   s_Settings.MaxSkippedRounds = 8;
   s_Settings.RestoreRetryIntervalSeconds = 15;
   s_Settings.RestoreTimeoutSeconds = 120;
   s_Settings.SchedulerWatchdogTimeoutSeconds = 300;
   s_Settings.PreventImmediateRepeat = true;
   JsonFileLoader<DZES_Settings>.SaveFile(ROOT + "/settings.json", s_Settings, settingsError);
   Print("[EventSchedulerZ] SETTINGS_MIGRATED version=2");
  }
  s_Settings.MinMajorEventGapSeconds = Math.Clamp(s_Settings.MinMajorEventGapSeconds, 60, 86400);
  s_Settings.MaxMajorEventGapSeconds = Math.Clamp(s_Settings.MaxMajorEventGapSeconds, s_Settings.MinMajorEventGapSeconds, 86400);
  s_Settings.RestartBlockSeconds = Math.Clamp(s_Settings.RestartBlockSeconds, 300, 7200);
  s_Settings.EventSafetyReserveSeconds = Math.Clamp(s_Settings.EventSafetyReserveSeconds, 0, 1800);
  s_Settings.ProviderCollectionSeconds = Math.Clamp(s_Settings.ProviderCollectionSeconds, 5, 120);
  s_Settings.KothPreferenceBonus = Math.Clamp(s_Settings.KothPreferenceBonus, 0, 25);
  s_Settings.WaitingBonusPerMinute = Math.Clamp(s_Settings.WaitingBonusPerMinute, 0, 20);
  s_Settings.StarvationBonusPerSkippedRound = Math.Clamp(s_Settings.StarvationBonusPerSkippedRound, 1, 100);
  s_Settings.MaxSkippedRounds = Math.Clamp(s_Settings.MaxSkippedRounds, 1, 20);
  s_Settings.RestoreRetryIntervalSeconds = Math.Clamp(s_Settings.RestoreRetryIntervalSeconds, 5, 60);
  s_Settings.RestoreTimeoutSeconds = Math.Clamp(s_Settings.RestoreTimeoutSeconds, 30, 600);
  s_Settings.SchedulerWatchdogTimeoutSeconds = Math.Clamp(s_Settings.SchedulerWatchdogTimeoutSeconds, 60, 1800);

  s_State = new DZES_State;
  string error;

  bool restoredPlan = false;
  bool loadedState = false;
  int now = GetGame().GetTime();
  int nowUTC = ExpansionStatic.GetTimestamp(true);
  if (FileExist(ROOT + "/state.json"))
  {
   if (!JsonFileLoader<DZES_State>.LoadFile(ROOT + "/state.json", s_State, error) || !s_State || (s_State.Version != 1 && s_State.Version != 2))
   {
    s_StorageFailed = true;
    s_Ready = true;
    Print("[EventSchedulerZ] BLOCKED invalid state: " + error);
    return;
   }

   loadedState = true;
   if (s_State.Version == 1)
   {
    s_State.Version = 2;
    Print("[EventSchedulerZ] STATE_MIGRATED version=2");
   }

   if (s_State.EventId != "" && s_State.ProviderId != "")
   {
    // Runtime entities cannot survive a restart, but the selected provider
    // must retain its place in the chain and is restarted through its normal
    // lease path after the short startup grace period.
    string recoveryOwner = s_State.ProviderId;
    int recoveryKothSkipped = s_State.KothSkipped;
    int recoveryCourierSkipped = s_State.CourierSkipped;
    int recoveryRavenSkipped = s_State.RavenSkipped;
    int recoveryConvoySkipped = s_State.ConvoySkipped;
    Print("[EventSchedulerZ] RESTART_RECOVERY owner=" + recoveryOwner + " oldEvent=" + s_State.EventId);
    if (!ArchiveCurrent("RESTART_RECOVERY"))
    {
     s_StorageFailed = true;
     s_Ready = true;
     return;
    }
    s_State = new DZES_State;
    s_State.LastEvent = recoveryOwner;
    s_State.KothSkipped = recoveryKothSkipped;
    s_State.CourierSkipped = recoveryCourierSkipped;
    s_State.RavenSkipped = recoveryRavenSkipped;
    s_State.ConvoySkipped = recoveryConvoySkipped;
    s_State.NextEvent = recoveryOwner;
    s_State.NextEventAt = now + STARTUP_GRACE_MS;
    s_State.NextEventAtUTCSeconds = nowUTC + STARTUP_GRACE_MS / 1000;
    s_State.Phase = DZES_Phase.SCHEDULED;
    s_State.RecoveryPolicy = "REQUEUE_ON_RESTART";
    restoredPlan = true;
    Print("[EventSchedulerZ] RESTART_REQUEUED owner=" + recoveryOwner + " in=" + (STARTUP_GRACE_MS / 1000).ToString() + "s");
   }
   else if (s_State.NextEvent != "")
   {
    int remainingSeconds = STARTUP_GRACE_MS / 1000;
    if (s_State.NextEventAtUTCSeconds > nowUTC)
     remainingSeconds = s_State.NextEventAtUTCSeconds - nowUTC;
    remainingSeconds = Math.Clamp(remainingSeconds, STARTUP_GRACE_MS / 1000, s_Settings.MaxMajorEventGapSeconds);
    s_State.NextEventAt = now + remainingSeconds * 1000;
    s_State.NextEventAtUTCSeconds = nowUTC + remainingSeconds;
    s_State.Phase = DZES_Phase.SCHEDULED;
    s_State.RecoveryPolicy = "REQUEUE_ON_RESTART";
    restoredPlan = true;
    Print("[EventSchedulerZ] SCHEDULE_RESTORED owner=" + s_State.NextEvent + " in=" + remainingSeconds.ToString() + "s");
   }
   else if (s_State.NextEventAtUTCSeconds > nowUTC)
   {
    int cooldownRemaining = Math.Clamp(s_State.NextEventAtUTCSeconds - nowUTC, STARTUP_GRACE_MS / 1000, s_Settings.MaxMajorEventGapSeconds);
    s_State.NextEventAt = now + cooldownRemaining * 1000;
    s_State.NextEventAtUTCSeconds = nowUTC + cooldownRemaining;
    s_State.Phase = DZES_Phase.CANDIDATE;
    restoredPlan = true;
    Print("[EventSchedulerZ] COOLDOWN_RESTORED in=" + cooldownRemaining.ToString() + "s lastEvent=" + s_State.LastEvent);
   }
   else
   {
    s_State.NextEventAt = 0;
    s_State.NextEventAtUTCSeconds = 0;
    s_State.NextEventAtUTC = "";
    s_State.EventId = "";
    s_State.ProviderId = "";
    s_State.Phase = DZES_Phase.CANDIDATE;
   }
  }

  if (!loadedState) s_State = new DZES_State;
  s_Heartbeat = 0;
  s_LastHeartbeatLog = 0;
  s_LastRestartBlockLog = 0;
  s_CleanupSince = 0;
  s_RestoreStartedAt = 0;
  s_LastRestoreRetryLog = 0;
  s_LastGrantAt = now;
  if (restoredPlan) s_RestoreStartedAt = now;
  s_Ready = true;
  s_StartupFastSchedule = s_State.NextEventAt <= 0;
  if (s_State.NextEventAt > 0)
   s_NextAllowedAt = s_State.NextEventAt;
  else
   s_NextAllowedAt = now + STARTUP_GRACE_MS;
  s_PlanningOpenAt = now + STARTUP_GRACE_MS;
  Save();
 }

 static bool Save()
 {
  string error;
  if (!s_State || !JsonFileLoader<DZES_State>.SaveFile(ROOT + "/state.json", s_State, error))
  {
   s_StorageFailed = true;
   Print("[EventSchedulerZ] STORAGE ERROR " + error);
   return false;
  }
  return true;
 }

 protected static bool ArchiveCurrent(string reason)
 {
  if (!s_State || s_State.EventId == "") return true;

  string error;
  string path = ROOT + "/history/" + s_State.EventId + ".json";

  if (!JsonFileLoader<DZES_State>.SaveFile(path, s_State, error))
  {
   Print("[EventSchedulerZ] HISTORY ERROR " + reason + " " + error);
   return false;
  }

  Print("[EventSchedulerZ] HISTORY " + reason + " " + s_State.EventId);
  return true;
 }

 protected static bool ReleaseCurrent(string reason, int cooldownMs = -1)
 {
  if (!s_State) return false;

 string oldOwner = s_State.ProviderId;
 string oldEvent = s_State.EventId;
  int oldKothSkipped = s_State.KothSkipped;
  int oldCourierSkipped = s_State.CourierSkipped;
  int oldRavenSkipped = s_State.RavenSkipped;
  int oldConvoySkipped = s_State.ConvoySkipped;

  if (!ArchiveCurrent(reason))
  {
   s_StorageFailed = true;
   return false;
  }

  if (oldOwner != "")
  {
   s_Queue.RemoveItem(oldOwner);
   s_LastRequest.Remove(oldOwner);
   s_Priority.Remove(oldOwner);
   s_QueuedSince.Remove(oldOwner);
   s_SelectionReason.Remove(oldOwner);
  }

  if (cooldownMs < 0)
   cooldownMs = Math.RandomIntInclusive(s_Settings.MinMajorEventGapSeconds, s_Settings.MaxMajorEventGapSeconds) * 1000;

  s_State = new DZES_State;
  s_State.LastEvent = oldOwner;
  s_State.KothSkipped = oldKothSkipped;
  s_State.CourierSkipped = oldCourierSkipped;
  s_State.RavenSkipped = oldRavenSkipped;
  s_State.ConvoySkipped = oldConvoySkipped;
  s_State.Phase = DZES_Phase.SLOT_RELEASE;
  s_State.NextEventAt = GetGame().GetTime() + cooldownMs;
  s_State.NextEventAtUTCSeconds = ExpansionStatic.GetTimestamp(true) + Math.Ceil(cooldownMs / 1000.0);
  s_Heartbeat = 0;
  s_LastHeartbeatLog = 0;
  s_CleanupSince = 0;
  s_NextAllowedAt = GetGame().GetTime() + cooldownMs;
  s_PlanningOpenAt = GetGame().GetTime() + s_Settings.ProviderCollectionSeconds * 1000;

   Print("[EventSchedulerZ] EVENT RELEASED event=" + oldOwner + " run=" + oldEvent + " reason=" + reason);
   Print("[EventSchedulerZ] SLOT_RELEASE " + reason + " " + oldEvent + " owner=" + oldOwner);
  Print("[EventSchedulerZ] NEXT_EVENT_AT in=" + Math.Ceil(cooldownMs / 1000.0).ToString() + "s");
  return Save();
 }

 static int DayOfWeek(int year, int month, int day)
 {
  array<int> offsets = {0,3,2,5,0,3,5,1,4,6,2,4};
  if (month < 3) year--;

  int quarter = year / 4;
  int century = year / 100;
  int cycle = year / 400;
  int total = year + quarter - century + cycle + offsets.Get(month - 1) + day;
  return total - (total / 7) * 7;
 }

 static int RestartDistanceUTC(int year, int month, int day, int hour, int minute, int second)
 {
  bool summer = month > 3 && month < 10;

  if (month == 3 || month == 10)
  {
   int sunday = 31 - DayOfWeek(year, month, 31);
   bool afterSwitch = day > sunday || (day == sunday && hour >= 1);

   if (month == 3) summer = afterSwitch;
   else summer = !afterSwitch;
  }

  int offset = 1;
  if (summer) offset = 2;

  int berlinHour = hour + offset;
  if (berlinHour >= 24) berlinHour = berlinHour - 24;

  int berlin = berlinHour * 3600 + minute * 60 + second;
  array<int> restartHours = {1,5,9,13,17,21};

  foreach (int restartHour : restartHours)
  {
   int delta = restartHour * 3600 - berlin;
   if (delta >= 0) return delta;
  }

  return 86400 - berlin + 3600;
 }

 static int SecondsUntilRestart()
 {
  int year, month, day, hour, minute, second;
  GetYearMonthDayUTC(year, month, day);
  GetHourMinuteSecondUTC(hour, minute, second);
  return RestartDistanceUTC(year, month, day, hour, minute, second);
 }

 static bool TryAcquire(string owner, int durationSeconds = 1800, int priority = 0, string reason = "Standardrotation")
 {
  if (!GetGame() || !GetGame().IsServer() || owner == "") return false;

  Initialize();
  if (s_StorageFailed || !s_State) return false;

  int now = GetGame().GetTime();
  Sweep(now);

  int providerGeneration;
  int providerFirstSeen;
  if (!s_ProviderGeneration.Find(owner, providerGeneration))
  {
   providerGeneration = s_NextProviderGeneration++;
   s_ProviderGeneration.Set(owner, providerGeneration);
   s_ProviderFirstSeen.Set(owner, now);
   providerFirstSeen = now;
   Print("[EventSchedulerZ] PROVIDER_REGISTERED owner=" + owner + " generation=" + providerGeneration.ToString());
  }
  else
   s_ProviderFirstSeen.Find(owner, providerFirstSeen);

  // Existing owner may continue only while it is not in cleanup.
  if (s_State.ProviderId == owner)
   return s_State.Phase != DZES_Phase.CLEANUP;

  // Register/refresh this provider in the queue.
  int oldPriority;
  bool alreadyQueued = s_Queue.Find(owner) >= 0;
  bool hadPriority = s_Priority.Find(owner, oldPriority);
  if (!alreadyQueued)
  {
   s_Queue.Insert(owner);
   s_QueuedSince.Set(owner, now);
    Print("[EventSchedulerZ] EVENT QUEUED event=" + owner + " run=none priority=" + priority.ToString() + " reason=" + reason);
    Print("[EventSchedulerZ] QUEUED owner=" + owner + " priority=" + priority.ToString());
  }
  else if (!hadPriority || oldPriority != priority)
  {
   Print("[EventSchedulerZ] QUEUE_DEDUPED owner=" + owner + " oldPriority=" + oldPriority.ToString() + " newPriority=" + priority.ToString());
  }
  s_LastRequest.Set(owner, now);
  s_Priority.Set(owner, priority);
  s_SelectionReason.Set(owner, reason);
  UpdateNextEvent();

  if (s_State.ProviderId == "" && s_State.NextEvent == owner && now >= s_State.NextEventAt)
  {
   int oldGeneration = s_State.ScheduledProviderGeneration;
   int providerAgeSeconds = Math.Max(0, (now - providerFirstSeen) / 1000);
   Print("[EventSchedulerZ] RESOLVE_PROVIDER owner=" + owner + " found=1 ready=1 age=" + providerAgeSeconds.ToString() + " generation=" + providerGeneration.ToString());
   if (oldGeneration != providerGeneration)
   {
    Print("[EventSchedulerZ] PROVIDER_REACQUIRED owner=" + owner + " oldGeneration=" + oldGeneration.ToString() + " newGeneration=" + providerGeneration.ToString());
    s_State.ScheduledProviderGeneration = providerGeneration;
    Save();
   }
  }

  // A restored provider has already waited through the restart itself.  Its
  // first matching request must be allowed to reacquire immediately; otherwise
  // one-shot providers can leave the immutable schedule wedged forever.
  if (s_State.ProviderId == "" && s_State.NextEvent == owner && s_State.RecoveryPolicy == "REQUEUE_ON_RESTART")
  {
   s_State.NextEventAt = now;
   s_State.NextEventAtUTCSeconds = ExpansionStatic.GetTimestamp(true);
   s_NextAllowedAt = now;
   s_State.RecoveryPolicy = "RESTORED_PROVIDER_READY";
   Print("[EventSchedulerZ] RESTART_READY owner=" + owner + " action=immediate_reacquire");
  }

  if (s_State.ProviderId != "" || now < s_NextAllowedAt || now < s_State.NextEventAt)
   return false;

  // A scheduled owner is immutable until it starts or is explicitly invalidated.
  if (s_State.NextEvent == "" || s_State.NextEvent != owner)
   return false;

  // Never start a major event too close to a scheduled restart.
  if (SecondsUntilRestart() <= Math.Max(s_Settings.RestartBlockSeconds, durationSeconds + s_Settings.EventSafetyReserveSeconds))
  {
   if (s_LastRestartBlockLog == 0 || now - s_LastRestartBlockLog >= 60000)
   {
    Print("[EventSchedulerZ] RESTART_BLOCK owner=" + owner + " seconds=" + SecondsUntilRestart().ToString());
    s_LastRestartBlockLog = now;
   }
   return false;
  }

  string selected = SelectQueuedOwner();
  if (s_State.RecoveryPolicy == "RESTORED_PROVIDER_READY") selected = owner;
  if (selected != owner)
   return false;

  LogSelection(owner, "highest_effective_priority");

  string previousEvent = s_State.LastEvent;
  UpdateSkippedRounds(owner);
  int nextKothSkipped = s_State.KothSkipped;
  int nextCourierSkipped = s_State.CourierSkipped;
  int nextRavenSkipped = s_State.RavenSkipped;
  int nextConvoySkipped = s_State.ConvoySkipped;

  s_Queue.RemoveItem(owner);
  s_LastRequest.Remove(owner);
  s_Priority.Remove(owner);
  s_QueuedSince.Remove(owner);
  s_SelectionReason.Remove(owner);

  s_State = new DZES_State;
  s_State.LastEvent = previousEvent;
  s_State.KothSkipped = nextKothSkipped;
  s_State.CourierSkipped = nextCourierSkipped;
  s_State.RavenSkipped = nextRavenSkipped;
  s_State.ConvoySkipped = nextConvoySkipped;

  int year, month, day, hour, minute, second;
  GetYearMonthDayUTC(year, month, day);
  GetHourMinuteSecondUTC(hour, minute, second);

  s_State.StartedUTC = string.Format("%1-%2-%3_%4-%5-%6", year, month, day, hour, minute, second);
  s_State.EventId = owner + "_" + s_State.StartedUTC + "_" + now.ToString();
  s_State.ProviderId = owner;
  s_State.Phase = DZES_Phase.RESERVED;

  s_Heartbeat = now;
  s_LastHeartbeatLog = now;
  s_CleanupSince = 0;
  s_LastGrantAt = now;
  s_RestoreStartedAt = 0;
  s_LastRestoreRetryLog = 0;

  if (!Save()) return false;

   Print("[EventSchedulerZ] EVENT GRANTED event=" + owner + " run=" + s_State.EventId);
   Print("[EventSchedulerZ] RESERVED " + s_State.EventId);
  return true;
 }

 static bool Transition(string owner, int phase, vector position = "0 0 0")
 {
  if (!s_State || s_State.ProviderId != owner || s_StorageFailed)
   return false;

  if (phase < s_State.Phase || phase > DZES_Phase.CLEANUP)
   return false;

  s_State.Phase = phase;
  s_State.Position = position;

  if (phase == DZES_Phase.CLEANUP && s_CleanupSince == 0 && GetGame())
   s_CleanupSince = GetGame().GetTime();

  Renew(owner);
  if (phase == DZES_Phase.STARTING)
   Print("[EventSchedulerZ] EVENT STARTED event=" + owner + " run=" + s_State.EventId + " phase=STARTING");
  else if (phase == DZES_Phase.ACTIVE)
   Print("[EventSchedulerZ] EVENT RUNTIME event=" + owner + " run=" + s_State.EventId + " phase=ACTIVE");
  else if (phase == DZES_Phase.COMPLETED)
   Print("[EventSchedulerZ] EVENT FINISHED event=" + owner + " run=" + s_State.EventId + " reason=completed");
  else if (phase == DZES_Phase.ABANDONED || phase == DZES_Phase.TIMEOUT)
   Print("[EventSchedulerZ] EVENT ABORTED event=" + owner + " run=" + s_State.EventId + " reason=" + PhaseName(phase));
  Print("[EventSchedulerZ] " + PhaseName(phase) + " " + s_State.EventId);
  return Save();
 }

 static void Renew(string owner)
 {
  if (GetGame() && GetGame().IsServer() && s_State && s_State.ProviderId == owner)
  {
   int now = GetGame().GetTime();
   s_Heartbeat = now;
   if (s_LastHeartbeatLog == 0 || now - s_LastHeartbeatLog >= HEARTBEAT_LOG_INTERVAL_MS)
   {
    s_LastHeartbeatLog = now;
    Print("[EventSchedulerZ] HEARTBEAT event=" + s_State.EventId + " phase=" + PhaseName(s_State.Phase));
   }
  }
 }

 static bool CommitReward(string owner, string uid)
 {
  if (!s_State || s_State.ProviderId != owner || uid == "" || s_State.RewardCommitted.Find(uid) >= 0 || s_StorageFailed)
   return false;

  if (s_State.Participants.Find(uid) < 0)
   return false;

  s_State.RewardCommitted.Insert(uid);
  return Save();
 }

 static void Participant(string owner, string uid)
 {
  if (!s_State || s_State.ProviderId != owner || uid == "" || s_State.Participants.Find(uid) >= 0)
   return;

  s_State.Participants.Insert(uid);
  Save();
 }

 static bool FinishCleanup(string owner)
 {
  if (!s_State || s_State.ProviderId != owner || s_State.Phase != DZES_Phase.CLEANUP)
   return false;

  s_State.Phase = DZES_Phase.CLEANUP_DONE;
  Save();
  Print("[EventSchedulerZ] CLEANUP_DONE " + s_State.EventId);
  Print("[EventSchedulerZ] CLEANUP_CONFIRMED " + s_State.EventId);
  return ReleaseCurrent("CLEANUP_CONFIRMED");
 }

 // ----- Read-only status API for WelcomeZ / diagnostics -----

 static string EventId()
 {
  Initialize();
  if (s_State) return s_State.EventId;
  return "";
 }

 static string Owner()
 {
  Initialize();
  if (s_State) return s_State.ProviderId;
  return "";
 }

 static string ActiveEvent()
 {
  return Owner();
 }

 static string ActivePhase()
 {
  Initialize();
  if (s_State) return PhaseName(s_State.Phase);
  return "CANDIDATE";
 }

 static string NextEvent()
 {
  Initialize();
  if (s_State) return s_State.NextEvent;
  return "";
 }

 static int SecondsUntilNextEvent()
 {
  return NextEventStartInSeconds();
 }

 static int CooldownRemaining()
 {
  return CooldownRemainingSeconds();
 }

 static bool RestartBlocked()
 {
  return IsRestartBlocked();
 }

 static void StoryTransition(string owner, string phase)
 {
  if (!GetGame() || !GetGame().IsServer() || owner == "" || phase == "") return;
  string previous;
  if (s_StoryPhases.Find(owner, previous) && previous == phase) return;
  s_StoryPhases.Set(owner, phase);
  Print("[EventSchedulerZ] STORY owner=" + owner + " phase=" + phase);
 }

 static string ActiveStoryEvents()
 {
  string result = "";
  foreach (string owner, string phase : s_StoryPhases)
  {
   if (phase == "STORY_COMPLETE" || phase == "CLEANUP") continue;
   if (result != "") result = result + "\n";
   result = result + owner + " - " + phase;
  }
  return result;
 }

 static int Phase()
 {
  Initialize();
  if (s_State) return s_State.Phase;
  return DZES_Phase.CANDIDATE;
 }

 static bool IsOwner(string owner)
 {
  Initialize();
  if (!s_State) return false;
  return s_State.ProviderId == owner;
 }

 static bool HasActiveMajorEvent()
 {
  Initialize();
  if (!s_State) return false;
  return s_State.ProviderId != "";
 }

 static string NextQueuedOwner()
 {
  Initialize();
  if (!GetGame() || !GetGame().IsServer() || s_StorageFailed) return "";

  Sweep(GetGame().GetTime());
  if (s_State && s_State.NextEvent != "") return s_State.NextEvent;
  return SelectQueuedOwner();
 }

 static int QueueCount()
 {
  Initialize();
  if (!s_Queue) return 0;
  return s_Queue.Count();
 }

 static int CooldownRemainingSeconds()
 {
  Initialize();
  if (!GetGame()) return 0;

  int remaining = s_NextAllowedAt - GetGame().GetTime();
  if (remaining <= 0) return 0;
  return (remaining + 999) / 1000;
 }

 static int CleanupRemainingSeconds()
 {
  Initialize();
  if (!GetGame() || !s_State || s_State.Phase != DZES_Phase.CLEANUP || s_CleanupSince <= 0)
   return 0;

  int remaining = CLEANUP_GRACE_MS - (GetGame().GetTime() - s_CleanupSince);
  if (remaining <= 0) return 0;
  return (remaining + 999) / 1000;
 }

 static int NextEventStartInSeconds()
 {
  return CooldownRemainingSeconds();
 }

 static int NextEventAt()
 {
  Initialize();
  if (!s_State) return 0;
  return s_State.NextEventAt;
 }

 static DZES_Status GetStatus()
 {
  Initialize();
  DZES_Status status = new DZES_Status;
  if (!GetGame() || !s_State) return status;
  Sweep(GetGame().GetTime());
  status.ActiveEvent = s_State.ProviderId;
  status.ActiveEventId = s_State.EventId;
  status.ActivePhase = PhaseName(s_State.Phase);
  status.NextEvent = s_State.NextEvent;
 status.NextEventAt = s_State.NextEventAt;
  status.NextEventAtUTC = s_State.NextEventAtUTC;
  status.SecondsUntilNextEvent = NextEventStartInSeconds();
  status.QueueCount = QueueCount();
  status.CooldownRemaining = CooldownRemainingSeconds();
  status.RestartBlock = IsRestartBlocked();
 status.SecondsUntilRestart = SecondsUntilRestart();
  status.SelectionReason = SelectionReason(status.NextEvent);
  status.SelectionPriority = EffectivePriority(status.NextEvent);
  return status;
 }

 static bool IsRestartBlocked(int expectedDurationSeconds = 1800)
 {
  Initialize();
  if (!s_Settings) return true;
  return SecondsUntilRestart() <= Math.Max(s_Settings.RestartBlockSeconds, expectedDurationSeconds + s_Settings.EventSafetyReserveSeconds);
 }

 static string PhaseName(int phase)
 {
  switch (phase)
  {
   case DZES_Phase.CANDIDATE: return "CANDIDATE";
   case DZES_Phase.QUEUED: return "QUEUED";
   case DZES_Phase.SCHEDULED: return "SCHEDULED";
   case DZES_Phase.RESERVED: return "RESERVED";
   case DZES_Phase.STARTING: return "STARTING";
   case DZES_Phase.ACTIVE: return "ACTIVE";
   case DZES_Phase.COMPLETED: return "COMPLETED";
   case DZES_Phase.ABANDONED: return "ABANDONED";
   case DZES_Phase.TIMEOUT: return "TIMEOUT";
   case DZES_Phase.CLEANUP: return "CLEANUP";
   case DZES_Phase.CLEANUP_DONE: return "CLEANUP_DONE";
   case DZES_Phase.SLOT_RELEASE: return "SLOT_RELEASE";
  }
  return "UNKNOWN";
 }

 static bool SchedulerReadyForMajorEvent(int expectedDurationSeconds = 1800)
 {
  Initialize();
  if (!GetGame() || !GetGame().IsServer() || s_StorageFailed || !s_State)
   return false;

  int now = GetGame().GetTime();
  Sweep(now);

  if (s_State.ProviderId != "" || now < s_NextAllowedAt)
   return false;

  return SecondsUntilRestart() > Math.Max(s_Settings.RestartBlockSeconds, expectedDurationSeconds + s_Settings.EventSafetyReserveSeconds);
 }

 protected static string SelectQueuedOwner()
 {
  if (!s_Queue || s_Queue.Count() == 0)
   return "";

  string selected = "";
  int highest = -2147483647;

  // Queue array order supplies FIFO for equal priorities.
 foreach (string queued : s_Queue)
 {
   int p = EffectivePriority(queued);

   if (selected == "" || p > highest)
   {
    highest = p;
    selected = queued;
   }
  }

 return selected;
}

 protected static int BasePriority(string owner)
 {
  int priority = 0;
  if (owner != "") s_Priority.Find(owner, priority);
  return priority;
 }

 protected static int WaitBonus(string owner)
 {
  if (owner == "" || !GetGame()) return 0;
  int queuedSince = GetGame().GetTime();
  s_QueuedSince.Find(owner, queuedSince);
  int waitedMinutes = Math.Max(0, (GetGame().GetTime() - queuedSince) / 60000);
  return waitedMinutes * s_Settings.WaitingBonusPerMinute;
 }

 protected static int SkippedRounds(string owner)
 {
  if (!s_State) return 0;
  if (owner == "KOTH") return s_State.KothSkipped;
  if (owner == "COURIER") return s_State.CourierSkipped;
  if (owner == "RAVEN") return s_State.RavenSkipped;
  if (owner == "CONVOY") return s_State.ConvoySkipped;
  return 0;
 }

 protected static int StarvationBonus(string owner)
 {
  return Math.Min(SkippedRounds(owner), s_Settings.MaxSkippedRounds) * s_Settings.StarvationBonusPerSkippedRound;
 }

 protected static int PreferenceBonus(string owner)
 {
  if (owner == "KOTH") return s_Settings.KothPreferenceBonus;
  return 0;
 }

 protected static int StoryBonus(string owner)
 {
  if (BasePriority(owner) >= 100) return 100000;
  return 0;
 }

 protected static int RepeatPenalty(string owner)
 {
  if (!s_Settings.PreventImmediateRepeat || !s_State || owner != s_State.LastEvent) return 0;
  foreach (string other : s_Queue)
  {
   if (other != owner) return -100000;
  }
  return 0;
 }

 protected static int EffectivePriority(string owner)
  {
   if (owner == "" || !GetGame()) return 0;
   return BasePriority(owner) + StoryBonus(owner) + WaitBonus(owner) + StarvationBonus(owner) + PreferenceBonus(owner) + RepeatPenalty(owner);
  }

 protected static void UpdateSkippedRounds(string selected)
 {
  if (!s_State) return;
  if (selected == "KOTH") s_State.KothSkipped = 0;
  else if (s_Queue.Find("KOTH") >= 0) s_State.KothSkipped = Math.Min(s_State.KothSkipped + 1, s_Settings.MaxSkippedRounds);
  if (selected == "COURIER") s_State.CourierSkipped = 0;
  else if (s_Queue.Find("COURIER") >= 0) s_State.CourierSkipped = Math.Min(s_State.CourierSkipped + 1, s_Settings.MaxSkippedRounds);
  if (selected == "RAVEN") s_State.RavenSkipped = 0;
  else if (s_Queue.Find("RAVEN") >= 0) s_State.RavenSkipped = Math.Min(s_State.RavenSkipped + 1, s_Settings.MaxSkippedRounds);
  if (selected == "CONVOY") s_State.ConvoySkipped = 0;
  else if (s_Queue.Find("CONVOY") >= 0) s_State.ConvoySkipped = Math.Min(s_State.ConvoySkipped + 1, s_Settings.MaxSkippedRounds);
 }

 protected static void LogSelection(string selected, string reason)
 {
  foreach (string candidate : s_Queue)
  {
   Print("[EventSchedulerZ] [SCHEDULER] Candidate " + candidate + ": base=" + BasePriority(candidate).ToString() + " storyBonus=" + StoryBonus(candidate).ToString() + " waitBonus=" + WaitBonus(candidate).ToString() + " starvationBonus=" + StarvationBonus(candidate).ToString() + " kothBonus=" + PreferenceBonus(candidate).ToString() + " repeatPenalty=" + RepeatPenalty(candidate).ToString() + " effective=" + EffectivePriority(candidate).ToString());
  }
  Print("[EventSchedulerZ] [SCHEDULER] LAST_EVENT=" + s_State.LastEvent + " CURRENT_OWNER=" + s_State.ProviderId + " QUEUE_SIZE=" + s_Queue.Count().ToString());
  if (selected == s_State.LastEvent && s_Queue.Count() == 1)
   Print("[EventSchedulerZ] REPEAT_ALLOWED event=" + selected + " reason=no_other_startable_candidate");
  Print("[EventSchedulerZ] [SCHEDULER] SELECTED event=" + selected + " reason=" + reason);
 }

 protected static string SelectionReason(string owner)
 {
  string reason = "Standardrotation";
  if (owner != "") s_SelectionReason.Find(owner, reason);
  return reason;
 }

 protected static void UpdateNextEvent()
 {
  if (!s_State) return;
  // SCHEDULED is a freeze boundary. Ticks, status reads and queue refreshes
  // may observe the plan but must never replace or postpone it.
  if (s_State.NextEvent != "" && s_State.NextEventAt > 0) return;
  if (s_State.ProviderId != "") return;
  if (GetGame() && GetGame().GetTime() < s_PlanningOpenAt) return;
  string selected = SelectQueuedOwner();
  int now = GetGame().GetTime();
  if (selected != "")
  {
   s_State.NextEvent = selected;
   int gapSeconds = 0;
   if (s_State.NextEventAt <= 0)
   {
    if (s_StartupFastSchedule)
    {
     gapSeconds = Math.RandomIntInclusive(STARTUP_EVENT_MIN_SECONDS, STARTUP_EVENT_MAX_SECONDS);
     s_StartupFastSchedule = false;
    }
    else
     gapSeconds = Math.RandomIntInclusive(s_Settings.MinMajorEventGapSeconds, s_Settings.MaxMajorEventGapSeconds);
    s_State.NextEventAt = now + gapSeconds * 1000;
    s_State.NextEventAtUTCSeconds = ExpansionStatic.GetTimestamp(true) + gapSeconds;
   }
   else
   {
    gapSeconds = Math.Max(0, (s_State.NextEventAt - now + 999) / 1000);
   }
   s_State.Phase = DZES_Phase.SCHEDULED;
   int scheduledGeneration;
   s_ProviderGeneration.Find(selected, scheduledGeneration);
   s_State.ScheduledProviderGeneration = scheduledGeneration;
   LogSelection(selected, "highest_effective_priority");
   int year, month, day, hour, minute, second;
   GetYearMonthDayUTC(year, month, day);
   GetHourMinuteSecondUTC(hour, minute, second);
   s_State.NextEventAtUTC = string.Format("%1-%2-%3 %4:%5:%6 UTC + %7s", year, month, day, hour, minute, second, gapSeconds);
   if (s_NextAllowedAt < s_State.NextEventAt) s_NextAllowedAt = s_State.NextEventAt;
   Print("[EventSchedulerZ] NEXT_EVENT owner=" + selected + " phase=SCHEDULED active=" + s_State.ProviderId + " countdown=" + gapSeconds.ToString());
   Print("[EventSchedulerZ] NEXT_EVENT_AT owner=" + selected + " at=" + s_State.NextEventAt.ToString() + " utc=" + s_State.NextEventAtUTC + " countdown=" + gapSeconds.ToString());
   Print("[EventSchedulerZ] SCHEDULE owner=" + selected + " dueAt=" + s_State.NextEventAt.ToString() + " providerGeneration=" + scheduledGeneration.ToString());
  }
  Save();
 }

 static bool InvalidateSchedule(string reason)
 {
  Initialize();
  if (!s_State || s_State.ProviderId != "" || s_State.NextEvent == "" || reason == "") return false;
  Print("[EventSchedulerZ] SCHEDULE_INVALIDATED owner=" + s_State.NextEvent + " reason=" + reason);
  s_State.NextEvent = "";
  s_State.NextEventAt = 0;
  s_State.NextEventAtUTCSeconds = 0;
  s_State.NextEventAtUTC = "";
   s_State.Phase = DZES_Phase.CANDIDATE;
   s_PlanningOpenAt = GetGame().GetTime() + s_Settings.ProviderCollectionSeconds * 1000;
  if (!Save()) return false;
  UpdateNextEvent();
  return true;
 }

 protected static void Sweep(int now)
 {
  if (!s_State) return;

  if (s_State.ProviderId != "")
  {
   if (s_State.Phase == DZES_Phase.RESERVED && now - s_Heartbeat > GRANT_START_TIMEOUT_MS)
   {
    Print("[EventSchedulerZ] START_FAILED event=" + s_State.ProviderId + " run=" + s_State.EventId + " reason=grant_not_started");
    Print("[EventSchedulerZ] EVENT ABORTED event=" + s_State.ProviderId + " run=" + s_State.EventId + " reason=start_failed");
    ReleaseCurrent("START_FAILED", 0);
    return;
   }
   if (s_State.Phase != DZES_Phase.CLEANUP && now - s_Heartbeat > HEARTBEAT_TIMEOUT_MS)
   {
     s_State.Phase = DZES_Phase.TIMEOUT;
     Save();
     Print("[EventSchedulerZ] EVENT ABORTED event=" + s_State.ProviderId + " run=" + s_State.EventId + " reason=heartbeat_timeout");
     Print("[EventSchedulerZ] TIMEOUT " + s_State.EventId);
     s_State.Phase = DZES_Phase.CLEANUP;
    s_CleanupSince = now;
    Save();
    Print("[EventSchedulerZ] CLEANUP_REQUIRED " + s_State.EventId);
   }
   else if (s_State.Phase == DZES_Phase.CLEANUP)
   {
    if (s_CleanupSince == 0)
     s_CleanupSince = now;

    if (now - s_CleanupSince > CLEANUP_GRACE_MS)
    {
     Print("[EventSchedulerZ] CLEANUP_TIMEOUT " + s_State.EventId);
     ReleaseCurrent("CLEANUP_TIMEOUT");
     return;
    }
   }
  }

  for (int i = s_Queue.Count() - 1; i >= 0; i--)
  {
   string candidate = s_Queue[i];
   int lastRequest = 0;

   // Do not age waiting providers out while another event is active. Major
   // events routinely run longer than QUEUE_STALE_MS; removing their waiting
   // competitors here let frequently polling RAVEN monopolise the rotation.
   if (s_State.ProviderId == "" && candidate != s_State.NextEvent && (!s_LastRequest.Find(candidate, lastRequest) || now - lastRequest > QUEUE_STALE_MS))
   {
    s_Queue.Remove(i);
    s_LastRequest.Remove(candidate);
    s_Priority.Remove(candidate);
    s_QueuedSince.Remove(candidate);
    s_SelectionReason.Remove(candidate);
   }
  }

  // Resolve scheduled owners from the current runtime registry. A schedule
  // stores only the owner and timing metadata; it never owns a provider object.
  if (s_State.ProviderId == "" && s_State.NextEvent != "" && s_State.NextEventAt > 0 && now >= s_State.NextEventAt)
  {
   string staleOwner = s_State.NextEvent;
   int scheduledLastRequest = 0;
   int providerTimeout = s_Settings.RestoreTimeoutSeconds * 1000;
   int retryInterval = Math.Clamp(s_Settings.RestoreRetryIntervalSeconds, 5, 10) * 1000;
   bool providerFound = s_LastRequest.Find(staleOwner, scheduledLastRequest);
   bool providerReady = providerFound && now - scheduledLastRequest <= retryInterval * 2;
   int currentGeneration;
   s_ProviderGeneration.Find(staleOwner, currentGeneration);
   if (!providerReady && (s_LastRestoreRetryLog == 0 || now - s_LastRestoreRetryLog >= retryInterval))
   {
    Print("[EventSchedulerZ] RESOLVE_PROVIDER owner=" + staleOwner + " found=" + providerFound.ToString() + " ready=0 age=" + Math.Max(0, (now - scheduledLastRequest) / 1000).ToString() + " generation=" + currentGeneration.ToString());
    Print("[EventSchedulerZ] START_DEFERRED owner=" + staleOwner + " reason=provider_not_ready retryIn=" + (retryInterval / 1000).ToString());
    s_LastRestoreRetryLog = now;
   }
   if (!providerReady && now > s_State.NextEventAt + providerTimeout)
   {
    Print("[EventSchedulerZ] SCHEDULE_DEFERRED owner=" + staleOwner + " reason=provider_not_ready grace=" + (providerTimeout / 1000).ToString());
    if (!providerFound || now - scheduledLastRequest > providerTimeout)
    {
     s_Queue.RemoveItem(staleOwner);
     s_LastRequest.Remove(staleOwner);
     s_Priority.Remove(staleOwner);
     s_QueuedSince.Remove(staleOwner);
     s_SelectionReason.Remove(staleOwner);
    }
    s_State.NextEvent = "";
    s_State.NextEventAt = 0;
    s_State.NextEventAtUTCSeconds = 0;
    s_State.NextEventAtUTC = "";
    s_State.RecoveryPolicy = "";
    s_State.ScheduledProviderGeneration = 0;
    s_State.Phase = DZES_Phase.CANDIDATE;
    s_PlanningOpenAt = now + s_Settings.ProviderCollectionSeconds * 1000;
    Save();
   }
  }
  if (s_State.ProviderId == "" && s_Queue.Count() > 0 && s_State.NextEvent == "" && now - s_LastGrantAt > s_Settings.SchedulerWatchdogTimeoutSeconds * 1000)
  {
   Print("[EventSchedulerZ] WATCHDOG_RECOVERY reason=no_event_granted_for_" + s_Settings.SchedulerWatchdogTimeoutSeconds.ToString() + "_seconds queue=" + s_Queue.Count().ToString());
   s_PlanningOpenAt = now;
   s_LastGrantAt = now;
  }
  UpdateNextEvent();
 }
}

void DZES_FillWelcomeStatus(array<string> output)
{
 if (!output) return;
 output.Clear();
 DZES_Status status = DZES_Scheduler.GetStatus();
 string eventStatus = "EVENTSTATUS NICHT VERFUEGBAR";
 string eventNext = "";
 string storyStatus = "STORYEVENT: KEINES AKTIV";
 string activeStories = DZES_Scheduler.ActiveStoryEvents();
 if (activeStories != "") storyStatus = "STORYEVENT:\n" + activeStories;
 if (status)
 {
  if (status.RestartBlock)
  {
   eventStatus = "EVENTPAUSE";
   eventNext = "NAECHSTES EVENT NACH SERVERRESTART";
  }
  else
  {
   if (status.ActiveEvent != "") eventStatus = "AKTIV: " + status.ActiveEvent + "  |  STATUS: " + status.ActivePhase;
   else eventStatus = "AKTIV: KEIN MAJOR EVENT";
   if (status.NextEvent != "")
   {
    eventNext = "NAECHSTES: " + status.NextEvent;
    if (status.SecondsUntilNextEvent > 0) eventNext = eventNext + "  |  START IN: " + status.SecondsUntilNextEvent.ToString() + " SEK";
    if (status.SelectionReason != "") eventNext = eventNext + "\nGRUND: " + status.SelectionReason;
   }
   else eventNext = "KEIN MAJOR EVENT GEPLANT";
  }
 }
 output.Insert(eventStatus);
 output.Insert(eventNext);
 output.Insert(storyStatus);
}
