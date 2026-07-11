class CfgPatches
{
	class DeutschZ_KOTH_InfectedSiege
	{
		units[] =
		{
			"DZKOTH_EventFlagpole",
			"DZKOTH_EventFlag",
			"DeutschZ_Barrel_Green",
			"DeutschZ_FireworksLauncher",
			"DZKOTH_RewardBarrel",
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
		samples[] = {{"\DeutschZ_KOTH_InfectedSiege\sounds\dzkoth_anthem", 1}};
		volume = 0.425;
	};
};

class CfgSoundSets
{
	class DZKOTH_EventMusic_SoundSet
	{
		soundShaders[] = {"DZKOTH_EventMusic_SoundShader"};
		volumeFactor = 1;
		frequencyFactor = 1;
		spatial = 0;
	};
};

class CfgMods
{
	class DeutschZ_KOTH_InfectedSiege
	{
		dir = "DeutschZ_KOTH_InfectedSiege";
		name = "DeutschZ K.o.t.H Infected Siege v2";
		credits = "DeutschZ";
		author = "DeutschZ";
		authorID = "";
		version = "0.4.0";
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
					"DeutschZ_KOTH_InfectedSiege/scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				value = "";
				files[] =
				{
					"DeutschZ_KOTH_InfectedSiege/scripts/4_World"
				};
			};
			class missionScriptModule
			{
				value = "";
				files[] =
				{
					"DeutschZ_KOTH_InfectedSiege/scripts/5_Mission"
				};
			};
		};
	};
};

class CfgVehicles
{
	class TerritoryFlag;
	class StaticFlagPole;
	class BaseBuildingBase;
	class Flag_Base;
	class PunchedCard;
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
			"DeutschZ_KOTH_InfectedSiege\data\items\deutschz_barrel_green_co.paa"
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
			"DeutschZ_KOTH_InfectedSiege\data\items\deutschz_fireworkslauncher_co.paa",
			"DeutschZ_KOTH_InfectedSiege\data\items\deutschz_fireworkslauncher_co.paa"
		};
	};

	class DZKOTH_RewardBarrel : DeutschZ_Barrel_Green
	{
		scope = 2;
		displayName = "DeutschZ KotH Reward Fass";
		descriptionShort = "Unbewegliches 500-Slot-Reward-Fass des DeutschZ KotH Events.";
	};

	class DZKOTH_RewardCrate : SeaChest
	{
		scope = 0;
		displayName = "DeutschZ KotH Belohnungskiste";
		descriptionShort = "Belohnungskiste nach erfolgreicher KotH-Eroberung.";
		itemsCargoSize[] = {10,50};
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] =
		{
			"DeutschZ_KOTH_InfectedSiege\data\textures\infectedsiedge\KOTH_sea_chest_co.paa"
		};
	};

	class DZKOTH_BossCorpse : SeaChest
	{
		scope = 0;
		displayName = "BosZ Remains";
		descriptionShort = "Searchable remains of the BosZ Zombie. Story items can appear here.";
		itemsCargoSize[] = {10,50};
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] =
		{
			"DeutschZ_KOTH_InfectedSiege\data\textures\infectedsiedge\KOTH_sea_chest_co.paa"
		};
	};

	class DZKOTH_BattlegroundZ_Keycard : PunchedCard
	{
		scope = 0;
		displayName = "DeutschZ BattlegroundZ Keycard";
		descriptionShort = "An unstable story keycard connected to DeutschZ BattlegroundZ.";
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
				class Health { hitpoints = 250; };
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
				class Health { hitpoints = 400; };
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
				class Health { hitpoints = 600; };
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
				class Health { hitpoints = 800; };
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
				class Health { hitpoints = 1000; };
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
				class Health { hitpoints = 7500; };
			};
		};
	};

	class DZKOTH_EventFlagpole : StaticFlagPole
	{
		scope = 2;
		displayName = "DeutschZ K.o.t.H Eventmast";
		descriptionShort = "Serverseitiger Eventmast fuer DeutschZ K.o.t.H Infected Siege.";
		scopeCurator = 2;
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

	class DZKOTH_EventTerminal : SeaChest
	{
		scope = 0;
		displayName = "DeutschZ KotH Versorgungskiste";
		descriptionShort = "Eventkiste. Nach gesicherter Zone 60 Sekunden aufbrechen, um den Fahnenmast zu aktivieren.";
		itemsCargoSize[] = {10,50};
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] =
		{
			"DeutschZ_KOTH_InfectedSiege\data\textures\infectedsiedge\KOTH_sea_chest_co.paa"
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
			"DeutschZ_KOTH_InfectedSiege\data\textures\infectedsiedge\KothZFlag_flag_white_co.paa"
		};
	};

	class DZEV_KOTH_Flagpole : TerritoryFlag
	{
		scope = 2;
		displayName = "DeutschZ K.o.t.H Flagpole";
		descriptionShort = "Event flagpole for DeutschZ K.o.t.H Infected Siege.";
	};

	class DZEV_KOTH_Flag : Flag_Base
	{
		scope = 2;
		displayName = "DeutschZ K.o.t.H Flag";
		descriptionShort = "Flag for the Infected Siege event.";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] =
		{
			"DeutschZ_KOTH_InfectedSiege\data\textures\infectedsiedge\KothZFlag_flag_white_co.paa"
		};
	};

	class DZEV_KOTH_KeyCard : PunchedCard
	{
		scope = 0;
		displayName = "DeutschZ K.o.t.H Keycard";
		descriptionShort = "Event keycard for the Infected Siege encounter.";
	};

	class DZEV_KOTH_SeaChest : SeaChest
	{
		scope = 0;
		displayName = "DeutschZ K.o.t.H Siege Chest";
		descriptionShort = "Reward chest for the Infected Siege event.";
		itemsCargoSize[] = {10,50};
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] =
		{
			"DeutschZ_KOTH_InfectedSiege\data\textures\infectedsiedge\KOTH_sea_chest_co.paa"
		};
	};

	class DZEV_Infected_Stage2 : ZmbM_PolicemanSpecForce_Heavy
	{
		scope = 2;
		displayName = "DeutschZ Siege Infected";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 220;
				};
			};
		};
	};

	class DZEV_Infected_Stage3 : ZmbM_Mummy
	{
		scope = 2;
		displayName = "DeutschZ Heavy Siege Infected";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 500;
				};
			};
		};
	};

	class DZEV_Boss_Mummy : ZmbM_Mummy
	{
		scope = 2;
		displayName = "DeutschZ Bozz Zombie";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 7500;
				};
			};
			class DamageZones
			{
				class Head
				{
					class Health
					{
						hitpoints = 2500;
					};
				};
				class Torso
				{
					class Health
					{
						hitpoints = 7500;
					};
				};
				class LeftArm
				{
					class Health
					{
						hitpoints = 5000;
					};
				};
				class RightArm
				{
					class Health
					{
						hitpoints = 5000;
					};
				};
				class LeftLeg
				{
					class Health
					{
						hitpoints = 5000;
					};
				};
				class RightLeg
				{
					class Health
					{
						hitpoints = 5000;
					};
				};
			};
		};
	};
};
