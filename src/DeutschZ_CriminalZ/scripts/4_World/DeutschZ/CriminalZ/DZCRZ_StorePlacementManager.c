class DZCRZ_StorePlacementManager
{
	protected static ref DZCRZ_StorePlacementManager s_Instance;
	protected bool m_Initialized;
	protected bool m_FallbackScanDone;
	protected bool m_FallbackScanScheduled;
	protected ref DZCRZ_StorePlacementSettings m_Settings;
	protected ref set<string> m_QueuedStores;
	protected ref set<string> m_ProcessedStores;
	protected ref map<string, int> m_StoreCounts;
	protected int m_ATMsCreated;
	protected int m_SignsCreated;
	protected int m_Reused;
	protected int m_Errors;
	protected bool m_ReferenceTestPassed;

	void DZCRZ_StorePlacementManager()
	{
		m_QueuedStores = new set<string>;
		m_ProcessedStores = new set<string>;
		m_StoreCounts = new map<string, int>;
	}

	static DZCRZ_StorePlacementManager Get()
	{
		if (!s_Instance)
			s_Instance = new DZCRZ_StorePlacementManager;
		return s_Instance;
	}

	static void Destroy()
	{
		s_Instance = null;
	}

	void Init()
	{
		if (!GetGame() || !GetGame().IsServer())
			return;
		if (m_Initialized)
		{
			EnsureFallbackScan();
			return;
		}
		m_Settings = DZCRZ_StorePlacementSettings.Load();
		m_Initialized = true;
		m_ReferenceTestPassed = RunReferenceTest();
		EnsureFallbackScan();
	}

	void EnsureFallbackScan()
	{
		if (m_FallbackScanDone || m_FallbackScanScheduled || !GetGame() || !GetGame().IsServer())
			return;
		m_FallbackScanScheduled = true;
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(FallbackWorldScan, 12000, false);
	}

	void QueueStore(House store)
	{
		if (!GetGame() || !GetGame().IsServer() || !store)
			return;
		if (!m_Initialized)
			Init();
		if (!m_Settings || !m_Settings.Enabled || !IsSupportedStore(store.GetType()))
			return;
		string key = BuildStoreKey(store);
		if (m_ProcessedStores.Find(key) >= 0 || m_QueuedStores.Find(key) >= 0)
			return;
		m_QueuedStores.Insert(key);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ProcessStore, m_Settings.SpawnDelayMilliseconds, false, store, key);
	}

	protected void ProcessStore(House store, string key)
	{
		int queuedIndex = m_QueuedStores.Find(key);
		if (queuedIndex >= 0)
			m_QueuedStores.Remove(queuedIndex);
		if (!store || !m_Settings || !m_Settings.Enabled || m_ProcessedStores.Find(key) >= 0)
			return;
		string storeType = store.GetType();
		if (!IsSupportedStore(storeType))
			return;
		m_ProcessedStores.Insert(key);
		IncrementStoreCount(storeType);
		array<ref DZCRZ_StorePlacementEntry> placements = m_Settings.GetPlacementsFor(storeType);
		foreach (DZCRZ_StorePlacementEntry placement: placements)
		{
			if (placement)
				PlaceObject(store, placement);
		}
	}

	protected void PlaceObject(House store, DZCRZ_StorePlacementEntry placement)
	{
		string configPath = "CfgVehicles " + placement.Type;
		if (!GetGame().ConfigIsExisting(configPath))
		{
			m_Errors++;
			Print("[DeutschZ StorePlacement] FEHLER Klasse fehlt: " + placement.Type + " Store=" + store.GetType() + " Pos=" + store.GetPosition().ToString());
			return;
		}
		vector worldPosition = store.ModelToWorld(placement.GetLocalPosition());
		vector worldOrientation = ComposeOrientation(store, placement.GetLocalOrientation());
		Object object = FindExisting(placement.Type, worldPosition);
		bool reused = object != null;
		if (!object)
		{
			int flags = ECE_SETUP | ECE_CREATEPHYSICS | ECE_UPDATEPATHGRAPH | ECE_KEEPHEIGHT | ECE_NOSURFACEALIGN | ECE_NOLIFETIME | ECE_NOPERSISTENCY_WORLD;
			object = GetGame().CreateObjectEx(placement.Type, worldPosition, flags, RF_IGNORE);
		}
		if (!object)
		{
			m_Errors++;
			Print("[DeutschZ StorePlacement] FEHLER Spawn: " + placement.Type + " Store=" + store.GetType() + " Pos=" + store.GetPosition().ToString());
			return;
		}
		object.SetPosition(worldPosition);
		object.SetOrientation(worldOrientation);
		object.SetScale(placement.Scale);
		if (reused)
			m_Reused++;
		else if (placement.Type == "ExpansionATM_2")
			m_ATMsCreated++;
		else if (placement.Type == "StaticObj_FueldStation_Sign" || placement.Type == "DZCRZ_StoreInfoSign")
			m_SignsCreated++;
		if (m_Settings.EnableDebugLogging)
		{
			string result = "SPAWN";
			if (reused)
				result = "REUSE";
			Print("[DeutschZ StorePlacement] Store=" + store.GetType() + " StorePos=" + store.GetPosition().ToString() + " StoreOri=" + store.GetOrientation().ToString() + " Object=" + placement.Type + " WorldPos=" + worldPosition.ToString() + " WorldOri=" + worldOrientation.ToString() + " Result=" + result);
		}
	}

	protected Object FindExisting(string typeName, vector position)
	{
		array<Object> objects = new array<Object>;
		array<CargoBase> proxies = new array<CargoBase>;
		GetGame().GetObjectsAtPosition3D(position, m_Settings.DuplicateRadius, objects, proxies);
		foreach (Object candidate: objects)
		{
			if (candidate && candidate.GetType() == typeName && vector.Distance(candidate.GetPosition(), position) <= m_Settings.DuplicateRadius)
				return candidate;
		}
		return null;
	}

	protected vector ComposeOrientation(Object store, vector localOrientation)
	{
		vector storeOrientation = store.GetOrientation();
		if (Math.AbsFloat(storeOrientation[1]) < 0.001 && Math.AbsFloat(storeOrientation[2]) < 0.001)
			return Vector(NormalizeAngle(storeOrientation[0] + localOrientation[0]), localOrientation[1], localOrientation[2]);
		vector storeMatrix[4];
		vector localMatrix[4];
		vector resultMatrix[4];
		store.GetTransform(storeMatrix);
		Math3D.YawPitchRollMatrix(localOrientation, localMatrix);
		Math3D.MatrixMultiply3(storeMatrix, localMatrix, resultMatrix);
		vector result = Math3D.MatrixToAngles(resultMatrix);
		result[0] = NormalizeAngle(result[0]);
		return result;
	}

	protected float NormalizeAngle(float angle)
	{
		while (angle > 180.0)
			angle = angle - 360.0;
		while (angle <= -180.0)
			angle = angle + 360.0;
		return angle;
	}

	protected bool IsSupportedStore(string typeName)
	{
		return m_Settings && m_Settings.StoreTypes.Find(typeName) >= 0;
	}

	protected string BuildStoreKey(Object store)
	{
		vector position = store.GetPosition();
		vector orientation = store.GetOrientation();
		return store.GetType() + "_" + Math.Round(position[0] * 10.0).ToString() + "_" + Math.Round(position[1] * 10.0).ToString() + "_" + Math.Round(position[2] * 10.0).ToString() + "_" + Math.Round(orientation[0] * 10.0).ToString() + "_" + Math.Round(orientation[1] * 10.0).ToString() + "_" + Math.Round(orientation[2] * 10.0).ToString();
	}

	protected void IncrementStoreCount(string storeType)
	{
		int count;
		m_StoreCounts.Find(storeType, count);
		m_StoreCounts.Set(storeType, count + 1);
	}

	protected void FallbackWorldScan()
	{
		m_FallbackScanScheduled = false;
		if (m_FallbackScanDone || !GetGame() || !GetGame().IsServer())
			return;
		m_FallbackScanDone = true;
		array<Object> objects = new array<Object>;
		array<CargoBase> proxies = new array<CargoBase>;
		GetGame().GetObjectsAtPosition3D("7680 0 7680", 12000.0, objects, proxies);
		foreach (Object object: objects)
		{
			House store = House.Cast(object);
			if (store && IsSupportedStore(store.GetType()))
				QueueStore(store);
		}
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(PrintSummary, m_Settings.SpawnDelayMilliseconds + 1000, false);
	}

	protected void PrintSummary()
	{
		int cityStores;
		int stairStores;
		m_StoreCounts.Find("Land_City_Store", cityStores);
		m_StoreCounts.Find("Land_City_Store_WithStairs", stairStores);
		Print("[DeutschZ StorePlacement] Stores gefunden: " + m_ProcessedStores.Count().ToString());
		Print("[DeutschZ StorePlacement] Land_City_Store: " + cityStores.ToString());
		Print("[DeutschZ StorePlacement] Land_City_Store_WithStairs: " + stairStores.ToString());
		Print("[DeutschZ StorePlacement] ATMs erstellt: " + m_ATMsCreated.ToString());
		Print("[DeutschZ StorePlacement] Schilder erstellt: " + m_SignsCreated.ToString());
		Print("[DeutschZ StorePlacement] Vorhandene Objekte wiederverwendet: " + m_Reused.ToString());
		Print("[DeutschZ StorePlacement] Fehler: " + m_Errors.ToString());
		Print("[DeutschZ StorePlacement] Referenztest: " + m_ReferenceTestPassed.ToString());
	}

	protected bool RunReferenceTest()
	{
		vector storePosition = "12051.7939453125 31.41855812072754 9925.650390625";
		float storeYaw = 103.11900329589844;
		vector atm = TransformYawReference(storePosition, storeYaw, "-8.898676 -1.205345 -3.431599");
		vector sign = TransformYawReference(storePosition, storeYaw, "0.725949 1.643461 10.425142");
		vector expectedATM = "12050.4716796875 30.213212966918947 9935.095703125";
		vector expectedSign = "12061.7822265625 33.06201934814453 9922.5771484375";
		float atmYaw = NormalizeAngle(storeYaw - 90.035360);
		float signYaw = NormalizeAngle(storeYaw - 174.602692);
		bool positionsValid = vector.Distance(atm, expectedATM) <= 0.02 && vector.Distance(sign, expectedSign) <= 0.02;
		bool rotationsValid = Math.AbsFloat(atmYaw - 13.083642959594727) <= 0.10 && Math.AbsFloat(signYaw - -71.48368835449219) <= 0.10;
		bool passed = positionsValid && rotationsValid;
		Print("[DeutschZ StorePlacement] Referenztest Position ATM=" + atm.ToString() + " Schild=" + sign.ToString() + " Ergebnis=" + passed.ToString());
		return passed;
	}

	protected vector TransformYawReference(vector storePosition, float storeYaw, vector localPosition)
	{
		float radians = storeYaw * Math.DEG2RAD;
		float cosine = Math.Cos(radians);
		float sine = Math.Sin(radians);
		vector result;
		result[0] = storePosition[0] + (localPosition[0] * cosine) + (localPosition[2] * sine);
		result[1] = storePosition[1] + localPosition[1];
		result[2] = storePosition[2] - (localPosition[0] * sine) + (localPosition[2] * cosine);
		return result;
	}
}

modded class House
{
	override void EEInit()
	{
		super.EEInit();
		if (GetGame() && GetGame().IsServer())
			DZCRZ_StorePlacementManager.Get().QueueStore(this);
	}
}
