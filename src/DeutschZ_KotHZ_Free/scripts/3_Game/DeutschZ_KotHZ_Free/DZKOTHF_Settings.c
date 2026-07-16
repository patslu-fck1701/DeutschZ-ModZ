class DZKOTHF_RewardItemSetting
{
	string Type;
	int Count;
	int MinQuantity;
	int MaxQuantity;
	float Chance;

	void DZKOTHF_RewardItemSetting(string type = "", int count = 1, int minQuantity = 1, int maxQuantity = 1, float chance = 1.0)
	{
		Type = type;
		Count = count;
		MinQuantity = minQuantity;
		MaxQuantity = maxQuantity;
		Chance = chance;
	}

	void Validate()
	{
		Count = Math.Clamp(Count, 1, 20);
		MinQuantity = Math.Max(MinQuantity, 1);
		MaxQuantity = Math.Max(MaxQuantity, MinQuantity);
		Chance = Math.Clamp(Chance, 0.0, 1.0);
	}
}

class DZKOTHF_Settings
{
	bool Enabled;
	bool AutoStart;
	int AutoStartDelaySeconds;
	int AnnounceSeconds;
	float CaptureRadius;
	int CaptureTimeSeconds;
	int CaptureTickMilliseconds;
	int CompletionCleanupDelaySeconds;
	int EnemyCount;
	ref array<string> EnemyClassNames;
	float SpawnRadius;
	int SpawnDelaySeconds;
	string RewardCrateClass;
	ref array<ref DZKOTHF_RewardItemSetting> RewardItems;
	int RewardLifetimeMinutes;
	ref array<float> RewardSpawnOffset;
	string EventName;
	string MarkerIcon;
	bool NotifyUseChatFallback;
	ref array<float> EventPosition;
	ref array<float> EventOrientation;
	ref array<string> AdminSteamIds;

	void DZKOTHF_Settings()
	{
		Enabled = true;
		AutoStart = true;
		AutoStartDelaySeconds = 3;
		AnnounceSeconds = 10;
		CaptureRadius = 25.0;
		CaptureTimeSeconds = 300;
		CaptureTickMilliseconds = 1000;
		CompletionCleanupDelaySeconds = 10;
		EnemyCount = 15;
		EnemyClassNames = {
			"ZmbM_HermitSkinny_Beige",
			"ZmbM_FarmerFat_Blue",
			"ZmbF_CitizenANormal_Beige",
			"ZmbM_CitizenASkinny_Blue",
			"ZmbF_CitizenBSkinny",
			"ZmbM_HikerSkinny_Green"
		};
		SpawnRadius = 40.0;
		SpawnDelaySeconds = 2;
		RewardCrateClass = DZKOTHF_Constants.DEFAULT_REWARD_CRATE_CLASSNAME;
		RewardItems = {
			new DZKOTHF_RewardItemSetting("M4A1", 1, 1, 1, 0.25),
			new DZKOTHF_RewardItemSetting("Mag_STANAG_30Rnd", 2, 20, 30, 0.75),
			new DZKOTHF_RewardItemSetting("Ammo_556x45", 2, 20, 40, 0.80),
			new DZKOTHF_RewardItemSetting("BandageDressing", 3, 1, 1, 1.00),
			new DZKOTHF_RewardItemSetting("Canteen", 1, 50, 100, 0.80),
			new DZKOTHF_RewardItemSetting("TacticalBaconCan", 2, 50, 100, 0.90)
		};
		RewardLifetimeMinutes = 10;
		RewardSpawnOffset = {3.0, 0.0, 0.0};
		EventName = "DeutschZ KotHZ Free";
		MarkerIcon = "DZ\\gear\\navigation\\data\\map_tree_ca.paa";
		NotifyUseChatFallback = false;
		EventPosition = {4552.346680, 317.997314, 8350.974609};
		EventOrientation = {0.0, 0.0, 0.0};
		AdminSteamIds = {};
	}

	void Validate()
	{
		AutoStartDelaySeconds = Math.Clamp(AutoStartDelaySeconds, 1, 3600);
		AnnounceSeconds = Math.Clamp(AnnounceSeconds, 1, 3600);
		CaptureRadius = Math.Clamp(CaptureRadius, 5.0, 250.0);
		CaptureTimeSeconds = Math.Clamp(CaptureTimeSeconds, 10, 7200);
		CaptureTickMilliseconds = Math.Clamp(CaptureTickMilliseconds, 250, 5000);
		CompletionCleanupDelaySeconds = Math.Clamp(CompletionCleanupDelaySeconds, 1, 600);
		EnemyCount = Math.Clamp(EnemyCount, 0, 100);
		SpawnRadius = Math.Clamp(SpawnRadius, 10.0, 250.0);
		SpawnDelaySeconds = Math.Clamp(SpawnDelaySeconds, 0, 300);
		RewardLifetimeMinutes = Math.Clamp(RewardLifetimeMinutes, 1, 120);

		if (!EnemyClassNames || EnemyClassNames.Count() == 0)
			EnemyClassNames = {"ZmbM_HermitSkinny_Beige"};

		if (RewardCrateClass == "")
			RewardCrateClass = DZKOTHF_Constants.DEFAULT_REWARD_CRATE_CLASSNAME;

		if (!RewardItems || RewardItems.Count() == 0)
		{
			RewardItems = {
				new DZKOTHF_RewardItemSetting("M4A1", 1, 1, 1, 0.25),
				new DZKOTHF_RewardItemSetting("Mag_STANAG_30Rnd", 2, 20, 30, 0.75),
				new DZKOTHF_RewardItemSetting("Ammo_556x45", 2, 20, 40, 0.80),
				new DZKOTHF_RewardItemSetting("BandageDressing", 3, 1, 1, 1.00),
				new DZKOTHF_RewardItemSetting("Canteen", 1, 50, 100, 0.80),
				new DZKOTHF_RewardItemSetting("TacticalBaconCan", 2, 50, 100, 0.90)
			};
		}

		foreach (DZKOTHF_RewardItemSetting rewardItem: RewardItems)
		{
			if (rewardItem)
				rewardItem.Validate();
		}

		if (!RewardSpawnOffset || RewardSpawnOffset.Count() != 3)
			RewardSpawnOffset = {3.0, 0.0, 0.0};

		if (EventName == "")
			EventName = "DeutschZ KotHZ Free";

		if (!EventPosition || EventPosition.Count() != 3)
			EventPosition = {4552.346680, 317.997314, 8350.974609};

		if (!EventOrientation || EventOrientation.Count() != 3)
			EventOrientation = {0.0, 0.0, 0.0};

		if (!AdminSteamIds)
			AdminSteamIds = {};
	}

