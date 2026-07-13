class DZCRZ_AlarmManager
{
	protected ref set<string> m_Active;

	void DZCRZ_AlarmManager()
	{
		m_Active = new set<string>;
	}

	void Start(string effectId, string soundSet, vector position, bool loop = true)
	{
		if (effectId == "" || soundSet == "")
			return;
		Stop(effectId);
		Broadcast(true, effectId, soundSet, position, loop);
		m_Active.Insert(effectId);
	}

	void Stop(string effectId)
	{
		if (effectId == "")
			return;
		Broadcast(false, effectId, "", vector.Zero, false);
		int index = m_Active.Find(effectId);
		if (index >= 0)
			m_Active.Remove(index);
	}

	void StopAll()
	{
		array<string> ids = new array<string>;
		foreach (string effectId: m_Active)
			ids.Insert(effectId);
		foreach (string id: ids)
			Stop(id);
	}

	protected void Broadcast(bool start, string effectId, string soundSet, vector position, bool loop)
	{
		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		foreach (Man man: players)
		{
			PlayerBase player = PlayerBase.Cast(man);
			if (player)
				DZCRZ_ServerRPC.SendEffect(player, start, effectId, soundSet, position, loop);
		}
	}
}
