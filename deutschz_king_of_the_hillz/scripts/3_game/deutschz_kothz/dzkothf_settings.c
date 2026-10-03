class DZKOTHF_RewardItemSetting
{
	string Type;
	int Count;
	int MinQuantity;
	int MaxQuantity;
	float Chance;
	bool Enabled;
	float MinHealthPercent;
	float MaxHealthPercent;
	ref array<string> Attachments;

	void DZKOTHF_RewardItemSetting(string type = "", int count = 1, int minQuantity = 1, int maxQuantity = 1, float chance = 1.0)
	{
		Type = type;
		Count = count;
		MinQuantity = minQuantity;
		MaxQuantity = maxQuantity;
		Chance = chance;
		Enabled = true;
		MinHealthPercent = 75.0;
		MaxHealthPercent = 100.0;
		Attachments = new array<string>;
	}

	void Validate()
	{
		Count = Math.Clamp(Count, 1, 20);
		MinQuantity = Math.Max(MinQuantity, 1);
		MaxQuantity = Math.Max(MaxQuantity, MinQuantity);
		Chance = Math.Clamp(Chance, 0.0, 1.0);
		MinHealthPercent = Math.Clamp(MinHealthPercent, 1.0, 100.0);
		MaxHealthPercent = Math.Clamp(MaxHealthPercent, MinHealthPercent, 100.0);
		if (!Attachments)
			Attachments = new array<string>;
	}
}

class DZKOTH_RareRewardSetting
{
	bool Enabled = true;
	string ClassName;
	float ChancePercent;
	int MaxCount = 1;

	void DZKOTH_RareRewardSetting(string type = "", float chance = 0)
	{
		ClassName = type;
		ChancePercent = chance;
	}
}

class DZKOTHF_ProGlobalSettings
{
	bool Enabled;
	bool DebugMode;
	int EventIntervalMinutes;
	int EventDurationMinutes;
	int CleanupDelayMinutes;
	int RewardMinimumItems;
	ref array<ref DZKOTH_RareRewardSetting> RareRewards;
	float RewardFireworkChancePercent;
	float ProgressHudRadius;
	bool ProgressLossWhenEmpty;
	float ProgressLossPerSecond;
	bool EnemyPlayersBlockCapture;
	bool AllowMultiplePlayersToSpeedUpCapture;
	float TickSeconds;
	int AnnouncementDelaySeconds;
	float BossWarningSeconds;
	float BossHealth;
	float BossDamageMultiplier;
	float BossSpawnMinDistance;
	float BossSpawnMaxDistance;
	int BossSpawnAttempts;
	float BossMovementSpeedMultiplier;
	bool BossDisableRunning;
	string BossClassName;
	float ZombieSpawnMinDistance;
	float ZombieSpawnMaxDistance;
	float ZombieSpawnMinimumSeparation;
	int ZombieSpawnAttempts;
	float SecretDocumentChancePercent;
	bool GlobalSecretDocumentAnnouncement;
	bool PermanentTracking;
	bool DebugCommandsEnabled;
	bool MusicEnabled;
	float MusicRadius;
	bool MusicLoopPlaylist;
	bool MusicRandomize;
	bool MusicPreventImmediateRepeat;
	int MusicTrackSeconds;
	int FireworksDelaySeconds;
	int FireworkDurationSeconds;
	int VictoryPhaseSeconds;
	bool BossPhaseEnabled;
	string BossTriggerMode;
	float BossTriggerPercent;
	int BossTriggerDelaySeconds;
	bool AncientScreamEnabled;
	string AncientScreamTrigger;
	int AncientScreamDelaySeconds;
	string RewardCrateTrigger;
	string FireworksTrigger;
	int VictoryEffectsDurationSeconds;
	int EventObjectCleanupDelaySeconds;
	int RewardCrateLifetimeSeconds;
	bool RejectTraderSpawnZones;
	float TraderExclusionRadius;
	float MinSpeedMultiplier;
	float MaxSpeedMultiplier;
	string CompletionSmokeColor;
	bool ExpansionPartyFriendlyCapture;
	ref array<string> AdminUIDs;

