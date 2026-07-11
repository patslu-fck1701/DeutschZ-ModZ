class CfgPatches
{
	class DeutschZ_MenuMusic
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] =
		{
			"DZ_Data",
			"DZ_Scripts",
			"DZ_Sounds_Effects"
		};
	};
};

class CfgNoxZPhoneMusic
{
	tracks[] =
	{
		"Deutsch Z - Ich Bin Der King|185|DZKOTHG_Speaker_King_SoundSet",
		"Deutsch Z - DayZ Laeuft|138|DZKOTHG_Speaker_DayZLaeuft_SoundSet",
		"Deutsch Z - Chernarus Allein|202|DZKOTHG_Speaker_ChernarusAllein_SoundSet"
	};
};

class CfgSoundShaders
{
	class DZKOTHG_MenuMusic_Base_SoundShader
	{
		volume = 0.75;
	};
	class DZKOTHG_MenuMusic01_SoundShader : DZKOTHG_MenuMusic_Base_SoundShader { samples[] = {{"\DeutschZ_MenuMusic\sounds\music01", 1}}; };
	class DZKOTHG_MenuMusic02_SoundShader : DZKOTHG_MenuMusic_Base_SoundShader { samples[] = {{"\DeutschZ_MenuMusic\sounds\music02", 1}}; };
	class DZKOTHG_MenuMusic03_SoundShader : DZKOTHG_MenuMusic_Base_SoundShader { samples[] = {{"\DeutschZ_MenuMusic\sounds\music03", 1}}; };
	class DZKOTHG_MenuMusic04_SoundShader : DZKOTHG_MenuMusic_Base_SoundShader { samples[] = {{"\DeutschZ_MenuMusic\sounds\music04", 1}}; };
	class DZKOTHG_MenuMusic05_SoundShader : DZKOTHG_MenuMusic_Base_SoundShader { samples[] = {{"\DeutschZ_MenuMusic\sounds\music05", 1}}; };
	class DZKOTHG_MenuMusic06_SoundShader : DZKOTHG_MenuMusic_Base_SoundShader { samples[] = {{"\DeutschZ_MenuMusic\sounds\music06", 1}}; };
	class DZKOTHG_MenuMusic07_SoundShader : DZKOTHG_MenuMusic_Base_SoundShader { samples[] = {{"\DeutschZ_MenuMusic\sounds\music07", 1}}; };
	class DZKOTHG_MenuMusic08_SoundShader : DZKOTHG_MenuMusic_Base_SoundShader { samples[] = {{"\DeutschZ_MenuMusic\sounds\music08", 1}}; };
	class DZKOTHG_MenuMusic09_SoundShader : DZKOTHG_MenuMusic_Base_SoundShader { samples[] = {{"\DeutschZ_MenuMusic\sounds\music09", 1}}; };
	class DZKOTHG_MenuMusic10_SoundShader : DZKOTHG_MenuMusic_Base_SoundShader { samples[] = {{"\DeutschZ_MenuMusic\sounds\music10", 1}}; };
	class DZKOTHG_MenuMusic11_SoundShader : DZKOTHG_MenuMusic_Base_SoundShader { samples[] = {{"\DeutschZ_MenuMusic\sounds\music11", 1}}; };

	class DZKOTHG_Speaker_King_SoundShader
	{
		samples[] = {{"\DeutschZ_MenuMusic\sounds\music05", 1}};
		volume = 1.5;
		range = 100;
	};

	class DZKOTHG_Speaker_DayZLaeuft_SoundShader
	{
		samples[] = {{"\DeutschZ_MenuMusic\sounds\music03", 1}};
		volume = 1.5;
		range = 100;
	};

	class DZKOTHG_Speaker_ChernarusAllein_SoundShader
	{
		samples[] = {{"\DeutschZ_MenuMusic\sounds\music02", 1}};
		volume = 1.5;
		range = 100;
	};
};

class CfgSoundSets
{
	class DZKOTHG_MenuMusic_Base_SoundSet
	{
		volumeFactor = 1;
		frequencyFactor = 1;
		spatial = 0;
	};
	class DZKOTHG_MenuMusic01_SoundSet : DZKOTHG_MenuMusic_Base_SoundSet { soundShaders[] = {"DZKOTHG_MenuMusic01_SoundShader"}; };
	class DZKOTHG_MenuMusic02_SoundSet : DZKOTHG_MenuMusic_Base_SoundSet { soundShaders[] = {"DZKOTHG_MenuMusic02_SoundShader"}; };
	class DZKOTHG_MenuMusic03_SoundSet : DZKOTHG_MenuMusic_Base_SoundSet { soundShaders[] = {"DZKOTHG_MenuMusic03_SoundShader"}; };
	class DZKOTHG_MenuMusic04_SoundSet : DZKOTHG_MenuMusic_Base_SoundSet { soundShaders[] = {"DZKOTHG_MenuMusic04_SoundShader"}; };
	class DZKOTHG_MenuMusic05_SoundSet : DZKOTHG_MenuMusic_Base_SoundSet { soundShaders[] = {"DZKOTHG_MenuMusic05_SoundShader"}; };
	class DZKOTHG_MenuMusic06_SoundSet : DZKOTHG_MenuMusic_Base_SoundSet { soundShaders[] = {"DZKOTHG_MenuMusic06_SoundShader"}; };
	class DZKOTHG_MenuMusic07_SoundSet : DZKOTHG_MenuMusic_Base_SoundSet { soundShaders[] = {"DZKOTHG_MenuMusic07_SoundShader"}; };
	class DZKOTHG_MenuMusic08_SoundSet : DZKOTHG_MenuMusic_Base_SoundSet { soundShaders[] = {"DZKOTHG_MenuMusic08_SoundShader"}; };
	class DZKOTHG_MenuMusic09_SoundSet : DZKOTHG_MenuMusic_Base_SoundSet { soundShaders[] = {"DZKOTHG_MenuMusic09_SoundShader"}; };
	class DZKOTHG_MenuMusic10_SoundSet : DZKOTHG_MenuMusic_Base_SoundSet { soundShaders[] = {"DZKOTHG_MenuMusic10_SoundShader"}; };
	class DZKOTHG_MenuMusic11_SoundSet : DZKOTHG_MenuMusic_Base_SoundSet { soundShaders[] = {"DZKOTHG_MenuMusic11_SoundShader"}; };

	class DZKOTHG_Speaker_King_SoundSet
	{
		soundShaders[] = {"DZKOTHG_Speaker_King_SoundShader"};
	};

	class DZKOTHG_Speaker_DayZLaeuft_SoundSet
	{
		soundShaders[] = {"DZKOTHG_Speaker_DayZLaeuft_SoundShader"};
	};

	class DZKOTHG_Speaker_ChernarusAllein_SoundSet
	{
		soundShaders[] = {"DZKOTHG_Speaker_ChernarusAllein_SoundShader"};
	};
};

class CfgMods
{
	class DeutschZ_MenuMusic
	{
		dir = "DeutschZ_MenuMusic";
		picture = "DeutschZ_MenuMusic/gui/menu_assets/logos/Emblem.paa";
		action = "https://project23947.websitepublisher.ai/";
		name = "DeutschZ Menu Music";
		credits = "DeutschZ";
		author = "DeutschZ";
		authorID = "";
		version = "1.2.1";
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
					"DeutschZ_MenuMusic/scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				value = "";
				files[] =
				{
					"DeutschZ_MenuMusic/scripts/4_World"
				};
			};
			class missionScriptModule
			{
				value = "";
				files[] =
				{
					"DeutschZ_MenuMusic/scripts/5_Mission"
				};
			};
		};
	};
};
