class DZBBC_MainConfig
{
	string EventName;
	bool AutoStart;
	int AutoStartDelaySeconds;
	int EventCooldownMinutes;
	float TickSeconds;
	float CombatRadius;
	float MarkerRadius;
	float HackDurationSeconds;
	float TerminalDecryptSeconds;
	float DataCoreLifetimeSeconds;
	float CleanupDelaySeconds;
	float SpawnMinDistance;
	float SpawnMaxDistance;
	float PlayerInteractionDistance;
	float SectorSize;
	bool DebugCommandsEnabled;
	bool AllowAdminWhenListEmpty;
	bool TestModeEnabled;
	bool TestModeAutoSpawnOnStart;
	bool TestModeTeleportOnJoin;
	bool TestModeVisualTestOnJoin;
	int TestModeTeleportRetryCount;
	int TestModeTeleportRetrySeconds;
	int TestModeAutoSpawnDelaySeconds;
	ref array<string> AdminUIDs;

	void DZBBC_MainConfig()
	{
		EventName = "#STR_DZBBC_EVENT_NAME";
		AutoStart = true;
		AutoStartDelaySeconds = 60;
		EventCooldownMinutes = 120;
		TickSeconds = 1.0;
		CombatRadius = 85.0;
		MarkerRadius = 420.0;
		HackDurationSeconds = 90.0;
		TerminalDecryptSeconds = 60.0;
		DataCoreLifetimeSeconds = 1800.0;
		CleanupDelaySeconds = 300.0;
		SpawnMinDistance = 20.0;
		SpawnMaxDistance = 55.0;
		PlayerInteractionDistance = 4.0;
		SectorSize = 1000.0;
		DebugCommandsEnabled = true;
		AllowAdminWhenListEmpty = true;
		TestModeEnabled = false;
		TestModeAutoSpawnOnStart = false;
		TestModeTeleportOnJoin = false;
		TestModeVisualTestOnJoin = false;
		TestModeTeleportRetryCount = 4;
		TestModeTeleportRetrySeconds = 4;
		TestModeAutoSpawnDelaySeconds = 20;
		AdminUIDs = new array<string>;
	}
}

class DZBBC_SceneObjectConfig
{
	string ClassName;
	ref array<float> Offset;
	ref array<float> Orientation;
	bool AlignToGround;
	bool Required;

	void DZBBC_SceneObjectConfig()
	{
		ClassName = "";
		Offset = DZBBC_Utils.MakeVectorArray(0.0, 0.0, 0.0);
		Orientation = DZBBC_Utils.MakeVectorArray(0.0, 0.0, 0.0);
		AlignToGround = true;
		Required = true;
	}

	vector GetOffset()
	{
		return DZBBC_Utils.ArrayToVector(Offset);
	}

	vector GetOrientation()
	{
		return DZBBC_Utils.ArrayToVector(Orientation);
	}
}

class DZBBC_CrashSiteConfig
{
	string Name;
	ref array<float> Position;
	ref array<float> BlackboxPosition;
	ref array<float> BlackboxOrientation;
	ref array<float> ContainerPosition;
	float Radius;
	ref array<string> WreckObjects;
	ref array<string> EffectObjects;
	ref array<ref DZBBC_SceneObjectConfig> SceneObjects;

	void DZBBC_CrashSiteConfig()
	{
		Name = "Airfield Convoy Test";
		Position = DZBBC_Utils.MakeVectorArray(4520.0, 0.0, 10280.0);
		BlackboxPosition = DZBBC_Utils.MakeVectorArray(4528.0, 0.0, 10272.0);
		BlackboxOrientation = DZBBC_Utils.MakeVectorArray(0.0, 0.0, 0.0);
		ContainerPosition = DZBBC_Utils.MakeVectorArray(4545.0, 0.0, 10292.0);
		Radius = 95.0;
		WreckObjects = new array<string>;
		WreckObjects.Insert(DZBBC_WRECK_PROXY_CLASSNAME);
		EffectObjects = new array<string>;
		EffectObjects.Insert("M18SmokeGrenade_White");
		SceneObjects = new array<ref DZBBC_SceneObjectConfig>;
		EnsureDefaultScene();
	}

	vector GetPosition()
	{
		return DZBBC_Utils.ArrayToVector(Position);
	}

	vector GetBlackboxPosition()
	{
		return DZBBC_Utils.ArrayToVector(BlackboxPosition, GetPosition());
	}

	vector GetBlackboxOrientation()
	{
		return DZBBC_Utils.ArrayToVector(BlackboxOrientation);
	}

	vector GetContainerPosition()
	{
		return DZBBC_Utils.ArrayToVector(ContainerPosition, GetPosition());
	}

