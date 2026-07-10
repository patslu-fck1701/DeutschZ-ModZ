class DZKOTH_MapMarkerManager
{
	protected string m_CurrentText;
	protected int m_CurrentColor;
	protected vector m_CurrentPos;
	protected string m_CurrentIconName;
	protected bool m_HasMarker;

	void ShowReady(DZKOTH_LocationConfig location)
	{
		if (!location)
			return;

		ShowMarker(GetMarkerName(location), location.GetFlagPosition(), ARGB(255, 255, 153, 0), DZKOTH_Const.MARKER_ICON_PATH, DZKOTH_Const.MARKER_ICON_NAME);
	}

	void ShowCapture(DZKOTH_LocationConfig location)
	{
		if (!location)
			return;

		ShowMarker(GetMarkerName(location) + " - Capture", location.GetFlagPosition(), ARGB(255, 255, 210, 0), DZKOTH_Const.MARKER_ICON_PATH, DZKOTH_Const.MARKER_ICON_NAME);
	}

	void ShowBoss(DZKOTH_LocationConfig location)
	{
		if (!location)
			return;

		ShowMarker("BosZ Zombie freigesetzt", location.GetFlagPosition(), ARGB(255, 255, 64, 64), DZKOTH_Const.MARKER_ICON_PATH, "Skull 3");
	}

	void ShowCompleted(DZKOTH_LocationConfig location)
	{
		if (!location)
			return;

		ShowMarker("KotH abgeschlossen", location.GetFlagPosition(), ARGB(255, 220, 40, 40), DZKOTH_Const.MARKER_ICON_PATH, DZKOTH_Const.MARKER_ICON_NAME);
	}

	void SyncToPlayer(PlayerBase player)
	{
		if (!player || !m_HasMarker)
			return;

		if (DZKOTH_ExpansionBridge.HasServerMarkerSupport())
			return;

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
		DZKOTH_ServerRPC.BroadcastMarker(marker);
		m_HasMarker = false;
	}

	protected void ShowMarker(string text, vector pos, int color, string iconPath, string iconName)
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
		if (!DZKOTH_ExpansionBridge.UpsertMarker(marker))
			DZKOTH_ServerRPC.BroadcastMarker(marker);
		DZKOTH_Utils.Log("Marker active: " + text + " at " + m_CurrentPos.ToString());
	}

	protected string GetMarkerName(DZKOTH_LocationConfig location)
	{
		if (!location)
			return "DeutschZ KotH";

		return "DeutschZ KotH - " + location.Name;
	}
}
