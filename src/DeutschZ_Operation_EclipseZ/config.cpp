class CfgPatches
{
	class DeutschZ_Operation_EclipseZ
	{
		units[] =
		{
			"DZECZ_StartReceiver",
			"DZECZ_NWAFStation",
			"DZECZ_TisyStation",
			"DZECZ_GasZoneStation",
			"DZECZ_FireworkBattery",
			"DZECZ_FinalTerminal",
			"DZECZ_EclipseRewardCrate",
			"DZECZ_EclipseToken",
			"DZECZ_EclipseGuardian"
		};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] =
		{
			"DZ_Data",
			"DZ_Scripts",
			"DZ_Structures_Specific",
			"DZ_Gear_Tools",
			"DZ_Gear_Containers",
			"DZ_Gear_Consumables",
			"DZ_Characters_Zombies",
			"DeutschZ_KOTH_InfectedSiege",
			"DeutschZ_BattlegroundZ",
			"DeutschZ_Blackbox_ConvoyZ"
		};
	};
};

class CfgMods
{
	class DeutschZ_Operation_EclipseZ
	{
		dir = "DeutschZ_Operation_EclipseZ";
		name = "DeutschZ Operation EclipseZ";
		credits = "DeutschZ";
		author = "DeutschZ";
		version = "0.1.0";
		type = "mod";
		dependencies[] = {"Game", "World", "Mission"};
		class defs
		{
			class gameScriptModule
			{
				value = "";
				files[] = {"DeutschZ_Operation_EclipseZ/scripts/3_Game"};
			};
			class worldScriptModule
			{
				value = "";
				files[] = {"DeutschZ_Operation_EclipseZ/scripts/4_World"};
			};
			class missionScriptModule
			{
				value = "";
				files[] = {"DeutschZ_Operation_EclipseZ/scripts/5_Mission"};
			};
		};
	};
};

class CfgVehicles
{
	class HouseNoDestruct;
	class FireworksLauncher;
	class SeaChest;
	class Paper;
	class ZmbM_Mummy;

	class DZECZ_RadioPanelBase : HouseNoDestruct
	{
		scope = 0;
		model = "\dz\structures\Specific\Radio\Radio_PanelBig.p3d";
		hiddenSelections[] = {"control_panel"};
		hiddenSelectionsTextures[] = {"\DeutschZ_BattlegroundZ\data\textures\dz_koth_terminal_control_panel_big_co.paa"};
		hiddenSelectionsMaterials[] = {"\DeutschZ_BattlegroundZ\data\textures\dz_koth_terminal_control_panel_big.rvmat"};
	};

	class DZECZ_StartReceiver : DZECZ_RadioPanelBase
	{
		scope = 2;
		displayName = "DeutschZ Eclipse Empfaenger";
		descriptionShort = "Ein alter Empfaenger, der nur auf erkannte Storytraeger reagiert.";
	};

	class DZECZ_NWAFStation : DZECZ_RadioPanelBase
	{
		scope = 2;
		displayName = "Eclipse Station Alpha";
		descriptionShort = "Eine alte Signalstation am NWAF.";
	};

	class DZECZ_TisyStation : DZECZ_RadioPanelBase
	{
		scope = 2;
		displayName = "Eclipse Station Bravo";
		descriptionShort = "Eine militaerische Relaisstation bei Tisy.";
	};

	class DZECZ_GasZoneStation : DZECZ_RadioPanelBase
	{
		scope = 2;
		displayName = "Eclipse Station Charlie";
		descriptionShort = "Ein kontaminierter Signalpunkt.";
	};

	class DZECZ_FinalTerminal : DZECZ_RadioPanelBase
	{
		scope = 2;
		displayName = "Eclipse Hauptterminal";
		descriptionShort = "Das letzte Terminal des Eclipse-Protokolls.";
	};

	class DZECZ_FireworkBattery : FireworksLauncher
	{
		scope = 2;
		displayName = "DeutschZ FeuerwerkBattery";
		descriptionShort = "Eine alte Signalbatterie fuer das Eclipse-Protokoll.";
	};

	class DZECZ_EclipseRewardCrate : SeaChest
	{
		scope = 2;
		displayName = "Eclipse Sicherheitskiste";
		descriptionShort = "Endgame-Belohnungen aus Operation EclipseZ.";
		itemsCargoSize[] = {10, 50};
	};

	class DZECZ_EclipseToken : Paper
	{
		scope = 2;
		displayName = "DeutschZ Eclipse Token";
		descriptionShort = "Beweis fuer den Abschluss von Operation EclipseZ.";
	};

	class DZECZ_EclipseGuardian : ZmbM_Mummy
	{
		scope = 2;
		displayName = "Eclipse Waechter";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 3500;
				};
			};
		};
	};
};
