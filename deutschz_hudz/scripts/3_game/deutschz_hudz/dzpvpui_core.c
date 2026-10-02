class DZPVPUI_Constants
{
    static const int PROTOCOL = 3;
    static const int RPC_LEADERBOARD_REQUEST = 7465101;
    static const int RPC_LEADERBOARD_RESPONSE = 7465102;
    static const int RPC_STATUS_REQUEST = 7465103;
    static const int RPC_STATUS_RESPONSE = 7465104;
    static const string PROFILE_DIR = "$profile:DeutschZ-System/pvp_ui";
    static const string SETTINGS_PATH = "$profile:DeutschZ-System/pvp_ui/settings.json";
    static const string SETTINGS_EXAMPLE_PATH = "$profile:DeutschZ-System/pvp_ui/settings.example.jsonc";
    static const string CLIENT_PATH = "$profile:DeutschZ-System/pvp_ui/client.json";
    static const string DISPLAY_CACHE_PATH = "$profile:DeutschZ-System/pvp_ui/display_cache.json";
    static const string LEADERBOARD_PATH = "$profile:DeutschZ-System/pvp_ui/leaderboard.json";
    static const string HUD_LAYOUT = "deutschz_hudz/gui/layouts/hudz_hud.layout";
    static const string LEADERBOARD_LAYOUT = "deutschz_hudz/gui/layouts/leaderboard.layout";
}

class DZPVPUI_ClientDisplayCache
{
    int Version = 1;
    string PlayerUid = "";
    string LeaderboardWinnerText = "";
    string LeaderboardSummary = "";
    string LeaderboardText = "";
    int AccountBalance = -1;
    string GroupTitle = "";
    int GroupBalance = -1;
    ref array<string> GroupMemberNames = new array<string>;
    ref array<int> GroupMemberOnline = new array<int>;
}

class DZPVPUI_SettingsGlobal
{
    bool Enabled = true;
    float UpdateIntervalSeconds = 0.10;
    float MusicDefaultVolume = 0.35;
    float MusicMaximumVolume = 0.70;
    int F12HoldMilliseconds = 450;

    void Validate()
    {
        UpdateIntervalSeconds = Math.Clamp(UpdateIntervalSeconds, 0.05, 1.0);
        MusicDefaultVolume = Math.Clamp(MusicDefaultVolume, 0.0, 1.0);
        MusicMaximumVolume = Math.Clamp(MusicMaximumVolume, 0.05, 1.0);
        MusicDefaultVolume = Math.Min(MusicDefaultVolume, MusicMaximumVolume);
        F12HoldMilliseconds = Math.Clamp(F12HoldMilliseconds, 250, 1000);
    }
}

class DZPVPUI_SettingsScope
{
    ref DZPVPUI_SettingsGlobal Global = new DZPVPUI_SettingsGlobal;
    ref array<string> Zones = new array<string>;
}

class DZPVPUI_SettingsFile
{
    int Version = 1;
    ref DZPVPUI_SettingsScope Free = new DZPVPUI_SettingsScope;
    ref DZPVPUI_SettingsScope Pro = new DZPVPUI_SettingsScope;
}

class DZPVPUI_ClientPreferences
{
    int Version = 5;
    bool HudVisible = true;
    bool CrosshairVisible = true;
    int CrosshairShape = 0;
    int CrosshairSize = 0;
    int CrosshairColor = 0;
    float MusicVolume = 0.35;
    int TrackIndex = 0;
    int PlaylistIndex = 0;

    void Validate(float maximumVolume)
    {
        if (Version < 3)
        {
            CrosshairShape = 0;
            CrosshairSize = 0;
            CrosshairColor = 0;
        }
        Version = 5;
        CrosshairShape = Math.Clamp(CrosshairShape, 0, 2);
        CrosshairSize = Math.Clamp(CrosshairSize, 0, 12);
        CrosshairColor = Math.Clamp(CrosshairColor, 0, 5);
        MusicVolume = Math.Clamp(MusicVolume, 0.0, maximumVolume);
        TrackIndex = Math.Clamp(TrackIndex, 0, 48);
        PlaylistIndex = Math.Clamp(PlaylistIndex, 0, 2);
    }
}

class DZPVPUI_State
{
    static ref DZPVPUI_SettingsGlobal Settings = new DZPVPUI_SettingsGlobal;
    static ref DZPVPUI_ClientPreferences Preferences = new DZPVPUI_ClientPreferences;
    static string LeaderboardWinnerText = "";
    static string LeaderboardSummary = "";
    static string LeaderboardText = "RANG  SPIELER                 PVP  INF  TOD  SCORE\n\nDATEN WERDEN GELADEN...";
    static int RestartSeconds = -1;
    static int RestartSyncTime = 0;
    static int AccountBalance = -1;
    static int HealthPercent = -1;
    static int BloodPercent = -1;
    static int EnergyPercent = -1;
    static int WaterPercent = -1;
}

