class DZBBC_MapMenuRenderer
{
	static void RenderVanilla(MapWidget mapWidget)
	{
		if (!mapWidget)
			return;

		array<ref DZBBC_MarkerData> markers = DZBBC_ClientMarkerStore.Get().GetMarkers();
		foreach (DZBBC_MarkerData marker: markers)
		{
			if (!marker)
				continue;

			string iconPath = marker.IconPath;
			if (iconPath == "")
				iconPath = DZBBC_DEFAULT_MARKER_ICON;

			mapWidget.AddUserMark(marker.Pos, DZBBC_Utils.Localize(marker.Text), marker.Color, iconPath);
		}
	}
}

modded class MapMenu
{
	override void OnShow()
	{
		super.OnShow();
		DZBBC_MapMenuRenderer.RenderVanilla(m_MapWidgetInstance);
	}
}
