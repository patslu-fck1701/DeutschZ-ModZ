class DZKOTH_RewardChest : SeaChest
{
	override bool IsTakeable()
	{
		return false;
	}

	override bool CanPutInCargo(EntityAI parent)
	{
		return false;
	}

	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}
}
