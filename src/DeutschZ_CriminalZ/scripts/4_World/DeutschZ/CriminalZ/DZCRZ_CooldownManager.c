class DZCRZ_CooldownManager
{
	protected ref map<string, float> m_Remaining;
	protected bool m_Persist;

	void DZCRZ_CooldownManager(bool persist)
	{
		m_Remaining = new map<string, float>;
		m_Persist = persist;
		Load();
	}

	bool IsActive(string targetId)
	{
		float remaining;
		return m_Remaining.Find(targetId, remaining) && remaining > 0.0;
	}

	float GetRemaining(string targetId)
	{
		float remaining;
		if (m_Remaining.Find(targetId, remaining))
			return remaining;
		return 0.0;
	}

	void Set(string targetId, float seconds)
	{
		if (targetId == "" || seconds <= 0.0)
			return;
		m_Remaining.Set(targetId, seconds);
		Save();
	}

	bool Clear(string targetId)
	{
		float remaining;
		if (m_Remaining.Find(targetId, remaining))
		{
			m_Remaining.Remove(targetId);
			Save();
			return true;
		}
		return false;
	}

	void Tick(float deltaSeconds)
	{
		array<string> expired = new array<string>;
		foreach (string targetId, float remaining: m_Remaining)
		{
			remaining = remaining - deltaSeconds;
			if (remaining <= 0.0)
				expired.Insert(targetId);
			else
				m_Remaining.Set(targetId, remaining);
		}
		foreach (string expiredId: expired)
			m_Remaining.Remove(expiredId);
	}

	void Save()
	{
		if (!m_Persist)
			return;
		DZCRZ_CooldownFile fileData = new DZCRZ_CooldownFile;
		foreach (string targetId, float remaining: m_Remaining)
		{
			if (remaining <= 0.0)
				continue;
			DZCRZ_CooldownEntry entry = new DZCRZ_CooldownEntry;
			entry.TargetId = targetId;
			entry.RemainingSeconds = remaining;
			fileData.Entries.Insert(entry);
		}
		string errorMessage;
		if (!JsonFileLoader<ref DZCRZ_CooldownFile>.SaveFile(DZCRZ_ProfilePaths.COOLDOWNS, fileData, errorMessage))
			DZCRZ_Log.Error("Cooldowns konnten nicht gespeichert werden: " + errorMessage);
	}

	protected void Load()
	{
		if (!m_Persist || !FileExist(DZCRZ_ProfilePaths.COOLDOWNS))
			return;
		DZCRZ_CooldownFile fileData = new DZCRZ_CooldownFile;
		string errorMessage;
		if (!JsonFileLoader<ref DZCRZ_CooldownFile>.LoadFile(DZCRZ_ProfilePaths.COOLDOWNS, fileData, errorMessage))
		{
			DZCRZ_Log.Error("Cooldowns konnten nicht geladen werden: " + errorMessage);
			return;
		}
		foreach (DZCRZ_CooldownEntry entry: fileData.Entries)
		{
			if (entry && entry.TargetId != "" && entry.RemainingSeconds > 0.0)
				m_Remaining.Set(entry.TargetId, entry.RemainingSeconds);
		}
		DZCRZ_Log.Info("Persistente Cooldowns geladen: " + m_Remaining.Count().ToString());
	}
}
