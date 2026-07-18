class DZKOTH_LootManager
{
	protected EntityAI m_RewardCrate;
	protected EntityAI m_BossCorpse;
	protected int m_RewardDespawnMs;
	protected bool m_RewardWasFilled;

	void SpawnRewards(DZKOTH_LocationConfig location, DZKOTH_LootConfig lootConfig, DZKOTH_MainConfig mainConfig, vector bossPosition)
	{
		if (!GetGame() || !location)
			return;

		CleanupRewardCrate();
		CleanupBossCorpse();

		m_RewardCrate = SpawnContainer(DZKOTH_Const.REWARD_CRATE_CLASSNAME, location.GetRewardCratePosition());
		m_BossCorpse = SpawnContainer(DZKOTH_Const.BOSS_CORPSE_CLASSNAME, bossPosition + "1.2 0 1.2");
		if (m_RewardCrate)
		{
			m_RewardCrate.SetOrientation(location.GetRewardCrateOrientation());
			DZKOTH_Utils.Log("Reward crate spawned at " + m_RewardCrate.GetPosition().ToString() + " orientation " + location.GetRewardCrateOrientation().ToString());
		}

		EnsureRewardMinimumLoot(m_RewardCrate);
		if (m_RewardCrate && lootConfig)
		{
			FillContainer(m_RewardCrate, lootConfig.RewardCrateLoot);
			EnsureRewardPoolVolume(m_RewardCrate, lootConfig.RewardCrateLoot, GetRewardMinimumItems(mainConfig));
		}
		TryCreateEventReward(m_RewardCrate, DZKOTH_Const.FIREWORKS_BATTERY_CLASSNAME, 10.0);
		m_RewardWasFilled = CountInventoryItems(m_RewardCrate) > 0;
		LogRewardContents(m_RewardCrate);

		if (m_BossCorpse && lootConfig)
			FillContainer(m_BossCorpse, lootConfig.BossCorpseLoot);

		if (m_BossCorpse && !HasInventoryItem(m_BossCorpse, DZKOTH_Const.KEYCARD_CLASSNAME))
			TryCreateKeycard(m_BossCorpse, 100.0);

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

		EnsureRewardMinimumLoot(m_RewardCrate);
		if (m_RewardCrate && lootConfig)
		{
			FillContainer(m_RewardCrate, lootConfig.RewardCrateLoot);
			EnsureRewardPoolVolume(m_RewardCrate, lootConfig.RewardCrateLoot, GetRewardMinimumItems(mainConfig));
		}
		TryCreateEventReward(m_RewardCrate, DZKOTH_Const.FIREWORKS_BATTERY_CLASSNAME, 10.0);
		m_RewardWasFilled = CountInventoryItems(m_RewardCrate) > 0;
		LogRewardContents(m_RewardCrate);

		ScheduleRewardCleanup(mainConfig);
	}

	void Cleanup()
	{
		if (!GetGame())
			return;

		CleanupRewardCrate();
		CleanupBossCorpse();
	}

	protected void ScheduleRewardCleanup(DZKOTH_MainConfig mainConfig)
	{
		if (!GetGame() || !m_RewardCrate)
			return;

		int minutes = 10;
		if (mainConfig && mainConfig.RewardDespawnMinutes > 0)
			minutes = mainConfig.RewardDespawnMinutes;
		if (minutes > 10)
			minutes = 10;

		m_RewardDespawnMs = minutes * 60000;
		if (m_RewardDespawnMs < 60000)
			m_RewardDespawnMs = 60000;

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(CheckRewardCrateEmpty);
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(CleanupRewardCrate);
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(CheckRewardCrateEmpty, 30000, true);
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

		if (m_RewardWasFilled && IsContainerEmpty(m_RewardCrate))
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
		m_RewardWasFilled = false;
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
		EntityAI container = EntityAI.Cast(GetGame().CreateObjectEx(type, pos, ECE_CREATEPHYSICS | ECE_PLACE_ON_SURFACE));
		if (!container && type == DZKOTH_Const.REWARD_CRATE_CLASSNAME)
		{
			DZKOTH_Utils.Warn("Primary reward chest failed. Retrying vanilla SeaChest.");
			container = EntityAI.Cast(GetGame().CreateObjectEx("SeaChest", pos, ECE_CREATEPHYSICS | ECE_PLACE_ON_SURFACE));
		}
		if (!container && type == DZKOTH_Const.BOSS_CORPSE_CLASSNAME)
		{
			DZKOTH_Utils.Warn("DeutschZ boss remains failed. Retrying vanilla SeaChest so the story keycard is not lost.");
			container = EntityAI.Cast(GetGame().CreateObjectEx("SeaChest", pos, ECE_CREATEPHYSICS | ECE_PLACE_ON_SURFACE));
		}
		if (!container)
			DZKOTH_Utils.Warn("Could not spawn loot container " + type + " at " + pos.ToString());
		else
			DZKOTH_Utils.Log("Loot container created: " + container.GetType() + " at " + pos.ToString());

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
			if (IsUniqueRewardClass(entry.ClassName) && HasInventoryItem(container, entry.ClassName))
				continue;
			if (GetGame().ConfigIsExisting("CfgMagazines " + entry.ClassName))
				count = Math.Min(count, Math.Max(0, 2 - CountInventoryClass(container, entry.ClassName)));

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
				else
					DZKOTH_Utils.Log("Reward item added source=pool class=" + entry.ClassName);
			}
		}
	}

	protected void EnsureRewardMinimumLoot(EntityAI container)
	{
		if (!container)
			return;

		int before = CountInventoryItems(container);
		string weaponClass = "M4A1";
		string magazineClass = "Mag_STANAG_30Rnd";
		string ammoClass = "AmmoBox_556x45_20Rnd";
		int weaponRoll = Math.RandomInt(0, 6);
		if (weaponRoll == 1)
		{
			weaponClass = "AKM";
			magazineClass = "Mag_AKM_30Rnd";
			ammoClass = "AmmoBox_762x39_20Rnd";
		}
		else if (weaponRoll == 2)
		{
			weaponClass = "AK74";
			magazineClass = "Mag_AK74_30Rnd";
			ammoClass = "AmmoBox_545x39_20Rnd";
		}
		else if (weaponRoll == 3)
		{
			weaponClass = "M16A2";
			magazineClass = "Mag_STANAG_30Rnd";
			ammoClass = "AmmoBox_556x45_20Rnd";
		}
		else if (weaponRoll == 4)
		{
			weaponClass = "FAL";
			magazineClass = "Mag_FAL_20Rnd";
			ammoClass = "AmmoBox_308Win_20Rnd";
		}
		else if (weaponRoll == 5)
		{
			weaponClass = "SVD";
			magazineClass = "Mag_SVD_10Rnd";
			ammoClass = "AmmoBox_762x54_20Rnd";
		}

		CreateGuaranteedItems(container, weaponClass, 1, true);
		CreateGuaranteedItems(container, magazineClass, 2);
		CreateGuaranteedItems(container, ammoClass, 1);
		CreateGuaranteedItems(container, "FirstAidKit", 1, true);
		CreateGuaranteedItems(container, "Epinephrine", 1, true);
		CreateGuaranteedItems(container, "M67Grenade", 1, true);

		DZKOTH_Utils.Log("Reward minimum check: " + before.ToString() + " -> " + CountInventoryItems(container).ToString() + " items.");
	}

	protected void EnsureRewardPoolVolume(EntityAI container, array<ref DZKOTH_LootEntry> entries, int minimumItems)
	{
		if (!container || !container.GetInventory() || !entries || entries.Count() == 0)
			return;
		if (minimumItems < 1)
			return;

		int attempts = 0;
		while (CountInventoryItems(container) < minimumItems && attempts < 120)
		{
			attempts++;
			DZKOTH_LootEntry entry = entries.Get(Math.RandomIntInclusive(0, entries.Count() - 1));
			if (!entry || entry.ClassName == "")
				continue;

			int classLimit = 3;
			if (IsUniqueRewardClass(entry.ClassName))
				classLimit = 1;
			else if (GetGame().ConfigIsExisting("CfgMagazines " + entry.ClassName))
				classLimit = 2;

			if (CountInventoryClass(container, entry.ClassName) >= classLimit)
				continue;
			if (!GetGame().ConfigIsExisting("CfgVehicles " + entry.ClassName) && !GetGame().ConfigIsExisting("CfgWeapons " + entry.ClassName) && !GetGame().ConfigIsExisting("CfgMagazines " + entry.ClassName))
				continue;

			EntityAI item = container.GetInventory().CreateInInventory(entry.ClassName);
			if (item)
				DZKOTH_Utils.Log("Reward item added source=minimum-volume class=" + entry.ClassName);
		}

		int finalCount = CountInventoryItems(container);
		if (finalCount < minimumItems)
			DZKOTH_Utils.Warn("Reward chest minimum volume not reached: " + finalCount.ToString() + "/" + minimumItems.ToString());
		else
			DZKOTH_Utils.Log("Reward chest minimum volume reached: " + finalCount.ToString() + " items with per-class duplicate caps.");
	}

	protected int GetRewardMinimumItems(DZKOTH_MainConfig mainConfig)
	{
		if (mainConfig && mainConfig.RewardMinimumItems >= 12 && mainConfig.RewardMinimumItems <= 40)
			return mainConfig.RewardMinimumItems;

		return 24;
	}

	protected void CreateGuaranteedItems(EntityAI container, string className, int count, bool unique = false)
	{
		if (!container || !container.GetInventory())
			return;
		if (unique && HasInventoryItem(container, className))
			return;

		if (!GetGame().ConfigIsExisting("CfgVehicles " + className) && !GetGame().ConfigIsExisting("CfgWeapons " + className) && !GetGame().ConfigIsExisting("CfgMagazines " + className))
		{
			DZKOTH_Utils.Warn("Guaranteed reward classname missing: " + className);
			return;
		}

		for (int i = 0; i < count; i++)
		{
			EntityAI item = container.GetInventory().CreateInInventory(className);
			if (!item)
				DZKOTH_Utils.Warn("Guaranteed reward item failed: " + className);
			else
				DZKOTH_Utils.Log("Reward item added source=guaranteed class=" + className);
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
			DZKOTH_Utils.Error("DeutschZ reward chest was not created.");
			return;
		}

		array<EntityAI> items = new array<EntityAI>;
		container.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
		foreach (EntityAI item: items)
		{
			if (item && item != container)
				DZKOTH_Utils.Log("Reward content class=" + item.GetType());
		}

		DZKOTH_Utils.Log("DeutschZ reward chest contains " + CountInventoryItems(container).ToString() + " inventory items.");
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
		else
			DZKOTH_Utils.Log("Boss remains story item added class=" + DZKOTH_Const.KEYCARD_CLASSNAME + " chance=100");
	}

	protected void TryCreateEventReward(EntityAI container, string className, float chancePercent)
	{
		if (!container || !container.GetInventory() || className == "")
			return;
		if (Math.RandomFloatInclusive(0.0, 100.0) > chancePercent)
			return;
		if (HasInventoryItem(container, className))
			return;

		EntityAI item = container.GetInventory().CreateInInventory(className);
		if (!item)
			DZKOTH_Utils.Warn("Could not place separate event reward " + className);
		else
			DZKOTH_Utils.Log("Reward item added source=event class=" + className + " chance=" + chancePercent.ToString());
	}

	protected bool IsUniqueRewardClass(string className)
	{
		return className == "M4A1" || className == "AKM" || className == "AK74" || className == "M16A2" || className == "FAL" || className == "SVD" || className == "ACOGOptic" || className == "M4_T3NRDSOptic" || className == "PSO1Optic" || className == "M4_Suppressor" || className == "AK_Suppressor" || className == DZKOTH_Const.FIREWORKS_BATTERY_CLASSNAME;
	}

	protected int CountInventoryClass(EntityAI container, string className)
	{
		if (!container || !container.GetInventory())
			return 0;

		array<EntityAI> items = new array<EntityAI>;
		container.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
		int count = 0;
		foreach (EntityAI item: items)
		{
			if (item && item.GetType() == className)
				count++;
		}

		return count;
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
