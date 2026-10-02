class DZATM_CooldownManager
{
    protected ref map<string, int> m_ExpiresAt;
    protected bool m_Persist;

    void DZATM_CooldownManager(bool persist)
    {
        m_ExpiresAt = new map<string, int>;
        m_Persist = persist;
        Load();
    }

    bool IsActive(string id)
    {
        int expiresAt;
        return m_ExpiresAt.Find(id, expiresAt) && expiresAt > NowUTC();
    }

    float GetRemaining(string id)
    {
        int expiresAt;
        if (m_ExpiresAt.Find(id, expiresAt)) return Math.Max(expiresAt - NowUTC(), 0);
        return 0.0;
    }

    void Set(string id, float seconds)
    {
        if (id == "" || seconds <= 0.0) return;
        m_ExpiresAt.Set(id, NowUTC() + Math.Ceil(seconds));
        Save();
    }

    bool Clear(string id)
    {
        int expiresAt;
        if (!m_ExpiresAt.Find(id, expiresAt)) return false;
        m_ExpiresAt.Remove(id);
        Save();
        return true;
    }

    void Tick(float deltaSeconds)
    {
        int now = NowUTC();
        array<string> expired = new array<string>;
        foreach (string id, int expiresAt: m_ExpiresAt)
            if (expiresAt <= now) expired.Insert(id);
        foreach (string expiredId: expired) m_ExpiresAt.Remove(expiredId);
    }

    void Save()
    {
        if (!m_Persist) return;
        DZATM_CooldownFile fileData = new DZATM_CooldownFile;
        int now = NowUTC();
        foreach (string id, int expiresAt: m_ExpiresAt)
        {
            if (expiresAt <= now) continue;
            DZATM_CooldownEntry entry = new DZATM_CooldownEntry;
            entry.TargetId = id;
            entry.ExpiresAtUTC = expiresAt;
            fileData.Entries.Insert(entry);
        }
        string errorMessage;
        if (!JsonFileLoader<ref DZATM_CooldownFile>.SaveFile(DZATM_ProfilePaths.COOLDOWNS, fileData, errorMessage))
            DZATM_Log.Error("Cooldowns konnten nicht gespeichert werden: " + errorMessage);
    }

    protected void Load()
    {
        if (!m_Persist || !FileExist(DZATM_ProfilePaths.COOLDOWNS)) return;
        DZATM_CooldownFile fileData = new DZATM_CooldownFile;
        string errorMessage;
        if (!JsonFileLoader<ref DZATM_CooldownFile>.LoadFile(DZATM_ProfilePaths.COOLDOWNS, fileData, errorMessage))
        {
            DZATM_Log.Error("Cooldowns konnten nicht geladen werden: " + errorMessage);
            return;
        }
        int now = NowUTC();
        foreach (DZATM_CooldownEntry entry: fileData.Entries)
        {
            if (!entry || entry.TargetId == "") continue;
            int expiresAt = entry.ExpiresAtUTC;
            if (expiresAt <= 0 && entry.RemainingSeconds > 0.0)
                expiresAt = now + Math.Ceil(entry.RemainingSeconds);
            if (expiresAt > now)
                m_ExpiresAt.Set(entry.TargetId, expiresAt);
        }
        if (fileData.Version < 2) Save();
    }

    protected int NowUTC()
    {
        return ExpansionStatic.GetTimestamp(true);
    }
}
