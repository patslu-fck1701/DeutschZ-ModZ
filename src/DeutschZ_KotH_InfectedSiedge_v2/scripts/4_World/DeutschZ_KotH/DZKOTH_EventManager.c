class DZKOTH_LocationRotationState
{
	string ConfigSignature;
	string LastLocationKey;
	ref array<string> RemainingLocationKeys;

	void DZKOTH_LocationRotationState()
	{
		ConfigSignature = "";
		LastLocationKey = "";
		RemainingLocationKeys = new array<string>;
	}
}

class DZKOTH_EventManager
{
	protected static ref DZKOTH_EventManager s_Instance;
	protected ref DZKOTH_ConfigBundle m_Config;
	protected ref DZKOTH_EventInstance m_Instance;
	protected ref DZKOTH_LocationRotationState m_LocationRotation;
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
		LoadLocationRotation();
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
			locationIndex = SelectAutomaticLocationIndex();

		if (locationIndex < 0 || locationIndex >= m_Config.Locations.Locations.Count())
		{
			DZKOTH_Utils.Error("Could not select an event location from the rotation bag.");
			return false;
		}

		DZKOTH_LocationConfig location = m_Config.Locations.Locations.Get(locationIndex);
		m_Instance = new DZKOTH_EventInstance(m_Config, location);
		if (!m_Instance.Start())
		{
			m_Instance = null;
			return false;
		}

