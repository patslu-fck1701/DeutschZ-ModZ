const string DZECZ_VERSION = "0.1.0";
const string DZECZ_PROFILE_DIR = "$profile:DZECZ";
const string DZECZ_CONFIG_PATH = "$profile:DZECZ/dzecz_config.json";
const string DZECZ_PROGRESS_PATH = "$profile:DZECZ/dzecz_progress.json";
const string DZECZ_MARKER_UID = "DZECZ_ACTIVE_SECTOR";

const int DZECZ_INACTIVE = 0;
const int DZECZ_PREREQUISITES_CHECK = 1;
const int DZECZ_RIDDLE_STARTED = 2;
const int DZECZ_STATION_ALPHA_ACTIVE = 3;
const int DZECZ_STATION_ALPHA_COMPLETE = 4;
const int DZECZ_STATION_BRAVO_ACTIVE = 5;
const int DZECZ_STATION_BRAVO_COMPLETE = 6;
const int DZECZ_STATION_CHARLIE_ACTIVE = 7;
const int DZECZ_STATION_CHARLIE_COMPLETE = 8;
const int DZECZ_ALL_FRAGMENTS_COMPLETE = 9;
const int DZECZ_FIREWORK_BATTERY_REVEALED = 10;
const int DZECZ_FIREWORK_BATTERY_ARMING = 11;
const int DZECZ_FIREWORK_COUNTDOWN = 12;
const int DZECZ_ECLIPSE_SIGNAL_SENT = 13;
const int DZECZ_FINAL_ZONE_REVEALED = 14;
const int DZECZ_FINAL_ZONE_ACTIVE = 15;
const int DZECZ_FINAL_TERMINAL_ACTIVE = 16;
const int DZECZ_FINAL_DEFENSE_ACTIVE = 17;
const int DZECZ_FINAL_CHOICE_ACTIVE = 18;
const int DZECZ_REWARD_UNLOCKED = 19;
const int DZECZ_COMPLETED = 20;
const int DZECZ_FAILED = 21;
const int DZECZ_CLEANUP = 22;

class DZECZ_Config
{
	string EventName;
	bool Enabled;
	bool RequiresBattlegroundZUnlock;
	bool RequiresFireworksLauncher;
	bool RequiresGasZoneFlare;
	bool RequiresSignalMatches;
	bool RequiresCodedBattery;
	bool AllowGroupItemCheck;
	float GroupItemCheckRadius;
	float PlayerInteractionDistance;
	float StationActivationSecondsNWAF;
	float StationActivationSecondsTisy;
	float StationActivationSecondsGasZone;
	float FireworkBatteryInsertBatterySeconds;
	float FireworkBatterySyncSeconds;
	float FireworkBatteryIgnitionSeconds;
	float FireworkCountdownSeconds;
	float FinalTerminalSeconds;
	float GlobalSectorHintSizeMeters;
	float ActivatorHintSizeMeters;
	bool NoPermanentTracking;
	float CleanupDelaySeconds;
	bool ConsumeCodedBattery;
	bool ConsumeSignalMatches;
	bool ConsumeGasZoneFlare;
	bool ConsumeFireworksLauncher;
	bool DamageFireworksLauncher;
	ref array<float> ReceiverPosition;
	ref array<float> NWAFPosition;
	ref array<float> TisyPosition;
	ref array<float> GasZonePosition;
	ref array<float> FireworkBatteryPosition;
	ref array<ref array<float>> FinalPositions;

	void DZECZ_Config()
	{
		SetDefaults();
	}

	void SetDefaults()
	{
		EventName = "DeutschZ Operation EclipseZ";
		Enabled = true;
		RequiresBattlegroundZUnlock = true;
		RequiresFireworksLauncher = true;
		RequiresGasZoneFlare = true;
		RequiresSignalMatches = true;
		RequiresCodedBattery = true;
		AllowGroupItemCheck = true;
		GroupItemCheckRadius = 10.0;
		PlayerInteractionDistance = 4.0;
		StationActivationSecondsNWAF = 45.0;
		StationActivationSecondsTisy = 60.0;
		StationActivationSecondsGasZone = 30.0;
		FireworkBatteryInsertBatterySeconds = 20.0;
		FireworkBatterySyncSeconds = 30.0;
		FireworkBatteryIgnitionSeconds = 20.0;
		FireworkCountdownSeconds = 30.0;
		FinalTerminalSeconds = 90.0;
		GlobalSectorHintSizeMeters = 1500.0;
		ActivatorHintSizeMeters = 400.0;
		NoPermanentTracking = true;
		CleanupDelaySeconds = 900.0;
		ConsumeCodedBattery = true;
		ConsumeSignalMatches = true;
		ConsumeGasZoneFlare = true;
		ConsumeFireworksLauncher = false;
		DamageFireworksLauncher = true;
		ReceiverPosition = MakePosition(8120.0, 475.0, 9210.0);
		NWAFPosition = MakePosition(4640.0, 340.0, 10420.0);
		TisyPosition = MakePosition(1640.0, 450.0, 14140.0);
		GasZonePosition = MakePosition(13750.0, 6.0, 13650.0);
		FireworkBatteryPosition = MakePosition(3705.0, 402.0, 5980.0);
		FinalPositions = new array<ref array<float>>;
		FinalPositions.Insert(MakePosition(1600.0, 450.0, 14080.0));
		FinalPositions.Insert(MakePosition(4550.0, 340.0, 10300.0));
		FinalPositions.Insert(MakePosition(1200.0, 6.0, 2400.0));
	}

	static ref array<float> MakePosition(float x, float y, float z)
	{
		ref array<float> values = new array<float>;
		values.Insert(x);
		values.Insert(y);
		values.Insert(z);
		return values;
	}

	static vector ToVector(array<float> values)
	{
		if (!values || values.Count() < 3)
			return "0 0 0";
		return Vector(values[0], values[1], values[2]);
	}

	static ref DZECZ_Config Load()
	{
		MakeDirectory(DZECZ_PROFILE_DIR);
		ref DZECZ_Config config = new DZECZ_Config;
		string errorMessage;
		if (FileExist(DZECZ_CONFIG_PATH))
		{
			if (!JsonFileLoader<ref DZECZ_Config>.LoadFile(DZECZ_CONFIG_PATH, config, errorMessage))
				Print("[DZECZ][ERROR] Config load failed: " + errorMessage);
		}
		else
		{
			JsonFileLoader<ref DZECZ_Config>.SaveFile(DZECZ_CONFIG_PATH, config, errorMessage);
		}
		return config;
	}
}

class DZECZ_Progress
{
	int State;
	ref array<int> StationOrder;
	int CurrentStationOrderIndex;
	int BatteryStep;
	int FinalLocationIndex;
	string ActivatorUID;
	bool RewardSpawned;

	void DZECZ_Progress()
	{
		Reset();
	}

	void Reset()
	{
		State = DZECZ_INACTIVE;
		StationOrder = new array<int>;
		CurrentStationOrderIndex = 0;
		BatteryStep = 0;
		FinalLocationIndex = -1;
		ActivatorUID = "";
		RewardSpawned = false;
	}
}
