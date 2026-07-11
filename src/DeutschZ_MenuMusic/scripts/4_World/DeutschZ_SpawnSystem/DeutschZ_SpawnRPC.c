class DZSPAWN_ClientRPC
{
	static void RequestMode(string mode)
	{
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (!player || !DZSPAWN_Mode.IsKnown(mode))
			return;

		player.RPCSingleParam(DZSPAWN_RPC.REQUEST_RESPAWN_MODE, new Param1<string>(mode), true);
	}
}

class DZSPAWN_ClientState
{
	protected static bool s_Accepted;
	protected static bool s_Denied;
	protected static string s_Mode;
	protected static string s_Reason;
	protected static int s_RemainingSeconds;

	static void SetAccepted(string mode)
	{
		s_Mode = mode;
		s_Accepted = true;
		s_Denied = false;
	}

	static void SetDenied(string reason, int remainingSeconds)
	{
		s_Reason = reason;
		s_RemainingSeconds = remainingSeconds;
		s_Denied = true;
		s_Accepted = false;
	}

	static bool ConsumeAccepted(out string mode)
	{
		if (!s_Accepted)
			return false;
		mode = s_Mode;
		s_Accepted = false;
		return true;
	}

	static bool ConsumeDenied(out string reason, out int remainingSeconds)
	{
		if (!s_Denied)
			return false;
		reason = s_Reason;
		remainingSeconds = s_RemainingSeconds;
		s_Denied = false;
		return true;
	}
}
