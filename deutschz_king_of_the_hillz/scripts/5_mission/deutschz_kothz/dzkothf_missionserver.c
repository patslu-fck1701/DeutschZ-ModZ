modded class MissionServer
{
	override void OnInit()
	{
		super.OnInit();

		if (GetGame() && GetGame().IsServer())
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DZKOTHF_DeferredInit, 1000, false);
	}

	override void OnMissionStart()
	{
		super.OnMissionStart();

		if (GetGame() && GetGame().IsServer())
		{
			DZKOTHF_EventController.GetInstance().InitServer();
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DZKOTHF_DeferredInit, 1000, false);
		}
	}

	override void OnMissionFinish()
	{
		if (GetGame() && GetGame().IsServer())
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(DZKOTHF_DeferredInit);
			DZKOTHF_EventController.DestroyInstance();
		}

		super.OnMissionFinish();
	}

	override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
	{
		super.InvokeOnConnect(player, identity);
		DZKOTHF_EventController.GetInstance().SyncPlayer(player);
	}

	override void InvokeOnDisconnect(PlayerBase player)
	{
		DZKOTHF_EventController.GetInstance().OnPlayerDisconnect(player);
		super.InvokeOnDisconnect(player);
	}

	void DZKOTHF_DeferredInit()
	{
		DZKOTHF_EventController.GetInstance().InitServer();
	}
}
