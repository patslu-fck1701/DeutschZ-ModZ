class DZKOTHF_ClientBridge
{
	static void BroadcastNotify(DZKOTHF_Settings settings, string message)
	{
		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		foreach (Man man: players)
			SendNotify(PlayerBase.Cast(man), settings, message);
	}

	static void SendNotify(PlayerBase player, DZKOTHF_Settings settings, string message)
	{
		if (!player || !player.GetIdentity() || !settings)
			return;

		if (settings.NotifyUseChatFallback)
		{
			GetGame().ChatMP(player, message, "colorAction");
			return;
		}

		ScriptRPC rpc = new ScriptRPC;
		rpc.Write(DZKOTHF_Constants.RPC_PROTOCOL_VERSION);
		rpc.Write(DZKOTHF_RPCMessage.NOTIFY);
		rpc.Write(settings.EventName);
		rpc.Write(message);
		rpc.Send(player, DZKOTHF_Constants.RPC_SYNC, true, player.GetIdentity());
	}

	static void BroadcastMarker(DZKOTHF_Settings settings, bool visible, vector position, string status)
	{
		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		foreach (Man man: players)
			SendMarker(PlayerBase.Cast(man), settings, visible, position, status);
	}

	static void SendMarker(PlayerBase player, DZKOTHF_Settings settings, bool visible, vector position, string status)
	{
		if (!player || !player.GetIdentity() || !settings)
			return;

		ScriptRPC rpc = new ScriptRPC;
		rpc.Write(DZKOTHF_Constants.RPC_PROTOCOL_VERSION);
		rpc.Write(DZKOTHF_RPCMessage.MARKER);
		rpc.Write(visible);
		rpc.Write(position);
		rpc.Write(settings.EventName + " - " + status);
		rpc.Write(settings.MarkerIcon);
		rpc.Send(player, DZKOTHF_Constants.RPC_SYNC, true, player.GetIdentity());
	}

	static void BroadcastProgress(DZKOTHF_Settings settings, bool visible, string status, float progress)
	{
		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		foreach (Man man: players)
			SendProgress(PlayerBase.Cast(man), settings, visible, status, progress);
	}

	static void SendProgress(PlayerBase player, DZKOTHF_Settings settings, bool visible, string status, float progress)
	{
		if (!player || !player.GetIdentity() || !settings)
			return;

		ScriptRPC rpc = new ScriptRPC;
		rpc.Write(DZKOTHF_Constants.RPC_PROTOCOL_VERSION);
		rpc.Write(DZKOTHF_RPCMessage.PROGRESS);
		rpc.Write(visible);
		rpc.Write(settings.EventName);
		rpc.Write(status);
		rpc.Write(Math.Clamp(progress, 0.0, 1.0));
		rpc.Send(player, DZKOTHF_Constants.RPC_SYNC, true, player.GetIdentity());
	}
}
