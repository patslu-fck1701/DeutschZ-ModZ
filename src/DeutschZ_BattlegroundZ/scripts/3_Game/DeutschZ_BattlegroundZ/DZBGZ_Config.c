class DZBGZ_MainConfig
{
	bool Enabled;
	bool AutoSpawnReader;
	float TickSeconds;
	float FirstSignalDelaySeconds;
	float SecondSignalDelaySeconds;
	float ThirdSignalDelaySeconds;
	float RiddleDelaySeconds;
	float ApproachRadius;
	float BattlegroundRadius;
	float ReaderUseDistance;
	float CleanupDelaySeconds;
	string EventName;
	ref array<float> ReaderPosition;
	ref array<float> ReaderOrientation;
	ref array<float> RevealedPingPosition;
	ref array<float> BattlegroundCenter;
	ref array<float> ReaderMarkerPosition;
	ref array<string> AdminUIDs;
	bool DebugCommandsEnabled;
	bool AllowAdminWhenListEmpty;

	void DZBGZ_MainConfig()
	{
		Enabled = true;
		AutoSpawnReader = true;
		TickSeconds = 10.0;
		FirstSignalDelaySeconds = 45.0;
		SecondSignalDelaySeconds = 180.0;
		ThirdSignalDelaySeconds = 180.0;
		RiddleDelaySeconds = 240.0;
		ApproachRadius = 360.0;
		BattlegroundRadius = 135.0;
		ReaderUseDistance = 4.0;
		CleanupDelaySeconds = 1800.0;
		EventName = "DeutschZ BattlegroundZ";
		BattlegroundCenter = DZBGZ_Utils.MakeVectorArray(450.677673, 293.246582, 9431.185547);
		ReaderPosition = DZBGZ_Utils.MakeVectorArray(468.677673, 293.246582, 9443.185547);
		ReaderOrientation = DZBGZ_Utils.MakeVectorArray(294.277313, 0.0, 0.0);
		ReaderMarkerPosition = DZBGZ_Utils.MakeVectorArray(468.677673, 293.246582, 9443.185547);
		RevealedPingPosition = DZBGZ_Utils.MakeVectorArray(8403.467773, 232.038162, 14246.509766);
		AdminUIDs = new array<string>;
		DebugCommandsEnabled = true;
		AllowAdminWhenListEmpty = true;
	}

	vector GetBattlegroundCenter()
	{
		return DZBGZ_Utils.ArrayToVector(BattlegroundCenter);
	}

	vector GetReaderPosition()
	{
		return DZBGZ_Utils.ArrayToVector(ReaderPosition, GetBattlegroundCenter());
	}

	vector GetReaderOrientation()
	{
		return DZBGZ_Utils.ArrayToVector(ReaderOrientation);
	}

	vector GetReaderMarkerPosition()
	{
		return DZBGZ_Utils.ArrayToVector(ReaderMarkerPosition, GetReaderPosition());
	}

	vector GetRevealedPingPosition()
	{
		return DZBGZ_Utils.ArrayToVector(RevealedPingPosition, GetBattlegroundCenter());
	}
}

class DZBGZ_UnlockEntry
{
	string UID;
	string Name;
	bool Revealed;

	void DZBGZ_UnlockEntry()
	{
		UID = "";
		Name = "";
		Revealed = false;
	}
}

class DZBGZ_UnlockStore
{
	ref array<ref DZBGZ_UnlockEntry> Entries;

	void DZBGZ_UnlockStore()
	{
		Entries = new array<ref DZBGZ_UnlockEntry>;
	}
}

class DZBGZ_Config
{
	static ref DZBGZ_MainConfig LoadMain()
	{
		DZBGZ_ProfilePaths.Ensure();

		ref DZBGZ_MainConfig config = new DZBGZ_MainConfig;
		string errorMessage;
		if (!FileExist(DZBGZ_PROFILE_CONFIG_JSON))
		{
			if (JsonFileLoader<ref DZBGZ_MainConfig>.SaveFile(DZBGZ_PROFILE_CONFIG_JSON, config, errorMessage))
				DZBGZ_Utils.Log("Created default config " + DZBGZ_PROFILE_CONFIG_JSON);
			else
				DZBGZ_Utils.Warn("Could not create config " + errorMessage);
		}
		else
		{
			if (!JsonFileLoader<ref DZBGZ_MainConfig>.LoadFile(DZBGZ_PROFILE_CONFIG_JSON, config, errorMessage))
				DZBGZ_Utils.Warn("Could not load config, using defaults. " + errorMessage);
		}

		return config;
	}
}
