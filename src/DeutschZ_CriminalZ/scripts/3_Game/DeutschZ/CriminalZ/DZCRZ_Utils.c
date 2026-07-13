class DZCRZ_Utils
{
	static int RandomDuration(int minimum, int maximum)
	{
		if (minimum < 1)
			minimum = 1;
		if (maximum < minimum)
			maximum = minimum;
		return Math.RandomIntInclusive(minimum, maximum);
	}

	static string ModuleLabel(int moduleType)
	{
		if (moduleType == DZCRZ_Const.MODULE_ATM)
			return "ATM-Ueberfall";
		if (moduleType == DZCRZ_Const.MODULE_GARAGE)
			return "Garagen-Hack";
		if (moduleType == DZCRZ_Const.MODULE_VEHICLE)
			return "Fahrzeug-Hack";
		return "CriminalZ-Hack";
	}
}
