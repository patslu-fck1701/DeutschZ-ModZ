class DZKOTH_MainConfig
{
	int Enabled;
	int DebugMode;
	bool UseExpansionNotify;
	bool UseExpansionMarkers;
	string EventName;
	float CaptureRadius;
	int CaptureTimeSeconds;
	int ChestActionDurationSeconds;
	int ZombieCount;
	float ZombieSpawnRadius;
	string MarkerName;
	bool EnemyPlayersBlockCapture;
	bool ProgressLossWhenEmpty;
	float ProgressLossPerSecond;
	int RequirePlayersInsideZone;
	bool AllowMultiplePlayersToSpeedUpCapture;
	int EventIntervalMinutes;
	int EventDurationMinutes;
	int EventCooldownMinutes;
	int CleanupDelayMinutes;
	float TickSeconds;
	float SpawnMinDistance;
	float SpawnMaxDistance;
	float BossWarningSeconds;
	float BossHealth;
	float BossDamageMultiplier;
	float KeycardChancePercent;
	bool GlobalKeycardAnnouncement;
	bool PermanentTracking;
	bool DebugCommandsEnabled;
	bool RequireTerminalHackBeforeCapture;
	int TerminalHackSeconds;
	bool TerminalHackDecayEnabled;
	float TerminalHackDecayPerSecond;
	ref array<string> AdminUIDs;

	void DZKOTH_MainConfig()
	{
		Enabled = 1;
		DebugMode = 1;
		UseExpansionNotify = true;
		UseExpansionMarkers = true;
		EventName = "DeutschZ KotH";
		CaptureRadius = 35.0;
		CaptureTimeSeconds = 300;
		ChestActionDurationSeconds = 60;
		ZombieCount = 5;
		ZombieSpawnRadius = 25.0;
		MarkerName = "DeutschZ KotH";
		EnemyPlayersBlockCapture = true;
		ProgressLossWhenEmpty = false;
		ProgressLossPerSecond = 0.25;
		RequirePlayersInsideZone = 1;
		AllowMultiplePlayersToSpeedUpCapture = false;
		EventIntervalMinutes = 60;
		EventDurationMinutes = 45;
		EventCooldownMinutes = 60;
		CleanupDelayMinutes = 5;
		TickSeconds = 1.0;
		SpawnMinDistance = 8.0;
		SpawnMaxDistance = 25.0;
		BossWarningSeconds = 4.0;
		BossHealth = 7500.0;
		BossDamageMultiplier = 8.0;
		KeycardChancePercent = 100.0;
		GlobalKeycardAnnouncement = true;
		PermanentTracking = false;
		DebugCommandsEnabled = true;
		RequireTerminalHackBeforeCapture = true;
		TerminalHackSeconds = 60;
		TerminalHackDecayEnabled = false;
		TerminalHackDecayPerSecond = 0.5;
		AdminUIDs = new array<string>;
	}
}

class DZKOTH_LocationConfig
{
	int Enabled;
	string Name;
	ref array<float> Center;
	ref array<float> Position;
	float Radius;
	ref array<float> FlagPosition;
	ref array<float> FlagOrientation;
	ref array<float> BossSpawnPosition;
	ref array<float> RewardCratePosition;
	ref array<float> RewardCrateOrientation;
	ref array<float> ChestPosition;
	ref array<float> ChestOrientation;
	ref array<float> TerminalPosition;

	void DZKOTH_LocationConfig()
	{
		Enabled = 1;
		Name = "Vybor Airfield";
		Radius = 35.0;
		Center = DZKOTH_Utils.MakeVectorArray(4514.432129, 339.373108, 10285.067383);
		Position = DZKOTH_Utils.MakeVectorArray(4514.432129, 339.373108, 10285.067383);
		FlagPosition = DZKOTH_Utils.MakeVectorArray(4514.432129, 339.373108, 10285.067383);
		FlagOrientation = DZKOTH_Utils.MakeVectorArray(143.979202, 0.0, 0.0);
		BossSpawnPosition = DZKOTH_Utils.MakeVectorArray(4527.5, 0.0, 10272.5);
		RewardCratePosition = DZKOTH_Utils.MakeVectorArray(4517.643555, 339.332092, 10288.263672);
		RewardCrateOrientation = DZKOTH_Utils.MakeVectorArray(-30.634066, 0.0, 0.0);
		ChestPosition = DZKOTH_Utils.MakeVectorArray(4517.643555, 339.332092, 10288.263672);
		ChestOrientation = DZKOTH_Utils.MakeVectorArray(-30.634066, 0.0, 0.0);
		TerminalPosition = DZKOTH_Utils.MakeVectorArray(4517.643555, 339.332092, 10288.263672);
	}

