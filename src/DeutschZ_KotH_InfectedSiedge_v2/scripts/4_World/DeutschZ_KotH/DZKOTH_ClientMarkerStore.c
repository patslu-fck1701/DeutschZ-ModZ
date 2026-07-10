class DZKOTH_ClientMarkerStore
{
	private static ref DZKOTH_ClientMarkerStore s_Instance;
	protected ref array<ref DZKOTH_MarkerData> m_Markers;

	static DZKOTH_ClientMarkerStore Get()
	{
		if (!s_Instance)
			s_Instance = new DZKOTH_ClientMarkerStore;

		return s_Instance;
	}

	void DZKOTH_ClientMarkerStore()
	{
		m_Markers = new array<ref DZKOTH_MarkerData>;
	}

	void OnMarkerRPC(ParamsReadContext ctx)
	{
		DZKOTH_MarkerData marker = new DZKOTH_MarkerData;
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

		ApplyExpansionMarker(marker);
	}

	void RenderVanilla(MapWidget mapWidget)
	{
#ifdef EXPANSIONMODNAVIGATION
		return;
#endif

		if (!mapWidget)
			return;

		foreach (DZKOTH_MarkerData marker: m_Markers)
		{
			if (!marker)
				continue;

			string iconPath = marker.IconPath;
			if (iconPath == "")
				iconPath = DZKOTH_Const.MARKER_ICON_PATH;

			mapWidget.AddUserMark(marker.Pos, marker.Text, marker.Color, iconPath);
		}
	}

	protected void Upsert(DZKOTH_MarkerData marker)
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

	protected void ApplyExpansionMarker(DZKOTH_MarkerData marker)
	{
#ifdef EXPANSIONMODNAVIGATION
		if (!marker)
			return;

		ExpansionMarkerModule module = ExpansionMarkerModule.GetModuleInstance();
		if (!module || !module.GetData())
			return;

		module.RemovePersonalMarkerByUID(marker.UID);
		if (marker.Remove)
			return;

		ExpansionMarkerData data = ExpansionMarkerData.Create(ExpansionMapMarkerType.PERSONAL, marker.UID, false);
		if (!data)
			return;

		string iconName = marker.IconName;
		if (iconName == "")
			iconName = "Flag";

		data.SetName(marker.Text);
		data.SetPosition(marker.Pos);
		data.SetColor(marker.Color);
		data.SetIconName(iconName);
		data.Set3D(marker.Marker3D);
		data.SetLockState(true);
		module.CreateMarker(data);
#endif
	}
}
