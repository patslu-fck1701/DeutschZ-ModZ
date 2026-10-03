class DZKOTH_LootManager
{
	protected EntityAI m_RewardCrate;
	protected EntityAI m_BossCorpse;
	protected EntityAI m_BossStoryItem;
	protected int m_RewardDespawnMs;
	protected bool m_RewardWasFilled;
	protected bool m_RareRewardsRolled;
	protected ref array<ref DZKOTH_RareRewardSetting> m_RareRewards;

	protected bool IsRareReward(string className)
	{
		className.ToLower();
		if (m_RareRewards)
		{
			foreach (DZKOTH_RareRewardSetting rule: m_RareRewards)
			{
				if (!rule)
					continue;
				string name = rule.ClassName;
				name.ToLower();
				if (name == className)
					return true;
			}
		}
		return false;
	}

	protected void RollRareRewards(EntityAI container)
	{
		if (!container || !container.GetInventory() || !m_RareRewards || m_RareRewardsRolled)
			return;
		m_RareRewardsRolled = true;
		array<string> rolledClasses = new array<string>;
		foreach (DZKOTH_RareRewardSetting rule: m_RareRewards)
		{
			if (!rule || rule.ClassName == "")
				continue;
			string name = rule.ClassName;
			name.ToLower();
			if (rolledClasses.Find(name) >= 0)
				continue;
			rolledClasses.Insert(name);
			if (rule.Enabled)
				RollRareReward(container, rule);
		}
	}

	protected void RollRareReward(EntityAI container, DZKOTH_RareRewardSetting rule)
	{
		string className = rule.ClassName;
		float chance = Math.Clamp(rule.ChancePercent, 0, 100);
		int roll = Math.RandomInt(0, 10000);
		DZKOTH_Utils.Log("RARE_REWARD_V2 class=" + className + " chance=" + chance.ToString() + " roll=" + roll.ToString());
		if (roll >= chance * 100 || HasInventoryItem(container, className) || rule.MaxCount < 1)
			return;
		// Rare rewards are globally limited to one item per event.
		for (int i = 0; i < Math.Clamp(rule.MaxCount, 0, 1); i++)
		{
			EntityAI rareItem = container.GetInventory().CreateInInventory(className);
			if (rareItem)
				ApplyConfiguredQuantity(rareItem, 1, 1);
			else
				DZKOTH_Utils.Warn("Rare reward creation failed: " + className);
		}
	}

	void SpawnRewards(DZKOTH_LocationConfig location, DZKOTH_LootConfig lootConfig, DZKOTH_MainConfig mainConfig, vector bossPosition)
	{
		if (mainConfig)
			m_RareRewards = mainConfig.RareRewards;
		if (!GetGame() || !location)
			return;

		if (!m_RewardCrate)
			m_RewardCrate = SpawnContainer(GetRewardCrateClassName(mainConfig), location.GetRewardCratePosition());
		if (m_RewardCrate)
		{
			m_RewardCrate.SetOrientation(location.GetRewardCrateOrientation());
			DZKOTH_Utils.Log("Reward crate spawned at " + m_RewardCrate.GetPosition().ToString() + " orientation " + location.GetRewardCrateOrientation().ToString());
		}

		if (!m_RewardWasFilled)
		{
				if (m_RewardCrate && lootConfig)
			{
				FillContainer(m_RewardCrate, lootConfig.RewardCrateLoot);
				// Do not refill failed chance rolls: probabilities apply once per event.
			}
			RollRareRewards(m_RewardCrate);
			TryCreateEventReward(m_RewardCrate, DZKOTH_Const.FIREWORKS_BATTERY_CLASSNAME, GetRewardFireworkChance(mainConfig));
			m_RewardWasFilled = CountInventoryItems(m_RewardCrate) > 0;
			LogRewardContents(m_RewardCrate);
		}

		SpawnBossRemains(lootConfig, mainConfig, bossPosition);

		ScheduleRewardCleanup(mainConfig);
	}

	void SpawnBossRemains(DZKOTH_LootConfig lootConfig, DZKOTH_MainConfig mainConfig, vector bossPosition)
	{
		if (mainConfig)
			m_RareRewards = mainConfig.RareRewards;
		if (!GetGame())
			return;

		CleanupBossCorpse();
		m_BossCorpse = SpawnContainer(DZKOTH_Const.BOSS_CORPSE_CLASSNAME, bossPosition + "1.2 0 1.2");
		if (m_BossCorpse && lootConfig)
			FillContainer(m_BossCorpse, lootConfig.BossCorpseLoot);

	}

	void TryCreateBossDocument(EntityAI boss, DZKOTH_MainConfig mainConfig)
	{
		if (mainConfig)
			m_RareRewards = mainConfig.RareRewards;
		float documentChance = 20.0;
		if (mainConfig)
			documentChance = Math.Clamp(mainConfig.KeycardChancePercent, 0.0, 20.0);
		TryCreateKeycard(boss, documentChance);
	}

	bool SpawnRewardCrate(DZKOTH_LocationConfig location, DZKOTH_LootConfig lootConfig, DZKOTH_MainConfig mainConfig = null)
	{
		if (mainConfig)
			m_RareRewards = mainConfig.RareRewards;
		if (!GetGame() || !location)
		{
			DZKOTH_Utils.Error("REWARD_CRATE_FAILED reason=missing game or location");
			return false;
		}
		if (m_RewardCrate)
		{
			DZKOTH_Utils.Log("Reward crate already exists; duplicate spawn suppressed. class=" + m_RewardCrate.GetType());
			return true;
		}
		vector requestedPosition = location.GetRewardCratePosition();
		string rewardCrateClassName = GetRewardCrateClassName(mainConfig);
		m_RewardCrate = SpawnContainer(rewardCrateClassName, requestedPosition);
		if (!m_RewardCrate)
		{
			DZKOTH_Utils.Error("REWARD_CRATE_FAILED class=" + rewardCrateClassName + " position=" + requestedPosition.ToString() + " reason=CreateObjectEx failed");
			return false;
		}
		m_RewardCrate.SetOrientation(location.GetRewardCrateOrientation());
		DZKOTH_Utils.Log("REWARD_CRATE_SPAWNED class=" + m_RewardCrate.GetType() + " position=" + m_RewardCrate.GetPosition().ToString() + " orientation=" + location.GetRewardCrateOrientation().ToString());
		if (lootConfig)
		{
			FillContainer(m_RewardCrate, lootConfig.RewardCrateLoot);
			// Do not refill failed chance rolls: probabilities apply once per event.
		}
		RollRareRewards(m_RewardCrate);
		TryCreateEventReward(m_RewardCrate, DZKOTH_Const.FIREWORKS_BATTERY_CLASSNAME, GetRewardFireworkChance(mainConfig));
		if (CountInventoryItems(m_RewardCrate) == 0)
		{
			DZKOTH_Utils.Warn("Configured reward pool produced no cargo; applying emergency supplies.");
			EnsureRewardMinimumLoot(m_RewardCrate);
		}
		m_RewardWasFilled = CountInventoryItems(m_RewardCrate) > 0;
		if (!m_RewardWasFilled)
			DZKOTH_Utils.Error("REWARD_CRATE_EMPTY class=" + m_RewardCrate.GetType() + " after configured and emergency supply attempts.");
		LogRewardContents(m_RewardCrate);
		ScheduleRewardCleanup(mainConfig);
		return true;
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
		m_RewardDespawnMs = minutes * 60000;
		if (mainConfig && mainConfig.RewardCrateLifetimeSeconds > 0)
			m_RewardDespawnMs = mainConfig.RewardCrateLifetimeSeconds * 1000;
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
		if (m_BossStoryItem && GetGame())
			GetGame().ObjectDelete(m_BossStoryItem);

		m_BossCorpse = null;
		m_BossStoryItem = null;
	}

	protected EntityAI SpawnContainer(string type, vector position)
	{
		vector pos = DZKOTH_Utils.Grounded(position);
		EntityAI container;
		for (int attempt = 0; attempt < 3 && !container; attempt++)
		{
			vector attemptPos = pos + Vector(attempt * 0.75, 0, attempt * -0.75);
			attemptPos = DZKOTH_Utils.Grounded(attemptPos);
			container = EntityAI.Cast(GetGame().CreateObjectEx(type, attemptPos, ECE_CREATEPHYSICS | ECE_PLACE_ON_SURFACE));
			if (!container)
				DZKOTH_Utils.Warn("Loot container spawn attempt " + (attempt + 1).ToString() + "/3 failed for " + type + " at " + attemptPos.ToString());
		}
		if (!container && type == DZKOTH_Const.BOSS_CORPSE_CLASSNAME)
		{
			DZKOTH_Utils.Warn("Preferred purple teddy boss remains unavailable. Falling back to SmallProtectorCase.");
			container = EntityAI.Cast(GetGame().CreateObjectEx(DZKOTH_Const.BOSS_CORPSE_FALLBACK_CLASSNAME, pos, ECE_CREATEPHYSICS | ECE_PLACE_ON_SURFACE));
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

		ref array<int> order = new array<int>;
		for (int index = 0; index < entries.Count(); index++)
			order.Insert(index);
		for (int shuffleIndex = order.Count() - 1; shuffleIndex > 0; shuffleIndex--)
		{
			int swapIndex = Math.RandomIntInclusive(0, shuffleIndex);
			int oldIndex = order.Get(shuffleIndex);
			order.Set(shuffleIndex, order.Get(swapIndex));
			order.Set(swapIndex, oldIndex);
		}
		int weaponLimit = Math.RandomIntInclusive(5, 9);
		int weaponCount = 0;
		foreach (int entryIndex: order)
		{
			DZKOTH_LootEntry entry = entries.Get(entryIndex);
			if (!entry || !entry.Enabled || entry.ClassName == "")
				continue;
			if (IsRareReward(entry.ClassName))
				continue;

			if (Math.RandomFloatInclusive(0.0, 100.0) > entry.Chance)
				continue;

			string selectedClassName = GetEntryClassName(entry);
			if (IsRareReward(selectedClassName))
				continue;
			bool isTtcWeapon = selectedClassName.Length() >= 4 && selectedClassName.Substring(0, 4) == "TTC_";
			if (isTtcWeapon && weaponCount >= weaponLimit)
				continue;
			int count = Math.Min(GetEntryCount(entry), 1);
			if (HasInventoryItem(container, selectedClassName))
				continue;
			if (GetGame().ConfigIsExisting("CfgMagazines " + selectedClassName))
				count = Math.Min(count, Math.Max(0, 2 - CountInventoryClass(container, selectedClassName)));

			for (int i = 0; i < count; i++)
			{
				if (!GetGame().ConfigIsExisting("CfgVehicles " + selectedClassName) && !GetGame().ConfigIsExisting("CfgWeapons " + selectedClassName) && !GetGame().ConfigIsExisting("CfgMagazines " + selectedClassName))
				{
					DZKOTH_Utils.Warn("Reward loot classname missing: " + selectedClassName);
					break;
				}

				EntityAI item = container.GetInventory().CreateInInventory(selectedClassName);
				if (!item)
					DZKOTH_Utils.Warn("Could not place loot " + selectedClassName + " in " + container.GetType());
				else
				{
					if (isTtcWeapon)
						weaponCount++;
					ApplyConfiguredQuantity(item, entry.MinQuantity, entry.MaxQuantity);
					ApplyConfiguredHealth(item, entry.MinHealthPercent, entry.MaxHealthPercent);
					CreateConfiguredAttachments(item, entry.Attachments);
					CreateExtraItems(container, entry.ExtraItems);
					DZKOTH_Utils.Log("Reward item added source=pool class=" + selectedClassName);
				}
			}
		}
	}

	protected string GetEntryClassName(DZKOTH_LootEntry entry)
	{
		if (!entry)
			return "";
		if (!entry.Alternatives || entry.Alternatives.Count() == 0)
			return entry.ClassName;
		int selection = Math.RandomIntInclusive(0, entry.Alternatives.Count());
		if (selection == 0)
			return entry.ClassName;
		return entry.Alternatives.Get(selection - 1);
	}

	protected void CreateExtraItems(EntityAI container, array<string> extraItems)
	{
		if (!container || !container.GetInventory() || !extraItems)
			return;
		foreach (string extraClass: extraItems)
		{
			if (IsRareReward(extraClass))
				continue;
			if (extraClass == "")
				continue;
			if (HasInventoryItem(container, extraClass))
				continue;
			if (!GetGame().ConfigIsExisting("CfgVehicles " + extraClass) && !GetGame().ConfigIsExisting("CfgWeapons " + extraClass) && !GetGame().ConfigIsExisting("CfgMagazines " + extraClass))
			{
				DZKOTH_Utils.Warn("Reward extra item classname missing: " + extraClass);
				continue;
			}
			if (!container.GetInventory().CreateInInventory(extraClass))
				DZKOTH_Utils.Warn("Could not place reward extra item " + extraClass + " in " + container.GetType());
		}
	}

	protected void EnsureRewardMinimumLoot(EntityAI container)
	{
		if (!container)
			return;

		int before = CountInventoryItems(container);
		// An empty crate receives supplies only; weapon chance rolls are never retried.
		CreateGuaranteedItems(container, "FirstAidKit", 1, true);
		CreateGuaranteedItems(container, "Epinephrine", 1, true);
		CreateGuaranteedItems(container, "M67Grenade", 1, true);

		DZKOTH_Utils.Log("Reward minimum check: " + before.ToString() + " -> " + CountInventoryItems(container).ToString() + " items.");
	}

		protected void ApplyConfiguredHealth(EntityAI entity, float minPercent, float maxPercent)
	{
		if (!entity)
			return;

		minPercent = Math.Clamp(minPercent, 1.0, 100.0);
		maxPercent = Math.Clamp(maxPercent, minPercent, 100.0);
		entity.SetHealth01("", "Health", Math.RandomFloatInclusive(minPercent, maxPercent) / 100.0);
	}

	protected void CreateConfiguredAttachments(EntityAI parent, array<string> attachments)
	{
		if (!parent || !parent.GetInventory() || !attachments)
			return;

		foreach (string attachmentClass: attachments)
		{
			if (IsRareReward(attachmentClass))
				continue;
			if (attachmentClass == "")
				continue;
			if (!GetGame().ConfigIsExisting("CfgVehicles " + attachmentClass) && !GetGame().ConfigIsExisting("CfgWeapons " + attachmentClass) && !GetGame().ConfigIsExisting("CfgMagazines " + attachmentClass))
			{
				DZKOTH_Utils.Warn("Reward attachment classname missing: " + attachmentClass);
				continue;
			}

			if (!parent.GetInventory().CreateAttachment(attachmentClass))
				DZKOTH_Utils.Warn("Could not attach reward item " + attachmentClass + " to " + parent.GetType());
		}
	}

	protected void ApplyConfiguredQuantity(EntityAI entity, int minQuantity, int maxQuantity)
	{
		if (!entity)
			return;

		minQuantity = Math.Max(minQuantity, 1);
		maxQuantity = Math.Max(maxQuantity, minQuantity);
		int quantity = Math.RandomIntInclusive(minQuantity, maxQuantity);

		Magazine magazine = Magazine.Cast(entity);
		if (magazine)
		{
			magazine.ServerSetAmmoCount(Math.Clamp(quantity, 0, magazine.GetAmmoMax()));
			return;
		}

		ItemBase item = ItemBase.Cast(entity);
		if (item && item.HasQuantity())
			item.SetQuantity(quantity);
	}

	protected int GetRewardMinimumItems(DZKOTH_MainConfig mainConfig)
	{
		if (mainConfig)
			return Math.Clamp(mainConfig.RewardMinimumItems, 0, 500);

		return 40;
	}

	protected float GetRewardFireworkChance(DZKOTH_MainConfig mainConfig)
	{
		if (mainConfig)
			return Math.Clamp(mainConfig.RewardFireworkChancePercent, 0.0, 100.0);
		return 25.0;
	}

	protected string GetRewardCrateClassName(DZKOTH_MainConfig mainConfig)
	{
		string className = DZKOTH_Const.REWARD_CRATE_CLASSNAME;
		if (mainConfig && mainConfig.RewardCrateClassName != "")
			className = mainConfig.RewardCrateClassName;
		if (!GetGame().ConfigIsExisting("CfgVehicles " + className))
		{
			DZKOTH_Utils.Warn("Invalid reward crate class '" + className + "'; using " + DZKOTH_Const.REWARD_CRATE_CLASSNAME);
			className = DZKOTH_Const.REWARD_CRATE_CLASSNAME;
		}
		return className;
	}

	protected void CreateGuaranteedItems(EntityAI container, string className, int count, bool unique = false)
	{
		if (IsRareReward(className))
			return;
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
		if (IsRareReward(DZKOTH_Const.KEYCARD_CLASSNAME))
			return;
		if (!container || !container.GetInventory())
			return;

		if (chancePercent < 0.0)
			chancePercent = 0.0;
		if (chancePercent > 20.0)
			chancePercent = 20.0;

		if (Math.RandomFloatInclusive(0.0, 100.0) > chancePercent)
			return;

		EntityAI keycard = container.GetInventory().CreateInInventory(DZKOTH_Const.KEYCARD_CLASSNAME);
		if (!keycard)
			DZKOTH_Utils.Warn("Could not place story document directly in BosZ mummy inventory.");
		else
			DZKOTH_Utils.Log("BosZ mummy story item added class=" + DZKOTH_Const.KEYCARD_CLASSNAME + " chance=" + chancePercent.ToString());
	}

	protected void TryCreateEventReward(EntityAI container, string className, float chancePercent)
	{
		if (IsRareReward(className))
			return;
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
			if (item && SameRewardClass(item.GetType(), className))
				count++;
		}

		return count;
	}

	protected bool SameRewardClass(string actual, string expected)
	{
		actual.ToLower();
		expected.ToLower();
		return actual == expected;
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
