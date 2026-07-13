class DZBBC_HackManager
{
	protected PlayerBase m_Player;
	protected Object m_Blackbox;
	protected float m_Progress;
	protected float m_Duration;
	protected bool m_Active;
	protected bool m_Stage30;
	protected bool m_Stage60;
	protected bool m_Stage90;

	bool Begin(PlayerBase player, Object blackbox, float duration)
	{
		if (!DZBBC_PlayerUtils.IsValidPlayer(player) || !blackbox || m_Active)
			return false;

		m_Player = player;
		m_Blackbox = blackbox;
		m_Duration = duration;
		if (m_Duration < 1.0)
			m_Duration = 90.0;
		m_Progress = 0.0;
		m_Stage30 = false;
		m_Stage60 = false;
		m_Stage90 = false;
		m_Active = true;
		return true;
	}

	void Tick(float deltaSeconds, DZBBC_EventInstance instance)
	{
		if (!m_Active || !instance)
			return;

		m_Progress += deltaSeconds;
		float percent = GetPercent();
		DZBBC_ServerRPC.SendHud(m_Player, DZBBC_HUD_HACK, "#STR_DZBBC_HUD_HACK", percent, 100.0);

		if (m_Progress >= 30.0 && !m_Stage30)
		{
			m_Stage30 = true;
			instance.OnHackStage(30, m_Player);
		}
		if (m_Progress >= 60.0 && !m_Stage60)
		{
			m_Stage60 = true;
			instance.OnHackStage(60, m_Player);
		}
		if (m_Progress > m_Duration)
			m_Progress = m_Duration;
	}

	void SetProgressSeconds(float seconds)
	{
		m_Progress = Math.Clamp(seconds, 0.0, m_Duration);
	}

	bool CanComplete(PlayerBase player)
	{
		if (!m_Active || !player || player != m_Player)
			return false;

		return m_Progress >= (m_Duration - 2.0);
	}

	bool Complete(PlayerBase player)
	{
		if (!CanComplete(player))
			return false;

		m_Progress = m_Duration;
		m_Stage90 = true;
		m_Active = false;
		return true;
	}

	bool Cancel(PlayerBase player = null)
	{
		if (!m_Active)
			return false;

		if (player && player != m_Player)
			return false;

		m_Active = false;
		m_Player = null;
		m_Blackbox = null;
		m_Progress = 0.0;
		return true;
	}

	bool IsActive()
	{
		return m_Active;
	}

	PlayerBase GetPlayer()
	{
		return m_Player;
	}

	Object GetBlackbox()
	{
		return m_Blackbox;
	}

	float GetProgressSeconds()
	{
		return m_Progress;
	}

	float GetPercent()
	{
		if (m_Duration <= 0.0)
			return 0.0;

		return Math.Clamp((m_Progress / m_Duration) * 100.0, 0.0, 100.0);
	}
}
