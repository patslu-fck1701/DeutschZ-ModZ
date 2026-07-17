class DZUIAZ_Constants
{
	static const string VERSION = "2.0.0-admin-pro-shell";
	static const int MENU_ID = 987610;
	static const int RPC_EVENT_ADMIN = 24872;
	static const int RPC_UIADMIN = 24873;
	static const int UI_RPC_PROTOCOL_VERSION = 2;
	static const int EVENT_RPC_PROTOCOL_VERSION = 1;
	static const string PROFILE_ROOT = "$profile:DeutschZ-System";
	static const string CONFIG_ROOT = "$profile:DeutschZ-System/DeutschZ_UiAdminZ";
	static const string LOG_SYSTEM_ROOT = "$profile:DeutschZ-System/LogZ";
	static const string LOG_ROOT = "$profile:DeutschZ-System/LogZ/DeutschZ_UiAdminZ";
	static const string SETTINGS_PATH = "$profile:DeutschZ-System/DeutschZ_UiAdminZ/UiAdminZSettings.json";
	static const string CORRUPT_BACKUP_PATH = "$profile:DeutschZ-System/DeutschZ_UiAdminZ/UiAdminZSettings.corrupt.backup.json";
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
	bool Enabled;
	bool AuditLogEnabled;
	bool ProDevelopmentEnabled;
	int RequestRateLimitMilliseconds;
	ref array<string> AdminSteamIds;

	void DZUIAZ_Settings()
	{
		Enabled = true;
		AuditLogEnabled = true;
		ProDevelopmentEnabled = false;
		RequestRateLimitMilliseconds = 1000;
		AdminSteamIds = {"76561199819501556"};
	}

	void Normalize()
	{
		if (RequestRateLimitMilliseconds < 500)
			RequestRateLimitMilliseconds = 500;
		if (RequestRateLimitMilliseconds > 10000)
			RequestRateLimitMilliseconds = 10000;
		if (!AdminSteamIds)
			AdminSteamIds = {"76561199819501556"};
	}

	bool IsAdmin(string steamId)
	{
		return Enabled && steamId != "" && AdminSteamIds && AdminSteamIds.Find(steamId) != -1;
	}
}

class DZUIAZ_Profile
{
	static void EnsureDirectories()
	{
		EnsureDirectory(DZUIAZ_Constants.PROFILE_ROOT);
		EnsureDirectory(DZUIAZ_Constants.CONFIG_ROOT);
		EnsureDirectory(DZUIAZ_Constants.LOG_SYSTEM_ROOT);
		EnsureDirectory(DZUIAZ_Constants.LOG_ROOT);
	}

	protected static void EnsureDirectory(string path)
	{
		if (!FileExist(path) && !MakeDirectory(path))
			Print("[DeutschZ UiAdminZ] ERROR: Verzeichnis konnte nicht erstellt werden: " + path);
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
		if (file != 0)
		{
			FPrintln(file, line);
			CloseFile(file);
		}
	}
}

class DZUIAZ_SettingsLoader
{
	static ref DZUIAZ_Settings Load()
	{
		DZUIAZ_Profile.EnsureDirectories();
		ref DZUIAZ_Settings settings = new DZUIAZ_Settings;
		string error;
		if (!FileExist(DZUIAZ_Constants.SETTINGS_PATH))
		{
			JsonFileLoader<ref DZUIAZ_Settings>.SaveFile(DZUIAZ_Constants.SETTINGS_PATH, settings, error);
			DZUIAZ_Log.Write("INFO", "Settings einmalig neu erzeugt: " + DZUIAZ_Constants.SETTINGS_PATH);
			return settings;
		}

		if (!JsonFileLoader<ref DZUIAZ_Settings>.LoadFile(DZUIAZ_Constants.SETTINGS_PATH, settings, error) || !settings)
		{
			bool backupCreated = CopyFile(DZUIAZ_Constants.SETTINGS_PATH, DZUIAZ_Constants.CORRUPT_BACKUP_PATH);
			DZUIAZ_Log.Write("ERROR", "Settings ungueltig. Backup=" + backupCreated.ToString() + " Fehler=" + error);
			settings = new DZUIAZ_Settings;
			JsonFileLoader<ref DZUIAZ_Settings>.SaveFile(DZUIAZ_Constants.SETTINGS_PATH, settings, error);
			return settings;
		}

		settings.Normalize();
		DZUIAZ_Log.Write("INFO", "Vorhandene gueltige Settings unveraendert geladen.");
		return settings;
	}
}

class DZUIAZ_ClientState
{
	static bool ModuleAvailable;
	static bool Authorized;
	static bool ProDevelopmentEnabled;
	static bool AuthorizationResponseReceived;
	static bool PendingOpen;
	static int AuthorizationRequestedAt;
	static string AdminName = "UNBEKANNT";
	static string LicenseState = "PRO GESPERRT - LICENSECORE NICHT IMPLEMENTIERT";
	static string EventState = "EVENTMOD NICHT GELADEN";
	static vector EventPosition;
	static float Radius;
	static float Progress;
	static int EnemyCount;
	static bool RewardReady;
	static string Result = "F7: Serverberechtigung wird angefordert";

	static void Reset()
	{
		ModuleAvailable = false;
		Authorized = false;
		ProDevelopmentEnabled = false;
		AuthorizationResponseReceived = false;
		PendingOpen = false;
		AuthorizationRequestedAt = 0;
		AdminName = "UNBEKANNT";
		LicenseState = "PRO GESPERRT - LICENSECORE NICHT IMPLEMENTIERT";
		EventState = "EVENTMOD NICHT GELADEN";
		EventPosition = vector.Zero;
		Radius = 0;
		Progress = 0;
		EnemyCount = 0;
		RewardReady = false;
		Result = "Keine Eventdaten empfangen";
	}
}
