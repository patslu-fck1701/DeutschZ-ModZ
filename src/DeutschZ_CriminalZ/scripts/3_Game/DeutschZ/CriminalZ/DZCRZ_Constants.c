class DZCRZ_Const
{
	static const int MODULE_ATM = 1;
	static const int MODULE_GARAGE = 2;
	static const int MODULE_VEHICLE = 3;
	static const int STATE_PENDING = 0;
	static const int STATE_ACTIVE = 1;
	static const int STATE_COMPLETED = 2;
	static const int STATE_CANCELLED = 3;
	static const int CANCEL_NONE = 0;
	static const int CANCEL_ACTION_ENDED = 1;
	static const int CANCEL_PLAYER_INVALID = 2;
	static const int CANCEL_TOO_FAR = 3;
	static const int CANCEL_TOOL_MISSING = 4;
	static const int CANCEL_TARGET_INVALID = 5;
	static const int CANCEL_SERVER_SHUTDOWN = 6;
	static const int CANCEL_ADMIN = 7;
	static const string VERSION = "0.1.1";
	static const string PREFIX = "[DZCRZ] ";
	static const string ATM_CLASS = "DZCRZ_ATM";
	static const string ATM_TOOL_CLASS = "RobTool_Crowbar";
	static const string HACK_TOOL_CLASS = "HackingTool_Lockpick";
	static const string TOKEN_CLASS = "CriminalZ_Token";
	static const string ATM_SOUNDSET = "UndergroundDoor_Alarm_Loop_SoundSet";
	static const string VEHICLE_SOUNDSET = "Offroad_Horn_SoundSet";
	static const string MARKER_ICON = "Schraubenschlussel";
	static const int MARKER_COLOR = ARGB(255, 220, 25, 25);
	static const int RPC_ADMIN_COMMAND = 479510;
	static const int RPC_CLIENT_PROGRESS = 479511;
	static const int RPC_CLIENT_MESSAGE = 479512;
	static const int RPC_CLIENT_EFFECT = 479513;
	static const int SERVER_TICK_MS = 1000;
	static const int STORE_SCAN_DELAY_MS = 15000;
}