	bool EnsureDefaultScene()
	{
		bool changed = false;
		if (!BlackboxOrientation || BlackboxOrientation.Count() < 3)
		{
			BlackboxOrientation = DZBBC_Utils.MakeVectorArray(0.0, 0.0, 0.0);
			changed = true;
		}

		if (!WreckObjects)
		{
			WreckObjects = new array<string>;
			changed = true;
		}
		else if (WreckObjects.Count() > 0)
		{
			WreckObjects.Clear();
			changed = true;
		}

		if (!EffectObjects)
		{
			EffectObjects = new array<string>;
			changed = true;
		}
		else if (EffectObjects.Count() > 0)
		{
			EffectObjects.Clear();
			changed = true;
		}

		if (!SceneObjects)
		{
			SceneObjects = new array<ref DZBBC_SceneObjectConfig>;
			changed = true;
		}

		if (SceneObjects.Count() > 0)
			return NormalizeSceneObjects() || changed;

		AddSceneObject("Wreck_UH1Y", 0.0, 0.0, 0.0, 42.0, 0.0, 0.0, true);
		AddSceneObject("StaticObj_Wreck_Decal_Big", 0.0, 0.0, 0.0, 42.0, 0.0, 0.0, false);
		AddSceneObject("StaticObj_Wreck_Ural", -20.0, 0.0, -7.0, 82.0, 0.0, 0.0, true);
		AddSceneObject("Land_Wreck_Uaz", -8.0, 0.0, -20.0, 118.0, 0.0, 0.0, true);
		AddSceneObject("StaticObj_Wreck_HMMWV", 18.0, 1.65, -14.0, 240.0, 0.0, 0.0, true);
		AddSceneObject("StaticObj_Wreck_Trailer_Flat", -30.0, 0.0, 8.0, 74.0, 0.0, 0.0, false);
		AddSceneObject("StaticObj_Wreck_BRDM", 30.0, 0.0, 13.0, 285.0, 0.0, 0.0, false);
		AddSceneObject("StaticObj_Wreck_Decal_Small1", 12.0, 0.0, 5.0, 0.0, 0.0, 0.0, false);
		AddSceneObject("StaticObj_Wreck_Decal_Small2", -14.0, 0.0, 15.0, 0.0, 0.0, 0.0, false);
		AddSceneObject("StaticObj_Wreck_T72_Chassis", 8.0, 2.05, 10.0, 25.0, 0.0, 0.0, false);
		AddSceneObject("StaticObj_Wreck_BMP1", -12.0, 0.0, 9.0, 170.0, 0.0, 0.0, false);
		AddSceneObject("M18SmokeGrenade_White", 6.0, 0.0, 4.0, 0.0, 0.0, 0.0, false);
		AddSceneObject("M18SmokeGrenade_White", -7.0, 0.0, 7.0, 0.0, 0.0, 0.0, false);
		AddSceneObject("M18SmokeGrenade_White", 15.0, 0.0, -6.0, 0.0, 0.0, 0.0, false);
		AddSceneObject("Roadflare", -4.0, 0.0, 14.0, 35.0, 0.0, 0.0, false);
		AddSceneObject("Roadflare", 14.0, 0.0, -2.0, 125.0, 0.0, 0.0, false);
		return true;
	}

	protected bool NormalizeSceneObjects()
	{
		bool changed = false;

		foreach (DZBBC_SceneObjectConfig sceneObject: SceneObjects)
		{
			if (!sceneObject)
				continue;

			if (sceneObject.ClassName.IndexOf("Barrel") >= 0 || sceneObject.ClassName == DZBBC_WRECK_PROXY_CLASSNAME || sceneObject.ClassName == "OffroadHatchback" || sceneObject.ClassName == "CivilianSedan")
			{
				sceneObject.ClassName = "";
				sceneObject.Required = false;
				changed = true;
				continue;
			}

			if (sceneObject.ClassName == "StaticObj_Wreck_T72_Turret" || sceneObject.ClassName == "StaticObj_Wreck_T72_Turret_DE")
			{
				sceneObject.ClassName = "StaticObj_Wreck_T72_Chassis";
				sceneObject.Required = false;
				changed = true;
			}

			if (sceneObject.AlignToGround && sceneObject.ClassName.IndexOf("HMMWV") >= 0 && sceneObject.Offset && sceneObject.Offset.Count() >= 3 && sceneObject.Offset.Get(1) < 1.6)
			{
				sceneObject.Offset = DZBBC_Utils.MakeVectorArray(sceneObject.Offset.Get(0), 1.65, sceneObject.Offset.Get(2));
				changed = true;
			}

			if (sceneObject.AlignToGround && sceneObject.ClassName.IndexOf("T72_Chassis") >= 0 && sceneObject.Offset && sceneObject.Offset.Count() >= 3 && sceneObject.Offset.Get(1) < 2.0)
			{
				sceneObject.Offset = DZBBC_Utils.MakeVectorArray(sceneObject.Offset.Get(0), 2.05, sceneObject.Offset.Get(2));
				changed = true;
			}
		}

		return changed;
	}

