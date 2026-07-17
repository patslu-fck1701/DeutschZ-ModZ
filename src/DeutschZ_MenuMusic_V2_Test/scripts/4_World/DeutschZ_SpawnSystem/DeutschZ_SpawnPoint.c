class DeutschZ_SpawnPoint
{
	string Name;
	string Mode;
	vector Position;
	vector Orientation;
	float Radius;
	bool Enabled;

	void DeutschZ_SpawnPoint()
	{
		Name = "Spawn";
		Mode = DZSPAWN_Mode.COAST;
		Position = "0 0 0";
		Orientation = "0 0 0";
		Radius = 12.0;
		Enabled = true;
	}

	static DeutschZ_SpawnPoint Create(string name, string mode, vector position)
	{
		DeutschZ_SpawnPoint point = new DeutschZ_SpawnPoint;
		point.Name = name;
		point.Mode = mode;
		point.Position = position;
		return point;
	}
}

class DeutschZ_SpawnPointsFile
{
	int Version;
	string MapName;
	ref array<ref DeutschZ_SpawnPoint> SpawnPoints;

	void DeutschZ_SpawnPointsFile()
	{
		Version = 1;
		MapName = "chernarusplus";
		SpawnPoints = new array<ref DeutschZ_SpawnPoint>;
	}
}
