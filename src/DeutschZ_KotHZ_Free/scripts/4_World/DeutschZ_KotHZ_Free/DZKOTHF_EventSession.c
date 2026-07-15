class DZKOTHF_EventSession
{
	protected int m_State;
	protected int m_StartedAt;
	protected string m_AbortReason;

	void DZKOTHF_EventSession()
	{
		m_State = DZKOTHF_EventState.IDLE;
		m_StartedAt = 0;
		m_AbortReason = "";
	}

	int GetState()
	{
		return m_State;
	}

	int GetStartedAt()
	{
		return m_StartedAt;
	}

	string GetAbortReason()
	{
		return m_AbortReason;
	}

	void MarkStarted()
	{
		m_StartedAt = GetGame().GetTime();
		m_AbortReason = "";
	}

	void SetAbortReason(string reason)
	{
		m_AbortReason = reason;
	}

	void SetState(int state)
	{
		m_State = state;
	}

	void Reset()
	{
		m_State = DZKOTHF_EventState.IDLE;
		m_StartedAt = 0;
		m_AbortReason = "";
	}
}