	bool IsEnabled()
	{
		return Enabled != 0;
	}

	vector GetPosition()
	{
		if (Center && Center.Count() >= 3)
			return DZKOTH_Utils.ArrayToVector(Center);

		return DZKOTH_Utils.ArrayToVector(Position);
	}

	vector GetFlagPosition()
	{
		return DZKOTH_Utils.ArrayToVector(FlagPosition, GetPosition());
	}

	vector GetFlagOrientation()
	{
		return DZKOTH_Utils.ArrayToVector(FlagOrientation, "0 0 0");
	}

	vector GetBossSpawnPosition()
	{
		return DZKOTH_Utils.ArrayToVector(BossSpawnPosition, GetPosition());
	}

	vector GetRewardCratePosition()
	{
		if (ChestPosition && ChestPosition.Count() >= 3)
			return DZKOTH_Utils.ArrayToVector(ChestPosition, GetPosition());

		return DZKOTH_Utils.ArrayToVector(RewardCratePosition, GetPosition());
	}

	vector GetRewardCrateOrientation()
	{
		if (ChestOrientation && ChestOrientation.Count() >= 3)
			return DZKOTH_Utils.ArrayToVector(ChestOrientation, "0 0 0");

		return DZKOTH_Utils.ArrayToVector(RewardCrateOrientation, "0 0 0");
	}

	vector GetTerminalPosition()
	{
		if (ChestPosition && ChestPosition.Count() >= 3)
			return DZKOTH_Utils.ArrayToVector(ChestPosition, GetFlagPosition() + "2 0 2");

		if (TerminalPosition && TerminalPosition.Count() >= 3)
			return DZKOTH_Utils.ArrayToVector(TerminalPosition, GetFlagPosition() + "2 0 2");

		return GetRewardCratePosition();
	}
}

class DZKOTH_LocationsConfig
{
	ref array<ref DZKOTH_LocationConfig> Locations;

	void DZKOTH_LocationsConfig()
	{
		Locations = new array<ref DZKOTH_LocationConfig>;
		Locations.Insert(new DZKOTH_LocationConfig);
	}
}

class DZKOTH_WaveConfig
{
	float TriggerProgress;
	int InfectedCountMin;
	int InfectedCountMax;
	float HealthMultiplier;
	float DamageMultiplier;
	string VisualEyes;
	bool DisableRunning;
	ref array<string> Types;

	void DZKOTH_WaveConfig()
	{
		TriggerProgress = 1.0;
		InfectedCountMin = 5;
		InfectedCountMax = 5;
		HealthMultiplier = 1.0;
		DamageMultiplier = 1.0;
		VisualEyes = "";
		DisableRunning = false;
		Types = new array<string>;
		Types.Insert("ZmbM_SoldierNormal_Base");
		Types.Insert("ZmbM_PatrolNormal_Autumn");
		Types.Insert("ZmbM_PatrolNormal_Flat");
	}
}

class DZKOTH_WavesConfig
{
	ref DZKOTH_WaveConfig WaveOne;
	ref DZKOTH_WaveConfig WaveTwo;
	ref DZKOTH_WaveConfig WaveThree;

	void DZKOTH_WavesConfig()
	{
		WaveOne = new DZKOTH_WaveConfig;

		WaveTwo = new DZKOTH_WaveConfig;
		WaveTwo.TriggerProgress = 33.0;
		WaveTwo.InfectedCountMin = 3;
		WaveTwo.InfectedCountMax = 3;
		WaveTwo.HealthMultiplier = 2.0;
		WaveTwo.DamageMultiplier = 2.0;

		WaveThree = new DZKOTH_WaveConfig;
		WaveThree.TriggerProgress = 66.0;
		WaveThree.InfectedCountMin = 3;
		WaveThree.InfectedCountMax = 3;
		WaveThree.HealthMultiplier = 5.0;
		WaveThree.DamageMultiplier = 5.0;
		WaveThree.VisualEyes = "Red";
		WaveThree.DisableRunning = true;
	}
}

