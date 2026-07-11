class DZKOTH_LootManager
{
	protected EntityAI m_RewardCrate;
	protected EntityAI m_BossCorpse;
	protected int m_RewardDespawnMs;

	void SpawnRewards(DZKOTH_LocationConfig location, DZKOTH_LootConfig lootConfig, DZKOTH_MainConfig mainConfig)
	{
		if (!GetGame() || !location)
			return;

		CleanupRewardCrate();
		CleanupBossCorpse();

		m_RewardCrate = SpawnContainer(DZKOTH_Const.REWARD_CRATE_CLASSNAME, location.GetRewardCratePosition());
		m_BossCorpse = SpawnContainer(DZKOTH_Const.BOSS_CORPSE_CLASSNAME, location.GetBossSpawnPosition() + "1.2 0 1.2");
		if (m_RewardCrate)
		{
			m_RewardCrate.SetOrientation(location.GetRewardCrateOrientation());
			DZKOTH_Utils.Log("Reward crate spawned at " + m_RewardCrate.GetPosition().ToString() + " orientation " + location.GetRewardCrateOrientation().ToString());
		}

		if (m_RewardCrate && lootConfig)
			FillContainer(m_RewardCrate, lootConfig.RewardCrateLoot);

		EnsureRewardFallbackLoot(m_RewardCrate);
		LogRewardContents(m_RewardCrate);

		if (m_BossCorpse && lootConfig)
			FillContainer(m_BossCorpse, lootConfig.BossCorpseLoot);

		if (m_BossCorpse && mainConfig && !HasInventoryItem(m_BossCorpse, DZKOTH_Const.KEYCARD_CLASSNAME))
			TryCreateKeycard(m_BossCorpse, mainConfig.KeycardChancePercent);

		ScheduleRewardCleanup(mainConfig);
	}

	void SpawnRewardCrate(DZKOTH_LocationConfig location, DZKOTH_LootConfig lootConfig, DZKOTH_MainConfig mainConfig = null)
	{
		if (!GetGame() || !location)
			return;

		CleanupRewardCrate();
		CleanupBossCorpse();

		m_RewardCrate = SpawnContainer(DZKOTH_Const.REWARD_CRATE_CLASSNAME, location.GetRewardCratePosition());
		if (m_RewardCrate)
		{
			m_RewardCrate.SetOrientation(location.GetRewardCrateOrientation());
			DZKOTH_Utils.Log("Reward crate spawned at " + m_RewardCrate.GetPosition().ToString() + " orientation " + location.GetRewardCrateOrientation().ToString());
		}

		if (m_RewardCrate && lootConfig)
			FillContainer(m_RewardCrate, lootConfig.RewardCrateLoot);

		EnsureRewardFallbackLoot(m_RewardCrate);
		LogRewardContents(m_RewardCrate);

		ScheduleRewardCleanup(mainConfig);
	}

	void Cleanup()
	{
		if (!GetGame())
			return;

		CleanupBossCorpse();
	}

	protected void ScheduleRewardCleanup(DZKOTH_MainConfig mainConfig)
	{
		if (!GetGame() || !m_RewardCrate)
			return;

		int minutes = 10;
		if (mainConfig && mainConfig.RewardDespawnMinutes > 0)
			minutes = mainConfig.RewardDespawnMinutes;

		m_RewardDespawnMs = minutes * 60000;
		if (m_RewardDespawnMs < 60000)
			m_RewardDespawnMs = 60000;

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(CheckRewardCrateEmpty);
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(CleanupRewardCrate);
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(CheckRewardCrateEmpty, 10000, true);
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(CleanupRewardCrate, m_RewardDespawnMs, false);
	}

	protected void CheckRewardCrateEmpty()
	{
		if (!m_RewardCrate)
		{
			if (GetGame())
				GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(CheckRewardCrateEmpty);
			return;
		}

		if (IsContainerEmpty(m_RewardCrate))
			CleanupRewardCrate();
	}

	protected void CleanupRewardCrate()
	{
		if (GetGame())
		{
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(CheckRewardCrateEmpty);
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(CleanupRewardCrate);
		}

		if (m_RewardCrate && GetGame())
			GetGame().ObjectDelete(m_RewardCrate);

		m_RewardCrate = null;
	}

	protected void CleanupBossCorpse()
	{
		if (m_BossCorpse && GetGame())
			GetGame().ObjectDelete(m_BossCorpse);

		m_BossCorpse = null;
	}

	protected EntityAI SpawnContainer(string type, vector position)
	{
		vector pos = DZKOTH_Utils.Grounded(position);
		EntityAI container = EntityAI.Cast(GetGame().CreateObjectEx(type, pos, ECE_PLACE_ON_SURFACE));
		if (!container)
			DZKOTH_Utils.Warn("Could not spawn loot container " + type + " at " + pos.ToString());

		return container;
	}

	protected void FillContainer(EntityAI container, array<ref DZKOTH_LootEntry> entries)
	{
		if (!container || !container.GetInventory() || !entries)
			return;

		foreach (DZKOTH_LootEntry entry: entries)
		{
			if (!entry || entry.ClassName == "")
				continue;

			if (Math.RandomFloatInclusive(0.0, 100.0) > entry.Chance)
				continue;

			int count = GetEntryCount(entry);
			for (int i = 0; i < count; i++)
			{
				if (!GetGame().ConfigIsExisting("CfgVehicles " + entry.ClassName) && !GetGame().ConfigIsExisting("CfgWeapons " + entry.ClassName) && !GetGame().ConfigIsExisting("CfgMagazines " + entry.ClassName))
				{
					DZKOTH_Utils.Warn("Reward loot classname missing: " + entry.ClassName);
					break;
				}

				EntityAI item = container.GetInventory().CreateInInventory(entry.ClassName);
				if (!item)
					DZKOTH_Utils.Warn("Could not place loot " + entry.ClassName + " in " + container.GetType());
			}
		}
	}

	protected void EnsureRewardFallbackLoot(EntityAI container)
	{
		if (!container || CountInventoryItems(container) > 0)
			return;

		DZKOTH_Utils.Warn("Configured reward pool produced no cargo. Adding guaranteed vanilla fallback loot.");
		CreateGuaranteedItems(container, "M4A1", 2);
		CreateGuaranteedItems(container, "AKM", 2);
		CreateGuaranteedItems(container, "Mag_STANAG_30Rnd", 8);
		CreateGuaranteedItems(container, "Mag_AKM_30Rnd", 8);
		CreateGuaranteedItems(container, "AmmoBox_556x45_20Rnd", 6);
		CreateGuaranteedItems(container, "AmmoBox_762x39_20Rnd", 6);
		CreateGuaranteedItems(container, "M67Grenade", 2);
	}

	protected void CreateGuaranteedItems(EntityAI container, string className, int count)
	{
		if (!container || !container.GetInventory())
			return;

		for (int i = 0; i < count; i++)
		{
			EntityAI item = container.GetInventory().CreateInInventory(className);
			if (!item)
				DZKOTH_Utils.Warn("Guaranteed reward item failed: " + className);
		}
	}

	protected int CountInventoryItems(EntityAI container)
	{
		if (!container || !container.GetInventory())
			return 0;

		array<EntityAI> items = new array<EntityAI>;
		container.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
		int count = 0;
		foreach (EntityAI item: items)
		{
			if (item && item != container)
				count++;
		}

		return count;
	}

	protected void LogRewardContents(EntityAI container)
	{
		if (!container)
		{
			DZKOTH_Utils.Error("DeutschZ reward barrel was not created.");
			return;
		}

		DZKOTH_Utils.Log("DeutschZ reward barrel contains " + CountInventoryItems(container).ToString() + " inventory items.");
	}

	protected int GetEntryCount(DZKOTH_LootEntry entry)
	{
		int min = entry.Min;
		int max = entry.Max;

		if (min < 1)
			min = 1;
		if (max < min)
			max = min;

		return Math.RandomIntInclusive(min, max);
	}

	protected void TryCreateKeycard(EntityAI container, float chancePercent)
	{
		if (!container || !container.GetInventory())
			return;

		if (chancePercent < 0.0)
			chancePercent = 0.0;
		if (chancePercent > 100.0)
			chancePercent = 100.0;

		if (Math.RandomFloatInclusive(0.0, 100.0) > chancePercent)
			return;

		EntityAI keycard = container.GetInventory().CreateInInventory(DZKOTH_Const.KEYCARD_CLASSNAME);
		if (!keycard)
			DZKOTH_Utils.Warn("Could not place story keycard in boss corpse.");
	}

	protected bool HasInventoryItem(EntityAI container, string className)
	{
		if (!container || !container.GetInventory() || className == "")
			return false;

		array<EntityAI> items = new array<EntityAI>;
		container.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
		foreach (EntityAI item: items)
		{
			if (item && item.GetType() == className)
				return true;
		}

		return false;
	}

	protected bool IsContainerEmpty(EntityAI container)
	{
		if (!container || !container.GetInventory())
			return true;

		array<EntityAI> items = new array<EntityAI>;
		container.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
		foreach (EntityAI item: items)
		{
			if (item && item != container)
				return false;
		}

		return true;
	}
}
