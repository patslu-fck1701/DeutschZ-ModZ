class DZKOTH_MainConfig
{
	int Enabled;
	int DebugMode;
	bool UseExpansionNotify;
	bool UseExpansionMarkers;
	string EventName;
	float CaptureRadius;
	int CaptureTimeSeconds;
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
	int RewardDespawnMinutes;
	int RewardMinimumItems;
	ref array<ref DZKOTH_RareRewardSetting> RareRewards;
	float RewardFireworkChancePercent;
	string FlagClassName;
	string RewardCrateClassName;
	float ProgressHudRadius;
	float TickSeconds;
	int AnnouncementDelaySeconds;
	float SpawnMinDistance;
	float SpawnMaxDistance;
	float SpawnMinimumSeparation;
	int SpawnAttempts;
	float BossWarningSeconds;
	float BossHealth;
	float BossDamageMultiplier;
	float BossSpawnMinDistance;
	float BossSpawnMaxDistance;
	int BossSpawnAttempts;
	float BossMovementSpeedMultiplier;
	bool BossDisableRunning;
	string BossClassName;
	float KeycardChancePercent;
	bool GlobalKeycardAnnouncement;
	bool PermanentTracking;
	bool DebugCommandsEnabled;
	string StartMode;
	bool MusicEnabled;
	float MusicVolume;
	float MusicRadius;
	bool MusicLoopPlaylist;
	bool MusicRandomize;
	bool MusicPreventImmediateRepeat;
	int MusicTrackSeconds;
	bool FireworkEnabled;
	int FireworkDurationSeconds;
	int VictoryPhaseSeconds;
	bool ProDevelopmentEnabled;
	bool LicenseCoreRequired;
	bool BossPhaseEnabled;
	string BossTriggerMode;
	float BossTriggerPercent;
	int BossTriggerDelaySeconds;
	bool AncientScreamEnabled;
	string AncientScreamTrigger;
	int AncientScreamDelaySeconds;
	string RewardCrateTrigger;
	string FireworksTrigger;
	int FireworksDelaySeconds;
	int VictoryEffectsDurationSeconds;
	int EventObjectCleanupDelaySeconds;
	int RewardCrateLifetimeSeconds;
	bool RejectTraderSpawnZones;
	float TraderExclusionRadius;
	ref array<string> AdminUIDs;

	void DZKOTH_MainConfig()
	{
		Enabled = 1;
		DebugMode = 1;
		UseExpansionNotify = true;
		UseExpansionMarkers = true;
		EventName = "DeutschZ KotH";
		CaptureRadius = 25.0;
		CaptureTimeSeconds = 300;
		ZombieCount = 5;
		ZombieSpawnRadius = 15.0;
		MarkerName = "DeutschZ KotH";
		EnemyPlayersBlockCapture = true;
		ProgressLossWhenEmpty = false;
		ProgressLossPerSecond = 0.25;
		RequirePlayersInsideZone = 1;
		AllowMultiplePlayersToSpeedUpCapture = true;
		EventIntervalMinutes = 60;
		EventDurationMinutes = 45;
		EventCooldownMinutes = 60;
		CleanupDelayMinutes = 10;
		RewardDespawnMinutes = 10;
		RewardMinimumItems = 0;
		RareRewards = new array<ref DZKOTH_RareRewardSetting>;
		RareRewards.Insert(new DZKOTH_RareRewardSetting("ToxicZ_Secret_Document", 20));
		RareRewards.Insert(new DZKOTH_RareRewardSetting("DZATM_RobTool", 15));
		RewardFireworkChancePercent = 25.0;
		FlagClassName = DZKOTH_Const.FLAG_CLASSNAME;
		RewardCrateClassName = DZKOTH_Const.REWARD_CRATE_CLASSNAME;
		ProgressHudRadius = 120.0;
		TickSeconds = 1.0;
		AnnouncementDelaySeconds = 300;
		SpawnMinDistance = 6.0;
		SpawnMaxDistance = 15.0;
		SpawnMinimumSeparation = 4.0;
		SpawnAttempts = 40;
		BossWarningSeconds = 4.0;
		BossHealth = 5000.0;
		BossDamageMultiplier = 10.0;
		BossSpawnMinDistance = 8.0;
		BossSpawnMaxDistance = 12.0;
		BossSpawnAttempts = 24;
		BossMovementSpeedMultiplier = 0.10;
		BossDisableRunning = true;
		BossClassName = "DZKOTH_BosZZombie";
		KeycardChancePercent = 20.0;
		GlobalKeycardAnnouncement = true;
		PermanentTracking = false;
		DebugCommandsEnabled = true;
		StartMode = "DIRECT";
		MusicEnabled = true;
		MusicVolume = 0.60;
		MusicRadius = 100.0;
		MusicLoopPlaylist = true;
		MusicRandomize = true;
		MusicPreventImmediateRepeat = true;
		MusicTrackSeconds = 0;
		FireworkEnabled = true;
		FireworkDurationSeconds = 30;
		VictoryPhaseSeconds = 60;
		ProDevelopmentEnabled = true;
		LicenseCoreRequired = false;
		BossPhaseEnabled = true;
		BossTriggerMode = "AFTER_CAPTURE_WIN";
		BossTriggerPercent = 100.0;
		BossTriggerDelaySeconds = 12;
		AncientScreamEnabled = false;
		AncientScreamTrigger = "BEFORE_BOSS";
		AncientScreamDelaySeconds = 3;
		RewardCrateTrigger = "ON_BOSS_DEFEATED";
		FireworksTrigger = "ON_BOSS_DEFEATED";
		FireworksDelaySeconds = 1;
		VictoryEffectsDurationSeconds = 30;
		EventObjectCleanupDelaySeconds = 60;
		RewardCrateLifetimeSeconds = 1800;
		RejectTraderSpawnZones = true;
		TraderExclusionRadius = 150.0;
		AdminUIDs = new array<string>;
		AdminUIDs.Insert("76561199819501556");
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
	float ForcedHealth;
	float MovementSpeedMultiplier;
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
		ForcedHealth = 0.0;
		MovementSpeedMultiplier = 1.0;
		VisualEyes = "";
		DisableRunning = true;
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
	ref DZKOTH_WaveConfig WaveFour;
	ref DZKOTH_WaveConfig WaveFive;

	void DZKOTH_WavesConfig()
	{
		WaveOne = new DZKOTH_WaveConfig;
		WaveOne.TriggerProgress = 0.0;
		WaveOne.InfectedCountMin = 5;
		WaveOne.InfectedCountMax = 5;
		WaveOne.ForcedHealth = 0.0;
		WaveOne.MovementSpeedMultiplier = 1.00;
		WaveOne.DisableRunning = false;
		WaveOne.Types.Clear();
		WaveOne.Types.Insert("ZmbM_HunterOld_Autumn");

		WaveTwo = new DZKOTH_WaveConfig;
		WaveTwo.TriggerProgress = 20.0;
		WaveTwo.InfectedCountMin = 6;
		WaveTwo.InfectedCountMax = 6;
		WaveTwo.DamageMultiplier = 1.25;
		WaveTwo.ForcedHealth = 0.0;
		WaveTwo.MovementSpeedMultiplier = 1.00;
		WaveTwo.DisableRunning = false;
		WaveTwo.Types.Clear();
		WaveTwo.Types.Insert("ZmbM_PolicemanSpecForce_Heavy");

		WaveThree = new DZKOTH_WaveConfig;
		WaveThree.TriggerProgress = 40.0;
		WaveThree.InfectedCountMin = 7;
		WaveThree.InfectedCountMax = 7;
		WaveThree.DamageMultiplier = 1.5;
		WaveThree.ForcedHealth = 250.0;
		WaveThree.MovementSpeedMultiplier = 1.00;
		WaveThree.DisableRunning = true;
		WaveThree.Types.Clear();
		WaveThree.Types.Insert("DZKOTH_Infected_600");

		WaveFour = new DZKOTH_WaveConfig;
		WaveFour.TriggerProgress = 60.0;
		WaveFour.InfectedCountMin = 8;
		WaveFour.InfectedCountMax = 8;
		WaveFour.DamageMultiplier = 1.75;
		WaveFour.ForcedHealth = 500.0;
		WaveFour.MovementSpeedMultiplier = 1.00;
		WaveFour.DisableRunning = true;
		WaveFour.Types.Clear();
		WaveFour.Types.Insert("DZKOTH_Infected_800");

		WaveFive = new DZKOTH_WaveConfig;
		WaveFive.TriggerProgress = 80.0;
		WaveFive.InfectedCountMin = 9;
		WaveFive.InfectedCountMax = 9;
		WaveFive.DamageMultiplier = 2.0;
		WaveFive.ForcedHealth = 1000.0;
		WaveFive.MovementSpeedMultiplier = 1.00;
		WaveFive.DisableRunning = true;
		WaveFive.Types.Clear();
		WaveFive.Types.Insert("DZKOTH_Infected_1000");
	}
}

class DZKOTH_LootEntry
{
	string ClassName;
	float Chance;
	int Min;
	int Max;
	int MinQuantity;
	int MaxQuantity;
	bool Enabled;
	float MinHealthPercent;
	float MaxHealthPercent;
	ref array<string> Attachments;
	ref array<string> ExtraItems;
	ref array<string> Alternatives;

	void DZKOTH_LootEntry()
	{
		ClassName = "";
		Chance = 100.0;
		Min = 1;
		Max = 1;
		MinQuantity = 1;
		MaxQuantity = 1;
		Enabled = true;
		MinHealthPercent = 75.0;
		MaxHealthPercent = 100.0;
		Attachments = new array<string>;
		ExtraItems = new array<string>;
		Alternatives = new array<string>;
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
		rifle.ClassName = "TTC_PP91";
		rifle.Chance = 100.0;
		RewardCrateLoot.Insert(rifle);

		DZKOTH_LootEntry mag = new DZKOTH_LootEntry;
		mag.ClassName = "TTC_PP91_Mag_20rnd";
		mag.Chance = 100.0;
		mag.Min = 3;
		mag.Max = 3;
		RewardCrateLoot.Insert(mag);

		DZKOTH_LootEntry ammo = new DZKOTH_LootEntry;
		ammo.ClassName = "Ammo_380";
		ammo.Chance = 100.0;
		ammo.Min = 2;
		ammo.Max = 2;
		RewardCrateLoot.Insert(ammo);

		DZKOTH_LootEntry bandage = new DZKOTH_LootEntry;
		bandage.ClassName = "BandageDressing";
		bandage.Chance = 100.0;
		BossCorpseLoot.Insert(bandage);

		DZKOTH_LootEntry epinephrine = new DZKOTH_LootEntry;
		epinephrine.ClassName = "Epinephrine";
		epinephrine.Chance = 75.0;
		BossCorpseLoot.Insert(epinephrine);

		DZKOTH_LootEntry grenade = new DZKOTH_LootEntry;
		grenade.ClassName = "M67Grenade";
		grenade.Chance = 50.0;
		BossCorpseLoot.Insert(grenade);
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
		ApplyUnifiedSettings(bundle, DZKOTHF_SettingsLoader.Load(), DZKOTHF_LocationLoader.LoadLocations());
		DZKOTH_Utils.Log("Config loaded");
		return bundle;
	}

	protected static void ApplyUnifiedSettings(DZKOTH_ConfigBundle bundle, DZKOTHF_Settings settings, DZKOTHF_LocationsSettings locations)
	{
		if (!bundle || !bundle.Main || !settings)
			return;

		bundle.Main.Enabled = settings.Enabled;
		bundle.Main.UseExpansionNotify = settings.UseExpansionNotificationsWhenAvailable;
		bundle.Main.UseExpansionMarkers = settings.UseExpansionMarkerWhenAvailable;
		bundle.Main.EventName = settings.EventName;
		bundle.Main.CaptureRadius = settings.CaptureRadius;
		bundle.Main.CaptureTimeSeconds = settings.CaptureTimeSeconds;
		bundle.Main.ZombieCount = settings.EnemyCount;
		bundle.Main.ZombieSpawnRadius = Math.Clamp(settings.SpawnRadius, 3.0, 250.0);
		bundle.Main.SpawnMinDistance = 3.0;
		bundle.Main.SpawnMaxDistance = bundle.Main.ZombieSpawnRadius;
		bundle.Main.MarkerName = settings.EventName;
		bundle.Main.RewardDespawnMinutes = settings.RewardLifetimeMinutes;
		bundle.Main.FlagClassName = settings.FlagClassName;
		bundle.Main.RewardCrateClassName = settings.RewardCrateClass;
		bundle.Main.MusicVolume = Math.Clamp(settings.MusicVolume, 0.0, 1.0);
		bundle.Main.FireworkEnabled = settings.FireworkEnabled;
		bundle.Main.AdminUIDs = settings.AdminSteamIds;

		if (settings.ProGlobal)
		{
			DZKOTHF_ProGlobalSettings pro = settings.ProGlobal;
			bundle.Main.Enabled = bundle.Main.Enabled && pro.Enabled;
			bundle.Main.DebugMode = pro.DebugMode;
			bundle.Main.DebugCommandsEnabled = pro.DebugCommandsEnabled;
			bundle.Main.EventIntervalMinutes = pro.EventIntervalMinutes;
			bundle.Main.EventCooldownMinutes = pro.EventIntervalMinutes;
			bundle.Main.EventDurationMinutes = pro.EventDurationMinutes;
			bundle.Main.CleanupDelayMinutes = pro.CleanupDelayMinutes;
			bundle.Main.RewardMinimumItems = pro.RewardMinimumItems;
			bundle.Main.RewardFireworkChancePercent = pro.RewardFireworkChancePercent;
			bundle.Main.ProgressHudRadius = pro.ProgressHudRadius;
			bundle.Main.ProgressLossWhenEmpty = pro.ProgressLossWhenEmpty;
			bundle.Main.ProgressLossPerSecond = pro.ProgressLossPerSecond;
			bundle.Main.EnemyPlayersBlockCapture = pro.EnemyPlayersBlockCapture;
			bundle.Main.AllowMultiplePlayersToSpeedUpCapture = pro.AllowMultiplePlayersToSpeedUpCapture;
			bundle.Main.RareRewards = pro.RareRewards;
			bundle.Main.TickSeconds = pro.TickSeconds;
			bundle.Main.AnnouncementDelaySeconds = pro.AnnouncementDelaySeconds;
			bundle.Main.BossWarningSeconds = pro.BossWarningSeconds;
			bundle.Main.BossHealth = pro.BossHealth;
			bundle.Main.BossDamageMultiplier = pro.BossDamageMultiplier;
			bundle.Main.BossSpawnMinDistance = pro.BossSpawnMinDistance;
			bundle.Main.BossSpawnMaxDistance = pro.BossSpawnMaxDistance;
			bundle.Main.BossSpawnAttempts = pro.BossSpawnAttempts;
			bundle.Main.BossMovementSpeedMultiplier = pro.BossMovementSpeedMultiplier;
			bundle.Main.BossDisableRunning = pro.BossDisableRunning;
			bundle.Main.BossClassName = pro.BossClassName;
			bundle.Main.SpawnMinDistance = pro.ZombieSpawnMinDistance;
			bundle.Main.SpawnMaxDistance = pro.ZombieSpawnMaxDistance;
			bundle.Main.SpawnMinimumSeparation = pro.ZombieSpawnMinimumSeparation;
			bundle.Main.SpawnAttempts = pro.ZombieSpawnAttempts;
			bundle.Main.KeycardChancePercent = Math.Clamp(pro.SecretDocumentChancePercent, 0.0, 20.0);
			bundle.Main.GlobalKeycardAnnouncement = pro.GlobalSecretDocumentAnnouncement;
			bundle.Main.PermanentTracking = pro.PermanentTracking;
			bundle.Main.MusicEnabled = pro.MusicEnabled;
			bundle.Main.MusicRadius = pro.MusicRadius;
			bundle.Main.MusicLoopPlaylist = pro.MusicLoopPlaylist;
			bundle.Main.MusicRandomize = pro.MusicRandomize;
			bundle.Main.MusicPreventImmediateRepeat = pro.MusicPreventImmediateRepeat;
			bundle.Main.MusicTrackSeconds = pro.MusicTrackSeconds;
			bundle.Main.FireworkDurationSeconds = pro.FireworkDurationSeconds;
			bundle.Main.VictoryPhaseSeconds = pro.VictoryPhaseSeconds;
			bundle.Main.BossPhaseEnabled = pro.BossPhaseEnabled;
			bundle.Main.BossTriggerMode = pro.BossTriggerMode;
			bundle.Main.BossTriggerPercent = pro.BossTriggerPercent;
			bundle.Main.BossTriggerDelaySeconds = pro.BossTriggerDelaySeconds;
			bundle.Main.AncientScreamEnabled = pro.AncientScreamEnabled;
			bundle.Main.AncientScreamTrigger = pro.AncientScreamTrigger;
			bundle.Main.AncientScreamDelaySeconds = pro.AncientScreamDelaySeconds;
			bundle.Main.RewardCrateTrigger = pro.RewardCrateTrigger;
			bundle.Main.FireworksTrigger = pro.FireworksTrigger;
			bundle.Main.FireworksDelaySeconds = pro.FireworksDelaySeconds;
			bundle.Main.VictoryEffectsDurationSeconds = pro.VictoryEffectsDurationSeconds;
			bundle.Main.EventObjectCleanupDelaySeconds = pro.EventObjectCleanupDelaySeconds;
			bundle.Main.RewardCrateLifetimeSeconds = pro.RewardCrateLifetimeSeconds;
			bundle.Main.RejectTraderSpawnZones = pro.RejectTraderSpawnZones;
			bundle.Main.TraderExclusionRadius = pro.TraderExclusionRadius;
		}

		ApplyUnifiedLocations(bundle, settings, locations);
		ApplyUnifiedRewards(bundle, settings);
		DZKOTH_Utils.Log("Unified KotHZ settings applied. License runtime=" + DZKOTH_Const.VERSION);
	}

	protected static void ApplyUnifiedLocations(DZKOTH_ConfigBundle bundle, DZKOTHF_Settings settings, DZKOTHF_LocationsSettings locations)
	{
		if (!bundle || !bundle.Locations || !bundle.Locations.Locations || !locations || !locations.Locations)
			return;

		bundle.Locations.Locations.Clear();
		foreach (DZKOTHF_LocationSetting source: locations.Locations)
		{
			if (!source || !source.IsValid())
				continue;

			DZKOTH_LocationConfig target = new DZKOTH_LocationConfig;
			vector center = source.GetPosition();
			vector orientation = source.GetOrientation();
			target.Enabled = 1;
			target.Name = source.Name;
			target.Radius = source.CaptureRadius;
			target.Center = DZKOTH_Utils.MakeVectorArray(center[0], center[1], center[2]);
			target.Position = DZKOTH_Utils.MakeVectorArray(center[0], center[1], center[2]);
			target.FlagPosition = DZKOTH_Utils.MakeVectorArray(center[0], center[1], center[2]);
			target.FlagOrientation = DZKOTH_Utils.MakeVectorArray(orientation[0], orientation[1], orientation[2]);
			vector bossPos = center + Vector(10.0, 0.0, 0.0);
			vector chestPos = center + settings.GetRewardCrateOffset();
			target.BossSpawnPosition = DZKOTH_Utils.MakeVectorArray(bossPos[0], bossPos[1], bossPos[2]);
			target.RewardCratePosition = DZKOTH_Utils.MakeVectorArray(chestPos[0], chestPos[1], chestPos[2]);
			target.RewardCrateOrientation = DZKOTH_Utils.MakeVectorArray(orientation[0], orientation[1], orientation[2]);
			target.ChestPosition = DZKOTH_Utils.MakeVectorArray(chestPos[0], chestPos[1], chestPos[2]);
			target.ChestOrientation = DZKOTH_Utils.MakeVectorArray(orientation[0], orientation[1], orientation[2]);
			bundle.Locations.Locations.Insert(target);
		}
	}

	protected static void ApplyUnifiedRewards(DZKOTH_ConfigBundle bundle, DZKOTHF_Settings settings)
	{
		if (!bundle || !bundle.Loot || !settings || !settings.RewardItems)
			return;
		// KotHZLoot.json is the authoritative, fully configurable reward pool.
		// The compact RewardItems list remains a fallback for installations that
		// do not provide a dedicated loot configuration.
		if (bundle.Loot.RewardCrateLoot && bundle.Loot.RewardCrateLoot.Count() > 0)
		{
			DZKOTH_Utils.Log("Dedicated KotHZLoot reward pool retained: " + bundle.Loot.RewardCrateLoot.Count().ToString() + " entries.");
			return;
		}

		bundle.Loot.RewardCrateLoot.Clear();
		foreach (DZKOTHF_RewardItemSetting source: settings.RewardItems)
		{
			if (!source || source.Type == "")
				continue;

			DZKOTH_LootEntry target = new DZKOTH_LootEntry;
			target.ClassName = source.Type;
			target.Chance = Math.Clamp(source.Chance * 100.0, 0.0, 100.0);
			target.Min = Math.Max(source.Count, 1);
			target.Max = Math.Max(source.Count, 1);
			target.MinQuantity = Math.Max(source.MinQuantity, 1);
			target.MaxQuantity = Math.Max(source.MaxQuantity, target.MinQuantity);
			target.Enabled = source.Enabled;
			target.MinHealthPercent = Math.Clamp(source.MinHealthPercent, 1.0, 100.0);
			target.MaxHealthPercent = Math.Clamp(source.MaxHealthPercent, target.MinHealthPercent, 100.0);
			target.Attachments = source.Attachments;
			bundle.Loot.RewardCrateLoot.Insert(target);
		}
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
		DZKOTH_ProfilePaths.Ensure();
		ref DZKOTH_WavesConfig config = new DZKOTH_WavesConfig;
		string errorMessage;
		if (!FileExist(DZKOTH_Const.PROFILE_WAVES_JSON))
		{
			if (!JsonFileLoader<ref DZKOTH_WavesConfig>.LoadFile(DZKOTH_Const.WAVES_JSON, config, errorMessage))
				DZKOTH_Utils.Warn("Could not load embedded waves config; defaults stay active. " + errorMessage);
			errorMessage = "";
			if (JsonFileLoader<ref DZKOTH_WavesConfig>.SaveFile(DZKOTH_Const.PROFILE_WAVES_JSON, config, errorMessage))
				DZKOTH_Utils.Log("Created editable waves config " + DZKOTH_Const.PROFILE_WAVES_JSON);
			else
				DZKOTH_Utils.Warn("Could not create editable waves config. " + errorMessage);
			return config;
		}
		if (!JsonFileLoader<ref DZKOTH_WavesConfig>.LoadFile(DZKOTH_Const.PROFILE_WAVES_JSON, config, errorMessage))
		{
			BackupInvalidProfileConfig(DZKOTH_Const.PROFILE_WAVES_JSON);
			DZKOTH_Utils.Warn("Could not load editable waves config; defaults stay active. " + errorMessage);
		}
		else if (MigrateWaveDefaults(config))
		{
			errorMessage = "";
			if (JsonFileLoader<ref DZKOTH_WavesConfig>.SaveFile(DZKOTH_Const.PROFILE_WAVES_JSON, config, errorMessage))
				DZKOTH_Utils.Log("Migrated legacy wave defaults to the current movement and health progression.");
			else
				DZKOTH_Utils.Warn("Could not save migrated wave movement settings. " + errorMessage);
		}
		return config;
	}

	protected static bool MigrateWaveDefaults(DZKOTH_WavesConfig config)
	{
		if (!config)
			return false;
		bool changed;
		if (config.WaveOne && Math.AbsFloat(config.WaveOne.ForcedHealth - 250.0) < 0.001 && config.WaveOne.Types && config.WaveOne.Types.Count() == 1 && config.WaveOne.Types[0] == "DZKOTH_Infected_250")
		{
			config.WaveOne.ForcedHealth = 0.0;
			config.WaveOne.MovementSpeedMultiplier = 1.00;
			config.WaveOne.DisableRunning = false;
			config.WaveOne.Types[0] = "ZmbM_HunterOld_Autumn";
			changed = true;
		}
		if (config.WaveTwo && Math.AbsFloat(config.WaveTwo.ForcedHealth - 400.0) < 0.001 && config.WaveTwo.Types && config.WaveTwo.Types.Count() == 1 && config.WaveTwo.Types[0] == "DZKOTH_Infected_400")
		{
			config.WaveTwo.ForcedHealth = 0.0;
			config.WaveTwo.MovementSpeedMultiplier = 1.00;
			config.WaveTwo.DisableRunning = false;
			config.WaveTwo.Types[0] = "ZmbM_PolicemanSpecForce_Heavy";
			changed = true;
		}
		if (config.WaveThree && (Math.AbsFloat(config.WaveThree.ForcedHealth - 600.0) < 0.001 || Math.AbsFloat(config.WaveThree.MovementSpeedMultiplier - 0.80) < 0.001))
		{
			config.WaveThree.ForcedHealth = 250.0;
			config.WaveThree.MovementSpeedMultiplier = 1.00;
			config.WaveThree.DisableRunning = true;
			changed = true;
		}
		if (config.WaveFour && (Math.AbsFloat(config.WaveFour.ForcedHealth - 800.0) < 0.001 || Math.AbsFloat(config.WaveFour.MovementSpeedMultiplier - 0.65) < 0.001))
		{
			config.WaveFour.ForcedHealth = 500.0;
			config.WaveFour.MovementSpeedMultiplier = 1.00;
			config.WaveFour.DisableRunning = true;
			changed = true;
		}
		if (config.WaveFive && Math.AbsFloat(config.WaveFive.MovementSpeedMultiplier - 0.55) < 0.001)
		{
			config.WaveFive.MovementSpeedMultiplier = 1.00;
			config.WaveFive.DisableRunning = true;
			changed = true;
		}
		return changed;
	}

	static ref DZKOTH_LootConfig LoadLootConfig()
	{
		DZKOTH_ProfilePaths.Ensure();
		ref DZKOTH_LootConfig config = new DZKOTH_LootConfig;
		string errorMessage;
		if (!FileExist(DZKOTH_Const.PROFILE_LOOT_JSON))
		{
			if (!JsonFileLoader<ref DZKOTH_LootConfig>.LoadFile(DZKOTH_Const.LOOT_JSON, config, errorMessage))
				DZKOTH_Utils.Warn("Could not load embedded loot config; defaults stay active. " + errorMessage);
			errorMessage = "";
			if (JsonFileLoader<ref DZKOTH_LootConfig>.SaveFile(DZKOTH_Const.PROFILE_LOOT_JSON, config, errorMessage))
				DZKOTH_Utils.Log("Created editable loot config " + DZKOTH_Const.PROFILE_LOOT_JSON);
			else
				DZKOTH_Utils.Warn("Could not create editable loot config. " + errorMessage);
			return config;
		}
		if (!JsonFileLoader<ref DZKOTH_LootConfig>.LoadFile(DZKOTH_Const.PROFILE_LOOT_JSON, config, errorMessage))
		{
			BackupInvalidProfileConfig(DZKOTH_Const.PROFILE_LOOT_JSON);
			DZKOTH_Utils.Warn("Could not load editable loot config; defaults stay active. " + errorMessage);
		}
		return config;
	}

protected static void BackupInvalidProfileConfig(string path)
	{
		if (!FileExist(path))
			return;

		int stamp = 0;
		if (GetGame())
			stamp = GetGame().GetTime();

		string backupPath = path + ".invalid_" + stamp.ToString();
		if (CopyFile(path, backupPath))
			DZKOTH_Utils.Warn("Invalid profile config copied to " + backupPath);
		else
			DZKOTH_Utils.Warn("Invalid profile config could not be copied before defaults were used: " + path);
	}

}
