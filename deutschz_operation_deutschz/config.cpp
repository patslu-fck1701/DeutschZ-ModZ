class CfgPatches
{
    class DeutschZ_Operation_DeutschZ
    {
        units[] =
        {
            "DeutschZ_OperationDeutschZ_MasterCardReader",
            "DeutschZ_OperationDeutschZ_Terminal",
            "DeutschZ_OperationDeutschZ_RewardChest"
        };
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] =
        {
            "DZ_Data",
            "DZ_Scripts",
            "DZ_Gear_Navigation",
            "DZ_Gear_Containers",
            "DZ_Structures_Specific",
            "DeutschZ_BattlegroundZ"
        };
    };
};

class CfgMods
{
    class DeutschZ_Operation_DeutschZ
    {
        dir = "deutschz_operation_deutschz";
        name = "DeutschZ Operation DeutschZ";
        author = "DeutschZ";
        version = "1.0.0";
        type = "mod";
        dependencies[] = {"Game", "World", "Mission"};
        class defs
        {
            class gameScriptModule
            {
                value = "";
                files[] = {"deutschz_operation_deutschz/scripts/3_Game"};
            };
            class worldScriptModule
            {
                value = "";
                files[] = {"deutschz_operation_deutschz/scripts/4_World"};
            };
            class missionScriptModule
            {
                value = "";
                files[] = {"deutschz_operation_deutschz/scripts/5_Mission"};
            };
        };
    };
};

class CfgVehicles
{
    class GPSReceiver;
    class SeaChest;
    class Land_Radio_PanelBig;

    class DeutschZ_OperationDeutschZ_MasterCardReader : GPSReceiver
    {
        scope = 2;
        displayName = "Operation-DeutschZ-MasterCardReader";
        descriptionShort = "Vollstaendiger Nachweis aus ConvoyZ und BattlegroundZ zur Autorisierung von Operation DeutschZ.";
    };

    class DeutschZ_OperationDeutschZ_Terminal : Land_Radio_PanelBig
    {
        scope = 2;
        displayName = "Operation-DeutschZ-Terminal";
        descriptionShort = "Terminal zur Aktivierung der finalen Operation DeutschZ.";
    };

    class DeutschZ_OperationDeutschZ_RewardChest : SeaChest
    {
        scope = 2;
        displayName = "Operation DeutschZ Belohnungskiste";
        descriptionShort = "Belohnung fuer den erfolgreichen Abschluss von Operation DeutschZ.";
        class Cargo
        {
            itemsCargoSize[] = {10, 50};
            openable = 0;
            allowOwnedCargoManipulation = 1;
        };
    };
};
