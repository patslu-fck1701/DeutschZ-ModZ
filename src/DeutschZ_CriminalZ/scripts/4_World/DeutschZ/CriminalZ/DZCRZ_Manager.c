class DZCRZ_Manager
{
	protected static ref DZCRZ_Manager s_Instance;
	protected bool m_Initialized;
	protected int m_SessionCounter;
	protected int m_SaveCounter;
	protected ref DZCRZ_Config m_Config;
	protected ref map<string, ref DZCRZ_HackSession> m_Sessions;
	protected ref DZCRZ_CooldownManager m_Cooldowns;
	protected ref DZCRZ_RewardManager m_Rewards;
	protected ref DZCRZ_MarkerManager m_Markers;
	protected ref DZCRZ_NotificationManager m_Notifications;
	protected ref DZCRZ_AlarmManager m_Alarms;
	protected ref DZCRZ_ATMManager m_ATMs;
	protected ref DZCRZ_GarageHackManager m_Garages;
	protected ref DZCRZ_VehicleHackManager m_Vehicles;
	protected ref DZCRZ_ExpansionStoreScanner m_StoreScanner;

	void DZCRZ_Manager()
	{
		m_Sessions = new map<string, ref DZCRZ_HackSession>;
	}

	static DZCRZ_Manager GetInstance()
	{
		if (!s_Instance)
			s_Instance = new DZCRZ_Manager;
		return s_Instance;
	}

	static void DestroyInstance()
	{
		if (s_Instance)
			s_Instance.Shutdown();
		s_Instance = null;
	}

	DZCRZ_Config GetConfig()
	{
		if (!m_Config && GetGame() && GetGame().IsServer())
			m_Config = DZCRZ_Config.Load();
		return m_Config;
	}

	void Init()
	{
		if (m_Initialized || !GetGame() || !GetGame().IsServer())
			return;
		DZCRZ_ProfilePaths.Ensure();
		m_Config = DZCRZ_Config.Load();
		m_Cooldowns = new DZCRZ_CooldownManager(m_Config.General.PersistCooldowns);
		m_Rewards = new DZCRZ_RewardManager(m_Config);
		m_Markers = new DZCRZ_MarkerManager;
		m_Notifications = new DZCRZ_NotificationManager;
		m_Alarms = new DZCRZ_AlarmManager;
		m_ATMs = new DZCRZ_ATMManager(m_Config);
		m_Garages = new DZCRZ_GarageHackManager;
		m_Vehicles = new DZCRZ_VehicleHackManager;
		m_StoreScanner = new DZCRZ_ExpansionStoreScanner(m_ATMs);
		DZCRZ_StorePlacementManager.Get().Init();
		m_ATMs.SpawnConfiguredATMs();
		m_Garages.CanHack(null);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Tick, DZCRZ_Const.SERVER_TICK_MS, true);
		if (m_Config.ATMModule.AutoCreateAtExpansionStores)
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(m_StoreScanner.Scan, DZCRZ_Const.STORE_SCAN_DELAY_MS, false);
		m_Initialized = true;
		WriteRuntime("running");
		DZCRZ_Log.Info("DeutschZ_CriminalZ " + DZCRZ_Const.VERSION + " initialisiert. ATM=vollstaendig, Fahrzeug=Expansion-API, Garage=API-Blocker.");
	}

	void Shutdown()
	{
		if (!m_Initialized)
			return;
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(Tick);
		CancelAll(DZCRZ_Const.CANCEL_SERVER_SHUTDOWN);
		if (m_Cooldowns)
			m_Cooldowns.Save();
		if (m_Markers)
			m_Markers.RemoveAll();
		if (m_Alarms)
			m_Alarms.StopAll();
		DZCRZ_StorePlacementManager.Destroy();
		WriteRuntime("clean_shutdown");
		m_Initialized = false;
		DZCRZ_Log.Info("CriminalZ sauber beendet.");
	}

	void RegisterATM(DZCRZ_ATM atm)
	{
		if (!m_Initialized)
			Init();
		if (m_ATMs)
			m_ATMs.RegisterATM(atm);
	}

	void RegisterExpansionATM(ExpansionATM_2 atm)
	{
		if (!m_Initialized)
			Init();
		if (m_ATMs)
			m_ATMs.RegisterExpansionATM(atm);
	}

	bool StartATMHack(PlayerBase player, Object atmObject)
	{
		if (!m_Initialized)
			Init();
		if (!m_Config || !m_Config.ATMModule.Enabled || !atmObject)
			return false;
		string targetId;
		float hackDuration;
		DZCRZ_ATM criminalATM = DZCRZ_ATM.Cast(atmObject);
		ExpansionATM_2 expansionATM = ExpansionATM_2.Cast(atmObject);
		if (criminalATM)
		{
			targetId = criminalATM.DZCRZ_GetATMId();
			hackDuration = criminalATM.DZCRZ_GetHackDuration();
		}
		else if (expansionATM)
		{
			targetId = expansionATM.DZCRZ_GetATMId();
			hackDuration = expansionATM.DZCRZ_GetHackDuration();
		}
		else
			return false;
		if (targetId == "")
			targetId = DZCRZ_PlayerUtils.SafeObjectId(atmObject);
		if (m_Cooldowns.IsActive(targetId))
		{
			m_Notifications.Personal(player, "CriminalZ", "Dieser ATM ist noch " + Math.Ceil(m_Cooldowns.GetRemaining(targetId) / 60.0).ToString() + " Minuten gesperrt.");
			DZCRZ_Log.Warn("ATM-Hack wegen Cooldown blockiert: " + targetId + " Spieler=" + DZCRZ_PlayerUtils.PlayerId(player));
			return false;
		}
		return StartHack(player, atmObject, targetId, DZCRZ_Const.MODULE_ATM, hackDuration, m_Config.ATMModule.RequiredRadiusMeters, m_Config.ATMModule.RequiredTool, m_Config.ATMModule.RequireToolInHands);
	}

	bool StartVehicleHack(PlayerBase player, Object vehicleObject)
	{
		if (!m_Initialized)
			Init();
		if (!m_Config || !m_Config.VehicleModule.Enabled || !vehicleObject || !m_Vehicles.IsLockedVehicle(vehicleObject))
			return false;
		CarScript car = CarScript.Cast(vehicleObject);
		if (!car)
			return false;
		return StartHack(player, vehicleObject, DZCRZ_PlayerUtils.SafeObjectId(vehicleObject), DZCRZ_Const.MODULE_VEHICLE, car.DZCRZ_GetHackDuration(), m_Config.VehicleModule.RequiredRadiusMeters, m_Config.VehicleModule.RequiredTool, m_Config.VehicleModule.RequireToolInHands);
	}

	protected bool StartHack(PlayerBase player, Object target, string targetId, int moduleType, float duration, float radius, string requiredTool, bool requireInHands)
	{
		if (!DZCRZ_PlayerUtils.IsValidPlayer(player) || !target || !DZCRZ_PlayerUtils.IsWithin(player, target, radius))
			return false;
		if (FindSession(player, target))
			return true;
		if (HasPlayerSession(DZCRZ_PlayerUtils.PlayerId(player)))
		{
			m_Notifications.Personal(player, "CriminalZ", "Du fuehrst bereits einen Hack aus.");
			return false;
		}
		if (m_Config.General.PreventParallelHackOnSameTarget && HasTargetSession(targetId))
		{
			m_Notifications.Personal(player, "CriminalZ", "Dieses Ziel wird bereits gehackt.");
			return false;
		}
		ItemBase tool = DZCRZ_PlayerUtils.FindTool(player, requiredTool, requireInHands);
		if (!tool || tool.IsRuined())
		{
			m_Notifications.Personal(player, "CriminalZ", "Benoetigtes Werkzeug fehlt oder ist ruiniert: " + requiredTool);
			return false;
		}
		m_SessionCounter++;
		DZCRZ_HackSession session = new DZCRZ_HackSession;
		session.SessionId = "DZCRZ_SESSION_" + m_SessionCounter.ToString();
		session.PlayerId = DZCRZ_PlayerUtils.PlayerId(player);
		session.TargetId = targetId;
		session.ModuleType = moduleType;
		session.DurationSeconds = Math.Clamp(duration, 10.0, 3600.0);
		session.RequiredRadius = radius;
		session.RequiredTool = requiredTool;
		session.RequireToolInHands = requireInHands;
		session.Player = player;
		session.Target = target;
		session.Tool = tool;
		session.Start();
		m_Sessions.Set(session.SessionId, session);
		StartPresentation(session);
		WriteRuntime("active");
		DZCRZ_Log.Info("Hack gestartet: session=" + session.SessionId + " type=" + moduleType.ToString() + " player=" + session.PlayerId + " target=" + targetId + " duration=" + session.DurationSeconds.ToString());
		return true;
	}

	void CompleteHack(PlayerBase player, Object target)
	{
		DZCRZ_HackSession session = FindSession(player, target);
		if (!session || session.State != DZCRZ_Const.STATE_ACTIVE)
		{
			DZCRZ_Log.Warn("Complete ohne gueltige Session blockiert. player=" + DZCRZ_PlayerUtils.PlayerId(player));
			return;
		}
		int cancelReason = session.Validate();
		if (cancelReason != DZCRZ_Const.CANCEL_NONE)
		{
			CancelSession(session, cancelReason);
			return;
		}
		if (session.GetElapsedSeconds() + 2.0 < session.DurationSeconds)
		{
			DZCRZ_Log.Warn("Vorzeitiger Complete-Versuch blockiert: session=" + session.SessionId + " elapsed=" + session.GetElapsedSeconds().ToString());
			CancelSession(session, DZCRZ_Const.CANCEL_ACTION_ENDED);
			return;
		}
		bool success;
		int payout;
		if (session.ModuleType == DZCRZ_Const.MODULE_ATM)
		{
			payout = m_Rewards.GrantATMReward(player, target.GetPosition());
			success = payout > 0;
			if (success)
				m_Cooldowns.Set(session.TargetId, m_Config.ATMModule.CooldownSeconds);
		}
		else if (session.ModuleType == DZCRZ_Const.MODULE_VEHICLE)
		{
			success = m_Vehicles.ForceUnlock(target);
		}
		else if (session.ModuleType == DZCRZ_Const.MODULE_GARAGE)
		{
			success = m_Garages.CompleteHack(target);
		}
		if (!success)
		{
			DZCRZ_Log.Error("Hack-Abschluss fehlgeschlagen: session=" + session.SessionId);
			CancelSession(session, DZCRZ_Const.CANCEL_TARGET_INVALID);
			return;
		}
		DamageToolOnSuccess(session);
		session.State = DZCRZ_Const.STATE_COMPLETED;
		string resultMessage = DZCRZ_Utils.ModuleLabel(session.ModuleType) + " erfolgreich abgeschlossen.";
		if (payout > 0)
			resultMessage = resultMessage + " Beute: " + payout.ToString() + " Euro.";
		m_Notifications.Personal(player, "CriminalZ", resultMessage);
		m_Notifications.Global("CriminalZ", "Die kriminelle Aktivitaet wurde abgeschlossen.", 8.0);
		DZCRZ_Log.Info("Hack abgeschlossen: session=" + session.SessionId + " payout=" + payout.ToString());
		CleanupSession(session);
	}

	void CancelHack(PlayerBase player, Object target, int reason)
	{
		DZCRZ_HackSession session = FindSession(player, target);
		if (session)
			CancelSession(session, reason);
	}

	void CancelPlayer(PlayerBase player, int reason)
	{
		if (!player)
			return;
		array<DZCRZ_HackSession> matches = new array<DZCRZ_HackSession>;
		foreach (string sessionId, DZCRZ_HackSession session: m_Sessions)
		{
			if (session && session.Player == player)
				matches.Insert(session);
		}
		foreach (DZCRZ_HackSession match: matches)
			CancelSession(match, reason);
	}

	void CancelAll(int reason = DZCRZ_Const.CANCEL_ADMIN)
	{
		array<DZCRZ_HackSession> sessions = new array<DZCRZ_HackSession>;
		foreach (string sessionId, DZCRZ_HackSession session: m_Sessions)
		{
			if (session)
				sessions.Insert(session);
		}
		foreach (DZCRZ_HackSession activeSession: sessions)
			CancelSession(activeSession, reason);
	}

	void Tick()
	{
		if (!m_Initialized)
			return;
		m_Cooldowns.Tick(1.0);
		array<DZCRZ_HackSession> invalidSessions = new array<DZCRZ_HackSession>;
		array<int> invalidReasons = new array<int>;
		foreach (string sessionId, DZCRZ_HackSession session: m_Sessions)
		{
			if (!session || session.State != DZCRZ_Const.STATE_ACTIVE)
				continue;
			int reason = session.Validate();
			if (reason != DZCRZ_Const.CANCEL_NONE)
			{
				invalidSessions.Insert(session);
				invalidReasons.Insert(reason);
				continue;
			}
			session.UpdateProgress();
			DZCRZ_ServerRPC.SendProgress(session.Player, true, session.Progress, session.DurationSeconds, DZCRZ_Utils.ModuleLabel(session.ModuleType));
		}
		for (int index = 0; index < invalidSessions.Count(); index++)
			CancelSession(invalidSessions[index], invalidReasons[index]);
		m_SaveCounter++;
		if (m_SaveCounter >= 60)
		{
			m_SaveCounter = 0;
			m_Cooldowns.Save();
			WriteRuntime("running");
		}
	}

	void HandleAdminCommand(PlayerIdentity sender, PlayerBase player, string command)
	{
		if (!sender || !player || !player.GetIdentity() || sender.GetPlainId() != player.GetIdentity().GetPlainId())
			return;
		if (!IsAdmin(sender.GetPlainId()))
		{
			DZCRZ_Log.Warn("Nicht autorisierter Admin-Befehl von " + sender.GetPlainId());
			return;
		}
		command.TrimInPlace();
		array<string> parts = new array<string>;
		command.Split(" ", parts);
		if (parts.Count() == 0)
			return;
		string verb = parts[0];
		verb.ToLower();
		if (verb == "status")
			m_Notifications.Personal(player, "CriminalZ Admin", "Sessions=" + m_Sessions.Count().ToString() + " ATMs=" + m_ATMs.GetCount().ToString() + " Debug=" + DZCRZ_RuntimeFlags.DebugEnabled.ToString());
		else if (verb == "reloadconfig")
		{
			ReloadConfig();
			m_Notifications.Personal(player, "CriminalZ Admin", "Config neu geladen.");
		}
		else if (verb == "cancelall")
		{
			CancelAll(DZCRZ_Const.CANCEL_ADMIN);
			m_Notifications.Personal(player, "CriminalZ Admin", "Alle Hacks abgebrochen.");
		}
		else if (verb == "listatms")
			m_Notifications.Personal(player, "CriminalZ Admin", "Registrierte ATMs: " + m_ATMs.GetCount().ToString());
		else if (verb == "spawnatm")
		{
			vector spawnPosition = player.GetPosition() + (player.GetDirection() * 2.0);
			spawnPosition[1] = GetGame().SurfaceY(spawnPosition[0], spawnPosition[2]);
			m_ATMs.SpawnAdminATM(spawnPosition, player.GetOrientation());
			m_Notifications.Personal(player, "CriminalZ Admin", "ATM erstellt und persistiert.");
		}
		else if (verb == "clearatmcd")
		{
			string nearestId = m_ATMs.FindNearestId(player.GetPosition(), 20.0);
			bool cleared = m_Cooldowns.Clear(nearestId);
			m_Notifications.Personal(player, "CriminalZ Admin", "ATM-Cooldown geloescht=" + cleared.ToString());
		}
		else if (verb == "debug" && parts.Count() > 1)
		{
			DZCRZ_RuntimeFlags.DebugEnabled = parts[1] == "on";
			m_Notifications.Personal(player, "CriminalZ Admin", "Debug=" + DZCRZ_RuntimeFlags.DebugEnabled.ToString());
		}
		else
			m_Notifications.Personal(player, "CriminalZ Admin", "Befehle: status reloadconfig cancelall listatms spawnatm clearatmcd debug on/off");
		DZCRZ_Log.Info("Admin-Befehl: uid=" + sender.GetPlainId() + " command=" + command);
	}

	protected bool IsAdmin(string uid)
	{
		if (!m_Config || !m_Config.General)
			return false;
		if (m_Config.General.AdminUIDs.Count() == 0)
			return m_Config.General.AllowAdminWhenListEmpty;
		return m_Config.General.AdminUIDs.Find(uid) >= 0;
	}

	protected void ReloadConfig()
	{
		m_Config = DZCRZ_Config.Load();
		m_Rewards = new DZCRZ_RewardManager(m_Config);
		m_ATMs.SetConfig(m_Config);
	}

	protected void StartPresentation(DZCRZ_HackSession session)
	{
		string label = DZCRZ_Utils.ModuleLabel(session.ModuleType);
		DZCRZ_ServerRPC.SendProgress(session.Player, true, 0.0, session.DurationSeconds, label);
		if (ShouldUseMarker(session.ModuleType))
			m_Markers.Create(session.SessionId, "CRIMINAL ACTIVITY", session.Target.GetPosition());
		string soundSet = DZCRZ_Const.ATM_SOUNDSET;
		if (session.ModuleType == DZCRZ_Const.MODULE_VEHICLE)
			soundSet = DZCRZ_Const.VEHICLE_SOUNDSET;
		m_Alarms.Start(session.SessionId, soundSet, session.Target.GetPosition(), true);
		m_Notifications.Global("CRIMINAL ACTIVITY", label + " wurde gemeldet. Position ist auf der Karte markiert.", 12.0);
	}

	protected bool ShouldUseMarker(int moduleType)
	{
		if (!m_Config.General.EnableExpansionMarkers)
			return false;
		if (moduleType == DZCRZ_Const.MODULE_ATM)
			return m_Config.ATMModule.EnableMapMarker;
		if (moduleType == DZCRZ_Const.MODULE_GARAGE)
			return m_Config.GarageModule.EnableMapMarker;
		if (moduleType == DZCRZ_Const.MODULE_VEHICLE)
			return m_Config.VehicleModule.EnableMapMarker;
		return false;
	}

	protected void CancelSession(DZCRZ_HackSession session, int reason)
	{
		if (!session || session.State != DZCRZ_Const.STATE_ACTIVE)
			return;
		session.State = DZCRZ_Const.STATE_CANCELLED;
		if (reason != DZCRZ_Const.CANCEL_SERVER_SHUTDOWN && reason != DZCRZ_Const.CANCEL_ADMIN)
			DamageToolOnFail(session);
		if (session.Player)
			m_Notifications.Personal(session.Player, "CriminalZ", "Hack abgebrochen. Grund=" + reason.ToString());
		DZCRZ_Log.Warn("Hack abgebrochen: session=" + session.SessionId + " reason=" + reason.ToString());
		CleanupSession(session);
	}

	protected void CleanupSession(DZCRZ_HackSession session)
	{
		m_Markers.Remove(session.SessionId);
		m_Alarms.Stop(session.SessionId);
		if (session.Player)
			DZCRZ_ServerRPC.SendProgress(session.Player, false, 0.0, session.DurationSeconds, "");
		m_Sessions.Remove(session.SessionId);
		WriteRuntime("running");
	}

	protected void DamageToolOnSuccess(DZCRZ_HackSession session)
	{
		if (session.ModuleType == DZCRZ_Const.MODULE_ATM && m_Config.ATMModule.DamageToolOnSuccess)
			DZCRZ_PlayerUtils.DamageTool(session.Tool, m_Config.ATMModule.ToolDamageOnSuccess);
		else if (session.ModuleType == DZCRZ_Const.MODULE_GARAGE && m_Config.GarageModule.DamageToolOnSuccess)
			DZCRZ_PlayerUtils.DamageTool(session.Tool, m_Config.GarageModule.ToolDamageOnSuccess);
		else if (session.ModuleType == DZCRZ_Const.MODULE_VEHICLE && m_Config.VehicleModule.DamageToolOnSuccess)
			DZCRZ_PlayerUtils.DamageTool(session.Tool, m_Config.VehicleModule.ToolDamageOnSuccess);
	}

	protected void DamageToolOnFail(DZCRZ_HackSession session)
	{
		if (session.ModuleType == DZCRZ_Const.MODULE_ATM && m_Config.ATMModule.DamageToolOnFail)
			DZCRZ_PlayerUtils.DamageTool(session.Tool, m_Config.ATMModule.ToolDamageOnFail);
		else if (session.ModuleType == DZCRZ_Const.MODULE_GARAGE && m_Config.GarageModule.DamageToolOnFail)
			DZCRZ_PlayerUtils.DamageTool(session.Tool, m_Config.GarageModule.ToolDamageOnFail);
		else if (session.ModuleType == DZCRZ_Const.MODULE_VEHICLE && m_Config.VehicleModule.DamageToolOnFail)
			DZCRZ_PlayerUtils.DamageTool(session.Tool, m_Config.VehicleModule.ToolDamageOnFail);
	}

	protected DZCRZ_HackSession FindSession(PlayerBase player, Object target)
	{
		foreach (string sessionId, DZCRZ_HackSession session: m_Sessions)
		{
			if (session && session.Player == player && session.Target == target)
				return session;
		}
		return null;
	}

	protected bool HasPlayerSession(string playerId)
	{
		foreach (string sessionId, DZCRZ_HackSession session: m_Sessions)
		{
			if (session && session.PlayerId == playerId && session.State == DZCRZ_Const.STATE_ACTIVE)
				return true;
		}
		return false;
	}

	protected bool HasTargetSession(string targetId)
	{
		foreach (string sessionId, DZCRZ_HackSession session: m_Sessions)
		{
			if (session && session.TargetId == targetId && session.State == DZCRZ_Const.STATE_ACTIVE)
				return true;
		}
		return false;
	}

	protected void WriteRuntime(string status)
	{
		if (!GetGame() || !GetGame().IsServer())
			return;
		DZCRZ_RuntimeFile runtime = new DZCRZ_RuntimeFile;
		runtime.LastStatus = status;
		foreach (string sessionId, DZCRZ_HackSession session: m_Sessions)
		{
			if (!session || session.State != DZCRZ_Const.STATE_ACTIVE)
				continue;
			DZCRZ_RuntimeSessionEntry entry = new DZCRZ_RuntimeSessionEntry;
			entry.SessionId = session.SessionId;
			entry.PlayerId = session.PlayerId;
			entry.TargetId = session.TargetId;
			entry.ModuleType = session.ModuleType;
			entry.DurationSeconds = session.DurationSeconds;
			entry.ElapsedSeconds = session.GetElapsedSeconds();
			runtime.ActiveSessions.Insert(entry);
		}
		string errorMessage;
		if (!JsonFileLoader<ref DZCRZ_RuntimeFile>.SaveFile(DZCRZ_ProfilePaths.RUNTIME_STATE, runtime, errorMessage))
			DZCRZ_Log.Error("Runtime-Status konnte nicht gespeichert werden: " + errorMessage);
	}
}
