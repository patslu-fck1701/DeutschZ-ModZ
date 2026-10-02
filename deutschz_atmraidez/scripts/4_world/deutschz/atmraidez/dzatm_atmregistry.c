class DZATM_ATMRegistry
{
    protected ref map<string, ExpansionATMBase> m_ATMs;

    void DZATM_ATMRegistry()
    {
        m_ATMs = new map<string, ExpansionATMBase>;
    }

    string Register(ExpansionATMBase atm)
    {
        if (!atm) return "";
        string id = DZATM_PlayerUtils.SafeATMId(atm);
        atm.DZATM_SetATMId(id);
        m_ATMs.Set(id, atm);
        return id;
    }

    void Unregister(ExpansionATMBase atm)
    {
        if (!atm) return;
        array<string> remove = new array<string>;
        foreach (string id, ExpansionATMBase registered: m_ATMs)
        {
            if (registered == atm) remove.Insert(id);
        }
        foreach (string removeId: remove) m_ATMs.Remove(removeId);
    }

    int Count() { return m_ATMs.Count(); }

    string FindNearestId(vector position, float maxDistance)
    {
        float best = maxDistance + 0.01;
        string bestId = "";
        foreach (string id, ExpansionATMBase atm: m_ATMs)
        {
            if (!atm) continue;
            float distance = vector.Distance(position, atm.GetPosition());
            if (distance <= maxDistance && distance < best)
            {
                best = distance;
                bestId = id;
            }
        }
        return bestId;
    }

    void UpdateLocks(DZATM_CooldownManager cooldowns)
    {
        if (!cooldowns) return;
        foreach (string id, ExpansionATMBase atm: m_ATMs)
        {
            if (!atm) continue;
            bool cooldownActive = cooldowns.IsActive(id);
            atm.DZATM_SetRaidLocked(cooldownActive);
            atm.DZATM_SetRaidActionBlocked(cooldownActive);
        }
    }

    void ApplyHackDuration(float seconds)
    {
        foreach (string id, ExpansionATMBase atm: m_ATMs)
            if (atm) atm.DZATM_SetHackDuration(seconds);
    }

    void UpdateLocksWithSessions(DZATM_CooldownManager cooldowns, array<string> activeTargetIds, array<string> blockedRaidActionIds)
    {
        if (!cooldowns || !activeTargetIds || !blockedRaidActionIds) return;
        foreach (string id, ExpansionATMBase atm: m_ATMs)
        {
            if (!atm) continue;
            bool cooldownActive = cooldowns.IsActive(id);
            atm.DZATM_SetRaidLocked(cooldownActive || activeTargetIds.Find(id) >= 0);
            atm.DZATM_SetRaidActionBlocked(cooldownActive || blockedRaidActionIds.Find(id) >= 0);
        }
    }
}
