class DZKOTHF_EventController
{
	protected static ref DZKOTHF_EventController s_Instance;
	protected ref DZKOTHF_EventSession m_Session;
	protected ref DZKOTHF_Settings m_Settings;
	protected bool m_Initialized;
	protected bool m_CaptureTickRunning;
	protected ref map<string, bool> m_ProgressRecipients;
	protected ref map<string, bool> m_MusicRecipients;
	protected string m_CurrentMusicTrack;
	protected int m_LastMusicTrackIndex;
	protected ref array<string> m_MusicTracks;
	protected ref DZKOTHF_LocationsSettings m_Locations;
	protected ref DZKOTHF_LocationHistory m_LocationHistory;

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
		m_ProgressRecipients = new map<string, bool>;
		m_MusicRecipients = new map<string, bool>;
		m_LastMusicTrackIndex = -1;
		m_MusicTracks = new array<string>;
		for (int i = 1; i <= 8; i++) m_MusicTracks.Insert(string.Format("DZKOTHF_Music%1_SoundSet", i.ToStringLen(2)));
	}

	void InitServer()
	{
		if (m_Initialized || !GetGame() || !GetGame().IsServer())
			return;

		m_Settings = DZKOTHF_SettingsLoader.Load();
		m_Locations = DZKOTHF_LocationLoader.LoadLocations();
		m_LocationHistory = DZKOTHF_LocationLoader.LoadHistory();
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
		StopMusicForAll();
		if (m_Session)
		{
			HideProgressForAll("MISSION ENDE", 0.0);
			m_Session.SetAbortReason("Mission shutdown");
			m_Session.CleanupWorldObjects();
			m_Session.CleanupRewardCrate("mission shutdown");
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

		m_Session.CleanupRewardCrate("new event start duplicate protection");
		m_ProgressRecipients.Clear();
		StopMusicForAll();
		if (!SelectRandomLocation())
		{
			DZKOTHF_Log.Error("Start rejected: no valid random event location is available.");
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
		DZKOTHF_ClientBridge.BroadcastMarker(m_Settings, true, m_Session.GetCenter(), "ANGEKUENDIGT");
		if (m_Settings.ShowProgressDuringAnnouncement)
			DZKOTHF_ClientBridge.BroadcastProgress(m_Settings, true, "ANGEKUENDIGT", 0.0);
		DZKOTHF_ClientBridge.BroadcastNotify(m_Settings, "Ein KotHZ-Event wurde angekuendigt.");
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ActivateEvent, m_Settings.AnnounceSeconds * 1000, false);
		return true;
	}

	bool ActivateEvent()
	{
		if (!TransitionTo(DZKOTHF_EventState.ACTIVE))
			return false;

		m_Session.SetSmokeState(DZKOTHF_SmokeState.GREEN);
		DZKOTHF_ClientBridge.BroadcastMarker(m_Settings, true, m_Session.GetCenter(), "AKTIV");
		SyncProgressForPlayers("AKTIV - WARTE AUF SPIELER", m_Session.GetCaptureProgress());
		DZKOTHF_ClientBridge.BroadcastNotify(m_Settings, "Die Capture-Phase ist aktiv.");
		SelectMusicTrack();
		SyncMusicForPlayers();
		m_CaptureTickRunning = true;
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SpawnEnemyBlock, m_Settings.SpawnDelaySeconds * 1000, false);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CaptureTick, m_Settings.CaptureTickMilliseconds, false);
		return true;
	}

	protected void SpawnEnemyBlock()
	{
		if (GetState() != DZKOTHF_EventState.ACTIVE || !m_Session)
			return;

		m_Session.SpawnEnemies(m_Settings);
	}

	protected void CaptureTick()
	{
		if (!m_CaptureTickRunning || GetState() != DZKOTHF_EventState.ACTIVE || !GetGame())
			return;

		array<PlayerBase> capturePlayers = new array<PlayerBase>;
		int playerCount = GetAlivePlayersInRadius(capturePlayers);
		int captureSideCount = CountCaptureSides(capturePlayers);
		m_Session.SetPlayerCount(playerCount);
		string progressStatus = "AKTIV";
		if (captureSideCount > 1)
		{
			m_Session.SetSmokeState(DZKOTHF_SmokeState.RED);
			progressStatus = "PAUSIERT - UMKAEMPFT";
		}
		else
		{
			m_Session.SetSmokeState(DZKOTHF_SmokeState.GREEN);
			if (playerCount == 0)
				progressStatus = "PAUSIERT - KEIN SPIELER";
			if (playerCount > 0 && captureSideCount == 1)
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

		SyncProgressForPlayers(progressStatus, m_Session.GetCaptureProgress());
		SyncMusicForPlayers();

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CaptureTick, m_Settings.CaptureTickMilliseconds, false);
	}

	protected int GetAlivePlayersInRadius(array<PlayerBase> capturePlayers)
	{
		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		int playerCount = 0;
		foreach (Man man: players)
		{
			PlayerBase player = PlayerBase.Cast(man);
			if (!player || !player.IsAlive())
				continue;

			if (IsPlayerInCaptureRadius(player))
			{
				playerCount++;
				capturePlayers.Insert(player);
			}
		}

		return playerCount;
	}

	protected int CountCaptureSides(array<PlayerBase> capturePlayers)
	{
		array<string> sideKeys = new array<string>;
		foreach (PlayerBase player: capturePlayers)
		{
			string sideKey = DZKOTHF_ExpansionBridge.GetCaptureSideKey(player, m_Settings);
			if (sideKey != "" && sideKeys.Find(sideKey) == -1)
				sideKeys.Insert(sideKey);
		}

		return sideKeys.Count();
	}

	bool CompleteCapture()
	{
		StopCaptureTick();
		if (!TransitionTo(DZKOTHF_EventState.CAPTURE_COMPLETE))
			return false;

		m_Session.SetCaptureProgress(1.0);
		m_Session.SetSmokeState(DZKOTHF_SmokeState.WHITE);
		DZKOTHF_ClientBridge.BroadcastMarker(m_Settings, false, vector.Zero, "");
		StopMusicForAll();
		HideProgressForAll("ABGESCHLOSSEN", 1.0);
		DZKOTHF_ClientBridge.BroadcastNotify(m_Settings, "Capture erfolgreich abgeschlossen.");
		if (!m_Session.SpawnRewardCrate(m_Settings))
			DZKOTHF_Log.Error("Capture completed but reward crate creation failed.");
		if (m_Settings.FireworkEnabled && !m_Session.SpawnWinFirework(m_Settings))
			DZKOTHF_Log.Error("Capture completed but win firework creation failed.");

		TransitionTo(DZKOTHF_EventState.REWARD);
		DZKOTHF_ClientBridge.BroadcastNotify(m_Settings, "Die Belohnungskiste wurde freigegeben.");
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CleanupRewardCrate, m_Settings.RewardLifetimeMinutes * 60000, false);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(BeginCleanup, m_Settings.CompletionCleanupDelaySeconds * 1000, false);
		return true;
	}

	protected void CleanupRewardCrate()
	{
		if (m_Session)
			m_Session.CleanupRewardCrate("configured lifetime expired");
	}

	bool BeginCleanup()
	{
		RemoveActiveEventCalls();

		if (!TransitionTo(DZKOTHF_EventState.CLEANUP))
			return false;

		DZKOTHF_ClientBridge.BroadcastMarker(m_Settings, false, vector.Zero, "");
		StopMusicForAll();
		HideProgressForAll("CLEANUP", 0.0);
		DZKOTHF_ClientBridge.BroadcastNotify(m_Settings, "KotHZ-Cleanup abgeschlossen.");
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
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(SpawnEnemyBlock);
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

	DZKOTHF_Settings GetSettings()
	{
		return m_Settings;
	}

	vector GetCenter()
	{
		if (!m_Session)
			return vector.Zero;
		return m_Session.GetCenter();
	}

	float GetCaptureRadius()
	{
		if (!m_Session)
			return 0.0;
		return m_Session.GetCaptureRadius();
	}

	float GetCaptureProgress()
	{
		if (!m_Session)
			return 0.0;
		return m_Session.GetCaptureProgress();
	}

	int GetEnemyCount()
	{
		if (!m_Session)
			return 0;
		return m_Session.GetEnemyCount();
	}

	bool HasRewardCrate()
	{
		return m_Session && m_Session.HasRewardCrate();
	}

	void SyncPlayer(PlayerBase player)
	{
		if (!player || !m_Settings || !m_Session)
			return;

		bool markerVisible = GetState() == DZKOTHF_EventState.ANNOUNCED || GetState() == DZKOTHF_EventState.ACTIVE;
		DZKOTHF_ClientBridge.SendMarker(player, m_Settings, markerVisible, m_Session.GetCenter(), GetStateName());

		bool progressVisible = false;
		if (GetState() == DZKOTHF_EventState.ACTIVE)
			progressVisible = IsPlayerInCaptureRadius(player);
		else if (GetState() == DZKOTHF_EventState.ANNOUNCED)
			progressVisible = m_Settings.ShowProgressDuringAnnouncement;
		DZKOTHF_ClientBridge.SendProgress(player, m_Settings, progressVisible, GetStateName(), m_Session.GetCaptureProgress());
		SetProgressRecipient(player, progressVisible);
		SyncMusicForPlayer(player);
	}

	void OnPlayerDisconnect(PlayerBase player)
	{
		if (!player || !player.GetIdentity())
			return;
		string playerId = player.GetIdentity().GetPlainId();
		m_ProgressRecipients.Remove(playerId);
		m_MusicRecipients.Remove(playerId);
	}

	protected bool IsPlayerInCaptureRadius(PlayerBase player)
	{
		return player && player.IsAlive() && m_Session && vector.Distance(player.GetPosition(), m_Session.GetCenter()) <= m_Session.GetCaptureRadius();
	}

	protected void SyncProgressForPlayers(string status, float progress)
	{
		if (!GetGame() || !m_Settings || !m_Session)
			return;

		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		foreach (Man man: players)
		{
			PlayerBase player = PlayerBase.Cast(man);
			if (!player || !player.GetIdentity())
				continue;

			bool visible = IsPlayerInCaptureRadius(player);
			if (visible)
			{
				DZKOTHF_ClientBridge.SendProgress(player, m_Settings, true, status, progress);
				SetProgressRecipient(player, true);
			}
			else if (IsProgressRecipient(player))
			{
				DZKOTHF_ClientBridge.SendProgress(player, m_Settings, false, status, progress);
				SetProgressRecipient(player, false);
			}
		}
	}

	protected void HideProgressForAll(string status, float progress)
	{
		if (!GetGame() || !m_Settings)
			return;

		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		foreach (Man man: players)
			DZKOTHF_ClientBridge.SendProgress(PlayerBase.Cast(man), m_Settings, false, status, progress);
		m_ProgressRecipients.Clear();
	}

	protected bool IsProgressRecipient(PlayerBase player)
	{
		if (!player || !player.GetIdentity())
			return false;
		return m_ProgressRecipients.Contains(player.GetIdentity().GetPlainId());
	}

	protected void SetProgressRecipient(PlayerBase player, bool visible)
	{
		if (!player || !player.GetIdentity())
			return;
		string playerId = player.GetIdentity().GetPlainId();
		if (visible)
			m_ProgressRecipients.Set(playerId, true);
		else
			m_ProgressRecipients.Remove(playerId);
	}

	protected void SelectMusicTrack()
	{
		m_CurrentMusicTrack = "";
		if (!m_MusicTracks || m_MusicTracks.Count() == 0)
			return;

		int count = m_MusicTracks.Count();
		int selectedIndex = Math.RandomInt(0, count);
		if (count > 1 && selectedIndex == m_LastMusicTrackIndex)
			selectedIndex = (selectedIndex + 1 + Math.RandomInt(0, count - 1)) % count;

		m_LastMusicTrackIndex = selectedIndex;
		m_CurrentMusicTrack = m_MusicTracks[selectedIndex];
		DZKOTHF_Log.Info("Music selected: index=" + selectedIndex.ToString() + " soundSet=" + m_CurrentMusicTrack + " volume=" + m_Settings.MusicVolume.ToString() + " max=0.45.");
	}

	protected bool SelectRandomLocation()
	{
		if (!m_Locations || !m_Locations.Locations || !m_Settings)
			return false;

		ref array<int> candidates = new array<int>;
		for (int index = 0; index < m_Locations.Locations.Count(); index++)
		{
			DZKOTHF_LocationSetting candidate = m_Locations.Locations[index];
			if (candidate && candidate.IsValid())
				candidates.Insert(index);
		}

		DZKOTHF_Log.Info("LOCATION CANDIDATES: " + candidates.Count().ToString());
		if (candidates.Count() == 0)
			return false;

		if (!m_LocationHistory)
			m_LocationHistory = new DZKOTHF_LocationHistory;
		DZKOTHF_Log.Info("LAST LOCATION: " + m_LocationHistory.LastName + "/" + m_LocationHistory.LastIndex.ToString());

		int candidatePosition = Math.RandomInt(0, candidates.Count());
		int selectedIndex = candidates[candidatePosition];
		bool repeatAvoided = false;
		if (candidates.Count() > 1 && selectedIndex == m_LocationHistory.LastIndex)
		{
			int shift = Math.RandomInt(1, candidates.Count());
			candidatePosition = (candidatePosition + shift) % candidates.Count();
			selectedIndex = candidates[candidatePosition];
			repeatAvoided = true;
		}

		DZKOTHF_LocationSetting selected = m_Locations.Locations[selectedIndex];
		vector selectedPosition = selected.GetPosition();
		vector selectedOrientation = selected.GetOrientation();
		m_Settings.EventPosition.Clear();
		m_Settings.EventPosition.Insert(selectedPosition[0]);
		m_Settings.EventPosition.Insert(selectedPosition[1]);
		m_Settings.EventPosition.Insert(selectedPosition[2]);
		m_Settings.EventOrientation.Clear();
		m_Settings.EventOrientation.Insert(selectedOrientation[0]);
		m_Settings.EventOrientation.Insert(selectedOrientation[1]);
		m_Settings.EventOrientation.Insert(selectedOrientation[2]);
		if (selected.CaptureRadius >= 5.0)
			m_Settings.CaptureRadius = Math.Clamp(selected.CaptureRadius, 5.0, 250.0);

		m_LocationHistory.LastIndex = selectedIndex;
		m_LocationHistory.LastName = selected.Name;
		DZKOTHF_LocationLoader.SaveHistory(m_LocationHistory);
		DZKOTHF_Log.Info("SELECTED LOCATION: " + selected.Name + "/" + selectedIndex.ToString());
		if (repeatAvoided)
			DZKOTHF_Log.Info("REPEAT AVOIDED: JA");
		else
			DZKOTHF_Log.Info("REPEAT AVOIDED: NEIN");
		return true;
	}

	protected bool IsMusicPhaseActive()
	{
		if (m_CurrentMusicTrack == "")
			return false;
		return GetState() == DZKOTHF_EventState.ACTIVE;
	}

	protected void SyncMusicForPlayers()
	{
		if (!GetGame() || !m_Settings)
			return;
		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		foreach (Man man: players)
			SyncMusicForPlayer(PlayerBase.Cast(man));
	}

	protected void SyncMusicForPlayer(PlayerBase player)
	{
		if (!player || !player.GetIdentity() || !m_Settings)
			return;
		string playerId = player.GetIdentity().GetPlainId();
		bool shouldPlay = IsMusicPhaseActive() && IsPlayerInCaptureRadius(player);
		bool isRecipient = m_MusicRecipients.Contains(playerId);
		if (shouldPlay && !isRecipient)
		{
			DZKOTHF_ClientBridge.SendMusic(player, m_Settings, true, m_CurrentMusicTrack);
			m_MusicRecipients.Set(playerId, true);
		}
		else if (!shouldPlay && isRecipient)
		{
			DZKOTHF_ClientBridge.SendMusic(player, m_Settings, false, "");
			m_MusicRecipients.Remove(playerId);
		}
	}

	protected void StopMusicForAll()
	{
		if (GetGame() && m_Settings)
		{
			array<Man> players = new array<Man>;
			GetGame().GetPlayers(players);
			foreach (Man man: players)
			{
				PlayerBase player = PlayerBase.Cast(man);
				if (player && player.GetIdentity() && m_MusicRecipients.Contains(player.GetIdentity().GetPlainId()))
					DZKOTHF_ClientBridge.SendMusic(player, m_Settings, false, "");
			}
		}
		m_MusicRecipients.Clear();
		m_CurrentMusicTrack = "";
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

	protected void RemoveActiveEventCalls()
	{
		StopCaptureTick();
		if (!GetGame())
			return;

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(StartEvent);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(ActivateEvent);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(SpawnEnemyBlock);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(BeginCleanup);
	}

	protected void RemoveScheduledCalls()
	{
		if (!GetGame())
			return;

		RemoveActiveEventCalls();
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(CleanupRewardCrate);
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
