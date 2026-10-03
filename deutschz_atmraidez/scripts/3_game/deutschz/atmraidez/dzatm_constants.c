class DZATM_Const
{
    static const string VERSION = "0.1.8";
    static const string ATM_TEXTURE = "deutschz_atmraidez\\paa\\atmraidez\\deutschz_expansion_atm_02_co.paa";
    static const string ATM_CLASS = "ExpansionATM_2";
    static const string PREFIX = "[DZATM] ";

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

    static const int PHASE_HACK = 1;
    static const int PHASE_GUARD = 2;

    static const int RPC_ADMIN_COMMAND = 1701180100;
    static const int RPC_CLIENT_PROGRESS = 1701180101;
    static const int RPC_CLIENT_MESSAGE = 1701180102;
    static const int RPC_CLIENT_EFFECT = 1701180103;
    static const int RPC_ADMIN_STATUS = 1701180104;

    static const int ADMIN_MENU_ID = 170118;
    static const int SERVER_TICK_MS = 1000;

    static const string ATM_SOUNDSET = "UndergroundDoor_Alarm_Loop_SoundSet";
    static const string CLIENT_BEACON_SOUNDSET = "#DZATM_RED_BEACON";
    static const string MARKER_ICON = "Car";
    static const int MARKER_COLOR = ARGB(255, 220, 25, 25);
}
