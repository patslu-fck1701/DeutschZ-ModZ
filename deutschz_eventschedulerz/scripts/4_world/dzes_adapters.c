class DZES_Progression
{
 static const string STORY_ROOT = "$profile:DeutschZ-System/deutschz_radiomissionz/event_completions";

 static bool Done(string step, string uid)
 {
  return uid != "" && FileExist(STORY_ROOT + "/" + step + "/" + uid + ".done");
 }

 static string NeededProvider(string uid)
 {
  if (!Done("koth", uid)) return "KOTH";
  if (!Done("convoy", uid)) return "CONVOY";
  if (!Done("combined", uid)) return "";
  if (!Done("toxic_started", uid)) return "";
  if (!Done("toxic", uid)) return "";
  if (!Done("atm", uid)) return "ATMRAID";
  if (!Done("courier", uid)) return "COURIER";
  if (!Done("battleground", uid)) return "";
  if (!Done("operation", uid)) return "";
  return "";
 }

 static int PriorityFor(string provider, out string reason)
 {
  reason = "Ausgewogene Standardrotation";
  if (!GetGame() || !GetGame().IsServer()) return 10;
  array<Man> players = new array<Man>;
  GetGame().GetPlayers(players);
  int demand = 0;
  foreach (Man man : players)
  {
   PlayerBase player = PlayerBase.Cast(man);
   if (!player || !player.GetIdentity()) continue;
   if (NeededProvider(player.GetIdentity().GetPlainId()) == provider) demand++;
  }
  if (demand <= 0) return 10;
  reason = demand.ToString() + " Online-Spieler benötigen " + provider + " als nächsten Storyschritt";
  return 100 + demand * 25;
 }

 static bool MarkCompleted(string step, PlayerBase player)
 {
  if (!GetGame() || !GetGame().IsServer() || step == "" || !player || !player.GetIdentity()) return false;
  string root = "$profile:DeutschZ-System/deutschz_radiomissionz";
  string completions = root + "/event_completions";
  string directory = completions + "/" + step;
  MakeDirectory("$profile:DeutschZ-System");
  MakeDirectory(root);
  MakeDirectory(completions);
  MakeDirectory(directory);
  FileHandle file = OpenFile(directory + "/" + player.GetIdentity().GetPlainId() + ".done", FileMode.WRITE);
  if (file == 0) return false;
  FPrintln(file, step);
  CloseFile(file);
  return true;
 }
}

class DZES_HardlineReward
{
 static bool Grant(PlayerBase player, string provider, int points)
 {
  if (!GetGame() || !GetGame().IsServer() || !player || !player.GetIdentity() || points <= 0) return false;
#ifdef EXPANSIONMODHARDLINE
  if (!GetExpansionSettings().GetHardline().UseReputation || player.Expansion_GetReputation() < 0) return false;
  int before = player.Expansion_GetReputation();
  if (!player.Expansion_SetReputation(before + points, true))
  {
   Print("[EventSchedulerZ] HARDLINE_FAILED provider=" + provider + " event=" + DZES_Scheduler.EventId() + " player=" + player.GetIdentity().GetPlainId());
   return false;
  }
  Print("[EventSchedulerZ] HARDLINE_GRANTED provider=" + provider + " event=" + DZES_Scheduler.EventId() + " player=" + player.GetIdentity().GetPlainId() + " points=" + points.ToString());
  return true;
#else
  return false;
#endif
 }
}

class DZES_ATMRaideZ_API
{
 static bool RequestStart(int expectedDurationSeconds = 1800)
 {
  string reason;
  int priority = DZES_Progression.PriorityFor("ATMRAID", reason);
  return DZES_ProviderAPI.RequestLease("ATMRAID", expectedDurationSeconds, priority, reason);
 }

 static bool Start(vector position = "0 0 0") { return DZES_ProviderAPI.Start("ATMRAID", position); }
 static bool Activate(vector position = "0 0 0") { return DZES_ProviderAPI.Activate("ATMRAID", position); }
 static bool Complete(vector position = "0 0 0") { return DZES_ProviderAPI.Complete("ATMRAID", position); }
 static bool BeginCleanup(vector position = "0 0 0") { return DZES_ProviderAPI.BeginCleanup("ATMRAID", position); }
 static bool FinishCleanup() { return DZES_ProviderAPI.FinishCleanup("ATMRAID"); }
 static void Heartbeat() { DZES_ProviderAPI.Heartbeat("ATMRAID"); }
 static void AddParticipant(PlayerBase player)
 {
  if (player && player.GetIdentity()) DZES_ProviderAPI.AddParticipant("ATMRAID", player.GetIdentity().GetPlainId());
 }
 static bool CommitCompletion(PlayerBase player)
 {
  if (!player || !player.GetIdentity()) return false;
  string uid = player.GetIdentity().GetPlainId();
  if (!DZES_ProviderAPI.CommitParticipantReward("ATMRAID", uid)) return false;
  return DZES_Progression.MarkCompleted("atm", player);
 }
}

