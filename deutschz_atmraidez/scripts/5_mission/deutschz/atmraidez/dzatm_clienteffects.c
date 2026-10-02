class DZATM_RedBlinkLight: PointLightBase
{
	protected float m_Time;
	protected float m_Interval = 0.75;
	protected float m_PulseBrightness = 8.0;

	void Configure(float interval, float radius, float brightness)
	{
		m_Interval = Math.Max(interval, 0.1);
		m_PulseBrightness = Math.Max(brightness, 0.1);
		SetVisibleDuringDaylight(true);
		SetRadiusTo(radius);
		SetCastShadow(true);
		SetDiffuseColor(1.0, 0.0, 0.0);
		SetAmbientColor(1.0, 0.0, 0.0);
	}

	override void OnFrameLightSource(IEntity other, float timeSlice)
	{
		m_Time = m_Time + timeSlice;
		float cycle = Math.ModFloat(m_Time, m_Interval * 2.0);
		if (cycle < m_Interval)
			SetBrightnessTo(m_PulseBrightness);
		else
			SetBrightnessTo(0.0);
	}
}

class DZATM_ClientEffects
{
	protected static ref DZATM_ClientEffects s_Instance;
	protected ref map<string, ref EffectSound> m_Sounds;
	protected ref map<string, DZATM_RedBlinkLight> m_Lights;

	void DZATM_ClientEffects()
	{
		m_Sounds = new map<string, ref EffectSound>;
		m_Lights = new map<string, DZATM_RedBlinkLight>;
	}

	static DZATM_ClientEffects Get()
	{
		if (!s_Instance)
			s_Instance = new DZATM_ClientEffects;
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
		while (DZATM_ClientEffectQueue.Events.Count() > 0)
		{
			DZATM_ClientEffectEvent eventData = DZATM_ClientEffectQueue.Events[0];
			DZATM_ClientEffectQueue.Events.Remove(0);
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
		if (soundSet.IndexOf(DZATM_Const.CLIENT_BEACON_SOUNDSET) == 0)
		{
			array<string> parts = new array<string>;
			soundSet.Split("|", parts);
			float interval = 0.75;
			float radius = 20.0;
			float brightness = 8.0;
			if (parts.Count() > 1) interval = parts[1].ToFloat();
			if (parts.Count() > 2) radius = parts[2].ToFloat();
			if (parts.Count() > 3) brightness = parts[3].ToFloat();
			DZATM_RedBlinkLight light = DZATM_RedBlinkLight.Cast(ScriptedLightBase.CreateLight(DZATM_RedBlinkLight, position));
			if (light)
			{
				light.Configure(interval, radius, brightness);
				m_Lights.Set(effectId, light);
			}
			return;
		}
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
		DZATM_RedBlinkLight light;
		if (m_Lights.Find(effectId, light) && light)
			light.FadeOut(0);
		m_Lights.Remove(effectId);
	}

	protected void StopAll()
	{
		foreach (string effectId, EffectSound effect: m_Sounds)
		{
			if (effect)
				effect.SoundStop();
		}
		m_Sounds.Clear();
		foreach (string lightId, DZATM_RedBlinkLight light: m_Lights)
		{
			if (light)
				light.FadeOut(0);
		}
		m_Lights.Clear();
	}
}
