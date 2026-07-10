class DZBBC_FinalDefenseManager
{
	protected ref DZBBC_AIFactionManager m_AI;
	protected bool m_Spawned;

	void Setup(DZBBC_AIFactionManager ai)
	{
		m_AI = ai;
		m_Spawned = false;
	}

	void Spawn(DZBBC_AIFactionConfig wave, DZBBC_MainConfig main, vector center)
	{
		if (m_Spawned || !m_AI)
			return;

		m_AI.SpawnFaction(wave, main, center);
		m_Spawned = true;
	}

	void Reset()
	{
		m_Spawned = false;
	}
}
