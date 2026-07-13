class DZBGZ_ClientMarkerStore
{
	protected static ref DZBGZ_ClientMarkerStore s_Instance;
	protected ref array<ref DZBGZ_MarkerData> m_Markers;

	static DZBGZ_ClientMarkerStore Get()
	{
		if (!s_Instance)
			s_Instance = new DZBGZ_ClientMarkerStore;

		return s_Instance;
	}

	void DZBGZ_ClientMarkerStore()
	{
		m_Markers = new array<ref DZBGZ_MarkerData>;
	}

	void OnMarkerRPC(ParamsReadContext ctx)
	{
		DZBGZ_MarkerData marker = new DZBGZ_MarkerData;
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

	array<ref DZBGZ_MarkerData> GetMarkers()
	{
		return m_Markers;
	}

	protected void Upsert(DZBGZ_MarkerData marker)
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

class DZBGZ_ClientFX
{
	protected static Widget s_Root;
	protected static TextWidget s_Text;
	protected static int s_HideAt;

	static void OnFXRPC(ParamsReadContext ctx)
	{
		int fx;
		string message;
		vector pos;

		if (!ctx.Read(fx))
			return;
		if (!ctx.Read(message))
			return;
		if (!ctx.Read(pos))
			return;

		if (fx == DZBGZ_FX_CLEAR)
		{
			Hide();
			return;
		}

		ShowGlitch(message, fx);
		PlayParticleForFX(fx, pos);
		Print(DZBGZ_LOG_PREFIX + "ClientFX " + fx.ToString() + " " + message);
	}

	static void Update()
	{
		if (!s_Root || !GetGame())
			return;

		if (s_HideAt > 0 && GetGame().GetTime() >= s_HideAt)
			Hide();
	}

	protected static void ShowGlitch(string message, int fx)
	{
		if (!GetGame() || GetGame().IsDedicatedServer() || !GetGame().GetWorkspace())
			return;

		if (!s_Root)
		{
			s_Root = GetGame().GetWorkspace().CreateWidgets("DeutschZ_BattlegroundZ/gui/layouts/dzbgz_glitch.layout");
			if (s_Root)
				s_Text = TextWidget.Cast(s_Root.FindAnyWidget("DZBGZ_GlitchText"));
		}

		if (!s_Root)
			return;

		if (s_Text)
			s_Text.SetText(message);

		s_Root.Show(true);
		int duration = 2600;
		if (fx == DZBGZ_FX_RIDDLE || fx == DZBGZ_FX_READER_ACCEPTED)
			duration = 6500;
		s_HideAt = GetGame().GetTime() + duration;
	}

	protected static void Hide()
	{
		if (s_Root)
			s_Root.Show(false);

		s_HideAt = 0;
	}

	protected static void PlayParticleForFX(int fx, vector pos)
	{
		if (!GetGame() || GetGame().IsDedicatedServer())
			return;

		int particleId = ParticleList.GRENADE_M18_WHITE_START;
		if (fx == DZBGZ_FX_READER_ACCEPTED)
			particleId = ParticleList.GRENADE_M18_RED_START;

		Particle particle = ParticleManager.GetInstance().PlayInWorld(particleId, DZBGZ_Utils.Grounded(pos));
		if (particle)
			particle.SetWiggle(5, 0.25);
	}
}
