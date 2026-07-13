class DZBBC_EventManager
{
	protected static ref DZBBC_EventManager s_Instance;
	protected ref DZBBC_ConfigBundle m_Config;
	protected ref DZBBC_EventInstance m_Instance;
	protected bool m_Initialized;

	static DZBBC_EventManager GetInstance()
	{
		if (!s_Instance)
			s_Instance = new DZBBC_EventManager;

		return s_Instance;
	}

	void InitServer()
	{
		if (m_Initialized || !GetGame() || !GetGame().IsServer())
			return;

		m_Config = DZBBC_Config.LoadAll();
		m_Initialized = true;
		DZBBC_Utils.Log("Server initialized, version " + DZBBC_VERSION);

		if (m_Config && m_Config.Main && m_Config.Main.AutoStart)
		{
			int delayMs = m_Config.Main.AutoStartDelaySeconds * 1000;
			if (delayMs < 1000)
				delayMs = 1000;
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(AutoStartEvent, delayMs, false);
			DZBBC_Utils.Log("AutoStart scheduled in " + m_Config.Main.AutoStartDelaySeconds.ToString() + " seconds");
		}
	}

	protected void AutoStartEvent()
	{
		StartEvent(0);
	}

	bool StartEvent(int crashsiteIndex = 0)
	{
		if (!GetGame() || !GetGame().IsServer())
			return false;

		if (!m_Config)
			m_Config = DZBBC_Config.LoadAll();

		if (m_Instance && m_Instance.IsActive())
		{
			DZBBC_Utils.Warn("StartEvent ignored: active instance exists.");
			return false;
		}

		m_Instance = new DZBBC_EventInstance(m_Config);
		return m_Instance.Start(crashsiteIndex);
	}

	void StopEvent()
	{
		if (m_Instance)
			m_Instance.StopAsFailed();
	}

	void CleanupEvent()
	{
		if (m_Instance)
			m_Instance.Cleanup();
	}

	void SyncPlayerState(PlayerBase player)
	{
		if (!player)
			return;

		if (m_Instance)
			m_Instance.SyncToPlayer(player);
	}

	bool StartBlackboxHack(PlayerBase player, Object blackbox)
	{
		return m_Instance && m_Instance.StartBlackboxHack(player, blackbox);
	}

	bool CanUseBlackboxAction(PlayerBase player, Object blackbox)
	{
		return m_Instance && m_Instance.CanUseBlackboxAction(player, blackbox);
	}

	bool CompleteBlackboxHack(PlayerBase player, Object blackbox)
	{
		return m_Instance && m_Instance.CompleteBlackboxHack(player, blackbox);
	}

	bool CancelBlackboxHack(PlayerBase player, Object blackbox, string reasonCode = "INPUT_RELEASED")
	{
		return m_Instance && m_Instance.CancelBlackboxHack(player, blackbox, reasonCode);
	}

	float GetBlackboxHackDuration()
	{
		if (!m_Config)
			m_Config = DZBBC_Config.LoadAll();

		if (!m_Config || !m_Config.Main || m_Config.Main.HackDurationSeconds < 1.0)
			return 90.0;

		return m_Config.Main.HackDurationSeconds;
	}

	bool StartTerminalDecrypt(PlayerBase player, Object terminal)
	{
		return m_Instance && m_Instance.StartTerminalDecrypt(player, terminal);
	}

	bool CompleteTerminalDecrypt(PlayerBase player, Object terminal)
	{
		return m_Instance && m_Instance.CompleteTerminalDecrypt(player, terminal);
	}

	bool CancelTerminalDecrypt(PlayerBase player, Object terminal)
	{
		return m_Instance && m_Instance.CancelTerminalDecrypt(player, terminal);
	}

	float GetTerminalDecryptDuration()
	{
		if (!m_Config)
			m_Config = DZBBC_Config.LoadAll();

		if (!m_Config || !m_Config.Main || m_Config.Main.TerminalDecryptSeconds < 1.0)
			return 60.0;

		return m_Config.Main.TerminalDecryptSeconds;
	}

	float GetDamageMultiplierForSource(EntityAI source)
	{
		if (!m_Instance || !source)
			return 1.0;

		return m_Instance.GetDamageMultiplierForSource(source);
	}

	bool ShouldBlockVehicleManagedUnitContact(Object unit)
	{
		return m_Instance && m_Instance.ShouldBlockVehicleManagedUnitContact(unit);
	}

	bool CanUseAdminCommand(PlayerIdentity identity)
	{
		if (!identity || !m_Config || !m_Config.Main || !m_Config.Main.DebugCommandsEnabled)
			return false;

		if (!m_Config.Main.AdminUIDs || m_Config.Main.AdminUIDs.Count() == 0)
			return m_Config.Main.AllowAdminWhenListEmpty;

		return m_Config.Main.AdminUIDs.Find(identity.GetId()) >= 0;
	}

	void HandleAdminCommand(PlayerIdentity identity, PlayerBase player, string rawCommand)
	{
		if (!CanUseAdminCommand(identity))
		{
			DZBBC_ServerRPC.SendNotification(player, "#STR_DZBBC_TITLE_ADMIN", "#STR_DZBBC_ADMIN_NO_PERMISSION", 5.0);
			return;
		}

		string command = rawCommand.Trim();
		command.ToLower();

		array<string> parts = new array<string>;
		command.Split(" ", parts);
		if (!parts || parts.Count() == 0)
			return;

		if (parts.Get(0) == DZBBC_ADMIN_COMMAND_PREFIX)
			parts.Remove(0);

		string action = "debug";
		if (parts.Count() > 0)
			action = parts.Get(0);

		if (action == "start")
		{
			int crashsiteIndex = ResolveCrashsiteIndex(parts);
			StartEvent(crashsiteIndex);
			Reply(player, "start ausgefuehrt fuer index " + crashsiteIndex.ToString());
		}
		else if (action == "stop")
		{
			StopEvent();
			Reply(player, "stop ausgefuehrt");
		}
		else if (action == "cleanup")
		{
			CleanupEvent();
			Reply(player, "cleanup ausgefuehrt");
		}
		else if (action == "forcecombatcomplete")
		{
			if (m_Instance)
				m_Instance.DebugForceCombatComplete();
			Reply(player, "forcecombatcomplete ausgefuehrt");
		}
		else if (action == "forceblackbox")
		{
			if (m_Instance)
				m_Instance.DebugForceBlackbox();
			Reply(player, "forceblackbox ausgefuehrt");
		}
		else if (action == "sethack" && parts.Count() >= 2)
		{
			if (m_Instance)
				m_Instance.DebugSetHack(parts.Get(1).ToFloat());
			Reply(player, "hack progress gesetzt");
		}
		else if (action == "givedatacore")
		{
			if (m_Instance)
				m_Instance.DebugGiveDataCore(player);
			Reply(player, "datacore gegeben");
		}
		else if (action == "forceterminal")
		{
			if (m_Instance)
				m_Instance.DebugForceTerminal(player);
			Reply(player, "forceterminal ausgefuehrt");
		}
		else if (action == "setdecrypt" && parts.Count() >= 2)
		{
			if (m_Instance)
				m_Instance.DebugSetDecrypt(parts.Get(1).ToFloat());
			Reply(player, "decrypt progress gesetzt");
		}
		else if (action == "testspawn")
		{
			Reply(player, "testspawn ist im Live-Build deaktiviert");
		}
		else if (action == "testvisual")
		{
			Reply(player, "testvisual ist im Live-Build deaktiviert");
		}
		else if (action == "testteleport")
		{
			Reply(player, "testteleport ist im Live-Build deaktiviert");
		}
		else if (action == "testcomplete")
		{
			Reply(player, "testcomplete ist im Live-Build deaktiviert");
		}
		else
		{
			Reply(player, GetDebugState());
		}
	}

	string GetDebugState()
	{
		if (!m_Instance)
			return "state=INACTIVE instance=null";

		return m_Instance.GetDebugState();
	}

	protected void Reply(PlayerBase player, string text)
	{
		DZBBC_Utils.Log("Admin: " + text);
		DZBBC_ServerRPC.SendNotification(player, "DZBBC Admin", text, 7.0);
	}

	protected int ResolveCrashsiteIndex(array<string> parts)
	{
		if (!parts || parts.Count() < 2)
			return 0;

		string token = parts.Get(1);
		token.ToLower();
		if (token == "airfield" || token == "nwaf")
			return 0;

		if (token == "vmc")
			return 1;

		int index = token.ToInt();
		if (index < 0)
			return 0;

		if (m_Config && m_Config.Locations && m_Config.Locations.CrashSites && index >= m_Config.Locations.CrashSites.Count())
			return 0;

		return index;
	}

}
