class DZKOTHG_ServerRPC
{
	static void SendMarker(PlayerBase player, DZKOTHG_MarkerData marker)
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
		rpc.Send(player, DZKOTHG_RPC_MARKER_SYNC, true, identity);
	}

	static void BroadcastMarker(DZKOTHG_MarkerData marker)
	{
		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);

		foreach (Man man: players)
		{
			PlayerBase player = PlayerBase.Cast(man);
			if (player)
				SendMarker(player, marker);
		}
	}
}
