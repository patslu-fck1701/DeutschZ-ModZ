class DZCRZ_ClientEffects
{
	protected static ref DZCRZ_ClientEffects s_Instance;
	protected ref map<string, ref EffectSound> m_Sounds;

	void DZCRZ_ClientEffects()
	{
		m_Sounds = new map<string, ref EffectSound>;
	}

	static DZCRZ_ClientEffects Get()
	{
		if (!s_Instance)
			s_Instance = new DZCRZ_ClientEffects;
		return s_Instance;
	}

	static void Destroy()
	{
		if (s_Instance)
			s_Instance.StopAll();
		s_Instance = null;
	}

	void Update()
	{
		while (DZCRZ_ClientEffectQueue.Events.Count() > 0)
		{
			DZCRZ_ClientEffectEvent eventData = DZCRZ_ClientEffectQueue.Events[0];
			DZCRZ_ClientEffectQueue.Events.Remove(0);
			if (!eventData)
				continue;
			if (eventData.Start)
				Start(eventData.EffectId, eventData.SoundSet, eventData.Position, eventData.Loop);
			else
				Stop(eventData.EffectId);
		}
	}

	protected void Start(string effectId, string soundSet, vector position, bool loop)
	{
		Stop(effectId);
		EffectSound effect = SEffectManager.PlaySound(soundSet, position, 0.1, 0.1, loop);
		if (effect)
			m_Sounds.Set(effectId, effect);
	}

	protected void Stop(string effectId)
	{
		EffectSound effect;
		if (m_Sounds.Find(effectId, effect) && effect)
			effect.SoundStop();
		m_Sounds.Remove(effectId);
	}

	protected void StopAll()
	{
		foreach (string effectId, EffectSound effect: m_Sounds)
		{
			if (effect)
				effect.SoundStop();
		}
		m_Sounds.Clear();
	}
}
