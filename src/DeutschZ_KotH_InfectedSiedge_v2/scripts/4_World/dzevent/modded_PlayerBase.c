modded class PlayerBase
{
	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

		if (!GetGame() || !GetGame().IsServer() || !damageResult || !IsAlive())
			return;

		DZKOTH_EventManager.GetInstance().CancelTerminalHack(this);

		if (!source)
			return;

		float multiplier = DZKOTH_EventManager.GetInstance().GetDamageMultiplierForSource(source);
		if (multiplier <= 1.0)
			return;

		DZKOTH_ApplyExtraInfectedDamage(damageResult, damageType, dmgZone, multiplier);
	}

	protected void DZKOTH_ApplyExtraInfectedDamage(TotalDamageResult damageResult, int damageType, string dmgZone, float multiplier)
	{
		float extraFactor = multiplier - 1.0;
		if (extraFactor <= 0.0)
			return;

		float healthDamage = damageResult.GetDamage(dmgZone, "Health") * extraFactor;
		float bloodDamage = damageResult.GetDamage(dmgZone, "Blood") * extraFactor;
		float shockDamage = damageResult.GetDamage(dmgZone, "Shock") * extraFactor;

		if (healthDamage <= 0.0)
			healthDamage = damageResult.GetDamage("", "Health") * extraFactor;
		if (bloodDamage <= 0.0)
			bloodDamage = damageResult.GetDamage("", "Blood") * extraFactor;
		if (shockDamage <= 0.0)
			shockDamage = damageResult.GetDamage("", "Shock") * extraFactor;

		if (healthDamage > 0.0)
			AddHealth("", "Health", -healthDamage);
		if (bloodDamage > 0.0)
			AddHealth("", "Blood", -bloodDamage);
		if (shockDamage > 0.0)
			AddHealth("", "Shock", -shockDamage);
	}

	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		if (rpc_type == DZKOTH_RPCIds.ADMIN_COMMAND)
		{
			if (GetGame() && GetGame().IsServer())
				DZKOTH_AdminDebug.HandleRPC(sender, this, ctx);
			return;
		}

		if (rpc_type == DZKOTH_RPCIds.MARKER)
		{
			DZKOTH_ClientMarkerStore.Get().OnMarkerRPC(ctx);
			return;
		}

		if (rpc_type == DZKOTH_RPCIds.CAPTURE_HUD || rpc_type == DZKOTH_RPCIds.BOSS_HUD)
		{
			DZKOTH_ClientHUD.OnHudRPC(ctx);
			return;
		}

		if (rpc_type == DZKOTH_RPCIds.WARNING)
		{
			DZKOTH_NotificationClient.OnWarningRPC(ctx);
			return;
		}

		if (rpc_type == DZKOTH_RPCIds.CLIENT_FX)
		{
			DZKOTH_ClientHUD.OnFXRPC(ctx);
			return;
		}

		if (rpc_type == DZKOTH_RPCIds.KEYCARD_SIGNAL)
		{
			DZKOTH_NotificationClient.OnKeycardSignalRPC(ctx);
			return;
		}

		if (rpc_type == DZEV_RPCIds.MARKER_SYNC)
		{
			DZEV_ClientMarkerStore.Get().OnMarkerRPC(ctx);
			return;
		}

		if (rpc_type == DZEV_RPCIds.PROGRESS_SYNC)
		{
			DZEV_ClientMarkerStore.Get().OnProgressRPC(ctx);
			return;
		}

		if (rpc_type == DZEV_RPCIds.EVENT_FX)
		{
			DZEV_ClientMarkerStore.Get().OnFXRPC(ctx);
			return;
		}

		super.OnRPC(sender, rpc_type, ctx);
	}
}
