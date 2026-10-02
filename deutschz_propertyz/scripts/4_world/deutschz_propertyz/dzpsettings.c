class DZPHouseTypeSetting
{
    string Type;
    int BuyPrice;
    int SellPrice;
    int StorageSlots;
    int WeaponSlots;
    int BarrelSlots;
    int SeaChestSlots;
    int WoodenCrateSlots;
    bool HasControllerReference;
    vector ControllerOffset;
    float ControllerYawOffset;
    bool HasSecondControllerReference;
    vector SecondControllerOffset;
    float SecondControllerYawOffset;
    vector BreachingChargeOffset;
    float BreachingChargeYawOffset;
    vector BreachingChargeHeavyOffset;
    float BreachingChargeHeavyYawOffset;
    vector SecondBreachingChargeOffset;
    float SecondBreachingChargeYawOffset;
    vector SecondBreachingChargeHeavyOffset;
    float SecondBreachingChargeHeavyYawOffset;
    vector RespawnOffset;

    void DZPHouseTypeSetting(string type = "", int buyPrice = 100000, int sellPrice = 75000, int storageSlots = 0, int weaponSlots = 0, int barrelSlots = 0, int seaChestSlots = 0, int woodenCrateSlots = 0)
    {
        Type = type;
        BuyPrice = buyPrice;
        SellPrice = sellPrice;
        StorageSlots = storageSlots;
        WeaponSlots = weaponSlots;
        BarrelSlots = barrelSlots;
        SeaChestSlots = seaChestSlots;
        WoodenCrateSlots = woodenCrateSlots;
    }
}

class DZPSettings
{
    int ConfigVersion = 10;
    bool Enabled = true;
    float InteractionDistance = 4.0;
    int MaxPropertiesPerPlayer = 4;
    bool ShowOwnedHouse3DMarkers = true;
    float OwnedHouseMarkerMaxDistance = 5000.0;
    bool UseExpansionMarketCurrency = true;
    bool LockAllDoorsAfterPurchase = true;
    int InsideExitRelockSeconds = 10;
    bool EnableExpansionHouseRespawn = true;
    bool HouseRespawnUseCooldown = true;
    ref array<ref DZPHouseTypeSetting> Houses = new array<ref DZPHouseTypeSetting>;

    void SetDefaults()
    {
        Houses.Clear();
        AddDefaultHouse("Land_House_2W02");
        AddDefaultHouse("Land_House_1W05_Yellow");
        AddDefaultHouse("Land_House_1W06");
        AddDefaultHouse("Land_Camp_House_red");
        AddDefaultHouse("Land_Village_Pub");
    }

