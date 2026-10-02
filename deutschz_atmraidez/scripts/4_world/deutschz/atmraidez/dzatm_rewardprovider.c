class DZATM_RewardProvider
{
    protected DZATM_Config m_Config;

    void DZATM_RewardProvider(DZATM_Config config)
    {
        m_Config = config;
    }

    int Grant(PlayerBase player, vector dropPosition)
    {
        if (!player || !m_Config || !m_Config.Raid)
            return 0;

        string provider = m_Config.Raid.RewardProvider;
        provider.ToLower();

        if (provider == "physicalcurrency")
            return GrantPhysicalCurrency(dropPosition);

        // BankingZ is deliberately a separate mod. Do not hard-reference BankingZ
        // here unless it becomes a declared dependency. Codex should implement a
        // clean adapter/provider when the BankingZ public API is finalized.
        DZATM_Log.Error("Unbekannter RewardProvider '" + m_Config.Raid.RewardProvider + "'. Keine Auszahlung.");
        return 0;
    }

    protected int GrantPhysicalCurrency(vector dropPosition)
    {
        int unitValue = m_Config.Raid.PayoutUnitValue;
        int minimumUnits = Math.Ceil(m_Config.Raid.PayoutMinAmount / unitValue);
        int maximumUnits = Math.Floor(m_Config.Raid.PayoutMaxAmount / unitValue);
        if (maximumUnits < minimumUnits) maximumUnits = minimumUnits;

        int units = Math.RandomIntInclusive(minimumUnits, maximumUnits);
        string className = m_Config.Raid.PayoutCurrencyClass;

        if (!GetGame().ConfigIsExisting("CfgVehicles " + className))
        {
            DZATM_Log.Error("Currency-Klasse existiert nicht: " + className);
            return 0;
        }

        dropPosition[1] = GetGame().SurfaceY(dropPosition[0], dropPosition[2]) + 0.1;
        int remaining = units;
        ref array<Object> created = new array<Object>;

        while (remaining > 0)
        {
            ItemBase currency = ItemBase.Cast(GetGame().CreateObjectEx(className, dropPosition, ECE_PLACE_ON_SURFACE));
            if (!currency)
            {
                DZATM_Log.Error("Currency konnte nicht erstellt werden: " + className);
                foreach (Object rollbackObject: created)
                    if (rollbackObject) GetGame().ObjectDelete(rollbackObject);
                return 0;
            }
            created.Insert(currency);

            int stack = remaining;
            if (currency.HasQuantity() && currency.GetQuantityMax() > 0)
                stack = Math.Min(remaining, currency.GetQuantityMax());
            if (currency.HasQuantity())
                currency.SetQuantity(stack);

            currency.SetSynchDirty();
            remaining = remaining - stack;
            dropPosition[0] = dropPosition[0] + 0.15;
        }

        return units * unitValue;
    }
}
