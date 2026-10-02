class CfgPatches
{
    class deutschz_welcomez
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] = {"DZ_Data", "DZ_Scripts", "deutschz_warningz"};
    };
};

class CfgMods
{
    class deutschz_welcomez
    {
        dir = "deutschz_welcomez";
        name = "DeutschZ WelcomeZ";
        author = "DeutschZ";
        type = "mod";
        dependencies[] = {"Game", "World", "Mission"};
        inputs = "deutschz_welcomez/inputs.xml";
        class defs
        {
            class gameScriptModule { files[] = {"deutschz_welcomez/scripts/3_game"}; };
            class worldScriptModule { files[] = {"deutschz_welcomez/scripts/4_world"}; };
            class missionScriptModule
            {
                files[] = {"deutschz_welcomez/scripts/5_mission"};
            };
        };
    };
};

class CfgSoundShaders
{
    class deutschz_welcomez_music_shader
    {
        samples[] = {{"\deutschz_welcomez\sounds\willkommen_bei_deutschz", 1}};
        range = 1;
        volume = 0.75;
    };
    class deutschz_welcomez_morozov_shader
    {
        samples[] = {{"\deutschz_welcomez\sounds\morozov_895", 1}};
        range = 1;
        volume = 0.75;
    };
    class deutschz_welcomez_king_shader
    {
        samples[] = {{"\deutschz_welcomez\sounds\ich_bin_der_king", 1}};
        range = 1;
        volume = 0.75;
    };
};

class CfgSoundSets
{
    class deutschz_welcomez_music_soundset
    {
        soundShaders[] = {"deutschz_welcomez_music_shader"};
        spatial = 0;
        volumeFactor = 1;
        frequencyFactor = 1;
    };
    class deutschz_welcomez_morozov_soundset
    {
        soundShaders[] = {"deutschz_welcomez_morozov_shader"};
        spatial = 0;
        volumeFactor = 1;
        frequencyFactor = 1;
    };
    class deutschz_welcomez_king_soundset
    {
        soundShaders[] = {"deutschz_welcomez_king_shader"};
        spatial = 0;
        volumeFactor = 1;
        frequencyFactor = 1;
    };
};
