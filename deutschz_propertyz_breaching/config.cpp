class CfgPatches
{
    class deutschz_propertyz_breaching
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 1.0;
        requiredAddons[] = {"deutschz_propertyz", "HDSN_BreachingCharge"};
    };
};

class CfgMods
{
    class deutschz_propertyz_breaching
    {
        dir = "deutschz_propertyz_breaching";
        hideName = 1;
        name = "DeutschZ PropertyZ Breaching Adapter";
        author = "DeutschZ";
        type = "mod";
        dependencies[] = {"World"};
        class defs
        {
            class worldScriptModule
            {
                value = "";
                files[] = {"deutschz_propertyz_breaching/scripts/4_World"};
            };
        };
    };
};
