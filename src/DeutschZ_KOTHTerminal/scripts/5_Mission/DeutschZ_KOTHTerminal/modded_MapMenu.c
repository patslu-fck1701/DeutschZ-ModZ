class DZKOTHG_MapMenuRenderer
{
	static void Render(MapWidget mapWidget)
	{
		if (!mapWidget)
			return;

		array<ref DZKOTHG_MarkerData> markers = DZKOTHG_ClientMarkerStore.Get().GetMarkers();
		foreach (DZKOTHG_MarkerData marker: markers)
		{
			if (!marker)
				continue;

			string iconPath = marker.IconPath;
			if (iconPath == "")
				iconPath = "dz/gear/navigation/data/map_transmitter_ca.paa";

			mapWidget.AddUserMark(marker.Pos, marker.Text, marker.Color, iconPath);
		}
	}
}

modded class MapMenu
{
	override void OnShow()
	{
		super.OnShow();
		DZKOTHG_MapMenuRenderer.Render(m_MapWidgetInstance);
	}
}
