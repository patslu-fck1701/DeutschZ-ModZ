class CfgPatches
{
	class DeutschZ_KotHZ_V2_Pro
	{
		units[] =
		{
			"DZKOTH_EventFlagpole",
			"DZKOTH_EventFlag",
			"DZKOTH_BattlegroundZ_Keycard",
			"DZKOTH_BossCorpse",
			"DZKOTH_RewardChest",
			"DeutschZ_Barrel_Green",
			"DeutschZ_FireworksLauncher",
			"DeutschZ_Regeln",
			"DZKOTH_EliteInfected",
			"DZKOTH_Infected_250",
			"DZKOTH_Infected_400",
			"DZKOTH_Infected_600",
			"DZKOTH_Infected_800",
			"DZKOTH_Infected_1000",
			"DZKOTH_BosZZombie"
		};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] =
		{
			"DZ_Data",
			"DZ_Scripts",
			"DZ_Gear_Camping",
			"DZ_Gear_Containers",
			"DZ_Gear_Navigation",
			"DZ_Gear_Tools",
			"DZ_Gear_Consumables",
			"DZ_Weapons_Explosives",
			"DZ_Characters_Zombies",
			"DZ_Sounds_Effects"
		};
	};
};

class CfgSoundShaders
{
	class DZKOTH_EventMusic_SoundShader
	{
		samples[] = {{"\DeutschZ_KotHZ_V2_Pro\sounds\dzkoth_anthem", 1}};
		volume = 0.45;
		radius = 100;
		range = 100;
	};
};

class CfgSoundSets
{
	class DZKOTH_EventMusic_SoundSet
	{
		soundShaders[] = {"DZKOTH_EventMusic_SoundShader"};
		volumeFactor = 1;
		frequencyFactor = 1;
		// The supplied anthem is stereo. Client code therefore applies the
		// 100 m positional fade and hard cutoff for this 2D sound set.
		spatial = 0;
	};
};

class CfgMods
{
	class DeutschZ_KotHZ_V2_Pro
	{
		dir = "DeutschZ_KotHZ_V2_Pro";
		name = "DeutschZ KotHZ V2 Pro";
		credits = "DeutschZ";
		author = "DeutschZ";
		authorID = "";
		version = "0.5.0-v2-pro";
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
					"DeutschZ_KotHZ_V2_Pro/scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				value = "";
				files[] =
				{
					"DeutschZ_KotHZ_V2_Pro/scripts/4_World"
				};
			};
			class missionScriptModule
			{
				value = "";
				files[] =
				{
					"DeutschZ_KotHZ_V2_Pro/scripts/5_Mission"
				};
			};
		};
	};
};

class CfgVehicles
{
	class StaticFlagPole;
	class Flag_Base;
	class PunchedCard;
	class Paper;
	class SeaChest;
	class Barrel_Green;
	class FireworksLauncher;
	class GPSReceiver;
	class ZmbM_PolicemanSpecForce_Heavy;
	class ZmbM_HunterOld_Autumn;
	class ZmbM_SoldierNormal_Base;
	class ZmbM_Mummy;

	class DeutschZ_Barrel_Green : Barrel_Green
	{
		scope = 2;
		displayName = "DeutschZ Fass";
		descriptionShort = "DeutschZ Lagerfass mit 500 Plaetzen.";
		hiddenSelectionsTextures[] =
		{
			"DeutschZ_KotHZ_V2_Pro\data\items\deutschz_barrel_green_co.paa"
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

	class DeutschZ_FireworksLauncher : FireworksLauncher
	{
		scope = 2;
		displayName = "DeutschZ Feuerwerkswerfer";
		descriptionShort = "DeutschZ Feuerwerkswerfer mit 25-m-Nebelsignal.";
		hiddenSelectionsTextures[] =
		{
			"DeutschZ_KotHZ_V2_Pro\data\items\deutschz_fireworkslauncher_co.paa",
			"DeutschZ_KotHZ_V2_Pro\data\items\deutschz_fireworkslauncher_co.paa"
		};
	};

	class DeutschZ_Regeln : Paper
	{
		scope = 2;
		displayName = "DeutschZ Regeln";
		descriptionShort = "Die wichtigsten Regeln fuer DeutschZ. Lesen, verstehen und fair ueberleben.";
		hiddenSelections[] = {"zbytek"};
		hiddenSelectionsTextures[] =
		{
			"DeutschZ_KotHZ_V2_Pro\data\items\deutschz_regeln_loot_paper_co.paa"
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 15;
					healthLevels[] =
					{
						{1, {"DeutschZ_KotHZ_V2_Pro\data\items\deutschz_regeln.rvmat"}},
						{0.7, {"DeutschZ_KotHZ_V2_Pro\data\items\deutschz_regeln.rvmat"}},
						{0.5, {"DeutschZ_KotHZ_V2_Pro\data\items\deutschz_regeln.rvmat"}},
						{0.3, {"DeutschZ_KotHZ_V2_Pro\data\items\deutschz_regeln.rvmat"}},
						{0, {"DeutschZ_KotHZ_V2_Pro\data\items\deutschz_regeln.rvmat"}}
					};
				};
			};
		};
	};

