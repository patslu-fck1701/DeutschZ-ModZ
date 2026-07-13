class DZCRZ_NotificationManager
{
	void Global(string title, string message, float duration = 10.0)
	{
		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		foreach (Man man: players)
		{
			PlayerBase player = PlayerBase.Cast(man);
			if (player)
				Personal(player, title, message, duration);
		}
	}

	void Personal(PlayerBase player, string title, string message, float duration = 8.0)
	{
		if (!player || !player.GetIdentity())
			return;
#ifdef EXPANSIONMODCORE
		ExpansionNotification(title, message, "Info", DZCRZ_Const.MARKER_COLOR, duration).Create(player.GetIdentity());
#else
		DZCRZ_ServerRPC.SendMessage(player, title + ": " + message, duration * 1000);
#endif
	}
}
