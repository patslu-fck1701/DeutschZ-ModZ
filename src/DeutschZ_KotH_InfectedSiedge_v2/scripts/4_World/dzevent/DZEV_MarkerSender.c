class DZEV_MarkerSender
{
	static void SendMarker(PlayerBase player, DZEV_MarkerData marker)
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
		rpc.Write(marker.IconName);
		rpc.Write(marker.Marker3D);
		rpc.Write(marker.Remove);
		rpc.Send(player, DZEV_RPCIds.MARKER_SYNC, true, identity);
	}

	static void SendProgress(PlayerBase player, int mode, string label, float current, float max)
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
		rpc.Send(player, DZEV_RPCIds.PROGRESS_SYNC, true, identity);
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
		rpc.Send(player, DZEV_RPCIds.EVENT_FX, true, identity);
	}

	static void Notify(PlayerBase player, string title, string text, int color = 0, float time = 7.0)
	{
		if (!player)
			return;

		PlayerIdentity identity = player.GetIdentity();
		if (!identity)
			return;

		Print("[DZEV] Notify fallback for " + identity.GetName() + ": " + title + " - " + text);
	}
}
