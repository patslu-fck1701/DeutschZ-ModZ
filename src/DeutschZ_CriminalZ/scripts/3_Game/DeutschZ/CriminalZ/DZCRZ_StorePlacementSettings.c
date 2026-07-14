class DZCRZ_StorePlacementEntry
{
	string Type;
	ref array<float> LocalPosition;
	ref array<float> LocalOrientation;
	float Scale = 1.0;

	void DZCRZ_StorePlacementEntry()
	{
		LocalPosition = new array<float>;
		LocalOrientation = new array<float>;
	}

	vector GetLocalPosition()
	{
		if (!LocalPosition || LocalPosition.Count() < 3)
			return vector.Zero;
		return Vector(LocalPosition[0], LocalPosition[1], LocalPosition[2]);
	}

	vector GetLocalOrientation()
	{
		if (!LocalOrientation || LocalOrientation.Count() < 3)
			return vector.Zero;
		return Vector(LocalOrientation[0], LocalOrientation[1], LocalOrientation[2]);
	}

	void SetLocalPosition(vector value)
	{
		LocalPosition.Clear();
		LocalPosition.Insert(value[0]);
		LocalPosition.Insert(value[1]);
		LocalPosition.Insert(value[2]);
	}

	void SetLocalOrientation(vector value)
	{
		LocalOrientation.Clear();
		LocalOrientation.Insert(value[0]);
		LocalOrientation.Insert(value[1]);
		LocalOrientation.Insert(value[2]);
	}

	DZCRZ_StorePlacementEntry Clone()
	{
		DZCRZ_StorePlacementEntry result = new DZCRZ_StorePlacementEntry;
		result.Type = Type;
		result.SetLocalPosition(GetLocalPosition());
		result.SetLocalOrientation(GetLocalOrientation());
		result.Scale = Scale;
		return result;
	}
}

class DZCRZ_StorePlacementProfile
{
	string StoreType;
	ref array<ref DZCRZ_StorePlacementEntry> Placements;

	void DZCRZ_StorePlacementProfile()
	{
		Placements = new array<ref DZCRZ_StorePlacementEntry>;
	}
}

class DZCRZ_StorePlacementSettings
{
	bool Enabled = true;
	ref array<string> StoreTypes;
	ref array<ref DZCRZ_StorePlacementEntry> Placements;
	ref array<ref DZCRZ_StorePlacementProfile> StoreProfiles;
	float DuplicateRadius = 0.75;
	int SpawnDelayMilliseconds = 2000;
	bool EnableDebugLogging = false;
	string CustomATMClassname = "DZCRZ_ATM";
	string CustomSignClassname = "DZCRZ_StoreInfoSign";

	void DZCRZ_StorePlacementSettings()
	{
		StoreTypes = new array<string>;
		Placements = new array<ref DZCRZ_StorePlacementEntry>;
		StoreProfiles = new array<ref DZCRZ_StorePlacementProfile>;
		SetDefaults();
	}

	void SetDefaults()
	{
		StoreTypes.Clear();
		StoreTypes.Insert("Land_City_Store");
		StoreTypes.Insert("Land_City_Store_WithStairs");
		Placements.Clear();
		DZCRZ_StorePlacementEntry atm = new DZCRZ_StorePlacementEntry;
		atm.Type = CustomATMClassname;
		atm.SetLocalPosition("-8.898676 -1.205345 -3.431599");
		atm.SetLocalOrientation("-90.035360 0 0");
		Placements.Insert(atm);
		DZCRZ_StorePlacementEntry sign = new DZCRZ_StorePlacementEntry;
		sign.Type = CustomSignClassname;
		sign.SetLocalPosition("0.725949 1.643461 10.425142");
		sign.SetLocalOrientation("-174.602692 0 0");
		Placements.Insert(sign);
		BuildDefaultProfiles();
	}

	void BuildDefaultProfiles()
	{
		StoreProfiles.Clear();
		foreach (string storeType: StoreTypes)
		{
			DZCRZ_StorePlacementProfile profile = new DZCRZ_StorePlacementProfile;
			profile.StoreType = storeType;
			foreach (DZCRZ_StorePlacementEntry placement: Placements)
				profile.Placements.Insert(placement.Clone());
			StoreProfiles.Insert(profile);
		}
	}

	bool Validate()
	{
		bool valid = Enabled && StoreTypes && StoreTypes.Count() == 2 && Placements && Placements.Count() == 2;
		if (!valid)
		{
			Enabled = true;
			SetDefaults();
		}
		DuplicateRadius = Math.Clamp(DuplicateRadius, 0.1, 5.0);
		SpawnDelayMilliseconds = Math.Clamp(SpawnDelayMilliseconds, 250, 30000);
		if (!StoreProfiles || StoreProfiles.Count() == 0)
			BuildDefaultProfiles();
		UpgradeLegacyPlacements();
		return valid;
	}

	protected void UpgradeLegacyPlacements()
	{
		if (CustomATMClassname == "")
			CustomATMClassname = "DZCRZ_ATM";
		if (CustomSignClassname == "")
			CustomSignClassname = "DZCRZ_StoreInfoSign";
		foreach (DZCRZ_StorePlacementEntry placement: Placements)
		{
			if (!placement)
				continue;
			if (placement.Type == "ExpansionATM_2")
				placement.Type = CustomATMClassname;
			else if (placement.Type == "StaticObj_FueldStation_Sign")
				placement.Type = CustomSignClassname;
		}
		foreach (DZCRZ_StorePlacementProfile profile: StoreProfiles)
		{
			if (!profile || !profile.Placements)
				continue;
			foreach (DZCRZ_StorePlacementEntry profilePlacement: profile.Placements)
			{
				if (!profilePlacement)
					continue;
				if (profilePlacement.Type == "ExpansionATM_2")
					profilePlacement.Type = CustomATMClassname;
				else if (profilePlacement.Type == "StaticObj_FueldStation_Sign")
					profilePlacement.Type = CustomSignClassname;
			}
		}
	}

	array<ref DZCRZ_StorePlacementEntry> GetPlacementsFor(string storeType)
	{
		foreach (DZCRZ_StorePlacementProfile profile: StoreProfiles)
		{
			if (profile && profile.StoreType == storeType && profile.Placements && profile.Placements.Count() > 0)
				return profile.Placements;
		}
		return Placements;
	}

	static DZCRZ_StorePlacementSettings Load()
	{
		DZCRZ_ProfilePaths.Ensure();
		DZCRZ_StorePlacementSettings settings = new DZCRZ_StorePlacementSettings;
		string errorMessage;
		bool loaded;
		if (FileExist(DZCRZ_ProfilePaths.STORE_PLACEMENT_CONFIG))
			loaded = JsonFileLoader<ref DZCRZ_StorePlacementSettings>.LoadFile(DZCRZ_ProfilePaths.STORE_PLACEMENT_CONFIG, settings, errorMessage);
		if (!loaded)
		{
			if (errorMessage != "")
				DZCRZ_Log.Error("StorePlacement-JSON ungueltig, sichere Defaults werden verwendet: " + errorMessage);
			settings = new DZCRZ_StorePlacementSettings;
		}
		if (!settings.Validate())
			DZCRZ_Log.Warn("StorePlacement-Konfiguration war unvollstaendig und wurde repariert.");
		if (!JsonFileLoader<ref DZCRZ_StorePlacementSettings>.SaveFile(DZCRZ_ProfilePaths.STORE_PLACEMENT_CONFIG, settings, errorMessage))
			DZCRZ_Log.Error("StorePlacement-Konfiguration konnte nicht gespeichert werden: " + errorMessage);
		return settings;
	}
}