	void DZKOTHF_ProGlobalSettings()
	{
		Enabled = true;
		DebugMode = false;
		EventIntervalMinutes = 60;
		EventDurationMinutes = 45;
		CleanupDelayMinutes = 10;
		RewardMinimumItems = 0;
		RareRewards = new array<ref DZKOTH_RareRewardSetting>;
		RareRewards.Insert(new DZKOTH_RareRewardSetting("ToxicZ_Secret_Document", 20));
		RareRewards.Insert(new DZKOTH_RareRewardSetting("DZATM_RobTool", 15));
		RewardFireworkChancePercent = 25.0;
		ProgressHudRadius = 100.0;
		ProgressLossWhenEmpty = false;
		ProgressLossPerSecond = 0.25;
		EnemyPlayersBlockCapture = true;
		AllowMultiplePlayersToSpeedUpCapture = true;
		TickSeconds = 1.0;
		AnnouncementDelaySeconds = 300;
		BossWarningSeconds = 4.0;
		BossHealth = 5000.0;
		BossDamageMultiplier = 10.0;
		BossSpawnMinDistance = 3.0;
		BossSpawnMaxDistance = 6.0;
		BossSpawnAttempts = 40;
		BossMovementSpeedMultiplier = 0.10;
		BossDisableRunning = true;
		BossClassName = "DZKOTH_BosZZombie";
		ZombieSpawnMinDistance = 6.0;
		ZombieSpawnMaxDistance = 15.0;
		ZombieSpawnMinimumSeparation = 4.0;
		ZombieSpawnAttempts = 40;
		SecretDocumentChancePercent = 20.0;
		GlobalSecretDocumentAnnouncement = true;
		PermanentTracking = false;
		DebugCommandsEnabled = true;
		MusicEnabled = true;
		MusicRadius = 100.0;
		MusicLoopPlaylist = true;
		MusicRandomize = true;
		MusicPreventImmediateRepeat = true;
		MusicTrackSeconds = 0;
		FireworksDelaySeconds = 1;
		FireworkDurationSeconds = 30;
		VictoryPhaseSeconds = 60;
		BossPhaseEnabled = true;
		BossTriggerMode = "AFTER_CAPTURE_WIN";
		BossTriggerPercent = 100.0;
		BossTriggerDelaySeconds = 12;
		AncientScreamEnabled = true;
		AncientScreamTrigger = "BEFORE_BOSS";
		AncientScreamDelaySeconds = 3;
		RewardCrateTrigger = "ON_BOSS_DEFEATED";
		FireworksTrigger = "ON_BOSS_DEFEATED";
		VictoryEffectsDurationSeconds = 30;
		EventObjectCleanupDelaySeconds = 60;
		RewardCrateLifetimeSeconds = 1800;
		RejectTraderSpawnZones = true;
		TraderExclusionRadius = 150.0;
		MinSpeedMultiplier = 0.55;
		MaxSpeedMultiplier = 1.35;
		CompletionSmokeColor = "RED";
		ExpansionPartyFriendlyCapture = true;
		AdminUIDs = new array<string>;
	}

