class DZKOTH_Const
{
	static const string VERSION = "0.5.0-v2-pro";
	static const string LOG_PREFIX = "[DeutschZ_KotH] ";
	static const string MOD_PREFIX = "DeutschZ_KotH_InfectedSiedge_v2";
	static const string PROFILE_NAME = "DeutschZ_KotH_InfectedSiedge_v2";

	static const string CONFIG_JSON = "DeutschZ_KotH_InfectedSiedge_v2/data/dzkoth_config.json";
	static const string LOCATIONS_JSON = "DeutschZ_KotH_InfectedSiedge_v2/data/dzkoth_locations.json";
	static const string WAVES_JSON = "DeutschZ_KotH_InfectedSiedge_v2/data/dzkoth_waves.json";
	static const string LOOT_JSON = "DeutschZ_KotH_InfectedSiedge_v2/data/dzkoth_loot.json";

	static const string PROFILE_DIR = "$profile:DeutschZ-System/DeutschZ_KotH_InfectedSiedge_v2";
	static const string PROFILE_CONFIG_DIR = "$profile:DeutschZ-System/DeutschZ_KotH_InfectedSiedge_v2";
	static const string PROFILE_DATA_DIR = "$profile:DeutschZ-System/DeutschZ_KotH_InfectedSiedge_v2/Data";
	static const string PROFILE_RUNTIME_DIR = "$profile:DeutschZ-System/DeutschZ_KotH_InfectedSiedge_v2/Runtime";
	static const string PROFILE_PERSISTENCE_DIR = "$profile:DeutschZ-System/DeutschZ_KotH_InfectedSiedge_v2/Persistence";
	static const string PROFILE_LOG_DIR = "$profile:DeutschZ-System/LogZ/DeutschZ_KotH_InfectedSiedge_v2";
	static const string PROFILE_LOG_FILE = "$profile:DeutschZ-System/LogZ/DeutschZ_KotH_InfectedSiedge_v2/KotHZ.log";
	static const string PROFILE_MIGRATION_LOG = "$profile:DeutschZ-System/LogZ/DeutschZ_Main_Settings/profile_migration.log";
	static const string PROFILE_STATE_JSON = "$profile:DeutschZ-System/DeutschZ_KotH_InfectedSiedge_v2/Runtime/state.json";
	static const string PROFILE_LOCATION_ROTATION_JSON = "$profile:DeutschZ-System/DeutschZ_KotH_InfectedSiedge_v2/KotHZLocationRotation.json";
	static const string PROFILE_KOTH_DIR = "$profile:DeutschZ-System/DeutschZ_KotH_InfectedSiedge_v2";
	static const string PROFILE_KOTH_CONFIG_JSON = "$profile:DeutschZ-System/DeutschZ_KotH_InfectedSiedge_v2/KotHZSettings.json";
	static const string PROFILE_GROUPS_JSON = "$profile:DeutschZ-System/DeutschZ_KotH_InfectedSiedge_v2/Persistence/groups.json";

	static const string MAIN_MARKER_UID = "DZKOTH_MAIN_EVENT";
	static const string BOSS_NAME = "BosZ Zombie";
	static const string BOSS_CLASSNAME = "DZKOTH_BosZZombie";
	static const string KEYCARD_CLASSNAME = "DZKOTH_BattlegroundZ_Keycard";
	static const string REWARD_CRATE_CLASSNAME = "DZKOTH_RewardChest";
	static const string FIREWORKS_BATTERY_CLASSNAME = "DeutschZ_FireworksLauncher";
	static const string BOSS_CORPSE_CLASSNAME = "DZKOTH_BossCorpse";
	static const string FLAGPOLE_CLASSNAME = "DZKOTH_EventFlagpole";
	static const string FLAG_CLASSNAME = "DZKOTH_EventFlag";
	static const string MARKER_ICON_NAME = "Skull 1";
	static const string MARKER_ICON_PATH = "dz/gear/navigation/data/map_tree_ca.paa";
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

		MigrateFile("$profile:DeutschZ-System/DeutschZ_KotH_InfectedSiedge_v2/Config/KotHConfig.json", DZKOTH_Const.PROFILE_KOTH_CONFIG_JSON);
		MigrateFile("$profile:KotH/KotHConfig.json", DZKOTH_Const.PROFILE_KOTH_CONFIG_JSON);
		MigrateFile("$profile:DZKOTH/state.json", DZKOTH_Const.PROFILE_STATE_JSON);
		MigrateFile("$profile:DZEventVanilla/groups.json", DZKOTH_Const.PROFILE_GROUPS_JSON);
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
