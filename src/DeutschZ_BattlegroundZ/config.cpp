class CfgPatches
{
	class DeutschZ_BattlegroundZ
	{
		units[] =
		{
			"DZBGZ_CardReader",
			"DZBGZ_EventMarkerObject"
		};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] =
		{
			"DZ_Data",
			"DZ_Scripts",
			"DZ_Structures_Specific",
			"DZ_Gear_Navigation",
			"DZ_Gear_Consumables",
			"DZ_Characters_Zombies",
			"DeutschZ_KOTH_InfectedSiege"
		};
	};
};

class CfgMods
{
	class DeutschZ_BattlegroundZ
	{
		dir = "DeutschZ_BattlegroundZ";
		name = "DeutschZ BattlegroundZ";
		credits = "DeutschZ";
		author = "DeutschZ";
		authorID = "";
		version = "0.1.0";
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
					"DeutschZ_BattlegroundZ/scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				value = "";
				files[] =
				{
					"DeutschZ_BattlegroundZ/scripts/4_World"
				};
			};
			class missionScriptModule
			{
				value = "";
				files[] =
				{
					"DeutschZ_BattlegroundZ/scripts/5_Mission"
				};
			};
		};
	};
};

class CfgVehicles
{
	class GPSReceiver;
	class Inventory_Base;

	class DZBGZ_CardReader : GPSReceiver
	{
		scope = 2;
		displayName = "DeutschZ BattlegroundZ Kartenleser";
		descriptionShort = "Ein instabiles militaerisches Lesegeraet. Es reagiert auf die BattlegroundZ Keycard.";
		hiddenSelections[] =
		{
			"gpsreceiver",
			"grid_1_0",
			"grid_1_1",
			"grid_1_2",
			"grid_2_0",
			"grid_2_1",
			"grid_2_2",
			"alt_0",
			"alt_1",
			"alt_2",
			"alt_3"
		};
		hiddenSelectionsTextures[] =
		{
			"\DeutschZ_BattlegroundZ\data\textures\cardreader_gpsreceiver_co.paa",
			"dz\gear\navigation\data\GPS_dash_ca.paa",
			"dz\gear\navigation\data\GPS_dash_ca.paa",
			"dz\gear\navigation\data\GPS_dash_ca.paa",
			"dz\gear\navigation\data\GPS_dash_ca.paa",
			"dz\gear\navigation\data\GPS_dash_ca.paa",
			"dz\gear\navigation\data\GPS_dash_ca.paa",
			"dz\gear\navigation\data\GPS_dash_ca.paa",
			"dz\gear\navigation\data\GPS_dash_ca.paa",
			"dz\gear\navigation\data\GPS_dash_ca.paa",
			"dz\gear\navigation\data\GPS_dash_ca.paa"
		};
	};

	class DZBGZ_EventMarkerObject : Inventory_Base
	{
		scope = 1;
		displayName = "DeutschZ BattlegroundZ Marker";
		descriptionShort = "Serverseitiger Hilfsmarker fuer DeutschZ BattlegroundZ.";
	};
};
