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

	static string GetCaptureGroupId(PlayerBase player)
	{
		if (!player)
			return "";

#ifdef LBmaster_Groups
		DZKOTHF_Settings settings = DZKOTHF_SettingsLoader.Load();
		LBGroup lbGroup;
		if (settings && settings.RespectLBmasterGroups)
			lbGroup = player.GetLBGroup();
		if (lbGroup)
		{
			string lbGroupId = lbGroup.shortname;
			if (lbGroupId == "")
				lbGroupId = lbGroup.name;

			if (lbGroupId != "")
				return "LBMASTER:" + lbGroupId;
		}
#endif

		string internalGroupId = GetPlayerGroupId(GetPlayerUid(player));
		if (internalGroupId != "")
			return "DZEV:" + internalGroupId;

		return "";
	}

	static int GetExpansionPartyId(PlayerBase player)
	{
		if (!player)
			return -1;

#ifdef EXPANSIONMODGROUPS
		DZKOTHF_Settings settings = DZKOTHF_SettingsLoader.Load();
		if (settings && settings.RespectExpansionParties)
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

		int partyId = GetExpansionPartyId(player);
		if (ownerPartyId != -1 && partyId == ownerPartyId)
			return true;

		if (ownerGroupId != "" && GetCaptureGroupId(player) == ownerGroupId)
			return true;

		return false;
	}
}
