class DZBBC_Coded9VBattery : Battery9V
{
}

class DZBBC_SignalMatches : Matchbox
{
	override void SetActions()
	{
		super.SetActions();
		RemoveAction(ActionLightItemOnFire);
	}

	override bool CanIgniteItem(EntityAI ignite_target = null)
	{
		return false;
	}
}
