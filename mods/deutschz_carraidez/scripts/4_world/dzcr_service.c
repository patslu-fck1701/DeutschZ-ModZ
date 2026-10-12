class DZCR_Service
{
 static const string ROOT = "$profile:DeutschZ-System/deutschz_carraidez";
 static ref DZCR_Service Instance;
 ref DZCR_Settings m_Settings;
 protected ref DZCR_Ledger m_Ledger;
 protected CarScript m_Vehicle;
 protected ref array<eAIBase> m_Guards = new array<eAIBase>;
 protected ref array<EntityAI> m_Owned = new array<EntityAI>;
 protected PlayerBase m_Capturer;
 protected string m_CapturerUID;
 protected int m_CaptureUTC;
 protected int m_DeadlineUTC;
 protected int m_CleanupUTC;
 protected bool m_Finished;

 static void Start()
 {
  if (!GetGame().IsServer() || Instance) return;
  DZCR_Service service = new DZCR_Service;
  if (!service.Initialize()) return;
  Instance = service;
  GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(service.Tick, 1000, true);
 }
 static void Stop()
 {
  if (!Instance) return;
  GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(Instance.Tick);
  Instance.Cleanup();
  Instance = null;
 }
 static bool Exists(string className)
 {
  return className != "" && GetGame().ConfigIsExisting("CfgVehicles " + className);
 }
 static bool ValidRunID(string runID)
 {
  if (runID == "" || runID.Length() > 64) return false;
  string allowed = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-";
  for (int character = 0; character < runID.Length(); character++)
  {
   if (!allowed.Contains(runID.Substring(character, 1))) return false;
  }
  return true;
 }
 bool ArchiveLedger(DZCR_Ledger ledger)
 {
  if (!ledger || !ValidRunID(ledger.runID)) return false;
  MakeDirectory(ROOT + "/runs");
  string archiveError;
  DZCR_Ledger archiveReadback;
  string archivePath = ROOT + "/runs/" + ledger.runID + ".json";
  if (FileExist(archivePath))
  {
   DZCR_Ledger previousArchive;
   bool previousLoaded = JsonFileLoader<DZCR_Ledger>.LoadFile(archivePath, previousArchive, archiveError);
   if (!previousLoaded || !previousArchive || previousArchive.schemaVersion != 1 || previousArchive.runID != ledger.runID) return false;
   // A committed payout can never be downgraded by a later failed cleanup.
   if (previousArchive.rewardCommitted && !ledger.rewardCommitted) return false;
   if (previousArchive.phase == "ABORTED" && ledger.phase != "ABORTED") return false;
  }
  bool archiveSaved = JsonFileLoader<DZCR_Ledger>.SaveFile(archivePath, ledger, archiveError);
  if (!archiveSaved) return false;
  bool archiveLoaded = JsonFileLoader<DZCR_Ledger>.LoadFile(archivePath, archiveReadback, archiveError);
  return archiveLoaded && archiveReadback && archiveReadback.schemaVersion == 1 && archiveReadback.runID == ledger.runID && archiveReadback.phase == ledger.phase && archiveReadback.rewardCommitted == ledger.rewardCommitted;
 }
 bool Commit(string phase, bool rewardCommitted)
 {
  string saveError;
  DZCR_Ledger proposed = new DZCR_Ledger;
  proposed.runID = m_Settings.runID;
  proposed.phase = phase;
  proposed.rewardCommitted = rewardCommitted;
  DZCR_Ledger readback;
  // Per-run consumed history is durable before the compatibility current ledger.
  if (!ArchiveLedger(proposed)) return false;
  bool savedLedger = JsonFileLoader<DZCR_Ledger>.SaveFile(ROOT + "/ledger.json", proposed, saveError);
  if (!savedLedger) return false;
  bool loadedLedger = JsonFileLoader<DZCR_Ledger>.LoadFile(ROOT + "/ledger.json", readback, saveError);
  if (!loadedLedger) return false;
  if (!readback || readback.schemaVersion != 1 || readback.runID != proposed.runID || readback.phase != proposed.phase || readback.rewardCommitted != proposed.rewardCommitted) return false;
  m_Ledger = readback;
  return true;
 }
 bool Initialize()
 {
  string loadError;
  if (FileExist(ROOT + "/config.json"))
  {
   bool loadedConfig = JsonFileLoader<DZCR_Settings>.LoadFile(ROOT + "/config.json", m_Settings, loadError);
   if (!loadedConfig || !m_Settings || m_Settings.schemaVersion != 1) return false;
  }
  else
  {
   m_Settings = new DZCR_Settings;
   MakeDirectory("$profile:DeutschZ-System");
   MakeDirectory(ROOT);
   bool savedConfig = JsonFileLoader<DZCR_Settings>.SaveFile(ROOT + "/config.json", m_Settings, loadError);
   if (!savedConfig) return false;
  }
  m_Settings.Validate();
  if (m_Settings.position[0] < 0 || m_Settings.position[0] > 15360 || m_Settings.position[2] < 0 || m_Settings.position[2] > 15360) return false;
  if (!m_Settings.enabled || !ValidRunID(m_Settings.runID) || !Exists(m_Settings.vehicleClass) || !Exists(m_Settings.guardClass)) return false;
  if (!Exists("SeaChest") || !Exists("Jeans_Blue") || !Exists("Hoodie_Black") || !Exists("AthleticShoes_Black") || !GetGame().ConfigIsExisting("CfgWeapons AKM") || !GetGame().ConfigIsExisting("CfgMagazines Mag_AKM_30Rnd")) return false;
  foreach (string rewardClass: m_Settings.rewards)
  {
   if (!GetGame().ConfigIsExisting("CfgVehicles " + rewardClass) && !GetGame().ConfigIsExisting("CfgMagazines " + rewardClass)) return false;
  }
  if (FileExist(ROOT + "/runs/" + m_Settings.runID + ".json"))
  {
   Print("[CarRaidZ] RUN_CONSUMED run=" + m_Settings.runID + "; explicit new run required; history preserved.");
   return false;
  }
  if (FileExist(ROOT + "/ledger.json"))
  {
   bool restoredLedger = JsonFileLoader<DZCR_Ledger>.LoadFile(ROOT + "/ledger.json", m_Ledger, loadError);
   if (!restoredLedger || !m_Ledger || m_Ledger.schemaVersion != 1) return false;
   if (m_Ledger.runID == "" || (m_Ledger.phase != "RESERVED" && m_Ledger.phase != "ACTIVE" && m_Ledger.phase != "CLAIMED" && m_Ledger.phase != "ABORTED")) return false;
   if (m_Ledger.rewardCommitted != (m_Ledger.phase == "CLAIMED")) return false;
   // Explicit new runID is required after every attempt, including a server crash.
   if (m_Ledger.runID == m_Settings.runID)
   {
    Print("[CarRaidZ] Run already reserved; no respawn or duplicate payout. Change runID for a new operator test.");
    return false;
   }
  }
  if (m_Ledger && !FileExist(ROOT + "/runs/" + m_Ledger.runID + ".json") && !ArchiveLedger(m_Ledger)) return false;
  // Durable reservation BEFORE any entity exists: a crash cannot repeat this run.
  if (!Commit("RESERVED", false)) return false;
  vector position = m_Settings.position;
  position[1] = GetGame().SurfaceY(position[0], position[2]);
  Object spawned = GetGame().CreateObjectEx(m_Settings.vehicleClass, position, ECE_SETUP | ECE_CREATEPHYSICS | ECE_PLACE_ON_SURFACE);
  m_Vehicle = CarScript.Cast(spawned);
  if (!m_Vehicle)
  {
   if (spawned) GetGame().ObjectDelete(spawned);
   Commit("ABORTED", false);
   return false;
  }
  m_Vehicle.SetLifetime(m_Settings.timeoutSeconds + m_Settings.cleanupSeconds);
  m_Vehicle.SetLifetimeMax(m_Settings.timeoutSeconds + m_Settings.cleanupSeconds);
  m_Vehicle.GetInventory().LockInventory(LOCK_FROM_SCRIPT);
  m_Owned.Insert(m_Vehicle);
  m_Vehicle.DZCR_SetStage(1);
  m_DeadlineUTC = ExpansionStatic.GetTimestamp(true) + m_Settings.timeoutSeconds;
  Print("[CarRaidZ] READY proposed operator test; vehicle=" + m_Settings.vehicleClass + " guards=" + m_Settings.guardCount.ToString());
  return true;
 }
 bool Near(PlayerBase player)
 {
  return player && player.IsAlive() && !player.IsUnconscious() && player.GetIdentity() && m_Vehicle && vector.Distance(player.GetPosition(), m_Vehicle.GetPosition()) <= m_Settings.interactionRadius;
 }
 bool MajorBusy()
 {
  return m_Settings.yieldToMajorEvents && DZES_Scheduler.HasActiveMajorEvent();
 }
 void Use(PlayerBase player, CarScript target)
 {
  if (m_Finished || target != m_Vehicle || !Near(player) || MajorBusy()) return;
  if (m_Vehicle.DZCR_Stage() == 1)
  {
   if (!Commit("ACTIVE", false)) {Finish(false);return;}
   eAIGroup group = eAIGroup.CreateGroup(new eAIFactionEast());
   if (!group) {Finish(false);return;}
   group.AddWaypoint(m_Vehicle.GetPosition());
   for (int guardIndex = 0; guardIndex < m_Settings.guardCount; guardIndex++)
   {
    vector guardPosition = m_Vehicle.GetPosition() + Vector(5 + guardIndex * 2, 0, 5);
    guardPosition[1] = GetGame().SurfaceY(guardPosition[0], guardPosition[2]);
    Object guardObject = GetGame().CreateObjectEx(m_Settings.guardClass, guardPosition, ECE_SETUP | ECE_INITAI | ECE_PLACE_ON_SURFACE);
    eAIBase guard = eAIBase.Cast(guardObject);
    if (!guard)
    {
     if (guardObject) GetGame().ObjectDelete(guardObject);
     Finish(false);
     return;
    }
    guard.SetGroup(group);
    guard.eAI_SetAccuracy(0.03, 0.12);
    guard.GetInventory().CreateAttachment("Jeans_Blue");
    guard.GetInventory().CreateAttachment("Hoodie_Black");
    guard.GetInventory().CreateAttachment("AthleticShoes_Black");
    guard.GetInventory().CreateInInventory("AKM");
    guard.GetInventory().CreateInInventory("Mag_AKM_30Rnd");
    guard.SetLifetime(m_Settings.timeoutSeconds + m_Settings.cleanupSeconds);
    m_Guards.Insert(guard);
    m_Owned.Insert(guard);
   }
   m_Vehicle.DZCR_SetStage(2);
   return;
  }
  if (m_Vehicle.DZCR_Stage() != 3 || m_Capturer) return;
  m_Capturer = player;
  m_CapturerUID = player.GetIdentity().GetId();
  m_CaptureUTC = ExpansionStatic.GetTimestamp(true) + m_Settings.captureSeconds;
  m_Vehicle.DZCR_SetStage(4);
 }
 void Tick()
 {
  int nowUTC = ExpansionStatic.GetTimestamp(true);
  if (m_Finished)
  {
   if (nowUTC >= m_CleanupUTC) Cleanup();
   return;
  }
  if (!m_Vehicle || !m_Vehicle.IsAlive() || nowUTC >= m_DeadlineUTC || m_DeadlineUTC - nowUTC > m_Settings.timeoutSeconds || (m_Vehicle.DZCR_Stage() > 1 && MajorBusy())) {Finish(false);return;}
  if (m_Vehicle.DZCR_Stage() == 2)
  {
   bool guardAlive;
   foreach (eAIBase guard: m_Guards)
   {
    if (guard && guard.IsAlive()) guardAlive = true;
   }
   if (!guardAlive) m_Vehicle.DZCR_SetStage(3);
  }
  if (m_Vehicle.DZCR_Stage() != 4) return;
  if (!Near(m_Capturer) || m_Capturer.GetIdentity().GetId() != m_CapturerUID)
  {
   m_Capturer = null;
   m_CapturerUID = "";
   m_Vehicle.DZCR_SetStage(3);
   return;
  }
  if (nowUTC < m_CaptureUTC) return;
  // Persist before spawning loot. Failure loses the reward rather than duplicating it.
  if (!Commit("CLAIMED", true)) {Finish(false);return;}
  vector rewardPosition = m_Vehicle.GetPosition() + "2 0 0";
  rewardPosition[1] = GetGame().SurfaceY(rewardPosition[0], rewardPosition[2]);
  EntityAI chest = EntityAI.Cast(GetGame().CreateObjectEx("SeaChest", rewardPosition, ECE_SETUP | ECE_PLACE_ON_SURFACE));
  if (chest)
  {
   chest.SetLifetime(m_Settings.cleanupSeconds);
   chest.SetLifetimeMax(m_Settings.cleanupSeconds);
   m_Owned.Insert(chest);
   foreach (string rewardClass: m_Settings.rewards) chest.GetInventory().CreateInInventory(rewardClass);
  }
  Finish(true);
 }
 void Finish(bool completed)
 {
  if (m_Finished) return;
  if (!completed && (!m_Ledger || !m_Ledger.rewardCommitted)) Commit("ABORTED", false);
  m_Finished = true;
  m_Capturer = null;
  m_CapturerUID = "";
  if (m_Vehicle) m_Vehicle.DZCR_SetStage(0);
  foreach (eAIBase guard: m_Guards)
  {
   if (guard && guard.IsAlive()) GetGame().ObjectDelete(guard);
  }
  m_Guards.Clear();
  m_CleanupUTC = ExpansionStatic.GetTimestamp(true) + m_Settings.cleanupSeconds;
  Print("[CarRaidZ] Finished completed=" + completed.ToString() + "; cleanup scheduled.");
 }
 void Cleanup()
 {
  foreach (EntityAI entity: m_Owned)
  {
   if (entity) GetGame().ObjectDelete(entity);
  }
  m_Owned.Clear();
  m_Guards.Clear();
  m_Vehicle = null;
  m_Capturer = null;
  GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(Tick);
 }
}
