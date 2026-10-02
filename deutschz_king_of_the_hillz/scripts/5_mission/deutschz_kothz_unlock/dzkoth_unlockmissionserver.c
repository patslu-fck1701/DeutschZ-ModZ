modded class MissionServer
{
	override void OnInit()
	{
		DZKOTH_UnlockRuntime.Register();
		super.OnInit();
		if (GetGame())
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DZKOTH_CleanupKnownTraderSmoke, 30000, false);
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DZKOTH_CleanupKnownTraderSmoke, 180000, false);
		}
	}

	override void OnMissionStart()
	{
		// OnInit can run before the dedicated-server context reports IsServer().
		// Register again here, before the base KotHZ starts its event controller.
		DZKOTH_UnlockRuntime.Register();
		super.OnMissionStart();
	}

	protected void DZKOTH_CleanupKnownTraderSmoke()
	{
		if (!GetGame() || !GetGame().IsServer())
			return;

		vector traderPosition = "3728.273193 403.113922 6003.633789";
		array<Object> objects = new array<Object>;
		array<CargoBase> proxies = new array<CargoBase>;
		GetGame().GetObjectsAtPosition3D(traderPosition, 40.0, objects, proxies);
		int removed = 0;
		foreach (Object object: objects)
		{
			SmokeGrenadeBase smoke = SmokeGrenadeBase.Cast(object);
			if (!smoke)
				continue;
			GetGame().ObjectDelete(smoke);
			removed++;
		}
		if (removed > 0)
			DZKOTH_Utils.Warn("Removed " + removed.ToString() + " orphan smoke grenade object(s) at ExpansionSign_Trader_SpawnZone " + traderPosition.ToString());
	}

	override void OnMissionFinish()
	{
		if (GetGame())
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(DZKOTH_CleanupKnownTraderSmoke);
		DZKOTH_UnlockRuntime.Unregister();
		super.OnMissionFinish();
	}
}
