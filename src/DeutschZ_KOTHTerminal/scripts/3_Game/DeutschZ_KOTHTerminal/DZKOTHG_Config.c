class DZKOTHG_ZoneSettings
{
	string ZoneName;
	string FoXyKOTHZoneName;
	string FlagPosition;
	string SupplyBoxPosition;
	string TerminalPosition;
	string TerminalOrientation;
	string MarkerPosition;
	int Radius;

	void DZKOTHG_ZoneSettings()
	{
		ZoneName = "";
		FoXyKOTHZoneName = "";
		FlagPosition = "0 0 0";
		SupplyBoxPosition = "0 0 0";
		TerminalPosition = "0 0 0";
		TerminalOrientation = "0 0 0";
		MarkerPosition = "0 0 0";
		Radius = 250;
	}

	vector GetFlagPosition()
	{
		return FlagPosition.ToVector();
	}

	vector GetSupplyBoxPosition()
	{
		return SupplyBoxPosition.ToVector();
	}

	vector GetTerminalPosition()
	{
		return TerminalPosition.ToVector();
	}

	vector GetTerminalOrientation()
	{
		return TerminalOrientation.ToVector();
	}

	vector GetMarkerPosition()
	{
		if (MarkerPosition == "")
			return GetFlagPosition();

		return MarkerPosition.ToVector();
	}

	string GetFoXyZoneName()
	{
		if (FoXyKOTHZoneName != "")
			return FoXyKOTHZoneName;

		return ZoneName;
	}
}

class DZKOTHG_Settings
{
	int EnableKOTHGate;
	string TerminalClassName;
	float HackDurationSeconds;
	int UseChopWoodAnimation;
	int DeleteTerminalAfterHack;
	int AllowOnlyOneHacker;
	float TerminalDistanceCheck;
	int PreStartTimeoutMinutes;
	int UseExpansionNotify;
	int UseExpansion3DMarker;
	int UseVanillaMapMarker;
	int EnableCaptureWaves;
	string UpgradeRewardContainerClassName;
	string PreNotifyMessage;
	string HackStartedMessage;
	string HackCompletedMessage;
	string PreStartExpiredMessage;
	string StartFailedMessage;
	ref array<ref DZKOTHG_ZoneSettings> Zones;

	void DZKOTHG_Settings()
	{
		EnableKOTHGate = 1;
		TerminalClassName = "DZ_KOTHTerminal";
		HackDurationSeconds = 60.0;
		UseChopWoodAnimation = 1;
		DeleteTerminalAfterHack = 1;
		AllowOnlyOneHacker = 1;
		TerminalDistanceCheck = 3.0;
		PreStartTimeoutMinutes = 30;
		UseExpansionNotify = 0;
		UseExpansion3DMarker = 0;
		UseVanillaMapMarker = 1;
		EnableCaptureWaves = 1;
		UpgradeRewardContainerClassName = "SeaChest";
		PreNotifyMessage = "KOTH-SIGNAL ENTDECKT: Ein umkaempfter Bereich wurde lokalisiert. Aktiviert das Terminal, um die Zone scharfzuschalten.";
		HackStartedMessage = "Terminal-Hack gestartet. Haltet die Position fuer 60 Sekunden.";
		HackCompletedMessage = "KOTH AKTIV: Das Terminal wurde aktiviert. Die Flagge ist online. Haltet die Zone und sichert euch die Belohnung.";
		PreStartExpiredMessage = "KOTH-Signal verloren. Terminal wurde nicht aktiviert.";
		StartFailedMessage = "KOTH konnte nicht gestartet werden. Gate wurde zurueckgesetzt.";
		Zones = new array<ref DZKOTHG_ZoneSettings>;
	}
}
