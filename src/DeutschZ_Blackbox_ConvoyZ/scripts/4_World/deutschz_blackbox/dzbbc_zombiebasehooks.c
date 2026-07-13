modded class ZombieBase
{
	override bool EEOnDamageCalculated(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		if (GetGame() && GetGame().IsServer() && source && Transport.Cast(source) && DZBBC_EventManager.GetInstance().ShouldBlockVehicleManagedUnitContact(this))
			return false;

		return super.EEOnDamageCalculated(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
	}

	override protected void EOnContact(IEntity other, Contact extra)
	{
		if (GetGame() && GetGame().IsServer())
		{
			Transport transport = Transport.Cast(other);
			if (transport && DZBBC_EventManager.GetInstance().ShouldBlockVehicleManagedUnitContact(this))
				return;
		}

		super.EOnContact(other, extra);
	}
}
