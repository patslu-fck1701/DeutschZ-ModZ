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
		DZKOTH_Utils.Log("BosZ Zombie spawned with " + m_MaxHealth.ToString() + " HP at " + bossPos.ToString());
		return true;
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
