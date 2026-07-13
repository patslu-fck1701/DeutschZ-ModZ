class DZCRZ_HackSession
{
	string SessionId;
	string PlayerId;
	string TargetId;
	int ModuleType;
	int State;
	int StartTime;
	float DurationSeconds;
	float Progress;
	float RequiredRadius;
	string RequiredTool;
	bool RequireToolInHands;
	PlayerBase Player;
	Object Target;
	ItemBase Tool;

	void DZCRZ_HackSession()
	{
		State = DZCRZ_Const.STATE_PENDING;
	}

	void Start()
	{
		StartTime = GetGame().GetTime();
		State = DZCRZ_Const.STATE_ACTIVE;
		Progress = 0.0;
	}

	float GetElapsedSeconds()
	{
		if (!GetGame())
			return 0.0;
		return (GetGame().GetTime() - StartTime) / 1000.0;
	}

	void UpdateProgress()
	{
		if (DurationSeconds <= 0.0)
		{
			Progress = 1.0;
			return;
		}
		Progress = Math.Clamp(GetElapsedSeconds() / DurationSeconds, 0.0, 1.0);
	}

	int Validate()
	{
		if (!DZCRZ_PlayerUtils.IsValidPlayer(Player))
			return DZCRZ_Const.CANCEL_PLAYER_INVALID;
		if (!Target || Target.IsDamageDestroyed())
			return DZCRZ_Const.CANCEL_TARGET_INVALID;
		if (!DZCRZ_PlayerUtils.IsWithin(Player, Target, RequiredRadius))
			return DZCRZ_Const.CANCEL_TOO_FAR;
		Tool = DZCRZ_PlayerUtils.FindTool(Player, RequiredTool, RequireToolInHands);
		if (!Tool || Tool.IsRuined())
			return DZCRZ_Const.CANCEL_TOOL_MISSING;
		return DZCRZ_Const.CANCEL_NONE;
	}
}
