enum DZCourierZ_State
{
	IDLE,
	CASE_PICKUP,
	KEY_PICKUP,
	AI_STOP_1,
	AI_STOP_2,
	INFECTED_STOP,
	FINAL_DELIVERY,
	COOLDOWN
}

enum DZCourierZ_RPCMessage
{
	PLAY_SOUND = 1,
	OPEN_REWARD = 2,
	SELECT_REWARD = 3,
	SET_ROUTE_MARKER = 4
}

enum DZCourierZ_RewardChoice
{
	KEEP_CASE = 1,
	SECRET_ITEM = 2,
	MONEY = 3,
	SECRET_HINT = 4
}

class DZCourierZ_Constants
{
	static const int RPC = 1701250100;
	static const int REWARD_MENU_ID = 170125;
	static const float VOICE_RADIUS = 25.0;
}

class DZCourierZ_Settings
{
	bool Enabled = true;
	int InitialDelaySeconds = 30;
	int CooldownSeconds = 10800;
	int StageTimeoutSeconds = 3600;
	float ActivationRadius = 300.0;
	float HandoverRadius = 8.0;
	float TrackUpdateSeconds = 60.0;
	float RouteMarkerSpacing = 500.0;
	float RouteMarkerAdvanceRadius = 50.0;
	int InfectedCount = 20;
	vector CasePosition = "4475 317 6410";
	vector KeyPosition = "6548 304 9305";
	vector AIStop1Position = "8156.332520 474.133148 9108.544922";
	vector AIStop2Position = "9457.860352 303.992645 8816.887695";
	vector InfectedStopPosition = "11332.013672 93.703644 6727.682617";
	vector FinalPosition = "13342 97 5442";
	int InitialCaseBanknotes = 2500;
	int CashRewardBanknotes = 12500;
	string RewardCurrencyClass = "ExpansionBanknoteEuro";
	string SecretTruckClass = "A2_KamAZ_Covered";
	vector SecretTruckPosition = "13353.585938 6.024979 5458.135742";
	vector SecretTruckOrientation = "176.643982 0 0";
	string SecretHint = "Der Auftrag endet hier nicht. Suche nach dem schwarzen G-Klasse-Konvoi - seine Route fuehrt weiter.";
}

class DZCourierZ_Log
{
	static void Info(string message) { Print("[DeutschZ CourierZ] " + message); }
	static void Warn(string message) { Print("[DeutschZ CourierZ][WARN] " + message); }
}
