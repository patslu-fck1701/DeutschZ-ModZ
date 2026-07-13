const string DZBBC_VERSION = "0.2.2-livefix";
const string DZBBC_LOG_PREFIX = "[DZBBC] ";
const string DZBBC_PROFILE_NAME = "DeutschZ_Blackbox_ConvoyZ";
const string DZBBC_PROFILE_DIR = "$profile:DeutschZ-System/DeutschZ_Blackbox_ConvoyZ";
const string DZBBC_PROFILE_CONFIG_DIR = "$profile:DeutschZ-System/DeutschZ_Blackbox_ConvoyZ/Config";
const string DZBBC_PROFILE_DATA_DIR = "$profile:DeutschZ-System/DeutschZ_Blackbox_ConvoyZ/Data";
const string DZBBC_PROFILE_RUNTIME_DIR = "$profile:DeutschZ-System/DeutschZ_Blackbox_ConvoyZ/Runtime";
const string DZBBC_PROFILE_PERSISTENCE_DIR = "$profile:DeutschZ-System/DeutschZ_Blackbox_ConvoyZ/Persistence";
const string DZBBC_PROFILE_LOG_DIR = "$profile:DeutschZ-System/LogZ/DeutschZ_Blackbox_ConvoyZ";
const string DZBBC_LOG_FILE = "$profile:DeutschZ-System/LogZ/DeutschZ_Blackbox_ConvoyZ/dzbbc.log";
const string DZBBC_MIGRATION_LOG = "$profile:DeutschZ-System/LogZ/DeutschZ_Main_Settings/profile_migration.log";
const string DZBBC_CONFIG_JSON = "$profile:DeutschZ-System/DeutschZ_Blackbox_ConvoyZ/Config/dzbbc_config.json";
const string DZBBC_LOCATIONS_JSON = "$profile:DeutschZ-System/DeutschZ_Blackbox_ConvoyZ/Config/dzbbc_locations.json";
const string DZBBC_AI_WAVES_JSON = "$profile:DeutschZ-System/DeutschZ_Blackbox_ConvoyZ/Config/dzbbc_ai_waves.json";
const string DZBBC_LOOT_JSON = "$profile:DeutschZ-System/DeutschZ_Blackbox_ConvoyZ/Config/dzbbc_loot.json";
const string DZBBC_TERMINALS_JSON = "$profile:DeutschZ-System/DeutschZ_Blackbox_ConvoyZ/Config/dzbbc_terminals.json";
const string DZBBC_MESSAGES_JSON = "$profile:DeutschZ-System/DeutschZ_Blackbox_ConvoyZ/Config/dzbbc_messages.json";

class DZBBC_ProfilePaths
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
		EnsureDirectory(DZBBC_PROFILE_DIR);
		EnsureDirectory(DZBBC_PROFILE_CONFIG_DIR);
		EnsureDirectory(DZBBC_PROFILE_DATA_DIR);
		EnsureDirectory(DZBBC_PROFILE_RUNTIME_DIR);
		EnsureDirectory(DZBBC_PROFILE_PERSISTENCE_DIR);
		EnsureDirectory(DZBBC_PROFILE_LOG_DIR);

		MigrateFile("$profile:DZBBC/dzbbc_config.json", DZBBC_CONFIG_JSON);
		MigrateFile("$profile:DZBBC/dzbbc_locations.json", DZBBC_LOCATIONS_JSON);
		MigrateFile("$profile:DZBBC/dzbbc_ai_waves.json", DZBBC_AI_WAVES_JSON);
		MigrateFile("$profile:DZBBC/dzbbc_loot.json", DZBBC_LOOT_JSON);
		MigrateFile("$profile:DZBBC/dzbbc_terminals.json", DZBBC_TERMINALS_JSON);
		MigrateFile("$profile:DZBBC/dzbbc_messages.json", DZBBC_MESSAGES_JSON);
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
		FileHandle file = OpenFile(DZBBC_MIGRATION_LOG, FileMode.APPEND);
		if (file != 0)
		{
			FPrintln(file, "[DeutschZ-System][" + DZBBC_PROFILE_NAME + "] " + message);
			CloseFile(file);
		}
	}

	static void AppendLog(string message)
	{
		FileHandle file = OpenFile(DZBBC_LOG_FILE, FileMode.APPEND);
		if (file != 0)
		{
			FPrintln(file, DZBBC_LOG_PREFIX + message);
			CloseFile(file);
		}
	}
}

const string DZBBC_MOD_ROOT = "deutschz_blackbox_convoyz";
const string DZBBC_DEFAULT_MARKER_ICON = "\\dz\\gear\\navigation\\data\\map_tree_ca.paa";
const string DZBBC_CRASHSITE_MARKER_UID = "DZBBC_CRASHSITE";
const string DZBBC_TERMINAL_MARKER_UID = "DZBBC_TERMINAL";
const string DZBBC_TRANSPORT_MARKER_UID = "DZBBC_TRANSPORT_HINT";

const string DZBBC_BLACKBOX_CLASSNAME = "DZBBC_Blackbox";
const string DZBBC_DATACORE_CLASSNAME = "DZBBC_DataCore";
const string DZBBC_TERMINAL_CLASSNAME = "DZBBC_MilitaryTerminal";
const string DZBBC_CONTAINER_CLASSNAME = "DZBBC_SecureSupplyContainer";
const string DZBBC_GASZONE_FLARE_CLASSNAME = "GasZonen_Leuchtfackel";
const string DZBBC_SIGNAL_MATCHES_CLASSNAME = "DZBBC_SignalMatches";
const string DZBBC_CODED_9V_CLASSNAME = "DZBBC_Coded9VBattery";
const string DZBBC_MARKER_OBJECT_CLASSNAME = "DZBBC_EventMarkerObject";
const string DZBBC_WRECK_PROXY_CLASSNAME = "DZBBC_WreckProxy";

const string DZBBC_ADMIN_COMMAND_PREFIX = "/dzbbc";
