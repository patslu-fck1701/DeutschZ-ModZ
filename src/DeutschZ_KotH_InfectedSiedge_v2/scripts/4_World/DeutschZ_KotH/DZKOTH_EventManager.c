class DZKOTH_EventManager
{
	protected static ref DZKOTH_EventManager s_Instance;
	protected ref DZKOTH_ConfigBundle m_Config;
	protected ref DZKOTH_EventInstance m_Instance;
	protected bool m_Initialized;

	static DZKOTH_EventManager GetInstance()
	{
		if (!s_Instance)
			s_Instance = new DZKOTH_EventManager;

		return s_Instance;
	}

	void InitServer()
	{
		if (m_Initialized || !GetGame() || !GetGame().IsServer())
			return;

		m_Config = DZKOTH_Config.LoadAll();
		if (!m_Config || !m_Config.Main || m_Config.Main.Enabled == 0)
		{
			m_Initialized = true;
			DZKOTH_Utils.Warn("Event manager initialized but KotH is disabled by config.");
			return;
		}

		DZKOTH_ExpansionBridge.Configure(m_Config.Main);
		DZKOTH_GroupResolver.InitServer();
		m_Initialized = true;
		DZKOTH_Utils.Log("Event manager initialized. Version " + DZKOTH_Const.VERSION);
		StartEvent();
	}

	bool StartEvent(int locationIndex = -1)
	{
		if (!m_Config || !m_Config.Main || m_Config.Main.Enabled == 0)
		{
			DZKOTH_Utils.Warn("StartEvent ignored: KotH disabled by config.");
			return false;
		}

		if (!m_Config || !m_Config.Locations || !m_Config.Locations.Locations || m_Config.Locations.Locations.Count() == 0)
		{
			DZKOTH_Utils.Error("No event locations configured.");
			return false;
		}

		if (m_Instance && m_Instance.GetState() != DZKOTH_States.INACTIVE)
		{
			DZKOTH_Utils.Warn("StartEvent ignored: an instance is already active.");
			return false;
		}

		if (locationIndex < 0 || locationIndex >= m_Config.Locations.Locations.Count())
			locationIndex = Math.RandomInt(0, m_Config.Locations.Locations.Count());

		DZKOTH_LocationConfig location = m_Config.Locations.Locations.Get(locationIndex);
		m_Instance = new DZKOTH_EventInstance(m_Config, location);
		return m_Instance.Start();
	}

	void ScheduleNextEvent()
	{
		if (!GetGame() || !m_Config || !m_Config.Main)
			return;

		int delayMs = m_Config.Main.EventCooldownMinutes * 60000;
		if (delayMs < 60000)
			delayMs = 60000;

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(StartNextEvent);
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(StartNextEvent, delayMs, false);
		DZKOTH_Utils.Log("Next event scheduled in " + m_Config.Main.EventCooldownMinutes.ToString() + " minute(s).");
	}

	protected void StartNextEvent()
	{
		StartEvent(-1);
	}

	void StopEvent()
	{
		if (!m_Instance)
			return;

		m_Instance.Destroy();
		m_Instance = null;
	}

	void CleanupEvent()
	{
		if (m_Instance)
			m_Instance.CleanupEvent();
	}

	void SyncStateToPlayer(PlayerBase player)
	{
		if (m_Instance)
			m_Instance.SyncStateToPlayer(player);
	}

	bool StartTerminalHack(PlayerBase player, Object terminal)
	{
		return m_Instance && m_Instance.StartTerminalHack(player, terminal);
	}

	bool BeginTerminalHack(PlayerBase player, Object terminal)
	{
		return m_Instance && m_Instance.BeginTerminalHack(player, terminal);
	}

	bool CancelTerminalHack(PlayerBase player, Object terminal = null)
	{
		return m_Instance && m_Instance.CancelTerminalHack(player, terminal);
	}

	float GetChestActionDuration()
	{
		if (!m_Config || !m_Config.Main)
			return 60.0;

		if (m_Config.Main.ChestActionDurationSeconds < 1)
			return 60.0;

		return m_Config.Main.ChestActionDurationSeconds;
	}

	bool ShouldBlockVehicleZombieContact(Object zombie)
	{
		if (!zombie || !m_Instance)
			return false;

		return m_Instance.IsManagedInfected(zombie) && m_Instance.IsPositionInsideActiveZone(zombie.GetPosition());
	}

	float GetDamageMultiplierForSource(EntityAI source)
	{
		if (!source || !m_Instance)
			return 1.0;

		return m_Instance.GetDamageMultiplierForSource(source);
	}

	void DebugForceBoss()
	{
		if (m_Instance)
			m_Instance.DebugForceBoss();
	}

	void DebugForceTerminalHack()
	{
		if (m_Instance)
			m_Instance.DebugForceTerminalHack();
	}

	void DebugSetTerminalHack(float progress)
	{
		if (m_Instance)
			m_Instance.DebugSetTerminalHack(progress);
	}

	void DebugSetProgress(float progress)
	{
		if (m_Instance)
			m_Instance.DebugSetProgress(progress);
	}

	bool DebugStartCapture(PlayerBase player)
	{
		if (!player)
			return false;

		if (!m_Instance || m_Instance.GetState() == DZKOTH_States.INACTIVE)
			StartEvent();

		if (!m_Instance)
			return false;

		return m_Instance.DebugStartCapture(player);
	}

	bool CanUseAdminCommand(string uid)
	{
		if (uid == "" || !m_Config || !m_Config.Main || !m_Config.Main.DebugCommandsEnabled || !m_Config.Main.AdminUIDs)
			return false;

		return m_Config.Main.AdminUIDs.Find(uid) >= 0;
	}

	string GetDebugState()
	{
		if (!m_Instance)
			return "no-instance";

		return m_Instance.GetDebugState();
	}
}
