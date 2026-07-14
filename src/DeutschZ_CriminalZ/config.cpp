class CfgPatches
{
    class DeutschZ_CriminalZ
    {
        units[] =
        {
            "DZCRZ_ATM",
            "RobTool_Crowbar",
            "HackingTool_Lockpick",
            "CriminalZ_Token",
            "DZCRZ_StoreInfoSign"
        };
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] =
        {
            "DZ_Data",
            "DZ_Scripts",
            "DZ_Gear_Tools",
            "DZ_Structures_Specific",
            "NoxZCurrency",
            "DayZExpansion_Core_Scripts",
            "DayZExpansion_Navigation_Scripts",
            "DayZExpansion_Vehicles_Scripts",
            "DayZExpansion_Garage_Scripts",
            "DayZExpansion_Market_Scripts",
            "DayZExpansion_Market_Objects"
        };
    };
};

class CfgMods
{
    class DeutschZ_CriminalZ
    {
        dir = "DeutschZ_CriminalZ";
        name = "DeutschZ CriminalZ";
        credits = "DeutschZ";
        author = "DeutschZ";
        authorID = "";
        version = "0.1.0";
        type = "mod";
        dependencies[] = {"Game", "World", "Mission"};
        class defs
        {
            class gameScriptModule
            {
                value = "";
                files[] = {"DeutschZ_CriminalZ/scripts/3_Game"};
            };
            class worldScriptModule
            {
                value = "";
                files[] = {"DeutschZ_CriminalZ/scripts/4_World"};
            };
            class missionScriptModule
            {
                value = "";
                files[] = {"DeutschZ_CriminalZ/scripts/5_Mission"};
            };
        };
    };
};

class CfgVehicles
{
    class ExpansionATM_2;
    class Crowbar;
    class Lockpick;
    class Inventory_Base;
    class StaticObj_FueldStation_Sign;

    class DZCRZ_ATM : ExpansionATM_2
    {
        scope = 2;
        displayName = "DeutschZ CriminalZ ATM";
        descriptionShort = "Ein vernetzter Geldautomat. Kriminelle Aktivitaeten werden global gemeldet.";
    };

    class RobTool_Crowbar : Crowbar
    {
        scope = 2;
        displayName = "CriminalZ Brechstange";
        descriptionShort = "Eine verstaerkte Brechstange fuer CriminalZ ATM-Ueberfaelle.";
        hiddenSelections[] = {"zbytek"};
        hiddenSelectionsTextures[] = {"\DeutschZ_CriminalZ\PAA\CriminalZ\RobTool_crowbar_co.paa"};
    };

    class HackingTool_Lockpick : Lockpick
    {
        scope = 2;
        displayName = "CriminalZ Hacking-Lockpick";
        descriptionShort = "Ein manipuliertes Schlosswerkzeug fuer Fahrzeug- und Garage-Hacks.";
        hiddenSelections[] = {"zbytek"};
        hiddenSelectionsTextures[] = {"\DeutschZ_CriminalZ\PAA\CriminalZ\HackingTool_lockpick_co.paa"};
    };

    class CriminalZ_Token : Inventory_Base
    {
        scope = 2;
        displayName = "CriminalZ Token";
        descriptionShort = "Ein unbekannter Token aus dem CriminalZ-Netzwerk. Zukuenftige Funktion vorbereitet.";
        model = "\dz\gear\consumables\Paper.p3d";
        itemSize[] = {1, 1};
        weight = 1;
    };

    class DZCRZ_StoreInfoSign : StaticObj_FueldStation_Sign
    {
        scope = 2;
        displayName = "DeutschZ CriminalZ Hinweisschild";
        descriptionShort = "Hinweisschild fuer die DeutschZ CriminalZ Aktivitaeten an City Stores.";
        model = "\DeutschZ_CriminalZ\models\DZCRZ_FuelStation_Sign.p3d";
    };
};
