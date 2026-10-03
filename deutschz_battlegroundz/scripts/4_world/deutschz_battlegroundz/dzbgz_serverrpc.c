class DZBGZ_ServerRPC
{
	static void SendMarker(PlayerBase player, DZBGZ_MarkerData marker)
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
		rpc.Send(player, DZBGZ_RPC_MARKER_SYNC, true, identity);
	}

	static void SendFX(PlayerBase player, int fx, string message, vector pos)
	{
		if (!player || !GetGame())
			return;

		PlayerIdentity identity = player.GetIdentity();
		if (!identity)
			return;

		ScriptRPC rpc = new ScriptRPC;
		rpc.Write(fx);
		rpc.Write(message);
		rpc.Write(pos);
		rpc.Send(player, DZBGZ_RPC_CLIENT_FX, true, identity);
	}

	static void SendNotification(PlayerBase player, string title, string message, float time = 8.0)
	{
		if (!player || !GetGame())
			return;

		PlayerIdentity identity = player.GetIdentity();
		if (!identity)
			return;

#ifdef EXPANSIONMODCORE
		ExpansionNotification(title, message, "Info", ARGB(255, 185, 25, 25), time).Create(identity);
#endif
		GetGame().ChatMP(player, title + ": " + message, "colorAction");
	}

	static void BroadcastNotification(string title, string message, float time = 8.0)
	{
		ref array<PlayerBase> players = DZBGZ_PlayerUtils.GetOnlinePlayers();
		foreach (PlayerBase player: players)
		{
			SendNotification(player, title, message, time);
		}
	}
}
