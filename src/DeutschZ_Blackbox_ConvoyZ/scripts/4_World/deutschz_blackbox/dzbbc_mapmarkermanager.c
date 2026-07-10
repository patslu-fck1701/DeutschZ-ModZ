class DZBBC_ServerRPC
{
	static void SendMarker(PlayerBase player, DZBBC_MarkerData marker)
	{
		if (!player || !marker || !GetGame())
			return;

		PlayerIdentity identity = player.GetIdentity();
		if (!identity)
			return;

		ScriptRPC rpc = new ScriptRPC;
		rpc.Write(marker.UID);
		rpc.Write(marker.Text);
		rpc.Write(marker.Pos);
		rpc.Write(marker.Color);
		rpc.Write(marker.IconPath);
		rpc.Write(marker.Remove);
		rpc.Send(player, DZBBC_RPC_MARKER_SYNC, true, identity);
	}

	static void BroadcastMarker(DZBBC_MarkerData marker)
	{
		ref array<PlayerBase> players = DZBBC_PlayerUtils.GetOnlinePlayers();
		foreach (PlayerBase player: players)
		{
			SendMarker(player, marker);
		}
	}

	static void SendHud(PlayerBase player, int mode, string label, float current, float max)
	{
		if (!player || !GetGame())
			return;

		PlayerIdentity identity = player.GetIdentity();
		if (!identity)
			return;

		ScriptRPC rpc = new ScriptRPC;
		rpc.Write(mode);
		rpc.Write(label);
		rpc.Write(current);
		rpc.Write(max);
		rpc.Send(player, DZBBC_RPC_HUD_SYNC, true, identity);
	}

	static void BroadcastHud(int mode, string label, float current, float max)
	{
		ref array<PlayerBase> players = DZBBC_PlayerUtils.GetOnlinePlayers();
		foreach (PlayerBase player: players)
		{
			SendHud(player, mode, label, current, max);
		}
	}

	static void SendNotification(PlayerBase player, string title, string message, float time = 7.0)
	{
		if (!player || !GetGame())
			return;

		PlayerIdentity identity = player.GetIdentity();
		if (!identity)
			return;

		title = DZBBC_Utils.Localize(title);
		message = DZBBC_Utils.Localize(message);

		if (DZBBC_ExpansionBridge.SendNotification(player, title, message, time))
			return;

		ScriptRPC rpc = new ScriptRPC;
		rpc.Write(title);
		rpc.Write(message);
		rpc.Write(time);
		rpc.Send(player, DZBBC_RPC_NOTIFICATION, true, identity);
		GetGame().ChatMP(player, title + ": " + message, "colorAction");
	}

	static void BroadcastNotification(string title, string message, float time = 7.0)
	{
		ref array<PlayerBase> players = DZBBC_PlayerUtils.GetOnlinePlayers();
		foreach (PlayerBase player: players)
		{
			SendNotification(player, title, message, time);
		}
	}

	static void SendFX(PlayerBase player, int fx, vector pos)
	{
		if (!player || !GetGame())
			return;

		PlayerIdentity identity = player.GetIdentity();
		if (!identity)
			return;

		ScriptRPC rpc = new ScriptRPC;
		rpc.Write(fx);
		rpc.Write(pos);
		rpc.Send(player, DZBBC_RPC_CLIENT_FX, true, identity);
	}

	static void BroadcastFX(int fx, vector pos)
	{
		ref array<PlayerBase> players = DZBBC_PlayerUtils.GetOnlinePlayers();
		foreach (PlayerBase player: players)
		{
			SendFX(player, fx, pos);
		}
	}
}

class DZBBC_MapMarkerManager
{
	protected ref array<ref DZBBC_MarkerData> m_Markers;

	void DZBBC_MapMarkerManager()
	{
		m_Markers = new array<ref DZBBC_MarkerData>;
	}

	void ShowCrashsite(DZBBC_CrashSiteConfig site)
	{
		if (!site)
			return;

		ShowMarker(DZBBC_CRASHSITE_MARKER_UID, "ConvoyZ Crashsite - " + site.Name, site.GetPosition(), ARGB(255, 220, 20, 20), DZBBC_DEFAULT_MARKER_ICON);
	}

	void ShowTerminal(DZBBC_TerminalLocationConfig terminal)
	{
		if (!terminal)
			return;

		ShowMarker(DZBBC_TERMINAL_MARKER_UID, "ConvoyZ Terminal - " + terminal.Name, terminal.GetPosition(), ARGB(255, 220, 20, 20), DZBBC_DEFAULT_MARKER_ICON);
	}

	void ShowTransportHint(string text, vector sectorCenter)
	{
		ShowMarker(DZBBC_TRANSPORT_MARKER_UID, text, sectorCenter, ARGB(220, 220, 20, 20), DZBBC_DEFAULT_MARKER_ICON);
	}

	void Remove(string uid)
	{
		DZBBC_MarkerData marker = new DZBBC_MarkerData;
		marker.UID = uid;
		marker.Remove = true;
		DZBBC_ExpansionBridge.RemoveServerMarker(uid);
		DZBBC_ServerRPC.BroadcastMarker(marker);
		RemoveLocal(uid);
	}

	void RemoveAll()
	{
		Remove(DZBBC_CRASHSITE_MARKER_UID);
		Remove(DZBBC_TERMINAL_MARKER_UID);
		Remove(DZBBC_TRANSPORT_MARKER_UID);
	}

	void SyncToPlayer(PlayerBase player)
	{
		if (!player)
			return;

		if (DZBBC_ExpansionBridge.HasServerMarkers())
			return;

		foreach (DZBBC_MarkerData marker: m_Markers)
		{
			if (marker)
				DZBBC_ServerRPC.SendMarker(player, marker);
		}
	}

	protected void ShowMarker(string uid, string text, vector pos, int color, string iconPath)
	{
		DZBBC_MarkerData marker = new DZBBC_MarkerData;
		marker.UID = uid;
		marker.Text = text;
		marker.Pos = DZBBC_Utils.Grounded(pos);
		marker.Color = color;
		marker.IconPath = iconPath;
		Upsert(marker);
		if (!DZBBC_ExpansionBridge.UpsertServerMarker(marker))
			DZBBC_ServerRPC.BroadcastMarker(marker);
	}

	protected void Upsert(DZBBC_MarkerData marker)
	{
		for (int i = 0; i < m_Markers.Count(); i++)
		{
			if (m_Markers[i] && m_Markers[i].UID == marker.UID)
			{
				m_Markers[i] = marker;
				return;
			}
		}

		m_Markers.Insert(marker);
	}

	protected void RemoveLocal(string uid)
	{
		for (int i = m_Markers.Count() - 1; i >= 0; i--)
		{
			if (m_Markers[i] && m_Markers[i].UID == uid)
				m_Markers.Remove(i);
		}
	}
}
