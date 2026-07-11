class DZBBC_MissionServer
{
	static void InitServer()
	{
		if (!GetGame() || !GetGame().IsServer())
			return;

		DZBBC_EventManager.GetInstance().InitServer();

#ifdef DIAG_DEVELOPER
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DiagSpawnStoryItems, 2500, false);
#endif
	}

#ifdef DIAG_DEVELOPER
	static void DiagSpawnStoryItems()
	{
		if (!GetGame() || !GetGame().IsServer())
			return;

		ref array<string> classNames = {
			"DZBBC_SignalMatches",
			"DZBBC_Coded9VBattery",
			"DZBBC_DataCore",
			"DZBBC_MilitaryTerminal",
			"DZBBC_SecureSupplyContainer"
		};

		vector testPosition = "7500 0 7500";
		testPosition[1] = GetGame().SurfaceY(testPosition[0], testPosition[2]) + 0.25;

		foreach (string className: classNames)
		{
			Object spawnedObject = GetGame().CreateObjectEx(className, testPosition, ECE_PLACE_ON_SURFACE);
			if (spawnedObject)
			{
				DZBBC_Utils.Log("DIAG spawn test OK: " + className);
				GetGame().ObjectDelete(spawnedObject);
			}
			else
			{
				DZBBC_Utils.Warn("DIAG spawn test FAILED: " + className);
			}
		}
	}
#endif

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
