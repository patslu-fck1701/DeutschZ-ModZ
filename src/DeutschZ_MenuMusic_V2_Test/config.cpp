class CfgPatches
{
	class DeutschZ_MenuMusic_V2_Test
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

class CfgSoundShaders
{
	class DZKOTHG_MenuMusic_Base_SoundShader
	{
		volume = 1;
	};
	class DZKOTHG_MenuMusic01_SoundShader : DZKOTHG_MenuMusic_Base_SoundShader { samples[] = {{"\DeutschZ_MenuMusic_V2_Test\sounds\music01", 1}}; };
	class DZKOTHG_MenuMusic02_SoundShader : DZKOTHG_MenuMusic_Base_SoundShader { samples[] = {{"\DeutschZ_MenuMusic_V2_Test\sounds\music02", 1}}; };
	class DZKOTHG_MenuMusic03_SoundShader : DZKOTHG_MenuMusic_Base_SoundShader { samples[] = {{"\DeutschZ_MenuMusic_V2_Test\sounds\music03", 1}}; };
	class DZKOTHG_MenuMusic04_SoundShader : DZKOTHG_MenuMusic_Base_SoundShader { samples[] = {{"\DeutschZ_MenuMusic_V2_Test\sounds\music04", 1}}; };
	class DZKOTHG_MenuMusic05_SoundShader : DZKOTHG_MenuMusic_Base_SoundShader { samples[] = {{"\DeutschZ_MenuMusic_V2_Test\sounds\music05", 1}}; };
	class DZKOTHG_MenuMusic06_SoundShader : DZKOTHG_MenuMusic_Base_SoundShader { samples[] = {{"\DeutschZ_MenuMusic_V2_Test\sounds\music06", 1}}; };
	class DZKOTHG_MenuMusic07_SoundShader : DZKOTHG_MenuMusic_Base_SoundShader { samples[] = {{"\DeutschZ_MenuMusic_V2_Test\sounds\music07", 1}}; };
	class DZKOTHG_MenuMusic08_SoundShader : DZKOTHG_MenuMusic_Base_SoundShader { samples[] = {{"\DeutschZ_MenuMusic_V2_Test\sounds\music08", 1}}; };
	class DZKOTHG_MenuMusic09_SoundShader : DZKOTHG_MenuMusic_Base_SoundShader { samples[] = {{"\DeutschZ_MenuMusic_V2_Test\sounds\music09", 1}}; };
	class DZKOTHG_MenuMusic10_SoundShader : DZKOTHG_MenuMusic_Base_SoundShader { samples[] = {{"\DeutschZ_MenuMusic_V2_Test\sounds\music10", 1}}; };
	class DZKOTHG_MenuMusic11_SoundShader : DZKOTHG_MenuMusic_Base_SoundShader { samples[] = {{"\DeutschZ_MenuMusic_V2_Test\sounds\music11", 1}}; };
};

class CfgSoundSets
{
	class DZKOTHG_MenuMusic_Base_SoundSet
	{
		volumeFactor = 0.66;
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
};

class CfgMods
{
	class DeutschZ_MenuMusic_V2_Test
	{
		dir = "DeutschZ_MenuMusic_V2_Test";
		picture = "DeutschZ_MenuMusic_V2_Test/gui/menu_assets/logos/Emblem.paa";
		action = "https://project23947.websitepublisher.ai/";
		name = "DeutschZ Menu Music V2 Test";
		credits = "DeutschZ";
		author = "DeutschZ";
		authorID = "";
		version = "2.0.0-test";
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
					"DeutschZ_MenuMusic_V2_Test/scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				value = "";
				files[] =
				{
					"DeutschZ_MenuMusic_V2_Test/scripts/4_World"
				};
			};
			class missionScriptModule
			{
				value = "";
				files[] =
				{
					"DeutschZ_MenuMusic_V2_Test/scripts/5_Mission"
				};
			};
		};
	};
};
