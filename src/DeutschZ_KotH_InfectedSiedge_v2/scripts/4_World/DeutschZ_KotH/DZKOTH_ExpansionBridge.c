class DZKOTH_ExpansionBridge
{
	protected static bool s_UseNotify = true;
	protected static bool s_UseMarkers = true;
	protected static bool s_LoggedNotifyFallback;
	protected static bool s_LoggedMarkerFallback;

	static void Configure(DZKOTH_MainConfig config)
	{
		if (!config)
			return;

		s_UseNotify = config.UseExpansionNotify;
		s_UseMarkers = config.UseExpansionMarkers;
	}

	static bool SendNotification(PlayerBase player, string title, string text, float time = 7.0)
	{
		if (!s_UseNotify || !player || !GetGame())
			return false;

		PlayerIdentity identity = player.GetIdentity();
		if (!identity)
			return false;

#ifdef EXPANSIONMODCORE
		ExpansionNotification(title, text, "Info", ARGB(255, 255, 153, 0), time).Create(identity);
		DZKOTH_Utils.Log("Expansion notify sent");
		return true;
#endif
		LogNotifyFallback();
		return false;
	}

	static bool UpsertMarker(DZKOTH_MarkerData marker)
	{
		if (!s_UseMarkers || !marker || marker.UID == "")
			return false;

#ifdef EXPANSIONMODNAVIGATION
		ExpansionMarkerModule markerModule = ExpansionMarkerModule.GetModuleInstance();
		if (!markerModule)
			return false;

		markerModule.RemoveServerMarker(marker.UID);
		ExpansionMarkerData expansionMarker = markerModule.CreateServerMarker(marker.Text, marker.IconName, marker.Pos, marker.Color, marker.Marker3D, marker.UID);
		if (!expansionMarker)
			return false;

		expansionMarker.SetVisibility(EXPANSION_MARKER_VIS_WORLD | EXPANSION_MARKER_VIS_MAP);
		DZKOTH_Utils.Log("Expansion marker created");
		return true;
#endif
		LogMarkerFallback();
		return false;
	}

	static bool HasServerMarkerSupport()
	{
		if (!s_UseMarkers)
			return false;

#ifdef EXPANSIONMODNAVIGATION
		return ExpansionMarkerModule.GetModuleInstance() != null;
#endif
		return false;
	}

	static void RemoveMarker(string uid)
	{
		if (!s_UseMarkers || uid == "")
			return;

#ifdef EXPANSIONMODNAVIGATION
		ExpansionMarkerModule markerModule = ExpansionMarkerModule.GetModuleInstance();
		if (markerModule)
		{
			markerModule.RemoveServerMarker(uid);
			DZKOTH_Utils.Log("Expansion marker removed");
		}
#endif
	}

	protected static void LogNotifyFallback()
	{
		if (s_LoggedNotifyFallback)
			return;

		s_LoggedNotifyFallback = true;
		DZKOTH_Utils.Warn("Expansion notify unavailable; vanilla RPC/chat fallback used.");
	}

	protected static void LogMarkerFallback()
	{
		if (s_LoggedMarkerFallback)
			return;

		s_LoggedMarkerFallback = true;
		DZKOTH_Utils.Warn("Expansion marker unavailable; vanilla map RPC fallback used.");
	}
}
