const string DZRMZ_VERSION = "2.1.0-final-audio";
const string DZRMZ_LOG_PREFIX = "[DeutschZ RadioMissionZ] ";

const string DZRMZ_PROFILE_ROOT = "$profile:DeutschZ-System/deutschz_radiomissionz";
const string DZRMZ_PROFILE_CONFIG_DIR = "$profile:DeutschZ-System/deutschz_radiomissionz/config";
const string DZRMZ_PROFILE_DATA_DIR = "$profile:DeutschZ-System/deutschz_radiomissionz/data";
const string DZRMZ_PROFILE_RUNTIME_DIR = "$profile:DeutschZ-System/deutschz_radiomissionz/runtime";
const string DZRMZ_PROFILE_PERSISTENCE_DIR = "$profile:DeutschZ-System/deutschz_radiomissionz/persistence";

const string DZRMZ_SETTINGS_PATH = "$profile:DeutschZ-System/deutschz_radiomissionz/config/radiomissionconfig.json";
const string DZRMZ_MISSIONS_PATH = "$profile:DeutschZ-System/deutschz_radiomissionz/data/missiondefinitions.json";
const string DZRMZ_LOCATIONS_PATH = "$profile:DeutschZ-System/deutschz_radiomissionz/data/locations.json";
const string DZRMZ_RUNTIME_PATH = "$profile:DeutschZ-System/deutschz_radiomissionz/runtime/activemission.json";
const string DZRMZ_HISTORY_PATH = "$profile:DeutschZ-System/deutschz_radiomissionz/persistence/missionhistory.json";
const string DZRMZ_LOG_PATH = "$profile:DeutschZ-System/deutschz_radiomissionz/runtime/radiomissionz.log";

const string DZRMZ_CATEGORY_CIVILIAN = "civilian";
const string DZRMZ_CATEGORY_SCAVENGER = "scavenger";
const string DZRMZ_CATEGORY_MILITARY = "military";
const string DZRMZ_TAG_DELIVERY = "delivery";

const string DZRMZ_OBJECTIVE_VISIT = "VISIT";
const string DZRMZ_OBJECTIVE_HOLD = "HOLD";
const string DZRMZ_OBJECTIVE_DELIVERY = "DELIVERY";

const string DZRMZ_SCENARIO_TRAPPED = "TRAPPED";
const string DZRMZ_SCENARIO_MEDICAL_AID = "MEDICAL_AID";
const string DZRMZ_SCENARIO_BESIEGED = "BESIEGED";
const string DZRMZ_SCENARIO_RESCUE = "RESCUE";
const int DZRMZ_RPC_PLAY_TRANSMISSION = 748951;

enum DZRMZ_EMissionState
{
	DZRMZ_STATE_IDLE = 0,
	DZRMZ_STATE_BROADCASTING,
	DZRMZ_STATE_ACTIVE,
	DZRMZ_STATE_COOLDOWN,
	DZRMZ_STATE_COMPLETING
}

class DZRMZ_ProfilePaths
{
	static void EnsureDirectory(string path)
	{
		if (!FileExist(path))
			MakeDirectory(path);
	}

	static void Ensure()
	{
		EnsureDirectory("$profile:DeutschZ-System");
		EnsureDirectory(DZRMZ_PROFILE_ROOT);
		EnsureDirectory(DZRMZ_PROFILE_CONFIG_DIR);
		EnsureDirectory(DZRMZ_PROFILE_DATA_DIR);
		EnsureDirectory(DZRMZ_PROFILE_RUNTIME_DIR);
		EnsureDirectory(DZRMZ_PROFILE_PERSISTENCE_DIR);
	}
}

class DZRMZ_Log
{
	static void Info(string message)
	{
		Write("INFO", message);
	}

	static void Warn(string message)
	{
		Write("WARN", message);
	}

	static void Error(string message)
	{
		Write("ERROR", message);
	}

	static void Debug(string message, bool enabled)
	{
		if (enabled)
			Write("DEBUG", message);
	}

	protected static void Write(string level, string message)
	{
		string line = DZRMZ_LOG_PREFIX + "[" + level + "] " + message;
		Print(line);

		DZRMZ_ProfilePaths.Ensure();
		FileHandle file = OpenFile(DZRMZ_LOG_PATH, FileMode.APPEND);
		if (file != 0)
		{
			FPrintln(file, line);
			CloseFile(file);
		}
	}
}


