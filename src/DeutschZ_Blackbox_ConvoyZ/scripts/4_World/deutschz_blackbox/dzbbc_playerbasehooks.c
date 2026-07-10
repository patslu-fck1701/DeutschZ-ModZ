modded class PlayerBase
{
	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

		if (!GetGame() || !GetGame().IsServer() || !damageResult || !source || !IsAlive())
			return;

		float multiplier = DZBBC_EventManager.GetInstance().GetDamageMultiplierForSource(source);
		if (multiplier <= 1.0)
			return;

		DZBBC_ApplyExtraDamage(damageResult, dmgZone, multiplier);
	}

	protected void DZBBC_ApplyExtraDamage(TotalDamageResult damageResult, string dmgZone, float multiplier)
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
		if (rpc_type == DZBBC_RPC_ADMIN_COMMAND)
		{
			if (GetGame() && GetGame().IsServer())
				DZBBC_EventManager.GetInstance().HandleAdminCommand(sender, this, DZBBC_ReadAdminCommand(ctx));
			return;
		}

		if (rpc_type == DZBBC_RPC_MARKER_SYNC)
		{
			DZBBC_ClientMarkerStore.Get().OnMarkerRPC(ctx);
			return;
		}

		if (rpc_type == DZBBC_RPC_HUD_SYNC)
		{
			DZBBC_ClientHUD.OnHudRPC(ctx);
			return;
		}

		if (rpc_type == DZBBC_RPC_NOTIFICATION)
		{
			DZBBC_ClientNotifications.OnNotificationRPC(ctx);
			return;
		}

		if (rpc_type == DZBBC_RPC_CLIENT_FX)
		{
			DZBBC_ClientEffects.OnFXRPC(ctx);
			return;
		}

		super.OnRPC(sender, rpc_type, ctx);
	}

	protected string DZBBC_ReadAdminCommand(ParamsReadContext ctx)
	{
		string command;
		if (!ctx.Read(command))
			return "";

		return command;
	}
}
