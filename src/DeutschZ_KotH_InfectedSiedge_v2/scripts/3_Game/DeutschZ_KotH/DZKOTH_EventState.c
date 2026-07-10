class DZKOTH_States
{
	static const int INACTIVE = 0;
	static const int ANNOUNCED = 1;
	static const int ZOMBIE_PREPHASE = 2;
	static const int WAITING_FOR_TERMINAL_HACK = 3;
	static const int WAITING_FOR_CHEST_BREAK = 3;
	static const int TERMINAL_HACK_ACTIVE = 4;
	static const int CHEST_BREAK_ACTIVE = 4;
	static const int TERMINAL_HACK_COMPLETE = 5;
	static const int CHEST_BREAK_COMPLETE = 5;
	static const int WAITING_FOR_PLAYER = 6;
	static const int CAPTURING = 7;
	static const int WAVE_ONE_ACTIVE = 8;
	static const int WAVE_TWO_ACTIVE = 9;
	static const int WAVE_THREE_ACTIVE = 10;
	static const int CAPTURE_COMPLETE = 11;
	static const int BOSS_WARNING = 12;
	static const int BOSS_ACTIVE = 13;
	static const int BOSS_DEFEATED = 14;
	static const int REWARD_ACTIVE = 15;
	static const int KEYCARD_TAKEN = 16;
	static const int COMPLETED = 17;
	static const int FAILED = 18;
	static const int CLEANUP = 19;
}