	void Validate()
	{
		EventIntervalMinutes = Math.Clamp(EventIntervalMinutes, 1, 1440);
		EventDurationMinutes = Math.Clamp(EventDurationMinutes, 5, 240);
		CleanupDelayMinutes = Math.Clamp(CleanupDelayMinutes, 1, 60);
		RewardMinimumItems = Math.Clamp(RewardMinimumItems, 0, 500);
		RewardFireworkChancePercent = Math.Clamp(RewardFireworkChancePercent, 0.0, 100.0);
		ProgressHudRadius = Math.Clamp(ProgressHudRadius, 10.0, 500.0);
		ProgressLossPerSecond = Math.Clamp(ProgressLossPerSecond, 0.0, 10.0);
		TickSeconds = Math.Clamp(TickSeconds, 0.25, 5.0);
		AnnouncementDelaySeconds = Math.Clamp(AnnouncementDelaySeconds, 0, 3600);
		BossWarningSeconds = Math.Clamp(BossWarningSeconds, 0.0, 60.0);
		BossHealth = Math.Clamp(BossHealth, 100.0, 50000.0);
		BossDamageMultiplier = Math.Clamp(BossDamageMultiplier, 1.0, 25.0);
		BossSpawnMinDistance = Math.Clamp(BossSpawnMinDistance, 3.0, 100.0);
		BossSpawnMaxDistance = Math.Clamp(BossSpawnMaxDistance, BossSpawnMinDistance, 150.0);
		BossSpawnAttempts = Math.Clamp(BossSpawnAttempts, 1, 100);
		BossMovementSpeedMultiplier = Math.Clamp(BossMovementSpeedMultiplier, 0.10, 1.00);
		if (BossClassName == "")
			BossClassName = "DZKOTH_BosZZombie";
		ZombieSpawnMinDistance = Math.Clamp(ZombieSpawnMinDistance, 3.0, 200.0);
		ZombieSpawnMaxDistance = Math.Clamp(ZombieSpawnMaxDistance, ZombieSpawnMinDistance, 250.0);
		ZombieSpawnMinimumSeparation = Math.Clamp(ZombieSpawnMinimumSeparation, 0.0, 25.0);
		ZombieSpawnAttempts = Math.Clamp(ZombieSpawnAttempts, 1, 200);
		SecretDocumentChancePercent = Math.Clamp(SecretDocumentChancePercent, 0.0, 20.0);
		MusicRadius = Math.Clamp(MusicRadius, 10.0, 500.0);
		MusicTrackSeconds = Math.Clamp(MusicTrackSeconds, 0, 1800);
		FireworksDelaySeconds = Math.Clamp(FireworksDelaySeconds, 0, 60);
		FireworkDurationSeconds = Math.Clamp(FireworkDurationSeconds, 5, 120);
		VictoryPhaseSeconds = Math.Clamp(VictoryPhaseSeconds, 10, 300);
		BossTriggerPercent = Math.Clamp(BossTriggerPercent, 0.0, 100.0);
		BossTriggerDelaySeconds = Math.Clamp(BossTriggerDelaySeconds, 0, 600);
		AncientScreamDelaySeconds = Math.Clamp(AncientScreamDelaySeconds, 0, 60);
		VictoryEffectsDurationSeconds = Math.Clamp(VictoryEffectsDurationSeconds, 5, 300);
		EventObjectCleanupDelaySeconds = Math.Clamp(EventObjectCleanupDelaySeconds, 30, 3600);
		RewardCrateLifetimeSeconds = Math.Clamp(RewardCrateLifetimeSeconds, 60, 7200);
		TraderExclusionRadius = Math.Clamp(TraderExclusionRadius, 25.0, 500.0);
		if (BossTriggerMode == "")
			BossTriggerMode = "AFTER_CAPTURE_WIN";
		if (AncientScreamTrigger == "")
			AncientScreamTrigger = "BEFORE_BOSS";
		if (RewardCrateTrigger == "")
			RewardCrateTrigger = "ON_BOSS_DEFEATED";
		if (FireworksTrigger == "")
			FireworksTrigger = "ON_BOSS_DEFEATED";
		MinSpeedMultiplier = Math.Clamp(MinSpeedMultiplier, 0.10, 1.35);
		MaxSpeedMultiplier = Math.Clamp(MaxSpeedMultiplier, 0.55, 2.00);
		if (MaxSpeedMultiplier < MinSpeedMultiplier)
			MaxSpeedMultiplier = MinSpeedMultiplier;
		if (CompletionSmokeColor == "")
			CompletionSmokeColor = "RED";
		if (!AdminUIDs)
			AdminUIDs = new array<string>;
	}
}

class DZKOTHF_ProZoneSettings
{
	string Name;
	bool Enabled;
	int InitialInfectedCount;
	bool BossEnabled;

