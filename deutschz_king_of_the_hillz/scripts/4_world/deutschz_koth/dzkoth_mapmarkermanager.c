class DZKOTH_MapMarkerManager
{
	protected string m_CurrentText;
	protected int m_CurrentColor;
	protected vector m_CurrentPos;
	protected string m_CurrentIconName;
	protected bool m_HasMarker;
	protected int m_ActiveProvider;

	void ShowReady(DZKOTH_LocationConfig location)
	{
		if (!location)
			return;

		ShowMarker(GetMarkerName(location), location.GetFlagPosition(), ARGB(255, 255, 153, 0), DZKOTH_Const.MARKER_ICON_PATH, DZKOTH_Const.MARKER_ICON_NAME, location.Radius);
	}

	void ShowCapture(DZKOTH_LocationConfig location)
	{
		if (!location)
			return;

		ShowMarker(GetMarkerName(location) + " - Capture", location.GetFlagPosition(), ARGB(255, 255, 210, 0), DZKOTH_Const.MARKER_ICON_PATH, DZKOTH_Const.MARKER_ICON_NAME, location.Radius);
	}

	void ShowBoss(DZKOTH_LocationConfig location)
	{
		if (!location)
			return;

		ShowMarker("BosZ Zombie freigesetzt", location.GetFlagPosition(), ARGB(255, 255, 64, 64), DZKOTH_Const.MARKER_ICON_PATH, "Skull 3", location.Radius);
	}

	void ShowCompleted(DZKOTH_LocationConfig location)
	{
		if (!location)
			return;

		ShowMarker("KotH abgeschlossen", location.GetFlagPosition(), ARGB(255, 220, 40, 40), DZKOTH_Const.MARKER_ICON_PATH, DZKOTH_Const.MARKER_ICON_NAME, location.Radius);
	}

	void SyncToPlayer(PlayerBase player)
	{
		if (!player || !m_HasMarker)
			return;

		if (m_ActiveProvider == 1)
			return;

		DZKOTHF_Settings settings = DZKOTHF_SettingsLoader.Load();
		if (m_ActiveProvider == 2 && DZKOTHF_LBmasterBridge.IsMarkerPreferred(settings))
			return;

		if (m_ActiveProvider == 3 && DZKOTH_BasicMapBridge.IsMarkerActive(settings))
		{
			DZKOTH_BasicMapBridge.SyncToPlayer(settings, player);
			return;
		}

		DZKOTH_MarkerData marker = new DZKOTH_MarkerData;
		marker.UID = DZKOTH_Const.MAIN_MARKER_UID;
		marker.Text = m_CurrentText;
		marker.Pos = m_CurrentPos;
		marker.Color = m_CurrentColor;
		marker.IconName = m_CurrentIconName;
		DZKOTH_ServerRPC.SendMarker(player, marker);
	}

	void Remove()
	{
		DZKOTH_MarkerData marker = new DZKOTH_MarkerData;
		marker.UID = DZKOTH_Const.MAIN_MARKER_UID;
		marker.Remove = true;
		DZKOTH_ExpansionBridge.RemoveMarker(DZKOTH_Const.MAIN_MARKER_UID);
		DZKOTHF_Settings settings = DZKOTHF_SettingsLoader.Load();
		DZKOTHF_LBmasterBridge.UpdateMarker(settings, false, vector.Zero, "");
		DZKOTH_BasicMapBridge.UpdateMarker(settings, false, vector.Zero, "", "");
		DZKOTH_ServerRPC.BroadcastMarker(marker);
		m_HasMarker = false;
		m_ActiveProvider = 0;
	}

	protected void ShowMarker(string text, vector pos, int color, string iconPath, string iconName, float radius)
	{
		m_CurrentText = text;
		m_CurrentColor = color;
		m_CurrentPos = DZKOTH_Utils.Grounded(pos);
		m_CurrentIconName = iconName;
		m_HasMarker = true;

		DZKOTH_MarkerData marker = new DZKOTH_MarkerData;
		marker.UID = DZKOTH_Const.MAIN_MARKER_UID;
		marker.Text = text;
		marker.Pos = m_CurrentPos;
		marker.Color = color;
		marker.IconPath = iconPath;
		marker.IconName = iconName;
		marker.Marker3D = true;
		DZKOTHF_Settings settings = DZKOTHF_SettingsLoader.Load();
		bool expansionHandled = DZKOTH_ExpansionBridge.UpsertMarker(marker);
		bool lbmasterHandled = false;
		bool basicMapHandled = false;
		if (expansionHandled)
		{
			DZKOTHF_LBmasterBridge.UpdateMarker(settings, false, m_CurrentPos, text);
			DZKOTH_BasicMapBridge.UpdateMarker(settings, false, vector.Zero, "", "");
		}
		else
		{
			lbmasterHandled = DZKOTHF_LBmasterBridge.UpdateMarker(settings, true, m_CurrentPos, text, radius, color);
			if (lbmasterHandled)
				DZKOTH_BasicMapBridge.UpdateMarker(settings, false, vector.Zero, "", "");
			else
				basicMapHandled = DZKOTH_BasicMapBridge.UpdateMarker(settings, true, m_CurrentPos, text, iconPath);
		}
		if (expansionHandled)
			m_ActiveProvider = 1;
		else if (lbmasterHandled)
			m_ActiveProvider = 2;
		else if (basicMapHandled)
			m_ActiveProvider = 3;
		else
			m_ActiveProvider = 4;

		if (m_ActiveProvider == 4)
			DZKOTH_ServerRPC.BroadcastMarker(marker);
		else
		{
			DZKOTH_MarkerData removeVanilla = new DZKOTH_MarkerData;
			removeVanilla.UID = DZKOTH_Const.MAIN_MARKER_UID;
			removeVanilla.Remove = true;
			DZKOTH_ServerRPC.BroadcastMarker(removeVanilla);
		}
		DZKOTH_Utils.Log("Marker active: " + text + " at " + m_CurrentPos.ToString());
	}

	protected string GetMarkerName(DZKOTH_LocationConfig location)
	{
		if (!location)
			return "DeutschZ KotH";

		return "DeutschZ KotH - " + location.Name;
	}
}
