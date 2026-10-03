class DZODZ_RewardEntry
{
    string ClassName = "";
    float Chance = 0.1;
    int MinimumCount = 1;
    int MaximumCount = 1;
}

class DZODZ_Settings
{
    int Enabled = 1;
    string TerminalPosition = "7500 0 7500";
    string TerminalOrientation = "0 0 0";
    int DebugEnabled = 0;
    int EventLootEnabled = 1;
    ref array<ref DZODZ_RewardEntry> EventLoot = new array<ref DZODZ_RewardEntry>;

    void DZODZ_Settings()
    {
        AddReward("M4A1", 1.0, 1, 1);
        AddReward("Mag_STANAG_30Rnd", 1.0, 2, 3);
        AddReward("AmmoBox_556x45_20Rnd", 1.0, 2, 4);
        AddReward("NVGoggles", 1.0, 1, 1);
        AddReward("LandMineTrap", 0.5, 1, 2);
    }

    protected void AddReward(string className, float chance, int minimum, int maximum)
    {
        ref DZODZ_RewardEntry entry = new DZODZ_RewardEntry;
        entry.ClassName = className;
        entry.Chance = chance;
        entry.MinimumCount = minimum;
        entry.MaximumCount = maximum;
        EventLoot.Insert(entry);
    }
}

class DZODZ_SettingsLoader
{
    static const string DIR = "$profile:DeutschZ-System/DeutschZ_Operation_DeutschZ/Config";
    static const string PATH = DIR + "/settings.json";

    static ref DZODZ_Settings Load()
    {
        MakeDirectory("$profile:DeutschZ-System");
        MakeDirectory("$profile:DeutschZ-System/DeutschZ_Operation_DeutschZ");
        MakeDirectory(DIR);
        ref DZODZ_Settings settings = new DZODZ_Settings();
        string error;
        if (FileExist(PATH))
            JsonFileLoader<ref DZODZ_Settings>.LoadFile(PATH, settings, error);
        else
            JsonFileLoader<ref DZODZ_Settings>.SaveFile(PATH, settings, error);
        return settings;
    }
}
