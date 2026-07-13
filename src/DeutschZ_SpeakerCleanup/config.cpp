class CfgPatches
{
	class DeutschZ_SpeakerCleanup
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] =
		{
			"DZ_Data",
			"DZ_Scripts",
			"NoxZ_Speaker"
		};
	};
};

class CfgMods
{
	class DeutschZ_SpeakerCleanup
	{
		dir = "DeutschZ_SpeakerCleanup";
		name = "DeutschZ Speaker Cleanup";
		author = "DeutschZ";
		version = "1.0.0";
		type = "mod";
		dependencies[] = {"World"};

		class defs
		{
			class worldScriptModule
			{
				value = "";
				files[] = {"DeutschZ_SpeakerCleanup/scripts/4_World"};
			};
		};
	};
};
