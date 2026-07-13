modded class MissionServer
{
	override void OnInit()
	{
		super.OnInit();
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DZCRZ_DeferredInit, 1500, false);
	}

	override void OnMissionStart()
	{
		super.OnMissionStart();
		if (GetGame() && GetGame().IsServer())
			DZCRZ_Manager.GetInstance().Init();
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DZCRZ_DeferredInit, 1500, false);
	}

	override void InvokeOnDisconnect(PlayerBase player)
	{
		DZCRZ_Manager.GetInstance().CancelPlayer(player, DZCRZ_Const.CANCEL_PLAYER_INVALID);
		super.InvokeOnDisconnect(player);
	}

	override void OnMissionFinish()
	{
		DZCRZ_Manager.DestroyInstance();
		super.OnMissionFinish();
	}

	protected void DZCRZ_DeferredInit()
	{
		if (GetGame() && GetGame().IsServer())
			DZCRZ_Manager.GetInstance().Init();
	}
}
