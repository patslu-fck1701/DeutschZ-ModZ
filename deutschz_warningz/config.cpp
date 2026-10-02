class cfgpatches
{
    class deutschz_warningz
    {
        units[] = {};
        weapons[] = {};
        requiredversion = 0.1;
        requiredaddons[] =
        {
            "dz_data",
            "dayzexpansion_core"
        };
    };
};

class cfgmods
{
    class deutschz_warningz
    {
        dir = "deutschz_warningz";
        name = "deutschz_warningz";
        author = "deutschz";
        type = "mod";

        dependencies[] =
        {
            "game",
            "world",
            "mission"
        };

        class defs
        {
            class gameScriptModule
            {
                files[] =
                {
                    "deutschz_warningz/scripts/3_game"
                };
            };
            class missionScriptModule
            {
                files[] =
                {
                    "deutschz_warningz/scripts/5_mission"
                };
            };
        };
    };
};

class cfgsoundshaders
{
    class deutschz_warningz_welcomeevents_shader
    {
        samples[] =
        {
            {
                "deutschz_warningz\sounds\welcomeevents.ogg",
                1
            }
        };
        volume = 1;
    };

    class deutschz_warningz_restart15min_shader
    {
        samples[] =
        {
            {
                "deutschz_warningz\sounds\restart15min.ogg",
                1
            }
        };
        volume = 1;
    };

    class deutschz_warningz_restart2min_shader
    {
        samples[] =
        {
            {
                "deutschz_warningz\sounds\restart2min.ogg",
                1
            }
        };
        volume = 1;
    };

    class deutschz_warningz_safezonewarning_shader
    {
        samples[] =
        {
            {
                "deutschz_warningz\sounds\safezonewarning.ogg",
                1
            }
        };
        volume = 1;
    };

    class deutschz_warningz_welcome1_shader { samples[] = {{"deutschz_warningz\sounds\wilkommen.ogg",1}}; volume = 1; };
    class deutschz_warningz_welcome2_shader { samples[] = {{"deutschz_warningz\sounds\wilkommen2.ogg",1}}; volume = 1; };
    class deutschz_warningz_safezone1_shader { samples[] = {{"deutschz_warningz\sounds\warnungsafezone.ogg",1}}; volume = 1; };
    class deutschz_warningz_safezone2_shader { samples[] = {{"deutschz_warningz\sounds\warnungsafezone2.ogg",1}}; volume = 1; };
    class deutschz_warningz_death1_shader { samples[] = {{"deutschz_warningz\sounds\tod.ogg",1}}; volume = 1; };
    class deutschz_warningz_death2_shader { samples[] = {{"deutschz_warningz\sounds\tod2.ogg",1}}; volume = 1; };
    class deutschz_warningz_death3_shader { samples[] = {{"deutschz_warningz\sounds\tod3.ogg",1}}; volume = 1; };
    class deutschz_warningz_logout1_shader { samples[] = {{"deutschz_warningz\sounds\bisbald.ogg",1}}; volume = 1; };
    class deutschz_warningz_logout2_shader { samples[] = {{"deutschz_warningz\sounds\dubistraus.ogg",1}}; volume = 1; };
    class deutschz_warningz_logout3_shader { samples[] = {{"deutschz_warningz\sounds\verbindunggetrennt.ogg",1}}; volume = 1; };
};

class cfgsoundsets
{
    class deutschz_warningz_base_soundset
    {
        sound3dprocessingtype = "character3dprocessingtype";
        spatial = 0;
        doppler = 0;
        loop = 0;
        volumeFactor = 1;
    };

    class deutschz_warningz_welcomeevents_soundset: deutschz_warningz_base_soundset
    {
        soundshaders[] =
        {
            "deutschz_warningz_welcomeevents_shader"
        };
    };

    class deutschz_warningz_restart15min_soundset: deutschz_warningz_base_soundset
    {
        soundshaders[] =
        {
            "deutschz_warningz_restart15min_shader"
        };
    };

    class deutschz_warningz_restart2min_soundset: deutschz_warningz_base_soundset
    {
        soundshaders[] =
        {
            "deutschz_warningz_restart2min_shader"
        };
    };

    class deutschz_warningz_safezonewarning_soundset: deutschz_warningz_base_soundset
    {
        soundshaders[] =
        {
            "deutschz_warningz_safezonewarning_shader"
        };
    };

    class deutschz_warningz_welcome1_soundset: deutschz_warningz_base_soundset { soundshaders[] = {"deutschz_warningz_welcome1_shader"}; };
    class deutschz_warningz_welcome2_soundset: deutschz_warningz_base_soundset { soundshaders[] = {"deutschz_warningz_welcome2_shader"}; };
    class deutschz_warningz_safezone1_soundset: deutschz_warningz_base_soundset { soundshaders[] = {"deutschz_warningz_safezone1_shader"}; };
    class deutschz_warningz_safezone2_soundset: deutschz_warningz_base_soundset { soundshaders[] = {"deutschz_warningz_safezone2_shader"}; };
    class deutschz_warningz_death1_soundset: deutschz_warningz_base_soundset { soundshaders[] = {"deutschz_warningz_death1_shader"}; };
    class deutschz_warningz_death2_soundset: deutschz_warningz_base_soundset { soundshaders[] = {"deutschz_warningz_death2_shader"}; };
    class deutschz_warningz_death3_soundset: deutschz_warningz_base_soundset { soundshaders[] = {"deutschz_warningz_death3_shader"}; };
    class deutschz_warningz_logout1_soundset: deutschz_warningz_base_soundset { soundshaders[] = {"deutschz_warningz_logout1_shader"}; };
    class deutschz_warningz_logout2_soundset: deutschz_warningz_base_soundset { soundshaders[] = {"deutschz_warningz_logout2_shader"}; };
    class deutschz_warningz_logout3_soundset: deutschz_warningz_base_soundset { soundshaders[] = {"deutschz_warningz_logout3_shader"}; };
};
