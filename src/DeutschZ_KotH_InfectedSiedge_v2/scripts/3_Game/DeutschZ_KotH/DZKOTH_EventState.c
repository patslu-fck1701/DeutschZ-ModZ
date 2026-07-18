class DZKOTH_States
{
	static const int INACTIVE = 0;
	static const int ANNOUNCED = 1;
	static const int ZOMBIE_PREPHASE = 2;
	// Values 3-5 belonged to the removed pre-capture gate flow. They stay
	// unused so existing numeric state diagnostics keep their historical IDs.
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
