class DZBBC_LootManager
{
	protected EntityAI m_Container;
	protected bool m_Opened;

	void SpawnReward(DZBBC_CrashSiteConfig site, DZBBC_LootConfig loot)
	{
		if (!site)
			return;

		m_Container = EntityAI.Cast(DZBBC_Utils.CreateObjectSafe(DZBBC_CONTAINER_CLASSNAME, site.GetContainerPosition()));
		if (!m_Container)
		{
			DZBBC_Utils.Warn("Reward container spawn failed.");
			return;
		}

		int created = FillLoot(loot);
		m_Opened = true;
		DZBBC_Utils.Log("Reward container spawned at " + m_Container.GetPosition().ToString() + " with " + created.ToString() + " loot item(s).");
	}

	protected int FillLoot(DZBBC_LootConfig loot)
	{
		if (!m_Container || !m_Container.GetInventory() || !loot || !loot.RewardContainerLoot)
			return 0;

		int created = 0;
		foreach (DZBBC_LootEntry entry: loot.RewardContainerLoot)
		{
			if (!entry || entry.ClassName == "")
				continue;

			float roll = Math.RandomFloatInclusive(0.0, 100.0);
			if (roll > entry.Chance)
				continue;

			int count = Math.RandomIntInclusive(entry.Min, entry.Max);
			if (count < 1)
				count = 1;

			for (int i = 0; i < count; i++)
			{
				EntityAI item = m_Container.GetInventory().CreateInInventory(entry.ClassName);
				if (!item)
					DZBBC_Utils.Warn("Could not create loot " + entry.ClassName);
				else
				{
					created++;
					DZBBC_Utils.Log("Loot spawned " + entry.ClassName + " in reward container.");
				}
			}
		}

		return created;
	}

	bool IsOpened()
	{
		return m_Opened;
	}

	void Cleanup()
	{
		DZBBC_Utils.DeleteObjectSafe(m_Container);
		m_Container = null;
		m_Opened = false;
	}
}
