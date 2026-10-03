class DZATM_GeneralConfig
{
    bool Enabled = true;
    bool EnableGlobalNotifications = true;
    bool EnablePersonalNotifications = true;
    bool EnableExpansionMarkers = true;
    bool PreventParallelRaidOnSameATM = true;
    bool PersistCooldowns = true;
    bool AllowAdminWhenListEmpty = false;
    ref array<string> AdminUIDs;

    void DZATM_GeneralConfig()
    {
        AdminUIDs = new array<string>;
    }
}

class DZATM_RaidConfig
{
    int Phase1HackSeconds = 180;
    int Phase2GuardSeconds = 720;
    float RequiredRadiusMeters = 20.0;
    int ATMCooldownSeconds = 7200;
    int PlayerCooldownSeconds = 7200;

    string RequiredTool = "DZATM_RobTool";
    bool AcceptAnyCrowbar = true;
    bool RequireToolInHands = true;
    bool DestroyToolOnSuccess = true;
    bool DamageToolOnFail = true;
    float ToolDamageOnFail = 5.0;

    bool EnableSiren = true;
    bool EnableMapMarker = true;
    bool EnableRedSmoke = true;
    bool EnableGreenSmokeOnSuccess = true;
    string RedSmokeClass = "M18SmokeGrenade_Red";
    string GreenSmokeClass = "M18SmokeGrenade_Green";
    float EffectHeightMeters = 1.5;

    bool EnableBlinkLight = true;
    float BlinkIntervalSeconds = 0.75;
    float BlinkRadiusMeters = 20.0;
    float BlinkBrightness = 8.0;

    string RewardProvider = "PhysicalCurrency";
    string PayoutCurrencyClass = "ExpansionBanknoteEuro";
    int PayoutUnitValue = 100;
    int PayoutMinAmount = 50000;
    int PayoutMaxAmount = 200000;

    void Validate()
    {
        Phase1HackSeconds = Math.Clamp(Phase1HackSeconds, 10, 3600);
        Phase2GuardSeconds = Math.Clamp(Phase2GuardSeconds, 0, 3600);
        RequiredRadiusMeters = Math.Clamp(RequiredRadiusMeters, 2.0, 100.0);
        ATMCooldownSeconds = Math.Clamp(ATMCooldownSeconds, 0, 86400);
        PlayerCooldownSeconds = Math.Clamp(PlayerCooldownSeconds, 0, 86400);
        ToolDamageOnFail = Math.Clamp(ToolDamageOnFail, 0.0, 100.0);
        EffectHeightMeters = Math.Clamp(EffectHeightMeters, 0.0, 10.0);
        BlinkIntervalSeconds = Math.Clamp(BlinkIntervalSeconds, 0.1, 10.0);
        BlinkRadiusMeters = Math.Clamp(BlinkRadiusMeters, 1.0, 100.0);
        BlinkBrightness = Math.Clamp(BlinkBrightness, 0.1, 50.0);
        PayoutMinAmount = Math.Clamp(PayoutMinAmount, 1, 1000000);
        PayoutMaxAmount = Math.Clamp(PayoutMaxAmount, PayoutMinAmount, 1000000);
        PayoutUnitValue = Math.Clamp(PayoutUnitValue, 1, 1000000);
        if (RequiredTool == "") RequiredTool = "DZATM_RobTool";
        if (RewardProvider == "") RewardProvider = "PhysicalCurrency";
        if (PayoutCurrencyClass == "") PayoutCurrencyClass = "ExpansionBanknoteEuro";
    }
}


class DZATM_ATMSpawnEntry
{
    bool Enabled = true;
    ref array<float> Position;
    ref array<float> Orientation;

    void DZATM_ATMSpawnEntry()
    {
        Position = new array<float>;
        Orientation = new array<float>;
    }

    bool IsValid()
    {
        return Position && Position.Count() >= 3 && Orientation && Orientation.Count() >= 3;
    }

    vector GetPosition()
    {
        vector value = "0 0 0";
        if (!IsValid()) return value;
        value[0] = Position[0];
        value[1] = Position[1];
        value[2] = Position[2];
        return value;
    }

    vector GetOrientation()
    {
        vector value = "0 0 0";
        if (!IsValid()) return value;
        value[0] = Orientation[0];
        value[1] = Orientation[1];
        value[2] = Orientation[2];
        return value;
    }
}

class DZATM_SpawnConfig
{
    bool Enabled = true;
    string ATMClass = "ExpansionATM_2";
    float ReuseRadiusMeters = 2.0;
    ref array<ref DZATM_ATMSpawnEntry> ATMs;

    void DZATM_SpawnConfig()
    {
        ATMs = new array<ref DZATM_ATMSpawnEntry>;
    }

