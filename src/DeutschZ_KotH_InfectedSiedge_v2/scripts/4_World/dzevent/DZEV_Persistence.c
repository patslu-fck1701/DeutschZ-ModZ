class DZEV_Persistence
{
	static void EnsureProfileDirectory()
	{
		DZKOTH_ProfilePaths.Ensure();
	}

	static ref DZEV_GroupsSave LoadGroups()
	{
		EnsureProfileDirectory();

		ref DZEV_GroupsSave groups = new DZEV_GroupsSave;
		string errorMessage;

		if (FileExist(DZEV_Paths.GROUPS_PATH))
		{
			if (!JsonFileLoader<ref DZEV_GroupsSave>.LoadFile(DZEV_Paths.GROUPS_PATH, groups, errorMessage))
			{
				Print("[DZEV] Groups load failed, using empty groups: " + errorMessage);
				groups = new DZEV_GroupsSave;
				SaveGroups(groups);
			}
		}
		else
		{
			SaveGroups(groups);
		}

		if (!groups.Groups)
			groups.Groups = new array<ref DZEV_GroupRecord>;

		return groups;
	}

	static void SaveGroups(DZEV_GroupsSave groups)
	{
		if (!groups)
			return;

		EnsureProfileDirectory();
		string errorMessage;
		if (!JsonFileLoader<ref DZEV_GroupsSave>.SaveFile(DZEV_Paths.GROUPS_PATH, groups, errorMessage))
			Print("[DZEV] Groups save failed: " + errorMessage);
	}
}
