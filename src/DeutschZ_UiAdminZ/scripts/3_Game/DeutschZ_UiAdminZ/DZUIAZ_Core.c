class DZUIAZ_Constants
{
	static const int MENU_ID = 987610;
	static const int RPC_ADMIN = 24872;
	static const int RPC_PROTOCOL_VERSION = 1;
	static const string PROFILE_ROOT = "$profile:DeutschZ-System";
	static const string CONFIG_ROOT = "$profile:DeutschZ-System/DeutschZ_UiAdminZ";
	static const string LOG_SYSTEM_ROOT = "$profile:DeutschZ-System/LogZ";
	static const string LOG_ROOT = "$profile:DeutschZ-System/LogZ/DeutschZ_UiAdminZ";
	static const string SETTINGS_PATH = "$profile:DeutschZ-System/DeutschZ_UiAdminZ/UiAdminZSettings.json";
	static const string LOG_PATH = "$profile:DeutschZ-System/LogZ/DeutschZ_UiAdminZ/UiAdminZ.log";
}

enum DZUIAZ_AdminAction
{
	STATUS = 1,
	START = 2,
	STOP = 3
}

class DZUIAZ_Settings
{
	bool Enabled = true;
	bool AuditLogEnabled = true;
}

class DZUIAZ_Profile
{
	static void EnsureDirectories()
	{
		if (!FileExist(DZUIAZ_Constants.PROFILE_ROOT)) MakeDirectory(DZUIAZ_Constants.PROFILE_ROOT);
		if (!FileExist(DZUIAZ_Constants.CONFIG_ROOT)) MakeDirectory(DZUIAZ_Constants.CONFIG_ROOT);
		if (!FileExist(DZUIAZ_Constants.LOG_SYSTEM_ROOT)) MakeDirectory(DZUIAZ_Constants.LOG_SYSTEM_ROOT);
		if (!FileExist(DZUIAZ_Constants.LOG_ROOT)) MakeDirectory(DZUIAZ_Constants.LOG_ROOT);
	}
}

class DZUIAZ_Log
{
	static void Write(string level, string message)
	{
		string line = "[DeutschZ UiAdminZ] " + level + ": " + message;
		Print(line);
		DZUIAZ_Profile.EnsureDirectories();
		FileHandle file = OpenFile(DZUIAZ_Constants.LOG_PATH, FileMode.APPEND);
		if (file != 0) {FPrintln(file, line); CloseFile(file);}
	}
}

class DZUIAZ_SettingsLoader
{
	static ref DZUIAZ_Settings Load()
	{
		DZUIAZ_Profile.EnsureDirectories();
		ref DZUIAZ_Settings settings = new DZUIAZ_Settings;
		string error;
		if (FileExist(DZUIAZ_Constants.SETTINGS_PATH))
		{
			if (!JsonFileLoader<ref DZUIAZ_Settings>.LoadFile(DZUIAZ_Constants.SETTINGS_PATH, settings, error) || !settings)
			{
				DZUIAZ_Log.Write("ERROR", "Settings ungueltig; sichere Defaults werden verwendet. " + error);
				settings = new DZUIAZ_Settings;
			}
		}
		JsonFileLoader<ref DZUIAZ_Settings>.SaveFile(DZUIAZ_Constants.SETTINGS_PATH, settings, error);
		return settings;
	}
}

class DZUIAZ_ClientState
{
	static bool ModuleAvailable;
	static bool Authorized;
	static string EventState = "KEINE ANTWORT";
	static vector EventPosition;
	static float Radius;
	static float Progress;
	static int EnemyCount;
	static bool RewardReady;
	static string Result = "F7: Status wird serverseitig angefordert";

	static void Reset()
	{
		ModuleAvailable = false; Authorized = false; EventState = "KEINE ANTWORT";
		EventPosition = vector.Zero; Radius = 0; Progress = 0; EnemyCount = 0; RewardReady = false;
		Result = "Keine Antwort vom Eventmodul";
	}
}