class DZKOTH_LootEntry
{
	string ClassName;
	float Chance;
	int Min;
	int Max;

	void DZKOTH_LootEntry()
	{
		ClassName = "";
		Chance = 100.0;
		Min = 1;
		Max = 1;
	}
}

class DZKOTH_LootConfig
{
	ref array<ref DZKOTH_LootEntry> RewardCrateLoot;
	ref array<ref DZKOTH_LootEntry> BossCorpseLoot;

	void DZKOTH_LootConfig()
	{
		RewardCrateLoot = new array<ref DZKOTH_LootEntry>;
		BossCorpseLoot = new array<ref DZKOTH_LootEntry>;

		DZKOTH_LootEntry rifle = new DZKOTH_LootEntry;
		rifle.ClassName = "M4A1";
		rifle.Chance = 100.0;
		RewardCrateLoot.Insert(rifle);

		DZKOTH_LootEntry mag = new DZKOTH_LootEntry;
		mag.ClassName = "Mag_STANAG_30Rnd";
		mag.Chance = 100.0;
		mag.Min = 3;
		mag.Max = 3;
		RewardCrateLoot.Insert(mag);

		DZKOTH_LootEntry ammo = new DZKOTH_LootEntry;
		ammo.ClassName = "Ammo_556x45";
		ammo.Chance = 100.0;
		ammo.Min = 2;
		ammo.Max = 2;
		RewardCrateLoot.Insert(ammo);

		DZKOTH_LootEntry keycard = new DZKOTH_LootEntry;
		keycard.ClassName = DZKOTH_Const.KEYCARD_CLASSNAME;
		keycard.Chance = 100.0;
		BossCorpseLoot.Insert(keycard);
	}
}

class DZKOTH_ProfileLootEntry
{
	string ClassName;
	int Quantity;

	void DZKOTH_ProfileLootEntry()
	{
		ClassName = "";
		Quantity = 1;
	}
}

class DZKOTH_ProfileLocation
{
	string Name;
	int Enabled;
	ref array<float> Center;
	ref array<float> FlagPosition;
	ref array<float> FlagOrientation;
	ref array<float> ChestPosition;
	ref array<float> ChestOrientation;

	void DZKOTH_ProfileLocation()
	{
		Name = "Vybor Airfield";
		Enabled = 1;
		Center = DZKOTH_Utils.MakeVectorArray(4514.432129, 339.373108, 10285.067383);
		FlagPosition = DZKOTH_Utils.MakeVectorArray(4514.432129, 339.373108, 10285.067383);
		FlagOrientation = DZKOTH_Utils.MakeVectorArray(143.979202, 0.0, 0.0);
		ChestPosition = DZKOTH_Utils.MakeVectorArray(4517.643555, 339.332092, 10288.263672);
		ChestOrientation = DZKOTH_Utils.MakeVectorArray(-30.634066, 0.0, 0.0);
	}
}

class DZKOTH_ProfileConfig
{
	int Enabled;
	int DebugMode;
	int UseExpansionNotify;
	int UseExpansionMarkers;
	int EventIntervalMinutes;
	int EventDurationMinutes;
	int CaptureDurationSeconds;
	int ChestActionDurationSeconds;
	int ZombieCount;
	float CaptureRadius;
	float ZombieSpawnRadius;
	string MarkerName;
	bool ProgressLossWhenEmpty;
	float ProgressLossPerSecond;
	bool EnemyPlayersBlockCapture;
	ref array<ref DZKOTH_ProfileLocation> Locations;
	ref array<string> ZombieTypes;
	ref array<ref DZKOTH_ProfileLootEntry> RewardLoot;

