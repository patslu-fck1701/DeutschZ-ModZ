class CfgPatches
{
    class DeutschZ_RoamingBlackmarket_Adapter
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] =
        {
            "DZ_Data",
            "DZ_Scripts",
			"DZ_Vehicles_Wheeled",
			"DF_Scripts",
			"DayZExpansion_Market_Scripts",
            "DayZExpansion_AI_Scripts",
            "DayZExpansion_Navigation_Scripts",
			"DeutschZ_PointsZ"
        };
    };
};

class CfgMods
{
    class DeutschZ_RoamingBlackmarket_Adapter
    {
        dir = "deutschz_roamingblackmarket_adapter";
        name = "DeutschZ Roaming Blackmarket Adapter";
        author = "DeutschZ";
        version = "2.0.0-mobile";
        type = "mod";
        dependencies[] = {"Game", "World", "Mission"};
        class defs
        {
            class gameScriptModule
            {
                value = "";
                files[] = {"deutschz_roamingblackmarket_adapter/scripts/3_Game"};
            };
            class worldScriptModule
            {
                value = "";
                files[] = {"deutschz_roamingblackmarket_adapter/scripts/4_World"};
            };
            class missionScriptModule
            {
                value = "";
                files[] = {"deutschz_roamingblackmarket_adapter/scripts/5_Mission"};
            };
        };
    };
};

class CfgSoundShaders
{
    class DZRB_BlackMart_Shader { samples[] = {{"\deutschz_roamingblackmarket_adapter\sounds\BlackMart", 1}}; volume = 1; };
    class DZRB_BlackMart1_Shader { samples[] = {{"\deutschz_roamingblackmarket_adapter\sounds\BlackMart1", 1}}; volume = 1; };
    class DZRB_BlackMart2_Shader { samples[] = {{"\deutschz_roamingblackmarket_adapter\sounds\BlackMart2", 1}}; volume = 1; };
};

class CfgSoundSets
{
    class DZRB_BlackMart_SoundSet { soundShaders[] = {"DZRB_BlackMart_Shader"}; spatial = 0; volumeFactor = 1; frequencyFactor = 1; };
    class DZRB_BlackMart1_SoundSet { soundShaders[] = {"DZRB_BlackMart1_Shader"}; spatial = 0; volumeFactor = 1; frequencyFactor = 1; };
    class DZRB_BlackMart2_SoundSet { soundShaders[] = {"DZRB_BlackMart2_Shader"}; spatial = 0; volumeFactor = 1; frequencyFactor = 1; };
};
