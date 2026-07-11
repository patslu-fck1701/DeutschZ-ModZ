class CfgPatches
{
	class DeutschZ_KOTHTerminal
	{
		units[] =
		{
			"DZ_KOTHTerminal",
			"DeutschZ_Barrel_Green",
			"DeutschZ_FireworksLauncher"
		};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] =
		{
			"DZ_Data",
			"DZ_Scripts",
			"DZ_Structures_Specific",
			"DZ_Gear_Containers",
			"DZ_Gear_Tools",
			"DZ_Weapons_Explosives",
			"FoXyKOTH_Core"
		};
	};
};

class CfgVehicles
{
	class Land_radio_panelbig;
	class Barrel_Green;
	class FireworksLauncher;

	class DZ_KOTHTerminal: Land_radio_panelbig
	{
		scope = 2;
		displayName = "DeutschZ KotH Terminal";
		descriptionShort = "Aktivierungsterminal fuer DeutschZ KotH-Zonen.";
		vehicleClass = "DeutschZ_Static";
		carveNavmesh = 1;
		weight = 0;
		model = "\dz\structures\Specific\Radio\Radio_PanelBig.p3d";
		hiddenSelections[] = {"control_panel"};
		hiddenSelectionsTextures[] =
		{
			"\DeutschZ_KOTHTerminal\data\radio\dz_koth_terminal_control_panel_big_co.paa"
		};
		hiddenSelectionsMaterials[] =
		{
			"\DeutschZ_KOTHTerminal\data\radio\dz_koth_terminal_control_panel_big.rvmat"
		};
	};

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
		name = "DeutschZ KOTH Terminal";
		credits = "DeutschZ";
		author = "DeutschZ";
		authorID = "";
		version = "1.0.0";
		type = "mod";
		dependencies[] =
		{
			"Game",
			"World",
			"Mission"
		};

		class defs
		{
			class gameScriptModule
			{
				value = "";
				files[] =
				{
					"DeutschZ_KOTHTerminal/scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				value = "";
				files[] =
				{
					"DeutschZ_KOTHTerminal/scripts/4_World"
				};
			};
			class missionScriptModule
			{
				value = "";
				files[] =
				{
					"DeutschZ_KOTHTerminal/scripts/5_Mission"
				};
			};
		};
	};
};
