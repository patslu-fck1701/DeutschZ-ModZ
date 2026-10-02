class CfgPatches
{
    class DeutschZ_FuelZ
    {
        units[] = {"DZ_FuelZ_NPC"};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] =
        {
            "DZ_Data",
            "DZ_Scripts",
            "DZ_Characters",
            "DayZExpansion_Dta_Core",
            "DayZExpansion_Core_Scripts",
            "DayZExpansion_Market_Scripts"
        };
    };
};

class CfgMods
{
    class DeutschZ_FuelZ
    {
        dir = "deutschz_fuelz";
        name = "DeutschZ FuelZ";
        credits = "DeutschZ";
        author = "DeutschZ";
        version = "0.1.2";
        type = "mod";
        dependencies[] = {"Game", "World", "Mission"};

        class defs
        {
            class gameScriptModule
            {
                value = "";
                files[] = {"deutschz_fuelz/scripts/3_game"};
            };
            class worldScriptModule
            {
                value = "";
                files[] = {"deutschz_fuelz/scripts/4_world"};
            };
            class missionScriptModule
            {
                value = "";
                files[] = {"deutschz_fuelz/scripts/5_mission"};
            };
        };
    };
};

class CfgVehicles
{
    class SurvivorM_Mirek;

    class DZ_FuelZ_NPC : SurvivorM_Mirek
    {
        scope = 2;
        displayName = "Tankwart";
        descriptionShort = "Verkauft Benzin gegen Euro.";
    };
};
