class DZATM_PlayerUtils
{
    static bool IsValidPlayer(PlayerBase player)
    {
        return player && player.IsAlive() && !player.IsUnconscious() && player.GetIdentity();
    }

    static string PlayerId(PlayerBase player)
    {
        if (!player || !player.GetIdentity()) return "unknown";
        return player.GetIdentity().GetPlainId();
    }

    static string PlayerName(PlayerBase player)
    {
        if (!player || !player.GetIdentity()) return "Unbekannt";
        return player.GetIdentity().GetName();
    }

    static bool IsWithin(PlayerBase player, Object target, float radius)
    {
        if (!player || !target) return false;
        return vector.Distance(player.GetPosition(), target.GetPosition()) <= radius;
    }

    static string SafeATMId(Object target)
    {
        if (!target) return "invalid";
        vector pos = target.GetPosition();
        vector orientation = target.GetOrientation();
        string worldName = "unknown";
        if (GetGame()) worldName = GetGame().GetWorldName();
        worldName.ToLower();
        return "ATM|" + worldName + "|" + target.GetType() + "|" + Math.Round(pos[0] * 10.0).ToString() + "|" + Math.Round(pos[1] * 10.0).ToString() + "|" + Math.Round(pos[2] * 10.0).ToString() + "|" + Math.Round(orientation[0] * 10.0).ToString();
    }

    static ItemBase FindRaidTool(PlayerBase player, DZATM_RaidConfig config)
    {
        if (!player || !config) return null;
        ItemBase hands = ItemBase.Cast(player.GetHumanInventory().GetEntityInHands());
        if (hands)
        {
            if (hands.IsKindOf(config.RequiredTool)) return hands;
            if (config.AcceptAnyCrowbar && hands.IsKindOf("Crowbar")) return hands;
        }
        if (config.RequireToolInHands) return null;

        array<EntityAI> inventory = new array<EntityAI>;
        player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, inventory);
        foreach (EntityAI entity: inventory)
        {
            ItemBase item = ItemBase.Cast(entity);
            if (!item) continue;
            if (item.IsKindOf(config.RequiredTool)) return item;
            if (config.AcceptAnyCrowbar && item.IsKindOf("Crowbar")) return item;
        }
        return null;
    }

    static void DamageTool(ItemBase tool, float amount)
    {
        if (tool && amount > 0)
            tool.AddHealth("", "Health", -amount);
    }
}
