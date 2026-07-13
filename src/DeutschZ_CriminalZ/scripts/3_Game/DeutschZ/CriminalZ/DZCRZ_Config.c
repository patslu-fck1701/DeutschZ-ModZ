class DZCRZ_GeneralConfig
{
	bool UseDeutschZProfileSystem = true;
	bool EnableGlobalNotifications = true;
	bool EnablePersonalNotifications = true;
	bool EnableExpansionMarkers = true;
	bool PreventParallelHackOnSameTarget = true;
	bool PersistCooldowns = true;
	bool AllowAdminWhenListEmpty = false;
	ref array<string> AdminUIDs;

	void DZCRZ_GeneralConfig()
	{
		AdminUIDs = new array<string>;
	}
}

class DZCRZ_ModuleConfig
{
	bool Enabled = true;
	int HackTimeMinSeconds = 300;
	int HackTimeMaxSeconds = 600;
	float RequiredRadiusMeters = 15.0;
	bool EnableAlarm = true;
	bool EnableGlobalNotification = true;
	bool EnableMapMarker = true;
	string MarkerColor = "Red";
	bool MarkerBlinking = true;
	string RequiredTool = "HackingTool_Lockpick";
	bool RequireToolInHands = true;
	bool DamageToolOnSuccess = true;
	float ToolDamageOnSuccess = 10.0;
	bool DamageToolOnFail = true;
	float ToolDamageOnFail = 5.0;

	void Validate()
	{
		HackTimeMinSeconds = Math.Clamp(HackTimeMinSeconds, 10, 3600);
		HackTimeMaxSeconds = Math.Clamp(HackTimeMaxSeconds, HackTimeMinSeconds, 3600);
		RequiredRadiusMeters = Math.Clamp(RequiredRadiusMeters, 2.0, 50.0);
		ToolDamageOnSuccess = Math.Clamp(ToolDamageOnSuccess, 0.0, 100.0);
		ToolDamageOnFail = Math.Clamp(ToolDamageOnFail, 0.0, 100.0);
		MarkerColor = "Red";
	}
}

class DZCRZ_ATMConfig: DZCRZ_ModuleConfig
{
	bool AutoCreateAtExpansionStores = true;
	bool FallbackWriteStoresToConfig = true;
	int CooldownSeconds = 7200;
	bool EnableSiren = true;
	string PayoutCurrencyMode = "NoxZ_Euro";
	int PayoutMinAmount = 45000;
	int PayoutMaxAmount = 185000;
	bool AllowBitcoinReward = false;

	void DZCRZ_ATMConfig()
	{
		RequiredTool = DZCRZ_Const.ATM_TOOL_CLASS;
	}

	override void Validate()
	{
		super.Validate();
		CooldownSeconds = Math.Clamp(CooldownSeconds, 60, 86400);
		PayoutMinAmount = Math.Clamp(PayoutMinAmount, 1, 1000000);
		PayoutMaxAmount = Math.Clamp(PayoutMaxAmount, PayoutMinAmount, 1000000);
		PayoutCurrencyMode = "NoxZ_Euro";
		AllowBitcoinReward = false;
		RequiredTool = DZCRZ_Const.ATM_TOOL_CLASS;
	}
}

class DZCRZ_GarageConfig: DZCRZ_ModuleConfig
{
	void DZCRZ_GarageConfig()
	{
		RequiredTool = DZCRZ_Const.HACK_TOOL_CLASS;
	}

	override void Validate()
	{
		super.Validate();
		RequiredTool = DZCRZ_Const.HACK_TOOL_CLASS;
	}
}

class DZCRZ_VehicleConfig: DZCRZ_ModuleConfig
{
	bool EnableHorn = true;
	bool EnableHazardLights = true;

	void DZCRZ_VehicleConfig()
	{
		RequiredTool = DZCRZ_Const.HACK_TOOL_CLASS;
	}

	override void Validate()
	{
		super.Validate();
		RequiredTool = DZCRZ_Const.HACK_TOOL_CLASS;
	}
}

