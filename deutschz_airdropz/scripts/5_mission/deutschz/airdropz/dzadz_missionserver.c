#ifdef SERVER
modded class MissionServer
{
	void MissionServer()
	{
		if (GetGame() && GetGame().IsServer()) GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DZADZ_Init, 5000, false);
	}
	override void OnMissionStart()
	{
		super.OnMissionStart();
		if (GetGame() && GetGame().IsServer())
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DZADZ_Init, 5000, false);
	}
	override void OnMissionFinish()
	{
		DZADZ_RavenManager.Get().Shutdown();
		super.OnMissionFinish();
	}
	protected void DZADZ_Init()
	{
		DZADZ_RavenManager.Get().Init();
	}
}
#endif
