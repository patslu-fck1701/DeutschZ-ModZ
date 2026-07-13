class DZCRZ_GarageHackManager
{
	protected bool m_BlockerLogged;

	bool CanHack(Object target)
	{
		if (!m_BlockerLogged)
		{
			m_BlockerLogged = true;
			DZCRZ_Log.Warn("Garage-Modul vorbereitet, aber deaktiviert: installierte Expansion-Version liefert keine allgemeine sichere Garage-Unlock-API.");
		}
		return false;
	}

	bool CompleteHack(Object target)
	{
		return false;
	}
}
