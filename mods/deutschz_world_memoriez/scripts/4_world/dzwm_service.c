class DZWM_Service
{
 static const string ROOT = "$profile:DeutschZ-System/deutschz_world_memoriez";
 static ref DZWM_Service s_Instance;
 ref DZWM_Settings config;
 ref DZWM_Save data;
 protected ref map<string, ref DZWM_Region> m_Regions = new map<string, ref DZWM_Region>;
 protected ref NoiseParams m_Noise;
 protected int m_Signals;
 protected int m_LastSave;
 protected int m_CheckCursor;
 protected int m_PlayerCursor;
 protected int m_Dropped;
 protected int m_DebugSignals;
 protected bool m_ReadOnly;
 protected bool m_Running;

 static int UTC()
 {
  int year, month, day, hour, minute, second;
  GetYearMonthDayUTC(year, month, day);
  GetHourMinuteSecondUTC(hour, minute, second);
  int total = 0;
  for (int y = 1970; y < year; y++)
  {
   total += 365;
   if (y % 4 == 0 && (y % 100 != 0 || y % 400 == 0)) total++;
  }
  ref array<int> months = {31,28,31,30,31,30,31,31,30,31,30,31};
  if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) months[1] = 29;
  for (int m = 1; m < month; m++) total += months[m - 1];
  return (total + day - 1) * 86400 + hour * 3600 + minute * 60 + second;
 }

 static void Start()
 {
  if (s_Instance || !GetGame() || !GetGame().IsServer()) return;
  s_Instance = new DZWM_Service;
  s_Instance.Init();
 }
 static void Stop()
 {
  if (!s_Instance) return;
  s_Instance.Shutdown();
  s_Instance = null;
 }
 static void Signal(vector position, string kind, string uniqueKey = "")
 {
  if (s_Instance) s_Instance.Record(position, kind, uniqueKey);
 }

 protected void Init()
 {
  MakeDirectory("$profile:DeutschZ-System");
  MakeDirectory(ROOT);
  config = new DZWM_Settings;
  string error;
  if (FileExist(ROOT + "/config.json"))
  {
   DZWM_Settings loaded;
   if (!JsonFileLoader<DZWM_Settings>.LoadFile(ROOT + "/config.json", loaded, error) || !loaded || loaded.schemaVersion != 1)
   {
    Print("[WorldMemoriez] CONFIG_INVALID disabled; original preserved " + error);
    return;
   }
   config = loaded;
  }
  else if (!JsonFileLoader<DZWM_Settings>.SaveFile(ROOT + "/config.json", config, error))
  {
   Print("[WorldMemoriez] CONFIG_WRITE_FAILED disabled " + error);
   return;
  }
  config.Validate();
  if (!config.enabled) { Print("[WorldMemoriez] DISABLED"); return; }
  Restore();
  SeedGrid();
  m_Noise = new NoiseParams;
  m_Noise.LoadFromPath("DZWM_AlarmNoise");
  DZES_Scheduler.WorldMemoryTransitions.Remove(OnEvent);
  DZES_Scheduler.WorldMemoryTransitions.Insert(OnEvent);
  DZWM_ExplosionBus.Events.Remove(OnExplosion);
  DZWM_ExplosionBus.Events.Insert(OnExplosion);
  GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(Tick);
  GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Tick, config.tickSeconds * 1000, true);
  m_LastSave = UTC();
  m_Running = true;
  Print("[WorldMemoriez] READY schema=1 regions=" + data.regions.Count().ToString() + " persistenceReadOnly=" + m_ReadOnly.ToString() + " spawns=0");
 }

 // Empty sectors are real grid cells with baseline values, not fake activity.
 protected void SeedGrid()
 {
  if (!config.preseedGrid) return;
  int minX = DZWM_Rules.Axis(config.worldMin, config.originX, config.regionSize);
  int minZ = DZWM_Rules.Axis(config.worldMin, config.originZ, config.regionSize);
  int maxX = DZWM_Rules.Axis(config.worldMax - 0.01, config.originX, config.regionSize);
  int maxZ = DZWM_Rules.Axis(config.worldMax - 0.01, config.originZ, config.regionSize);
  for (int cellX = minX; cellX <= maxX && data.regions.Count() < config.maxRegions; cellX++)
  {
   for (int cellZ = minZ; cellZ <= maxZ && data.regions.Count() < config.maxRegions; cellZ++)
   {
    float anchorX = DZWM_Rules.Clamp(config.originX + (cellX + 0.5) * config.regionSize, config.worldMin, config.worldMax - 0.01);
    float anchorZ = DZWM_Rules.Clamp(config.originZ + (cellZ + 0.5) * config.regionSize, config.worldMin, config.worldMax - 0.01);
    Region(Vector(anchorX, 0, anchorZ));
   }
  }
  Print("[WorldMemoriez] GRID sectors=" + ((maxX - minX + 1) * (maxZ - minZ + 1)).ToString() + " tracked=" + data.regions.Count().ToString() + " max=" + config.maxRegions.ToString());
 }

 protected bool Valid(DZWM_Save candidate)
 {
  if (!candidate || candidate.schemaVersion != 1 || candidate.revision < 0 || !candidate.regions || !candidate.seen) return false;
  if (candidate.regionSize != config.regionSize || candidate.originX != config.originX || candidate.originZ != config.originZ) return false;
  if (candidate.regions.Count() > config.maxRegions || candidate.seen.Count() > config.maxDedupe) return false;
  ref array<string> ids = new array<string>;
  foreach (DZWM_Region region : candidate.regions)
  {
   if (!region || !InBounds(region.anchor) || region.id != RegionId(region.anchor) || ids.Find(region.id) >= 0) return false;
   ids.Insert(region.id);
   if (!ValueValid(region.activity) || !ValueValid(region.noise) || !ValueValid(region.violence) || !ValueValid(region.pressure) || !ValueValid(region.supply) || !ValueValid(region.recovery)) return false;
   if (region.lastUTC < 0 || region.lastStimulusUTC < 0 || region.lastReactionUTC < 0 || region.presenceSamples < 0 || region.eventSamples < 0) return false;
  }
  foreach (DZWM_Seen entry : candidate.seen)
  {
   if (!entry || entry.key == "" || entry.key.Length() > 160 || entry.utc < 0) return false;
  }
  return true;
 }
 protected bool ValueValid(float value)
 {
  return value == value && value >= 0 && value <= 100;
 }
 protected void Restore()
 {
  DZWM_Save first;
  DZWM_Save second;
  string errA, errB;
  bool a = JsonFileLoader<DZWM_Save>.LoadFile(ROOT + "/state.a.json", first, errA) && Valid(first);
  bool b = JsonFileLoader<DZWM_Save>.LoadFile(ROOT + "/state.b.json", second, errB) && Valid(second);
  if (a) data = first;
  if (b && (!data || second.revision > data.revision)) data = second;
  if (!data)
  {
   data = new DZWM_Save;
   data.regionSize = config.regionSize;
   data.originX = config.originX;
   data.originZ = config.originZ;
   // Unknown schemas, changed grid and broken saves are never silently replaced.
   m_ReadOnly = FileExist(ROOT + "/state.a.json") || FileExist(ROOT + "/state.b.json");
   if (m_ReadOnly) Print("[WorldMemoriez] RESTORE_FAILED preserved saves; transient simulation only. Restore compatible backup or archive both slots.");
  }
  foreach (DZWM_Region savedRegion : data.regions)
  {
   savedRegion.tickUsed = 0;
   m_Regions.Set(savedRegion.id, savedRegion);
  }
  PruneSeen(UTC());
 }
 protected void Save()
 {
  if (!data || m_ReadOnly) return;
  int oldRevision = data.revision;
  if (oldRevision >= 2000000000) { m_ReadOnly = true; return; }
  data.revision++;
  string target = ROOT + "/state.a.json";
  if (data.revision % 2 == 0) target = ROOT + "/state.b.json";
  string error;
  DZWM_Save readback;
  if (!JsonFileLoader<DZWM_Save>.SaveFile(target, data, error) || !JsonFileLoader<DZWM_Save>.LoadFile(target, readback, error) || !Valid(readback) || readback.revision != data.revision)
  {
   data.revision = oldRevision;
   Print("[WorldMemoriez] SAVE_FAILED previous slot retained " + error);
   return;
  }
  if (config.debugLogging) Print("[WorldMemoriez] SAVED revision=" + data.revision.ToString() + " regions=" + data.regions.Count().ToString());
 }
 protected void Shutdown()
 {
  DZES_Scheduler.WorldMemoryTransitions.Remove(OnEvent);
  DZWM_ExplosionBus.Events.Remove(OnExplosion);
  if (GetGame()) GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(Tick);
  if (m_Running) Save();
  m_Running = false;
  Print("[WorldMemoriez] STOPPED listeners removed");
 }

 protected bool InBounds(vector position)
 {
  return position[0] >= config.worldMin && position[0] < config.worldMax && position[2] >= config.worldMin && position[2] < config.worldMax;
 }
 protected string RegionId(vector position)
 {
  int x = DZWM_Rules.Axis(position[0], config.originX, config.regionSize);
  int z = DZWM_Rules.Axis(position[2], config.originZ, config.regionSize);
  return x.ToString() + ":" + z.ToString();
 }
 protected DZWM_Region Region(vector position)
 {
  if (!InBounds(position)) return null;
  string id = RegionId(position);
  DZWM_Region region;
  if (m_Regions.Find(id, region)) return region;
  if (data.regions.Count() >= config.maxRegions) return null;
  region = new DZWM_Region;
  region.id = id;
  region.anchor = position;
  region.supply = config.supplyBaseline;
  region.lastUTC = UTC();
  data.regions.Insert(region);
  m_Regions.Set(id, region);
  return region;
 }
 protected void Recover(DZWM_Region region, int now)
 {
  int last = Math.Min(now, region.lastUTC);
  int quietEnd = Math.Min(now, region.lastStimulusUTC + config.quietSeconds);
  float elapsed = Math.Max(0, now - Math.Max(last, quietEnd));
  elapsed = Math.Min(elapsed, 604800);
  region.activity = DZWM_Rules.Decay(region.activity, 0, elapsed, config.halfLifeSeconds);
  region.noise = DZWM_Rules.Decay(region.noise, 0, elapsed, config.halfLifeSeconds);
  region.violence = DZWM_Rules.Decay(region.violence, 0, elapsed, config.halfLifeSeconds);
  region.pressure = DZWM_Rules.Decay(region.pressure, 0, elapsed, config.halfLifeSeconds);
  if (config.enableSupplyRecovery) region.supply = DZWM_Rules.Decay(region.supply, config.supplyBaseline, elapsed, config.halfLifeSeconds);
  region.recovery = DZWM_Rules.Recovery(region.noise, region.violence, region.pressure);
  region.lastUTC = now;
  if (region.lastStimulusUTC > now) region.lastStimulusUTC = now;
  if (region.lastReactionUTC > now) region.lastReactionUTC = now;
 }
 protected void PruneSeen(int now)
 {
  for (int i = data.seen.Count() - 1; i >= 0; i--)
  {
   if (data.seen[i].utc > now || now - data.seen[i].utc > config.dedupeSeconds) data.seen.Remove(i);
  }
 }
 protected bool Seen(string key)
 {
  foreach (DZWM_Seen item : data.seen) if (item.key == key) return true;
  return false;
 }
 protected void Remember(string key, int now)
 {
  if (key == "") return;
  // Never evict unexpired event ids; refuse new keyed signals at capacity.
  DZWM_Seen item = new DZWM_Seen;
  item.key = key;
  item.utc = now;
  data.seen.Insert(item);
 }
 protected bool KindEnabled(string kind)
 {
  if (kind == "presence") return true;
  if (kind == "combat") return config.enableCombatSensor;
  if (kind == "shot") return config.enableShotSensor;
  if (kind == "explosion") return config.enableExplosionSensor;
  if (kind == "infected") return config.enableInfectedKillSensor;
  if (kind == "playerkill") return config.enablePlayerKillSensor;
  if (kind == "event_active" || kind == "supply") return config.enableEventSensor;
  return false;
 }
 protected void Apply(DZWM_Region region, vector position, string kind, float scale, int now)
 {
  Recover(region, now);
  float activity, noise, violence, pressure, supply;
  if (kind == "presence") activity = config.presenceWeight;
  if (kind == "shot") noise = config.shotWeight;
  if (kind == "explosion") noise = config.explosionWeight;
  if (kind == "infected") { violence = config.infectedKillWeight; pressure = config.infectedKillWeight; }
  if (kind == "combat") violence = config.combatWeight;
  if (kind == "playerkill") violence = config.playerKillWeight;
  if (kind == "event_active") { activity = config.eventActivityWeight; noise = config.eventNoiseWeight; pressure = config.eventPressureWeight; }
  if (kind == "supply") supply = config.supplyWeight;
  float cost = Math.Max(activity, Math.Max(noise, Math.Max(violence, Math.Max(pressure, supply)))) * scale;
  float allowed = Math.Min(cost, Math.Max(0, config.perRegionTickCap - region.tickUsed));
  if (allowed <= 0 || cost <= 0) return;
  float factor = scale * allowed / cost;
  region.activity = DZWM_Rules.Add(region.activity, activity * factor, 100);
  region.noise = DZWM_Rules.Add(region.noise, noise * factor, 100);
  region.violence = DZWM_Rules.Add(region.violence, violence * factor, 100);
  region.pressure = DZWM_Rules.Add(region.pressure, pressure * factor, 100);
  region.supply = DZWM_Rules.Add(region.supply, supply * factor, 100);
  region.tickUsed += allowed;
  if (config.debugLogging && kind != "presence" && m_DebugSignals < 16)
  {
   m_DebugSignals++;
   Print("[WorldMemoriez] SENSOR kind=" + kind + " region=" + region.id + " applied=" + allowed.ToString() + " noise=" + region.noise.ToString() + " violence=" + region.violence.ToString() + " supply=" + region.supply.ToString());
  }
  region.lastStimulusUTC = now;
  region.recovery = DZWM_Rules.Recovery(region.noise, region.violence, region.pressure);
  if (kind != "presence") region.anchor = position;
  if (kind == "presence") region.presenceSamples = Math.Min(1000000000, region.presenceSamples + 1);
  if (kind == "event_active" || kind == "supply") region.eventSamples = Math.Min(1000000000, region.eventSamples + 1);
 }
 protected void Record(vector position, string kind, string key)
 {
  if (!m_Running || !GetGame() || !GetGame().IsServer() || !KindEnabled(kind) || !InBounds(position)) return;
  if (key.Length() > 160 || (key != "" && (Seen(key) || data.seen.Count() >= config.maxDedupe))) return;
  if (m_Signals >= config.maxSignalsPerTick) { m_Dropped++; return; }
  DZWM_Region region = Region(position);
  if (!region) { m_Dropped++; return; }
  int now = UTC();
  m_Signals++;
  Remember(key, now);
  Apply(region, position, kind, 1, now);
  if ((kind != "shot" && kind != "explosion") || config.noiseRadius <= 0) return;
  // At most 8 neighbouring cells. Distance to cell rectangle bounds the radius.
  int cx = DZWM_Rules.Axis(position[0], config.originX, config.regionSize);
  int cz = DZWM_Rules.Axis(position[2], config.originZ, config.regionSize);
  for (int dx = -1; dx <= 1; dx++)
  {
   for (int dz = -1; dz <= 1; dz++)
   {
    if (dx == 0 && dz == 0) continue;
    float minX = config.originX + (cx + dx) * config.regionSize;
    float minZ = config.originZ + (cz + dz) * config.regionSize;
    float nearX = DZWM_Rules.Clamp(position[0], minX, minX + config.regionSize - 0.01);
    float nearZ = DZWM_Rules.Clamp(position[2], minZ, minZ + config.regionSize - 0.01);
    vector neighbour = Vector(nearX, position[1], nearZ);
    if (vector.Distance(position, neighbour) > config.noiseRadius) continue;
    DZWM_Region adjacent = Region(neighbour);
    if (adjacent) Apply(adjacent, neighbour, kind, 0.25, now);
   }
  }
 }
 // Called only after EventSchedulerZ successfully persists a real transition.
 void OnExplosion(vector position)
 {
  Record(position, "explosion", "");
 }
 void OnEvent(string provider, string eventId, int phase, vector position)
 {
  if (provider != "KOTH" && provider != "COURIER" && provider != "RAVEN" && provider != "CONVOY") return;
  if (eventId == "" || position == vector.Zero) return;
  string key = eventId + ":" + phase.ToString();
  if (phase == DZES_Phase.ACTIVE) Record(position, "event_active", key);
  // Supply means access to a successfully completed supply event, NOT its loot quantity.
  if (phase == DZES_Phase.COMPLETED && (provider == "COURIER" || provider == "RAVEN")) Record(position, "supply", key);
 }
 protected void Tick()
 {
  if (!m_Running || !GetGame()) return;
  int now = UTC();
  m_Signals = 0;
  m_DebugSignals = 0;
  PruneSeen(now);
  foreach (DZWM_Region state : data.regions) { state.tickUsed = 0; Recover(state, now); }
  ref array<Man> players = new array<Man>;
  GetGame().GetPlayers(players);
  ref array<vector> occupied = new array<vector>;
  int amount = Math.Min(players.Count(), config.maxPlayersPerTick);
  for (int i = 0; i < amount; i++)
  {
   int index = (m_PlayerCursor + i) % players.Count();
   Man player = players[index];
   if (!player || !player.IsAlive()) continue;
   vector pos = player.GetPosition();
   occupied.Insert(pos);
   Record(pos, "presence", "");
  }
  if (players.Count() > 0) m_PlayerCursor = (m_PlayerCursor + amount) % players.Count();
  int reactions = 0;
  // Ignore quiet preseeded cells when scheduling reactions: a full grid must
  // not delay one active region by hundreds of seconds.
  array<DZWM_Region> candidates = new array<DZWM_Region>;
  foreach (DZWM_Region candidate : data.regions)
  {
   if (candidate.alarm || DZWM_Rules.Severity(candidate.noise, candidate.violence, candidate.pressure) > config.alarmResetThreshold) candidates.Insert(candidate);
  }
  int checks = Math.Min(config.maxReactionChecksPerTick, candidates.Count());
  for (int c = 0; c < checks; c++)
  {
   int selected = (m_CheckCursor + c) % candidates.Count();
   DZWM_Region region = candidates[selected];
   float severity = DZWM_Rules.Severity(region.noise, region.violence, region.pressure);
   if (severity <= config.alarmResetThreshold) region.alarm = false;
   if (severity >= config.alarmThreshold) region.alarm = true;
   if (!config.enableAlarmReaction || !region.alarm || reactions >= config.maxReactionsPerTick || now - region.lastReactionUTC < config.reactionCooldownSeconds) continue;
   bool nearby = false;
   foreach (vector playerPosition : occupied)
   {
    if (vector.Distance(playerPosition, region.anchor) <= config.noiseRadius) { nearby = true; break; }
   }
   if (!nearby || !GetGame().GetNoiseSystem()) continue;
   float strength = config.reactionNoiseMultiplier * (1 - config.supplyAlarmReduction * region.supply / 100);
   GetGame().GetNoiseSystem().AddNoiseTarget(region.anchor, config.reactionLifetimeSeconds, m_Noise, strength);
   region.lastReactionUTC = now;
   reactions++;
   Print("[WorldMemoriez] REACTION region=" + region.id + " severity=" + severity.ToString() + " lifetime=" + config.reactionLifetimeSeconds.ToString() + " spawns=0");
  }
  if (candidates.Count() > 0) m_CheckCursor = (m_CheckCursor + checks) % candidates.Count();
  if (now - m_LastSave >= config.saveSeconds || now < m_LastSave)
  {
   if (config.debugLogging)
   {
    foreach (DZWM_Region diagnostic : data.regions)
     Print("[WorldMemoriez] REGION id=" + diagnostic.id + " activity=" + diagnostic.activity.ToString() + " noise=" + diagnostic.noise.ToString() + " violence=" + diagnostic.violence.ToString() + " pressure=" + diagnostic.pressure.ToString() + " supply=" + diagnostic.supply.ToString() + " recovery=" + diagnostic.recovery.ToString());
   }
   if (m_Dropped > 0) Print("[WorldMemoriez] RATE_LIMIT dropped=" + m_Dropped.ToString());
   m_Dropped = 0;
   Save();
   m_LastSave = now;
  }
 }
}

modded class Weapon_Base
{
 override void OnFire(int muzzle_index)
 {
  super.OnFire(muzzle_index);
  DZWM_Service.Signal(GetPosition(), "shot");
 }
}
modded class ZombieBase
{
 protected bool m_DZWM_Killed;
 override void EEKilled(Object killer)
 {
  super.EEKilled(killer);
  if (m_DZWM_Killed) return;
  m_DZWM_Killed = true;
  DZWM_Service.Signal(GetPosition(), "infected");
 }
}
modded class PlayerBase
{
 protected bool m_DZWM_Killed;
 override void EEKilled(Object killer)
 {
  super.EEKilled(killer);
  if (m_DZWM_Killed || !killer || killer == this) return;
  m_DZWM_Killed = true;
  EntityAI entity = EntityAI.Cast(killer);
  if (!entity) return;
  Man attacker = Man.Cast(entity.GetHierarchyRootPlayer());
  if (!attacker || attacker == this) return;
  DZWM_Service.Signal(GetPosition(), "playerkill");
 }
}
