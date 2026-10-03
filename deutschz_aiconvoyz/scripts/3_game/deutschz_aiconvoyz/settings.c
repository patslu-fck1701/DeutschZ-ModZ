class deutschz_aiconvoyz_loot_setting
{
    string class_name;
    int count;
    float chance_percent;
}

class deutschz_aiconvoyz_global_setting
{
    bool enabled;
    string blackbox_code = "1426";
    string helicrash_class = "RFFSHeli_Blackhawk_wreck";
    vector helicrash_position = "7200.386719 132.490768 12677.213867";
    vector helicrash_orientation = "0 0 0";
    vector blackbox_position = "7202.933105 131.886719 12678.544922";
    vector blackbox_orientation = "-72.242828 3.042660 13.986501";
    float destination_player_radius;
    float convoy_crew_capture_radius;
    int crew_capture_delay_seconds;
    int team_one_ai_count;
    int team_two_ai_count;
    string team_one_loadout;
    string team_two_loadout;
    int hack_seconds;
    int hostile_support_arrival_seconds;
    float combat_sound_max_distance;
    float combat_sound_min_volume;
    float helicrash_distance;
    ref array<ref deutschz_aiconvoyz_loot_setting> blackbox_loot;
}

class deutschz_aiconvoyz_settings
{
    int Version;
    ref deutschz_aiconvoyz_global_setting Global;
}

class deutschz_aiconvoyz_defaults
{
    static deutschz_aiconvoyz_loot_setting Loot(string className, int count, float chance)
    {
        deutschz_aiconvoyz_loot_setting value = new deutschz_aiconvoyz_loot_setting();
        value.class_name = className;
        value.count = count;
        value.chance_percent = chance;
        return value;
    }

    static deutschz_aiconvoyz_settings Create()
    {
        deutschz_aiconvoyz_settings root = new deutschz_aiconvoyz_settings();
        root.Version = 2;
        root.Global = new deutschz_aiconvoyz_global_setting();
        root.Global.enabled = true;
        root.Global.destination_player_radius = 300.0;
        root.Global.convoy_crew_capture_radius = 120.0;
        root.Global.crew_capture_delay_seconds = 5;
        root.Global.team_one_ai_count = 4;
        root.Global.team_two_ai_count = 4;
        root.Global.team_one_loadout = "WestLoadout";
        root.Global.team_two_loadout = "EastLoadout";
        root.Global.hack_seconds = 90;
        root.Global.hostile_support_arrival_seconds = 120;
        root.Global.combat_sound_max_distance = 1800.0;
        root.Global.combat_sound_min_volume = 0.05;
        root.Global.helicrash_distance = 500.0;
        root.Global.blackbox_loot = new array<ref deutschz_aiconvoyz_loot_setting>();
        root.Global.blackbox_loot.Insert(Loot("BandageDressing", 2, 100.0));
        root.Global.blackbox_loot.Insert(Loot("AmmoBox_556x45_20Rnd", 1, 75.0));
        root.Global.blackbox_loot.Insert(Loot("AmmoBox_762x39_20Rnd", 1, 75.0));
        root.Global.blackbox_loot.Insert(Loot("Morphine", 1, 50.0));
        return root;
    }
}
