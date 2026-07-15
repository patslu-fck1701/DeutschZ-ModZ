class CfgPatches
{
	class DeutschZ_KotHZ_Free
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] =
		{
			"DZ_Data",
			"DZ_Scripts"
		};
	};
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
		version = "1.0.0-phase-a";
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
