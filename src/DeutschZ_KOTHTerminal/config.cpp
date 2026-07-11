class CfgPatches
{
	class DeutschZ_KOTHTerminal
	{
		units[] =
		{
			"DeutschZ_Barrel_Green",
			"DeutschZ_FireworksLauncher"
		};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] =
		{
			"DZ_Data",
			"DZ_Scripts",
			"DZ_Gear_Containers",
			"DZ_Weapons_Explosives"
		};
	};
};

class CfgVehicles
{
	class Barrel_Green;
	class FireworksLauncher;

	class DeutschZ_Barrel_Green: Barrel_Green
	{
		scope = 2;
		displayName = "DeutschZ Fass";
		descriptionShort = "DeutschZ Lagerfass mit 500 Plaetzen.";
		hiddenSelectionsTextures[] =
		{
			"\DeutschZ_KOTHTerminal\data\items\deutschz_barrel_green_co.paa"
		};
		itemSize[] = {10, 15};
		weight = 10000;
		class Cargo
		{
			itemsCargoSize[] = {10, 50};
			openable = 0;
			allowOwnedCargoManipulation = 1;
		};
	};

	class DeutschZ_FireworksLauncher: FireworksLauncher
	{
		scope = 2;
		displayName = "DeutschZ Feuerwerkswerfer";
		descriptionShort = "DeutschZ Feuerwerkswerfer mit Nebelsignal.";
		hiddenSelectionsTextures[] =
		{
			"\DeutschZ_KOTHTerminal\data\items\deutschz_fireworkslauncher_co.paa",
			"\DeutschZ_KOTHTerminal\data\items\deutschz_fireworkslauncher_co.paa"
		};
	};
};

class CfgMods
{
	class DeutschZ_KOTHTerminal
	{
		dir = "DeutschZ_KOTHTerminal";
		picture = "";
		action = "";
		name = "DeutschZ Items";
		credits = "DeutschZ";
		author = "DeutschZ";
		authorID = "";
		version = "1.0.0";
		type = "mod";
		dependencies[] =
		{
			"World"
		};

		class defs
		{
			class worldScriptModule
			{
				value = "";
				files[] =
				{
					"DeutschZ_KOTHTerminal/scripts/4_World"
				};
			};
		};
	};
};