	void DZKOTH_ProfileConfig()
	{
		Enabled = 1;
		DebugMode = 1;
		UseExpansionNotify = 1;
		UseExpansionMarkers = 1;
		EventIntervalMinutes = 60;
		EventDurationMinutes = 45;
		CaptureDurationSeconds = 300;
		ChestActionDurationSeconds = 60;
		ZombieCount = 5;
		CaptureRadius = 35.0;
		ZombieSpawnRadius = 25.0;
		MarkerName = "DeutschZ KotH";
		ProgressLossWhenEmpty = false;
		ProgressLossPerSecond = 0.25;
		EnemyPlayersBlockCapture = true;

		Locations = new array<ref DZKOTH_ProfileLocation>;
		Locations.Insert(new DZKOTH_ProfileLocation);

		ZombieTypes = new array<string>;
		ZombieTypes.Insert("ZmbM_SoldierNormal_Base");
		ZombieTypes.Insert("ZmbM_PatrolNormal_Autumn");
		ZombieTypes.Insert("ZmbM_PatrolNormal_Flat");

		RewardLoot = new array<ref DZKOTH_ProfileLootEntry>;
		AddDefaultLoot("M4A1", 1);
		AddDefaultLoot("Mag_STANAG_30Rnd", 3);
		AddDefaultLoot("Ammo_556x45", 2);
	}

	protected void AddDefaultLoot(string className, int quantity)
	{
		DZKOTH_ProfileLootEntry entry = new DZKOTH_ProfileLootEntry;
		entry.ClassName = className;
		entry.Quantity = quantity;
		RewardLoot.Insert(entry);
	}
}

class DZKOTH_ConfigBundle
{
	ref DZKOTH_MainConfig Main;
	ref DZKOTH_LocationsConfig Locations;
	ref DZKOTH_WavesConfig Waves;
	ref DZKOTH_LootConfig Loot;

	void DZKOTH_ConfigBundle()
	{
		Main = new DZKOTH_MainConfig;
		Locations = new DZKOTH_LocationsConfig;
		Waves = new DZKOTH_WavesConfig;
		Loot = new DZKOTH_LootConfig;
	}
}

class DZKOTH_Config
{
	static ref DZKOTH_ConfigBundle LoadAll()
	{
		ref DZKOTH_ConfigBundle bundle = new DZKOTH_ConfigBundle;
		bundle.Main = LoadMainConfig();
		bundle.Locations = LoadLocationsConfig();
		bundle.Waves = LoadWavesConfig();
		bundle.Loot = LoadLootConfig();
		ApplyProfileConfig(bundle, LoadProfileConfig());
		DZKOTH_Utils.Log("Config loaded");
		return bundle;
	}

	static ref DZKOTH_MainConfig LoadMainConfig()
	{
		ref DZKOTH_MainConfig config = new DZKOTH_MainConfig;
		string errorMessage;
		if (!JsonFileLoader<ref DZKOTH_MainConfig>.LoadFile(DZKOTH_Const.CONFIG_JSON, config, errorMessage))
			DZKOTH_Utils.Warn("Could not load main config from " + DZKOTH_Const.CONFIG_JSON + ". Defaults stay active. " + errorMessage);

		return config;
	}

	static ref DZKOTH_LocationsConfig LoadLocationsConfig()
	{
		ref DZKOTH_LocationsConfig config = new DZKOTH_LocationsConfig;
		string errorMessage;
		if (!JsonFileLoader<ref DZKOTH_LocationsConfig>.LoadFile(DZKOTH_Const.LOCATIONS_JSON, config, errorMessage))
			DZKOTH_Utils.Warn("Could not load locations config from " + DZKOTH_Const.LOCATIONS_JSON + ". Defaults stay active. " + errorMessage);

		return config;
	}

	static ref DZKOTH_WavesConfig LoadWavesConfig()
	{
		ref DZKOTH_WavesConfig config = new DZKOTH_WavesConfig;
		string errorMessage;
		if (!JsonFileLoader<ref DZKOTH_WavesConfig>.LoadFile(DZKOTH_Const.WAVES_JSON, config, errorMessage))
			DZKOTH_Utils.Warn("Could not load waves config from " + DZKOTH_Const.WAVES_JSON + ". Defaults stay active. " + errorMessage);

		return config;
	}

