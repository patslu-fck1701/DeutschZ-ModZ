class DZWZ_Service
{
 static const string ROOT = "$profile:DeutschZ-System/deutschz_witnessz";
 static ref DZWZ_Service Instance;
 ref DZWZ_Settings m_Settings;
 protected ref DZWZ_Save m_Data;
 protected ref array<ref DZWZ_Pending> m_Pending = new array<ref DZWZ_Pending>;
 protected ref map<string, ref DZWZ_Memory> m_Index = new map<string, ref DZWZ_Memory>;
 protected int m_BudgetUTC;
 protected int m_Checks;
 protected int m_Cursor;
 protected int m_SaveUTC;

 static void Start()
 {
  if (!GetGame().IsServer() || Instance) return;
  DZWZ_Service service = new DZWZ_Service;
  if (!service.Initialize()) return;
  Instance = service;
  GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(service.Tick, 1000, true);
  Print("[WitnessZ] READY bounded visible witnesses; no player-name display.");
 }
 static void Stop()
 {
  if (!Instance) return;
  GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(Instance.Tick);
  Instance.Save();
  Instance.m_Pending.Clear();
  Instance = null;
 }
 bool Valid(DZWZ_Save candidate)
 {
  if (!candidate || candidate.schemaVersion != 1 || candidate.revision < 0 || !candidate.memories || candidate.memories.Count() > m_Settings.maxMemories) return false;
  ref map<string, bool> keys = new map<string, bool>;
  foreach (DZWZ_Memory memory: candidate.memories)
  {
   if (!memory || memory.subjectUID == "" || memory.subjectUID.Length() > 128 || memory.faction == "" || memory.faction.Length() > 128 || memory.expiresUTC <= 0) return false;
   string memoryKey = memory.subjectUID + ":" + memory.faction;
   if (keys.Contains(memoryKey)) return false;
   keys.Insert(memoryKey, true);
  }
  return true;
 }
 bool Initialize()
 {
  string loadError;
  if (FileExist(ROOT + "/config.json"))
  {
   bool loadedConfig = JsonFileLoader<DZWZ_Settings>.LoadFile(ROOT + "/config.json", m_Settings, loadError);
   if (!loadedConfig || !m_Settings || m_Settings.schemaVersion != 1) return false;
  }
  else
  {
   m_Settings = new DZWZ_Settings;
   MakeDirectory("$profile:DeutschZ-System");
   MakeDirectory(ROOT);
   bool savedConfig = JsonFileLoader<DZWZ_Settings>.SaveFile(ROOT + "/config.json", m_Settings, loadError);
   if (!savedConfig) return false;
  }
  m_Settings.Validate();
  m_Data = new DZWZ_Save;
  ref array<string> slots = {"state.a.json", "state.b.json"};
  bool existing;
  bool restored;
  foreach (string slot: slots)
  {
   if (!FileExist(ROOT + "/" + slot)) continue;
   existing = true;
   DZWZ_Save candidate;
   bool loadedSlot = JsonFileLoader<DZWZ_Save>.LoadFile(ROOT + "/" + slot, candidate, loadError);
   if (loadedSlot && candidate && candidate.schemaVersion != 1) return false;
   if (loadedSlot && Valid(candidate))
   {
    if (!restored || candidate.revision > m_Data.revision) m_Data = candidate;
    restored = true;
   }
  }
  if (existing && !restored)
  {
   Print("[WitnessZ] Disabled: unreadable state preserved.");
   return false;
  }
  int nowUTC = ExpansionStatic.GetTimestamp(true);
  for (int memoryIndex = m_Data.memories.Count() - 1; memoryIndex >= 0; memoryIndex--)
  {
   DZWZ_Memory remembered = m_Data.memories[memoryIndex];
   // Clock jumps backwards never grant a longer than configured memory.
   if (remembered.expiresUTC <= nowUTC || remembered.expiresUTC > nowUTC + m_Settings.memorySeconds) m_Data.memories.Remove(memoryIndex);
   else m_Index.Insert(remembered.subjectUID + ":" + remembered.faction, remembered);
  }
  m_SaveUTC = nowUTC;
  return true;
 }
 bool Save()
 {
  if (!m_Data) return false;
  string saveError;
  int nextRevision = m_Data.revision + 1;
  if (nextRevision <= 0) return false;
  string filename = ROOT + "/state.a.json";
  if (nextRevision % 2 == 0) filename = ROOT + "/state.b.json";
  int oldRevision = m_Data.revision;
  m_Data.revision = nextRevision;
  DZWZ_Save readback;
  bool savedState = JsonFileLoader<DZWZ_Save>.SaveFile(filename, m_Data, saveError);
  bool readState;
  if (savedState)
  {
   readState = JsonFileLoader<DZWZ_Save>.LoadFile(filename, readback, saveError);
  }
  if (!savedState || !readState || !Valid(readback) || readback.revision != nextRevision)
  {
   m_Data.revision = oldRevision;
   Print("[WitnessZ] State save failed.");
   return false;
  }
  m_SaveUTC = ExpansionStatic.GetTimestamp(true);
  return true;
 }
 static string FactionOf(eAIBase witness)
 {
  if (!witness || !witness.GetGroup() || !witness.GetGroup().GetFaction()) return "";
  return witness.GetGroup().GetFaction().GetName();
 }
 static bool ClearRay(eAIBase witness, PlayerBase subject)
 {
  if (!witness || !subject) return false;
  vector start = witness.GetPosition() + "0 1.5 0";
  vector end = subject.GetPosition() + "0 1.2 0";
  Object hitObject;
  vector hitPosition;
  if (!DayZPhysics.RayCastBullet(start, end, 0xFFFFFFFF, witness, hitObject, hitPosition, null, null)) return true;
  return hitObject == subject;
 }
 bool Identifies(eAIBase witness, PlayerBase subject)
 {
  if (!witness || !witness.IsAlive() || witness.IsUnconscious() || !subject || !subject.GetIdentity()) return false;
  // A covered face never supplies an identity to this conservative test implementation.
  if (subject.FindAttachmentBySlotName("Mask") || subject.FindAttachmentBySlotName("Headgear")) return false;
  float distance = vector.Distance(witness.GetPosition(), subject.GetPosition());
  if (distance > m_Settings.identificationRange) return false;
  vector direction = vector.Direction(witness.GetPosition(), subject.GetPosition()).Normalized();
  if (vector.Dot(witness.Expansion_GetAimDirection(), direction) < m_Settings.minimumFacingDot) return false;
  eAITargetInformation info = witness.eAI_GetTargetInformation();
  if (!info || info.GetEntity() != subject || !witness.eAI_HasLOS(info)) return false;
  return ClearRay(witness, subject);
 }
 void Observe(PlayerBase attacker, PlayerBase victim)
 {
  if (!m_Settings.enabled || !attacker || !victim || attacker == victim || attacker.IsAI() || !attacker.GetIdentity()) return;
  int nowUTC = ExpansionStatic.GetTimestamp(true);
  if (m_BudgetUTC != nowUTC) {m_BudgetUTC = nowUTC; m_Checks = 0;}
  array<eAIBase> allAI = eAIBase.eAI_GetAll();
  if (!allAI || allAI.Count() == 0) return;
  int visited;
  while (visited < allAI.Count() && m_Checks < m_Settings.maxChecksPerSecond && m_Pending.Count() < m_Settings.maxPending)
  {
   if (m_Cursor >= allAI.Count()) m_Cursor = 0;
   eAIBase witness = allAI[m_Cursor++];
   visited++;
   m_Checks++;
   if (witness == victim || !Identifies(witness, attacker)) continue;
   // Seeing the attacker alone is not evidence of seeing the actual hit.
   if (vector.Distance(witness.GetPosition(), victim.GetPosition()) > m_Settings.identificationRange || !ClearRay(witness, victim)) continue;
   string faction = FactionOf(witness);
   if (faction == "") continue;
   string uid = attacker.GetIdentity().GetId();
   bool duplicate;
   foreach (DZWZ_Pending pendingCheck: m_Pending)
   {
    if (pendingCheck.witness == witness && pendingCheck.subjectUID == uid && pendingCheck.faction == faction) duplicate = true;
   }
   if (duplicate) continue;
   DZWZ_Pending pending = new DZWZ_Pending;
   pending.witness = witness;
   pending.subjectUID = uid;
   pending.faction = faction;
   pending.dueUTC = nowUTC + m_Settings.reportDelaySeconds;
   m_Pending.Insert(pending);
  }
 }
 void Tick()
 {
  int nowUTC = ExpansionStatic.GetTimestamp(true);
  for (int expiredIndex = m_Data.memories.Count() - 1; expiredIndex >= 0; expiredIndex--)
  {
   DZWZ_Memory expired = m_Data.memories[expiredIndex];
   if (expired.expiresUTC <= nowUTC)
   {
    m_Index.Remove(expired.subjectUID + ":" + expired.faction);
    m_Data.memories.Remove(expiredIndex);
   }
  }
  for (int pendingIndex = m_Pending.Count() - 1; pendingIndex >= 0; pendingIndex--)
  {
   DZWZ_Pending pending = m_Pending[pendingIndex];
   if (!pending.witness || !pending.witness.IsAlive() || FactionOf(pending.witness) != pending.faction)
   {
    m_Pending.Remove(pendingIndex);
    continue;
   }
   if (nowUTC < pending.dueUTC) continue;
   if (pending.witness.IsUnconscious()) {m_Pending.Remove(pendingIndex);continue;}
   string memoryKey = pending.subjectUID + ":" + pending.faction;
   DZWZ_Memory memory;
   if (!m_Index.Find(memoryKey, memory))
   {
    if (m_Data.memories.Count() >= m_Settings.maxMemories) {m_Pending.Remove(pendingIndex);continue;}
    memory = new DZWZ_Memory;
    memory.subjectUID = pending.subjectUID;
    memory.faction = pending.faction;
    m_Data.memories.Insert(memory);
    m_Index.Insert(memoryKey, memory);
   }
   memory.expiresUTC = nowUTC + m_Settings.memorySeconds;
   m_Pending.Remove(pendingIndex);
  }
  if (nowUTC - m_SaveUTC >= 30) Save();
 }
 bool Remembers(eAIBase witness, PlayerBase subject)
 {
  if (!m_Settings.enabled || !subject || !subject.GetIdentity()) return false;
  DZWZ_Memory memory;
  if (!m_Index.Find(subject.GetIdentity().GetId() + ":" + FactionOf(witness), memory)) return false;
  int nowUTC = ExpansionStatic.GetTimestamp(true);
  if (memory.expiresUTC <= nowUTC) return false;
  if (m_BudgetUTC != nowUTC) {m_BudgetUTC = nowUTC; m_Checks = 0;}
  if (m_Checks >= m_Settings.maxChecksPerSecond) return false;
  m_Checks++;
  return Identifies(witness, subject);
 }
}
