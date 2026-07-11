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
	static const string PROFILE_ROOT = "$profile:DeutschZ";
	static const string PROFILE_DIR = "$profile:DeutschZ/SpawnSystem";
	static const string SETTINGS = "$profile:DeutschZ/SpawnSystem/SpawnSettings.json";
	static const string POINTS = "$profile:DeutschZ/SpawnSystem/SpawnPoints_Chernarus.json";
	static const string LOADOUTS = "$profile:DeutschZ/SpawnSystem/Loadouts.json";
}
