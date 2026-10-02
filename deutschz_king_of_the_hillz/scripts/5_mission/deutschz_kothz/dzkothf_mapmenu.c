modded class MapMenu
{
	protected int m_DZKOTHF_MarkerRevision = -1;

	override void OnShow()
	{
		super.OnShow();
		DZKOTHF_RefreshMarker();
	}

	override void Update(float timeslice)
	{
		super.Update(timeslice);
		if (m_DZKOTHF_MarkerRevision != DZKOTHF_ClientState.Revision)
			DZKOTHF_RefreshMarker();
	}

	override void LoadMapMarkers()
	{
		super.LoadMapMarkers();
		DZKOTHF_AddEventMarker();
	}

	protected void DZKOTHF_RefreshMarker()
	{
		m_DZKOTHF_MarkerRevision = DZKOTHF_ClientState.Revision;
		if (!m_MapWidgetInstance)
			return;

		m_MapWidgetInstance.ClearUserMarks();
		LoadMapMarkers();
	}

	protected void DZKOTHF_AddEventMarker()
	{
		if (!m_MapWidgetInstance || !DZKOTHF_ClientState.MarkerVisible)
			return;

		string markerIcon = DZKOTHF_ClientState.MarkerIcon;
		if (markerIcon == "")
			markerIcon = DZKOTHF_Constants.DEFAULT_MARKER_ICON;
		m_MapWidgetInstance.AddUserMark(DZKOTHF_ClientState.MarkerPosition, DZKOTHF_ClientState.MarkerText, ARGB(255, 80, 200, 80), markerIcon);
	}
}
