class DZKOTH_TerminalHackManager
{
	protected EntityAI m_Terminal;
	protected PlayerBase m_Hacker;
	protected float m_ProgressSeconds;
	protected bool m_Active;
	protected bool m_Complete;
	protected bool m_ActionUnlocked;

	bool SpawnTerminal(DZKOTH_LocationConfig location)
	{
		if (!GetGame() || !location)
			return false;

		Cleanup();

		vector pos = DZKOTH_Utils.Grounded(location.GetTerminalPosition());
		Object object = GetGame().CreateObjectEx(DZKOTH_Const.TERMINAL_CLASSNAME, pos, ECE_PLACE_ON_SURFACE | ECE_SETUP);
		m_Terminal = EntityAI.Cast(object);
		if (!m_Terminal)
		{
			DZKOTH_Utils.Warn("Could not spawn event chest at " + pos.ToString());
			if (object)
				GetGame().ObjectDelete(object);
			return false;
		}

		m_Terminal.SetOrientation(location.GetRewardCrateOrientation());
		SetActionUnlocked(false);
		DZKOTH_Utils.Log("Chest spawned at " + m_Terminal.GetPosition().ToString());
		return true;
	}

	void SetActionUnlocked(bool unlocked)
	{
		m_ActionUnlocked = unlocked;
		DZKOTH_EventTerminal chest = DZKOTH_EventTerminal.Cast(m_Terminal);
		if (chest)
			chest.DZKOTH_SetActionUnlocked(unlocked);
	}

	bool Begin(PlayerBase player, Object terminal, DZKOTH_MainConfig config)
	{
		if (!IsTerminal(terminal) || !player || !config || m_Complete || !m_ActionUnlocked)
			return false;

		if (!player.IsAlive() || player.IsUnconscious())
			return false;

		if (vector.Distance(player.GetPosition(), terminal.GetPosition()) > 6.0)
			return false;

		m_Hacker = player;
		m_Active = true;
		m_ProgressSeconds = 0.0;
		DZKOTH_Utils.Log("Chest action started by " + DZKOTH_GroupResolver.GetPlayerUid(player));
		return true;
	}

	bool CompleteFromAction(PlayerBase player, Object terminal, DZKOTH_MainConfig config)
	{
		if (!IsTerminal(terminal) || !player || !config || m_Complete || !m_ActionUnlocked || !m_Active || m_Hacker != player)
			return false;

		if (!player.IsAlive() || player.IsUnconscious())
			return false;

		if (vector.Distance(player.GetPosition(), terminal.GetPosition()) > 6.0)
			return false;

		m_Hacker = player;
		m_ProgressSeconds = GetDuration(config);
		m_Complete = true;
		m_Active = false;
		SetActionUnlocked(false);
		DZKOTH_Utils.Log("Chest action completed by " + DZKOTH_GroupResolver.GetPlayerUid(player));
		return true;
	}

	bool Cancel(PlayerBase player, Object terminal = null)
	{
		if (!m_Active || m_Complete)
			return false;

		if (player && m_Hacker && player != m_Hacker)
			return false;

		if (terminal && !IsTerminal(terminal))
			return false;

		m_Active = false;
		m_Hacker = null;
		m_ProgressSeconds = 0.0;
		DZKOTH_Utils.Log("Chest action interrupted.");
		return true;
	}

	bool Tick(float delta, DZKOTH_EventInstance eventInstance, DZKOTH_MainConfig config)
	{
		if (m_Complete)
			return true;

		if (!config)
			return false;

		if (m_Active)
		{
			if (!IsHackerValid())
			{
				Cancel(m_Hacker);
				return false;
			}

			m_ProgressSeconds = m_ProgressSeconds + delta;
			float duration = GetDuration(config);
			if (m_ProgressSeconds >= duration)
			{
				m_ProgressSeconds = duration;
				m_Complete = true;
				m_Active = false;
				SetActionUnlocked(false);
				DZKOTH_Utils.Log("Chest action complete.");
				if (eventInstance)
					eventInstance.OnTerminalHackComplete(m_Hacker);
				return true;
			}

			return false;
		}

		return false;
	}

	bool IsTerminal(Object object)
	{
		return m_Terminal && object && object == m_Terminal;
	}

	bool IsSpawned()
	{
		return m_Terminal != null;
	}

	bool IsActive()
	{
		return m_Active;
	}

	bool IsComplete()
	{
		return m_Complete;
	}

	bool IsActionUnlocked()
	{
		return m_ActionUnlocked;
	}

	float GetProgressPercent(DZKOTH_MainConfig config)
	{
		float duration = GetDuration(config);
		if (duration <= 0.0)
			return 100.0;

		return Math.Clamp((m_ProgressSeconds / duration) * 100.0, 0.0, 100.0);
	}

	string GetHackerName()
	{
		if (!m_Hacker)
			return "";

		return DZKOTH_GroupResolver.GetPlayerUid(m_Hacker);
	}

	void SetProgressPercent(float percent, DZKOTH_MainConfig config)
	{
		m_ProgressSeconds = Math.Clamp(percent, 0.0, 100.0) * 0.01 * GetDuration(config);
	}

	void ForceComplete(DZKOTH_EventInstance eventInstance)
	{
		m_Complete = true;
		m_Active = false;
		SetActionUnlocked(false);
		if (eventInstance)
			eventInstance.OnTerminalHackComplete(m_Hacker);
	}

	void Cleanup()
	{
		if (m_Terminal && GetGame())
			GetGame().ObjectDelete(m_Terminal);

		m_Terminal = null;
		m_Hacker = null;
		m_ProgressSeconds = 0.0;
		m_Active = false;
		m_Complete = false;
		m_ActionUnlocked = false;
	}

	protected bool IsHackerValid()
	{
		if (!m_Hacker || !m_Hacker.IsAlive() || m_Hacker.IsUnconscious() || !m_Terminal)
			return false;

		return vector.Distance(m_Hacker.GetPosition(), m_Terminal.GetPosition()) <= 6.0;
	}

	protected float GetDuration(DZKOTH_MainConfig config)
	{
		if (!config)
			return 60.0;

		if (config.ChestActionDurationSeconds >= 1)
			return config.ChestActionDurationSeconds;

		if (config.TerminalHackSeconds >= 1)
			return config.TerminalHackSeconds;

		return 60.0;
	}
}
