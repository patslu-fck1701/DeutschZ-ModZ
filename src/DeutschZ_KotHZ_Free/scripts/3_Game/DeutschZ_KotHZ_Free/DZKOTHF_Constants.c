class DZKOTHF_Constants
{
	static const string VERSION = "1.0.0-clientfix-2";
	static const int RPC_SYNC = 24871;
	static const int RPC_ADMIN = 24872;
	static const int RPC_PROTOCOL_VERSION = 1;
	static const string LOG_PREFIX = "[DeutschZ KotHZ Free] ";
	static const string PROFILE_SYSTEM_ROOT = "$profile:DeutschZ-System";
	static const string CONFIG_ROOT = "$profile:DeutschZ-System/DeutschZ_KotHZ";
	static const string LOG_SYSTEM_ROOT = "$profile:DeutschZ-System/LogZ";
	static const string LOG_ROOT = "$profile:DeutschZ-System/LogZ/DeutschZ_KotHZ";
	static const string SETTINGS_PATH = "$profile:DeutschZ-System/DeutschZ_KotHZ/KotHZSettings.json";
	static const string LOG_PATH = "$profile:DeutschZ-System/LogZ/DeutschZ_KotHZ/KotHZ.log";
	static const string PREVIOUS_SETTINGS_PATH = "$profile:DeutschZ-System/DeutschZ_KotHZ/KotHZFreeSettings.json";
	static const string LEGACY_PROFILE_ROOT = "$profile:DeutschZ";
	static const string LEGACY_CONFIG_ROOT = "$profile:DeutschZ/KotHZ_Free";
	static const string LEGACY_SETTINGS_PATH = "$profile:DeutschZ/KotHZ_Free/KotHZFreeSettings.json";
	static const string FLAGPOLE_CLASSNAME = "DZKOTHF_EventFlagpole";
	static const string FLAG_CLASSNAME = "DZKOTHF_EventFlag";
	static const string DEFAULT_REWARD_CRATE_CLASSNAME = "DZKOTHF_RewardCrate";
	static const string DEFAULT_MARKER_ICON = "\\dz\\gear\\navigation\\data\\map_tree_ca.paa";
}

enum DZKOTHF_RPCMessage
{
	NOTIFY = 1,
	MARKER = 2,
	PROGRESS = 3
}

enum DZKOTHF_AdminAction
{
	STATUS = 1,
	START = 2,
	STOP = 3
}

class DZKOTHF_ClientState
{
	static bool MarkerVisible;
	static vector MarkerPosition;
	static string MarkerText;
	static string MarkerIcon;
	static int Revision;
	static bool ProgressVisible;
	static string ProgressEventName;
	static string ProgressStatus;
	static float ProgressValue;
	static int ProgressRevision;

	static void SetMarker(bool visible, vector position, string text, string icon)
	{
		MarkerVisible = visible;
		MarkerPosition = position;
		MarkerText = text;
		MarkerIcon = icon;
		Revision++;
	}

	static void SetProgress(bool visible, string eventName, string status, float value)
	{
		ProgressVisible = visible;
		ProgressEventName = eventName;
		ProgressStatus = status;
		ProgressValue = Math.Clamp(value, 0.0, 1.0);
		ProgressRevision++;
	}

	static void Reset()
	{
		SetMarker(false, vector.Zero, "", "");
		SetProgress(false, "", "", 0.0);
	}
}

class DZKOTHF_ProfilePaths
{
	static void EnsureDirectories()
	{
		EnsureDirectory(DZKOTHF_Constants.PROFILE_SYSTEM_ROOT);
		EnsureDirectory(DZKOTHF_Constants.CONFIG_ROOT);
		EnsureDirectory(DZKOTHF_Constants.LOG_SYSTEM_ROOT);
		EnsureDirectory(DZKOTHF_Constants.LOG_ROOT);
	}

	protected static void EnsureDirectory(string path)
	{
		if (!FileExist(path) && !MakeDirectory(path))
			Print(DZKOTHF_Constants.LOG_PREFIX + "ERROR: Directory could not be created: " + path);
	}
}

class DZKOTHF_Log
{
	static void Info(string message)
	{
		Write("INFO", message);
	}

	static void Warning(string message)
	{
		Write("WARNING", message);
	}

	static void Error(string message)
	{
		Write("ERROR", message);
	}

	protected static void Write(string level, string message)
	{
		string line = DZKOTHF_Constants.LOG_PREFIX + level + ": " + message;
		Print(line);
		DZKOTHF_ProfilePaths.EnsureDirectories();
		FileHandle file = OpenFile(DZKOTHF_Constants.LOG_PATH, FileMode.APPEND);
		if (file != 0)
		{
			FPrintln(file, line);
			CloseFile(file);
		}
	}
}
