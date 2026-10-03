enum DZADZ_RavenPhase
{
	DZADZ_IDLE = 0,
	DZADZ_INTERCEPT = 1,
	DZADZ_TARGET_ZONE = 2,
	DZADZ_DROP_RELEASED = 3,
	DZADZ_TRANSPONDER = 4,
	DZADZ_OPENED = 5,
	DZADZ_CLEANUP = 6,
	DZADZ_FLIGHT = 7,
	DZADZ_LANDED = 8
}

class DZADZ_EventBus
{
	static ref ScriptInvoker SI_RavenPhase = new ScriptInvoker;
	static ref ScriptInvoker SI_Completed = new ScriptInvoker;
	static void Emit(int phase, vector position, float searchRadius)
	{
		SI_RavenPhase.Invoke(phase, position, searchRadius);
	}
}

class DZADZ_EventLock
{
	protected static ref map<string, bool> s_Active = new map<string, bool>;
	static void Set(string eventId, bool active)
	{
		if (active)
			s_Active.Set(eventId, true);
		else
			s_Active.Remove(eventId);
	}
	static bool AnyActive()
	{
		foreach (string id, bool active : s_Active)
		{
			if (active)
			{
				return true;
			}
		}
		return false;
	}
	static bool IsActive(string eventId)
	{
		return s_Active.Contains(eventId);
	}
}
