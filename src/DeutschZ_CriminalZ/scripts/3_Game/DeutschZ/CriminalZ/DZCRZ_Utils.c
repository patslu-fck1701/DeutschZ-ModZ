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

	static string CancelReasonLabel(int reason)
	{
		if (reason == DZCRZ_Const.CANCEL_ACTION_ENDED)
			return "Aktion vorzeitig beendet";
		if (reason == DZCRZ_Const.CANCEL_PLAYER_INVALID)
			return "Spieler nicht handlungsfaehig";
		if (reason == DZCRZ_Const.CANCEL_TOO_FAR)
			return "Zu weit vom Ziel entfernt";
		if (reason == DZCRZ_Const.CANCEL_TOOL_MISSING)
			return "Werkzeug fehlt oder ist ruiniert";
		if (reason == DZCRZ_Const.CANCEL_TARGET_INVALID)
			return "Ziel nicht mehr gueltig";
		if (reason == DZCRZ_Const.CANCEL_SERVER_SHUTDOWN)
			return "Server wird beendet";
		if (reason == DZCRZ_Const.CANCEL_ADMIN)
			return "Durch Administration beendet";
		return "Unbekannter Grund";
	}
}
