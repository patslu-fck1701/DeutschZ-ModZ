class DZKOTH_WaveManager
{
	protected ref array<EntityAI> m_Spawned;
	protected ref array<float> m_DamageMultipliers;
	protected ref array<vector> m_SpawnPositions;

	void DZKOTH_WaveManager()
	{
		m_Spawned = new array<EntityAI>;
		m_DamageMultipliers = new array<float>;
		m_SpawnPositions = new array<vector>;
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
				SpawnWaveAround(player.GetPosition(), wave, mainConfig, fallbackCenter, totalGoal);
			}
		}

		if (!spawnedForPlayer)
			SpawnWaveAround(fallbackCenter, wave, mainConfig, fallbackCenter, totalGoal);
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
		m_SpawnPositions.Clear();
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

	protected void SpawnWaveAround(vector origin, DZKOTH_WaveConfig wave, DZKOTH_MainConfig mainConfig, vector eventCenter, int totalGoal = 0)
	{
		int count = GetWaveCount(wave);
		for (int i = 0; i < count; i++)
			SpawnOne(origin, wave, mainConfig, eventCenter, totalGoal);
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

	protected void SpawnOne(vector origin, DZKOTH_WaveConfig wave, DZKOTH_MainConfig mainConfig, vector eventCenter, int totalGoal = 0)
	{
		string type = wave.Types.Get(Math.RandomIntInclusive(0, wave.Types.Count() - 1));
		vector spawnPos = FindSpawnPosition(origin, mainConfig.SpawnMinDistance, mainConfig.SpawnMaxDistance, eventCenter, mainConfig.CaptureRadius + 3.0);
		EntityAI infected = EntityAI.Cast(GetGame().CreateObjectEx(type, spawnPos, ECE_PLACE_ON_SURFACE | ECE_INITAI | ECE_EQUIP_ATTACHMENTS));
		if (!infected)
		{
			DZKOTH_Utils.Warn("Could not spawn infected type " + type);
			return;
		}

		ApplyWaveStats(infected, wave);
		m_Spawned.Insert(infected);
		m_DamageMultipliers.Insert(NormalizeMultiplier(wave.DamageMultiplier));
		m_SpawnPositions.Insert(spawnPos);
		if (totalGoal > 0)
			DZKOTH_Utils.Log("Zombie spawned " + m_Spawned.Count().ToString() + "/" + totalGoal.ToString());
		else
			DZKOTH_Utils.Log("Zombie spawned " + m_Spawned.Count().ToString());
	}

	protected vector FindSpawnPosition(vector origin, float minDistance, float maxDistance, vector eventCenter, float exclusionRadius)
	{
		float minimumOutsideDistance = exclusionRadius + 2.0;
		if (minDistance < minimumOutsideDistance)
			minDistance = minimumOutsideDistance;
		if (maxDistance < minDistance)
			maxDistance = minDistance + 15.0;

		vector pos = origin;
		for (int attempt = 0; attempt < 20; attempt++)
		{
			float angle = Math.RandomFloatInclusive(0.0, 6.28318);
			float distance = Math.RandomFloatInclusive(minDistance, maxDistance);
			pos = origin + Vector(Math.Cos(angle) * distance, 0, Math.Sin(angle) * distance);
			pos = DZKOTH_Utils.Grounded(pos);

			if (!GetGame().SurfaceIsSea(pos[0], pos[2]) && vector.Distance(pos, eventCenter) > exclusionRadius && IsSeparatedFromOtherSpawns(pos, 7.0))
				return pos;
		}

		float fallbackDistance = exclusionRadius + 5.0;
		float fallbackAngle = m_SpawnPositions.Count() * 0.9;
		return DZKOTH_Utils.Grounded(eventCenter + Vector(Math.Cos(fallbackAngle) * fallbackDistance, 0, Math.Sin(fallbackAngle) * fallbackDistance));
	}

	protected bool IsSeparatedFromOtherSpawns(vector position, float minimumDistance)
	{
		foreach (vector existing: m_SpawnPositions)
		{
			if (vector.Distance(position, existing) < minimumDistance)
				return false;
		}

		return true;
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

		float targetHealth = wave.ForcedHealth;
		if (targetHealth <= 0.0)
		{
			float healthMultiplier = wave.HealthMultiplier;
			if (healthMultiplier <= 0.0)
				healthMultiplier = 1.0;

			targetHealth = baseHealth * healthMultiplier;
		}

		infected.SetHealth("", "Health", targetHealth);

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
