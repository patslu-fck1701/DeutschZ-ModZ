class DZBBC_HackManager
{
	protected PlayerBase m_Player;
	protected float m_Progress;
	protected float m_Duration;
	protected bool m_Active;
	protected bool m_Stage30;
	protected bool m_Stage60;
	protected bool m_Stage90;

	bool Begin(PlayerBase player, float duration)
	{
		if (!DZBBC_PlayerUtils.IsValidPlayer(player) || m_Active)
			return false;

		m_Player = player;
		m_Duration = duration;
		if (m_Duration < 1.0)
			m_Duration = 120.0;
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

		if (!DZBBC_PlayerUtils.IsValidPlayer(m_Player))
		{
			Cancel();
			instance.OnHackFailed();
			return;
		}

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
		if (m_Progress >= 90.0 && !m_Stage90)
		{
			m_Stage90 = true;
			instance.OnHackStage(90, m_Player);
		}
		if (m_Progress >= m_Duration)
		{
			m_Active = false;
			instance.OnHackComplete(m_Player);
		}
	}

	void SetProgressSeconds(float seconds)
	{
		m_Progress = Math.Clamp(seconds, 0.0, m_Duration);
	}

	void Cancel()
	{
		m_Active = false;
		m_Player = null;
	}

	bool IsActive()
	{
		return m_Active;
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
