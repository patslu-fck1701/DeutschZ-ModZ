class DZKOTH_GroupResolver
{
	protected static bool s_Initialized;

	static void InitServer()
	{
		if (s_Initialized)
			return;

		DZEV_GroupManager.GetInstance().InitServer();
		s_Initialized = true;
	}

	static string GetPlayerUid(PlayerBase player)
	{
		if (!player || !player.GetIdentity())
			return "";

		return player.GetIdentity().GetId();
	}

	static string GetPlayerGroupId(string playerUid)
	{
		if (playerUid == "")
			return "";

		InitServer();
		return DZEV_GroupManager.GetInstance().GetPlayerGroupId(playerUid);
	}

	static int GetExpansionPartyId(PlayerBase player)
	{
		if (!player)
			return -1;

#ifdef EXPANSIONMODGROUPS
		return player.Expansion_GetPartyID();
#endif

		return -1;
	}

	static bool IsCaptureTeamMember(PlayerBase player, string ownerUid, string ownerGroupId, int ownerPartyId)
	{
		if (!player || !player.GetIdentity())
			return false;

		string uid = GetPlayerUid(player);
		if (uid != "" && uid == ownerUid)
			return true;

		if (ownerGroupId != "" && GetPlayerGroupId(uid) == ownerGroupId)
			return true;

		int partyId = GetExpansionPartyId(player);
		if (ownerPartyId != -1 && partyId == ownerPartyId)
			return true;

		return false;
	}
}
