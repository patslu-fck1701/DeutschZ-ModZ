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
	class DZKOTHG_MenuMusic_SoundShader
	{
		samples[] =
		{
			{"\DeutschZ_MenuMusic\sounds\music01", 1},
			{"\DeutschZ_MenuMusic\sounds\music02", 1},
			{"\DeutschZ_MenuMusic\sounds\music03", 1},
			{"\DeutschZ_MenuMusic\sounds\music04", 1},
			{"\DeutschZ_MenuMusic\sounds\music05", 1},
			{"\DeutschZ_MenuMusic\sounds\music06", 1},
			{"\DeutschZ_MenuMusic\sounds\music07", 1},
			{"\DeutschZ_MenuMusic\sounds\music08", 1},
			{"\DeutschZ_MenuMusic\sounds\music09", 1},
			{"\DeutschZ_MenuMusic\sounds\music10", 1},
			{"\DeutschZ_MenuMusic\sounds\music11", 1}
		};
		volume = 0.75;
	};

	class DZ_Menu_Shader: DZKOTHG_MenuMusic_SoundShader {};
	class GC_Menu_Shader: DZKOTHG_MenuMusic_SoundShader {};

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
	class DZKOTHG_MenuMusic_SoundSet
	{
		soundShaders[] =
		{
			"DZKOTHG_MenuMusic_SoundShader"
		};
		volumeFactor = 1;
		frequencyFactor = 1;
		spatial = 0;
	};

	class DZ_Menu_SoundSet: DZKOTHG_MenuMusic_SoundSet {};
	class GC_Menu_SoundSet: DZKOTHG_MenuMusic_SoundSet {};

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
		version = "1.0.0";
		type = "mod";
		dependencies[] =
		{
			"Game",
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
