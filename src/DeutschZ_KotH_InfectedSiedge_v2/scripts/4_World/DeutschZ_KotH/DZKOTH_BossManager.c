class DZKOTH_BossManager
{
	protected EntityAI m_Boss;
	protected float m_MaxHealth;
	protected float m_DamageMultiplier;

	bool SpawnBoss(DZKOTH_LocationConfig location, DZKOTH_MainConfig mainConfig)
	{
		if (!GetGame() || !location || !mainConfig)
			return false;

		Cleanup();

		vector bossPos = DZKOTH_Utils.Grounded(location.GetBossSpawnPosition());
		return SpawnBossAt(bossPos, mainConfig);
	}

	bool SpawnBossNearPlayer(PlayerBase player, DZKOTH_LocationConfig location, DZKOTH_MainConfig mainConfig)
	{
		if (!GetGame() || !mainConfig)
			return false;

		Cleanup();

		vector origin = "0 0 0";
		if (player)
			origin = player.GetPosition();
		else if (location)
			origin = location.GetPosition();

		vector bossPos = FindSpawnPosition(origin, mainConfig.BossSpawnMinDistance, mainConfig.BossSpawnMaxDistance);
		return SpawnBossAt(bossPos, mainConfig);
	}

	protected bool SpawnBossAt(vector bossPos, DZKOTH_MainConfig mainConfig)
	{
		m_Boss = CreateBossEntity(DZKOTH_Const.BOSS_CLASSNAME, bossPos);
		if (!m_Boss)
			m_Boss = CreateBossEntity("DZKOTH_BosZZombie", bossPos);

		if (!m_Boss)
		{
			DZKOTH_Utils.Error("Could not spawn BosZ Zombie at " + bossPos.ToString());
			return false;
		}

		m_MaxHealth = mainConfig.BossHealth;
		if (m_MaxHealth <= 0.0)
			m_MaxHealth = 7500.0;

		m_DamageMultiplier = mainConfig.BossDamageMultiplier;
		if (m_DamageMultiplier < 1.0)
			m_DamageMultiplier = 1.0;

		m_Boss.SetHealth("", "Health", m_MaxHealth);
		DisableRunning(m_Boss);
		DZKOTH_Utils.Log("BosZ Zombie spawned with " + m_MaxHealth.ToString() + " HP at " + bossPos.ToString());
		return true;
	}

	protected vector FindSpawnPosition(vector origin, float minDistance, float maxDistance)
	{
		if (minDistance < 8.0)
			minDistance = 8.0;
		if (maxDistance < minDistance)
			maxDistance = minDistance + 6.0;

		vector pos = origin;
		for (int attempt = 0; attempt < 12; attempt++)
		{
			float angle = Math.RandomFloatInclusive(0.0, 6.28318);
			float distance = Math.RandomFloatInclusive(minDistance, maxDistance);
			pos = origin + Vector(Math.Cos(angle) * distance, 0, Math.Sin(angle) * distance);
			pos = DZKOTH_Utils.Grounded(pos);

			if (!GetGame().SurfaceIsSea(pos[0], pos[2]))
				return pos;
		}

		return DZKOTH_Utils.Grounded(origin);
	}

	protected void DisableRunning(EntityAI boss)
	{
		if (!boss)
			return;

		boss.SetHealth("LeftLeg", "Health", 0.0);
		boss.SetHealth("RightLeg", "Health", 0.0);
	}

	protected EntityAI CreateBossEntity(string className, vector bossPos)
	{
		if (className == "" || !GetGame())
			return null;

		if (!GetGame().ConfigIsExisting("CfgVehicles " + className))
		{
			DZKOTH_Utils.Warn("Boss class missing: " + className);
			return null;
		}

		Object object = GetGame().CreateObjectEx(className, bossPos, ECE_SETUP | ECE_INITAI | ECE_PLACE_ON_SURFACE | ECE_EQUIP_ATTACHMENTS);
		EntityAI boss = EntityAI.Cast(object);
		if (!boss)
		{
			DZKOTH_Utils.Warn("Boss spawn failed for " + className + " at " + bossPos.ToString());
			if (object)
				GetGame().ObjectDelete(object);
			return null;
		}

		DZKOTH_Utils.Log("Boss entity created from " + className + " at " + boss.GetPosition().ToString());
		return boss;
	}

	bool IsAlive()
	{
		return m_Boss && m_Boss.IsAlive() && GetHealth() > 0.0;
	}

	float GetHealth()
	{
		if (!m_Boss)
			return 0.0;

		float health = m_Boss.GetHealth("", "Health");
		if (health < 0.0)
			health = 0.0;

		return health;
	}

	float GetMaxHealth()
	{
		if (m_MaxHealth <= 0.0)
			return 7500.0;

		return m_MaxHealth;
	}

	vector GetPosition()
	{
		if (!m_Boss)
			return "0 0 0";

		return m_Boss.GetPosition();
	}

	bool IsBoss(EntityAI source)
	{
		return source && m_Boss && source == m_Boss;
	}

	float GetDamageMultiplier(EntityAI source)
	{
		if (!IsBoss(source))
			return 1.0;

		if (m_DamageMultiplier < 1.0)
			return 1.0;

		return m_DamageMultiplier;
	}

	void Cleanup()
	{
		if (m_Boss && GetGame())
			GetGame().ObjectDelete(m_Boss);

		m_Boss = null;
		m_MaxHealth = 0.0;
		m_DamageMultiplier = 1.0;
	}
}