    void AddDefaultHouse(string type)
    {
        DZPHouseTypeSetting house;
        if (type == "Land_House_2W02")
        {
            house = new DZPHouseTypeSetting(type, 1500000, 1125000, 2000, 0, 0, 0, 0);
            house.HasControllerReference = true;
            house.ControllerOffset = "-1.811028 -2.952560 0.003163";
            house.ControllerYawOffset = -175.508340;
            house.BreachingChargeOffset = "-1.546594 -2.611602 0.960505";
            house.BreachingChargeYawOffset = -94.287621;
            house.BreachingChargeHeavyOffset = "-1.572547 -3.006210 0.960417";
            house.BreachingChargeHeavyYawOffset = -94.508355;
            house.RespawnOffset = "0.238000 -4.021000 0.812000";
        }
        else if (type == "Land_House_1W05_Yellow")
        {
            house = new DZPHouseTypeSetting(type, 2000000, 1500000, 1200, 12, 0, 0, 0);
            house.HasControllerReference = true;
            house.ControllerOffset = "1.590475 -0.019150 4.386347";
            house.ControllerYawOffset = -178.149433;
            house.BreachingChargeOffset = "1.781076 0.177703 5.367896";
            house.BreachingChargeYawOffset = -88.149444;
            house.BreachingChargeHeavyOffset = "1.782461 -0.177399 4.958636";
            house.BreachingChargeHeavyYawOffset = -79.149437;
            house.RespawnOffset = "1.080000 0.100000 2.975000";
        }
        else if (type == "Land_House_1W06")
        {
            house = new DZPHouseTypeSetting(type, 2250000, 1687500, 1000, 0, 6, 0, 0);
            house.HasControllerReference = true;
            house.ControllerOffset = "1.776811 0.284912 -4.891746";
            house.ControllerYawOffset = -5.034684;
            house.BreachingChargeOffset = "2.531944 0.620239 -4.670717";
            house.BreachingChargeYawOffset = 179.008632;
            house.BreachingChargeHeavyOffset = "2.413539 0.215881 -4.671242";
            house.BreachingChargeHeavyYawOffset = 179.008632;
            house.HasSecondControllerReference = true;
            house.SecondControllerOffset = "-0.622014 -1.589829 4.035198";
            house.SecondControllerYawOffset = -90.991368;
            house.SecondBreachingChargeOffset = "-1.439462 -1.317688 3.843507";
            house.SecondBreachingChargeYawOffset = 8.008621;
            house.SecondBreachingChargeHeavyOffset = "-1.256735 -1.678268 3.843095";
            house.SecondBreachingChargeHeavyYawOffset = -0.991364;
            house.RespawnOffset = "1.264000 0.300000 -3.478000";
        }
        else if (type == "Land_Camp_House_red")
        {
            house = new DZPHouseTypeSetting(type, 2500000, 1875000, 900, 0, 0, 6, 8);
            house.HasControllerReference = true;
            house.ControllerOffset = "0.850010 -0.266816 -1.106810";
            house.ControllerYawOffset = 92.759207;
            house.BreachingChargeOffset = "0.154405 -0.102417 -0.922543";
            house.BreachingChargeYawOffset = -179.999971;
            house.BreachingChargeHeavyOffset = "0.200482 -0.582001 -0.922630";
            house.BreachingChargeHeavyYawOffset = -179.999971;
        }
        else if (type == "Land_Village_Pub")
        {
            house = new DZPHouseTypeSetting(type, 2500000, 1875000, 900, 0, 0, 6, 8);
            house.HasControllerReference = true;
            house.ControllerOffset = "-4.303724 -2.041229 1.788989";
            house.ControllerYawOffset = -91.139099;
            house.BreachingChargeOffset = "-3.412277 -1.818695 1.609703";
            house.BreachingChargeYawOffset = 0.000016;
            house.BreachingChargeHeavyOffset = "-3.422325 -2.160400 1.610064";
            house.BreachingChargeHeavyYawOffset = 0.000016;
            house.HasSecondControllerReference = true;
            house.SecondControllerOffset = "-2.433074 -1.808716 -5.354181";
            house.SecondControllerYawOffset = 0.0;
            house.SecondBreachingChargeOffset = "-2.022701 -1.698029 -5.303460";
            house.SecondBreachingChargeYawOffset = 179.999987;
            house.SecondBreachingChargeHeavyOffset = "-2.081879 -1.994415 -5.303472";
            house.SecondBreachingChargeHeavyYawOffset = 179.999987;
        }
        if (house) Houses.Insert(house);
    }
}

class DZPSettingsService
{
    private static ref DZPSettings s_Settings;
    private static const string DIR = "$profile:DeutschZ-System\\PropertyZ";
    private static const string FILE = "$profile:DeutschZ-System\\PropertyZ\\PropertyZSettings.json";

    static DZPSettings Get()
    {
        if (!s_Settings)
            Load();
        return s_Settings;
    }

    static void Load()
    {
        if (!FileExist("$profile:DeutschZ-System")) MakeDirectory("$profile:DeutschZ-System");
        if (!FileExist(DIR)) MakeDirectory(DIR);

        s_Settings = new DZPSettings();
        if (FileExist(FILE))
            JsonFileLoader<DZPSettings>.JsonLoadFile(FILE, s_Settings);

        if (!s_Settings.Houses || s_Settings.Houses.Count() == 0)
        {
            s_Settings.SetDefaults();
            JsonFileLoader<DZPSettings>.JsonSaveFile(FILE, s_Settings);
        }
        else if (s_Settings.ConfigVersion < 10)
        {
            if (s_Settings.ConfigVersion < 2)
                MigrateToVersion2(s_Settings);
            if (s_Settings.ConfigVersion < 3)
                MigrateToVersion3(s_Settings);
            if (s_Settings.ConfigVersion < 5)
                MigrateToVersion5(s_Settings);
            if (s_Settings.ConfigVersion < 6)
                MigrateToVersion6(s_Settings);
            if (s_Settings.ConfigVersion < 7)
                MigrateToVersion7(s_Settings);
            if (s_Settings.ConfigVersion < 8)
                MigrateToVersion8(s_Settings);
            if (s_Settings.ConfigVersion < 9)
                MigrateToVersion9(s_Settings);
            MigrateToVersion10(s_Settings);
            JsonFileLoader<DZPSettings>.JsonSaveFile(FILE, s_Settings);
        }
    }

