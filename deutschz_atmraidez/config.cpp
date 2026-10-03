class CfgPatches
{
    class DeutschZ_ATMRaideZ
    {
        units[] = {"DZATM_RobTool"};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] =
        {
            "DZ_Data",
            "DZ_Scripts",
            "DZ_Gear_Tools",
            "DayZExpansion_Core_Scripts",
            "DayZExpansion_Navigation_Scripts",
            "DayZExpansion_Market_Scripts",
            "DayZExpansion_Market_Objects"
        };
    };
};

class CfgMods
{
    class DeutschZ_ATMRaideZ
    {
        dir = "deutschz_atmraidez";
        name = "DeutschZ ATM RaideZ";
        credits = "DeutschZ";
        author = "DeutschZ";
        authorID = "";
        version = "0.1.8";
        type = "mod";
        dependencies[] = {"Game", "World", "Mission"};

        class defs
        {
            class gameScriptModule
            {
                value = "";
                files[] = {"deutschz_atmraidez/scripts/3_game"};
            };
            class worldScriptModule
            {
                value = "";
                files[] = {"deutschz_atmraidez/scripts/4_world"};
            };
            class missionScriptModule
            {
                value = "";
                files[] = {"deutschz_atmraidez/scripts/5_mission"};
            };
        };
    };
};

class CfgVehicles
{
    class Crowbar;

    class DZATM_RobTool : Crowbar
    {
        scope = 2;
        displayName = "DeutschZ ATM Brechstange";
        descriptionShort = "Verstaerkte Brechstange fuer ATM-Ueberfaelle.";
        model = "\dz\gear\tools\Crowbar.p3d";
        hiddenSelections[] = {"zbytek"};
        hiddenSelectionsTextures[] = {"deutschz_atmraidez\paa\atmraidez\robtool_crowbar_co.paa"};
    };
};
