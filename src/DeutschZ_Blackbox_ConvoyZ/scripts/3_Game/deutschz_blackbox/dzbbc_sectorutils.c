class DZBBC_SectorUtils
{
	static string GetSectorLabel(vector pos, float sectorSize = 1000.0)
	{
		if (sectorSize <= 0.0)
			sectorSize = 1000.0;

		int sectorX = (int)Math.Floor(pos[0] / sectorSize);
		int sectorZ = (int)Math.Floor(pos[2] / sectorSize);
		return "Sektor " + sectorX.ToString() + "-" + sectorZ.ToString();
	}

	static vector GetSectorCenter(vector pos, float sectorSize = 1000.0)
	{
		if (sectorSize <= 0.0)
			sectorSize = 1000.0;

		int sectorX = (int)Math.Floor(pos[0] / sectorSize);
		int sectorZ = (int)Math.Floor(pos[2] / sectorSize);
		vector center = Vector((sectorX * sectorSize) + (sectorSize * 0.5), 0, (sectorZ * sectorSize) + (sectorSize * 0.5));
		return DZBBC_Utils.Grounded(center);
	}
}
