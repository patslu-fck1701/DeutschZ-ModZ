class DZKOTHF_ClientBridge
{
	static void BroadcastNotify(DZKOTHF_Settings settings, string message)
	{
		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		foreach (Man man: players)
		{
			PlayerBase player = PlayerBase.Cast(man);
			SendNotify(player, settings, message);
		}
	}

	static void SendNotify(PlayerBase player, DZKOTHF_Settings settings, string message)
	{
		if (!player || !player.GetIdentity() || !settings)
			return;

		if (DZKOTHF_ExpansionBridge.SendNotification(player, settings, message))
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
		bool expansionHandled = DZKOTHF_ExpansionBridge.UpdateMarker(settings, visible, position, status);
		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		foreach (Man man: players)
		{
			PlayerBase player = PlayerBase.Cast(man);
			SendMarker(player, settings, visible && !expansionHandled, position, status);
		}
	}

	static void SendMarker(PlayerBase player, DZKOTHF_Settings settings, bool visible, vector position, string status)
	{
		if (!player || !player.GetIdentity() || !settings)
			return;

		if (DZKOTHF_ExpansionBridge.IsMarkerPreferred(settings))
			visible = false;

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
		{
			PlayerBase player = PlayerBase.Cast(man);
			SendProgress(player, settings, visible, status, progress);
		}
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

	static void SendMusic(PlayerBase player, DZKOTHF_Settings settings, bool play, string soundSet)
	{
		if (!player || !player.GetIdentity() || !settings)
			return;

		ScriptRPC rpc = new ScriptRPC;
		rpc.Write(DZKOTHF_Constants.RPC_PROTOCOL_VERSION);
		rpc.Write(DZKOTHF_RPCMessage.MUSIC);
		rpc.Write(play);
		rpc.Write(soundSet);
		rpc.Write(Math.Clamp(settings.MusicVolume, 0.0, 0.45));
		rpc.Send(player, DZKOTHF_Constants.RPC_SYNC, true, player.GetIdentity());
	}
}
