class DZPoints_TopGamesCallback : RestCallback
{
	protected string m_PlayerID;

	void DZPoints_TopGamesCallback(string playerID)
	{
		m_PlayerID = playerID;
	}

	override void OnSuccess(string data, int dataSize)
	{
		DZPoints_TopGames.HandleClaimResponse(m_PlayerID,data,dataSize);
	}

	override void OnError(int errorCode)
	{
		DZPoints_TopGames.HandleFailure(m_PlayerID,"http_" + errorCode.ToString());
	}

	override void OnTimeout()
	{
		DZPoints_TopGames.HandleFailure(m_PlayerID,"timeout");
	}
}

class DZPoints_TopGames
{
	protected static bool s_Started;

	static void Start()
	{
		if (s_Started || !GetGame() || !GetGame().IsServer()) return;
		s_Started = true;
		DZPoints_Settings settings = DZPoints_Service.Settings();
		if (!settings || !settings.TopGamesEnabled)
		{
			Print("[DeutschZ PointsZ] TOPGAMES_DISABLED");
			return;
		}
		if (DZPoints_Service.TopGamesToken() == "")
		{
			Print("[DeutschZ PointsZ] TOPGAMES_DISABLED reason=missing_server_token");
			return;
		}
		int intervalMs = Math.Max(settings.PollIntervalSeconds,60) * 1000;
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Poll,15000,false);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Poll,intervalMs,true);
		Print("[DeutschZ PointsZ] TOPGAMES_STARTED intervalSeconds=" + settings.PollIntervalSeconds.ToString());
	}

	static void Poll()
	{
		if (!GetGame() || !GetGame().IsServer()) return;
		string token = DZPoints_Service.TopGamesToken();
		if (token == "") return;
		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		foreach (Man man : players)
		{
			PlayerBase player = PlayerBase.Cast(man);
			if (!player || !player.GetIdentity()) continue;
			Claim(player.GetIdentity().GetPlainId(),token);
		}
	}

	protected static void Claim(string playerID, string token)
	{
		RestApi api = CreateRestApi();
		if (!api)
		{
			HandleFailure(playerID,"rest_api_unavailable");
			return;
		}
		RestContext context = api.GetRestContext("https://api.top-games.net/v1/votes/");
		if (!context)
		{
			HandleFailure(playerID,"rest_context_unavailable");
			return;
		}
		string path = "claim-steam?server_token=" + token + "&steam_id=" + playerID + "&standard_http_code=0";
		context.GET(new DZPoints_TopGamesCallback(playerID),path);
	}

	static void HandleClaimResponse(string playerID, string data, int dataSize)
	{
		if (dataSize <= 0 || data == "")
		{
			HandleFailure(playerID,"empty_response");
			return;
		}
		int result = data.ToInt();
		if (result == 1)
		{
			if (!DZPoints_Service.CreditVote(playerID)) HandleFailure(playerID,"account_save_failed");
			return;
		}
		if (result == 0 || result == 2) return;
		HandleFailure(playerID,"unexpected_response");
	}

	static void HandleFailure(string playerID, string reason)
	{
		Print("[DeutschZ PointsZ] TOPGAMES_REQUEST_FAILED player=" + playerID + " reason=" + reason);
	}
}