class DZPVPUI_ClientDisplayCacheStore
{
    static ref DZPVPUI_ClientDisplayCache Cache;
    protected static bool s_Loaded;
    protected static string s_GroupSignature;

    static void Load()
    {
        if (s_Loaded)
        {
            ApplyToState();
            return;
        }
        s_Loaded = true;
        Cache = new DZPVPUI_ClientDisplayCache;
        MakeDirectory("$profile:DeutschZ-System");
        MakeDirectory(DZPVPUI_Constants.PROFILE_DIR);
        if (FileExist(DZPVPUI_Constants.DISPLAY_CACHE_PATH))
        {
            string error;
            JsonFileLoader<DZPVPUI_ClientDisplayCache>.LoadFile(DZPVPUI_Constants.DISPLAY_CACHE_PATH, Cache, error);
            if (error != "")
            {
                Print("[DZPVPUI][CACHE] load failed: " + error);
                Cache = new DZPVPUI_ClientDisplayCache;
            }
        }
        Validate();
        ApplyToState();
        s_GroupSignature = BuildGroupSignature(Cache.GroupTitle, Cache.GroupBalance, Cache.GroupMemberNames, Cache.GroupMemberOnline);
        Print("[DZPVPUI][CACHE] client display cache loaded");
    }

    static void SaveServerSnapshot(string playerUid)
    {
        Load();
        bool changed;
        if (playerUid != "" && Cache.PlayerUid != playerUid) { Cache.PlayerUid = playerUid; changed = true; }
        if (DZPVPUI_State.LeaderboardWinnerText != "" && Cache.LeaderboardWinnerText != DZPVPUI_State.LeaderboardWinnerText) { Cache.LeaderboardWinnerText = DZPVPUI_State.LeaderboardWinnerText; changed = true; }
        if (DZPVPUI_State.LeaderboardSummary != "" && Cache.LeaderboardSummary != DZPVPUI_State.LeaderboardSummary) { Cache.LeaderboardSummary = DZPVPUI_State.LeaderboardSummary; changed = true; }
        if (DZPVPUI_State.LeaderboardText != "" && Cache.LeaderboardText != DZPVPUI_State.LeaderboardText) { Cache.LeaderboardText = DZPVPUI_State.LeaderboardText; changed = true; }
        if (DZPVPUI_State.AccountBalance >= 0 && Cache.AccountBalance != DZPVPUI_State.AccountBalance) { Cache.AccountBalance = DZPVPUI_State.AccountBalance; changed = true; }
        if (changed)
            Save();
    }

    static void SaveGroupSnapshot(string title, int balance, array<string> names, array<int> online)
    {
        if (title == "" || !names || !online)
            return;
        Load();
        string signature = BuildGroupSignature(title, balance, names, online);
        if (signature == s_GroupSignature)
            return;
        Cache.GroupTitle = title;
        Cache.GroupBalance = balance;
        Cache.GroupMemberNames = new array<string>;
        Cache.GroupMemberOnline = new array<int>;
        int count = Math.Min(names.Count(), online.Count());
        for (int i = 0; i < count; i++)
        {
            Cache.GroupMemberNames.Insert(names.Get(i));
            Cache.GroupMemberOnline.Insert(online.Get(i));
        }
        s_GroupSignature = signature;
        Save();
    }

    static void ApplyToState()
    {
        if (!Cache) return;
        if (Cache.LeaderboardWinnerText != "") DZPVPUI_State.LeaderboardWinnerText = Cache.LeaderboardWinnerText;
        if (Cache.LeaderboardSummary != "") DZPVPUI_State.LeaderboardSummary = Cache.LeaderboardSummary;
        if (Cache.LeaderboardText != "") DZPVPUI_State.LeaderboardText = Cache.LeaderboardText;
        if (Cache.AccountBalance >= 0) DZPVPUI_State.AccountBalance = Cache.AccountBalance;
    }

    protected static void Validate()
    {
        if (!Cache) Cache = new DZPVPUI_ClientDisplayCache;
        Cache.Version = 1;
        if (!Cache.GroupMemberNames) Cache.GroupMemberNames = new array<string>;
        if (!Cache.GroupMemberOnline) Cache.GroupMemberOnline = new array<int>;
    }

    protected static string BuildGroupSignature(string title, int balance, array<string> names, array<int> online)
    {
        string signature = title + "|" + balance.ToString();
        if (!names || !online) return signature;
        int count = Math.Min(names.Count(), online.Count());
        for (int i = 0; i < count; i++) signature += "|" + names.Get(i) + ":" + online.Get(i).ToString();
        return signature;
    }

    protected static void Save()
    {
        string error;
        JsonFileLoader<DZPVPUI_ClientDisplayCache>.SaveFile(DZPVPUI_Constants.DISPLAY_CACHE_PATH, Cache, error);
        if (error != "") Print("[DZPVPUI][CACHE] save failed: " + error);
        else Print("[DZPVPUI][CACHE] client display cache updated");
    }
}
