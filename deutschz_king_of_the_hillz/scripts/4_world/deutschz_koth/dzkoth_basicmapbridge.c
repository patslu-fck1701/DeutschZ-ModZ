class DZKOTH_BasicMapBridge
{
	protected static const string GROUP_ID = "DeutschZ_KotHZ";
	protected static bool s_MarkerActive;
	#ifdef BASICMAP
	protected static ref array<ref BasicMapMarker> s_Markers;
	#endif

	static void Init()
	{
#ifdef BASICMAP
		EnsureRegistered();
#endif
	}

	static bool UpdateMarker(DZKOTHF_Settings settings, bool visible, vector position, string text, string iconPath)
	{
#ifdef BASICMAP
		if (!settings || !settings.UseBasicMapMarkerWhenAvailable || !BasicMap())
			return false;

		EnsureRegistered();
		s_Markers = new array<ref BasicMapMarker>;
		if (visible)
		{
			array<int> color = {255, 153, 0};
			BasicMapMarker marker = new BasicMapMarker(text, position, iconPath, color, 255, true);
			marker.SetCanEdit(false);
			s_Markers.Insert(marker);
		}

		BasicMap().SetMarkers(GROUP_ID, s_Markers);
		BasicMap().SetMarkersRemote(GROUP_ID, s_Markers);
		s_MarkerActive = visible;
		if (visible)
			DZKOTH_Utils.Log("BasicMap marker created");
		else
			DZKOTH_Utils.Log("BasicMap marker removed");
		return true;
#endif
		return false;
	}

	static bool IsMarkerActive(DZKOTHF_Settings settings)
	{
#ifdef BASICMAP
		return settings && settings.UseBasicMapMarkerWhenAvailable && s_MarkerActive;
#endif
		return false;
	}

	static void SyncToPlayer(DZKOTHF_Settings settings, PlayerBase player)
	{
#ifdef BASICMAP
		if (!settings || !settings.UseBasicMapMarkerWhenAvailable || !s_MarkerActive || !s_Markers || !player || !player.GetIdentity() || !BasicMap())
			return;

		EnsureRegistered();
		BasicMap().SetMarkersRemote(GROUP_ID, s_Markers, player.GetIdentity());
#endif
	}

	protected static void EnsureRegistered()
	{
#ifdef BASICMAP
		if (BasicMap() && !BasicMap().GetGroup(GROUP_ID))
			BasicMap().RegisterGroup(GROUP_ID, new BasicMapGroupMetaData(GROUP_ID, "DeutschZ KotHZ", false), new BasicMapMarkerFactory);
#endif
	}
}
