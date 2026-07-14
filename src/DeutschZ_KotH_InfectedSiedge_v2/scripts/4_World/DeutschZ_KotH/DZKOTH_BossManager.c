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

		vector origin = location.GetFlagPosition();
		vector bossPos = FindSpawnPosition(origin, mainConfig.BossSpawnMinDistance, mainConfig.BossSpawnMaxDistance);
		if (bossPos == vector.Zero)
		{
			bossPos = DZKOTH_Utils.Grounded(location.GetBossSpawnPosition());
			DZKOTH_Utils.Warn("No safe mast-relative boss position found; static BossSpawnPosition fallback active at " + bossPos.ToString());
		}

		return SpawnBossAt(bossPos, mainConfig);
	}

	bool SpawnBossNearPlayer(PlayerBase player, DZKOTH_LocationConfig location, DZKOTH_MainConfig mainConfig)
	{
		if (!GetGame() || !mainConfig)
			return false;

		Cleanup();

		vector origin = "0 0 0";
		if (location)
			origin = location.GetFlagPosition();
		else if (player)
			origin = player.GetPosition();

		vector bossPos = FindSpawnPosition(origin, mainConfig.BossSpawnMinDistance, mainConfig.BossSpawnMaxDistance);
		if (bossPos == vector.Zero && location)
			bossPos = DZKOTH_Utils.Grounded(location.GetBossSpawnPosition());
		if (bossPos == vector.Zero)
			return false;

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

		DisableRunning(m_Boss);
		m_Boss.SetHealth("", "Health", m_MaxHealth);
		float appliedHealth = m_Boss.GetHealth("", "Health");
		float configuredMaximum = m_Boss.GetMaxHealth("", "Health");
		DZKOTH_Utils.Log("BosZ Zombie health applied=" + appliedHealth.ToString() + " configuredMax=" + configuredMaximum.ToString() + " target=" + m_MaxHealth.ToString() + " at " + bossPos.ToString());
		if (appliedHealth < m_MaxHealth)
			DZKOTH_Utils.Error("BosZ Zombie health was clamped below the configured 7500 HP target.");
		return true;
	}

	protected vector FindSpawnPosition(vector origin, float minDistance, float maxDistance)
	{
		if (minDistance < 8.0)
			minDistance = 8.0;
		if (maxDistance < minDistance)
			maxDistance = minDistance;

		for (int attempt = 0; attempt < 24; attempt++)
		{
			float angle = Math.RandomFloatInclusive(0.0, 6.28318);
			float distance = Math.RandomFloatInclusive(minDistance, maxDistance);
			vector pos = origin + Vector(Math.Cos(angle) * distance, 0, Math.Sin(angle) * distance);
			pos = DZKOTH_Utils.Grounded(pos);

			if (IsSafeSpawnPosition(pos))
				return pos;
		}

		return vector.Zero;
	}

	protected bool IsSafeSpawnPosition(vector pos)
	{
		if (!GetGame() || GetGame().SurfaceIsSea(pos[0], pos[2]) || GetGame().SurfaceIsPond(pos[0], pos[2]))
			return false;

		array<vector> terrainSamples = new array<vector>;
		terrainSamples.Insert(pos + "1 0 1");
		terrainSamples.Insert(pos + "-1 0 1");
		terrainSamples.Insert(pos + "1 0 -1");
		terrainSamples.Insert(pos + "-1 0 -1");
		if (GetGame().GetHighestSurfaceYDifference(terrainSamples) > 1.25)
			return false;

		array<Object> excluded = new array<Object>;
		array<Object> collided = new array<Object>;
		if (GetGame().IsBoxColliding(pos + "0 0.9 0", vector.Zero, "1.6 1.8 1.6", excluded, collided))
			return false;

		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		foreach (Man man: players)
		{
			if (man && vector.Distance(man.GetPosition(), pos) < 4.0)
				return false;
		}

		return true;
	}

	protected void DisableRunning(EntityAI boss)
	{
		if (!boss)
			return;

		DayZInfected infected = DayZInfected.Cast(boss);
		if (!infected || !infected.GetInputController())
		{
			DZKOTH_Utils.Warn("BosZ Zombie movement controller unavailable; slow-walk override was not applied.");
			return;
		}

		infected.GetInputController().OverrideMovementSpeed(true, 1.0);
		DZKOTH_Utils.Log("BosZ Zombie movement locked to the valid walk stage (1); running remains disabled.");
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