	void DZKOTHF_ProZoneSettings(string name = "", bool enabled = true)
	{
		Name = name;
		Enabled = enabled;
		InitialInfectedCount = -1;
		BossEnabled = true;
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
	string FlagClassName;
	string RewardCrateClass;
	ref array<ref DZKOTHF_RewardItemSetting> RewardItems;
	int RewardLifetimeMinutes;
	ref array<float> RewardCrateOffset;
	bool FireworkEnabled;
	ref array<float> FireworkOffset;
	float MusicVolume;
	float MusicRadius;
	string EventName;
	string MarkerIcon;
	bool UseLBmasterMarkerWhenAvailable;
	string LBmasterMarkerIcon;
	bool LBmasterMarkerDisplay3D;
	bool LBmasterMarkerDisplayMap;
	bool LBmasterMarkerDisplayGPS;
	bool RespectLBmasterGroups;
	bool UseExpansionMarkerWhenAvailable;
	string ExpansionMarkerIcon;
	bool UseExpansionNotificationsWhenAvailable;
	bool RespectExpansionParties;
	bool UseBasicMapMarkerWhenAvailable;
	bool ShowProgressDuringAnnouncement;
	bool NotifyUseChatFallback;
	ref array<float> EventPosition;
	ref array<float> EventOrientation;
	ref array<string> AdminSteamIds;
	ref DZKOTHF_ProGlobalSettings ProGlobal;
	ref array<ref DZKOTHF_ProZoneSettings> ProZones;

	void DZKOTHF_Settings()
	{
		Enabled = true;
		AutoStart = true;
		AutoStartDelaySeconds = 3;
		AnnounceSeconds = 300;
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
		SpawnDelaySeconds = 0;
		FlagClassName = "DZKOTH_ProEventFlag";
		RewardCrateClass = "DZKOTH_ProRewardCrate";
		RewardItems = {
			new DZKOTHF_RewardItemSetting("TTC_PP91", 1, 1, 1, 0.25),
			new DZKOTHF_RewardItemSetting("TTC_PP91_Mag_20rnd", 2, 20, 30, 0.75),
			new DZKOTHF_RewardItemSetting("Ammo_380", 2, 20, 40, 0.80),
			new DZKOTHF_RewardItemSetting("BandageDressing", 3, 1, 1, 1.00),
			new DZKOTHF_RewardItemSetting("Canteen", 1, 50, 100, 0.80),
			new DZKOTHF_RewardItemSetting("TacticalBaconCan", 2, 50, 100, 0.90)
		};
		RewardLifetimeMinutes = 10;
		RewardCrateOffset = {-3.0, 0.0, 0.0};
		FireworkEnabled = true;
		FireworkOffset = {3.0, 0.0, 0.0};
		MusicVolume = 0.60;
		MusicRadius = 100.0;
		EventName = "DeutschZ KotHZ";
		MarkerIcon = DZKOTHF_Constants.DEFAULT_MARKER_ICON;
		UseLBmasterMarkerWhenAvailable = true;
		LBmasterMarkerIcon = DZKOTHF_Constants.DEFAULT_MARKER_ICON;
		LBmasterMarkerDisplay3D = true;
		LBmasterMarkerDisplayMap = true;
		LBmasterMarkerDisplayGPS = true;
		RespectLBmasterGroups = true;
		UseExpansionMarkerWhenAvailable = true;
		ExpansionMarkerIcon = "Skull 3";
		UseExpansionNotificationsWhenAvailable = true;
		RespectExpansionParties = true;
		UseBasicMapMarkerWhenAvailable = true;
		ShowProgressDuringAnnouncement = false;
		NotifyUseChatFallback = false;
		EventPosition = {4552.346680, 317.997314, 8350.974609};
		EventOrientation = {0.0, 0.0, 0.0};
		AdminSteamIds = {"76561199819501556"};
		ProGlobal = new DZKOTHF_ProGlobalSettings;
		ProZones = new array<ref DZKOTHF_ProZoneSettings>;
	}

	void Validate()
	{
		AutoStartDelaySeconds = Math.Clamp(AutoStartDelaySeconds, 1, 3600);
		AnnounceSeconds = Math.Clamp(AnnounceSeconds, 0, 3600);
		CaptureRadius = Math.Clamp(CaptureRadius, 5.0, 250.0);
		CaptureTimeSeconds = Math.Clamp(CaptureTimeSeconds, 10, 7200);
		CaptureTickMilliseconds = Math.Clamp(CaptureTickMilliseconds, 250, 5000);
		CompletionCleanupDelaySeconds = Math.Clamp(CompletionCleanupDelaySeconds, 1, 600);
		// FREE uses one static infected block (no waves); the amount stays admin-editable.
		EnemyCount = Math.Clamp(EnemyCount, 0, 200);
		SpawnRadius = Math.Clamp(SpawnRadius, 10.0, 250.0);
		SpawnDelaySeconds = Math.Clamp(SpawnDelaySeconds, 0, 300);
		RewardLifetimeMinutes = Math.Clamp(RewardLifetimeMinutes, 1, 120);
		MusicVolume = Math.Clamp(MusicVolume, 0.0, 1.0);
		MusicRadius = Math.Clamp(MusicRadius, 10.0, 500.0);

		if (!EnemyClassNames || EnemyClassNames.Count() == 0)
			EnemyClassNames = {"ZmbM_HermitSkinny_Beige"};

		if (FlagClassName == "")
			FlagClassName = "DZKOTH_ProEventFlag";
		if (RewardCrateClass == "")
			RewardCrateClass = "DZKOTH_ProRewardCrate";

		if (!RewardItems || RewardItems.Count() == 0)
		{
			RewardItems = {
				new DZKOTHF_RewardItemSetting("TTC_PP91", 1, 1, 1, 0.25),
				new DZKOTHF_RewardItemSetting("TTC_PP91_Mag_20rnd", 2, 20, 30, 0.75),
				new DZKOTHF_RewardItemSetting("Ammo_380", 2, 20, 40, 0.80),
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

		if (!RewardCrateOffset || RewardCrateOffset.Count() != 3)
			RewardCrateOffset = {-3.0, 0.0, 0.0};

		if (!FireworkOffset || FireworkOffset.Count() != 3)
			FireworkOffset = {3.0, 0.0, 0.0};

		if (EventName == "")
			EventName = "DeutschZ KotHZ";

		if (MarkerIcon == "" || MarkerIcon.Contains("map_" + "tree_ca.paa"))
			MarkerIcon = DZKOTHF_Constants.DEFAULT_MARKER_ICON;

		if (LBmasterMarkerIcon == "")
			LBmasterMarkerIcon = MarkerIcon;

		if (ExpansionMarkerIcon == "")
			ExpansionMarkerIcon = "Skull 3";

		if (!EventPosition || EventPosition.Count() != 3)
			EventPosition = {4552.346680, 317.997314, 8350.974609};

		if (!EventOrientation || EventOrientation.Count() != 3)
			EventOrientation = {0.0, 0.0, 0.0};

		if (!AdminSteamIds)
			AdminSteamIds = {"76561199819501556"};

		if (!ProGlobal)
			ProGlobal = new DZKOTHF_ProGlobalSettings;
		ProGlobal.Validate();

		if (!ProZones)
			ProZones = new array<ref DZKOTHF_ProZoneSettings>;
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

	vector GetRewardCrateOffset()
	{
		return Vector(RewardCrateOffset[0], RewardCrateOffset[1], RewardCrateOffset[2]);
	}

	vector GetFireworkOffset()
	{
		return Vector(FireworkOffset[0], FireworkOffset[1], FireworkOffset[2]);
	}
}

class DZKOTHF_SettingsLoader
{
	static ref DZKOTHF_Settings Load()
	{
		DZKOTHF_ProfilePaths.EnsureDirectories();

		ref DZKOTHF_Settings settings = new DZKOTHF_Settings;
		string errorMessage;
		bool writeSettings;
		string writeReason;
		if (FileExist(DZKOTHF_Constants.SETTINGS_PATH))
		{
			if (!JsonFileLoader<ref DZKOTHF_Settings>.LoadFile(DZKOTHF_Constants.SETTINGS_PATH, settings, errorMessage) || !settings)
			{
				DZKOTHF_Log.Error("Settings are corrupt and will be regenerated from safe defaults. " + errorMessage);
				if (!FileExist(DZKOTHF_Constants.CORRUPT_SETTINGS_BACKUP_PATH))
				{
					if (CopyFile(DZKOTHF_Constants.SETTINGS_PATH, DZKOTHF_Constants.CORRUPT_SETTINGS_BACKUP_PATH))
						DZKOTHF_Log.Warning("Corrupt settings were preserved at " + DZKOTHF_Constants.CORRUPT_SETTINGS_BACKUP_PATH + ".");
					else
						DZKOTHF_Log.Error("Corrupt settings could not be copied to the backup path before regeneration.");
				}
				else
				{
					DZKOTHF_Log.Warning("An earlier corrupt-settings backup already exists and remains untouched at " + DZKOTHF_Constants.CORRUPT_SETTINGS_BACKUP_PATH + ".");
				}
				settings = new DZKOTHF_Settings;
				writeSettings = true;
				writeReason = "corrupt settings regenerated";
			}
			else
			{
				// A valid current file is read-only during normal boot/gameplay.  Defaults
				// are written only for creation, migration or corruption recovery below.
				writeSettings = false;
			}
		}
		else if (FileExist(DZKOTHF_Constants.PREVIOUS_SETTINGS_PATH))
		{
			if (JsonFileLoader<ref DZKOTHF_Settings>.LoadFile(DZKOTHF_Constants.PREVIOUS_SETTINGS_PATH, settings, errorMessage) && settings)
			{
				DZKOTHF_Log.Warning("Previous settings filename detected and migrated once to KotHZSettings.json. Previous file retained: " + DZKOTHF_Constants.PREVIOUS_SETTINGS_PATH + ".");
				writeReason = "previous settings migrated";
			}
			else
			{
				DZKOTHF_Log.Error("Previous settings filename was detected but could not be loaded; safe defaults are used. Previous file retained. " + errorMessage);
				settings = new DZKOTHF_Settings;
				writeReason = "unreadable previous settings replaced by defaults";
			}
			writeSettings = true;
		}
		else if (FileExist(DZKOTHF_Constants.LEGACY_SETTINGS_PATH))
		{
			if (JsonFileLoader<ref DZKOTHF_Settings>.LoadFile(DZKOTHF_Constants.LEGACY_SETTINGS_PATH, settings, errorMessage) && settings)
			{
				DZKOTHF_Log.Warning("Legacy settings detected and migrated once to the new config path. Legacy file retained: " + DZKOTHF_Constants.LEGACY_SETTINGS_PATH + ".");
				writeReason = "legacy settings migrated";
			}
			else
			{
				DZKOTHF_Log.Error("Legacy settings were detected but could not be loaded; safe defaults are used. Legacy file retained. " + errorMessage);
				settings = new DZKOTHF_Settings;
				writeReason = "unreadable legacy settings replaced by defaults";
			}
			writeSettings = true;
		}
		else
		{
			writeSettings = true;
			writeReason = "settings created because no file existed";
		}

		settings.Validate();
		if (writeSettings)
		{
			if (!JsonFileLoader<ref DZKOTHF_Settings>.SaveFile(DZKOTHF_Constants.SETTINGS_PATH, settings, errorMessage))
				DZKOTHF_Log.Error("Settings could not be written (" + writeReason + "). " + errorMessage);
			else
				DZKOTHF_Log.Info("Settings safely written: " + writeReason + ". Path: " + DZKOTHF_Constants.SETTINGS_PATH + ".");
		}

		if (FileExist(DZKOTHF_Constants.LEGACY_SETTINGS_PATH) && FileExist(DZKOTHF_Constants.SETTINGS_PATH))
			DZKOTHF_Log.Warning("Legacy settings remain untouched at " + DZKOTHF_Constants.LEGACY_SETTINGS_PATH + "; the new config path is authoritative.");

		if (FileExist(DZKOTHF_Constants.PREVIOUS_SETTINGS_PATH) && FileExist(DZKOTHF_Constants.SETTINGS_PATH))
			DZKOTHF_Log.Warning("Previous settings file remains untouched at " + DZKOTHF_Constants.PREVIOUS_SETTINGS_PATH + "; KotHZSettings.json is authoritative.");

		return settings;
	}
}
