class DZSPAWN_RPC
{
	static const int REQUEST_RESPAWN_MODE = 974210;
	static const int RESPAWN_DENIED = 974211;
	static const int RESPAWN_ACCEPTED = 974212;
}

class DZSPAWN_Mode
{
	static const string RANDOM = "RANDOM";
	static const string COAST = "COAST";
	static const string INLAND = "INLAND";
	static const string TRADER_NEAR = "TRADER_NEAR";
	static const string SAFE_RANDOM = "SAFE_RANDOM";
	static const string ADMIN_TEST = "ADMIN_TEST";
	static const string DISABLED_KEEP_GEAR = "DISABLED_KEEP_GEAR";

	static bool IsKnown(string mode)
	{
		return mode == RANDOM || mode == COAST || mode == INLAND || mode == TRADER_NEAR || mode == SAFE_RANDOM || mode == ADMIN_TEST || mode == DISABLED_KEEP_GEAR;
	}
}

class DZSPAWN_Paths
{
	static const string PROFILE_NAME = "DeutschZ_Main_Settings";
	static const string PROFILE_ROOT = "$profile:DeutschZ-System";
	static const string PROFILE_DIR = "$profile:DeutschZ-System/DeutschZ_Main_Settings";
	static const string CONFIG_DIR = "$profile:DeutschZ-System/DeutschZ_Main_Settings/Config";
	static const string DATA_DIR = "$profile:DeutschZ-System/DeutschZ_Main_Settings/Data";
	static const string RUNTIME_DIR = "$profile:DeutschZ-System/DeutschZ_Main_Settings/Runtime";
	static const string PERSISTENCE_DIR = "$profile:DeutschZ-System/DeutschZ_Main_Settings/Persistence";
	static const string LOG_ROOT = "$profile:DeutschZ-System/LogZ";
	static const string LOG_DIR = "$profile:DeutschZ-System/LogZ/DeutschZ_Main_Settings";
	static const string LOG_FILE = "$profile:DeutschZ-System/LogZ/DeutschZ_Main_Settings/main_settings.log";
	static const string MIGRATION_LOG = "$profile:DeutschZ-System/LogZ/DeutschZ_Main_Settings/profile_migration.log";
	static const string SETTINGS = "$profile:DeutschZ-System/DeutschZ_Main_Settings/Config/SpawnSettings.json";
	static const string POINTS = "$profile:DeutschZ-System/DeutschZ_Main_Settings/Data/SpawnPoints_Chernarus.json";
	static const string LOADOUTS = "$profile:DeutschZ-System/DeutschZ_Main_Settings/Config/Loadouts.json";
}

class DZMS_ProfilePaths
{
	static void EnsureDirectory(string path)
	{
		if (!FileExist(path))
			MakeDirectory(path);
	}

	static void Ensure()
	{
		EnsureDirectory(DZSPAWN_Paths.PROFILE_ROOT);
		EnsureDirectory(DZSPAWN_Paths.LOG_ROOT);
		EnsureDirectory(DZSPAWN_Paths.PROFILE_DIR);
		EnsureDirectory(DZSPAWN_Paths.CONFIG_DIR);
		EnsureDirectory(DZSPAWN_Paths.DATA_DIR);
		EnsureDirectory(DZSPAWN_Paths.RUNTIME_DIR);
		EnsureDirectory(DZSPAWN_Paths.PERSISTENCE_DIR);
		EnsureDirectory(DZSPAWN_Paths.LOG_DIR);

		MigrateFile("$profile:DeutschZ/SpawnSystem/SpawnSettings.json", DZSPAWN_Paths.SETTINGS);
		MigrateFile("$profile:DeutschZ/SpawnSystem/SpawnPoints_Chernarus.json", DZSPAWN_Paths.POINTS);
		MigrateFile("$profile:DeutschZ/SpawnSystem/Loadouts.json", DZSPAWN_Paths.LOADOUTS);
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
		FileHandle file = OpenFile(DZSPAWN_Paths.MIGRATION_LOG, FileMode.APPEND);
		if (file != 0)
		{
			FPrintln(file, "[DeutschZ-System][" + DZSPAWN_Paths.PROFILE_NAME + "] " + message);
			CloseFile(file);
		}
	}

	static void AppendLog(string message)
	{
		FileHandle file = OpenFile(DZSPAWN_Paths.LOG_FILE, FileMode.APPEND);
		if (file != 0)
		{
			FPrintln(file, "[DeutschZ Main Settings] " + message);
			CloseFile(file);
		}
	}
}

class DZSPAWN_Log
{
	static void Info(string message)
	{
		Print("[DeutschZ SpawnSystem] " + message);
		DZMS_ProfilePaths.AppendLog("SpawnSystem " + message);
	}

	static void Warn(string message)
	{
		Print("[DeutschZ SpawnSystem][WARN] " + message);
		DZMS_ProfilePaths.AppendLog("SpawnSystem WARN " + message);
	}

	static void Error(string message)
	{
		Print("[DeutschZ SpawnSystem][ERROR] " + message);
		DZMS_ProfilePaths.AppendLog("SpawnSystem ERROR " + message);
	}
}