	void AddSceneObject(string className, float x, float y, float z, float yaw, float pitch, float roll, bool required)
	{
		DZBBC_SceneObjectConfig objectConfig = new DZBBC_SceneObjectConfig;
		objectConfig.ClassName = className;
		objectConfig.Offset = DZBBC_Utils.MakeVectorArray(x, y, z);
		objectConfig.Orientation = DZBBC_Utils.MakeVectorArray(yaw, pitch, roll);
		objectConfig.AlignToGround = true;
		objectConfig.Required = required;
		SceneObjects.Insert(objectConfig);
	}

	void AddSceneObjectExact(string className, vector absolutePosition, vector orientation, bool required)
	{
		vector sitePosition = GetPosition();
		DZBBC_SceneObjectConfig objectConfig = new DZBBC_SceneObjectConfig;
		objectConfig.ClassName = className;
		objectConfig.Offset = DZBBC_Utils.MakeVectorArray(absolutePosition[0] - sitePosition[0], absolutePosition[1] - sitePosition[1], absolutePosition[2] - sitePosition[2]);
		objectConfig.Orientation = DZBBC_Utils.MakeVectorArray(orientation[0], orientation[1], orientation[2]);
		objectConfig.AlignToGround = false;
		objectConfig.Required = required;
		SceneObjects.Insert(objectConfig);
	}

	bool EnsureSceneObjectExact(string className, vector absolutePosition, vector orientation, bool required)
	{
		if (!SceneObjects)
			SceneObjects = new array<ref DZBBC_SceneObjectConfig>;

		vector expectedOffset = absolutePosition - GetPosition();
		foreach (DZBBC_SceneObjectConfig sceneObject: SceneObjects)
		{
			if (sceneObject && sceneObject.ClassName == className && vector.Distance(sceneObject.GetOffset(), expectedOffset) < 0.25)
				return false;
		}

		AddSceneObjectExact(className, absolutePosition, orientation, required);
		return true;
	}
}

class DZBBC_LocationsConfig
{
	ref array<ref DZBBC_CrashSiteConfig> CrashSites;

	void DZBBC_LocationsConfig()
	{
		CrashSites = new array<ref DZBBC_CrashSiteConfig>;
		CrashSites.Insert(new DZBBC_CrashSiteConfig);

		DZBBC_CrashSiteConfig vmcSite = new DZBBC_CrashSiteConfig;
		vmcSite.Name = "VMC Convoy Test";
		vmcSite.Position = DZBBC_Utils.MakeVectorArray(4490.0, 0.0, 8280.0);
		vmcSite.BlackboxPosition = DZBBC_Utils.MakeVectorArray(4500.0, 0.0, 8272.0);
		vmcSite.ContainerPosition = DZBBC_Utils.MakeVectorArray(4518.0, 0.0, 8292.0);
		vmcSite.Radius = 95.0;
		CrashSites.Insert(vmcSite);

		CrashSites.Insert(BuildTriKrestaSite());
		CrashSites.Insert(BuildTriKrestaWestSite());
		CrashSites.Insert(BuildKamenskWestSite());
		CrashSites.Insert(BuildKamenskEastSite());
		EnsureAirfieldBRDM();
	}

	bool EnsureRequiredLocations()
	{
		bool changed = false;
		if (!CrashSites)
		{
			CrashSites = new array<ref DZBBC_CrashSiteConfig>;
			changed = true;
		}

		changed = EnsureSite(BuildTriKrestaSite()) || changed;
		changed = EnsureSite(BuildTriKrestaWestSite()) || changed;
		changed = EnsureSite(BuildKamenskWestSite()) || changed;
		changed = EnsureSite(BuildKamenskEastSite()) || changed;
		changed = EnsureAirfieldBRDM() || changed;

		foreach (DZBBC_CrashSiteConfig site: CrashSites)
		{
			if (site)
				changed = site.EnsureDefaultScene() || changed;
		}

		return changed;
	}

	protected bool EnsureSite(DZBBC_CrashSiteConfig candidate)
	{
		if (!candidate || FindSite(candidate.Name))
			return false;

		CrashSites.Insert(candidate);
		return true;
	}

	protected DZBBC_CrashSiteConfig FindSite(string name)
	{
		foreach (DZBBC_CrashSiteConfig site: CrashSites)
		{
			if (site && site.Name == name)
				return site;
		}

		return null;
	}

	protected bool EnsureAirfieldBRDM()
	{
		DZBBC_CrashSiteConfig airfield = FindSite("Airfield Convoy Test");
		if (!airfield)
			return false;

		return airfield.EnsureSceneObjectExact("StaticObj_Wreck_BRDM", Vector(4446.887695, 338.927277, 10296.633789), Vector(36.443153, 0.0, 0.0), false);
	}

