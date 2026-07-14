class DZCRZ_ATMManager
{
	protected DZCRZ_Config m_Config;
	protected ref map<string, Object> m_ATMs;
	protected ref DZCRZ_ATMPositionsFile m_Positions;

	void DZCRZ_ATMManager(DZCRZ_Config config)
	{
		m_Config = config;
		m_ATMs = new map<string, Object>;
		m_Positions = new DZCRZ_ATMPositionsFile;
		LoadPositions();
	}

	void SetConfig(DZCRZ_Config config)
	{
		m_Config = config;
	}

	void SpawnConfiguredATMs()
	{
		if (!m_Config || !m_Config.ATMModule || !m_Config.ATMModule.Enabled)
			return;
		foreach (DZCRZ_ATMPosition entry: m_Positions.Positions)
		{
			if (entry && entry.Enabled)
				SpawnATM(entry);
		}
		DZCRZ_Log.Info("Konfigurierte ATMs aktiv: " + m_ATMs.Count().ToString());
	}

	DZCRZ_ATM SpawnAdminATM(vector position, vector orientation)
	{
		DZCRZ_ATMPosition entry = new DZCRZ_ATMPosition;
		entry.Id = "atm_admin_" + Math.Round(position[0]).ToString() + "_" + Math.Round(position[2]).ToString();
		entry.Name = "Admin ATM";
		entry.SetPosition(position);
		entry.SetOrientation(orientation);
		m_Positions.Positions.Insert(entry);
		SavePositions();
		return SpawnATM(entry);
	}

	void RegisterATM(DZCRZ_ATM atm)
	{
		if (!atm)
			return;
		string atmId = atm.DZCRZ_GetATMId();
		if (atmId == "")
		{
			atmId = DZCRZ_PlayerUtils.SafeObjectId(atm);
			atm.DZCRZ_SetATMId(atmId);
		}
		m_ATMs.Set(atmId, atm);
	}

	void RegisterExpansionATM(ExpansionATM_2 atm)
	{
		if (!atm)
			return;
		string atmId = atm.DZCRZ_GetATMId();
		if (atmId == "")
		{
			atmId = DZCRZ_PlayerUtils.SafeObjectId(atm);
			atm.DZCRZ_SetATMId(atmId);
		}
		if (m_Config && m_Config.ATMModule)
			atm.DZCRZ_AssignRandomDuration(m_Config.ATMModule.HackTimeMinSeconds, m_Config.ATMModule.HackTimeMaxSeconds);
		m_ATMs.Set(atmId, atm);
	}

	void UnregisterObject(Object atmObject)
	{
		if (!atmObject)
			return;
		array<string> removeIds = new array<string>;
		foreach (string atmId, Object registeredATM: m_ATMs)
		{
			if (registeredATM == atmObject)
				removeIds.Insert(atmId);
		}
		foreach (string removeId: removeIds)
			m_ATMs.Remove(removeId);
	}

	int GetCount()
	{
		return m_ATMs.Count();
	}

	string FindNearestId(vector position, float maximumDistance = 10.0)
	{
		string nearestId;
		float nearestDistance = maximumDistance;
		foreach (string atmId, Object atm: m_ATMs)
		{
			if (!atm)
				continue;
			float distance = vector.Distance(position, atm.GetPosition());
			if (distance <= nearestDistance)
			{
				nearestDistance = distance;
				nearestId = atmId;
			}
		}
		return nearestId;
	}

	bool AddStoreATM(vector position, vector orientation, string name)
	{
		foreach (DZCRZ_ATMPosition existing: m_Positions.Positions)
		{
			if (existing && vector.Distance(existing.GetPosition(), position) < 35.0)
				return false;
		}
		DZCRZ_ATMPosition entry = new DZCRZ_ATMPosition;
		entry.Id = "atm_store_" + Math.Round(position[0]).ToString() + "_" + Math.Round(position[2]).ToString();
		entry.Name = name;
		entry.SetPosition(position);
		entry.SetOrientation(orientation);
		m_Positions.Positions.Insert(entry);
		SpawnATM(entry);
		if (m_Config.ATMModule.FallbackWriteStoresToConfig)
			SavePositions();
		return true;
	}

	protected DZCRZ_ATM SpawnATM(DZCRZ_ATMPosition entry)
	{
		if (!entry || entry.Id == "")
			return null;
		Object existingObject;
		if (m_ATMs.Find(entry.Id, existingObject) && existingObject)
			return DZCRZ_ATM.Cast(existingObject);
		vector position = entry.GetPosition();
		DZCRZ_ATM atm = DZCRZ_ATM.Cast(GetGame().CreateObjectEx(DZCRZ_Const.ATM_CLASS, position, ECE_CREATEPHYSICS | ECE_UPDATEPATHGRAPH));
		if (!atm)
		{
			DZCRZ_Log.Error("ATM konnte nicht gespawnt werden: " + entry.Id);
			return null;
		}
		atm.SetOrientation(entry.GetOrientation());
		atm.SetPosition(position);
		atm.DZCRZ_SetATMId(entry.Id);
		atm.DZCRZ_AssignRandomDuration(m_Config.ATMModule.HackTimeMinSeconds, m_Config.ATMModule.HackTimeMaxSeconds);
		atm.SetSynchDirty();
		m_ATMs.Set(entry.Id, atm);
		return atm;
	}

	protected void LoadPositions()
	{
		string errorMessage;
		if (FileExist(DZCRZ_ProfilePaths.ATM_POSITIONS))
		{
			if (!JsonFileLoader<ref DZCRZ_ATMPositionsFile>.LoadFile(DZCRZ_ProfilePaths.ATM_POSITIONS, m_Positions, errorMessage))
				DZCRZ_Log.Error("ATM-Positionen konnten nicht geladen werden: " + errorMessage);
		}
		if (!m_Positions || m_Positions.Positions.Count() == 0)
			CreateDefaultPositions();
		SavePositions();
	}

	protected void CreateDefaultPositions()
	{
		m_Positions = new DZCRZ_ATMPositionsFile;
		AddDefault("atm_green_mountain_01", "Green Mountain 1", "3708.102051 402.980347 5959.185547", "162.440445 0 0");
		AddDefault("atm_green_mountain_02", "Green Mountain 2", "3706.087158 402.322876 6003.259766", "154.170258 0 0");
		AddDefault("atm_kamenka_blackmarket_01", "Kamenka Blackmarket", "1133.584473 5.857156 2418.129150", "-114.936584 0 0");
	}

	protected void AddDefault(string id, string name, vector position, vector orientation)
	{
		DZCRZ_ATMPosition entry = new DZCRZ_ATMPosition;
		entry.Id = id;
		entry.Name = name;
		entry.SetPosition(position);
		entry.SetOrientation(orientation);
		m_Positions.Positions.Insert(entry);
	}

	protected void SavePositions()
	{
		string errorMessage;
		if (!JsonFileLoader<ref DZCRZ_ATMPositionsFile>.SaveFile(DZCRZ_ProfilePaths.ATM_POSITIONS, m_Positions, errorMessage))
			DZCRZ_Log.Error("ATM-Positionen konnten nicht gespeichert werden: " + errorMessage);
	}
}
