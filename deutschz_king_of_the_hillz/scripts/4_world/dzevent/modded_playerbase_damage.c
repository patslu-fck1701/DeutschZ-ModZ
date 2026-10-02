modded class PlayerBase
{
	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

		if (!GetGame() || !GetGame().IsServer() || !damageResult || !IsAlive() || !source)
			return;

		float multiplier = DZKOTH_EventManager.GetInstance().GetDamageMultiplierForSource(source);
		if (multiplier <= 1.0)
			return;

		DZKOTH_ApplyExtraInfectedDamage(damageResult, dmgZone, multiplier);
	}

	protected void DZKOTH_ApplyExtraInfectedDamage(TotalDamageResult damageResult, string dmgZone, float multiplier)
	{
		float extraFactor = multiplier - 1.0;
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
}
