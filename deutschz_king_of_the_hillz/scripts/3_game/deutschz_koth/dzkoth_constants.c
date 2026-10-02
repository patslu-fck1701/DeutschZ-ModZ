class DZKOTH_Const
{
	static const string VERSION = "0.5.1-trader-smoke-fix";
	static const string LOG_PREFIX = "[DeutschZ KotHZ Licensed] ";
	static const string MOD_PREFIX = "deutschz_king_of_the_hillz";
	static const string PROFILE_NAME = "DeutschZ_KotHZ";

	static const string CONFIG_JSON = "deutschz_king_of_the_hillz/data/dzkoth_config.json";
	static const string LOCATIONS_JSON = "deutschz_king_of_the_hillz/data/dzkoth_locations.json";
	static const string WAVES_JSON = "deutschz_king_of_the_hillz/data/dzkoth_waves.json";
	static const string LOOT_JSON = "deutschz_king_of_the_hillz/data/dzkoth_loot.json";

	static const string PROFILE_DIR = "$profile:DeutschZ-System/DeutschZ_KotHZ";
	static const string PROFILE_CONFIG_DIR = "$profile:DeutschZ-System/DeutschZ_KotHZ/Config";
	static const string PROFILE_DATA_DIR = "$profile:DeutschZ-System/DeutschZ_KotHZ/Data";
	static const string PROFILE_RUNTIME_DIR = "$profile:DeutschZ-System/DeutschZ_KotHZ/Runtime";
	static const string PROFILE_PERSISTENCE_DIR = "$profile:DeutschZ-System/DeutschZ_KotHZ/Persistence";
	static const string PROFILE_LOG_DIR = "$profile:DeutschZ-System/LogZ/DeutschZ_KotHZ";
	static const string PROFILE_LOG_FILE = "$profile:DeutschZ-System/LogZ/DeutschZ_KotHZ/KotHZ.log";
	static const string PROFILE_MIGRATION_LOG = "$profile:DeutschZ-System/LogZ/DeutschZ_Main_Settings/profile_migration.log";
	static const string PROFILE_STATE_JSON = "$profile:DeutschZ-System/DeutschZ_KotHZ/Runtime/state.json";
	static const string PROFILE_LOCATION_ROTATION_JSON = "$profile:DeutschZ-System/DeutschZ_KotHZ/Persistence/KotHZLocationRotation.json";
	static const string PROFILE_WAVES_JSON = "$profile:DeutschZ-System/DeutschZ_KotHZ/Config/KotHZWaves.json";
	static const string PROFILE_LOOT_JSON = "$profile:DeutschZ-System/DeutschZ_KotHZ/Config/KotHZLoot.json";
	static const string PROFILE_GROUPS_JSON = "$profile:DeutschZ-System/DeutschZ_KotHZ/Persistence/groups.json";

	static const string MAIN_MARKER_UID = "DZKOTH_MAIN_EVENT";
	static const string BOSS_NAME = "BosZ Zombie";
	static const string BOSS_CLASSNAME = "DZKOTH_BosZZombie";
	static const string KEYCARD_CLASSNAME = "DZKOTH_BattlegroundZ_SecretDocument";
	static const string REWARD_CRATE_CLASSNAME = "DZKOTH_ProRewardCrate";
	static const string FIREWORKS_BATTERY_CLASSNAME = "DZKOTH_FireworksLauncher";
	static const string BOSS_CORPSE_CLASSNAME = "rag_teddybear";
	static const string BOSS_CORPSE_FALLBACK_CLASSNAME = "SmallProtectorCase";
	static const string FLAGPOLE_CLASSNAME = "DZKOTH_EventFlagpole";
	static const string FLAG_CLASSNAME = "DZKOTH_ProEventFlag";
	static const string MARKER_ICON_NAME = "Skull 1";
	static const string MARKER_ICON_PATH = "\\deutschz_king_of_the_hillz\\data\\kothz_flag_dayz_co.paa";
	static const string ADMIN_COMMAND_PREFIX = "!dzkoth";
	static const string ADMIN_COMMAND_PREFIX_ALT = "/dzkoth";
}

class DZKOTH_ProfilePaths
{
	static void EnsureDirectory(string path)
	{
		if (!FileExist(path))
			MakeDirectory(path);
	}

