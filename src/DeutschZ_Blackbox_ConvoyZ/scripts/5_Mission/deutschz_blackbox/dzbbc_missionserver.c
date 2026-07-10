class DZBBC_MissionServer
{
	static void InitServer()
	{
		if (!GetGame() || !GetGame().IsServer())
			return;

		DZBBC_EventManager.GetInstance().InitServer();
	}

	static void SyncPlayerState(PlayerBase player)
	{
		if (!player || !GetGame() || !GetGame().IsServer())
			return;

		DZBBC_EventManager.GetInstance().SyncPlayerState(player);
	}
}

modded class MissionServer
{
	override void OnInit()
	{
		super.OnInit();
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(DZBBC_DeferredInit, 1500, false);
	}

	override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
	{
		super.InvokeOnConnect(player, identity);

		if (player)
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(DZBBC_SyncPlayerState, 3000, false, player);
	}

	void DZBBC_DeferredInit()
	{
		DZBBC_MissionServer.InitServer();
	}

	void DZBBC_SyncPlayerState(PlayerBase player)
	{
		DZBBC_MissionServer.SyncPlayerState(player);
	}
}
