class DZRMZ_RewardService
{
	static int GrantMissionReward(PlayerBase player, DZRMZ_MissionDefinition definition, DZRMZ_Settings settings)
	{
		if (definition && definition.RewardCurrencyClass != "" && definition.RewardCurrencyAmount > 0)
		{
			EntityAI currency = player.GetInventory().CreateInInventory(definition.RewardCurrencyClass);
			if (!currency) currency = EntityAI.Cast(GetGame().CreateObjectEx(definition.RewardCurrencyClass, player.GetPosition(), ECE_PLACE_ON_SURFACE));
			ItemBase money = ItemBase.Cast(currency);
			if (!money) return 0;
			// ExpansionBanknoteEuro is a 100-Euro denomination on this server.
			money.SetQuantity(definition.RewardCurrencyAmount / 100);
			money.SetSynchDirty();
			return 1;
		}
		return GrantSuccessRewards(player, settings);
	}

	static int GrantSuccessRewards(PlayerBase player, DZRMZ_Settings settings)
	{
		if (!GetGame() || !GetGame().IsServer() || !player || !player.GetIdentity() || !settings || !settings.SuccessRewardsEnabled || !settings.SuccessRewards)
			return 0;

		int granted = 0;
		foreach (DZRMZ_RewardDefinition reward : settings.SuccessRewards)
		{
			if (!reward || !reward.IsStructurallyValid())
			{
				DZRMZ_Log.Warn("Ungueltiger oder leerer Reward-Eintrag wurde uebersprungen.");
				continue;
			}
			if (!ClassExists(reward.ClassName))
			{
				DZRMZ_Log.Warn("Reward-Klasse existiert nicht und wurde uebersprungen: " + reward.ClassName);
				continue;
			}
			if (Math.RandomFloat01() > reward.Chance)
				continue;

			int count = reward.MinimumCount;
			if (reward.MaximumCount > reward.MinimumCount)
				count = Math.RandomInt(reward.MinimumCount, reward.MaximumCount + 1);
			for (int i = 0; i < count; i++)
			{
				EntityAI created = player.GetInventory().CreateInInventory(reward.ClassName);
				if (!created)
				{
					created = EntityAI.Cast(GetGame().CreateObjectEx(reward.ClassName, player.GetPosition(), ECE_PLACE_ON_SURFACE));
					if (created)
						DZRMZ_Log.Info(string.Format("Reward %1 wurde wegen vollem Inventar bei %2 auf dem Boden erstellt.", reward.ClassName, player.GetIdentity().GetName()));
				}
				if (created)
				{
					granted++;
				}
				else
				{
					DZRMZ_Log.Warn(string.Format("Reward %1 konnte fuer %2 weder im Inventar noch auf dem Boden erstellt werden.", reward.ClassName, player.GetIdentity().GetName()));
					break;
				}
			}
		}

		return granted;
	}

	protected static bool ClassExists(string className)
	{
		if (!GetGame() || className == "")
			return false;
		if (GetGame().ConfigIsExisting(CFG_VEHICLESPATH + " " + className))
			return true;
		if (GetGame().ConfigIsExisting(CFG_WEAPONSPATH + " " + className))
			return true;
		if (GetGame().ConfigIsExisting(CFG_MAGAZINESPATH + " " + className))
			return true;
		return false;
	}
}
