modded class MissionServer
{
	override void OnInit()
	{
		super.OnInit();

		if (GetGame() && GetGame().IsServer())
			DZKOTHF_EventController.GetInstance().InitServer();
	}

	override void OnMissionFinish()
	{
		if (GetGame() && GetGame().IsServer())
			DZKOTHF_EventController.DestroyInstance();

		super.OnMissionFinish();
	}
}
