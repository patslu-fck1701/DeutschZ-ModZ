class DZATM_MarkerManager
{
	protected ref set<string> m_Markers;

	void DZATM_MarkerManager()
	{
		m_Markers = new set<string>;
	}

	void Create(string uid, string label, vector position)
	{
		if (uid == "")
			return;
#ifdef EXPANSIONMODNAVIGATION
		ExpansionMarkerModule markerModule = ExpansionMarkerModule.GetModuleInstance();
		if (!markerModule)
		{
			DZATM_Log.Warn("Expansion MarkerModule ist nicht initialisiert.");
			return;
		}
		markerModule.RemoveServerMarker(uid);
		ExpansionMarkerData marker = markerModule.CreateServerMarker(label, DZATM_Const.MARKER_ICON, position, DZATM_Const.MARKER_COLOR, true, uid);
		if (!marker)
		{
			DZATM_Log.Warn("Expansion-Marker konnte nicht erstellt werden: " + uid);
			return;
		}
		marker.SetVisibility(EXPANSION_MARKER_VIS_WORLD | EXPANSION_MARKER_VIS_MAP);
		m_Markers.Insert(uid);
#else
		DZATM_Log.Warn("EXPANSIONMODNAVIGATION fehlt; ATM-Raid-Marker deaktiviert.");
#endif
	}

	void Remove(string uid)
	{
		if (uid == "")
			return;
#ifdef EXPANSIONMODNAVIGATION
		ExpansionMarkerModule markerModule = ExpansionMarkerModule.GetModuleInstance();
		if (markerModule)
			markerModule.RemoveServerMarker(uid);
#endif
		int index = m_Markers.Find(uid);
		if (index >= 0)
			m_Markers.Remove(index);
	}

	void RemoveAll()
	{
		array<string> markerIds = new array<string>;
		foreach (string markerId: m_Markers)
			markerIds.Insert(markerId);
		foreach (string uid: markerIds)
			Remove(uid);
	}
}
