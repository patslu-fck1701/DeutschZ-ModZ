const string DZBGZ_VERSION = "0.1.0";
const string DZBGZ_LOG_PREFIX = "[DZBGZ] ";
const string DZBGZ_PROFILE_NAME = "DeutschZ_BattlegroundZ";
const string DZBGZ_PROFILE_DIR = "$profile:DeutschZ-System/DeutschZ_BattlegroundZ";
const string DZBGZ_PROFILE_CONFIG_DIR = "$profile:DeutschZ-System/DeutschZ_BattlegroundZ/Config";
const string DZBGZ_PROFILE_DATA_DIR = "$profile:DeutschZ-System/DeutschZ_BattlegroundZ/Data";
const string DZBGZ_PROFILE_RUNTIME_DIR = "$profile:DeutschZ-System/DeutschZ_BattlegroundZ/Runtime";
const string DZBGZ_PROFILE_PERSISTENCE_DIR = "$profile:DeutschZ-System/DeutschZ_BattlegroundZ/Persistence";
const string DZBGZ_PROFILE_LOG_DIR = "$profile:DeutschZ-System/LogZ/DeutschZ_BattlegroundZ";
const string DZBGZ_LOG_FILE = "$profile:DeutschZ-System/LogZ/DeutschZ_BattlegroundZ/dzbgz.log";
const string DZBGZ_MIGRATION_LOG = "$profile:DeutschZ-System/LogZ/DeutschZ_Main_Settings/profile_migration.log";
const string DZBGZ_PROFILE_CONFIG_JSON = "$profile:DeutschZ-System/DeutschZ_BattlegroundZ/Config/dzbgz_config.json";
const string DZBGZ_UNLOCKS_JSON = "$profile:DeutschZ-System/DeutschZ_BattlegroundZ/Persistence/dzbgz_unlocks.json";
const string DZBGZ_KEYCARD_CLASSNAME = "DZKOTH_BattlegroundZ_Keycard";
const string DZBGZ_READER_CLASSNAME = "DZBGZ_CardReader";
const string DZBGZ_MARKER_UID = "DZBGZ_REVEALED_ZONE";
const string DZBGZ_READER_MARKER_UID = "DZBGZ_CARD_READER";
const string DZBGZ_ADMIN_COMMAND_PREFIX = "/dzbgz";
const string DZBGZ_DEFAULT_MARKER_ICON = "\\dz\\gear\\navigation\\data\\map_tree_ca.paa";

class DZBGZ_ProfilePaths
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
		EnsureDirectory(DZBGZ_PROFILE_DIR);
		EnsureDirectory(DZBGZ_PROFILE_CONFIG_DIR);
		EnsureDirectory(DZBGZ_PROFILE_DATA_DIR);
		EnsureDirectory(DZBGZ_PROFILE_RUNTIME_DIR);
		EnsureDirectory(DZBGZ_PROFILE_PERSISTENCE_DIR);
		EnsureDirectory(DZBGZ_PROFILE_LOG_DIR);

		MigrateFile("$profile:DZBGZ/dzbgz_config.json", DZBGZ_PROFILE_CONFIG_JSON);
		MigrateFile("$profile:DZBGZ/dzbgz_unlocks.json", DZBGZ_UNLOCKS_JSON);
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
		FileHandle file = OpenFile(DZBGZ_MIGRATION_LOG, FileMode.APPEND);
		if (file != 0)
		{
			FPrintln(file, "[DeutschZ-System][" + DZBGZ_PROFILE_NAME + "] " + message);
			CloseFile(file);
		}
	}

	static void AppendLog(string message)
	{
		FileHandle file = OpenFile(DZBGZ_LOG_FILE, FileMode.APPEND);
		if (file != 0)
		{
			FPrintln(file, DZBGZ_LOG_PREFIX + message);
			CloseFile(file);
		}
	}
}
