class DZKOTH_WaveManager
{
	protected static const float SPAWN_RADIUS_MIN = 6.0;
	protected static const float SPAWN_RADIUS_MAX = 15.0;
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

		// Every wave is centered on the event mast. This keeps all infected within
		// the configured event area even when the capturing player moves around.
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
		vector spawnPos = FindSpawnPosition(origin, mainConfig.SpawnMinDistance, mainConfig.SpawnMaxDistance);
		if (spawnPos == vector.Zero)
		{
			DZKOTH_Utils.Warn("No safe infected spawn position found around mast for type " + type);
			return;
		}

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

	protected vector FindSpawnPosition(vector origin, float minDistance, float maxDistance)
	{
		if (minDistance < SPAWN_RADIUS_MIN)
			minDistance = SPAWN_RADIUS_MIN;
		if (maxDistance > SPAWN_RADIUS_MAX || maxDistance <= 0.0)
			maxDistance = SPAWN_RADIUS_MAX;
		if (maxDistance < minDistance)
			minDistance = Math.Max(3.0, maxDistance - 4.0);

		vector pos = origin;
		for (int attempt = 0; attempt < 20; attempt++)
		{
			float angle = Math.RandomFloatInclusive(0.0, 6.28318);
			float distance = Math.RandomFloatInclusive(minDistance, maxDistance);
			pos = origin + Vector(Math.Cos(angle) * distance, 0, Math.Sin(angle) * distance);
			pos = DZKOTH_Utils.Grounded(pos);

			if (IsSafeSpawnPosition(pos) && IsSeparatedFromOtherSpawns(pos, 4.0))
				return pos;
		}

		return vector.Zero;
	}

	protected bool IsSafeSpawnPosition(vector pos)
	{
		if (!GetGame() || GetGame().SurfaceIsSea(pos[0], pos[2]) || GetGame().SurfaceIsPond(pos[0], pos[2]))
			return false;

		array<vector> terrainSamples = new array<vector>;
		terrainSamples.Insert(pos + "0.75 0 0.75");
		terrainSamples.Insert(pos + "-0.75 0 0.75");
		terrainSamples.Insert(pos + "0.75 0 -0.75");
		terrainSamples.Insert(pos + "-0.75 0 -0.75");
		if (GetGame().GetHighestSurfaceYDifference(terrainSamples) > 1.25)
			return false;

		array<Object> excluded = new array<Object>;
		array<Object> collided = new array<Object>;
		if (GetGame().IsBoxColliding(pos + "0 0.9 0", vector.Zero, "0.9 1.8 0.9", excluded, collided))
			return false;

		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		foreach (Man man: players)
		{
			if (man && vector.Distance(man.GetPosition(), pos) < 3.5)
				return false;
		}

		return true;
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
		float actualHealth = infected.GetHealth("", "Health");
		float actualMaxHealth = infected.GetMaxHealth("", "Health");

		if (wave.DisableRunning)
		{
			DayZInfected infectedAgent = DayZInfected.Cast(infected);
			if (infectedAgent && infectedAgent.GetInputController())
				infectedAgent.GetInputController().OverrideMovementSpeed(true, 1.0);
		}

		DZKOTH_Utils.Log("Applied zombie stats type=" + infected.GetType() + " health=" + actualHealth.ToString() + "/" + actualMaxHealth.ToString() + " walkOnly=" + wave.DisableRunning.ToString());
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