    void Validate()
    {
        // ATM RaideZ is intentionally fixed to the requested Expansion ATM model.
        ATMClass = DZATM_Const.ATM_CLASS;
        ReuseRadiusMeters = Math.Clamp(ReuseRadiusMeters, 0.5, 10.0);
        if (!ATMs) ATMs = new array<ref DZATM_ATMSpawnEntry>;
        EnsureDefaultATMs();
    }

    protected void EnsureDefaultATMs()
    {
        // Keep the requested Chernarus ATM set available even when an older profile JSON
        // has no Spawns block or when ATM_RaideZ.json has not been created yet.
        if (!ATMs) ATMs = new array<ref DZATM_ATMSpawnEntry>;
        if (ATMs.Count() > 0) return;

        AddDefaultATM(2671.698486, 205.659073, 5290.393066, 12.303730, 0.000000, -0.000000);
        AddDefaultATM(3048.137695, 304.109772, 7980.731445, -55.780781, 0.000000, -0.000000);
        AddDefaultATM(3085.295898, 209.175995, 12672.470703, 80.266129, 0.000000, -0.000000);
        AddDefaultATM(3841.386475, 311.367584, 8873.330078, 116.048309, 0.000000, -0.000000);
        AddDefaultATM(5664.817871, 64.439514, 2435.434082, -178.914734, 0.000000, -0.000000);
        AddDefaultATM(6048.077148, 301.057587, 7841.287109, -129.817108, 0.000000, -0.000000);
        AddDefaultATM(6073.935547, 52.976711, 3437.264893, -0.637233, -0.217462, 0.996062);
        AddDefaultATM(6384.567871, 15.683525, 2860.328369, 92.993843, 0.000000, -0.000000);
        AddDefaultATM(6522.647949, 61.760174, 3624.875244, 92.797661, 0.000000, -0.000000);
        AddDefaultATM(9541.625977, 84.323975, 13661.754883, -114.418518, 0.000000, -0.000000);
        AddDefaultATM(10429.598633, 6.247161, 2373.250488, 55.489651, 0.000000, -0.000000);
        AddDefaultATM(10681.122070, 211.194153, 8012.735840, -144.923080, 0.000000, -0.000000);
        AddDefaultATM(11132.894531, 199.239899, 12294.167969, 31.540527, 0.000000, -0.000000);
        AddDefaultATM(11847.361328, 33.506332, 14354.948242, 86.680527, 0.000000, -0.000000);
        AddDefaultATM(12023.482422, 54.188873, 9153.021484, -134.065735, 0.000000, -0.000000);
        AddDefaultATM(12114.365234, 12.331938, 9770.633789, 12.870724, 0.000000, -0.000000);
    }

    protected void AddDefaultATM(float x, float y, float z, float yaw, float pitch, float roll)
    {
        DZATM_ATMSpawnEntry entry = new DZATM_ATMSpawnEntry;
        entry.Position.Insert(x);
        entry.Position.Insert(y);
        entry.Position.Insert(z);
        entry.Orientation.Insert(yaw);
        entry.Orientation.Insert(pitch);
        entry.Orientation.Insert(roll);
        ATMs.Insert(entry);
    }
}

class DZATM_Config
{
    int Version = 3;
    bool DebugMode = false;
    ref DZATM_GeneralConfig General;
    ref DZATM_RaidConfig Raid;
    ref DZATM_SpawnConfig Spawns;

    void DZATM_Config()
    {
        General = new DZATM_GeneralConfig;
        Raid = new DZATM_RaidConfig;
        Spawns = new DZATM_SpawnConfig;
    }

    void Validate()
    {
        if (!General) General = new DZATM_GeneralConfig;
        if (!Raid) Raid = new DZATM_RaidConfig;
        if (!Spawns) Spawns = new DZATM_SpawnConfig;
        Raid.Validate();
        Spawns.Validate();
        if (Version < 3)
        {
            Version = 3;
        }
        DZATM_RuntimeFlags.DebugEnabled = DebugMode;
    }

    static DZATM_Config Load()
    {
        DZATM_ProfilePaths.Ensure();
        DZATM_Config config = new DZATM_Config;
        string errorMessage;

        if (FileExist(DZATM_ProfilePaths.MAIN_CONFIG))
        {
            if (!JsonFileLoader<ref DZATM_Config>.LoadFile(DZATM_ProfilePaths.MAIN_CONFIG, config, errorMessage))
                DZATM_Log.Error("Config konnte nicht geladen werden: " + errorMessage);
        }

        config.Validate();

        if (!JsonFileLoader<ref DZATM_Config>.SaveFile(DZATM_ProfilePaths.MAIN_CONFIG, config, errorMessage))
            DZATM_Log.Error("Config konnte nicht gespeichert werden: " + errorMessage);

        return config;
    }
}