	class DZKOTH_RewardBarrel : DeutschZ_Barrel_Green
	{
		scope = 0;
	};

	class DZKOTH_RewardChest : SeaChest
	{
		scope = 2;
		displayName = "DeutschZ KotH Reward Chest";
		descriptionShort = "Unbewegliche 500-Slot-Belohnungskiste des DeutschZ KotH Events.";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] =
		{
			"\DeutschZ_KotHZ_V2_Pro\data\textures\infectedsiedge\KothZ_FREE_sea_chest_co.paa"
		};
		class Cargo
		{
			itemsCargoSize[] = {10,50};
			openable = 0;
			allowOwnedCargoManipulation = 1;
		};
	};

	class DZKOTH_RewardCrate : DZKOTH_RewardChest
	{
		scope = 0;
	};

	class DZKOTH_BossCorpse : SeaChest
	{
		scope = 2;
		displayName = "BosZ Remains";
		descriptionShort = "Searchable remains of the BosZ Zombie. Story items can appear here.";
		itemsCargoSize[] = {10,50};
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] =
		{
			"DeutschZ_KotHZ_V2_Pro\data\textures\infectedsiedge\KOTH_sea_chest_co.paa"
		};
	};

	class DZKOTH_BattlegroundZ_Keycard : PunchedCard
	{
		scope = 2;
		displayName = "DeutschZ BattlegroundZ Keycard";
		descriptionShort = "An unstable story keycard connected to DeutschZ BattlegroundZ.";
		hiddenSelections[] = {"component01"};
		hiddenSelectionsTextures[] =
		{
			"\DeutschZ_KotHZ_V2_Pro\data\textures\infectedsiedge\DeutschZxKOTHZ_punchedcard_co.paa"
		};
	};

	class DZKOTH_EliteInfected : ZmbM_PolicemanSpecForce_Heavy
	{
		scope = 2;
		displayName = "DeutschZ KotH Elite Infected";
	};

	class DZKOTH_Infected_250 : ZmbM_HunterOld_Autumn
	{
		scope = 2;
		displayName = "DeutschZ KotH Hunter 250";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 250;
				};
			};
		};
	};

	class DZKOTH_Infected_400 : ZmbM_PolicemanSpecForce_Heavy
	{
		scope = 2;
		displayName = "DeutschZ KotH Police 400";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 400;
				};
			};
		};
	};

	class DZKOTH_Infected_600 : ZmbM_SoldierNormal_Base
	{
		scope = 2;
		displayName = "DeutschZ KotH Military 600";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 600;
				};
			};
		};
	};

	class DZKOTH_Infected_800 : ZmbM_SoldierNormal_Base
	{
		scope = 2;
		displayName = "DeutschZ KotH Military 800";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 800;
				};
			};
		};
	};

	class DZKOTH_Infected_1000 : ZmbM_SoldierNormal_Base
	{
		scope = 2;
		displayName = "DeutschZ KotH Military 1000";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 1000;
				};
			};
		};
	};

	class DZKOTH_BosZZombie : ZmbM_Mummy
	{
		scope = 2;
		displayName = "BosZ Zombie";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 7500;
				};
			};
		};
	};

	class DZKOTH_EventFlagpole : StaticFlagPole
	{
		// Runtime-spawnable; scope 0 makes DayZ treat the class as abstract.
		// Admin/territory interactions are removed in script instead.
		scope = 2;
		displayName = "DeutschZ K.o.t.H Eventmast";
		descriptionShort = "Serverseitiger Eventmast fuer DeutschZ K.o.t.H Infected Siege.";
		scopeCurator = 0;
		weight = 100000;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 999999;
				};
			};
			class GlobalArmor
			{
				class Projectile
				{
					class Health { damage = 0; };
					class Blood { damage = 0; };
					class Shock { damage = 0; };
				};
				class Melee
				{
					class Health { damage = 0; };
					class Blood { damage = 0; };
					class Shock { damage = 0; };
				};
				class FragGrenade
				{
					class Health { damage = 0; };
					class Blood { damage = 0; };
					class Shock { damage = 0; };
				};
			};
			class DamageZones {};
		};
	};

	class DZKOTH_EventFlag : Flag_Base
	{
		scope = 2;
		displayName = "DeutschZ K.o.t.H Eventflagge";
		descriptionShort = "Eventflagge fuer DeutschZ K.o.t.H Infected Siege.";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] =
		{
			"DeutschZ_KotHZ_V2_Pro\data\textures\infectedsiedge\KothZ_FREE_flag_dayz_co.paa"
		};
	};
};
