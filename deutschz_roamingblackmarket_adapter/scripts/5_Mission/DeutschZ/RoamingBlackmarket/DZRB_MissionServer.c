modded class MissionServer
{
	override void OnMissionFinish()
	{
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(DZRB_StartMobileTrader);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(DZRB_ClearStaleMarker);
		DZRB_MobileManager.GetInstance().Shutdown();
		super.OnMissionFinish();
	}
	override void OnInit()
	{
		super.OnInit();
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DZRB_ClearStaleMarker, 3000, false);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DZRB_StartMobileTrader, 5000, false);
	}

	override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
	{
		super.InvokeOnConnect(player, identity);
		DZRB_MobileManager.GetInstance().SyncMarker(player);
	}

	private void DZRB_ClearStaleMarker()
	{
		DZRB_MarkerService.RemoveCurrent(false);
	}

	private void DZRB_StartMobileTrader()
	{
		DZRB_MobileManager.GetInstance().Initialize();
	}
}
