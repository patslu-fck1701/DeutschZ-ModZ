class DZCRZ_PlayerUtils
{
	static bool IsValidPlayer(PlayerBase player)
	{
		return player && player.IsAlive() && !player.IsUnconscious() && player.GetIdentity();
	}

	static string PlayerId(PlayerBase player)
	{
		if (!player || !player.GetIdentity())
			return "unknown";
		return player.GetIdentity().GetPlainId();
	}

	static string PlayerName(PlayerBase player)
	{
		if (!player || !player.GetIdentity())
			return "Unbekannt";
		return player.GetIdentity().GetName();
	}

	static ItemBase FindTool(PlayerBase player, string className, bool requireInHands)
	{
		if (!player || className == "")
			return null;
		ItemBase hands = ItemBase.Cast(player.GetHumanInventory().GetEntityInHands());
		if (hands && hands.IsKindOf(className))
			return hands;
		if (requireInHands)
			return null;
		array<EntityAI> inventory = new array<EntityAI>;
		player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, inventory);
		foreach (EntityAI entity: inventory)
		{
			ItemBase item = ItemBase.Cast(entity);
			if (item && item.IsKindOf(className))
				return item;
		}
		return null;
	}

	static bool IsWithin(PlayerBase player, Object target, float radius)
	{
		if (!player || !target)
			return false;
		return vector.Distance(player.GetPosition(), target.GetPosition()) <= radius;
	}

	static string SafeObjectId(Object target)
	{
		if (!target)
			return "invalid";
		vector pos = target.GetPosition();
		return target.GetType() + "_" + Math.Round(pos[0]).ToString() + "_" + Math.Round(pos[2]).ToString();
	}

	static void DamageTool(ItemBase tool, float amount)
	{
		if (!tool || amount <= 0)
			return;
		tool.AddHealth("", "Health", -amount);
	}
}
