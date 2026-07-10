class DZKOTH_MissionServer
{
	static void InitServer()
	{
		if (!GetGame() || !GetGame().IsServer())
			return;

		DZKOTH_EventManager.GetInstance().InitServer();
	}

	static void SyncPlayerState(PlayerBase player)
	{
		if (!player || !GetGame() || !GetGame().IsServer())
			return;

		DZKOTH_EventManager.GetInstance().SyncStateToPlayer(player);
	}
}
