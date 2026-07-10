class DZBBC_PlayerUtils
{
	static bool IsValidPlayer(PlayerBase player)
	{
		return player && player.GetIdentity() && player.IsAlive() && !player.IsUnconscious();
	}

	static string GetPlayerUid(PlayerBase player)
	{
		if (!player || !player.GetIdentity())
			return "";

		return player.GetIdentity().GetId();
	}

	static string GetPlayerName(PlayerBase player)
	{
		if (!player || !player.GetIdentity())
			return "unknown";

		return player.GetIdentity().GetName();
	}

	static ref array<PlayerBase> GetOnlinePlayers()
	{
		ref array<PlayerBase> result = new array<PlayerBase>;
		if (!GetGame())
			return result;

		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		foreach (Man man: players)
		{
			PlayerBase player = PlayerBase.Cast(man);
			if (player)
				result.Insert(player);
		}

		return result;
	}
}
