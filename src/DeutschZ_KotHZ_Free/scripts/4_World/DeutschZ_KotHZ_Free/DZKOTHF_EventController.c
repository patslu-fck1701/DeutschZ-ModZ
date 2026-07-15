class DZKOTHF_EventController
{
	protected static ref DZKOTHF_EventController s_Instance;
	protected ref DZKOTHF_EventSession m_Session;
	protected ref DZKOTHF_Settings m_Settings;
	protected bool m_Initialized;
	protected bool m_CaptureTickRunning;

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

		m_Settings = DZKOTHF_SettingsLoader.Load();
		m_Initialized = true;
		DZKOTHF_Log.Info("Controller initialized. Version " + DZKOTHF_Constants.VERSION + ". State " + GetStateName() + ".");

		if (m_Settings && m_Settings.Enabled && m_Settings.AutoStart)
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(StartEvent, m_Settings.AutoStartDelaySeconds * 1000, false);
	}

	void ShutdownServer()
	{
		if (!m_Initialized)
			return;

		RemoveScheduledCalls();
		if (m_Session)
		{
			m_Session.SetAbortReason("Mission shutdown");
			m_Session.CleanupWorldObjects();
			m_Session.SetState(DZKOTHF_EventState.IDLE);
		}

		m_Initialized = false;
		DZKOTHF_Log.Info("Controller shut down.");
	}

	bool StartEvent()
	{
		if (!m_Initialized || !GetGame() || !GetGame().IsServer() || !m_Settings || !m_Settings.Enabled)
		{
			DZKOTHF_Log.Warning("Start rejected: controller or settings are not ready on the server.");
			return false;
		}

		if (GetState() != DZKOTHF_EventState.IDLE)
		{
			DZKOTHF_Log.Warning("Start rejected: event is already in state " + GetStateName() + ".");
			return false;
		}

		m_Session.MarkStarted();
		if (!m_Session.SpawnWorldObjects(m_Settings))
		{
			m_Session.Reset();
			return false;
		}

		if (!TransitionTo(DZKOTHF_EventState.ANNOUNCED))
		{
			m_Session.Reset();
			return false;
		}

		m_Session.SetSmokeState(DZKOTHF_SmokeState.WHITE);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ActivateEvent, m_Settings.AnnounceSeconds * 1000, false);
		return true;
	}

	bool ActivateEvent()
	{
		if (!TransitionTo(DZKOTHF_EventState.ACTIVE))
			return false;

		m_Session.SetSmokeState(DZKOTHF_SmokeState.GREEN);
		m_CaptureTickRunning = true;
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CaptureTick, m_Settings.CaptureTickMilliseconds, false);
		return true;
	}

	protected void CaptureTick()
	{
		if (!m_CaptureTickRunning || GetState() != DZKOTHF_EventState.ACTIVE || !GetGame())
			return;

		int playerCount = CountAlivePlayersInRadius();
		m_Session.SetPlayerCount(playerCount);
		if (playerCount > 1)
		{
			m_Session.SetSmokeState(DZKOTHF_SmokeState.RED);
		}
		else
		{
			m_Session.SetSmokeState(DZKOTHF_SmokeState.GREEN);
			if (playerCount == 1)
			{
				float tickSeconds = m_Settings.CaptureTickMilliseconds / 1000.0;
				float nextProgress = m_Session.GetCaptureProgress() + (tickSeconds / m_Settings.CaptureTimeSeconds);
				m_Session.SetCaptureProgress(nextProgress);
				if (nextProgress >= 1.0)
				{
					CompleteCapture();
					return;
				}
			}
		}

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CaptureTick, m_Settings.CaptureTickMilliseconds, false);
	}

	protected int CountAlivePlayersInRadius()
	{
		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		int playerCount = 0;
		foreach (Man man: players)
		{
			PlayerBase player = PlayerBase.Cast(man);
			if (!player || !player.IsAlive())
				continue;

			if (vector.Distance(player.GetPosition(), m_Session.GetCenter()) <= m_Session.GetCaptureRadius())
				playerCount++;
		}

		return playerCount;
	}

	bool CompleteCapture()
	{
		StopCaptureTick();
		if (!TransitionTo(DZKOTHF_EventState.CAPTURE_COMPLETE))
			return false;

		m_Session.SetCaptureProgress(1.0);
		m_Session.SetSmokeState(DZKOTHF_SmokeState.WHITE);
		TransitionTo(DZKOTHF_EventState.REWARD);
		DZKOTHF_Log.Info("Phase B capture complete. No reward system is active in this phase.");
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(BeginCleanup, m_Settings.CompletionCleanupDelaySeconds * 1000, false);
		return true;
	}

	bool BeginCleanup()
	{
		StopCaptureTick();
		if (GetGame())
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(ActivateEvent);

		if (!TransitionTo(DZKOTHF_EventState.CLEANUP))
			return false;

		m_Session.CleanupWorldObjects();
		return FinishCleanup();
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

		StopCaptureTick();
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(ActivateEvent);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(BeginCleanup);
		m_Session.SetAbortReason(reason);
		m_Session.SetSmokeState(DZKOTHF_SmokeState.RED);
		DZKOTHF_Log.Warning("Event abort requested: " + reason + ".");
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(BeginCleanup, 3000, false);
		return true;
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

	protected void StopCaptureTick()
	{
		m_CaptureTickRunning = false;
		if (GetGame())
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(CaptureTick);
	}

	protected void RemoveScheduledCalls()
	{
		if (!GetGame())
			return;

		m_CaptureTickRunning = false;
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(StartEvent);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(ActivateEvent);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(CaptureTick);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(BeginCleanup);
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
