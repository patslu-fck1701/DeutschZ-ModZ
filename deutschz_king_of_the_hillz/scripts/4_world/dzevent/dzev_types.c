class DZEV_GroupRecord
{
	string GroupId;
	string OwnerId;
	ref array<string> Members;

	void DZEV_GroupRecord()
	{
		GroupId = "";
		OwnerId = "";
		Members = new array<string>;
	}

	bool HasMember(string uid)
	{
		if (uid == "" || !Members)
			return false;

		return Members.Find(uid) >= 0;
	}
}

class DZEV_GroupsSave
{
	ref array<ref DZEV_GroupRecord> Groups;

	void DZEV_GroupsSave()
	{
		Groups = new array<ref DZEV_GroupRecord>;
	}
}
