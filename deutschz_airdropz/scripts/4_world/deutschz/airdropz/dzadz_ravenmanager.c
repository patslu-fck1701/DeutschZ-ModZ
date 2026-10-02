#ifdef SERVER
class DZADZ_RavenManager
{
 protected static ref DZADZ_RavenManager s_Instance;
 protected ref DZADZ_Settings m_Settings;
 protected ref ExpansionMissionEventAirdrop m_Event;
 protected ExpansionAirdropContainerBase_Server m_Container;
 protected int m_Phase;
 protected int m_NextStart;
 protected int m_AnnouncedAt;
 protected int m_Deadline;
 protected int m_FlightDeadline;
 protected bool m_Initialized;
 protected bool m_Landed;
 protected bool m_EnemiesSpawned;
 protected bool m_ResponseSpawned;
 protected bool m_Opened;
 protected bool m_Completed;
 protected int m_SecuredSeconds;
 protected int m_LandedAt;
 protected int m_Variant;
 protected int m_LastLocationIndex = -1;
 protected vector m_Target;
 protected vector m_SearchCenter;
 protected float m_SearchRadius;
 protected ref array<Object> m_Enemies = new array<Object>;
 protected ref map<string, int> m_Presence = new map<string, int>;
 protected ref array<string> m_Markers = new array<string>;
#ifdef EXPANSIONMODAI
 protected eAIGroup m_Group;
#endif
 static DZADZ_RavenManager Get()
 {
  if (!s_Instance)
   s_Instance = new DZADZ_RavenManager;
  return s_Instance;
 }
 void Init()
 {
  if (m_Initialized || !GetGame() || !GetGame().IsServer()) return;
  Print("[AirdropZ] RAVEN init requested.");
  m_Settings = new DZADZ_Settings;
  string root = "$profile:DeutschZ-System/deutschz_airdropz";
  MakeDirectory("$profile:DeutschZ-System"); MakeDirectory(root);
  string error;
  if (FileExist(root + "/settings.json"))
  {
   if (!JsonFileLoader<DZADZ_Settings>.LoadFile(root + "/settings.json", m_Settings, error) || !m_Settings || m_Settings.Version != 1)
   { Print("[AirdropZ] Invalid settings: " + error); return; }
  }
  else if (!JsonFileLoader<DZADZ_Settings>.SaveFile(root + "/settings.json", m_Settings, error)) return;
  m_Settings.ApplyPublished();
  m_Settings.SealBreakSeconds = Math.Max(45, m_Settings.SealBreakSeconds);
  if (!m_Settings.Enabled) { Print("[AirdropZ] INIT_DISABLED"); return; }
  if (!m_Settings.Locations || m_Settings.Locations.Count() == 0)
  {
   m_Settings.RestoreDefaultLocations();
   error = "";
   if (!m_Settings.Locations || m_Settings.Locations.Count() != 12 || !JsonFileLoader<DZADZ_Settings>.SaveFile(root + "/settings.json", m_Settings, error))
   {
    Print("[AirdropZ] INIT_BLOCKED reason=LOCATION_RECOVERY_FAILED " + error);
    return;
   }
  }
  Print("[AirdropZ] LOCATIONS_LOADED count=" + m_Settings.Locations.Count().ToString());
  m_Settings.AnnouncementSeconds = Math.Max(120, m_Settings.AnnouncementSeconds);
  m_Settings.LifetimeSeconds = Math.Clamp(m_Settings.LifetimeSeconds, 300, 1800);
  m_Settings.OpenedLifetimeSeconds = Math.Clamp(m_Settings.OpenedLifetimeSeconds, 60, 900);
  m_Settings.MinimumPlayers = Math.Max(1, m_Settings.MinimumPlayers);
  m_Settings.Reputation = Math.Clamp(m_Settings.Reputation, 0, 100);
  m_Settings.UnsealFailureChancePercent = Math.Clamp(m_Settings.UnsealFailureChancePercent, 0, 90);
  DZES_Scheduler.Initialize();
  m_Phase = DZADZ_RavenPhase.DZADZ_IDLE;
  m_NextStart = GetGame().GetTime();
  m_Initialized = true;
  GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Tick, 5000, true);
  Print("[AirdropZ] READY");
 }
 void Shutdown()
 {
  if (GetGame()) GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(Tick);
  Cleanup(); m_Initialized = false; s_Instance = null;
 }
 protected int PlayerCount()
 {
  array<Man> players = new array<Man>; GetGame().GetPlayers(players); int count;
  foreach (Man man : players) { PlayerBase p = PlayerBase.Cast(man); if (p && p.GetIdentity() && p.IsAlive()) count++; }
  return count;
 }
 protected void Tick()
 {
  if (!m_Initialized || !GetGame() || !GetGame().IsServer()) return;
  int now = GetGame().GetTime();
  if (m_Phase == DZADZ_RavenPhase.DZADZ_IDLE)
  {
   if (now < m_NextStart || PlayerCount() < m_Settings.MinimumPlayers) return;
   string selectionReason;
   int selectionPriority = DZES_Progression.PriorityFor("RAVEN", selectionReason);
   if (DZES_Scheduler.TryAcquire("RAVEN", m_Settings.AnnouncementSeconds + m_Settings.LifetimeSeconds + 180, selectionPriority, selectionReason)) BeginIntercept();
   return;
  }
  DZES_Scheduler.Renew("RAVEN");
  if (DZES_Scheduler.SecondsUntilRestart() <= 60 || now >= m_Deadline) { Cleanup(); return; }
  if (m_Phase == DZADZ_RavenPhase.DZADZ_INTERCEPT && now - m_AnnouncedAt >= 120000)
  { ShowSearchArea(); SetPhase(DZADZ_RavenPhase.DZADZ_TARGET_ZONE); }
  if (m_Phase == DZADZ_RavenPhase.DZADZ_TARGET_ZONE && now - m_AnnouncedAt >= (m_Settings.AnnouncementSeconds - 120) * 1000) LaunchDrop();
  if (m_Phase == DZADZ_RavenPhase.DZADZ_FLIGHT)
  {
   if (!m_Event || (!m_Event.m_Plane && !m_Event.m_Container) || now >= m_FlightDeadline) { Cleanup(); return; }
   if (m_Event.m_Container)
   {
    m_Container = ExpansionAirdropContainerBase_Server.Cast(m_Event.m_Container);
    if (!m_Container) { Cleanup(); return; }
    m_Container.DZADZ_SetSealSeconds(m_Settings.SealBreakSeconds);
    m_Deadline = now + m_Settings.LifetimeSeconds * 1000;
    DZES_Scheduler.Transition("RAVEN", DZES_Phase.ACTIVE, m_Container.GetPosition());
    SetPhase(DZADZ_RavenPhase.DZADZ_DROP_RELEASED);
   }
  }
  if (!m_Container) { if (m_Landed) Cleanup(); return; }
  if (!m_Landed && m_Container.Expansion_HasLanded())
  { m_Landed = true; m_LandedAt=now; m_Target = m_Container.GetPosition(); Print("[AirdropZ] CRATE_LANDED " + m_Target.ToString()); }
  if (!m_Landed || m_Completed) return;
  array<Man> players = new array<Man>; GetGame().GetPlayers(players);
  bool near; bool securing;
  foreach (Man man : players)
  {
   PlayerBase p = PlayerBase.Cast(man);
   if (!p || !p.GetIdentity() || !p.IsAlive() || p.IsUnconscious()) continue;
   float distance = vector.Distance(p.GetPosition(), m_Container.GetPosition());
   if (distance <= 300) near = true;
   if (distance <= 75)
   {
    securing=true;
    string uid = p.GetIdentity().GetPlainId(); int seconds = m_Presence.Get(uid) + 5; m_Presence.Set(uid, seconds);
    if (seconds >= 30) DZES_Scheduler.Participant("RAVEN", uid);
   }
  }
  if (near && !m_EnemiesSpawned) { m_EnemiesSpawned = true; SpawnInfected(); }
  if(near && !m_Opened && now-m_LandedAt >= Math.Max(60,m_Settings.RecoveryDelaySeconds)*1000) SpawnResponse();
  if(m_Opened && securing && AliveEnemies()==0) m_SecuredSeconds+=5; else m_SecuredSeconds=0;
  if(m_Opened && m_SecuredSeconds>=30) FinishSecured();
 }
 protected void BeginIntercept()
 {
  int locationIndex = Math.RandomInt(0, m_Settings.Locations.Count());
  if (m_Settings.Locations.Count() > 1 && locationIndex == m_LastLocationIndex)
   locationIndex = (locationIndex + 1) % m_Settings.Locations.Count();
  m_LastLocationIndex = locationIndex;
  m_Target = m_Settings.Locations[locationIndex];
  m_Target[1] = GetGame().SurfaceY(m_Target[0], m_Target[2]);
  if (GetGame().SurfaceIsSea(m_Target[0], m_Target[2]) || GetGame().SurfaceIsPond(m_Target[0], m_Target[2])) { Cleanup(); return; }
  m_SearchRadius = Math.RandomFloatInclusive(800,1200);
  m_SearchCenter = m_Target + Vector(Math.RandomFloatInclusive(-450,450),0,Math.RandomFloatInclusive(-450,450));
  m_SearchCenter[1] = GetGame().SurfaceY(m_SearchCenter[0],m_SearchCenter[2]);
  int roll = Math.RandomInt(0,100);
  if (roll < 35) m_Variant = 0; else if (roll < 65) m_Variant = 1; else if (roll < 82) m_Variant = 2; else if (roll < 92) m_Variant = 3; else m_Variant = 4;
  Print("[AirdropZ] RAVEN_VARIANT " + m_Variant.ToString());
  m_AnnouncedAt = GetGame().GetTime();
  m_Deadline = m_AnnouncedAt + (m_Settings.AnnouncementSeconds + m_Settings.LifetimeSeconds + 180) * 1000;
  m_Landed = false; m_Opened = false; m_Completed=false; m_SecuredSeconds=0; m_EnemiesSpawned = false; m_ResponseSpawned = false; m_Presence.Clear();
  if (!DZES_Scheduler.Transition("RAVEN", DZES_Phase.STARTING,m_Target)) { Cleanup(); return; }
  DZADZ_Radio.Broadcast("RAVEN", "RAVEN meldet einen Versorgungsabwurf.");
  SetPhase(DZADZ_RavenPhase.DZADZ_INTERCEPT);
 }
 protected void SetPhase(int phase)
 {
  m_Phase = phase; DZADZ_EventBus.Emit(phase,m_SearchCenter,m_SearchRadius);
  DZADZ_Radio.BroadcastPhase(phase,m_Variant);
  Print("[AirdropZ] PHASE " + phase.ToString() + " event=" + DZES_Scheduler.EventId());
 }
 protected void LaunchDrop()
 {
  SetPhase(DZADZ_RavenPhase.DZADZ_FLIGHT);
  if (!ExpansionMissionModule.s_Instance) { Cleanup(); return; }
  array<string> types = {"DZADZ_RavenSupply","DZADZ_RavenMedical","DZADZ_RavenEcho","DZADZ_RavenBlack","DZADZ_RavenQuarantine"};
  if (!GetGame().ConfigIsExisting("CfgVehicles " + types[m_Variant])) { Cleanup(); return; }
  m_Event = new ExpansionMissionEventAirdrop;
  m_Event.Enabled = false; m_Event.Weight = 1; m_Event.MissionName = "RAVEN Two-One"; m_Event.MissionMaxTime = m_Settings.LifetimeSeconds;
  m_Event.ShowNotification = false; m_Event.Height = 450; m_Event.DropZoneHeight = 200; m_Event.Speed = 120; m_Event.DropZoneSpeed = 85;
  m_Event.Container = types[m_Variant]; m_Event.FallSpeed = 4.5;
  m_Event.DropLocation = new ExpansionAirdropLocation(m_Target[0],m_Target[2],50,"RAVEN");
  m_Event.Infected = {"ZmbM_SoldierNormal"}; m_Event.InfectedCount = 0;
  m_Event.Loot = new array<ref ExpansionLoot>; m_Event.Loot.Insert(new ExpansionLoot("DZADZ_RavenManifest")); m_Event.ItemCount = 1;
  // Direct Start: this event is owned exclusively by the DeutschZ scheduler.
  m_Event.Start(); m_FlightDeadline = GetGame().GetTime() + 300000;
  if (!m_Event.m_Plane) { Print("[AirdropZ] AIRCRAFT_SPAWN FAILED"); Cleanup(); return; }
  Print("[AirdropZ] AIRCRAFT_SPAWN OK");
  DZADZ_Radio.Broadcast("RAVEN", "RAVEN ist unterwegs - Versorgungsabwurf bestaetigt.");
  Print("[AirdropZ] DROP_STARTED target=" + m_Target.ToString());
 }
 bool CanUnseal(ExpansionAirdropContainerBase_Server container)
 {
  return m_Initialized && m_Landed && !m_Opened && container && container == m_Container && container.DZADZ_IsSealed();
 }
 void BeginUnseal(PlayerBase player, ExpansionAirdropContainerBase_Server container)
 {
  if (!CanUnseal(container) || !player || !player.GetIdentity() || !player.IsAlive()) return;
  Print("[AirdropZ] UNSEAL_ATTEMPT player="+player.GetIdentity().GetPlainId()+" variant="+m_Variant.ToString()+" position="+container.GetPosition().ToString());
  if (m_Phase == DZADZ_RavenPhase.DZADZ_TRANSPONDER) return;
  RemoveMarkers(); CreateMarker("DZADZ_EXACT","RAVEN: Transponder aktiv",container.GetPosition(),true);
  SetPhase(DZADZ_RavenPhase.DZADZ_TRANSPONDER);
  SpawnResponse();
 }
 void ReportUnsealFailure(PlayerBase player, ExpansionAirdropContainerBase_Server container)
 {
  if (!container || container != m_Container || !container.DZADZ_IsSealed()) return;
  string uid="unknown";
  if (player && player.GetIdentity()) uid=player.GetIdentity().GetPlainId();
  Print("[AirdropZ] UNSEAL_FAILED player="+uid+" variant="+m_Variant.ToString()+" reason=interrupted_or_condition_failed");
  SpawnResponse();
 }
 void CompleteUnseal(PlayerBase player, ExpansionAirdropContainerBase_Server container)
 {
  if (!CanUnseal(container) || !player || !player.GetIdentity() || !player.IsAlive() || player.IsUnconscious()) return;
  if (vector.Distance(player.GetPosition(),container.GetPosition()) > 5) return;
  int failureRoll=Math.RandomInt(0,100);
  if(failureRoll<m_Settings.UnsealFailureChancePercent)
  {
   Print("[AirdropZ] UNSEAL_FAILED player="+player.GetIdentity().GetPlainId()+" variant="+m_Variant.ToString()+" reason=seal_jammed roll="+failureRoll.ToString()+" chance="+m_Settings.UnsealFailureChancePercent.ToString());
   DZADZ_Radio.Broadcast("RAVEN: VERSIEGELUNG BLOCKIERT","Der Oeffnungsversuch ist fehlgeschlagen. Die Versiegelung ist noch intakt; ein weiterer Versuch ist moeglich.");
   SpawnResponse();
   return;
  }
  string opener = player.GetIdentity().GetPlainId(); DZES_Scheduler.Participant("RAVEN",opener);
  m_Opened = true; container.DZADZ_SetSealed(false); PopulateLoot();
  m_Deadline = Math.Min(m_Deadline,GetGame().GetTime() + m_Settings.OpenedLifetimeSeconds * 1000);
  SetPhase(DZADZ_RavenPhase.DZADZ_OPENED);
 }
 protected int AliveEnemies()
 {
  int count = 0;
  foreach (Object enemy : m_Enemies)
  {
   if (enemy && enemy.IsAlive())
   {
    count++;
   }
  }
  return count;
 }
 protected void FinishSecured()
 {
  if(m_Completed || !m_Container || !m_Opened) return;
  if(!DZES_Scheduler.Transition("RAVEN",DZES_Phase.COMPLETED,m_Container.GetPosition())) return;
  m_Completed=true;
  Print("[AirdropZ] RAVEN_COMPLETED event=" + DZES_Scheduler.EventId());
  array<Man> players = new array<Man>; GetGame().GetPlayers(players);
  foreach (Man man : players)
  {
   PlayerBase participant = PlayerBase.Cast(man);
   if (!participant || !participant.GetIdentity() || !participant.IsAlive() || participant.IsUnconscious()) continue;
   string uid = participant.GetIdentity().GetPlainId();
   if (m_Presence.Get(uid) < 30 || vector.Distance(participant.GetPosition(),m_Container.GetPosition()) > 75) continue;
   if (!DZES_Scheduler.CommitReward("RAVEN",uid)) continue;
   RecordCompletion(participant);
  }
  DZADZ_Radio.Broadcast("RAVEN: Fracht gesichert","Der Bergungsbereich ist frei. Die anwesenden Teilnehmer haben den Auftrag abgeschlossen. Bewahre die Transportnummer auf: Sie passt nicht zum alten Freigabestempel.");
 }
 protected void RecordCompletion(PlayerBase player)
 {
  string uid = player.GetIdentity().GetPlainId();
  string root="$profile:DeutschZ-System/deutschz_airdropz/completions"; MakeDirectory(root);
  FileHandle handle=OpenFile(root+"/"+uid+"_"+DZES_Scheduler.EventId()+".done",FileMode.WRITE);
  if (handle == 0) { Print("[AirdropZ] Completion receipt failed " + uid); return; }
  FPrintln(handle,"schema=1;event="+DZES_Scheduler.EventId()+";variant="+m_Variant.ToString()); CloseFile(handle);
  DZADZ_EventBus.SI_Completed.Invoke(player,DZES_Scheduler.EventId(),m_Variant);
#ifdef EXPANSIONMODHARDLINE
  if (GetExpansionSettings().GetHardline().UseReputation && player.Expansion_GetReputation() >= 0)
   if (!player.Expansion_SetReputation(player.Expansion_GetReputation()+m_Settings.Reputation,true)) Print("[AirdropZ] Hardline save failed " + uid);
#endif
 }
 protected void PopulateLoot()
 {
  DZADZ_KothLootConfig lootConfig = DZADZ_KothLootConfig.Load();
  if (!lootConfig || !lootConfig.RewardCrateLoot)
  {
   Print("[AirdropZ] KOTH loot pool unavailable; Raven manifest remains untouched");
   return;
  }

  int created = 0;
  foreach (DZADZ_KothLootEntry entry : lootConfig.RewardCrateLoot)
  {
   if (!entry || !entry.Enabled || entry.ClassName == "") continue;
   if (Math.RandomFloatInclusive(0.0,100.0) > entry.Chance) continue;
   if (!GetGame().ConfigIsExisting("CfgVehicles "+entry.ClassName) && !GetGame().ConfigIsExisting("CfgWeapons "+entry.ClassName) && !GetGame().ConfigIsExisting("CfgMagazines "+entry.ClassName)) continue;
   int count = Math.RandomIntInclusive(Math.Max(1,entry.Min),Math.Max(Math.Max(1,entry.Min),entry.Max));
   for (int i=0;i<count;i++)
   {
    EntityAI item=m_Container.GetInventory().CreateInInventory(entry.ClassName);
    if (!item) { Print("[AirdropZ] Loot creation failed " + entry.ClassName); break; }
    foreach (string attachment : entry.Attachments) if (attachment!="" && GetGame().ConfigIsExisting("CfgVehicles "+attachment)) item.GetInventory().CreateAttachment(attachment);
    foreach (string extra : entry.ExtraItems) if (extra!="") m_Container.GetInventory().CreateInInventory(extra);
    created++;
   }
  }
  Print("[AirdropZ] KOTH reward pool applied items="+created.ToString()+" manifest=DZADZ_RavenManifest");
 }
 protected bool GroundPoint(vector center, float minRadius, float maxRadius, out vector result)
 {
  for (int i=0;i<15;i++)
  {
   float angle=Math.RandomFloatInclusive(0,Math.PI2); float radius=Math.RandomFloatInclusive(minRadius,maxRadius);
   result=center+Vector(Math.Sin(angle)*radius,0,Math.Cos(angle)*radius);
   if (GetGame().SurfaceIsSea(result[0],result[2]) || GetGame().SurfaceIsPond(result[0],result[2])) continue;
   result[1]=GetGame().SurfaceY(result[0],result[2]);
   if (Math.AbsFloat(GetGame().SurfaceY(result[0]+2,result[2])-result[1])>1.5) continue;
   result[1]=result[1]+0.2; return true;
  }
  return false;
 }
 protected void SpawnInfected()
 {
  for (int i=0;i<8;i++)
  {
   vector pos; if (!GroundPoint(m_Container.GetPosition(),20,50,pos)) continue;
   Object infected=GetGame().CreateObjectEx("ZmbM_SoldierNormal",pos,ECE_PLACE_ON_SURFACE|ECE_INITAI|ECE_NOPERSISTENCY_WORLD);
   if (infected) m_Enemies.Insert(infected);
  }
  Print("[AirdropZ] Infected spawned="+m_Enemies.Count().ToString());
 }
 protected void SpawnResponse()
 {
  if (m_ResponseSpawned || !m_Container) return;
#ifdef EXPANSIONMODAI
  m_Group=eAIGroup.CreateGroup(new eAIFactionEast());
  if (!m_Group)
  {
   Print("[AirdropZ] RECOVERY_TEAM_SPAWN_FAILED variant="+m_Variant.ToString()+" reason=group_create_failed");
   return;
  }
  m_ResponseSpawned=true;
  m_Group.AddWaypoint(m_Container.GetPosition());
  int count=4; if(m_Variant==3) count=6;
  int spawned=0;
  for(int i=0;i<count;i++)
  {
   vector pos; if(!GroundPoint(m_Container.GetPosition(),150,250,pos)) continue;
   eAIBase ai=eAIBase.Cast(GetGame().CreateObjectEx("eAI_SurvivorM_Boris",pos,ECE_SETUP|ECE_INITAI|ECE_PLACE_ON_SURFACE|ECE_NOPERSISTENCY_WORLD));
   if(!ai) continue;
   ai.SetGroup(m_Group);
   ai.GetInventory().CreateAttachment("M65Jacket_Olive"); ai.GetInventory().CreateAttachment("CargoPants_Green"); ai.GetInventory().CreateAttachment("CombatBoots_Black"); ai.GetInventory().CreateAttachment("PlateCarrierVest");
   array<string> weapons={"M4A1","AKM","FAL","SVD"};
   array<string> magazines={"Mag_STANAG_30Rnd","Mag_AKM_30Rnd","Mag_FAL_20Rnd","Mag_SVD_10Rnd"};
   int weaponIndex=Math.RandomInt(0,weapons.Count());
   EntityAI weapon=ai.GetHumanInventory().CreateInHands(weapons[weaponIndex]);
   if(weapon) weapon.GetInventory().CreateAttachment(magazines[weaponIndex]);
   ai.GetInventory().CreateInInventory(magazines[weaponIndex]);
   ai.GetInventory().CreateInInventory(magazines[weaponIndex]);
   m_Enemies.Insert(ai);
   spawned++;
  }
  Print("[AirdropZ] RECOVERY_TEAM_SPAWNED variant="+m_Variant.ToString()+" count="+spawned.ToString());
#else
  Print("[AirdropZ] RECOVERY_TEAM_SPAWN_FAILED variant="+m_Variant.ToString()+" reason=expansion_ai_unavailable");
#endif
 }
 protected void ShowSearchArea()
 {
  CreateMarker("DZADZ_SEARCH","RAVEN: rotes Suchgebiet ca. "+Math.Round(m_SearchRadius).ToString()+" m",m_SearchCenter,false);
 }
 protected void CreateMarker(string uid,string label,vector position,bool exact)
 {
  ExpansionMarkerModule module=ExpansionMarkerModule.GetModuleInstance(); if(!module) return;
  module.RemoveServerMarker(uid);
  ExpansionMarkerData marker=module.CreateServerMarker(label,"Airdrop",position,ARGB(255,210,25,25),exact,uid);
  if(!marker) return;
  if(exact && m_Settings.Exact3DMarker) marker.SetVisibility(EXPANSION_MARKER_VIS_WORLD|EXPANSION_MARKER_VIS_MAP); else marker.SetVisibility(EXPANSION_MARKER_VIS_MAP);
  m_Markers.Insert(uid);
 }
  protected void RemoveMarkers()
  {
   ExpansionMarkerModule module=ExpansionMarkerModule.GetModuleInstance();
   if(module)
   {
    foreach(string uid:m_Markers)
    {
     module.RemoveServerMarker(uid);
    }
   }
   m_Markers.Clear();
  }
  protected void Cleanup()
  {
   DZES_Scheduler.Transition("RAVEN",DZES_Phase.CLEANUP);
   RemoveMarkers();
   if(m_Event)
   {
    m_Event.End();
   }
   m_Event=null;
   m_Container=null;
#ifdef EXPANSIONMODAI
   if(m_Group) { m_Group.ClearAI(true); m_Group=null; }
#endif
   foreach(Object enemy:m_Enemies)
   {
    if(enemy)
    {
     GetGame().ObjectDelete(enemy);
    }
   }
   m_Enemies.Clear();
   m_Presence.Clear();
   DZADZ_EventBus.Emit(DZADZ_RavenPhase.DZADZ_CLEANUP,m_Target,0);
   DZES_Scheduler.FinishCleanup("RAVEN");
   m_Phase=DZADZ_RavenPhase.DZADZ_IDLE;
   m_Landed=false;
   if(GetGame())
   {
    m_NextStart=GetGame().GetTime()+30000;
   }
  }
}
#endif