		RecordSelectedLocation(location);
		DZKOTH_Utils.Log("Location rotation selected " + location.Name + " (index " + locationIndex.ToString() + ").");
		return true;
	}

	protected void LoadLocationRotation()
	{
		DZKOTH_ProfilePaths.Ensure();
		m_LocationRotation = new DZKOTH_LocationRotationState;

		if (!FileExist(DZKOTH_Const.PROFILE_LOCATION_ROTATION_JSON))
			return;

		string errorMessage;
		if (!JsonFileLoader<ref DZKOTH_LocationRotationState>.LoadFile(DZKOTH_Const.PROFILE_LOCATION_ROTATION_JSON, m_LocationRotation, errorMessage))
		{
			DZKOTH_Utils.Warn("Could not load location rotation. A fresh shuffle bag will be used. " + errorMessage);
			m_LocationRotation = new DZKOTH_LocationRotationState;
		}

		if (!m_LocationRotation)
			m_LocationRotation = new DZKOTH_LocationRotationState;

		if (!m_LocationRotation.RemainingLocationKeys)
			m_LocationRotation.RemainingLocationKeys = new array<string>;
	}

	protected void SaveLocationRotation()
	{
		if (!m_LocationRotation)
			return;

		string errorMessage;
		if (!JsonFileLoader<ref DZKOTH_LocationRotationState>.SaveFile(DZKOTH_Const.PROFILE_LOCATION_ROTATION_JSON, m_LocationRotation, errorMessage))
			DZKOTH_Utils.Warn("Could not save location rotation. " + errorMessage);
	}

	protected int SelectAutomaticLocationIndex()
	{
		if (!m_LocationRotation)
			m_LocationRotation = new DZKOTH_LocationRotationState;

		if (!m_Config || !m_Config.Locations || !m_Config.Locations.Locations || m_Config.Locations.Locations.Count() == 0)
			return -1;

		ref array<int> candidates = new array<int>;
		ref array<int> repeatSafeCandidates = new array<int>;
		string lastKey = m_LocationRotation.LastLocationKey;

		for (int i = 0; i < m_Config.Locations.Locations.Count(); i++)
		{
			DZKOTH_LocationConfig location = m_Config.Locations.Locations.Get(i);
			if (!location || !location.IsEnabled())
				continue;

			candidates.Insert(i);
			if (GetLocationKey(location) != lastKey)
				repeatSafeCandidates.Insert(i);
		}

		if (candidates.Count() == 0)
			return -1;

		bool repeatAvoided = false;
		array<int> pickFrom = candidates;
		if (repeatSafeCandidates.Count() > 0 && candidates.Count() > 1)
		{
			pickFrom = repeatSafeCandidates;
			repeatAvoided = true;
		}

		int selectedCandidate = Math.RandomInt(0, pickFrom.Count());
		int selectedIndex = pickFrom.Get(selectedCandidate);
		DZKOTH_LocationConfig selectedLocation = m_Config.Locations.Locations.Get(selectedIndex);

		DZKOTH_Utils.Log("LOCATION CANDIDATES: " + candidates.Count().ToString());
		DZKOTH_Utils.Log("LAST LOCATION: " + lastKey);
		if (selectedLocation)
			DZKOTH_Utils.Log("SELECTED LOCATION: " + selectedLocation.Name + " / " + selectedIndex.ToString());
		else
			DZKOTH_Utils.Log("SELECTED LOCATION: <invalid> / " + selectedIndex.ToString());
		if (repeatAvoided)
			DZKOTH_Utils.Log("REPEAT AVOIDED: JA");
		else
			DZKOTH_Utils.Log("REPEAT AVOIDED: NEIN");

		return selectedIndex;
	}

	protected void EnsureRotationBag()
	{
		if (!m_LocationRotation)
			m_LocationRotation = new DZKOTH_LocationRotationState;

		if (!m_LocationRotation.RemainingLocationKeys)
			m_LocationRotation.RemainingLocationKeys = new array<string>;

		string signature = BuildLocationConfigSignature();
		if (m_LocationRotation.ConfigSignature != signature)
		{
			m_LocationRotation.ConfigSignature = signature;
			m_LocationRotation.RemainingLocationKeys.Clear();
			DZKOTH_Utils.Log("Location configuration changed. Rotation bag reset.");
		}
		else
		{
			SanitizeRotationBag();
		}

		if (m_LocationRotation.RemainingLocationKeys.Count() > 0)
			return;

		foreach (DZKOTH_LocationConfig location: m_Config.Locations.Locations)
		{
			if (location)
				m_LocationRotation.RemainingLocationKeys.Insert(GetLocationKey(location));
		}

		ShuffleLocationKeys(m_LocationRotation.RemainingLocationKeys);
		AvoidImmediateLocationRepeat(m_LocationRotation.RemainingLocationKeys);
		DZKOTH_Utils.Log("Location rotation bag refilled with " + m_LocationRotation.RemainingLocationKeys.Count().ToString() + " location(s).");
	}

	protected void SanitizeRotationBag()
	{
		if (!m_LocationRotation || !m_LocationRotation.RemainingLocationKeys)
			return;

		for (int i = m_LocationRotation.RemainingLocationKeys.Count() - 1; i >= 0; i--)
		{
			string key = m_LocationRotation.RemainingLocationKeys.Get(i);
			bool removeKey = key == "" || FindLocationIndexByKey(key) < 0;
			if (!removeKey)
			{
				for (int earlierIndex = 0; earlierIndex < i; earlierIndex++)
				{
					if (m_LocationRotation.RemainingLocationKeys.Get(earlierIndex) == key)
					{
						removeKey = true;
						break;
					}
				}
			}

			if (removeKey)
				m_LocationRotation.RemainingLocationKeys.Remove(i);
		}
	}

	protected void RecordSelectedLocation(DZKOTH_LocationConfig location)
	{
		if (!location)
			return;

		string key = GetLocationKey(location);

		m_LocationRotation.LastLocationKey = key;
		if (m_LocationRotation.RemainingLocationKeys)
			m_LocationRotation.RemainingLocationKeys.Clear();
		SaveLocationRotation();
	}

	protected void ShuffleLocationKeys(array<string> keys)
	{
		if (!keys)
			return;

		for (int i = keys.Count() - 1; i > 0; i--)
		{
			int swapIndex = Math.RandomIntInclusive(0, i);
			string current = keys.Get(i);
			keys.Set(i, keys.Get(swapIndex));
			keys.Set(swapIndex, current);
		}
	}

	protected void AvoidImmediateLocationRepeat(array<string> keys)
	{
		if (!keys || keys.Count() < 2 || !m_LocationRotation || m_LocationRotation.LastLocationKey == "")
			return;

		int nextIndex = keys.Count() - 1;
		if (keys.Get(nextIndex) != m_LocationRotation.LastLocationKey)
			return;

		for (int i = 0; i < nextIndex; i++)
		{
			if (keys.Get(i) == m_LocationRotation.LastLocationKey)
				continue;

			string replacement = keys.Get(i);
			keys.Set(i, keys.Get(nextIndex));
			keys.Set(nextIndex, replacement);
			return;
		}
	}

	protected int FindLocationIndexByKey(string key)
	{
		if (!m_Config || !m_Config.Locations || !m_Config.Locations.Locations)
			return -1;

		for (int i = 0; i < m_Config.Locations.Locations.Count(); i++)
		{
			DZKOTH_LocationConfig location = m_Config.Locations.Locations.Get(i);
			if (location && GetLocationKey(location) == key)
				return i;
		}

		return -1;
	}

	protected string BuildLocationConfigSignature()
	{
		string signature = "";
		if (!m_Config || !m_Config.Locations || !m_Config.Locations.Locations)
			return signature;

		foreach (DZKOTH_LocationConfig location: m_Config.Locations.Locations)
		{
			if (location)
				signature = signature + "[" + GetLocationKey(location) + "]";
		}

		return signature;
	}

	protected string GetLocationKey(DZKOTH_LocationConfig location)
	{
		if (!location)
			return "";

		vector position = location.GetPosition();
		return location.Name + "|" + position[0].ToString() + "|" + position[2].ToString();
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
