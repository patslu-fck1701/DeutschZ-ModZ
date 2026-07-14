class DZEV_MarkerData
{
	string UID;
	string Text;
	vector Pos;
	int Color;
	string IconName;
	bool Marker3D;
	bool Remove;

	void DZEV_MarkerData()
	{
		UID = "";
		Text = "";
		Pos = "0 0 0";
		Color = ARGB(255, 255, 255, 255);
		IconName = "Flag";
		Marker3D = true;
		Remove = false;
	}
}

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
