class DZODZ_PlayerItems
{
    static EntityAI Find(PlayerBase player, string typeName)
    {
        if (!player)
            return null;
        array<EntityAI> items = {};
        player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
        foreach (EntityAI item : items)
        {
            if (item && item.GetType() == typeName)
                return item;
        }
        return null;
    }
}