    static DZPHouseTypeSetting Find(string typeName)
    {
        DZPSettings settings = Get();
        if (!settings || !settings.Enabled) return null;
        foreach (DZPHouseTypeSetting house : settings.Houses)
            if (house && house.Type == typeName) return house;
        return null;
    }

    private static void MigrateToVersion8(DZPSettings settings)
    {
        bool hasCampHouse;
        bool hasVillagePub;
        foreach (DZPHouseTypeSetting house : settings.Houses)
        {
            if (!house) continue;
            if (house.Type == "Land_Camp_House_red") hasCampHouse = true;
            if (house.Type == "Land_Village_Pub") hasVillagePub = true;
        }
        if (!hasCampHouse) settings.AddDefaultHouse("Land_Camp_House_red");
        if (!hasVillagePub) settings.AddDefaultHouse("Land_Village_Pub");
        settings.ConfigVersion = 8;
    }

    private static void MigrateToVersion9(DZPSettings settings)
    {
        foreach (DZPHouseTypeSetting house : settings.Houses)
        {
            if (!house) continue;
            if (house.Type != "Land_Camp_House_red" && house.Type != "Land_Village_Pub") continue;

            DZPSettings defaults = new DZPSettings();
            defaults.AddDefaultHouse(house.Type);
            if (defaults.Houses.Count() == 0) continue;
            DZPHouseTypeSetting defaultHouse = defaults.Houses[0];
            house.HasControllerReference = defaultHouse.HasControllerReference;
            house.ControllerOffset = defaultHouse.ControllerOffset;
            house.ControllerYawOffset = defaultHouse.ControllerYawOffset;
            house.HasSecondControllerReference = defaultHouse.HasSecondControllerReference;
            house.SecondControllerOffset = defaultHouse.SecondControllerOffset;
            house.SecondControllerYawOffset = defaultHouse.SecondControllerYawOffset;
            house.BreachingChargeOffset = defaultHouse.BreachingChargeOffset;
            house.BreachingChargeYawOffset = defaultHouse.BreachingChargeYawOffset;
            house.BreachingChargeHeavyOffset = defaultHouse.BreachingChargeHeavyOffset;
            house.BreachingChargeHeavyYawOffset = defaultHouse.BreachingChargeHeavyYawOffset;
            house.SecondBreachingChargeOffset = defaultHouse.SecondBreachingChargeOffset;
            house.SecondBreachingChargeYawOffset = defaultHouse.SecondBreachingChargeYawOffset;
            house.SecondBreachingChargeHeavyOffset = defaultHouse.SecondBreachingChargeHeavyOffset;
            house.SecondBreachingChargeHeavyYawOffset = defaultHouse.SecondBreachingChargeHeavyYawOffset;
        }
        settings.ConfigVersion = 9;
    }

    private static void MigrateToVersion10(DZPSettings settings)
    {
        foreach (DZPHouseTypeSetting house : settings.Houses)
        {
            if (!house) continue;
            DZPSettings defaults = new DZPSettings();
            defaults.AddDefaultHouse(house.Type);
            if (defaults.Houses.Count() == 0) continue;
            DZPHouseTypeSetting defaultHouse = defaults.Houses[0];
            house.BuyPrice = defaultHouse.BuyPrice;
            house.SellPrice = defaultHouse.SellPrice;
            house.StorageSlots = defaultHouse.StorageSlots;
            house.WeaponSlots = defaultHouse.WeaponSlots;
            house.BarrelSlots = defaultHouse.BarrelSlots;
            house.SeaChestSlots = defaultHouse.SeaChestSlots;
            house.WoodenCrateSlots = defaultHouse.WoodenCrateSlots;
        }
        settings.ConfigVersion = 10;
    }

