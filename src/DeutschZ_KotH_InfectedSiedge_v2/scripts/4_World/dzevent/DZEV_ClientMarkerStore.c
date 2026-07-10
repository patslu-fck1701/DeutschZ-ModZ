class DZEV_ClientMarkerStore
{
	private static ref DZEV_ClientMarkerStore s_Instance;
	protected ref array<ref DZEV_MarkerData> m_Markers;

	static DZEV_ClientMarkerStore Get()
	{
		if (!s_Instance)
			s_Instance = new DZEV_ClientMarkerStore;

		return s_Instance;
	}

	void DZEV_ClientMarkerStore()
	{
		m_Markers = new array<ref DZEV_MarkerData>;
	}

	void OnMarkerRPC(ParamsReadContext ctx)
	{
		DZEV_MarkerData marker = new DZEV_MarkerData;
		if (!ctx.Read(marker.UID))
			return;
		if (!ctx.Read(marker.Text))
			return;
		if (!ctx.Read(marker.Pos))
			return;
		if (!ctx.Read(marker.Color))
			return;
		if (!ctx.Read(marker.IconName))
			return;
		if (!ctx.Read(marker.Marker3D))
			return;
		if (!ctx.Read(marker.Remove))
			return;

		if (marker.Remove)
			Remove(marker.UID);
		else
			Upsert(marker);

		DZEV_EventBus.OnClientMarker().Invoke(marker);
	}

	void OnProgressRPC(ParamsReadContext ctx)
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

		DZEV_ClientProgressState.Set(mode, label, current, max);
		DZEV_EventBus.OnClientProgress().Invoke(mode, label, current, max);
	}

	void OnFXRPC(ParamsReadContext ctx)
	{
		int fx;
		vector pos;

		if (!ctx.Read(fx))
			return;
		if (!ctx.Read(pos))
			return;

		if (fx == DZEV_FXIds.MUSIC_START)
			DZEV_ClientProgressHud.Get().PlayMusic(pos);
		else if (fx == DZEV_FXIds.FIREWORKS)
			DZEV_ClientProgressHud.Get().PlayFireworks(pos);
		else if (fx == DZEV_FXIds.BOSS_ALARM)
			DZEV_ClientProgressHud.Get().PlayBossAlarm(pos);
		else if (fx == DZEV_FXIds.MUSIC_STOP)
			DZEV_ClientProgressHud.Get().StopMusic();
	}

	void RenderVanilla(MapWidget mapWidget)
	{
		if (!mapWidget)
			return;

		foreach (DZEV_MarkerData marker: m_Markers)
		{
			if (marker)
				mapWidget.AddUserMark(marker.Pos, marker.Text, marker.Color, DZKOTH_Const.MARKER_ICON_PATH);
		}
	}

	protected void Upsert(DZEV_MarkerData marker)
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
