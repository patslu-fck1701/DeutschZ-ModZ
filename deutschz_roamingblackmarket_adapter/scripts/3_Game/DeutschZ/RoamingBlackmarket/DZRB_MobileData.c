enum DZRB_MobileState
{
	DZRB_INACTIVE = 0,
	DZRB_SPAWNING,
	DZRB_TRAVEL,
	DZRB_APPROACHING_STOP,
	DZRB_PARKING,
	DZRB_OPEN,
	DZRB_CLOSING,
	DZRB_DEPARTING,
	DZRB_STUCK_RECOVERY,
	DZRB_ROUTE_FINISHED,
	DZRB_CLEANUP
}

class DZRB_TradeStop
{
	string StopID;
	int WaypointIndex;
	vector ParkingPosition;
	vector ParkingOrientation;
	vector TraderPosition;
	int StopDurationSeconds;
	string Notification;

	void DZRB_TradeStop()
	{
		StopID = "";
		WaypointIndex = -1;
		StopDurationSeconds = 900;
		Notification = "Der Schwarzmarkt hat geoeffnet.";
	}
}

class DZRB_Route
{
	int Version;
	string RouteID;
	bool Enabled;
	bool Loop;
	ref array<vector> Waypoints;
	ref array<ref DZRB_TradeStop> TradeStops;

	void DZRB_Route()
	{
		Version = 1;
		RouteID = "";
		Enabled = true;
		Loop = true;
		Waypoints = new array<vector>();
		TradeStops = new array<ref DZRB_TradeStop>();
	}
}

class DZRB_MobileSettings
{
	static const string ROOT_DIR = "$profile:DeutschZ-System/RoamingBlackMarket";
	static const string ROUTES_DIR = "$profile:DeutschZ-System/RoamingBlackMarket/Routes";
	static const string SETTINGS_PATH = "$profile:DeutschZ-System/RoamingBlackMarket/settings.json";
	static const string STATE_PATH = "$profile:DeutschZ-System/RoamingBlackMarket/State.json";

	int Version;
	bool Enabled;
	string PreferredRouteID;
	string VehicleClassName;
	ref array<string> VehicleAttachments;
	float CruiseSpeedKmh;
	float ApproachSpeedKmh;
	float WaypointReachedMeters;
	float ApproachDistanceMeters;
	float StuckSpeedKmh;
	int StuckSeconds;
	int RouteTickMilliseconds;
	int StateSaveSeconds;
	int MarkerUpdateSeconds;
	string MarkerMode;
	bool VehicleInvulnerable;
	bool PreventCargoAccess;
	bool PreventPartRemoval;
	bool PreventVehicleEntry;
	bool ResumeAtLastSafeWaypoint;
	bool RandomStartAtTradeStop;
	int RandomStopsPerLoop;
	int MinStopWaypointDistance;
	string TraderClassName;
	ref array<string> TraderOutfit;
	ref array<string> TraderFiles;
	string TraderZoneFile;
	float TraderZoneRadius;
	int MinRequiredReputation;
	int MaxRequiredReputation;

	void DZRB_MobileSettings()
	{
		Version = 8;
		Enabled = true;
		PreferredRouteID = "ChernarusPlus_V9_15m_LOOP_XZ";
		VehicleClassName = "Truck_01_Covered_YellowBright";
		VehicleAttachments = new array<string>();
		SetTruckAttachments();
		CruiseSpeedKmh = 55.0;
		ApproachSpeedKmh = 18.0;
		WaypointReachedMeters = 7.0;
		ApproachDistanceMeters = 80.0;
		StuckSpeedKmh = 1.5;
		StuckSeconds = 12;
		RouteTickMilliseconds = 250;
		StateSaveSeconds = 10;
		MarkerUpdateSeconds = 10;
		MarkerMode = "LIVE";
		VehicleInvulnerable = true;
		PreventCargoAccess = true;
		PreventPartRemoval = true;
		PreventVehicleEntry = true;
		ResumeAtLastSafeWaypoint = true;
		RandomStartAtTradeStop = true;
		RandomStopsPerLoop = 4;
		MinStopWaypointDistance = 120;
		TraderClassName = "ExpansionTraderLinda";
		TraderOutfit = new array<string>();
		TraderOutfit.Insert("BoonieHat_Black");
		TraderOutfit.Insert("BalaclavaMask_Black");
		TraderOutfit.Insert("TacticalShirt_Black");
		TraderOutfit.Insert("PlateCarrierVest");
		TraderOutfit.Insert("CargoPants_Black");
		TraderOutfit.Insert("TacticalGloves_Black");
		TraderOutfit.Insert("CombatBoots_Black");
		TraderFiles = new array<string>();
		TraderFiles.Insert("Blackmarket_Weapons");
		TraderFiles.Insert("Blackmarket_Armor");
		TraderFiles.Insert("Blackmarket_Exchange");
		TraderZoneFile = "Blackmarket";
		TraderZoneRadius = 35.0;
		MinRequiredReputation = 0;
		MaxRequiredReputation = 2147483647;
	}

