class DZECZ_MissionBootstrap
{
	static void InitServer()
	{
		if (!GetGame() || !GetGame().IsServer())
			return;
		DZECZ_EventManager.GetInstance().InitServer();
	}
}

modded class MissionServer
{
	override void OnInit()
	{
		super.OnInit();
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DZECZ_DeferredInit, 1500, false);
	}

	override void OnMissionStart()
	{
		super.OnMissionStart();
		DZECZ_MissionBootstrap.InitServer();
	}

	void DZECZ_DeferredInit()
	{
		DZECZ_MissionBootstrap.InitServer();
	}
}