#ifdef SERVER
modded class DZKOTH_EventInstance
{
 protected bool m_DZES_Owned;
 protected bool m_DZES_StartRetryScheduled;
 override bool Start()
 {
  if (m_DZES_Owned) return false;
  string reason;
  if (!DZES_Scheduler.TryAcquire("KOTH",2400,DZES_Progression.PriorityFor("KOTH",reason),reason))
  {
   DZES_QueueStartRetry();
   return false;
  }
  DZES_CancelStartRetry();
  m_DZES_Owned=true;
  DZES_Scheduler.Transition("KOTH",DZES_Phase.STARTING);
  if (!super.Start()) { CleanupEvent(false); return false; }
  DZES_Scheduler.Transition("KOTH",DZES_Phase.ACTIVE); return true;
 }
 protected void DZES_QueueStartRetry()
 {
  if (m_DZES_StartRetryScheduled || !GetGame()) return;
  m_DZES_StartRetryScheduled=true;
  GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DZES_RetryStart,10000,false);
 }
 protected void DZES_RetryStart()
 {
  m_DZES_StartRetryScheduled=false;
  if (!m_DZES_Owned) Start();
 }
 protected void DZES_CancelStartRetry()
 {
  if (GetGame()) GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(DZES_RetryStart);
  m_DZES_StartRetryScheduled=false;
 }
 override void Tick()
 {
  if(m_DZES_Owned) DZES_Scheduler.Renew("KOTH");
  super.Tick();
 }
 override protected void HandleBossDefeated()
 {
  super.HandleBossDefeated();
  foreach (PlayerBase player : m_PlayersInside)
  {
   if (!player || !player.GetIdentity()) continue;
   DZES_Scheduler.Participant("KOTH", player.GetIdentity().GetPlainId());
   if (DZES_Scheduler.CommitReward("KOTH", player.GetIdentity().GetPlainId()))
   {
    DZES_Progression.MarkCompleted("koth", player);
    DZES_HardlineReward.Grant(player, "KOTH", 300);
   }
  }
 }
 override void CleanupEvent(bool returnToReady = false)
 {
  DZES_CancelStartRetry();
  if(m_DZES_Owned) DZES_Scheduler.Transition("KOTH",DZES_Phase.CLEANUP);
  super.CleanupEvent(false);
  if(m_DZES_Owned) { DZES_Scheduler.FinishCleanup("KOTH"); m_DZES_Owned=false; }
  if(returnToReady) Start();
 }
}
modded class DZKOTHF_EventController
{
 protected bool m_DZES_FreeOwned;
 protected bool m_DZES_StartRetryScheduled;
 override bool StartEvent()
 {
  string reason;
  if(m_DZES_FreeOwned) return false;
  if(!DZES_Scheduler.TryAcquire("KOTH",2400,DZES_Progression.PriorityFor("KOTH",reason),reason))
  {
   DZES_QueueStartRetry();
   return false;
  }
  DZES_CancelStartRetry();
  m_DZES_FreeOwned=true; DZES_Scheduler.Transition("KOTH",DZES_Phase.STARTING);
  if(!super.StartEvent())
  {
   DZES_Scheduler.Transition("KOTH",DZES_Phase.CLEANUP); DZES_Scheduler.FinishCleanup("KOTH"); m_DZES_FreeOwned=false; return false;
  }
  DZES_Scheduler.Transition("KOTH",DZES_Phase.ACTIVE);
  GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DZES_Heartbeat,30000,true); return true;
 }
 protected void DZES_QueueStartRetry()
 {
  if(m_DZES_StartRetryScheduled || !GetGame()) return;
  m_DZES_StartRetryScheduled=true;
  GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DZES_RetryStart,10000,false);
 }
 protected void DZES_RetryStart()
 {
  m_DZES_StartRetryScheduled=false;
  if(!m_DZES_FreeOwned) StartEvent();
 }
 protected void DZES_CancelStartRetry()
 {
  if(GetGame()) GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(DZES_RetryStart);
  m_DZES_StartRetryScheduled=false;
 }
 protected void DZES_Heartbeat() { if(m_DZES_FreeOwned) DZES_Scheduler.Renew("KOTH"); }
 override bool CompleteCapture()
 {
  array<PlayerBase> capturePlayers = new array<PlayerBase>;
  GetAlivePlayersInRadius(capturePlayers);
  bool completed = super.CompleteCapture();
  if (!completed) return false;
  foreach (PlayerBase player : capturePlayers)
  {
   if (!player || !player.GetIdentity()) continue;
   DZES_Scheduler.Participant("KOTH", player.GetIdentity().GetPlainId());
   if (DZES_Scheduler.CommitReward("KOTH", player.GetIdentity().GetPlainId()))
   {
    DZES_Progression.MarkCompleted("koth", player);
    DZES_HardlineReward.Grant(player, "KOTH", 300);
   }
  }
  return true;
 }
 override bool FinishCleanup()
 {
  bool result=super.FinishCleanup();
  if(result && m_DZES_FreeOwned)
  {
   CleanupRewardCrate();
   DZES_Scheduler.Transition("KOTH",DZES_Phase.CLEANUP); DZES_Scheduler.FinishCleanup("KOTH"); m_DZES_FreeOwned=false;
   GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(DZES_Heartbeat);
  }
  return result;
 }
 override void ShutdownServer()
 {
  super.ShutdownServer();
  DZES_CancelStartRetry();
  GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(DZES_Heartbeat);
  if(m_DZES_FreeOwned) { DZES_Scheduler.Transition("KOTH",DZES_Phase.CLEANUP); DZES_Scheduler.FinishCleanup("KOTH"); m_DZES_FreeOwned=false; }
 }
}
modded class AIConvoyManager
{
 protected AIConvoyEventParams m_DZES_PendingParams;
 protected bool m_DZES_RetryScheduled;

 // The foreign manager's advance-warning timer is not an authority anymore.
 // It may prepare a real event, but only EventSchedulerZ may announce/start it.
 override void AnnouncePending()
 {
  StartEvent();
 }

 override void NotifyConvoyAdvance(AIConvoyEventParams params, int seconds)
 {
  // Intentionally silent. A denied TryAcquire must never promise players that
  // a convoy is about to arrive.
 }

 override protected bool LaunchEvent(AIConvoyEventParams params)
 {
  string reason;
  if(!params) return false;
  if(m_ActiveEvent || (g_deutschz_aiconvoyz_controller && g_deutschz_aiconvoyz_controller.DZES_HasEncounter()))
  {
   Print("[EventSchedulerZ] CONVOY_DUPLICATE_BLOCKED reason=active_run_exists");
   return false;
  }
  if(!DZES_Scheduler.TryAcquire("CONVOY",2400,DZES_Progression.PriorityFor("CONVOY",reason),reason))
  {
   DZES_QueueRetry(params);
   return false;
  }
  DZES_CancelRetry();
  DZES_Scheduler.Transition("CONVOY",DZES_Phase.STARTING);
  bool started=super.LaunchEvent(params);
  if(!started) { DZES_Scheduler.Transition("CONVOY",DZES_Phase.CLEANUP); DZES_Scheduler.FinishCleanup("CONVOY"); }
  else
  {
   DZES_Scheduler.Transition("CONVOY",DZES_Phase.ACTIVE);
   GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(DZES_Heartbeat);
   GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DZES_Heartbeat,30000,true);
  }
  return started;
 }
 protected void DZES_QueueRetry(AIConvoyEventParams params)
 {
  m_DZES_PendingParams=params;
  if(m_DZES_RetryScheduled) return;
  m_DZES_RetryScheduled=true;
  GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DZES_RetryQueuedLaunch,10000,false);
 }
 protected void DZES_RetryQueuedLaunch()
 {
  m_DZES_RetryScheduled=false;
  AIConvoyEventParams pending=m_DZES_PendingParams;
  if(!pending || m_ActiveEvent) { m_DZES_PendingParams=null; return; }
  LaunchEvent(pending);
 }
 protected void DZES_CancelRetry()
 {
  GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(DZES_RetryQueuedLaunch);
  m_DZES_RetryScheduled=false;
  m_DZES_PendingParams=null;
 }
 protected void DZES_Heartbeat()
 {
  if(m_ActiveEvent || (g_deutschz_aiconvoyz_controller && g_deutschz_aiconvoyz_controller.DZES_HasEncounter())) { DZES_Scheduler.Renew("CONVOY"); return; }
  if(g_deutschz_aiconvoyz_controller) g_deutschz_aiconvoyz_controller.CleanupEncounter();
  DZES_Scheduler.Transition("CONVOY",DZES_Phase.CLEANUP); DZES_Scheduler.FinishCleanup("CONVOY");
  GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(DZES_Heartbeat);
 }
 override void OnEventEnded(AIConvoyEvent ev)
 {
  super.OnEventEnded(ev);
  if(g_deutschz_aiconvoyz_controller && g_deutschz_aiconvoyz_controller.DZES_HasEncounter())
  {
   DZES_Scheduler.Renew("CONVOY");
   return;
  }
  DZES_Scheduler.Transition("CONVOY",DZES_Phase.COMPLETED);
  DZES_Scheduler.Transition("CONVOY",DZES_Phase.CLEANUP);
  DZES_Scheduler.FinishCleanup("CONVOY");
  DZES_CancelRetry();
  GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(DZES_Heartbeat);
 }
}
modded class deutschz_aiconvoyz_controller
{
 bool DZES_HasEncounter() { return m_state != deutschz_aiconvoyz_state.IDLE && m_state != deutschz_aiconvoyz_state.DECODER_RECOVERED; }
}
modded class DZCourierZ_Manager
{
 protected bool m_DZES_CourierOwned;
 override protected void BeginEvent()
 {
  string reason;
  if(!DZES_Scheduler.TryAcquire("COURIER",2400,DZES_Progression.PriorityFor("COURIER",reason),reason)) return;
  m_DZES_CourierOwned=true;
  DZES_Scheduler.Transition("COURIER",DZES_Phase.STARTING);
  super.BeginEvent();
  if(IsActiveState()) DZES_Scheduler.Transition("COURIER",DZES_Phase.ACTIVE);
  else { DZES_Scheduler.Transition("COURIER",DZES_Phase.CLEANUP); DZES_Scheduler.FinishCleanup("COURIER"); m_DZES_CourierOwned=false; }
 }
 override protected void Tick()
 {
  super.Tick();
  if(m_DZES_CourierOwned && IsActiveState()) DZES_Scheduler.Renew("COURIER");
 }
 override protected void EndEvent()
 {
  if(m_DZES_CourierOwned)
  {
   if(m_RewardCommitted && m_Carrier && m_Carrier.GetIdentity())
   {
    string uid=m_Carrier.GetIdentity().GetPlainId();
    DZES_Scheduler.Participant("COURIER",uid);
    if(DZES_Scheduler.CommitReward("COURIER",uid))
    {
     DZES_Progression.MarkCompleted("courier",m_Carrier);
     DZES_HardlineReward.Grant(m_Carrier,"COURIER",50);
    }
   }
   DZES_Scheduler.Transition("COURIER",DZES_Phase.COMPLETED);
  }
  super.EndEvent();
  if(m_DZES_CourierOwned)
  {
   DZES_Scheduler.Transition("COURIER",DZES_Phase.CLEANUP);
   DZES_Scheduler.FinishCleanup("COURIER");
   m_DZES_CourierOwned=false;
  }
 }
}
modded class DZToxicZController
{
 override bool ActivateSignalMarker(PlayerBase player,Object marker)
 {
  bool activated=super.ActivateSignalMarker(player,marker);
  if(activated)
  {
   DZES_Scheduler.StoryTransition("TOXICZ","STORY_TRIGGER");
   DZES_Scheduler.StoryTransition("TOXICZ","ACTIVE");
  }
  return activated;
 }
 override protected void Tick()
 {
  if(m_Save && m_Save.State!=DZToxicZState.WAITING && m_Save.State!=DZToxicZState.COOLDOWN)
   DZES_Scheduler.StoryTransition("TOXICZ","ACTIVE");
  super.Tick();
  if(m_Save && m_Save.State==DZToxicZState.COOLDOWN) DZES_Scheduler.StoryTransition("TOXICZ","STORY_COMPLETE");
 }
 override void Stop()
 {
  super.Stop();
  DZES_Scheduler.StoryTransition("TOXICZ","CLEANUP");
 }
}
modded class DZBGZ_EventManager
{
 override bool TryActivateReader(PlayerBase player,Object reader)
 {
  if(m_BattlegroundSpawned) return super.TryActivateReader(player,reader);
  if(!DZBGZ_PlayerUtils.IsValidPlayer(player) || !reader) return false;
  DZES_Scheduler.StoryTransition("BATTLEGROUND","STORY_TRIGGER");
  bool result=super.TryActivateReader(player,reader);
  if(result) DZES_Scheduler.StoryTransition("BATTLEGROUND","ACTIVE");
  else CleanupDynamic();
  return result;
 }
 override protected void Tick() { if(m_BattlegroundSpawned) DZES_Scheduler.StoryTransition("BATTLEGROUND","ACTIVE"); super.Tick(); }
 override void CleanupDynamic()
 {
  DZES_Scheduler.StoryTransition("BATTLEGROUND","CLEANUP");
  super.CleanupDynamic(); DZES_Scheduler.StoryTransition("BATTLEGROUND","STORY_COMPLETE");
 }
}
modded class DZODZ_Manager
{
 override bool CreateMasterReader(PlayerBase player)
 {
  if(!player || !player.GetIdentity()) return false;
  DZES_Scheduler.StoryTransition("OPERATION","STORY_TRIGGER");
  bool result=super.CreateMasterReader(player);
  if(result)
  {
   DZES_Scheduler.StoryTransition("OPERATION","ACTIVE");
  }
  else DZES_Scheduler.StoryTransition("OPERATION","CLEANUP");
  return result;
 }
}
#endif
