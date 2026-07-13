class DZCRZ_RewardManager
{
	protected DZCRZ_Config m_Config;

	void DZCRZ_RewardManager(DZCRZ_Config config)
	{
		m_Config = config;
	}

	int GrantATMReward(PlayerBase player, vector fallbackPosition)
	{
		if (!player || !m_Config || !m_Config.ATMModule || m_Config.ATMModule.AllowBitcoinReward)
		{
			DZCRZ_Log.Error("Reward blockiert: ungueltiger Spieler oder Bitcoin-Schutz ausgeloest.");
			return 0;
		}
		int amount = Math.RandomIntInclusive(m_Config.ATMModule.PayoutMinAmount, m_Config.ATMModule.PayoutMaxAmount);
		int remaining = amount;
		foreach (DZCRZ_Denomination denomination: m_Config.NoxZEuroDenominations)
		{
			if (!denomination || denomination.Value <= 0 || denomination.ClassName.IndexOf("NoxZ_Euro") != 0 || denomination.ClassName.IndexOf("Bitcoin") >= 0)
				continue;
			int quantity = remaining / denomination.Value;
			if (quantity <= 0)
				continue;
			SpawnCurrency(player, fallbackPosition, denomination.ClassName, quantity);
			remaining = remaining - (quantity * denomination.Value);
		}
		if (remaining != 0)
		{
			DZCRZ_Log.Error("Payout konnte nicht exakt aufgeteilt werden. Rest=" + remaining.ToString());
			return amount - remaining;
		}
		return amount;
	}

	protected void SpawnCurrency(PlayerBase player, vector fallbackPosition, string className, int quantity)
	{
		EntityAI entity = player.GetInventory().CreateInInventory(className);
		if (!entity)
		{
			vector dropPosition = fallbackPosition;
			dropPosition[1] = GetGame().SurfaceY(dropPosition[0], dropPosition[2]) + 0.1;
			entity = EntityAI.Cast(GetGame().CreateObjectEx(className, dropPosition, ECE_PLACE_ON_SURFACE));
		}
		ItemBase currency = ItemBase.Cast(entity);
		if (!currency)
		{
			DZCRZ_Log.Error("Currency-Klasse konnte nicht erstellt werden: " + className);
			return;
		}
		if (currency.HasQuantity())
			currency.SetQuantity(quantity);
	}
}
