class DZKOTH_MissionServer
{
	static void InitServer()
	{
		if (!GetGame() || !GetGame().IsServer())
			return;

		DZKOTH_EventManager.GetInstance().InitServer();

#ifdef DIAG_DEVELOPER
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DiagValidateMilitaryInfected, 3000, false);
#endif
	}

#ifdef DIAG_DEVELOPER
	static void DiagValidateMilitaryInfected()
	{
		if (!GetGame() || !GetGame().IsServer())
			return;

		vector testPosition = "7510 0 7510";
		testPosition[1] = GetGame().SurfaceY(testPosition[0], testPosition[2]);
		DayZInfected infected = DayZInfected.Cast(GetGame().CreateObjectEx("DZKOTH_Infected_600", testPosition, ECE_PLACE_ON_SURFACE | ECE_INITAI));
		if (!infected)
		{
			DZKOTH_Utils.Error("DIAG infected test FAILED: DZKOTH_Infected_600 did not spawn");
			return;
		}

		float maxHealth = infected.GetMaxHealth("", "Health");
		if (infected.GetInputController())
			infected.GetInputController().OverrideMovementSpeed(true, 1.0);

		DZKOTH_Utils.Log("DIAG infected test: DZKOTH_Infected_600 maxHealth=" + maxHealth.ToString() + " movementOverride=walk-stage-1");
		GetGame().ObjectDelete(infected);
	}
#endif

	static void SyncPlayerState(PlayerBase player)
	{
		if (!player || !GetGame() || !GetGame().IsServer())
			return;

		DZKOTH_EventManager.GetInstance().SyncStateToPlayer(player);
	}
}
