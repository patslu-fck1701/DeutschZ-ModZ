class CfgPatches
{
	class DeutschZ_KotHZ_Free
	{
		units[] =
		{
			"DZKOTHF_EventFlagpole",
			"DZKOTHF_EventFlag"
		};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] =
		{
			"DZ_Data",
			"DZ_Scripts",
			"DZ_Gear_Camping"
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
		version = "1.0.0-phase-b";
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
	};
};
