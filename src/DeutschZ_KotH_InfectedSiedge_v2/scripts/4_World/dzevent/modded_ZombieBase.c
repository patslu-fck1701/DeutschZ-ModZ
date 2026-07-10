modded class ZombieBase
{
	override protected void EOnContact(IEntity other, Contact extra)
	{
		if (GetGame() && GetGame().IsServer())
		{
			Transport transport = Transport.Cast(other);
			if (transport && DZKOTH_EventManager.GetInstance().ShouldBlockVehicleZombieContact(this))
				return;
		}

		super.EOnContact(other, extra);
	}
}
