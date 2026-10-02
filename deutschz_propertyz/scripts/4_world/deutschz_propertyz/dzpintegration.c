class DZPEventBus
{
    static ref ScriptInvoker OnPropertyPurchased = new ScriptInvoker();
    static ref ScriptInvoker OnPropertySold = new ScriptInvoker();
    static ref ScriptInvoker OnPropertyDoorLockChanged = new ScriptInvoker();
}

class DZPExpansionEconomy
{
    static ExpansionMarketModule GetMarket()
    {
        return ExpansionMarketModule.Cast(CF_ModuleCoreManager.Get(ExpansionMarketModule));
    }

    static int Balance(PlayerBase player)
    {
        ExpansionMarketModule market = GetMarket();
        if (!market || !player) return 0;
        array<int> monies = new array<int>;
        return market.GetPlayerWorth(player, monies);
    }

    static bool Debit(PlayerBase player, int amount)
    {
        ExpansionMarketModule market = GetMarket();
        if (!market || !player || amount < 0) return false;
        array<int> monies = new array<int>;
        if (market.GetPlayerWorth(player, monies) < amount) return false;
        return market.RemoveMoney(amount, player);
    }

    static bool Credit(PlayerBase player, int amount)
    {
        ExpansionMarketModule market = GetMarket();
        if (!market || !player || amount < 0) return false;
        EntityAI parent = player;
        market.SpawnMoney(player, parent, amount, true);
        return true;
    }
}