    private static void MigrateToVersion2(DZPSettings settings)
    {
        foreach (DZPHouseTypeSetting house : settings.Houses)
        {
            if (!house) continue;
            if (house.Type == "Land_House_1W02")
            {
                house.BuyPrice = 400000; house.SellPrice = 300000;
                house.StorageSlots = 2000;
            }
            else if (house.Type == "Land_House_1W06")
            {
                house.BuyPrice = 550000; house.SellPrice = 410000;
                house.StorageSlots = 1000; house.BarrelSlots = 4; house.SeaChestSlots = 3;
            }
            else if (house.Type == "Land_House_2W02")
            {
                house.BuyPrice = 650000; house.SellPrice = 485000;
                house.StorageSlots = 1000; house.WeaponSlots = 5;
            }
            else if (house.Type == "Land_House_1W05_Yellow")
            {
                house.BuyPrice = 850000; house.SellPrice = 635000;
                house.StorageSlots = 1500; house.WeaponSlots = 8; house.BarrelSlots = 4; house.SeaChestSlots = 4;
            }
        }
        settings.ConfigVersion = 2;
    }

    private static void MigrateToVersion3(DZPSettings settings)
    {
        if (settings.MaxPropertiesPerPlayer < 2)
            settings.MaxPropertiesPerPlayer = 4;
        settings.ShowOwnedHouse3DMarkers = true;
        if (settings.OwnedHouseMarkerMaxDistance <= 0)
            settings.OwnedHouseMarkerMaxDistance = 5000.0;
        settings.ConfigVersion = 3;
    }

    private static void MigrateToVersion5(DZPSettings settings)
    {
        // Einmalige, absichtlich enge Korrektur: nur die drei freigegebenen Referenzhäuser.
        settings.SetDefaults();
        settings.ConfigVersion = 5;
    }

    private static void MigrateToVersion6(DZPSettings settings)
    {
        if (settings.InsideExitRelockSeconds <= 0)
            settings.InsideExitRelockSeconds = 10;
        settings.ConfigVersion = 6;
    }

    private static void MigrateToVersion7(DZPSettings settings)
    {
        foreach (DZPHouseTypeSetting house : settings.Houses)
        {
            DZPHouseTypeSetting defaultHouse = new DZPHouseTypeSetting();
            DZPSettings defaults = new DZPSettings();
            if (!house) continue;
            defaults.AddDefaultHouse(house.Type);
            if (defaults.Houses.Count() == 0) continue;
            defaultHouse = defaults.Houses[0];
            house.HasControllerReference = defaultHouse.HasControllerReference;
            house.ControllerOffset = defaultHouse.ControllerOffset;
            house.ControllerYawOffset = defaultHouse.ControllerYawOffset;
            house.HasSecondControllerReference = defaultHouse.HasSecondControllerReference;
            house.SecondControllerOffset = defaultHouse.SecondControllerOffset;
            house.SecondControllerYawOffset = defaultHouse.SecondControllerYawOffset;
            house.BreachingChargeOffset = defaultHouse.BreachingChargeOffset;
            house.BreachingChargeYawOffset = defaultHouse.BreachingChargeYawOffset;
            house.BreachingChargeHeavyOffset = defaultHouse.BreachingChargeHeavyOffset;
            house.BreachingChargeHeavyYawOffset = defaultHouse.BreachingChargeHeavyYawOffset;
            house.SecondBreachingChargeOffset = defaultHouse.SecondBreachingChargeOffset;
            house.SecondBreachingChargeYawOffset = defaultHouse.SecondBreachingChargeYawOffset;
            house.SecondBreachingChargeHeavyOffset = defaultHouse.SecondBreachingChargeHeavyOffset;
            house.SecondBreachingChargeHeavyYawOffset = defaultHouse.SecondBreachingChargeHeavyYawOffset;
            house.RespawnOffset = defaultHouse.RespawnOffset;
        }
        settings.EnableExpansionHouseRespawn = true;
        settings.HouseRespawnUseCooldown = true;
        settings.ConfigVersion = 7;
    }
}
