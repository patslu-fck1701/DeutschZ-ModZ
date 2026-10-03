class deutschz_warningz_rpc
{
    static const string namespace_name = "deutschz_warningz";
    static const string play_sound_method = "play_sound";

    static const string welcomeevents = "welcomeevents";
    static const string restart15min = "restart15min";
    static const string restart2min = "restart2min";
    static const string safezonewarning = "safezonewarning";
    static const string welcome1 = "welcome1";
    static const string welcome2 = "welcome2";
    static const string safezone1 = "safezone1";
    static const string safezone2 = "safezone2";
    static const string death1 = "death1";
    static const string death2 = "death2";
    static const string death3 = "death3";
    static const string logout1 = "logout1";
    static const string logout2 = "logout2";
    static const string logout3 = "logout3";
    static const string safezone_enabled = "safezone_enabled";
    static const string safezone_disabled = "safezone_disabled";

    static string random_welcome()
    {
        int choice = Math.RandomInt(0, 3);
        if (choice == 1) return welcome1;
        if (choice == 2) return welcome2;
        return welcomeevents;
    }

    static string random_safezone()
    {
        int choice = Math.RandomInt(0, 3);
        if (choice == 1) return safezone1;
        if (choice == 2) return safezone2;
        return safezonewarning;
    }

    static string random_death()
    {
        int choice = Math.RandomInt(0, 3);
        if (choice == 1) return death2;
        if (choice == 2) return death3;
        return death1;
    }

    static string random_logout()
    {
        int choice = Math.RandomInt(0, 3);
        if (choice == 1) return logout2;
        if (choice == 2) return logout3;
        return logout1;
    }
};
