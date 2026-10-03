class DZRMZ_Storage
{
	protected static ref array<string> s_WriteBlockedPaths;
	protected static const string VOICE_DIR = "$profile:DeutschZ-System/deutschz_radiomissionz/persistence/voices";

	static string GetOrChooseVoice(string uid, string audioId)
	{
		if (!GetGame() || !GetGame().IsServer() || uid == "" || audioId == "") return "";
		DZRMZ_ProfilePaths.Ensure();
		if (!FileExist(VOICE_DIR)) MakeDirectory(VOICE_DIR);
		string path = VOICE_DIR + "/" + uid + ".json";
		ref DZRMZ_PlayerVoiceState state = new DZRMZ_PlayerVoiceState;
		string errorMessage;
		if (FileExist(path) && (!JsonFileLoader<ref DZRMZ_PlayerVoiceState>.LoadFile(path, state, errorMessage) || !state))
		{
			DZRMZ_Log.Error("Stimmwahl konnte nicht geladen werden; vorhandene Datei bleibt unangetastet: " + errorMessage);
			return "";
		}
		string variant = state.GetVariant(audioId);
		if (variant == "MALE" || variant == "FEMALE") return variant;
		variant = "MALE";
		if (Math.RandomInt(0, 2) == 1) variant = "FEMALE";
		state.SetVariant(audioId, variant);
		if (!JsonFileLoader<ref DZRMZ_PlayerVoiceState>.SaveFile(path, state, errorMessage))
		{
			DZRMZ_Log.Error("Stimmwahl konnte nicht gespeichert werden; Audio wird nicht abgespielt: " + errorMessage);
			return "";
		}
		return variant;
	}

	static ref DZRMZ_Settings LoadSettings()
	{
		DZRMZ_ProfilePaths.Ensure();
		ref DZRMZ_Settings settings = new DZRMZ_Settings;
		string errorMessage;

		if (FileExist(DZRMZ_SETTINGS_PATH))
		{
			if (!JsonFileLoader<ref DZRMZ_Settings>.LoadFile(DZRMZ_SETTINGS_PATH, settings, errorMessage) || !settings)
			{
				PreserveInvalidOrBlockWrites(DZRMZ_SETTINGS_PATH);
				DZRMZ_Log.Error("RadioMissionConfig.json ist ungueltig. Sichere Standardwerte werden im Speicher verwendet: " + errorMessage);
				settings = new DZRMZ_Settings;
			}
		}

		settings.Validate();
		SaveSettings(settings);
		return settings;
	}

	static ref DZRMZ_MissionCatalog LoadMissions()
	{
		DZRMZ_ProfilePaths.Ensure();
		ref DZRMZ_MissionCatalog catalog = new DZRMZ_MissionCatalog;
		string errorMessage;

		if (FileExist(DZRMZ_MISSIONS_PATH))
		{
			if (!JsonFileLoader<ref DZRMZ_MissionCatalog>.LoadFile(DZRMZ_MISSIONS_PATH, catalog, errorMessage) || !catalog || !catalog.IsValid())
			{
				PreserveInvalidOrBlockWrites(DZRMZ_MISSIONS_PATH);
				DZRMZ_Log.Error("missiondefinitions.json ist ungueltig. Sichere Standardmissionen werden im Speicher erzeugt: " + errorMessage);
				catalog = DZRMZ_DefaultFactory.CreateMissionCatalog();
			}
		}
		else
		{
			catalog = DZRMZ_DefaultFactory.CreateMissionCatalog();
		}

		if (!catalog || !catalog.IsValid())
			catalog = DZRMZ_DefaultFactory.CreateMissionCatalog();
		int migratedEventEntries = DZRMZ_DefaultFactory.MigrateEventGrid(catalog);
		if (migratedEventEntries > 0)
			DZRMZ_Log.Info(migratedEventEntries.ToString() + " Event-Grid-Eintraege wurden auf Schema 3 migriert.");
		SaveMissions(catalog);
		return catalog;
	}

	static ref DZRMZ_LocationCatalog LoadLocations()
	{
		DZRMZ_ProfilePaths.Ensure();
		ref DZRMZ_LocationCatalog catalog = new DZRMZ_LocationCatalog;
		string errorMessage;

		if (FileExist(DZRMZ_LOCATIONS_PATH))
		{
			if (!JsonFileLoader<ref DZRMZ_LocationCatalog>.LoadFile(DZRMZ_LOCATIONS_PATH, catalog, errorMessage) || !catalog || !catalog.IsValid())
			{
				PreserveInvalidOrBlockWrites(DZRMZ_LOCATIONS_PATH);
				DZRMZ_Log.Error("Locations.json ist ungueltig. Chernarus-Standardorte werden im Speicher erzeugt: " + errorMessage);
				catalog = DZRMZ_DefaultFactory.CreateLocationCatalog();
			}
		}
		else
		{
			catalog = DZRMZ_DefaultFactory.CreateLocationCatalog();
		}

		if (!catalog || !catalog.IsValid())
			catalog = DZRMZ_DefaultFactory.CreateLocationCatalog();
		SaveLocations(catalog);
		return catalog;
	}

	static ref DZRMZ_ActiveMissionRuntime LoadRuntime()
	{
		DZRMZ_ProfilePaths.Ensure();
		ref DZRMZ_ActiveMissionRuntime runtime = new DZRMZ_ActiveMissionRuntime;
		string errorMessage;

		if (FileExist(DZRMZ_RUNTIME_PATH))
		{
			if (!JsonFileLoader<ref DZRMZ_ActiveMissionRuntime>.LoadFile(DZRMZ_RUNTIME_PATH, runtime, errorMessage) || !runtime)
			{
				PreserveInvalidOrBlockWrites(DZRMZ_RUNTIME_PATH);
				DZRMZ_Log.Error("ActiveMission.json ist ungueltig. Laufzeitstatus wird nur im Speicher sicher zurueckgesetzt: " + errorMessage);
				runtime = new DZRMZ_ActiveMissionRuntime;
			}
		}

		ValidateRuntime(runtime);
		SaveRuntime(runtime);
		return runtime;
	}

	static ref DZRMZ_MissionHistory LoadHistory()
	{
		DZRMZ_ProfilePaths.Ensure();
		ref DZRMZ_MissionHistory history = new DZRMZ_MissionHistory;
		string errorMessage;

		if (FileExist(DZRMZ_HISTORY_PATH))
		{
			if (!JsonFileLoader<ref DZRMZ_MissionHistory>.LoadFile(DZRMZ_HISTORY_PATH, history, errorMessage) || !history)
			{
				PreserveInvalidOrBlockWrites(DZRMZ_HISTORY_PATH);
				DZRMZ_Log.Error("MissionHistory.json ist ungueltig. Neue Historie wird im Speicher angelegt: " + errorMessage);
				history = new DZRMZ_MissionHistory;
			}
		}

		history.Validate();
		SaveHistory(history);
		return history;
	}

	static bool SaveSettings(DZRMZ_Settings settings)
	{
		if (IsWriteBlocked(DZRMZ_SETTINGS_PATH))
			return false;
		string errorMessage;
		if (!settings || !JsonFileLoader<ref DZRMZ_Settings>.SaveFile(DZRMZ_SETTINGS_PATH, settings, errorMessage))
		{
			DZRMZ_Log.Error("RadioMissionConfig.json konnte nicht gespeichert werden: " + errorMessage);
			return false;
		}
		return true;
	}

	static bool SaveMissions(DZRMZ_MissionCatalog catalog)
	{
		if (IsWriteBlocked(DZRMZ_MISSIONS_PATH))
			return false;
		string errorMessage;
		if (!catalog || !JsonFileLoader<ref DZRMZ_MissionCatalog>.SaveFile(DZRMZ_MISSIONS_PATH, catalog, errorMessage))
		{
			DZRMZ_Log.Error("MissionDefinitions.json konnte nicht gespeichert werden: " + errorMessage);
			return false;
		}
		return true;
	}

	static bool SaveLocations(DZRMZ_LocationCatalog catalog)
	{
		if (IsWriteBlocked(DZRMZ_LOCATIONS_PATH))
			return false;
		string errorMessage;
		if (!catalog || !JsonFileLoader<ref DZRMZ_LocationCatalog>.SaveFile(DZRMZ_LOCATIONS_PATH, catalog, errorMessage))
		{
			DZRMZ_Log.Error("Locations.json konnte nicht gespeichert werden: " + errorMessage);
			return false;
		}
		return true;
	}

	static bool SaveRuntime(DZRMZ_ActiveMissionRuntime runtime)
	{
		if (IsWriteBlocked(DZRMZ_RUNTIME_PATH))
			return false;
		string errorMessage;
		if (!runtime || !JsonFileLoader<ref DZRMZ_ActiveMissionRuntime>.SaveFile(DZRMZ_RUNTIME_PATH, runtime, errorMessage))
		{
			DZRMZ_Log.Error("ActiveMission.json konnte nicht gespeichert werden: " + errorMessage);
			return false;
		}
		return true;
	}

	static bool SaveHistory(DZRMZ_MissionHistory history)
	{
		if (IsWriteBlocked(DZRMZ_HISTORY_PATH))
			return false;
		string errorMessage;
		if (!history || !JsonFileLoader<ref DZRMZ_MissionHistory>.SaveFile(DZRMZ_HISTORY_PATH, history, errorMessage))
		{
			DZRMZ_Log.Error("MissionHistory.json konnte nicht gespeichert werden: " + errorMessage);
			return false;
		}
		return true;
	}

	static void ResetRuntimeState()
	{
		s_WriteBlockedPaths = null;
	}

	protected static void PreserveInvalidOrBlockWrites(string sourcePath)
	{
		string backupPath = sourcePath + ".invalid";
		if (FileExist(backupPath))
		{
			for (int i = 1; i <= 100; i++)
			{
				string candidate = string.Format("%1.invalid.%2", sourcePath, i);
				if (!FileExist(candidate))
				{
					backupPath = candidate;
					break;
				}
			}
		}

		if (!FileExist(backupPath) && CopyFile(sourcePath, backupPath))
		{
			DZRMZ_Log.Warn("Ungueltige JSON gesichert: " + backupPath);
			return;
		}

		if (!s_WriteBlockedPaths)
			s_WriteBlockedPaths = new array<string>;
		if (s_WriteBlockedPaths.Find(sourcePath) == -1)
			s_WriteBlockedPaths.Insert(sourcePath);
		DZRMZ_Log.Error("Ungueltige JSON konnte nicht gesichert werden. Original bleibt unangetastet und Schreiben ist fuer diesen Lauf gesperrt: " + sourcePath);
	}

	protected static bool IsWriteBlocked(string path)
	{
		return s_WriteBlockedPaths && s_WriteBlockedPaths.Find(path) > -1;
	}

	protected static void ValidateRuntime(DZRMZ_ActiveMissionRuntime runtime)
	{
		if (!runtime)
			return;
		if (runtime.SchemaVersion < 2)
			runtime.SchemaVersion = 2;
		if (runtime.State < DZRMZ_EMissionState.DZRMZ_STATE_IDLE || runtime.State > DZRMZ_EMissionState.DZRMZ_STATE_COMPLETING)
			runtime.Reset();
		if (runtime.SecondsUntilNextMission < 0.0)
			runtime.SecondsUntilNextMission = 0.0;
		if (runtime.MissionSecondsRemaining < 0.0)
			runtime.MissionSecondsRemaining = 0.0;
		if (runtime.BroadcastSecondsRemaining < 0.0)
			runtime.BroadcastSecondsRemaining = 0.0;
		if (runtime.BroadcastLineIndex < 0)
			runtime.BroadcastLineIndex = 0;
		if (runtime.HoldProgressSeconds < 0.0)
			runtime.HoldProgressSeconds = 0.0;
		if (runtime.OwnerMissingSeconds < 0.0)
			runtime.OwnerMissingSeconds = 0.0;
		if (runtime.ThreatWaveIndex < 0) runtime.ThreatWaveIndex = 0;
		if (runtime.ThreatWavePauseSeconds < 0.0) runtime.ThreatWavePauseSeconds = 0.0;
		if (runtime.CompletionId == "")
			runtime.CompletionRecorded = false;
		if (!runtime.RewardPayoutCommitted)
		{
			runtime.RewardPayoutFinished = false;
			runtime.RewardItemsGranted = 0;
			runtime.RewardReputationGranted = 0;
		}
		if (runtime.RewardItemsGranted < -1)
			runtime.RewardItemsGranted = -1;
	}
}