	protected DZBBC_CrashSiteConfig BuildTriKrestaSite()
	{
		DZBBC_CrashSiteConfig site = CreateEmptySite("Tri Kresta Convoy", Vector(450.677673, 293.246582, 9431.185547), Vector(294.277313, 295.107788, 9316.286133), Vector(-96.307762, 0.0, 0.0), Vector(468.677673, 293.246582, 9443.185547));

		site.AddSceneObjectExact("Land_Wreck_Uaz", Vector(438.763275, 291.023895, 9420.252930), Vector(-155.625824, 0.0, 0.0), true);
		site.AddSceneObjectExact("StaticObj_Wreck_Ural", Vector(450.677673, 293.246582, 9431.185547), Vector(152.133408, 0.0, 0.0), true);
		site.AddSceneObjectExact("StaticObj_Wreck_BMP2", Vector(473.891968, 294.778442, 9446.760742), Vector(36.443157, 0.0, 0.0), true);
		site.AddSceneObjectExact("StaticObj_Wreck_HMMWV", Vector(463.719116, 296.274017, 9451.163086), Vector(120.065880, 0.0, 0.0), true);
		site.AddSceneObjectExact("Land_Wreck_V3S", Vector(436.167877, 294.386902, 9451.475586), Vector(36.443153, 0.0, 0.0), true);
		site.AddSceneObjectExact("StaticObj_Wreck_BMP1", Vector(440.783661, 299.964600, 9489.972656), Vector(36.443153, 0.0, 0.0), true);
		site.AddSceneObjectExact("StaticObj_Wreck_HMMWV", Vector(477.080627, 294.821808, 9459.090820), Vector(36.443153, 0.0, 0.0), true);
		site.AddSceneObjectExact("RFFSHeli_CH53e_wreck", Vector(440.783661, 299.964600, 9489.972656), Vector(36.443153, 0.0, 0.0), false);
		AddCrashEffects(site);
		return site;
	}

	protected DZBBC_CrashSiteConfig BuildTriKrestaWestSite()
	{
		vector center = Vector(347.892303, 294.774872, 9359.527344);
		DZBBC_CrashSiteConfig site = CreateEmptySite("Tri Kresta West Crash", center, center + Vector(-8.0, 0.0, -7.0), Vector(-91.420685, 0.0, 0.0), center + Vector(17.0, 0.0, 12.0));
		site.AddSceneObject("Wreck_UH1Y", 0.0, 0.0, 0.0, -91.420685, 0.0, 0.0, true);
		site.AddSceneObject("StaticObj_Wreck_HMMWV", 15.0, 1.65, 11.0, 46.0, 0.0, 0.0, true);
		site.AddSceneObject("Land_Wreck_Uaz", -14.0, 0.0, -10.0, 138.0, 0.0, 0.0, true);
		site.AddSceneObject("StaticObj_Wreck_BMP1", 20.0, 1.35, -13.0, 12.0, 0.0, 0.0, false);
		AddCrashEffects(site);
		return site;
	}

	protected DZBBC_CrashSiteConfig BuildKamenskWestSite()
	{
		vector center = Vector(8483.358398, 223.918228, 14197.457031);
		DZBBC_CrashSiteConfig site = CreateEmptySite("Kamensk West Convoy", center, center + Vector(8.0, 0.0, -7.0), Vector(85.747742, 0.0, 0.0), center + Vector(20.0, 0.0, 12.0));
		site.AddSceneObject("Wreck_UH1Y", 0.0, 0.0, 0.0, 85.747742, 0.0, 0.0, true);
		site.AddSceneObject("StaticObj_Wreck_HMMWV", 16.0, 1.65, 10.0, 42.0, 0.0, 0.0, true);
		site.AddSceneObject("Land_Wreck_Uaz", -13.0, 0.0, -9.0, 128.0, 0.0, 0.0, true);
		site.AddSceneObject("StaticObj_Wreck_BRDM", -22.0, 1.15, 15.0, 205.0, 0.0, 0.0, false);
		AddCrashEffects(site);
		return site;
	}

	protected DZBBC_CrashSiteConfig BuildKamenskEastSite()
	{
		vector center = Vector(8403.467773, 232.038162, 14246.509766);
		DZBBC_CrashSiteConfig site = CreateEmptySite("Kamensk East Convoy", center, center + Vector(-9.0, 0.0, 8.0), Vector(-91.151955, 0.0, 0.0), center + Vector(18.0, 0.0, -13.0));
		site.AddSceneObject("Wreck_Mi8_Crashed", 0.0, 0.0, 0.0, -91.151955, 0.0, 0.0, true);
		site.AddSceneObject("StaticObj_Wreck_Ural", -17.0, 0.8, 9.0, 235.0, 0.0, 0.0, true);
		site.AddSceneObject("StaticObj_Wreck_BMP2", 19.0, 1.35, -12.0, 33.0, 0.0, 0.0, true);
		site.AddSceneObject("Land_Wreck_V3S", 24.0, 0.0, 14.0, 142.0, 0.0, 0.0, true);
		AddCrashEffects(site);
		return site;
	}

