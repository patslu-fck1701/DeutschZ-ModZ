class CfgPatches
{
	class DeutschZ_Blackbox_ConvoyZ
	{
		units[] =
		{
			"DZBBC_Blackbox",
			"DZBBC_DataCore",
			"DZBBC_MilitaryTerminal",
			"DZBBC_SecureSupplyContainer",
			"DZBBC_GasZoneFlare",
			"GasZonen_Leuchtfackel",
			"DZBBC_EventMarkerObject",
			"DZBBC_WreckProxy"
		};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] =
		{
			"DZ_Data",
			"DZ_Scripts",
			"DZ_Gear_Containers",
			"DZ_Gear_Navigation",
			"DZ_Gear_Tools",
			"DZ_Gear_Consumables",
			"DZ_Structures_Wrecks",
			"DZ_Characters_Zombies"
		};
	};
};

class CfgMods
{
	class DeutschZ_Blackbox_ConvoyZ
	{
		dir = "deutschz_blackbox_convoyz";
		name = "DeutschZ Blackbox ConvoyZ";
		credits = "DeutschZ";
		author = "DeutschZ";
		authorID = "";
		version = "0.1.1-livefix";
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
					"deutschz_blackbox_convoyz/scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				value = "";
				files[] =
				{
					"deutschz_blackbox_convoyz/scripts/4_World"
				};
			};
			class missionScriptModule
			{
				value = "";
				files[] =
				{
					"deutschz_blackbox_convoyz/scripts/5_Mission"
				};
			};
		};
	};
};

class CfgVehicles
{
	class Inventory_Base;
	class SeaChest;
	class GPSReceiver;
	class Roadflare;
	class SmallProtectorCase;
	class ScientificBriefcase;
	class PunchedCard;
	class HouseNoDestruct;

	class DZBBC_Blackbox : ScientificBriefcase
	{
		scope = 2;
		displayName = "#STR_DZBBC_BLACKBOX_NAME";
		descriptionShort = "#STR_DZBBC_BLACKBOX_DESC";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] =
		{
			"deutschz_blackbox_convoyz\data\textures\convoyz\convoyblackbox_loot_protectorcase_co.paa"
		};
	};

	class DZBBC_DataCore : PunchedCard
	{
		scope = 2;
		displayName = "#STR_DZBBC_DATACORE_NAME";
		descriptionShort = "#STR_DZBBC_DATACORE_DESC";
	};

	class DZBBC_MilitaryTerminal : SeaChest
	{
		scope = 2;
		displayName = "#STR_DZBBC_TERMINAL_NAME";
		descriptionShort = "#STR_DZBBC_TERMINAL_DESC";
		itemsCargoSize[] = {10,50};
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] =
		{
			"deutschz_blackbox_convoyz\data\textures\convoyz\logogreen_sea_chest_co.paa"
		};
	};

	class DZBBC_SecureSupplyContainer : SeaChest
	{
		scope = 2;
		displayName = "#STR_DZBBC_CONTAINER_NAME";
		descriptionShort = "#STR_DZBBC_CONTAINER_DESC";
		itemsCargoSize[] = {10,50};
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] =
		{
			"deutschz_blackbox_convoyz\data\textures\convoyz\logogreen_sea_chest_co.paa"
		};
	};

	class DZBBC_GasZoneFlare : Roadflare
	{
		scope = 2;
		displayName = "#STR_DZBBC_FLARE_NAME";
		descriptionShort = "#STR_DZBBC_FLARE_DESC";
	};

	class GasZonen_Leuchtfackel : DZBBC_GasZoneFlare
	{
		scope = 2;
		displayName = "#STR_DZBBC_FLARE_LEGACY_NAME";
		descriptionShort = "#STR_DZBBC_FLARE_LEGACY_DESC";
	};

	class DZBBC_EventMarkerObject : Inventory_Base
	{
		scope = 1;
		displayName = "#STR_DZBBC_EVENTMARKER_NAME";
		descriptionShort = "#STR_DZBBC_EVENTMARKER_DESC";
	};

	class DZBBC_WreckProxy : HouseNoDestruct
	{
		scope = 1;
		displayName = "#STR_DZBBC_WRECKPROXY_NAME";
		descriptionShort = "#STR_DZBBC_WRECKPROXY_DESC";
	};
};

