class DZPHouseRespawnData
{
    int SchemaVersion = 1;
    ref map<string, int> LastUsedUTC = new map<string, int>;
}

// One real-time cooldown per owner, independent of house purchases and Expansion settings.
class DZPHouseRespawnStore
{
    private static ref DZPHouseRespawnStore s_Instance;
    private static const string FILE = "$profile:DeutschZ-System\\PropertyZ\\HouseRespawns.json";
    private ref DZPHouseRespawnData m_Data;
    private bool m_Ready;

    static DZPHouseRespawnStore Get()
    {
        if (!s_Instance) s_Instance = new DZPHouseRespawnStore;
        return s_Instance;
    }

    void DZPHouseRespawnStore()
    {
        if (!GetGame().IsServer()) return;
        DZPSettingsService.Get();
        m_Data = new DZPHouseRespawnData;
        string errorMessage;
        if (FileExist(FILE) && !JsonFileLoader<DZPHouseRespawnData>.LoadFile(FILE, m_Data, errorMessage))
        {
            Print("[PropertyZ] House respawn disabled: " + errorMessage);
            return;
        }
        if (!m_Data || m_Data.SchemaVersion != 1 || !m_Data.LastUsedUTC) return;
        m_Ready = true;
    }

    static int Remaining(int nowUTC, int lastUsedUTC)
    {
        // Treat future timestamps conservatively after a server clock correction.
        if (lastUsedUTC > nowUTC) return 86400;
        int elapsed = nowUTC - lastUsedUTC;
        if (elapsed >= 86400) return 0;
        return 86400 - elapsed;
    }

    // Persist before allowing the teleport. A failed write never grants a free respawn.
    bool IsBlocked(string playerUID, bool consume)
    {
        if (!GetGame().IsServer() || !m_Ready || playerUID == "") return true;
        int nowUTC = ExpansionStatic.GetTimestamp(true);
        if (nowUTC <= 0) return true;
        int lastUsedUTC;
        if (m_Data.LastUsedUTC.Find(playerUID, lastUsedUTC) && Remaining(nowUTC, lastUsedUTC) > 0) return true;
        if (!consume) return false;

        m_Data.LastUsedUTC.Set(playerUID, nowUTC);
        string errorMessage;
        if (!JsonFileLoader<DZPHouseRespawnData>.SaveFile(FILE, m_Data, errorMessage))
        {
            m_Ready = false;
            Print("[PropertyZ] House respawn disabled: " + errorMessage);
            return true;
        }
        Print("[PropertyZ] House respawn reserved; next use after 86400 seconds.");
        return false;
    }
}
