class DZKOTHF_LBmasterBridge
{
	#ifdef LBmaster_Groups
	protected static int s_MarkerUID = -1;
	protected static bool s_MarkerActive;
	#endif

	static bool UpdateMarker(DZKOTHF_Settings settings, bool visible, vector position, string status, float radius = -1.0, int markerColor = 0)
	{
		#ifdef LBmaster_Groups
		if (!settings || !LBStaticMarkerManager.Get)
			return false;

		RemoveActiveMarker();
		if (!settings.UseLBmasterMarkerWhenAvailable)
			return false;

		if (!visible)
			return true;

		string iconPath = settings.LBmasterMarkerIcon;
		if (iconPath == "")
			iconPath = settings.MarkerIcon;

		if (markerColor == 0)
			markerColor = ARGB(255, 145, 20, 20);
		if (radius <= 0.0)
			radius = settings.CaptureRadius;
		LBServerMarker marker = LBStaticMarkerManager.Get.AddTempServerMarker(settings.EventName + " - " + status, position, iconPath, markerColor, false, settings.LBmasterMarkerDisplay3D, settings.LBmasterMarkerDisplayMap, settings.LBmasterMarkerDisplayGPS);

		if (!marker)
		{
			DZKOTHF_Log.Warning("LBmaster marker could not be created; next marker provider remains active.");
			return false;
		}

		marker.SetRadius(radius, markerColor, true);
		s_MarkerUID = marker.uid;
		s_MarkerActive = true;
		DZKOTHF_Log.Info("LBmaster marker created: uid=" + s_MarkerUID.ToString() + ", radius=" + radius.ToString() + ".");
		return true;
		#else
		return false;
		#endif
	}

	static bool IsMarkerPreferred(DZKOTHF_Settings settings)
	{
		#ifdef LBmaster_Groups
		return settings && settings.UseLBmasterMarkerWhenAvailable && s_MarkerActive;
		#else
		return false;
		#endif
	}

	protected static void RemoveActiveMarker()
	{
		#ifdef LBmaster_Groups
		if (s_MarkerUID >= 0 && LBStaticMarkerManager.Get)
			LBStaticMarkerManager.Get.RemoveServerMarker(s_MarkerUID);
		s_MarkerUID = -1;
		s_MarkerActive = false;
		#endif
	}
}
