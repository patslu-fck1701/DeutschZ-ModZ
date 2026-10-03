class DZRMZ_InventoryService
{
	static int CountValidDeliveryItems(PlayerBase player, DZRMZ_MissionDefinition definition)
	{
		if (!player || !definition || definition.RequiredItemClass == "")
			return 0;

		array<EntityAI> inventoryItems = new array<EntityAI>;
		player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, inventoryItems);
		int count = 0;

		foreach (EntityAI item : inventoryItems)
		{
			if (IsValidDeliveryItem(item, definition))
				count++;
		}

		return count;
	}

	static bool ConsumeValidDeliveryItems(PlayerBase player, DZRMZ_MissionDefinition definition)
	{
		if (!GetGame() || !GetGame().IsServer() || !player || !definition || definition.RequiredItemClass == "" || definition.RequiredQuantity < 1)
			return false;

		array<EntityAI> inventoryItems = new array<EntityAI>;
		array<EntityAI> matches = new array<EntityAI>;
		player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, inventoryItems);

		foreach (EntityAI item : inventoryItems)
		{
			if (IsValidDeliveryItem(item, definition))
				matches.Insert(item);
		}

		if (matches.Count() < definition.RequiredQuantity)
			return false;

		for (int i = 0; i < definition.RequiredQuantity; i++)
		{
			if (matches[i])
				GetGame().ObjectDelete(matches[i]);
		}

		return true;
	}

	static bool IsValidDeliveryItem(EntityAI item, DZRMZ_MissionDefinition definition)
	{
		if (!item || !definition || item.GetType() != definition.RequiredItemClass)
			return false;
		if (!definition.AllowRuinedDeliveryItems && item.IsRuined())
			return false;
		if (item.HasQuantity() && item.GetQuantity() <= 0.0)
			return false;
		if (definition.MinimumItemQuantity > 0.0)
		{
			if (!item.HasQuantity() || item.GetQuantity() < definition.MinimumItemQuantity)
				return false;
		}
		if (definition.RequiredLiquidTypeMask != LIQUID_NONE)
		{
			if ((item.GetLiquidType() & definition.RequiredLiquidTypeMask) == 0)
				return false;
		}
		if (definition.RequireStoredEnergy)
		{
			if (!item.GetCompEM() || item.GetCompEM().GetEnergy() <= 0.0)
				return false;
		}
		return true;
	}
}
