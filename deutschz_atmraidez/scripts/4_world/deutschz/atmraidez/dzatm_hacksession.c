class DZATM_HackSession
{
    string SessionId;
    string PlayerId;
    string TargetId;
    int State;
    int Phase;
    int StartTime;
    int PhaseStartTime;
    float Phase1Seconds;
    float Phase2Seconds;
    float RequiredRadius;
    float Progress;
    bool RewardCommitted;
    Object SmokeObject;
    PlayerBase Player;
    ExpansionATMBase Target;
    ItemBase Tool;

    void DZATM_HackSession()
    {
        State = DZATM_Const.STATE_PENDING;
        Phase = DZATM_Const.PHASE_HACK;
    }

    void Start()
    {
        StartTime = GetGame().GetTime();
        PhaseStartTime = StartTime;
        State = DZATM_Const.STATE_ACTIVE;
        Progress = 0.0;
    }

    void BeginGuardPhase()
    {
        Phase = DZATM_Const.PHASE_GUARD;
        PhaseStartTime = GetGame().GetTime();
        Progress = 0.0;
    }

    float GetPhaseElapsedSeconds()
    {
        if (!GetGame()) return 0.0;
        return (GetGame().GetTime() - PhaseStartTime) / 1000.0;
    }

    void UpdateProgress()
    {
        float duration = Phase1Seconds;
        if (Phase == DZATM_Const.PHASE_GUARD) duration = Phase2Seconds;
        if (duration <= 0.0) Progress = 1.0;
        else Progress = Math.Clamp(GetPhaseElapsedSeconds() / duration, 0.0, 1.0);
    }

    int Validate(DZATM_Config config)
    {
        if (!DZATM_PlayerUtils.IsValidPlayer(Player)) return DZATM_Const.CANCEL_PLAYER_INVALID;
        if (!Target || Target.IsDamageDestroyed()) return DZATM_Const.CANCEL_TARGET_INVALID;
        if (!DZATM_PlayerUtils.IsWithin(Player, Target, RequiredRadius)) return DZATM_Const.CANCEL_TOO_FAR;

        if (Phase == DZATM_Const.PHASE_HACK)
        {
            Tool = DZATM_PlayerUtils.FindRaidTool(Player, config.Raid);
            if (!Tool || Tool.IsRuined()) return DZATM_Const.CANCEL_TOOL_MISSING;
        }
        return DZATM_Const.CANCEL_NONE;
    }
}
