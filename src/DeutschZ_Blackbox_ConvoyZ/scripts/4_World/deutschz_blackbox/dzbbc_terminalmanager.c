class DZBBC_TerminalManager
{
	protected ref array<Object> m_Terminals;
	protected DZBBC_TerminalLocationConfig m_ActiveTerminalConfig;
	protected Object m_ActiveTerminal;
	protected PlayerBase m_Player;
	protected float m_DecryptProgress;
	protected float m_DecryptDuration;
	protected bool m_DecryptActive;

	void DZBBC_TerminalManager()
	{
		m_Terminals = new array<Object>;
	}

	void SpawnTerminals(DZBBC_TerminalsConfig config)
	{
		if (!config || !config.Terminals)
			return;

		foreach (DZBBC_TerminalLocationConfig terminal: config.Terminals)
		{
			if (!terminal)
				continue;

			Object object = DZBBC_Utils.CreateObjectSafe(DZBBC_TERMINAL_CLASSNAME, terminal.GetPosition());
			if (object)
			{
				m_Terminals.Insert(object);
				DZBBC_Utils.Log("Terminal spawned " + terminal.Name + " at " + object.GetPosition().ToString());
				if (!m_ActiveTerminal)
				{
					m_ActiveTerminal = object;
					m_ActiveTerminalConfig = terminal;
				}
			}
		}
	}

	bool Begin(PlayerBase player, Object target, DZBBC_DataCoreTracker tracker, DZBBC_MainConfig main)
	{
		if (!DZBBC_PlayerUtils.IsValidPlayer(player) || !target || !tracker || !tracker.IsCarrier(player))
			return false;

		if (m_Terminals.Find(target) < 0)
			return false;

		if (main && vector.Distance(player.GetPosition(), target.GetPosition()) > main.PlayerInteractionDistance)
			return false;

		m_Player = player;
		m_DecryptDuration = main.TerminalDecryptSeconds;
		if (m_DecryptDuration < 1.0)
			m_DecryptDuration = 180.0;
		m_DecryptProgress = 0.0;
		m_DecryptActive = true;
		m_ActiveTerminal = target;
		return true;
	}

	bool Tick(float deltaSeconds)
	{
		if (!m_DecryptActive)
			return false;

		if (!DZBBC_PlayerUtils.IsValidPlayer(m_Player))
		{
			m_DecryptActive = false;
			return false;
		}

		m_DecryptProgress += deltaSeconds;
		DZBBC_ServerRPC.SendHud(m_Player, DZBBC_HUD_TERMINAL, "#STR_DZBBC_HUD_TERMINAL", GetPercent(), 100.0);
		return m_DecryptProgress >= m_DecryptDuration;
	}

	void SetProgress(float percent)
	{
		if (m_DecryptDuration <= 0.0)
			m_DecryptDuration = 180.0;

		m_DecryptProgress = Math.Clamp((percent / 100.0) * m_DecryptDuration, 0.0, m_DecryptDuration);
	}

	DZBBC_TerminalLocationConfig GetActiveTerminalConfig()
	{
		return m_ActiveTerminalConfig;
	}

	Object GetActiveTerminal()
	{
		return m_ActiveTerminal;
	}

	float GetPercent()
	{
		if (m_DecryptDuration <= 0.0)
			return 0.0;

		return Math.Clamp((m_DecryptProgress / m_DecryptDuration) * 100.0, 0.0, 100.0);
	}

	bool IsDecryptActive()
	{
		return m_DecryptActive;
	}

	void Cleanup()
	{
		foreach (Object terminal: m_Terminals)
		{
			DZBBC_Utils.DeleteObjectSafe(terminal);
		}

		m_Terminals.Clear();
		m_ActiveTerminal = null;
		m_ActiveTerminalConfig = null;
		m_Player = null;
		m_DecryptActive = false;
		m_DecryptProgress = 0.0;
	}
}
