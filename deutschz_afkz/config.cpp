class CfgPatches
{
    class deutschz_afkz
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] = {"DZ_Data", "DZ_Scripts"};
    };
};

class CfgMods
{
    class deutschz_afkz
    {
        dir = "deutschz_afkz";
        name = "DeutschZ AFKZ";
        author = "DeutschZ";
        type = "mod";
        dependencies[] = {"Game", "World", "Mission"};
        class defs
        {
            class gameScriptModule { files[] = {"deutschz_afkz/scripts/3_game"}; };
            class worldScriptModule { files[] = {"deutschz_afkz/scripts/4_world"}; };
            class missionScriptModule { files[] = {"deutschz_afkz/scripts/5_mission"}; };
        };
    };
};

class CfgSoundShaders
{
    class deutschz_afkz_dance_shader
    {
        samples[] = {{"\deutschz_afkz\sounds\afk_tanz_mal", 1}};
        range = 1;
        volume = 1;
    };
};

class CfgSoundSets
{
    class deutschz_afkz_dance_soundset
    {
        soundShaders[] = {"deutschz_afkz_dance_shader"};
        spatial = 0;
        volumeFactor = 1;
        frequencyFactor = 1;
    };
};
