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
	protected FireworksLauncher m_FireworkLauncher;
	protected bool m_FireworkTriggered;

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

		vector offset = settings.GetRewardCrateOffset();
		vector position = GetWorldPositionFromMastOffset(offset);
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

	bool SpawnWinFirework(DZKOTHF_Settings settings)
	{
		if (!GetGame() || !GetGame().IsServer() || !settings || !settings.FireworkEnabled || m_State != DZKOTHF_EventState.CAPTURE_COMPLETE)
			return false;

		if (m_FireworkTriggered || m_FireworkLauncher)
		{
			DZKOTHF_Log.Warning("Firework spawn rejected: win firework was already triggered.");
			return false;
		}

		vector position = GetWorldPositionFromMastOffset(settings.GetFireworkOffset());
		int flags = ECE_SETUP | ECE_CREATEPHYSICS | ECE_NOLIFETIME | ECE_NOPERSISTENCY_WORLD;
		m_FireworkLauncher = FireworksLauncher.Cast(GetGame().CreateObjectEx(DZKOTHF_Constants.FIREWORK_CLASSNAME, position, flags));
		if (!m_FireworkLauncher)
		{
			DZKOTHF_Log.Error("Firework creation failed: class=" + DZKOTHF_Constants.FIREWORK_CLASSNAME + " position=" + position.ToString() + ".");
			return false;
		}

		m_FireworkLauncher.SetPosition(position);
		if (m_Flagpole)
			m_FireworkLauncher.SetOrientation(m_Flagpole.GetOrientation());
		m_FireworkTriggered = true;
		DZKOTHF_Log.Info("FIREWORK SPAWNED class=" + DZKOTHF_Constants.FIREWORK_CLASSNAME + " position=" + position.ToString() + ".");
		m_FireworkLauncher.OnIgnitedThis(null);
		DZKOTHF_Log.Info("FIREWORK STARTED class=" + DZKOTHF_Constants.FIREWORK_CLASSNAME + ".");
		return true;
	}

	void CleanupFirework(string reason)
	{
		if (m_FireworkLauncher && GetGame())
			GetGame().ObjectDelete(m_FireworkLauncher);
		if (m_FireworkLauncher || m_FireworkTriggered)
			DZKOTHF_Log.Info("FIREWORK CLEANED reason=" + reason + ".");
		m_FireworkLauncher = null;
		m_FireworkTriggered = false;
	}

	protected vector GetWorldPositionFromMastOffset(vector localOffset)
	{
		vector position = m_Center + localOffset;
		if (m_Flagpole)
			position = m_Flagpole.ModelToWorld(localOffset);
		position[1] = GetGame().SurfaceY(position[0], position[2]) + localOffset[1];
		return position;
	}

	protected int FillRewardCrate(DZKOTHF_Settings settings)
	{
		if (!m_RewardCrate || !m_RewardCrate.GetInventory())
			return 0;

		int createdItems = 0;
		ref array<string> selectedWeapons = new array<string>;
		DZKOTHF_RewardItemSetting selectedWeapon = SelectWeightedReward(settings.GuaranteedWeaponPool, selectedWeapons);
		if (selectedWeapon)
			createdItems += CreateWeaponPackage(selectedWeapon, selectedWeapons, createdItems, settings.MaxTotalRewardItems, "GuaranteedWeapons");

		int randomWeaponCount = Math.RandomIntInclusive(settings.RandomWeaponMin, settings.RandomWeaponMax);
		for (int weaponIndex = 0; weaponIndex < randomWeaponCount && createdItems < settings.MaxTotalRewardItems; weaponIndex++)
		{
			array<string> excludedWeapons = null;
			if (!settings.AllowDuplicateWeapons)
				excludedWeapons = selectedWeapons;
			selectedWeapon = SelectWeightedReward(settings.RandomWeaponPool, excludedWeapons);
			if (!selectedWeapon)
				break;
			createdItems += CreateWeaponPackage(selectedWeapon, selectedWeapons, createdItems, settings.MaxTotalRewardItems, "RandomWeapons");
		}

		createdItems += CreateWeightedPoolRewards(settings.MagazineAmmoPool, 2, createdItems, settings.MaxTotalRewardItems, "MagazineAmmo");
		createdItems += CreateWeightedPoolRewards(settings.AttachmentPool, 1, createdItems, settings.MaxTotalRewardItems, "Attachments");
		createdItems += CreateWeightedPoolRewards(settings.MedicalPool, 2, createdItems, settings.MaxTotalRewardItems, "Medical");
		createdItems += CreateWeightedPoolRewards(settings.UtilityPool, 2, createdItems, settings.MaxTotalRewardItems, "Utility");
		if (Math.RandomFloatInclusive(0.0, 1.0) <= 0.50)
			createdItems += CreateWeightedPoolRewards(settings.RarePool, 1, createdItems, settings.MaxTotalRewardItems, "Rare");

		createdItems += CreateLegacyRewards(settings.RewardItems, createdItems, settings.MaxTotalRewardItems);
		DZKOTHF_Log.Info("Reward summary: guaranteedWeapons=1 randomWeapons=" + randomWeaponCount.ToString() + " uniqueWeapons=" + selectedWeapons.Count().ToString() + " totalItems=" + createdItems.ToString() + " maxItems=" + settings.MaxTotalRewardItems.ToString() + ".");
		return createdItems;
	}

	protected int CreateWeaponPackage(DZKOTHF_RewardItemSetting weaponEntry, array<string> selectedWeapons, int currentCount, int maxTotal, string groupName)
	{
		if (!weaponEntry || currentCount >= maxTotal)
			return 0;

		int createdItems = 0;
		createdItems += CreateRewardItem(weaponEntry.Type, 1, 1, groupName);
		if (createdItems == 0)
			return 0;

		if (selectedWeapons && selectedWeapons.Find(weaponEntry.Type) == -1)
			selectedWeapons.Insert(weaponEntry.Type);

		string magazineType = GetWeaponMagazineType(weaponEntry.Type);
		if (magazineType != "" && currentCount + createdItems < maxTotal)
			createdItems += CreateRewardItem(magazineType, 10, 30, groupName + "Support");

		string ammoType = GetWeaponAmmoType(weaponEntry.Type);
		if (ammoType != "" && currentCount + createdItems < maxTotal)
			createdItems += CreateRewardItem(ammoType, 20, 40, groupName + "Support");

		return createdItems;
	}

	protected int CreateWeightedPoolRewards(array<ref DZKOTHF_RewardItemSetting> pool, int rolls, int currentCount, int maxTotal, string groupName)
	{
		if (!pool || rolls <= 0 || currentCount >= maxTotal)
			return 0;

		int createdItems = 0;
		for (int roll = 0; roll < rolls && currentCount + createdItems < maxTotal; roll++)
		{
			DZKOTHF_RewardItemSetting reward = SelectWeightedReward(pool, null);
			if (reward)
				createdItems += CreateRewardItem(reward.Type, reward.MinQuantity, reward.MaxQuantity, groupName);
		}

		return createdItems;
	}

	protected int CreateLegacyRewards(array<ref DZKOTHF_RewardItemSetting> rewards, int currentCount, int maxTotal)
	{
		if (!rewards || currentCount >= maxTotal)
			return 0;

		int createdItems = 0;
		foreach (DZKOTHF_RewardItemSetting reward: rewards)
		{
			if (!reward || reward.Type == "" || currentCount + createdItems >= maxTotal)
				continue;
			if (Math.RandomFloatInclusive(0.0, 1.0) > reward.Chance)
				continue;

			for (int index = 0; index < reward.Count && currentCount + createdItems < maxTotal; index++)
				createdItems += CreateRewardItem(reward.Type, reward.MinQuantity, reward.MaxQuantity, "RewardItems");
		}

		return createdItems;
	}

	protected DZKOTHF_RewardItemSetting SelectWeightedReward(array<ref DZKOTHF_RewardItemSetting> pool, array<string> excludedTypes)
	{
		if (!pool || pool.Count() == 0)
			return null;

		float totalWeight = 0.0;
		foreach (DZKOTHF_RewardItemSetting candidate: pool)
		{
			if (!candidate || candidate.Type == "" || candidate.Chance <= 0.0)
				continue;
			if (excludedTypes && excludedTypes.Find(candidate.Type) != -1)
				continue;
			totalWeight += candidate.Chance;
		}

		if (totalWeight <= 0.0)
			return null;

		float roll = Math.RandomFloatInclusive(0.0, totalWeight);
		float cursor = 0.0;
		foreach (DZKOTHF_RewardItemSetting entry: pool)
		{
			if (!entry || entry.Type == "" || entry.Chance <= 0.0)
				continue;
			if (excludedTypes && excludedTypes.Find(entry.Type) != -1)
				continue;
			cursor += entry.Chance;
			if (roll <= cursor)
				return entry;
		}

		return null;
	}

	protected int CreateRewardItem(string type, int minQuantity, int maxQuantity, string groupName)
	{
		if (type == "" || !m_RewardCrate || !m_RewardCrate.GetInventory())
			return 0;

		EntityAI entity = m_RewardCrate.GetInventory().CreateInInventory(type);
		if (!entity)
		{
			DZKOTHF_Log.Warning("Reward item class=" + type + " success=NO target=" + m_RewardCrate.GetType() + " count=0 group=" + groupName + " reason=CreateInInventory_failed_or_no_cargo_space.");
			return 0;
		}

		ItemBase item = ItemBase.Cast(entity);
		if (item && item.HasQuantity())
			item.SetQuantity(Math.RandomIntInclusive(minQuantity, maxQuantity));

		DZKOTHF_Log.Info("Reward item class=" + type + " success=YES target=" + m_RewardCrate.GetType() + " count=1 group=" + groupName + " reason=created.");
		return 1;
	}

	protected string GetWeaponMagazineType(string weaponType)
	{
		if (weaponType == "M4A1" || weaponType == "M16A2") return "Mag_STANAG_30Rnd";
		if (weaponType == "AKM") return "Mag_AKM_30Rnd";
		if (weaponType == "AK74") return "Mag_AK74_30Rnd";
		if (weaponType == "FAL") return "Mag_FAL_20Rnd";
		if (weaponType == "SVD") return "Mag_SVD_10Rnd";
		if (weaponType == "Saiga") return "Mag_Saiga_8Rnd";
		if (weaponType == "MP5K") return "Mag_MP5_30Rnd";
		if (weaponType == "UMP45") return "Mag_UMP_25Rnd";
		return "";
	}

	protected string GetWeaponAmmoType(string weaponType)
	{
		if (weaponType == "M4A1" || weaponType == "M16A2") return "Ammo_556x45";
		if (weaponType == "AKM" || weaponType == "SKS") return "Ammo_762x39";
		if (weaponType == "AK74") return "Ammo_545x39";
		if (weaponType == "FAL" || weaponType == "Winchester70") return "Ammo_308Win";
		if (weaponType == "SVD" || weaponType == "Mosin9130") return "Ammo_762x54";
		if (weaponType == "Saiga") return "Ammo_12gaPellets";
		if (weaponType == "MP5K") return "Ammo_9x19";
		if (weaponType == "UMP45") return "Ammo_45ACP";
		return "";
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
		CleanupFirework("event cleanup");
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
		m_FireworkLauncher = null;
		m_FireworkTriggered = false;
	}
}
