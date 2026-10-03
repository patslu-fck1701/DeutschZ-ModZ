modded class MissionServer
{
	void MissionServer()
	{
		// Fallback fuer Mod-Kombinationen, in denen ein spaeter geladener
		// MissionServer-Hook OnMissionStart nicht bis zu uns weiterreicht.
		// DZRMZ_MissionManager.Init() ist idempotent und verhindert Doppelstarts.
		if (GetGame() && GetGame().IsServer())
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DZRMZ_DeferredInit, 2000, false);
	}

	override void OnMissionStart()
	{
		super.OnMissionStart();
		if (GetGame() && GetGame().IsServer())
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DZRMZ_DeferredInit, 2000, false);
	}

	override void OnMissionFinish()
	{
		if (GetGame())
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(DZRMZ_DeferredInit);
		DZRMZ_MissionManager.DestroyInstance();
		super.OnMissionFinish();
	}

	protected void DZRMZ_DeferredInit()
	{
		if (GetGame() && GetGame().IsServer())
			DZRMZ_MissionManager.GetInstance().Init();
	}
}
