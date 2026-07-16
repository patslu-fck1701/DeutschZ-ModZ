class CfgPatches
{
	class DeutschZ_KotHZ_Free
	{
		units[] =
		{
			"DZKOTHF_EventFlagpole",
			"DZKOTHF_EventFlag",
			"DZKOTHF_RewardCrate"
		};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] =
		{
			"DZ_Data",
			"DZ_Scripts",
			"DZ_Gear_Camping",
			"DZ_Sounds_Effects"
		};
	};
};

class CfgSoundShaders
{
	class DZKOTHF_Music_Base_SoundShader { volume = 1; };
	class DZKOTHF_Music01_SoundShader: DZKOTHF_Music_Base_SoundShader { samples[] = {{"\DeutschZ_KotHZ_Free\sounds\track01", 1}}; };
	class DZKOTHF_Music02_SoundShader: DZKOTHF_Music_Base_SoundShader { samples[] = {{"\DeutschZ_KotHZ_Free\sounds\track02", 1}}; };
	class DZKOTHF_Music03_SoundShader: DZKOTHF_Music_Base_SoundShader { samples[] = {{"\DeutschZ_KotHZ_Free\sounds\track03", 1}}; };
	class DZKOTHF_Music04_SoundShader: DZKOTHF_Music_Base_SoundShader { samples[] = {{"\DeutschZ_KotHZ_Free\sounds\track04", 1}}; };
	class DZKOTHF_Music05_SoundShader: DZKOTHF_Music_Base_SoundShader { samples[] = {{"\DeutschZ_KotHZ_Free\sounds\track05", 1}}; };
	class DZKOTHF_Music06_SoundShader: DZKOTHF_Music_Base_SoundShader { samples[] = {{"\DeutschZ_KotHZ_Free\sounds\track06", 1}}; };
	class DZKOTHF_Music07_SoundShader: DZKOTHF_Music_Base_SoundShader { samples[] = {{"\DeutschZ_KotHZ_Free\sounds\track07", 1}}; };
	class DZKOTHF_Music08_SoundShader: DZKOTHF_Music_Base_SoundShader { samples[] = {{"\DeutschZ_KotHZ_Free\sounds\track08", 1}}; };
};

class CfgSoundSets
{
	class DZKOTHF_Music_Base_SoundSet { volumeFactor = 0.33; frequencyFactor = 1; spatial = 0; };
	class DZKOTHF_Music01_SoundSet: DZKOTHF_Music_Base_SoundSet { soundShaders[] = {"DZKOTHF_Music01_SoundShader"}; };
	class DZKOTHF_Music02_SoundSet: DZKOTHF_Music_Base_SoundSet { soundShaders[] = {"DZKOTHF_Music02_SoundShader"}; };
	class DZKOTHF_Music03_SoundSet: DZKOTHF_Music_Base_SoundSet { soundShaders[] = {"DZKOTHF_Music03_SoundShader"}; };
	class DZKOTHF_Music04_SoundSet: DZKOTHF_Music_Base_SoundSet { soundShaders[] = {"DZKOTHF_Music04_SoundShader"}; };
	class DZKOTHF_Music05_SoundSet: DZKOTHF_Music_Base_SoundSet { soundShaders[] = {"DZKOTHF_Music05_SoundShader"}; };
	class DZKOTHF_Music06_SoundSet: DZKOTHF_Music_Base_SoundSet { soundShaders[] = {"DZKOTHF_Music06_SoundShader"}; };
	class DZKOTHF_Music07_SoundSet: DZKOTHF_Music_Base_SoundSet { soundShaders[] = {"DZKOTHF_Music07_SoundShader"}; };
	class DZKOTHF_Music08_SoundSet: DZKOTHF_Music_Base_SoundSet { soundShaders[] = {"DZKOTHF_Music08_SoundShader"}; };
};

class CfgMods
{
	class DeutschZ_KotHZ_Free
	{
		dir = "DeutschZ_KotHZ_Free";
		name = "DeutschZ KotHZ Free";
		credits = "DeutschZ";
		author = "DeutschZ";
		authorID = "";
		version = "1.0.1-gameplayfix";
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
					"DeutschZ_KotHZ_Free/scripts/3_Game"
				};
			};

			class worldScriptModule
			{
				value = "";
				files[] =
				{
					"DeutschZ_KotHZ_Free/scripts/4_World"
				};
			};

			class missionScriptModule
			{
				value = "";
				files[] =
				{
					"DeutschZ_KotHZ_Free/scripts/5_Mission"
				};
			};
		};
	};
};

class CfgVehicles
{
	class StaticFlagPole;
	class Flag_Chernarus;
	class SeaChest;

	class DZKOTHF_EventFlagpole: StaticFlagPole
	{
		scope = 2;
		displayName = "DeutschZ KotHZ Mast";
		descriptionShort = "Serververwalteter KotHZ-Capture-Mast.";
	};

	class DZKOTHF_EventFlag: Flag_Chernarus
	{
		scope = 2;
		displayName = "DeutschZ KotHZ Fahne";
		descriptionShort = "Serververwaltete KotHZ-Capture-Fahne.";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"\DeutschZ_KotHZ_Free\data\kothz_free_flag_dayz_co.paa"};
	};

	class DZKOTHF_RewardCrate: SeaChest
	{
		scope = 2;
		displayName = "DeutschZ KotHZ Belohnungskiste";
		descriptionShort = "Serververwaltete KotHZ-Free-Belohnungskiste.";
		itemsCargoSize[] = {10, 50};
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"\DeutschZ_KotHZ_Free\data\kothz_free_sea_chest_co.paa"};
	};
};