class DZCRZ_Config
{
	int Version = 1;
	bool DebugMode = true;
	ref DZCRZ_GeneralConfig General;
	ref DZCRZ_ATMConfig ATMModule;
	ref DZCRZ_GarageConfig GarageModule;
	ref DZCRZ_VehicleConfig VehicleModule;
	ref array<ref DZCRZ_Denomination> NoxZEuroDenominations;
	ref array<ref DZCRZ_ATMPosition> ATMPositions;

	void DZCRZ_Config()
	{
		General = new DZCRZ_GeneralConfig;
		ATMModule = new DZCRZ_ATMConfig;
		GarageModule = new DZCRZ_GarageConfig;
		VehicleModule = new DZCRZ_VehicleConfig;
		NoxZEuroDenominations = new array<ref DZCRZ_Denomination>;
		ATMPositions = new array<ref DZCRZ_ATMPosition>;
		SetDefaultDenominations();
	}

	void Validate()
	{
		if (!General)
			General = new DZCRZ_GeneralConfig;
		if (!ATMModule)
			ATMModule = new DZCRZ_ATMConfig;
		if (!GarageModule)
			GarageModule = new DZCRZ_GarageConfig;
		if (!VehicleModule)
			VehicleModule = new DZCRZ_VehicleConfig;
		ATMModule.Validate();
		GarageModule.Validate();
		VehicleModule.Validate();
		ValidateDenominations();
		DZCRZ_RuntimeFlags.DebugEnabled = DebugMode;
	}

	protected void ValidateDenominations()
	{
		if (!NoxZEuroDenominations)
			NoxZEuroDenominations = new array<ref DZCRZ_Denomination>;
		for (int index = NoxZEuroDenominations.Count() - 1; index >= 0; index--)
		{
			DZCRZ_Denomination denomination = NoxZEuroDenominations[index];
			if (!denomination || denomination.Value <= 0 || denomination.ClassName.IndexOf("NoxZ_Euro") != 0 || denomination.ClassName.IndexOf("Bitcoin") >= 0)
				NoxZEuroDenominations.Remove(index);
		}
		if (NoxZEuroDenominations.Count() == 0)
			SetDefaultDenominations();
	}

	protected void SetDefaultDenominations()
	{
		NoxZEuroDenominations.Clear();
		NoxZEuroDenominations.Insert(new DZCRZ_Denomination("NoxZ_Euro500", 500));
		NoxZEuroDenominations.Insert(new DZCRZ_Denomination("NoxZ_Euro200", 200));
		NoxZEuroDenominations.Insert(new DZCRZ_Denomination("NoxZ_Euro100", 100));
		NoxZEuroDenominations.Insert(new DZCRZ_Denomination("NoxZ_Euro50", 50));
		NoxZEuroDenominations.Insert(new DZCRZ_Denomination("NoxZ_Euro20", 20));
		NoxZEuroDenominations.Insert(new DZCRZ_Denomination("NoxZ_Euro10", 10));
		NoxZEuroDenominations.Insert(new DZCRZ_Denomination("NoxZ_Euro5", 5));
		NoxZEuroDenominations.Insert(new DZCRZ_Denomination("NoxZ_Euro2", 2));
		NoxZEuroDenominations.Insert(new DZCRZ_Denomination("NoxZ_Euro1", 1));
	}

	static DZCRZ_Config Load()
	{
		DZCRZ_ProfilePaths.Ensure();
		DZCRZ_Config config = new DZCRZ_Config;
		string errorMessage;
		if (FileExist(DZCRZ_ProfilePaths.MAIN_CONFIG))
		{
			if (!JsonFileLoader<ref DZCRZ_Config>.LoadFile(DZCRZ_ProfilePaths.MAIN_CONFIG, config, errorMessage))
				DZCRZ_Log.Error("Config konnte nicht geladen werden: " + errorMessage);
		}
		config.Validate();
		if (!JsonFileLoader<ref DZCRZ_Config>.SaveFile(DZCRZ_ProfilePaths.MAIN_CONFIG, config, errorMessage))
			DZCRZ_Log.Error("Config konnte nicht gespeichert werden: " + errorMessage);
		return config;
	}
}
