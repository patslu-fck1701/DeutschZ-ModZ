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

	protected void DZKOTHF_RefreshMarker()
	{
		m_DZKOTHF_MarkerRevision = DZKOTHF_ClientState.Revision;
		if (!m_MapWidgetInstance)
			return;

		m_MapWidgetInstance.ClearUserMarks();
		if (m_Map)
			LoadMapMarkers();

		if (DZKOTHF_ClientState.MarkerVisible)
			m_MapWidgetInstance.AddUserMark(DZKOTHF_ClientState.MarkerPosition, DZKOTHF_ClientState.MarkerText, ARGB(255, 80, 200, 80), DZKOTHF_ClientState.MarkerIcon);
	}
}
