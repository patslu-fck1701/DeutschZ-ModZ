class deutschz_afkz_rpc
{
    static const int activity = 5519021;
    static const int state = 5519022;
};

class deutschz_afkz_client_state
{
    static bool pending;
    static bool active;
    static float sound_volume = 0.8;
};

class deutschz_afkz_settings
{
    int Version = 1;
    int AfkSeconds = 300;
    float GroundVehicleStopSpeed = 0.45;
    float HelicopterHorizontalStopSpeed = 0.35;
    float HelicopterLandingHeight = 0.8;
    float HelicopterDescentSpeed = 0.8;
    float SoundVolume = 0.8;

    void validate()
    {
        Version = 1;
        AfkSeconds = Math.Clamp(AfkSeconds, 60, 3600);
        GroundVehicleStopSpeed = Math.Clamp(GroundVehicleStopSpeed, 0.1, 2.0);
        HelicopterHorizontalStopSpeed = Math.Clamp(HelicopterHorizontalStopSpeed, 0.1, 2.0);
        HelicopterLandingHeight = Math.Clamp(HelicopterLandingHeight, 0.3, 2.0);
        HelicopterDescentSpeed = Math.Clamp(HelicopterDescentSpeed, 0.2, 2.0);
        SoundVolume = Math.Clamp(SoundVolume, 0.0, 1.0);
    }
};

class deutschz_afkz_settings_loader
{
    static const string directory = "$profile:DeutschZ-System/DeutschZ_AFKZ";
    static const string path = "$profile:DeutschZ-System/DeutschZ_AFKZ/settings.json";

    static deutschz_afkz_settings load()
    {
        deutschz_afkz_settings settings = new deutschz_afkz_settings();
        if (FileExist(path))
            JsonFileLoader<deutschz_afkz_settings>.JsonLoadFile(path, settings);
        settings.validate();
        if (!FileExist(directory))
        {
            if (!FileExist("$profile:DeutschZ-System"))
                MakeDirectory("$profile:DeutschZ-System");
            MakeDirectory(directory);
        }
        JsonFileLoader<deutschz_afkz_settings>.JsonSaveFile(path, settings);
        return settings;
    }
};