	static ref DZKOTH_LootConfig LoadLootConfig()
	{
		ref DZKOTH_LootConfig config = new DZKOTH_LootConfig;
		string errorMessage;
		if (!JsonFileLoader<ref DZKOTH_LootConfig>.LoadFile(DZKOTH_Const.LOOT_JSON, config, errorMessage))
			DZKOTH_Utils.Warn("Could not load loot config from " + DZKOTH_Const.LOOT_JSON + ". Defaults stay active. " + errorMessage);

		return config;
	}

	static ref DZKOTH_ProfileConfig LoadProfileConfig()
	{
		if (!FileExist(DZKOTH_Const.PROFILE_KOTH_DIR))
			MakeDirectory(DZKOTH_Const.PROFILE_KOTH_DIR);

		ref DZKOTH_ProfileConfig config = new DZKOTH_ProfileConfig;
		string errorMessage;
		if (!FileExist(DZKOTH_Const.PROFILE_KOTH_CONFIG_JSON))
		{
			if (JsonFileLoader<ref DZKOTH_ProfileConfig>.SaveFile(DZKOTH_Const.PROFILE_KOTH_CONFIG_JSON, config, errorMessage))
				DZKOTH_Utils.Log("Created default profile config " + DZKOTH_Const.PROFILE_KOTH_CONFIG_JSON);
			else
				DZKOTH_Utils.Warn("Could not create profile config " + DZKOTH_Const.PROFILE_KOTH_CONFIG_JSON + ". " + errorMessage);
			return config;
		}

		if (!JsonFileLoader<ref DZKOTH_ProfileConfig>.LoadFile(DZKOTH_Const.PROFILE_KOTH_CONFIG_JSON, config, errorMessage))
			DZKOTH_Utils.Warn("Could not load profile config " + DZKOTH_Const.PROFILE_KOTH_CONFIG_JSON + ". Defaults stay active. " + errorMessage);

		return config;
	}

	protected static void ApplyProfileConfig(DZKOTH_ConfigBundle bundle, DZKOTH_ProfileConfig profile)
	{
		if (!bundle || !bundle.Main || !profile)
			return;

		bundle.Main.Enabled = profile.Enabled;
		bundle.Main.DebugMode = profile.DebugMode;
		bundle.Main.DebugCommandsEnabled = profile.DebugMode != 0;
		bundle.Main.UseExpansionNotify = profile.UseExpansionNotify != 0;
		bundle.Main.UseExpansionMarkers = profile.UseExpansionMarkers != 0;
		bundle.Main.EventName = "DeutschZ KotH";
		bundle.Main.EventIntervalMinutes = profile.EventIntervalMinutes;
		bundle.Main.EventDurationMinutes = profile.EventDurationMinutes;
		bundle.Main.EventCooldownMinutes = profile.EventIntervalMinutes;
		bundle.Main.CaptureTimeSeconds = profile.CaptureDurationSeconds;
		bundle.Main.ChestActionDurationSeconds = profile.ChestActionDurationSeconds;
		bundle.Main.TerminalHackSeconds = profile.ChestActionDurationSeconds;
		bundle.Main.ZombieCount = profile.ZombieCount;
		bundle.Main.CaptureRadius = profile.CaptureRadius;
		bundle.Main.ZombieSpawnRadius = profile.ZombieSpawnRadius;
		bundle.Main.SpawnMinDistance = 8.0;
		bundle.Main.SpawnMaxDistance = profile.ZombieSpawnRadius;
		bundle.Main.MarkerName = profile.MarkerName;
		bundle.Main.ProgressLossWhenEmpty = profile.ProgressLossWhenEmpty;
		bundle.Main.ProgressLossPerSecond = profile.ProgressLossPerSecond;
		bundle.Main.EnemyPlayersBlockCapture = profile.EnemyPlayersBlockCapture;
		bundle.Main.RequireTerminalHackBeforeCapture = true;
		bundle.Main.TerminalHackDecayEnabled = false;

		if (bundle.Main.CaptureTimeSeconds < 1)
			bundle.Main.CaptureTimeSeconds = 300;
		if (bundle.Main.ChestActionDurationSeconds < 1)
			bundle.Main.ChestActionDurationSeconds = 60;
		if (bundle.Main.TerminalHackSeconds < 1)
			bundle.Main.TerminalHackSeconds = bundle.Main.ChestActionDurationSeconds;
		if (bundle.Main.ZombieCount < 1)
			bundle.Main.ZombieCount = 5;
		if (bundle.Main.CaptureRadius <= 0.0)
			bundle.Main.CaptureRadius = 35.0;
		if (bundle.Main.ZombieSpawnRadius <= 0.0)
			bundle.Main.ZombieSpawnRadius = 25.0;
		if (bundle.Main.SpawnMaxDistance < bundle.Main.SpawnMinDistance)
			bundle.Main.SpawnMaxDistance = bundle.Main.SpawnMinDistance + 5.0;
		if (bundle.Main.MarkerName == "")
			bundle.Main.MarkerName = "DeutschZ KotH";

		ApplyProfileLocations(bundle, profile);
		ApplyProfileZombies(bundle, profile);
		ApplyProfileLoot(bundle, profile);
	}