	private void SetTruckAttachments()
	{
		VehicleAttachments.Clear();
		for (int singleWheel = 0; singleWheel < 2; singleWheel++) VehicleAttachments.Insert("Truck_01_Wheel");
		for (int doubleWheel = 0; doubleWheel < 4; doubleWheel++) VehicleAttachments.Insert("Truck_01_WheelDouble");
		VehicleAttachments.Insert("TruckBattery");
		VehicleAttachments.Insert("HeadlightH7");
		VehicleAttachments.Insert("HeadlightH7");
		VehicleAttachments.Insert("Truck_01_Hood");
		VehicleAttachments.Insert("Truck_01_Door_1_1");
		VehicleAttachments.Insert("Truck_01_Door_2_1");
	}

	private void Migrate()
	{
		if (Version < 3)
		{
			VehicleClassName = "Truck_01_Covered_YellowBright";
			VehicleAttachments = new array<string>();
			SetTruckAttachments();
			PreventVehicleEntry = true;
			TraderOutfit = new array<string>();
			TraderOutfit.Insert("BoonieHat_Black");
			TraderOutfit.Insert("BalaclavaMask_Black");
			TraderOutfit.Insert("TacticalShirt_Black");
			TraderOutfit.Insert("PlateCarrierVest");
			TraderOutfit.Insert("CargoPants_Black");
			TraderOutfit.Insert("TacticalGloves_Black");
			TraderOutfit.Insert("CombatBoots_Black");
			MinRequiredReputation = 0;
			MaxRequiredReputation = 2147483647;
		}
		if (Version < 4)
		{
			if (CruiseSpeedKmh == 28.0) CruiseSpeedKmh = 40.0;
			if (ApproachSpeedKmh == 9.0) ApproachSpeedKmh = 14.0;
		}
		if (Version < 5)
		{
			RandomStopsPerLoop = 4;
			MinStopWaypointDistance = 120;
		}
		if (Version < 6)
		{
			PreferredRouteID = "ChernarusPlus_V9_15m_LOOP_XZ";
		}
		if (Version < 7)
		{
			if (MarkerMode == "STOP_ONLY" || MarkerMode == "") MarkerMode = "LIVE";
		}
		if (Version < 8)
		{
			if (CruiseSpeedKmh == 40.0) CruiseSpeedKmh = 55.0;
			if (ApproachSpeedKmh == 14.0) ApproachSpeedKmh = 18.0;
		}
		Version = 8;
	}

	static DZRB_MobileSettings Load()
	{
		if (!FileExist("$profile:DeutschZ-System")) MakeDirectory("$profile:DeutschZ-System");
		if (!FileExist(ROOT_DIR)) MakeDirectory(ROOT_DIR);
		if (!FileExist(ROUTES_DIR)) MakeDirectory(ROUTES_DIR);

		DZRB_MobileSettings settings = new DZRB_MobileSettings();
		string error;
		if (FileExist(SETTINGS_PATH))
		{
			if (!JsonFileLoader<DZRB_MobileSettings>.LoadFile(SETTINGS_PATH, settings, error))
				Print("[DeutschZ_RBM] SETTINGS_ERROR " + error);
			else
			{
				settings.Migrate();
				error = "";
				JsonFileLoader<DZRB_MobileSettings>.SaveFile(SETTINGS_PATH, settings, error);
				if (error != "") Print("[DeutschZ_RBM] SETTINGS_MIGRATION_ERROR " + error);
			}
		}
		else
		{
			JsonFileLoader<DZRB_MobileSettings>.SaveFile(SETTINGS_PATH, settings, error);
			if (error != "") Print("[DeutschZ_RBM] SETTINGS_CREATE_ERROR " + error);
		}
		return settings;
	}
}

class DZRB_RuntimeState
{
	int Version;
	string RouteID;
	int WaypointIndex;
	int CurrentState;
	string CurrentStopID;
	string LastCompletedStop;
	vector LastPosition;
	vector LastValidPosition;

	void DZRB_RuntimeState()
	{
		Version = 1;
		RouteID = "";
		WaypointIndex = 0;
		CurrentState = DZRB_MobileState.DZRB_INACTIVE;
		CurrentStopID = "";
		LastCompletedStop = "";
	}
}

class DZRB_RouteFiles
{
	static void List(out TStringArray names)
	{
		names = new TStringArray();
		string fileName;
		FileAttr attr;
		FindFileHandle handle = FindFile(DZRB_MobileSettings.ROUTES_DIR + "/*.json", fileName, attr, FindFileFlags.ALL);
		if (!handle) return;
		while (true)
		{
			if (fileName != "" && !(attr & FileAttr.DIRECTORY)) names.Insert(fileName);
			if (!FindNextFile(handle, fileName, attr)) break;
		}
		CloseFindFile(handle);
	}
}

class DZRB_StatusBridge
{
	static string s_Status = "UNTERWEGS";
	static void Set(string status) { s_Status = status; }
}

void DZRB_FillWelcomeStatus(array<string> output)
{
	if (!output) return;
	output.Clear();
	output.Insert("SCHWARZMARKT\n" + DZRB_StatusBridge.s_Status);
}
