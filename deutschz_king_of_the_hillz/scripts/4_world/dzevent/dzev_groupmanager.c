class DZEV_GroupManager
{
	private static ref DZEV_GroupManager s_Instance;
	private ref DZEV_GroupsSave m_Save;

	static DZEV_GroupManager GetInstance()
	{
		if (!s_Instance)
			s_Instance = new DZEV_GroupManager;

		return s_Instance;
	}

	void InitServer()
	{
		m_Save = DZEV_Persistence.LoadGroups();
		Print("[DZEV] Group manager ready. Groups: " + m_Save.Groups.Count());
	}

	string GetPlayerGroupId(string playerUid)
	{
		if (!m_Save || !m_Save.Groups || playerUid == "")
			return "";

		foreach (DZEV_GroupRecord group: m_Save.Groups)
		{
			if (group && group.HasMember(playerUid))
				return group.GroupId;
		}

		return "";
	}

	bool CanSeeGroupScopedMarker(string playerUid, string markerGroupId)
	{
		if (markerGroupId == "")
			return true;

		return GetPlayerGroupId(playerUid) == markerGroupId;
	}

	bool TryJoinGroup(string playerUid, string groupId, string ownerUid = "")
	{
		if (playerUid == "" || groupId == "")
			return false;

		if (!m_Save)
			m_Save = DZEV_Persistence.LoadGroups();

		string current = GetPlayerGroupId(playerUid);
		if (current != "" && current != groupId)
			return false;

		DZEV_GroupRecord group = FindGroup(groupId);
		if (!group)
		{
			group = new DZEV_GroupRecord;
			group.GroupId = groupId;
			group.OwnerId = ownerUid;
			group.Members.Insert(playerUid);
			m_Save.Groups.Insert(group);
		}
		else if (!group.HasMember(playerUid))
		{
			group.Members.Insert(playerUid);
		}

		DZEV_Persistence.SaveGroups(m_Save);
		return true;
	}

	bool TryLeaveGroup(string playerUid)
	{
		if (!m_Save || playerUid == "")
			return false;

		foreach (DZEV_GroupRecord group: m_Save.Groups)
		{
			if (!group || !group.Members)
				continue;

			int index = group.Members.Find(playerUid);
			if (index >= 0)
			{
				group.Members.Remove(index);
				DZEV_Persistence.SaveGroups(m_Save);
				return true;
			}
		}

		return false;
	}

	private DZEV_GroupRecord FindGroup(string groupId)
	{
		if (!m_Save || !m_Save.Groups)
			return null;

		foreach (DZEV_GroupRecord group: m_Save.Groups)
		{
			if (group && group.GroupId == groupId)
				return group;
		}

		return null;
	}
}
