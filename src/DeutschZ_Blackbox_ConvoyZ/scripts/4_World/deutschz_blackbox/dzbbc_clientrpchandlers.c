class DZBBC_ClientHUD
{
	static void OnHudRPC(ParamsReadContext ctx)
	{
		int mode;
		string label;
		float current;
		float max;

		if (!ctx.Read(mode))
			return;
		if (!ctx.Read(label))
			return;
		if (!ctx.Read(current))
			return;
		if (!ctx.Read(max))
			return;

		if (mode == DZBBC_HUD_HIDE)
		{
			DZBBC_ClientHudState.Hide();
			Print(DZBBC_LOG_PREFIX + "HUD hide");
			return;
		}

		DZBBC_ClientHudState.Set(mode, label, current, max);
		Print(DZBBC_LOG_PREFIX + "HUD " + DZBBC_Utils.Localize(label) + " " + current.ToString() + "/" + max.ToString());
	}
}

class DZBBC_ClientNotifications
{
	static void OnNotificationRPC(ParamsReadContext ctx)
	{
		string title;
		string message;
		float time;

		if (!ctx.Read(title))
			return;
		if (!ctx.Read(message))
			return;
		if (!ctx.Read(time))
			return;

		Print(DZBBC_LOG_PREFIX + DZBBC_Utils.Localize(title) + ": " + DZBBC_Utils.Localize(message));
	}
}

class DZBBC_ClientEffects
{
	protected static ref array<Particle> s_CrashParticles;

	static void OnFXRPC(ParamsReadContext ctx)
	{
		int fx;
		vector pos;

		if (!ctx.Read(fx))
			return;
		if (!ctx.Read(pos))
			return;

		Print(DZBBC_LOG_PREFIX + "ClientFX " + fx.ToString() + " at " + pos.ToString());

		if (fx == DZBBC_FX_CLEAR)
		{
			ClearCrashParticles();
			return;
		}

		if (fx == DZBBC_FX_CRASHSITE)
		{
			PlayCrashsiteFX(pos);
			return;
		}

		if (fx == DZBBC_FX_AUDIO_FRAGMENT || fx == DZBBC_FX_SIGNAL_WARNING || fx == DZBBC_FX_FINAL_WAVE)
		{
			PlaySmokePulse(pos, ParticleList.GRENADE_M18_WHITE_LOOP);
			return;
		}

		if (fx == DZBBC_FX_REWARD)
			PlaySmokePulse(pos, ParticleList.GRENADE_M18_GREEN_LOOP);
	}

	protected static void PlayCrashsiteFX(vector pos)
	{
		ClearCrashParticles();
		EnsureParticleStore();

		PlayPersistentParticle(ParticleList.GRENADE_M18_WHITE_LOOP, pos + Vector(4.0, 0.0, 4.0));
		PlayPersistentParticle(ParticleList.GRENADE_M18_WHITE_LOOP, pos + Vector(-7.0, 0.0, 7.0));
		PlayPersistentParticle(ParticleList.GRENADE_M18_WHITE_LOOP, pos + Vector(14.0, 0.0, -5.0));
		PlayPersistentParticle(ParticleList.BARREL_NORMAL_SMOKE, pos + Vector(8.0, 0.0, 10.0));
		PlayPersistentParticle(ParticleList.CAMP_NORMAL_SMOKE, pos + Vector(-11.0, 0.0, 8.0));
		PlaySmokePulse(pos, ParticleList.GRENADE_M18_WHITE_START);
	}

	protected static void PlaySmokePulse(vector pos, int particleId)
	{
		if (!GetGame() || GetGame().IsDedicatedServer())
			return;

		Particle particle = ParticleManager.GetInstance().PlayInWorld(particleId, DZBBC_Utils.Grounded(pos));
		if (particle)
			particle.SetWiggle(6, 0.25);
	}

	protected static void PlayPersistentParticle(int particleId, vector pos)
	{
		if (!GetGame() || GetGame().IsDedicatedServer())
			return;

		EnsureParticleStore();
		Particle particle = ParticleManager.GetInstance().PlayInWorld(particleId, DZBBC_Utils.Grounded(pos));
		if (!particle)
			return;

		particle.SetWiggle(8, 0.25);
		s_CrashParticles.Insert(particle);
	}

	protected static void ClearCrashParticles()
	{
		if (!s_CrashParticles)
			return;

		foreach (Particle particle: s_CrashParticles)
		{
			if (particle)
				particle.Stop();
		}

		s_CrashParticles.Clear();
	}

	protected static void EnsureParticleStore()
	{
		if (!s_CrashParticles)
			s_CrashParticles = new array<Particle>;
	}
}

class DZBBC_ClientMarkerStore
{
	protected static ref DZBBC_ClientMarkerStore s_Instance;
	protected ref array<ref DZBBC_MarkerData> m_Markers;

	static DZBBC_ClientMarkerStore Get()
	{
		if (!s_Instance)
			s_Instance = new DZBBC_ClientMarkerStore;

		return s_Instance;
	}

	void DZBBC_ClientMarkerStore()
	{
		m_Markers = new array<ref DZBBC_MarkerData>;
	}

	void OnMarkerRPC(ParamsReadContext ctx)
	{
		DZBBC_MarkerData marker = new DZBBC_MarkerData;
		if (!ctx.Read(marker.UID))
			return;
		if (!ctx.Read(marker.Text))
			return;
		if (!ctx.Read(marker.Pos))
			return;
		if (!ctx.Read(marker.Color))
			return;
		if (!ctx.Read(marker.IconPath))
			return;
		if (!ctx.Read(marker.Remove))
			return;

		if (marker.Remove)
			Remove(marker.UID);
		else
			Upsert(marker);
	}

	array<ref DZBBC_MarkerData> GetMarkers()
	{
		return m_Markers;
	}

	protected void Upsert(DZBBC_MarkerData marker)
	{
		for (int i = 0; i < m_Markers.Count(); i++)
		{
			if (m_Markers[i] && m_Markers[i].UID == marker.UID)
			{
				m_Markers[i] = marker;
				return;
			}
		}

		m_Markers.Insert(marker);
	}

	protected void Remove(string uid)
	{
		for (int i = m_Markers.Count() - 1; i >= 0; i--)
		{
			if (m_Markers[i] && m_Markers[i].UID == uid)
				m_Markers.Remove(i);
		}
	}
}
