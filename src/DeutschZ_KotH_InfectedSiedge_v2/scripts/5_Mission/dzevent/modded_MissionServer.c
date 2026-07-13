modded class MissionServer
{
	override void OnInit()
	{
		super.OnInit();
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DZKOTH_DeferredInit, 1500, false);
	}

	override void OnMissionStart()
	{
		super.OnMissionStart();
		DZKOTH_MissionServer.InitServer();
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DZKOTH_DeferredInit, 1500, false);
	}

	override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
	{
		super.InvokeOnConnect(player, identity);

		if (player)
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(DZKOTH_SyncPlayerState, 3000, false, player);
	}

	void DZKOTH_DeferredInit()
	{
		DZKOTH_MissionServer.InitServer();
	}

	void DZKOTH_SyncPlayerState(PlayerBase player)
	{
		DZKOTH_MissionServer.SyncPlayerState(player);
	}
}
