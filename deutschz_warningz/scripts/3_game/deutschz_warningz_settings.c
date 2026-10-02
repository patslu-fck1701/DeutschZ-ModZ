class deutschz_warningz_settings
{
    int config_version = 1;
    int restart_interval_minutes = 240;
    int restart_offset_minutes = 0;
    int welcome_delay_seconds = 8;
    bool enable_welcome = true;
    bool enable_restart15min = true;
    bool enable_restart2min = true;
    bool enable_safezonewarning = true;

    static string get_directory()
    {
        return "$profile:DeutschZ-System\\warningz";
    }

    static string get_path()
    {
        return get_directory() + "\\settings.json";
    }

    static void ensure_directory()
    {
        if (!FileExist("$profile:DeutschZ-System"))
            MakeDirectory("$profile:DeutschZ-System");

        if (!FileExist(get_directory()))
            MakeDirectory(get_directory());
    }

    static bool has_required_keys(string path)
    {
        FileHandle handle = OpenFile(path, FileMode.READ);
        if (handle == 0)
            return false;

        string content;
        string line;

        while (FGets(handle, line) >= 0)
            content += line;

        CloseFile(handle);

        bool valid = true;
        valid = valid && content.IndexOf("\"config_version\"") >= 0;
        valid = valid && content.IndexOf("\"restart_interval_minutes\"") >= 0;
        valid = valid && content.IndexOf("\"restart_offset_minutes\"") >= 0;
        valid = valid && content.IndexOf("\"welcome_delay_seconds\"") >= 0;
        valid = valid && content.IndexOf("\"enable_welcome\"") >= 0;
        valid = valid && content.IndexOf("\"enable_restart15min\"") >= 0;
        valid = valid && content.IndexOf("\"enable_restart2min\"") >= 0;
        valid = valid && content.IndexOf("\"enable_safezonewarning\"") >= 0;
        return valid;
    }

    static deutschz_warningz_settings load()
    {
        ensure_directory();

        deutschz_warningz_settings settings = new deutschz_warningz_settings;
        string error_message;
        bool replace_file;

        if (!FileExist(get_path()))
        {
            replace_file = true;
        }
        else if (!has_required_keys(get_path()))
        {
            replace_file = true;
        }
        else if (!JsonFileLoader<deutschz_warningz_settings>.LoadFile(get_path(), settings, error_message))
        {
            replace_file = true;
            Print("[deutschz_warningz] defekte settings werden ersetzt: " + error_message);
        }
        else if (!settings.is_valid())
        {
            replace_file = true;
            Print("[deutschz_warningz] ungueltige settings werden durch standardwerte ersetzt.");
        }

        if (replace_file)
        {
            settings = new deutschz_warningz_settings;

            if (!JsonFileLoader<deutschz_warningz_settings>.SaveFile(get_path(), settings, error_message))
                ErrorEx("[deutschz_warningz] settings konnten nicht gespeichert werden: " + error_message);
            else
                Print("[deutschz_warningz] standard-settings erstellt: " + get_path());
        }

        return settings;
    }

    bool is_valid()
    {
        if (config_version != 1)
            return false;

        if (restart_interval_minutes < 20)
            return false;

        if (restart_offset_minutes < 0 || restart_offset_minutes >= restart_interval_minutes)
            return false;

        if (welcome_delay_seconds < 0 || welcome_delay_seconds > 600)
            return false;

        return true;
    }
};