	protected DZBBC_CrashSiteConfig CreateEmptySite(string name, vector center, vector blackboxPosition, vector blackboxOrientation, vector containerPosition)
	{
		DZBBC_CrashSiteConfig site = new DZBBC_CrashSiteConfig;
		site.Name = name;
		site.Position = DZBBC_Utils.MakeVectorArray(center[0], center[1], center[2]);
		site.BlackboxPosition = DZBBC_Utils.MakeVectorArray(blackboxPosition[0], blackboxPosition[1], blackboxPosition[2]);
		site.BlackboxOrientation = DZBBC_Utils.MakeVectorArray(blackboxOrientation[0], blackboxOrientation[1], blackboxOrientation[2]);
		site.ContainerPosition = DZBBC_Utils.MakeVectorArray(containerPosition[0], containerPosition[1], containerPosition[2]);
		site.Radius = 120.0;
		site.WreckObjects.Clear();
		site.EffectObjects.Clear();
		site.SceneObjects.Clear();
		return site;
	}

	protected void AddCrashEffects(DZBBC_CrashSiteConfig site)
	{
		if (!site)
			return;

		site.AddSceneObject("M18SmokeGrenade_White", 5.0, 0.0, 5.0, 0.0, 0.0, 0.0, false);
		site.AddSceneObject("M18SmokeGrenade_White", -8.0, 0.0, -4.0, 0.0, 0.0, 0.0, false);
		site.AddSceneObject("Roadflare", 12.0, 0.0, -6.0, 120.0, 0.0, 0.0, false);
	}
}

class DZBBC_AIFactionConfig
{
	string FactionName;
	int Count;
	float HealthMultiplier;
	float DamageMultiplier;
	float AccuracyMin;
	float AccuracyMax;
	ref array<string> Types;
	ref array<string> Loadout;
	bool UseExpansionAI;
	int ExpansionCount;
	string ExpansionFaction;
	string ExpansionLoadoutFile;
	ref array<string> ExpansionTypes;
	ref array<string> ExpansionLoadout;

	void DZBBC_AIFactionConfig()
	{
		FactionName = "RussianZ";
		Count = 2;
		HealthMultiplier = 1.0;
		DamageMultiplier = 1.0;
		AccuracyMin = 0.10;
		AccuracyMax = 0.30;
		Types = new array<string>;
		Types.Insert("ZmbM_SoldierNormal");
		Types.Insert("ZmbM_usSoldier_normal_Woodland");
		Loadout = new array<string>;
		UseExpansionAI = true;
		ExpansionCount = 3;
		ExpansionFaction = "RussianZ";
		ExpansionLoadoutFile = "EastLoadout";
		ExpansionTypes = new array<string>;
		ExpansionTypes.Insert("eAI_SurvivorM_Mirek");
		ExpansionTypes.Insert("eAI_SurvivorM_Denis");
		ExpansionTypes.Insert("eAI_SurvivorF_Linda");
		ExpansionLoadout = new array<string>;
	}

	void EnsureDefaults(int zombieCount, int expansionCount, string expansionFaction)
	{
		if (!Types)
			Types = new array<string>;

		if (!Loadout)
			Loadout = new array<string>;

		if (!ExpansionTypes)
			ExpansionTypes = new array<string>;

		if (!ExpansionLoadout)
			ExpansionLoadout = new array<string>;

		if (Count < 0 || Count > zombieCount)
			Count = zombieCount;

		if (ExpansionCount <= 0)
			ExpansionCount = expansionCount;

		if (ExpansionFaction == "")
			ExpansionFaction = expansionFaction;

		if (AccuracyMin <= 0.0 || AccuracyMin > 1.0)
			AccuracyMin = 0.10;
		if (AccuracyMax < AccuracyMin || AccuracyMax > 1.0)
			AccuracyMax = 0.30;

		if (ExpansionLoadoutFile == "")
			ExpansionLoadoutFile = GetDefaultExpansionLoadoutFile(ExpansionFaction);

		UseExpansionAI = true;

		if (ExpansionTypes.Count() == 0)
		{
			ExpansionTypes.Insert("eAI_SurvivorM_Mirek");
			ExpansionTypes.Insert("eAI_SurvivorM_Denis");
			ExpansionTypes.Insert("eAI_SurvivorM_Boris");
			ExpansionTypes.Insert("eAI_SurvivorF_Linda");
			ExpansionTypes.Insert("eAI_SurvivorF_Maria");
		}
	}

	protected string GetDefaultExpansionLoadoutFile(string factionName)
	{
		if (factionName == "Russians" || factionName == "RussianZ" || factionName == "East" || factionName == "east")
			return "EastLoadout";

		if (factionName == "Americans" || factionName == "AmericanZ" || factionName == "West" || factionName == "west")
			return "WestLoadout";

		if (factionName == "Mercenaries" || factionName == "mercenaries")
			return "PoliceLoadout";

		if (factionName == "Guards" || factionName == "guards")
			return "TTSKOLoadout";

		return "BanditLoadout";
	}

