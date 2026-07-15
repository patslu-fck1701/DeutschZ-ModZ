class DZKOTHF_EventController
{
	protected static ref DZKOTHF_EventController s_Instance;
	protected ref DZKOTHF_EventSession m_Session;
	protected bool m_Initialized;

	static DZKOTHF_EventController GetInstance()
	{
		if (!s_Instance)
			s_Instance = new DZKOTHF_EventController;

		return s_Instance;
	}

	static void DestroyInstance()
	{
		if (s_Instance)
			s_Instance.ShutdownServer();

		s_Instance = null;
	}

	void DZKOTHF_EventController()
	{
		m_Session = new DZKOTHF_EventSession;
	}

	void InitServer()
	{
		if (m_Initialized || !GetGame() || !GetGame().IsServer())
			return;

		m_Initialized = true;
		DZKOTHF_Log.Info("Controller initialized. Version " + DZKOTHF_Constants.VERSION + ". State " + GetStateName() + ".");
	}

	void ShutdownServer()
	{
		if (!m_Initialized)
			return;

		if (GetState() != DZKOTHF_EventState.IDLE)
		{
			m_Session.SetAbortReason("Mission shutdown");
			TransitionTo(DZKOTHF_EventState.CLEANUP);
			TransitionTo(DZKOTHF_EventState.IDLE);
		}

		m_Initialized = false;
		DZKOTHF_Log.Info("Controller shut down.");
	}

	bool StartEvent()
	{
		if (!m_Initialized || !GetGame() || !GetGame().IsServer())
		{
			DZKOTHF_Log.Warning("Start rejected: controller is not initialized on the server.");
			return false;
		}

		if (GetState() != DZKOTHF_EventState.IDLE)
		{
			DZKOTHF_Log.Warning("Start rejected: event is already in state " + GetStateName() + ".");
			return false;
		}

		m_Session.MarkStarted();
		return TransitionTo(DZKOTHF_EventState.ANNOUNCED);
	}

	bool ActivateEvent()
	{
		return TransitionTo(DZKOTHF_EventState.ACTIVE);
	}

	bool CompleteCapture()
	{
		return TransitionTo(DZKOTHF_EventState.CAPTURE_COMPLETE);
	}

	bool ActivateReward()
	{
		return TransitionTo(DZKOTHF_EventState.REWARD);
	}

	bool BeginCleanup()
	{
		return TransitionTo(DZKOTHF_EventState.CLEANUP);
	}

	bool FinishCleanup()
	{
		if (!TransitionTo(DZKOTHF_EventState.IDLE))
			return false;

		m_Session.Reset();
		return true;
	}

	bool AbortEvent(string reason)
	{
		if (GetState() == DZKOTHF_EventState.IDLE || GetState() == DZKOTHF_EventState.CLEANUP)
			return false;

		m_Session.SetAbortReason(reason);
		DZKOTHF_Log.Warning("Event abort requested: " + reason + ".");
		return TransitionTo(DZKOTHF_EventState.CLEANUP);
	}

	int GetState()
	{
		if (!m_Session)
			return DZKOTHF_EventState.IDLE;

		return m_Session.GetState();
	}

	string GetStateName()
	{
		return DZKOTHF_EventState.ToString(GetState());
	}

	protected bool TransitionTo(int nextState)
	{
		int currentState = GetState();
		if (!IsTransitionAllowed(currentState, nextState))
		{
			DZKOTHF_Log.Warning("Rejected state transition " + DZKOTHF_EventState.ToString(currentState) + " -> " + DZKOTHF_EventState.ToString(nextState) + ".");
			return false;
		}

		m_Session.SetState(nextState);
		DZKOTHF_Log.Info("State " + DZKOTHF_EventState.ToString(currentState) + " -> " + DZKOTHF_EventState.ToString(nextState) + ".");
		return true;
	}

	protected bool IsTransitionAllowed(int currentState, int nextState)
	{
		if (currentState == DZKOTHF_EventState.IDLE)
			return nextState == DZKOTHF_EventState.ANNOUNCED;

		if (currentState == DZKOTHF_EventState.ANNOUNCED)
			return nextState == DZKOTHF_EventState.ACTIVE || nextState == DZKOTHF_EventState.CLEANUP;

		if (currentState == DZKOTHF_EventState.ACTIVE)
			return nextState == DZKOTHF_EventState.CAPTURE_COMPLETE || nextState == DZKOTHF_EventState.CLEANUP;

		if (currentState == DZKOTHF_EventState.CAPTURE_COMPLETE)
			return nextState == DZKOTHF_EventState.REWARD || nextState == DZKOTHF_EventState.CLEANUP;

		if (currentState == DZKOTHF_EventState.REWARD)
			return nextState == DZKOTHF_EventState.CLEANUP;

		if (currentState == DZKOTHF_EventState.CLEANUP)
			return nextState == DZKOTHF_EventState.IDLE;

		return false;
	}
}
