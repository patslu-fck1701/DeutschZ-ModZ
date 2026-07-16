class DZKOTHF_ExpansionBridge
{
	static const string MARKER_UID = "DZKOTHF_FREE_EVENT_MARKER";
	protected static bool s_ExpansionMarkerActive;

	static bool UpdateMarker(DZKOTHF_Settings settings, bool visible, vector position, string status)
	{
		#ifdef EXPANSIONMODNAVIGATION
		if (!settings || !settings.UseExpansionMarkerWhenAvailable)
			return false;

		ExpansionMarkerModule markerModule = ExpansionMarkerModule.GetModuleInstance();
		if (!markerModule)
			return false;

		markerModule.RemoveServerMarker(MARKER_UID);
		s_ExpansionMarkerActive = false;
		if (visible)
		{
			ExpansionMarkerData marker = markerModule.CreateServerMarker(settings.EventName + " - " + status, settings.ExpansionMarkerIcon, position, ARGB(255, 145, 20, 20), true, MARKER_UID);
			if (!marker)
				return false;
			s_ExpansionMarkerActive = true;
		}

		return true;
		#else
		return false;
		#endif
	}

	static bool IsMarkerPreferred(DZKOTHF_Settings settings)
	{
		#ifdef EXPANSIONMODNAVIGATION
		return settings && settings.UseExpansionMarkerWhenAvailable && s_ExpansionMarkerActive;
		#else
		return false;
		#endif
	}

	static bool SendNotification(PlayerBase player, DZKOTHF_Settings settings, string message)
	{
		#ifdef EXPANSIONMODCORE
		if (!player || !player.GetIdentity() || !settings || !settings.UseExpansionNotificationsWhenAvailable)
			return false;

		ExpansionNotification(settings.EventName, message).Info(player.GetIdentity());
		return true;
		#else
		return false;
		#endif
	}

	static string GetCaptureSideKey(PlayerBase player, DZKOTHF_Settings settings)
	{
		if (!player || !player.GetIdentity())
			return "";

		#ifdef EXPANSIONMODGROUPS
		if (settings && settings.RespectExpansionParties)
		{
			int partyId = player.Expansion_GetPartyID();
			if (partyId >= 0)
				return "EXPANSION_PARTY_" + partyId.ToString();
		}
		#endif

		return "PLAYER_" + player.GetIdentity().GetPlainId();
	}
}