	void ApplyConvoyDifficulty(float accuracyMin, float accuracyMax, bool addMorphine = false)
	{
		DamageMultiplier = 1.0;
		AccuracyMin = accuracyMin;
		AccuracyMax = accuracyMax;
		ExpansionLoadoutFile = "DZBBC_ConvoyWeakLoadout";
		if (!ExpansionLoadout)
			ExpansionLoadout = new array<string>;
		ExpansionLoadout.Clear();
		ExpansionLoadout.Insert("BandageDressing");
		if (addMorphine)
			ExpansionLoadout.Insert("Morphine");
	}
}

class DZBBC_AIWavesConfig
{
	ref DZBBC_AIFactionConfig RussianZ;
	ref DZBBC_AIFactionConfig AmericanZ;
	ref DZBBC_AIFactionConfig RecoveryUnit90;
	ref DZBBC_AIFactionConfig FinalDefense;

	void DZBBC_AIWavesConfig()
	{
		RussianZ = new DZBBC_AIFactionConfig;
		RussianZ.FactionName = "RussianZ";
		RussianZ.Count = 2;
		RussianZ.Types.Clear();
		RussianZ.Types.Insert("ZmbM_SoldierNormal");
		RussianZ.Types.Insert("ZmbM_PatrolNormal_Autumn");
		RussianZ.Loadout.Insert("BandageDressing");
		RussianZ.Loadout.Insert("Ammo_762x39");
		RussianZ.Loadout.Insert("TacticalBaconCan");
		RussianZ.ExpansionCount = 3;
		RussianZ.ExpansionFaction = "RussianZ";
		RussianZ.ExpansionLoadoutFile = "DZBBC_ConvoyWeakLoadout";
		RussianZ.AccuracyMin = 0.10;
		RussianZ.AccuracyMax = 0.28;
		RussianZ.ExpansionLoadout.Insert("BandageDressing");
		RussianZ.ExpansionLoadout.Insert("TacticalBaconCan");

		AmericanZ = new DZBBC_AIFactionConfig;
		AmericanZ.FactionName = "AmericanZ";
		AmericanZ.Count = 2;
		AmericanZ.Types.Clear();
		AmericanZ.Types.Insert("ZmbM_usSoldier_normal_Woodland");
		AmericanZ.Types.Insert("ZmbM_usSoldier_Heavy_Woodland");
		AmericanZ.Loadout.Insert("BandageDressing");
		AmericanZ.Loadout.Insert("Ammo_556x45");
		AmericanZ.Loadout.Insert("UnknownFoodCan");
		AmericanZ.ExpansionCount = 3;
		AmericanZ.ExpansionFaction = "AmericanZ";
		AmericanZ.ExpansionLoadoutFile = "DZBBC_ConvoyWeakLoadout";
		AmericanZ.AccuracyMin = 0.10;
		AmericanZ.AccuracyMax = 0.28;
		AmericanZ.ExpansionLoadout.Insert("BandageDressing");

		RecoveryUnit90 = new DZBBC_AIFactionConfig;
		RecoveryUnit90.FactionName = "Special recovery unit";
		RecoveryUnit90.Count = 1;
		RecoveryUnit90.HealthMultiplier = 1.5;
		RecoveryUnit90.DamageMultiplier = 1.0;
		RecoveryUnit90.AccuracyMin = 0.14;
		RecoveryUnit90.AccuracyMax = 0.34;
		RecoveryUnit90.Types.Clear();
		RecoveryUnit90.Types.Insert("ZmbM_PolicemanSpecForce_Heavy");
		RecoveryUnit90.Loadout.Insert("BandageDressing");
		RecoveryUnit90.Loadout.Insert("Morphine");
		RecoveryUnit90.Loadout.Insert("Ammo_12gaPellets");
		RecoveryUnit90.ExpansionCount = 2;
		RecoveryUnit90.ExpansionFaction = "Mercenaries";
		RecoveryUnit90.ExpansionLoadoutFile = "DZBBC_ConvoyWeakLoadout";
		RecoveryUnit90.ExpansionLoadout.Insert("BandageDressing");
		RecoveryUnit90.ExpansionLoadout.Insert("Morphine");

		FinalDefense = new DZBBC_AIFactionConfig;
		FinalDefense.FactionName = "Final defense wave";
		FinalDefense.Count = 2;
		FinalDefense.HealthMultiplier = 2.0;
		FinalDefense.DamageMultiplier = 1.0;
		FinalDefense.AccuracyMin = 0.18;
		FinalDefense.AccuracyMax = 0.40;
		FinalDefense.Types.Clear();
		FinalDefense.Types.Insert("ZmbM_PolicemanSpecForce_Heavy");
		FinalDefense.Types.Insert("ZmbM_usSoldier_Heavy_Woodland");
		FinalDefense.Loadout.Insert("BandageDressing");
		FinalDefense.Loadout.Insert("Epinephrine");
		FinalDefense.Loadout.Insert("Ammo_308Win");
		FinalDefense.ExpansionCount = 4;
		FinalDefense.ExpansionFaction = "Raiders";
		FinalDefense.ExpansionLoadoutFile = "DZBBC_ConvoyWeakLoadout";
		FinalDefense.ExpansionLoadout.Insert("BandageDressing");
		FinalDefense.ExpansionLoadout.Insert("Morphine");
	}

