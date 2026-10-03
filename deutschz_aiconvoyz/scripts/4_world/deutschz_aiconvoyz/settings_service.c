class deutschz_aiconvoyz_settings_service
{
    static const string DIRECTORY = "$profile:DeutschZ-System/aiconvoyz";
    static const string FILE = DIRECTORY + "/settings.json";

    static deutschz_aiconvoyz_settings Load()
    {
        MakeDirectory("$profile:DeutschZ-System");
        MakeDirectory(DIRECTORY);

        deutschz_aiconvoyz_settings settings;
        string error;
        if (FileExist(FILE) && JsonFileLoader<deutschz_aiconvoyz_settings>.LoadFile(FILE, settings, error) && settings && settings.Version == 2 && settings.Global)
            return settings;

        if (FileExist(FILE))
        {
            CopyFile(FILE, FILE + ".pre_v2_" + GetGame().GetTime().ToString() + ".bak");
            Print("[DeutschZ AIConvoyZ] Existing settings are incompatible with v2 and were backed up");
        }

        settings = deutschz_aiconvoyz_defaults.Create();
        if (!JsonFileLoader<deutschz_aiconvoyz_settings>.SaveFile(FILE, settings, error))
            Print("[DeutschZ AIConvoyZ] ERROR: cannot write v2 settings: " + error);
        return settings;
    }
}
