class CfgPatches
{
	class DeutschZ_UiAdminZ
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data", "DZ_Scripts"};
	};
};

class CfgMods
{
	class DeutschZ_UiAdminZ
	{
		dir = "DeutschZ_UiAdminZ";
		name = "DeutschZ UiAdminZ";
		author = "DeutschZ";
		version = "2.0.0-admin-pro-shell";
		type = "mod";
		dependencies[] = {"Game", "World", "Mission"};
		class defs
		{
			class gameScriptModule {value = ""; files[] = {"DeutschZ_UiAdminZ/scripts/3_Game"};};
			class worldScriptModule {value = ""; files[] = {"DeutschZ_UiAdminZ/scripts/4_World"};};
			class missionScriptModule {value = ""; files[] = {"DeutschZ_UiAdminZ/scripts/5_Mission"};};
		};
	};
};
