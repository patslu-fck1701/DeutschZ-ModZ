class DZBBC_PlayerUtils
{
	static bool IsValidPlayer(PlayerBase player)
	{
		return player && player.GetIdentity() && player.IsAlive() && !player.IsUnconscious();
	}

	static string GetPlayerUid(PlayerBase player)
	{
		if (!player || !player.GetIdentity())
			return "";

		return player.GetIdentity().GetId();
	}

	static string GetPlayerName(PlayerBase player)
	{
		if (!player || !player.GetIdentity())
			return "unknown";

		return player.GetIdentity().GetName();
	}

	static ref array<PlayerBase> GetOnlinePlayers()
	{
		ref array<PlayerBase> result = new array<PlayerBase>;
		if (!GetGame())
			return result;

		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		foreach (Man man: players)
		{
			PlayerBase player = PlayerBase.Cast(man);
			if (player)
				result.Insert(player);
		}

		return result;
	}

	static EntityAI FindInventoryItem(PlayerBase player, string className)
	{
		if (!player || !player.GetInventory() || className == "")
			return null;

		array<EntityAI> items = new array<EntityAI>;
		player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
		foreach (EntityAI item: items)
		{
			if (item && item.GetType() == className)
				return item;
		}

		return null;
	}

	static bool ContainsInventoryItem(PlayerBase player, EntityAI item)
	{
		if (!player || !player.GetInventory() || !item)
			return false;

		array<EntityAI> items = new array<EntityAI>;
		player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
		return items.Find(item) >= 0;
	}
}
