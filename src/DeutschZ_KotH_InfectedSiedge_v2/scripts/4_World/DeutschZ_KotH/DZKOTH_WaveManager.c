class DZKOTH_WaveManager
{
	protected ref array<EntityAI> m_Spawned;
	protected ref array<float> m_DamageMultipliers;

	void DZKOTH_WaveManager()
	{
		m_Spawned = new array<EntityAI>;
		m_DamageMultipliers = new array<float>;
	}

	void SpawnWaveForPlayers(array<PlayerBase> players, DZKOTH_WaveConfig wave, DZKOTH_MainConfig mainConfig, vector fallbackCenter, int totalGoal = 0)
	{
		if (!GetGame() || !wave || !mainConfig || !wave.Types || wave.Types.Count() == 0)
			return;

		bool spawnedForPlayer = false;
		if (players)
		{
			foreach (PlayerBase player: players)
			{
				if (!IsValidPlayer(player))
					continue;

				spawnedForPlayer = true;
				SpawnWaveAround(player.GetPosition(), wave, mainConfig, totalGoal);
			}
		}

		if (!spawnedForPlayer)
			SpawnWaveAround(fallbackCenter, wave, mainConfig, totalGoal);
	}

	void Cleanup()
	{
		if (!m_Spawned || !GetGame())
			return;

		foreach (EntityAI infected: m_Spawned)
		{
			if (infected)
				GetGame().ObjectDelete(infected);
		}

		m_Spawned.Clear();
		m_DamageMultipliers.Clear();
	}

	bool IsManagedInfected(Object object)
	{
		if (!object || !m_Spawned)
			return false;

		EntityAI infected = EntityAI.Cast(object);
		if (!infected)
			return false;

		return m_Spawned.Find(infected) >= 0;
	}

	int CountAlive()
	{
		if (!m_Spawned)
			return 0;

		int count = 0;
		foreach (EntityAI infected: m_Spawned)
		{
			if (infected && infected.IsAlive())
				count++;
		}

		return count;
	}

	int CountSpawned()
	{
		if (!m_Spawned)
			return 0;

		return m_Spawned.Count();
	}

	int CountDead()
	{
		return CountSpawned() - CountAlive();
	}

	float GetDamageMultiplier(EntityAI source)
	{
		if (!source || !m_Spawned || !m_DamageMultipliers)
			return 1.0;

		int index = m_Spawned.Find(source);
		if (index < 0 || index >= m_DamageMultipliers.Count())
			return 1.0;

		float multiplier = m_DamageMultipliers.Get(index);
		if (multiplier < 1.0)
			return 1.0;

		return multiplier;
	}

	protected void SpawnWaveAround(vector origin, DZKOTH_WaveConfig wave, DZKOTH_MainConfig mainConfig, int totalGoal = 0)
	{
		int count = GetWaveCount(wave);
		for (int i = 0; i < count; i++)
			SpawnOne(origin, wave, mainConfig, totalGoal);
	}

	protected int GetWaveCount(DZKOTH_WaveConfig wave)
	{
		int min = wave.InfectedCountMin;
		int max = wave.InfectedCountMax;

		if (min < 1)
			min = 1;
		if (max < min)
			max = min;

		return Math.RandomIntInclusive(min, max);
	}

	protected void SpawnOne(vector origin, DZKOTH_WaveConfig wave, DZKOTH_MainConfig mainConfig, int totalGoal = 0)
	{
		string type = wave.Types.Get(Math.RandomIntInclusive(0, wave.Types.Count() - 1));
		vector spawnPos = FindSpawnPosition(origin, mainConfig.SpawnMinDistance, mainConfig.SpawnMaxDistance);
		EntityAI infected = EntityAI.Cast(GetGame().CreateObjectEx(type, spawnPos, ECE_PLACE_ON_SURFACE | ECE_INITAI | ECE_EQUIP_ATTACHMENTS));
		if (!infected)
		{
			DZKOTH_Utils.Warn("Could not spawn infected type " + type);
			return;
		}

		ApplyWaveStats(infected, wave);
		m_Spawned.Insert(infected);
		m_DamageMultipliers.Insert(NormalizeMultiplier(wave.DamageMultiplier));
		if (totalGoal > 0)
			DZKOTH_Utils.Log("Zombie spawned " + m_Spawned.Count().ToString() + "/" + totalGoal.ToString());
		else
			DZKOTH_Utils.Log("Zombie spawned " + m_Spawned.Count().ToString());
	}

	protected vector FindSpawnPosition(vector origin, float minDistance, float maxDistance)
	{
		if (minDistance < 8.0)
			minDistance = 8.0;
		if (maxDistance < minDistance)
			maxDistance = minDistance + 10.0;

		vector pos = origin;
		for (int attempt = 0; attempt < 10; attempt++)
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

	protected void ApplyWaveStats(EntityAI infected, DZKOTH_WaveConfig wave)
	{
		if (!infected || !wave)
			return;

		float baseHealth = infected.GetMaxHealth("", "Health");
		if (baseHealth <= 0.0)
			baseHealth = infected.GetHealth("", "Health");
		if (baseHealth <= 0.0)
			baseHealth = 100.0;

		float healthMultiplier = wave.HealthMultiplier;
		if (healthMultiplier <= 0.0)
			healthMultiplier = 1.0;

		infected.SetHealth("", "Health", baseHealth * healthMultiplier);

		if (wave.DisableRunning)
		{
			infected.SetHealth("LeftLeg", "Health", 0.0);
			infected.SetHealth("RightLeg", "Health", 0.0);
		}
	}

	protected float NormalizeMultiplier(float multiplier)
	{
		if (multiplier < 1.0)
			return 1.0;

		return multiplier;
	}

	protected bool IsValidPlayer(PlayerBase player)
	{
		return player && player.GetIdentity() && player.IsAlive() && !player.IsUnconscious();
	}
}
