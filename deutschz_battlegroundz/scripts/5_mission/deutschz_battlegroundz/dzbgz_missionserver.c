class DZBGZ_MissionServer
{
	static void InitServer()
	{
		if (!GetGame() || !GetGame().IsServer())
			return;

		DZBGZ_EventManager.GetInstance().InitServer();
	}

	static void SyncPlayerState(PlayerBase player)
	{
		if (!player || !GetGame() || !GetGame().IsServer())
			return;

		DZBGZ_EventManager.GetInstance().SyncPlayerState(player);
	}
}

modded class MissionServer
{
	override void OnInit()
	{
		super.OnInit();
		Print("[DZBGZ][BOOT] MissionServer OnInit reached.");
	}

	override void OnMissionStart()
	{
		super.OnMissionStart();
		Print("[DZBGZ][BOOT] MissionServer OnMissionStart reached.");
	}

	override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
	{
		super.InvokeOnConnect(player, identity);

		if (player && identity)
		{
			DZBGZ_MissionServer.InitServer();
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(DZBGZ_SyncPlayerState, 3500, false, player);
		}
	}

	void DZBGZ_DeferredInit()
	{
		Print("[DZBGZ][BOOT] Deferred init called.");
		DZBGZ_MissionServer.InitServer();
	}

	void DZBGZ_SyncPlayerState(PlayerBase player)
	{
		DZBGZ_MissionServer.SyncPlayerState(player);
	}
}
