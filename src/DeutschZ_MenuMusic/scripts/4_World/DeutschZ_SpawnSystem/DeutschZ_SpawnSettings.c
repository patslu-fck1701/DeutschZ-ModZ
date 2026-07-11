class DeutschZ_SpawnModeSettings
{
	bool RANDOM;
	bool COAST;
	bool INLAND;
	bool TRADER_NEAR;
	bool SAFE_RANDOM;
	bool ADMIN_TEST;
	bool DISABLED_KEEP_GEAR;

	void DeutschZ_SpawnModeSettings()
	{
		RANDOM = true;
		COAST = true;
		INLAND = false;
		TRADER_NEAR = false;
		SAFE_RANDOM = true;
		ADMIN_TEST = false;
		DISABLED_KEEP_GEAR = false;
	}

	bool IsEnabled(string mode)
	{
		if (mode == DZSPAWN_Mode.RANDOM)
			return RANDOM;
		if (mode == DZSPAWN_Mode.COAST)
			return COAST;
		if (mode == DZSPAWN_Mode.INLAND)
			return INLAND;
		if (mode == DZSPAWN_Mode.TRADER_NEAR)
			return TRADER_NEAR;
		if (mode == DZSPAWN_Mode.SAFE_RANDOM)
			return SAFE_RANDOM;
		if (mode == DZSPAWN_Mode.ADMIN_TEST)
			return ADMIN_TEST;
		if (mode == DZSPAWN_Mode.DISABLED_KEEP_GEAR)
			return DISABLED_KEEP_GEAR;
		return false;
	}
}

class DeutschZ_SpawnSettings
{
	int Version;
	bool EnableSpawnSystem;
	bool EnableDeathScreenButtons;
	string DefaultSpawnMode;
	int RespawnCooldownSeconds;
	bool AllowKeepGear;
	bool AllowLoadoutSelection;
	bool BlockWaterSpawns;
	bool BlockSteepTerrain;
	float MaxTerrainSlope;
	float MinDistanceToPlayers;
	float MinDistanceToTrader;
	float MinDistanceToEvents;
	float MinDistanceToSafeZones;
	float SpawnSearchRadius;
	float SpawnHeightOffset;
	bool DebugLogs;
	ref DeutschZ_SpawnModeSettings SpawnModes;
	ref array<vector> TraderPositions;
	ref array<vector> EventPositions;
	ref array<vector> SafeZonePositions;

	void DeutschZ_SpawnSettings()
	{
		Version = 1;
		EnableSpawnSystem = true;
		EnableDeathScreenButtons = true;
		DefaultSpawnMode = DZSPAWN_Mode.SAFE_RANDOM;
		RespawnCooldownSeconds = 120;
		AllowKeepGear = false;
		AllowLoadoutSelection = true;
		BlockWaterSpawns = true;
		BlockSteepTerrain = true;
		MaxTerrainSlope = 35.0;
		MinDistanceToPlayers = 250.0;
		MinDistanceToTrader = 350.0;
		MinDistanceToEvents = 750.0;
		MinDistanceToSafeZones = 300.0;
		SpawnSearchRadius = 12.0;
		SpawnHeightOffset = 0.25;
		DebugLogs = true;
		SpawnModes = new DeutschZ_SpawnModeSettings;
		TraderPositions = new array<vector>;
		EventPositions = new array<vector>;
		SafeZonePositions = new array<vector>;

		TraderPositions.Insert("3708 402 5976");
		TraderPositions.Insert("1143 7 2423");
		TraderPositions.Insert("4971 11 2436");
		SafeZonePositions.Insert("3708 402 5976");
		SafeZonePositions.Insert("4971 11 2436");
		AddEventPositions();
	}

	protected void AddEventPositions()
	{
		EventPositions.Insert("8000 339 14637");
		EventPositions.Insert("4552 318 8351");
		EventPositions.Insert("3781 361 14802");
		EventPositions.Insert("2935 271 9680");
		EventPositions.Insert("1101 183 7196");
		EventPositions.Insert("4906 338 9430");
		EventPositions.Insert("315 295 9340");
		EventPositions.Insert("1246 173 5848");
		EventPositions.Insert("9707 299 8891");
		EventPositions.Insert("12139 140 12658");
		EventPositions.Insert("3630 334 6922");
	}

	static ref DeutschZ_SpawnSettings Load()
	{
		DeutschZ_SpawnStorage.EnsureDirectories();
		ref DeutschZ_SpawnSettings settings = new DeutschZ_SpawnSettings;
		string error;
		if (!FileExist(DZSPAWN_Paths.SETTINGS))
		{
			JsonFileLoader<ref DeutschZ_SpawnSettings>.SaveFile(DZSPAWN_Paths.SETTINGS, settings, error);
			Print("[DeutschZ SpawnSystem] Created SpawnSettings.json");
			return settings;
		}

		if (!JsonFileLoader<ref DeutschZ_SpawnSettings>.LoadFile(DZSPAWN_Paths.SETTINGS, settings, error))
			Print("[DeutschZ SpawnSystem][ERROR] SpawnSettings.json invalid, defaults active: " + error);
		else
			Print("[DeutschZ SpawnSystem] Loaded settings");

		return settings;
	}
}

class DeutschZ_SpawnStorage
{
	static void EnsureDirectories()
	{
		if (!FileExist(DZSPAWN_Paths.PROFILE_ROOT))
			MakeDirectory(DZSPAWN_Paths.PROFILE_ROOT);
		if (!FileExist(DZSPAWN_Paths.PROFILE_DIR))
			MakeDirectory(DZSPAWN_Paths.PROFILE_DIR);
	}
}
