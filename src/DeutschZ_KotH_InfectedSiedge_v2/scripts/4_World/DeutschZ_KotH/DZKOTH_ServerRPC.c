class DZKOTH_ServerRPC
{
	static void SendMarker(PlayerBase player, DZKOTH_MarkerData marker)
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
		rpc.Write(marker.IconName);
		rpc.Write(marker.Marker3D);
		rpc.Write(marker.Remove);
		rpc.Send(player, DZKOTH_RPCIds.MARKER, true, identity);
	}

	static void BroadcastMarker(DZKOTH_MarkerData marker)
	{
		if (!marker || !GetGame())
			return;

		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);

		foreach (Man man: players)
		{
			PlayerBase player = PlayerBase.Cast(man);
			if (player)
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
		rpc.Send(player, DZKOTH_RPCIds.CAPTURE_HUD, true, identity);
	}

	static void BroadcastHud(array<PlayerBase> players, int mode, string label, float current, float max)
	{
		if (!players)
			return;

		foreach (PlayerBase player: players)
		{
			if (player)
				SendHud(player, mode, label, current, max);
		}
	}

	static void SendWarning(PlayerBase player, string title, string text, float time = 7.0)
	{
		if (!player || !GetGame())
			return;

		PlayerIdentity identity = player.GetIdentity();
		if (!identity)
			return;

		if (DZKOTH_ExpansionBridge.SendNotification(player, title, text, time))
			return;

		ScriptRPC rpc = new ScriptRPC;
		rpc.Write(title);
		rpc.Write(text);
		rpc.Write(time);
		rpc.Send(player, DZKOTH_RPCIds.WARNING, true, identity);

		GetGame().ChatMP(player, title + ": " + text, "colorAction");
	}

	static void BroadcastWarning(string title, string text, float time = 7.0)
	{
		if (!GetGame())
			return;

		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);

		foreach (Man man: players)
		{
			PlayerBase player = PlayerBase.Cast(man);
			if (player)
				SendWarning(player, title, text, time);
		}
	}

	static void BroadcastWarningToPlayers(array<PlayerBase> players, string title, string text, float time = 7.0)
	{
		if (!players)
			return;

		foreach (PlayerBase player: players)
		{
			if (player)
				SendWarning(player, title, text, time);
		}
	}

	static void SendPlayerUIMessage(PlayerBase player, string title, string text, float time = 5.0)
	{
		if (!player || !GetGame())
			return;

		PlayerIdentity identity = player.GetIdentity();
		if (!identity)
			return;

		ScriptRPC rpc = new ScriptRPC;
		rpc.Write(title);
		rpc.Write(text);
		rpc.Write(time);
		rpc.Send(player, DZKOTH_RPCIds.WARNING, true, identity);
	}

	static void BroadcastPlayerUIMessage(array<PlayerBase> players, string title, string text, float time = 5.0)
	{
		if (!players)
			return;

		foreach (PlayerBase player: players)
		{
			if (player)
				SendPlayerUIMessage(player, title, text, time);
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
		rpc.Send(player, DZKOTH_RPCIds.CLIENT_FX, true, identity);
	}

	static void BroadcastFX(array<PlayerBase> players, int fx, vector pos)
	{
		if (!players)
			return;

		foreach (PlayerBase player: players)
		{
			if (player)
				SendFX(player, fx, pos);
		}
	}

	static void BroadcastFXGlobal(int fx, vector pos)
	{
		if (!GetGame())
			return;

		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);

		foreach (Man man: players)
		{
			PlayerBase player = PlayerBase.Cast(man);
			if (player)
				SendFX(player, fx, pos);
		}
	}

	static void BroadcastKeycardSignal(string playerName, vector pos)
	{
		if (!GetGame())
			return;

		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);

		foreach (Man man: players)
		{
			PlayerBase player = PlayerBase.Cast(man);
			if (!player || !player.GetIdentity())
				continue;

			ScriptRPC rpc = new ScriptRPC;
			rpc.Write(playerName);
			rpc.Write(pos);
			rpc.Send(player, DZKOTH_RPCIds.KEYCARD_SIGNAL, true, player.GetIdentity());
			SendFX(player, DZKOTH_FXIds.KEYCARD_SIGNAL, pos);
		}
	}
}
