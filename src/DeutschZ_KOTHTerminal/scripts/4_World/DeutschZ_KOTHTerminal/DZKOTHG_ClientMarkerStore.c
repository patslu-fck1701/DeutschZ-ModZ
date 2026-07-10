class DZKOTHG_ClientMarkerStore
{
	protected static ref DZKOTHG_ClientMarkerStore s_Instance;
	protected ref array<ref DZKOTHG_MarkerData> m_Markers;

	void DZKOTHG_ClientMarkerStore()
	{
		m_Markers = new array<ref DZKOTHG_MarkerData>;
	}

	static DZKOTHG_ClientMarkerStore Get()
	{
		if (!s_Instance)
			s_Instance = new DZKOTHG_ClientMarkerStore();

		return s_Instance;
	}

	array<ref DZKOTHG_MarkerData> GetMarkers()
	{
		return m_Markers;
	}

	void OnMarkerRPC(ParamsReadContext ctx)
	{
		string uid;
		string text;
		vector pos;
		int color;
		string iconPath;
		bool remove;

		if (!ctx.Read(uid) || !ctx.Read(text) || !ctx.Read(pos) || !ctx.Read(color) || !ctx.Read(iconPath) || !ctx.Read(remove))
		{
			Print("[DZKOTHG] Invalid marker RPC payload");
			return;
		}

		RemoveMarker(uid);
		if (remove)
			return;

		DZKOTHG_MarkerData marker = new DZKOTHG_MarkerData();
		marker.UID = uid;
		marker.Text = text;
		marker.Pos = pos;
		marker.Color = color;
		marker.IconPath = iconPath;
		marker.Remove = false;
		m_Markers.Insert(marker);
	}

	protected void RemoveMarker(string uid)
	{
		for (int i = m_Markers.Count() - 1; i >= 0; i--)
		{
			if (m_Markers[i] && m_Markers[i].UID == uid)
				m_Markers.Remove(i);
		}
	}
}
