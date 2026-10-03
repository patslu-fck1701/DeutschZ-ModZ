class DZATM_ProfilePaths
{
    static const string ROOT = "$profile:DeutschZ-System/ATM_RaideZ";
    static const string CONFIG = "$profile:DeutschZ-System/ATM_RaideZ/Config";
    static const string PERSISTENCE = "$profile:DeutschZ-System/ATM_RaideZ/Persistence";
    static const string RUNTIME = "$profile:DeutschZ-System/ATM_RaideZ/Runtime";
    static const string LOG_ROOT = "$profile:DeutschZ-System/LogZ/atmraidez";

    static const string MAIN_CONFIG = "$profile:DeutschZ-System/ATM_RaideZ/Config/ATM_RaideZ.json";
    static const string COOLDOWNS = "$profile:DeutschZ-System/ATM_RaideZ/Persistence/cooldowns.json";
    static const string LOG_FILE = "$profile:DeutschZ-System/LogZ/atmraidez/atmraidez.log";

    static void Ensure()
    {
        EnsureDirectory("$profile:DeutschZ-System");
        EnsureDirectory(ROOT);
        EnsureDirectory(CONFIG);
        EnsureDirectory(PERSISTENCE);
        EnsureDirectory(RUNTIME);
        EnsureDirectory("$profile:DeutschZ-System/LogZ");
        EnsureDirectory(LOG_ROOT);
    }

    protected static void EnsureDirectory(string path)
    {
        if (!FileExist(path))
            MakeDirectory(path);
    }
}