	bool IsAdmin(string steamId)
	{
		return steamId != "" && AdminSteamIds && AdminSteamIds.Find(steamId) != -1;
	}

	vector GetEventPosition()
	{
		return Vector(EventPosition[0], EventPosition[1], EventPosition[2]);
	}

	vector GetEventOrientation()
	{
		return Vector(EventOrientation[0], EventOrientation[1], EventOrientation[2]);
	}

	vector GetRewardSpawnOffset()
	{
		return Vector(RewardSpawnOffset[0], RewardSpawnOffset[1], RewardSpawnOffset[2]);
	}
}

class DZKOTHF_SettingsLoader
{
	static ref DZKOTHF_Settings Load()
	{
		DZKOTHF_ProfilePaths.EnsureDirectories();

		ref DZKOTHF_Settings settings = new DZKOTHF_Settings;
		string errorMessage;
		if (FileExist(DZKOTHF_Constants.SETTINGS_PATH))
		{
			if (!JsonFileLoader<ref DZKOTHF_Settings>.LoadFile(DZKOTHF_Constants.SETTINGS_PATH, settings, errorMessage) || !settings)
			{
				DZKOTHF_Log.Error("Settings could not be loaded; safe defaults are used. " + errorMessage);
				settings = new DZKOTHF_Settings;
			}
		}
		else if (FileExist(DZKOTHF_Constants.PREVIOUS_SETTINGS_PATH))
		{
			if (JsonFileLoader<ref DZKOTHF_Settings>.LoadFile(DZKOTHF_Constants.PREVIOUS_SETTINGS_PATH, settings, errorMessage) && settings)
				DZKOTHF_Log.Warning("Previous settings filename detected and migrated once to KotHZSettings.json. Previous file retained: " + DZKOTHF_Constants.PREVIOUS_SETTINGS_PATH + ".");
			else
			{
				DZKOTHF_Log.Error("Previous settings filename was detected but could not be loaded; safe defaults are used. Previous file retained. " + errorMessage);
				settings = new DZKOTHF_Settings;
			}
		}
		else if (FileExist(DZKOTHF_Constants.LEGACY_SETTINGS_PATH))
		{
			if (JsonFileLoader<ref DZKOTHF_Settings>.LoadFile(DZKOTHF_Constants.LEGACY_SETTINGS_PATH, settings, errorMessage) && settings)
				DZKOTHF_Log.Warning("Legacy settings detected and migrated once to the new config path. Legacy file retained: " + DZKOTHF_Constants.LEGACY_SETTINGS_PATH + ".");
			else
			{
				DZKOTHF_Log.Error("Legacy settings were detected but could not be loaded; safe defaults are used. Legacy file retained. " + errorMessage);
				settings = new DZKOTHF_Settings;
			}
		}

		settings.Validate();
		if (!JsonFileLoader<ref DZKOTHF_Settings>.SaveFile(DZKOTHF_Constants.SETTINGS_PATH, settings, errorMessage))
			DZKOTHF_Log.Error("Settings could not be saved. " + errorMessage);
		else
			DZKOTHF_Log.Info("Settings loaded from " + DZKOTHF_Constants.SETTINGS_PATH + ".");

		if (FileExist(DZKOTHF_Constants.LEGACY_SETTINGS_PATH) && FileExist(DZKOTHF_Constants.SETTINGS_PATH))
			DZKOTHF_Log.Warning("Legacy settings remain untouched at " + DZKOTHF_Constants.LEGACY_SETTINGS_PATH + "; the new config path is authoritative.");

		if (FileExist(DZKOTHF_Constants.PREVIOUS_SETTINGS_PATH) && FileExist(DZKOTHF_Constants.SETTINGS_PATH))
			DZKOTHF_Log.Warning("Previous settings file remains untouched at " + DZKOTHF_Constants.PREVIOUS_SETTINGS_PATH + "; KotHZSettings.json is authoritative.");

		return settings;
	}
}