	void Normalize()
	{
		if (RussianZ)
		{
			if (RussianZ.FactionName == "Russians" || RussianZ.FactionName == "East" || RussianZ.FactionName == "east")
				RussianZ.FactionName = "RussianZ";
			if (RussianZ.ExpansionFaction == "Russians" || RussianZ.ExpansionFaction == "East" || RussianZ.ExpansionFaction == "east")
				RussianZ.ExpansionFaction = "RussianZ";
			RussianZ.EnsureDefaults(2, 3, "RussianZ");
			RussianZ.ApplyConvoyDifficulty(0.03, 0.12);
		}

		if (AmericanZ)
		{
			if (AmericanZ.FactionName == "Americans" || AmericanZ.FactionName == "West" || AmericanZ.FactionName == "west")
				AmericanZ.FactionName = "AmericanZ";
			if (AmericanZ.ExpansionFaction == "Americans" || AmericanZ.ExpansionFaction == "West" || AmericanZ.ExpansionFaction == "west")
				AmericanZ.ExpansionFaction = "AmericanZ";
			AmericanZ.EnsureDefaults(2, 3, "AmericanZ");
			AmericanZ.ApplyConvoyDifficulty(0.03, 0.12);
		}

		if (RecoveryUnit90)
		{
			RecoveryUnit90.EnsureDefaults(1, 2, "Mercenaries");
			RecoveryUnit90.ApplyConvoyDifficulty(0.04, 0.14, true);
		}

		if (FinalDefense)
		{
			FinalDefense.EnsureDefaults(2, 4, "Raiders");
			FinalDefense.ApplyConvoyDifficulty(0.05, 0.16, true);
		}
	}
}

class DZBBC_LootEntry
{
	string ClassName;
	int Min;
	int Max;
	float Chance;

	void DZBBC_LootEntry()
	{
		ClassName = "";
		Min = 1;
		Max = 1;
		Chance = 100.0;
	}
}

class DZBBC_LootConfig
{
	ref array<ref DZBBC_LootEntry> RewardContainerLoot;

	void DZBBC_LootConfig()
	{
		RewardContainerLoot = new array<ref DZBBC_LootEntry>;
		AddGuaranteed(DZBBC_GASZONE_FLARE_CLASSNAME);
		AddChance(DZBBC_SIGNAL_MATCHES_CLASSNAME, 75.0, 1, 1);
		AddChance(DZBBC_CODED_9V_CLASSNAME, 50.0, 1, 1);
		AddChance("M4A1", 25.0, 1, 1);
		AddChance("Ammo_556x45", 75.0, 2, 6);
		AddChance("PlateCarrierVest", 35.0, 1, 1);
		AddChance("NVGoggles", 25.0, 1, 1);
	}

	void AddGuaranteed(string className)
	{
		AddChance(className, 100.0, 1, 1);
	}

	void AddChance(string className, float chance, int min, int max)
	{
		DZBBC_LootEntry entry = new DZBBC_LootEntry;
		entry.ClassName = className;
		entry.Chance = chance;
		entry.Min = min;
		entry.Max = max;
		RewardContainerLoot.Insert(entry);
	}

	bool EnsureStoryLootDefaults()
	{
		bool changed = false;
		if (!HasClassName(DZBBC_SIGNAL_MATCHES_CLASSNAME))
		{
			AddChance(DZBBC_SIGNAL_MATCHES_CLASSNAME, 75.0, 1, 1);
			changed = true;
		}
		if (!HasClassName(DZBBC_CODED_9V_CLASSNAME))
		{
			AddChance(DZBBC_CODED_9V_CLASSNAME, 50.0, 1, 1);
			changed = true;
		}
		return changed;
	}

	protected bool HasClassName(string className)
	{
		if (!RewardContainerLoot)
			return false;

		foreach (DZBBC_LootEntry entry: RewardContainerLoot)
		{
			if (entry && entry.ClassName == className)
				return true;
		}
		return false;
	}
}

class DZBBC_TerminalLocationConfig
{
	string Name;
	ref array<float> Position;

	void DZBBC_TerminalLocationConfig()
	{
		Name = "Airfield Decrypt Terminal";
		Position = DZBBC_Utils.MakeVectorArray(4565.0, 0.0, 10330.0);
	}