	protected static void ApplyProfileLocations(DZKOTH_ConfigBundle bundle, DZKOTH_ProfileConfig profile)
	{
		if (!bundle || !bundle.Locations || !bundle.Locations.Locations || !profile || !profile.Locations)
			return;

		bundle.Locations.Locations.Clear();
		foreach (DZKOTH_ProfileLocation profileLocation: profile.Locations)
		{
			if (!profileLocation || profileLocation.Enabled == 0)
				continue;

			DZKOTH_LocationConfig location = new DZKOTH_LocationConfig;
			location.Enabled = profileLocation.Enabled;
			location.Name = profileLocation.Name;
			location.Radius = bundle.Main.CaptureRadius;
			location.Center = profileLocation.Center;
			location.Position = profileLocation.Center;
			location.FlagPosition = profileLocation.FlagPosition;
			location.FlagOrientation = profileLocation.FlagOrientation;
			location.ChestPosition = profileLocation.ChestPosition;
			location.ChestOrientation = profileLocation.ChestOrientation;
			location.RewardCratePosition = profileLocation.ChestPosition;
			location.RewardCrateOrientation = profileLocation.ChestOrientation;
			location.TerminalPosition = profileLocation.ChestPosition;
			bundle.Locations.Locations.Insert(location);
		}

		if (bundle.Locations.Locations.Count() == 0)
			bundle.Locations.Locations.Insert(new DZKOTH_LocationConfig);
	}

	protected static void ApplyProfileZombies(DZKOTH_ConfigBundle bundle, DZKOTH_ProfileConfig profile)
	{
		if (!bundle || !bundle.Waves || !bundle.Waves.WaveOne || !profile)
			return;

		bundle.Waves.WaveOne.InfectedCountMin = bundle.Main.ZombieCount;
		bundle.Waves.WaveOne.InfectedCountMax = bundle.Main.ZombieCount;
		bundle.Waves.WaveOne.TriggerProgress = 0.0;
		bundle.Waves.WaveOne.HealthMultiplier = 1.0;
		bundle.Waves.WaveOne.DamageMultiplier = 1.0;
		bundle.Waves.WaveOne.DisableRunning = false;

		if (profile.ZombieTypes && profile.ZombieTypes.Count() > 0)
		{
			bundle.Waves.WaveOne.Types.Clear();
			foreach (string zombieType: profile.ZombieTypes)
			{
				if (zombieType != "")
					bundle.Waves.WaveOne.Types.Insert(zombieType);
			}
		}
	}

	protected static void ApplyProfileLoot(DZKOTH_ConfigBundle bundle, DZKOTH_ProfileConfig profile)
	{
		if (!bundle || !bundle.Loot || !profile || !profile.RewardLoot)
			return;

		bundle.Loot.RewardCrateLoot.Clear();
		foreach (DZKOTH_ProfileLootEntry profileEntry: profile.RewardLoot)
		{
			if (!profileEntry || profileEntry.ClassName == "")
				continue;

			DZKOTH_LootEntry entry = new DZKOTH_LootEntry;
			entry.ClassName = profileEntry.ClassName;
			entry.Chance = 100.0;
			entry.Min = profileEntry.Quantity;
			entry.Max = profileEntry.Quantity;
			if (entry.Min < 1)
				entry.Min = 1;
			if (entry.Max < entry.Min)
				entry.Max = entry.Min;
			bundle.Loot.RewardCrateLoot.Insert(entry);
		}
	}
}
