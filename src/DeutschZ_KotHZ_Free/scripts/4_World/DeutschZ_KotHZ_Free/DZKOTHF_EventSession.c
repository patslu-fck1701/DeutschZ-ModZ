class DZKOTHF_EventSession
{
	protected int m_State;
	protected int m_StartedAt;
	protected string m_AbortReason;
	protected vector m_Center;
	protected float m_CaptureRadius;
	protected float m_CaptureProgress;
	protected int m_PlayerCount;
	protected int m_SmokeState;
	protected DZKOTHF_EventFlagpole m_Flagpole;
	protected EntityAI m_EventFlag;
	protected ref array<DayZInfected> m_Enemies;
	protected bool m_EnemySpawnAttempted;
	protected EntityAI m_RewardCrate;

	void DZKOTHF_EventSession()
	{
		ResetRuntime();
	}

	int SpawnEnemies(DZKOTHF_Settings settings)
	{
		if (!GetGame() || !GetGame().IsServer() || !settings || m_State != DZKOTHF_EventState.ACTIVE)
		{
			DZKOTHF_Log.Warning("Enemy spawn rejected because no active server event exists.");
			return 0;
		}

		if (m_EnemySpawnAttempted || (m_Enemies && m_Enemies.Count() > 0))
		{
			DZKOTHF_Log.Warning("Enemy spawn rejected to prevent a duplicate block.");
			return GetEnemyCount();
		}

		m_EnemySpawnAttempted = true;
		int requested = settings.EnemyCount;
		for (int i = 0; i < requested; i++)
		{
			string className = settings.EnemyClassNames.GetRandomElement();
			vector position = m_Center;
			position[0] = position[0] + Math.RandomFloatInclusive(-settings.SpawnRadius, settings.SpawnRadius);
			position[2] = position[2] + Math.RandomFloatInclusive(-settings.SpawnRadius, settings.SpawnRadius);
			position[1] = GetGame().SurfaceY(position[0], position[2]);

			DayZInfected infected = DayZInfected.Cast(GetGame().CreateObjectEx(className, position, ECE_SETUP | ECE_INITAI | ECE_PLACE_ON_SURFACE));
			if (!infected)
			{
				DZKOTHF_Log.Error("Vanilla infected spawn failed: class=" + className + " position=" + position.ToString() + ".");
				continue;
			}

			infected.SetOrientation(Vector(Math.RandomFloatInclusive(0.0, 360.0), 0.0, 0.0));
			m_Enemies.Insert(infected);
		}

		DZKOTHF_Log.Info("Enemy block spawned: requested=" + requested.ToString() + " created=" + m_Enemies.Count().ToString() + " waves=0 boss=0 expansionAI=0.");
		return m_Enemies.Count();
	}

	void CleanupEnemies()
	{
		int removed = 0;
		if (m_Enemies && GetGame())
		{
			foreach (DayZInfected infected: m_Enemies)
			{
				if (infected)
				{
					GetGame().ObjectDelete(infected);
					removed++;
				}
			}
			m_Enemies.Clear();
		}

		m_EnemySpawnAttempted = false;
		if (removed > 0)
			DZKOTHF_Log.Info("Enemy cleanup completed: removed=" + removed.ToString() + ".");
	}

	int GetEnemyCount()
	{
		if (!m_Enemies)
			return 0;

		int alive = 0;
		foreach (DayZInfected infected: m_Enemies)
		{
			if (infected && infected.IsAlive())
				alive++;
		}
		return alive;
	}

	bool SpawnRewardCrate(DZKOTHF_Settings settings)
	{
		if (!GetGame() || !GetGame().IsServer() || !settings || m_State != DZKOTHF_EventState.CAPTURE_COMPLETE)
		{
			DZKOTHF_Log.Warning("Reward spawn rejected because capture is not complete on the server.");
			return false;
		}

		if (m_RewardCrate)
		{
			DZKOTHF_Log.Warning("Reward spawn rejected: exactly-one crate already exists.");
			return false;
		}

		vector offset = settings.GetRewardSpawnOffset();
		vector position = m_Center + offset;
		position[1] = GetGame().SurfaceY(position[0], position[2]) + offset[1];
		int flags = ECE_SETUP | ECE_CREATEPHYSICS | ECE_PLACE_ON_SURFACE | ECE_NOLIFETIME | ECE_NOPERSISTENCY_WORLD;
		m_RewardCrate = EntityAI.Cast(GetGame().CreateObjectEx(settings.RewardCrateClass, position, flags));
		if (!m_RewardCrate)
		{
			DZKOTHF_Log.Error("Reward crate spawn failed: class=" + settings.RewardCrateClass + " position=" + position.ToString() + ".");
			return false;
		}

		m_RewardCrate.SetPosition(position);
		m_RewardCrate.SetAllowDamage(false);
		int createdItems = FillRewardCrate(settings);
		DZKOTHF_Log.Info("Reward crate spawned: class=" + settings.RewardCrateClass + " count=1 items=" + createdItems.ToString() + " lifetime=" + settings.RewardLifetimeMinutes.ToString() + "min.");
		return true;
	}

	protected int FillRewardCrate(DZKOTHF_Settings settings)
	{
		if (!m_RewardCrate || !m_RewardCrate.GetInventory())
			return 0;

		int createdItems = 0;
		foreach (DZKOTHF_RewardItemSetting reward: settings.RewardItems)
		{
			if (!reward || reward.Type == "")
				continue;

			for (int index = 0; index < reward.Count; index++)
			{
				if (Math.RandomFloatInclusive(0.0, 1.0) > reward.Chance)
					continue;

				EntityAI entity = m_RewardCrate.GetInventory().CreateInInventory(reward.Type);
				if (!entity)
				{
					DZKOTHF_Log.Warning("Reward item could not be created in crate: " + reward.Type + ".");
					continue;
				}

				ItemBase item = ItemBase.Cast(entity);
				if (item && item.HasQuantity())
					item.SetQuantity(Math.RandomIntInclusive(reward.MinQuantity, reward.MaxQuantity));

				createdItems++;
			}
		}

		return createdItems;
	}

	void CleanupRewardCrate(string reason)
	{
		if (!m_RewardCrate)
			return;

		if (GetGame())
			GetGame().ObjectDelete(m_RewardCrate);

		m_RewardCrate = null;
		DZKOTHF_Log.Info("Reward crate removed: " + reason + ".");
	}

	bool HasRewardCrate()
	{
		return m_RewardCrate != null;
	}

	bool SpawnWorldObjects(DZKOTHF_Settings settings)
	{
		if (!GetGame() || !GetGame().IsServer() || !settings)
			return false;

		m_Center = settings.GetEventPosition();
		m_Center[1] = GetGame().SurfaceY(m_Center[0], m_Center[2]);
		m_CaptureRadius = settings.CaptureRadius;

		int flags = ECE_SETUP | ECE_CREATEPHYSICS | ECE_PLACE_ON_SURFACE | ECE_NOLIFETIME | ECE_NOPERSISTENCY_WORLD;
		m_Flagpole = DZKOTHF_EventFlagpole.Cast(GetGame().CreateObjectEx(DZKOTHF_Constants.FLAGPOLE_CLASSNAME, m_Center, flags));
		if (!m_Flagpole)
		{
			DZKOTHF_Log.Error("Flagpole spawn failed at " + m_Center.ToString() + ".");
			return false;
		}

		m_Flagpole.SetPosition(m_Center);
		m_Flagpole.SetOrientation(settings.GetEventOrientation());
		m_EventFlag = m_Flagpole.EnsureExactlyOneEventFlag();
		if (!m_EventFlag)
		{
			DZKOTHF_Log.Error("Exactly-one event flag creation failed.");
			CleanupWorldObjects();
			return false;
		}

		SetCaptureProgress(0.0);
		DZKOTHF_Log.Info("World objects spawned: mast=1 flag=1 center=" + m_Center.ToString() + " radius=" + m_CaptureRadius.ToString() + "m.");
		return true;
	}

	void CleanupWorldObjects()
	{
		CleanupEnemies();
		bool hadWorldObjects = m_EventFlag || m_Flagpole || m_SmokeState != DZKOTHF_SmokeState.NONE;
		SetSmokeState(DZKOTHF_SmokeState.NONE);

		if (m_EventFlag && GetGame())
			GetGame().ObjectDelete(m_EventFlag);

		m_EventFlag = null;
		if (m_Flagpole && GetGame())
			GetGame().ObjectDelete(m_Flagpole);

		m_Flagpole = null;
		if (hadWorldObjects)
			DZKOTHF_Log.Info("World cleanup completed: mast=0 flag=0 smoke=NONE.");
	}

	void SetCaptureProgress(float progress)
	{
		m_CaptureProgress = Math.Clamp(progress, 0.0, 1.0);
		if (m_Flagpole)
			m_Flagpole.SetCaptureProgress(m_CaptureProgress);
	}

	void SetSmokeState(int smokeState)
	{
		if (m_SmokeState == smokeState)
			return;

		m_SmokeState = smokeState;
		if (m_Flagpole)
			m_Flagpole.SetSmokeState(smokeState);

		DZKOTHF_Log.Info("Smoke state " + DZKOTHF_SmokeState.ToString(smokeState) + ".");
	}

	void SetPlayerCount(int playerCount)
	{
		m_PlayerCount = playerCount;
	}

	int GetState()
	{
		return m_State;
	}

	int GetStartedAt()
	{
		return m_StartedAt;
	}

	string GetAbortReason()
	{
		return m_AbortReason;
	}

	vector GetCenter()
	{
		return m_Center;
	}

	float GetCaptureRadius()
	{
		return m_CaptureRadius;
	}

	float GetCaptureProgress()
	{
		return m_CaptureProgress;
	}

	int GetPlayerCount()
	{
		return m_PlayerCount;
	}

	void MarkStarted()
	{
		m_StartedAt = GetGame().GetTime();
		m_AbortReason = "";
	}

	void SetAbortReason(string reason)
	{
		m_AbortReason = reason;
	}

	void SetState(int state)
	{
		m_State = state;
	}

	void Reset()
	{
		CleanupWorldObjects();
		ResetRuntime();
	}

	protected void ResetRuntime()
	{
		m_State = DZKOTHF_EventState.IDLE;
		m_StartedAt = 0;
		m_AbortReason = "";
		m_Center = vector.Zero;
		m_CaptureRadius = 0.0;
		m_CaptureProgress = 0.0;
		m_PlayerCount = 0;
		m_SmokeState = DZKOTHF_SmokeState.NONE;
		m_Flagpole = null;
		m_EventFlag = null;
		m_Enemies = new array<DayZInfected>;
		m_EnemySpawnAttempted = false;
	}
}