	vector GetPosition()
	{
		return DZBBC_Utils.ArrayToVector(Position);
	}
}

class DZBBC_TerminalsConfig
{
	ref array<ref DZBBC_TerminalLocationConfig> Terminals;

	void DZBBC_TerminalsConfig()
	{
		Terminals = new array<ref DZBBC_TerminalLocationConfig>;
		Terminals.Insert(new DZBBC_TerminalLocationConfig);

		DZBBC_TerminalLocationConfig vmcTerminal = new DZBBC_TerminalLocationConfig;
		vmcTerminal.Name = "VMC Decrypt Terminal";
		vmcTerminal.Position = DZBBC_Utils.MakeVectorArray(4540.0, 0.0, 8320.0);
		Terminals.Insert(vmcTerminal);
	}

	bool NormalizeLiveTerminals()
	{
		bool changed = false;
		if (!Terminals)
		{
			Terminals = new array<ref DZBBC_TerminalLocationConfig>;
			changed = true;
		}

		for (int i = Terminals.Count() - 1; i >= 0; i--)
		{
			DZBBC_TerminalLocationConfig terminal = Terminals.Get(i);
			if (!terminal)
			{
				Terminals.Remove(i);
				changed = true;
				continue;
			}

			string nameLower = terminal.Name;
			nameLower.ToLower();
			vector pos = terminal.GetPosition();
			bool nearGreenMountain = vector.Distance(pos, "3700 0 5980") < 900.0;
			if (nameLower.IndexOf("green") >= 0 || nameLower.IndexOf("mountain") >= 0 || nearGreenMountain)
			{
				Terminals.Remove(i);
				changed = true;
			}
		}

		if (!HasTerminalNamed("Airfield Decrypt Terminal"))
		{
			DZBBC_TerminalLocationConfig airfield = new DZBBC_TerminalLocationConfig;
			airfield.Name = "Airfield Decrypt Terminal";
			airfield.Position = DZBBC_Utils.MakeVectorArray(4565.0, 0.0, 10330.0);
			Terminals.Insert(airfield);
			changed = true;
		}

		if (!HasTerminalNamed("VMC Decrypt Terminal"))
		{
			DZBBC_TerminalLocationConfig vmc = new DZBBC_TerminalLocationConfig;
			vmc.Name = "VMC Decrypt Terminal";
			vmc.Position = DZBBC_Utils.MakeVectorArray(4540.0, 0.0, 8320.0);
			Terminals.Insert(vmc);
			changed = true;
		}

		return changed;
	}

	protected bool HasTerminalNamed(string name)
	{
		foreach (DZBBC_TerminalLocationConfig terminal: Terminals)
		{
			if (terminal && terminal.Name == name)
				return true;
		}

		return false;
	}
}

class DZBBC_MessagesConfig
{
	string EventAnnounce;
	string PlayerDetected;
	string CombatComplete;
	string AudioFragment30;
	string GlobalWarning60;
	string RecoveryUnit90;
	string DatacoreExtracted;
	string TerminalStarted;
	string RewardUnlocked;
	string Completed;
	string Failed;

	void DZBBC_MessagesConfig()
	{
		EventAnnounce = "#STR_DZBBC_MSG_EVENT_ANNOUNCE";
		PlayerDetected = "#STR_DZBBC_MSG_PLAYER_DETECTED";
		CombatComplete = "#STR_DZBBC_MSG_COMBAT_COMPLETE";
		AudioFragment30 = "#STR_DZBBC_MSG_AUDIO_30";
		GlobalWarning60 = "#STR_DZBBC_MSG_GLOBAL_WARNING";
		RecoveryUnit90 = "#STR_DZBBC_MSG_RECOVERY_90";
		DatacoreExtracted = "#STR_DZBBC_MSG_DATACORE_EXTRACTED";
		TerminalStarted = "#STR_DZBBC_MSG_TERMINAL_STARTED";
		RewardUnlocked = "#STR_DZBBC_MSG_REWARD_UNLOCKED";
		Completed = "#STR_DZBBC_MSG_COMPLETED";
		Failed = "#STR_DZBBC_MSG_FAILED";
	}
}

class DZBBC_ConfigBundle
{
	ref DZBBC_MainConfig Main;
	ref DZBBC_LocationsConfig Locations;
	ref DZBBC_AIWavesConfig AIWaves;
	ref DZBBC_LootConfig Loot;
	ref DZBBC_TerminalsConfig Terminals;
	ref DZBBC_MessagesConfig Messages;

	void DZBBC_ConfigBundle()
	{
		Main = new DZBBC_MainConfig;
		Locations = new DZBBC_LocationsConfig;
		AIWaves = new DZBBC_AIWavesConfig;
		Loot = new DZBBC_LootConfig;
		Terminals = new DZBBC_TerminalsConfig;
		Messages = new DZBBC_MessagesConfig;
	}
}
