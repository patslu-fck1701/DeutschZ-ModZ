class DZKOTHG_Manager
{
	protected static ref DZKOTHG_Manager s_Instance;
	protected ref DZKOTHG_Settings m_Settings;
	protected ref array<string> m_AllowedFoXyStarts;
	protected Object m_Terminal;
	protected PlayerBase m_CurrentHacker;
	protected string m_ActiveFoXyZoneName;
	protected int m_State;
	protected int m_CustomWaveStage;
	protected bool m_Initialized;
	protected ref array<Object> m_CustomWaveEnemies;

	static const string SETTINGS_PATH = "$profile:DeutschZ/KOTHGate/KOTHGateSettings.json";
	static const string MARKER_UID = "DeutschZ_KOTHTerminal_PreStart";

	void DZKOTHG_Manager()
	{
		m_State = DZKOTHG_State.IDLE;
		m_CustomWaveStage = 0;
		m_AllowedFoXyStarts = new array<string>;
		m_CustomWaveEnemies = new array<Object>;
	}

	static DZKOTHG_Manager GetInstance()
	{
		if (!s_Instance)
			s_Instance = new DZKOTHG_Manager();

		return s_Instance;
	}

	void Init()
	{
		if (m_Initialized)
		{
			Print("[DZKOTHG] Init skipped because manager is already initialized.");
			return;
		}

		Print("[DZKOTHG] Init begin.");
		LoadSettings();
		if (!m_Settings)
		{
			Print("[DZKOTHG][ERROR] Init aborted because settings are missing.");
			return;
		}

		ScriptInvoker invoker;
		invoker = FoXyKOTH_API.GetOnBeforeEventStartInvoker();
		if (invoker)
			invoker.Insert(OnBeforeFoXyEventStart);
		else
			Print("[DZKOTHG][WARN] Missing FoXy hook: OnBeforeEventStart.");

		invoker = FoXyKOTH_API.GetOnEventStartedInvoker();
		if (invoker)
			invoker.Insert(OnFoXyEventStarted);
		else
			Print("[DZKOTHG][WARN] Missing FoXy hook: OnEventStarted.");

		invoker = FoXyKOTH_API.GetOnEventCapturedInvoker();
		if (invoker)
			invoker.Insert(OnFoXyEventCaptured);
		else
			Print("[DZKOTHG][WARN] Missing FoXy hook: OnEventCaptured.");

		invoker = FoXyKOTH_API.GetOnEventFailedInvoker();
		if (invoker)
			invoker.Insert(OnFoXyEventFailed);
		else
			Print("[DZKOTHG][WARN] Missing FoXy hook: OnEventFailed.");

		invoker = FoXyKOTH_API.GetOnEventCleanupInvoker();
		if (invoker)
			invoker.Insert(OnFoXyEventCleanup);
		else
			Print("[DZKOTHG][WARN] Missing FoXy hook: OnEventCleanup.");

		invoker = FoXyKOTH_API.GetOnMilestoneReachedInvoker();
		if (invoker)
			invoker.Insert(OnFoXyMilestoneReached);
		else
			Print("[DZKOTHG][WARN] Missing FoXy hook: OnMilestoneReached.");

		invoker = FoXyKOTH_API.GetOnCaptureProgressChangedInvoker();
		if (invoker)
			invoker.Insert(OnFoXyCaptureProgressChanged);
		else
			Print("[DZKOTHG][WARN] Missing FoXy hook: OnCaptureProgressChanged.");

		m_State = DZKOTHG_State.IDLE;
		m_Initialized = true;
		Print("[DZKOTHG] Initialized. Gate enabled=" + m_Settings.EnableKOTHGate.ToString() + ", zones=" + m_Settings.Zones.Count().ToString());
	}

	float GetHackDurationSeconds()
	{
		if (!m_Settings)
			return 60.0;

		return m_Settings.HackDurationSeconds;
	}

	void LoadSettings()
	{
		if (!FileExist("$profile:DeutschZ"))
			MakeDirectory("$profile:DeutschZ");
		if (!FileExist("$profile:DeutschZ/KOTHGate"))
			MakeDirectory("$profile:DeutschZ/KOTHGate");

		m_Settings = new DZKOTHG_Settings();
		if (FileExist(SETTINGS_PATH))
		{
			string errorMessage;
			if (!JsonFileLoader<DZKOTHG_Settings>.LoadFile(SETTINGS_PATH, m_Settings, errorMessage))
			{
				Print("[DZKOTHG][ERROR] Settings invalid, writing defaults: " + errorMessage);
				m_Settings = new DZKOTHG_Settings();
				JsonFileLoader<DZKOTHG_Settings>.JsonSaveFile(SETTINGS_PATH, m_Settings);
			}

			if (!m_Settings.Zones)
				m_Settings.Zones = new array<ref DZKOTHG_ZoneSettings>;
			JsonFileLoader<DZKOTHG_Settings>.JsonSaveFile(SETTINGS_PATH, m_Settings);
			return;
		}

		JsonFileLoader<DZKOTHG_Settings>.JsonSaveFile(SETTINGS_PATH, m_Settings);
		Print("[DZKOTHG] Created default settings at " + SETTINGS_PATH);
	}

	void OnBeforeFoXyEventStart(string zoneName, FoXyKOTH_CancelToken token)
	{
		if (!token || !m_Settings || m_Settings.EnableKOTHGate != 1)
			return;

		DZKOTHG_ZoneSettings zone = GetZoneByFoXyName(zoneName);
		if (!zone)
			return;

		if (ConsumeAllowedStart(zoneName))
		{
			m_State = DZKOTHG_State.FOXY_RUNNING;
			CleanupTerminal();
			BroadcastMarker(zone, true);
			Print("[DZKOTHG] FoXy start allowed after terminal hack: " + zoneName);
			return;
		}

		token.Cancel = true;

		if (m_State != DZKOTHG_State.IDLE && m_ActiveFoXyZoneName != "")
		{
			Print("[DZKOTHG] Blocked FoXy start for " + zoneName + " because gate is busy with " + m_ActiveFoXyZoneName);
			return;
		}

		StartPreKOTHZone(zone);
	}

	protected void StartPreKOTHZone(DZKOTHG_ZoneSettings zone)
	{
		if (!zone)
			return;

		m_State = DZKOTHG_State.PRE_NOTIFY;
		m_ActiveFoXyZoneName = zone.GetFoXyZoneName();
		m_CurrentHacker = null;
		m_CustomWaveStage = 0;

		Print("[DZKOTHG] Starting pre-gate for " + m_ActiveFoXyZoneName);
		BroadcastNotify(m_Settings.PreNotifyMessage);

		if (!SpawnTerminalForZone(zone))
		{
			m_State = DZKOTHG_State.ABORTED;
			BroadcastNotify("KOTH Terminal: Terminal konnte nicht gespawnt werden.");
			ResetGate();
			return;
		}

		BroadcastMarker(zone, false);

		m_State = DZKOTHG_State.WAITING_FOR_TERMINAL;
		int timeoutMs = m_Settings.PreStartTimeoutMinutes * 60 * 1000;
		if (timeoutMs < 60000)
			timeoutMs = 60000;

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.AbortPreStart);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.AbortPreStart, timeoutMs, false);
	}

	protected bool SpawnTerminalForZone(DZKOTHG_ZoneSettings zone)
	{
		CleanupTerminal();

		vector pos = zone.GetTerminalPosition();
		m_Terminal = GetGame().CreateObjectEx(m_Settings.TerminalClassName, pos, ECE_PLACE_ON_SURFACE);
		if (!m_Terminal)
		{
			Print("[DZKOTHG] Failed to spawn terminal class " + m_Settings.TerminalClassName + " at " + pos.ToString());
			return false;
		}

		m_Terminal.SetPosition(pos);
		m_Terminal.SetOrientation(zone.GetTerminalOrientation());

		DZ_KOTHTerminal terminal = DZ_KOTHTerminal.Cast(m_Terminal);
		if (terminal)
			terminal.DZKOTHG_SetGateTerminal(true);
		else
			Print("[DZKOTHG] Terminal spawned but is not DZ_KOTHTerminal: " + m_Settings.TerminalClassName);

		Print("[DZKOTHG] Terminal spawned for " + zone.ZoneName + " at " + pos.ToString());
		return true;
	}

	bool CanHackTerminal(PlayerBase player, Object terminal)
	{
		if (!player || !terminal || !m_Settings)
			return false;

		if (!player.IsAlive() || player.IsUnconscious())
			return false;

		if (terminal != m_Terminal)
			return false;

		if (m_State != DZKOTHG_State.WAITING_FOR_TERMINAL && m_State != DZKOTHG_State.HACKING)
			return false;

		if (m_Settings.AllowOnlyOneHacker == 1 && m_State == DZKOTHG_State.HACKING && m_CurrentHacker && m_CurrentHacker != player)
			return false;

		float maxDistance = m_Settings.TerminalDistanceCheck;
		if (maxDistance < 1.0)
			maxDistance = 3.0;

		return vector.Distance(player.GetPosition(), terminal.GetPosition()) <= maxDistance;
	}

	void OnHackStarted(PlayerBase player, Object terminal)
	{
		if (!CanHackTerminal(player, terminal))
			return;

		m_State = DZKOTHG_State.HACKING;
		m_CurrentHacker = player;
		BroadcastNotify(m_Settings.HackStartedMessage);
		Print("[DZKOTHG] Hack started by " + GetPlayerDebugName(player) + " for " + m_ActiveFoXyZoneName);
	}

	void OnHackEnded(PlayerBase player, Object terminal)
	{
		if (m_State != DZKOTHG_State.HACKING)
			return;

		if (m_CurrentHacker && player == m_CurrentHacker)
		{
			m_CurrentHacker = null;
			m_State = DZKOTHG_State.WAITING_FOR_TERMINAL;
			Print("[DZKOTHG] Hack interrupted for " + m_ActiveFoXyZoneName);
		}
	}

	void CompleteTerminalHack(PlayerBase player, Object terminal)
	{
		if (!CanHackTerminal(player, terminal))
		{
			Print("[DZKOTHG] Hack completion rejected.");
			return;
		}

		m_State = DZKOTHG_State.ACTIVATED;
		m_CurrentHacker = null;
		BroadcastNotify(m_Settings.HackCompletedMessage);

		if (m_AllowedFoXyStarts.Find(m_ActiveFoXyZoneName) == -1)
			m_AllowedFoXyStarts.Insert(m_ActiveFoXyZoneName);

		bool started = FoXyKOTH_API.ForceStartEvent(m_ActiveFoXyZoneName);
		if (!started)
		{
			ConsumeAllowedStart(m_ActiveFoXyZoneName);
			BroadcastNotify(m_Settings.StartFailedMessage);
			Print("[DZKOTHG] ForceStartEvent failed for " + m_ActiveFoXyZoneName);
			m_State = DZKOTHG_State.ABORTED;
			ResetGate();
			return;
		}

		Print("[DZKOTHG] ForceStartEvent succeeded for " + m_ActiveFoXyZoneName);
	}

	void AbortPreStart()
	{
		if (m_State != DZKOTHG_State.WAITING_FOR_TERMINAL && m_State != DZKOTHG_State.HACKING)
			return;

		BroadcastNotify(m_Settings.PreStartExpiredMessage);
		Print("[DZKOTHG] PreStart expired for " + m_ActiveFoXyZoneName);
		m_State = DZKOTHG_State.ABORTED;
		ResetGate();
	}

	void OnFoXyEventStarted(string zoneName, FoXyKOTH_Event kothEvent)
	{
		if (zoneName != m_ActiveFoXyZoneName)
			return;

		m_State = DZKOTHG_State.FOXY_RUNNING;
		CleanupTerminal();

		DZKOTHG_ZoneSettings zone = GetZoneByFoXyName(zoneName);
		if (zone)
			BroadcastMarker(zone, true);

		Print("[DZKOTHG] FoXy event entered WAITING after gate: " + zoneName);

		SpawnCaptureWave(zoneName, 1);
	}

	void OnFoXyEventCaptured(string zoneName, FoXyKOTH_Event kothEvent, string playerNames)
	{
		if (zoneName == m_ActiveFoXyZoneName)
			m_State = DZKOTHG_State.FINISHED;
	}

	void OnFoXyEventFailed(string zoneName, FoXyKOTH_Event kothEvent, float captureProgress)
	{
		if (zoneName == m_ActiveFoXyZoneName)
			m_State = DZKOTHG_State.FINISHED;
	}

	void OnFoXyMilestoneReached(string zoneName, int milestone, float actualProgress)
	{
		if (zoneName != m_ActiveFoXyZoneName)
			return;

		ProcessCaptureProgress(zoneName, actualProgress);
	}

	void OnFoXyCaptureProgressChanged(string zoneName, float oldProgress, float newProgress, int playerCount)
	{
		if (zoneName != m_ActiveFoXyZoneName)
			return;

		ProcessCaptureProgress(zoneName, newProgress);
	}

	void OnFoXyEventCleanup(string zoneName)
	{
		if (zoneName != m_ActiveFoXyZoneName)
			return;

		Print("[DZKOTHG] FoXy cleanup received, resetting gate: " + zoneName);
		ResetGate();
	}

	protected void ResetGate()
	{
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.AbortPreStart);
		CleanupTerminal();
		CleanupCaptureWaves();

		DZKOTHG_ZoneSettings zone = GetZoneByFoXyName(m_ActiveFoXyZoneName);
		if (zone)
			BroadcastMarker(zone, true);

		m_ActiveFoXyZoneName = "";
		m_CurrentHacker = null;
		m_AllowedFoXyStarts.Clear();
		m_CustomWaveStage = 0;
		m_State = DZKOTHG_State.IDLE;
	}

	protected void ProcessCaptureProgress(string zoneName, float progress)
	{
		if (!m_Settings || m_Settings.EnableCaptureWaves != 1)
			return;

		if (progress >= 25.0)
			SpawnCaptureWave(zoneName, 2);
		if (progress >= 50.0)
			SpawnCaptureWave(zoneName, 3);
		if (progress >= 75.0)
			SpawnCaptureWave(zoneName, 4);
	}

	protected void SpawnCaptureWave(string zoneName, int stage)
	{
		if (!m_Settings || m_Settings.EnableCaptureWaves != 1)
			return;

		if (zoneName == "" || zoneName != m_ActiveFoXyZoneName)
			return;

		if (stage <= m_CustomWaveStage)
			return;

		DZKOTHG_ZoneSettings zone = GetZoneByFoXyName(zoneName);
		if (!zone)
			return;

		array<string> classes = new array<string>;
		int count = FillCaptureWaveClasses(stage, classes);
		if (count <= 0 || classes.Count() == 0)
			return;

		vector center = FoXyKOTH_API.GetEventPosition(zoneName);
		if (center[0] == 0.0 && center[1] == 0.0 && center[2] == 0.0)
			center = zone.GetFlagPosition();

		int spawned = 0;
		for (int i = 0; i < count; i++)
		{
			string className = classes.Get(Math.RandomInt(0, classes.Count()));
			vector spawnPos = GetCaptureWaveSpawnPosition(center);
			Object enemy = GetGame().CreateObjectEx(className, spawnPos, ECE_PLACE_ON_SURFACE);
			if (!enemy)
			{
				Print("[DZKOTHG] Capture wave spawn failed: " + className + " at " + spawnPos.ToString());
				continue;
			}

			enemy.SetOrientation(Vector(Math.RandomInt(0, 360), 0.0, 0.0));
			m_CustomWaveEnemies.Insert(enemy);
			spawned++;
		}

		m_CustomWaveStage = stage;
		BroadcastNotify(GetCaptureWaveNotification(stage));
		Print("[DZKOTHG] Capture wave " + stage.ToString() + " spawned " + spawned.ToString() + "/" + count.ToString() + " enemies for " + zoneName);
	}

	protected int FillCaptureWaveClasses(int stage, array<string> classes)
	{
		if (!classes)
			return 0;

		if (stage == 1)
		{
			classes.Insert("ZmbM_HunterOld_Autumn");
			classes.Insert("ZmbM_HunterOld_Spring");
			classes.Insert("ZmbM_HunterOld_Summer");
			classes.Insert("ZmbM_HunterOld_Winter");
			return 5;
		}

		if (stage == 2)
		{
			classes.Insert("ZmbM_PolicemanFat");
			classes.Insert("ZmbM_PolicemanSpecForce");
			classes.Insert("ZmbF_PoliceWomanNormal");
			return 4;
		}

		if (stage == 3)
		{
			classes.Insert("ZmbM_SoldierNormal");
			classes.Insert("ZmbM_PatrolNormal_PautRev");
			classes.Insert("ZmbM_PatrolNormal_Autumn");
			return 5;
		}

		if (stage == 4)
		{
			classes.Insert("ZmbM_Mummy");
			return 3;
		}

		return 0;
	}

	protected string GetCaptureWaveNotification(int stage)
	{
		if (stage == 1)
			return "KOTH Start: Jaeger-Zombies sind in der Zone.";
		if (stage == 2)
			return "KOTH 25 Prozent: Polizei-Zombies ruecken an.";
		if (stage == 3)
			return "KOTH 50 Prozent: Militaer-Zombies ruecken an.";
		if (stage == 4)
			return "KOTH 75 Prozent: Mumien betreten die Zone.";

		return "KOTH: Weitere Gegner betreten die Zone.";
	}

	protected vector GetCaptureWaveSpawnPosition(vector center)
	{
		vector pos = center;
		float radius = 45.0;
		float minRadius = 18.0;

		for (int i = 0; i < 20; i++)
		{
			pos = center;
			pos[0] = center[0] + Math.RandomFloatInclusive(-radius, radius);
			pos[2] = center[2] + Math.RandomFloatInclusive(-radius, radius);
			if (vector.Distance(pos, center) < minRadius)
				continue;

			pos[1] = GetGame().SurfaceY(pos[0], pos[2]);
			return pos;
		}

		pos = center;
		pos[0] = center[0] + minRadius;
		pos[1] = GetGame().SurfaceY(pos[0], pos[2]);
		return pos;
	}

	protected void CleanupCaptureWaves()
	{
		if (!m_CustomWaveEnemies)
			return;

		foreach (Object enemy: m_CustomWaveEnemies)
		{
			if (enemy)
				GetGame().ObjectDelete(enemy);
		}

		m_CustomWaveEnemies.Clear();
	}

	protected void CleanupTerminal()
	{
		if (!m_Terminal)
			return;

		DZ_KOTHTerminal terminal = DZ_KOTHTerminal.Cast(m_Terminal);
		if (terminal)
			terminal.DZKOTHG_SetGateTerminal(false);

		if (m_Settings && m_Settings.DeleteTerminalAfterHack == 1)
			GetGame().ObjectDelete(m_Terminal);

		m_Terminal = null;
	}

	protected void BroadcastNotify(string message)
	{
		if (!message)
			return;

		if (m_Settings && m_Settings.UseExpansionNotify == 1)
		{
#ifdef EXPANSIONMOD
			NotificationSystem.Create_Expansion("DeutschZ KOTH", message, "Trader", ARGB(255, 255, 196, 0), 10.0);
			Print("[DZKOTHG] Expansion notify: " + message);
			return;
#else
			Print("[DZKOTHG][WARN] Expansion notify requested but Expansion is not loaded. Using vanilla notify.");
#endif
		}

		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);

		foreach (Man man: players)
		{
			PlayerBase player = PlayerBase.Cast(man);
			if (!player)
				continue;

			PlayerIdentity identity = player.GetIdentity();
			if (!identity)
				continue;

			NotificationSystem.SendNotificationToPlayerIdentityExtended(identity, 10.0, "DeutschZ KOTH", message, "");
		}

		Print("[DZKOTHG] Notify: " + message);
	}

	protected void BroadcastMarker(DZKOTHG_ZoneSettings zone, bool remove)
	{
		if (!zone || !m_Settings)
			return;

		bool expansionHandled = false;
		if (m_Settings.UseExpansion3DMarker == 1)
			expansionHandled = BroadcastExpansionMarker(zone, remove);

		if ((expansionHandled && !remove) || m_Settings.UseVanillaMapMarker != 1)
			return;

		DZKOTHG_MarkerData marker = new DZKOTHG_MarkerData();
		marker.UID = MARKER_UID;
		marker.Text = "KOTH Terminal - " + zone.ZoneName;
		marker.Pos = zone.GetTerminalPosition();
		marker.Color = ARGB(255, 255, 196, 0);
		marker.IconPath = "dz/gear/navigation/data/map_transmitter_ca.paa";
		marker.Remove = remove;
		DZKOTHG_ServerRPC.BroadcastMarker(marker);
		Print("[DZKOTHG] Vanilla marker fallback used for " + zone.ZoneName);
	}

	protected bool BroadcastExpansionMarker(DZKOTHG_ZoneSettings zone, bool remove)
	{
#ifdef EXPANSIONMODNAVIGATION
		if (!zone)
			return false;

		if (!GetExpansionSettings() || !GetExpansionSettings().GetMap())
		{
			Print("[DZKOTHG][WARN] Expansion map settings are not available.");
			return false;
		}

		GetExpansionSettings().GetMap().RemoveServerMarker(MARKER_UID);
		if (remove)
		{
			Print("[DZKOTHG] Expansion 3D marker removed: " + MARKER_UID);
			return true;
		}

		ExpansionServerMarkerData expansionMarker = new ExpansionServerMarkerData(MARKER_UID);
		expansionMarker.Set3D(true);
		expansionMarker.SetName("KOTH Terminal - " + zone.ZoneName);
		expansionMarker.SetIconName("Trader");
		expansionMarker.SetPosition(zone.GetTerminalPosition());
		expansionMarker.SetColor(ARGB(255, 255, 196, 0));
		expansionMarker.SetVisibility(EXPANSION_MARKER_VIS_WORLD | EXPANSION_MARKER_VIS_MAP);

		bool added = GetExpansionSettings().GetMap().AddServerMarker(expansionMarker);
		if (added)
		{
			Print("[DZKOTHG] Expansion 3D marker created for " + zone.ZoneName);
			return true;
		}

		Print("[DZKOTHG][WARN] Expansion 3D marker could not be created for " + zone.ZoneName);
		return false;
#else
		Print("[DZKOTHG][WARN] Expansion 3D marker requested but Expansion navigation is not loaded.");
		return false;
#endif
	}

	protected DZKOTHG_ZoneSettings GetZoneByFoXyName(string zoneName)
	{
		if (!m_Settings || !m_Settings.Zones)
			return null;

		foreach (DZKOTHG_ZoneSettings zone: m_Settings.Zones)
		{
			if (zone && zone.GetFoXyZoneName() == zoneName)
				return zone;
		}

		return null;
	}

	protected bool ConsumeAllowedStart(string zoneName)
	{
		int idx = m_AllowedFoXyStarts.Find(zoneName);
		if (idx == -1)
			return false;

		m_AllowedFoXyStarts.Remove(idx);
		return true;
	}

	protected string GetPlayerDebugName(PlayerBase player)
	{
		if (!player || !player.GetIdentity())
			return "unknown";

		return player.GetIdentity().GetName();
	}
}