	static void Ensure()
	{
		EnsureDirectory("$profile:DeutschZ-System");
		EnsureDirectory("$profile:DeutschZ-System/LogZ");
		EnsureDirectory("$profile:DeutschZ-System/LogZ/DeutschZ_Main_Settings");
		EnsureDirectory(DZKOTH_Const.PROFILE_DIR);
		EnsureDirectory(DZKOTH_Const.PROFILE_CONFIG_DIR);
		EnsureDirectory(DZKOTH_Const.PROFILE_DATA_DIR);
		EnsureDirectory(DZKOTH_Const.PROFILE_RUNTIME_DIR);
		EnsureDirectory(DZKOTH_Const.PROFILE_PERSISTENCE_DIR);
		EnsureDirectory(DZKOTH_Const.PROFILE_LOG_DIR);

		// Only migrate files with the same schema. The base mod owns the main settings file.
		MigrateFile("$profile:DeutschZ-System/DeutschZ_KotHZ/KotHZLocationRotation.json", DZKOTH_Const.PROFILE_LOCATION_ROTATION_JSON);
		MigrateFile("$profile:DZKOTH/state.json", DZKOTH_Const.PROFILE_STATE_JSON);
		MigrateFile("$profile:DZEventVanilla/groups.json", DZKOTH_Const.PROFILE_GROUPS_JSON);
		MigrateFile("$profile:DeutschZ-System/deutschz_kothz/Runtime/state.json", DZKOTH_Const.PROFILE_STATE_JSON);
		MigrateFile("$profile:DeutschZ-System/deutschz_kothz/Persistence/KotHZLocationRotation.json", DZKOTH_Const.PROFILE_LOCATION_ROTATION_JSON);
		MigrateFile("$profile:DeutschZ-System/deutschz_kothz/Config/KotHZWaves.json", DZKOTH_Const.PROFILE_WAVES_JSON);
		MigrateFile("$profile:DeutschZ-System/deutschz_kothz/Config/KotHZLoot.json", DZKOTH_Const.PROFILE_LOOT_JSON);
		MigrateFile("$profile:DeutschZ-System/deutschz_kothz/Persistence/groups.json", DZKOTH_Const.PROFILE_GROUPS_JSON);
		MigrateFile("$profile:deutschz-system/deutschz_kothz/Runtime/state.json", DZKOTH_Const.PROFILE_STATE_JSON);
		MigrateFile("$profile:deutschz-system/deutschz_kothz/Persistence/KotHZLocationRotation.json", DZKOTH_Const.PROFILE_LOCATION_ROTATION_JSON);
		MigrateFile("$profile:deutschz-system/deutschz_kothz/Config/KotHZWaves.json", DZKOTH_Const.PROFILE_WAVES_JSON);
		MigrateFile("$profile:deutschz-system/deutschz_kothz/Config/KotHZLoot.json", DZKOTH_Const.PROFILE_LOOT_JSON);
		MigrateFile("$profile:deutschz-system/deutschz_kothz/Persistence/groups.json", DZKOTH_Const.PROFILE_GROUPS_JSON);
	}

	static void MigrateFile(string oldPath, string newPath)
	{
		if (FileExist(newPath) || !FileExist(oldPath))
			return;
		if (CopyFile(oldPath, newPath))
			AppendMigration("Migration: " + oldPath + " -> " + newPath);
		else
			AppendMigration("ERROR Migration fehlgeschlagen: " + oldPath + " -> " + newPath);
	}

	static void AppendMigration(string message)
	{
		FileHandle file = OpenFile(DZKOTH_Const.PROFILE_MIGRATION_LOG, FileMode.APPEND);
		if (file != 0)
		{
			FPrintln(file, "[DeutschZ-System][" + DZKOTH_Const.PROFILE_NAME + "] " + message);
			CloseFile(file);
		}
	}

	static void AppendLog(string message)
	{
		FileHandle file = OpenFile(DZKOTH_Const.PROFILE_LOG_FILE, FileMode.APPEND);
		if (file != 0)
		{
			FPrintln(file, DZKOTH_Const.LOG_PREFIX + message);
			CloseFile(file);
		}
	}
}
