class DZFuelZStation
{
    string BuildingPosition;
    string BuildingOrientation;
    string Position;
    string Orientation;
    bool PlaceBuilding;

    void DZFuelZStation(string buildingPosition = "", string position = "", string orientation = "", bool placeBuilding = false, string buildingOrientation = "")
    {
        BuildingPosition = buildingPosition;
        BuildingOrientation = buildingOrientation;
        Position = position;
        Orientation = orientation;
        PlaceBuilding = placeBuilding;
    }
};

class DZFuelZSettings
{
    int Version = 5;
    float PricePerLiter = 5.0;
    bool LivePriceEnabled = true;
    string TankerkoenigApiKey = "";
    float LivePriceLatitude = 52.520008;
    float LivePriceLongitude = 13.404954;
    float LivePriceRadiusKm = 25.0;
    string LiveFuelType = "e10";
    float LivePriceMultiplier = 1.0;
    float InteractionDistance = 3.0;
    float VehicleRange = 20.0;
    string FuelShopClassname = "Land_FuelStation_Build";
    string NPCClassname = "DZ_FuelZ_NPC";
    string NPCLocalOffset = "-1.458551 -1.537858 -1.251150";
    float NPCRelativeYaw = 182.122833;
    ref array<string> CurrencyClassnames;
    ref array<int> CurrencyValues;
    ref array<ref DZFuelZStation> Stations;

    void DZFuelZSettings()
    {
        SetDefaultCurrencies();
        SetDefaultStations();
    }

    void SetDefaultCurrencies()
    {
        CurrencyClassnames = {"ExpansionBanknoteEuro"};
        CurrencyValues = {100};
    }

    void SetDefaultStations()
    {
        Stations = new array<ref DZFuelZStation>;
        Stations.Insert(new DZFuelZStation("300.600555 296.574921 9399.578125"));
        Stations.Insert(new DZFuelZStation("1121.985229 7.177962 2417.455566", "1120.208130 5.603887 2417.623047", "144 0 0"));
        Stations.Insert(new DZFuelZStation("2701.584473 212.615799 5599.729004", "2702.624756 211.061478 5601.134277", "-98.999985 0 0"));
        Stations.Insert(new DZFuelZStation("3000.457520 344.119537 7482.815430", "3000.142822 342.572113 7484.609863", "-144 0 0"));
        Stations.Insert(new DZFuelZStation("3031.052734 211.179871 12656.985352", "3032.011963 209.627945 12658.302734", "-107.999992 0 0"));
        Stations.Insert(new DZFuelZStation("3668.298828 313.144897 9002.197266", "3667.948486 311.554626 9003.751953", "-161.999985 0 0"));
        Stations.Insert(new DZFuelZStation("4349.941895 178.404449 13085.363281", "4349.487305 176.862640 13086.927734", "-143.999985 0 0"));
        Stations.Insert(new DZFuelZStation("4394.242188 340.575195 10820.508789", "4394.512207 339.004730 10822.007813", "-125.999992 0 0", true, "63.110371 0 0"));
        Stations.Insert(new DZFuelZStation("4726.754395 283.438812 6389.692871"));
        Stations.Insert(new DZFuelZStation("5862.750000 10.833452 2212.286377", "5860.993652 9.258333 2213.143311", "152.999985 0 0"));
        Stations.Insert(new DZFuelZStation("5857.977051 279.267883 10116.846680"));
        Stations.Insert(new DZFuelZStation("6874.834473 8.880091 3094.625000", "6872.729004 7.306866 3094.652100", "125.999992 0 0"));
        Stations.Insert(new DZFuelZStation("7470.551270 122.652611 12659.392578", "7468.937012 121.088470 12659.026367", "125.999992 0 0"));
        Stations.Insert(new DZFuelZStation("9518.679688 7.212095 2003.847534", "9519.614258 5.645594 2005.373291", "-90 0 0"));
        Stations.Insert(new DZFuelZStation("10119.334961 231.165192 5199.791016"));
        Stations.Insert(new DZFuelZStation("10442.137695 224.715897 8884.391602"));
        Stations.Insert(new DZFuelZStation("10750.474609 145.989441 10782.383789"));
        Stations.Insert(new DZFuelZStation("12978.325195 7.710056 10079.391602", "12977.201172 6.165229 10078.000977", "78.898048 0 0"));
        Stations.Insert(new DZFuelZStation("13369.897461 6.213018 6611.593262", "13369.409180 4.618965 6610.030762", "63 0 0"));
        Stations.Insert(new DZFuelZStation("13574.409180 35.871765 13304.538086", "13576.019531 34.299641 13305.074219", "-53.999996 0 0"));
    }

    void Validate()
    {
        if (PricePerLiter <= 0) PricePerLiter = 5.0;
        if (LivePriceRadiusKm < 1.0 || LivePriceRadiusKm > 25.0) LivePriceRadiusKm = 25.0;
        if (LiveFuelType != "e5" && LiveFuelType != "e10" && LiveFuelType != "diesel") LiveFuelType = "e10";
        if (LivePriceMultiplier <= 0) LivePriceMultiplier = 1.0;
        if (InteractionDistance < 1.0) InteractionDistance = 3.0;
        if (Version < 4 && VehicleRange == 12.0) VehicleRange = 20.0;
        if (VehicleRange < 2.0) VehicleRange = 20.0;
        if (FuelShopClassname == "") FuelShopClassname = "Land_FuelStation_Build";
        if (NPCClassname == "") NPCClassname = "DZ_FuelZ_NPC";
        if (NPCLocalOffset == "") NPCLocalOffset = "-1.458551 -1.537858 -1.251150";
        if (Version < 5)
            SetDefaultCurrencies();
        if (!CurrencyClassnames || !CurrencyValues || CurrencyClassnames.Count() != CurrencyValues.Count())
            SetDefaultCurrencies();
        if (Version < 3 || !Stations || Stations.Count() == 0)
            SetDefaultStations();
        Version = 5;
    }

    int GetCurrencyValue(string className)
    {
        for (int i = 0; i < CurrencyClassnames.Count(); i++)
        {
            if (CurrencyClassnames[i] == className)
                return CurrencyValues[i];
        }
        return 0;
    }
};

class DZFuelZLivePriceStation
{
    float price;
    bool isOpen;
};

class DZFuelZLivePriceResponse
{
    bool ok;
    string message;
    ref array<ref DZFuelZLivePriceStation> stations;
};

class DZFuelZPriceCache
{
    float PricePerLiter;
    string Source;
};

class DZFuelZSettingsService
{
    private static ref DZFuelZSettings s_Settings;
    private static const string ROOT = "$profile:DeutschZ-System";
    private static const string DIRECTORY = "$profile:DeutschZ-System/deutschz_fuelz";
    private static const string FILE = "$profile:DeutschZ-System/deutschz_fuelz/Settings.json";
    private static const string LEGACY_FILE = "$profile:DeutschZ_FuelZ/Settings.json";

    static DZFuelZSettings Get()
    {
        if (!s_Settings)
            Load();
        return s_Settings;
    }

    static void Load()
    {
        s_Settings = new DZFuelZSettings;
        if (!FileExist(ROOT)) MakeDirectory(ROOT);
        if (!FileExist(DIRECTORY)) MakeDirectory(DIRECTORY);
        if (!FileExist(FILE) && FileExist(LEGACY_FILE))
        {
            if (CopyFile(LEGACY_FILE, FILE)) Print("[FuelZ] Legacy Settings.json nach DeutschZ-System migriert; Quelle bleibt als Backup erhalten.");
            else Print("[FuelZ] ERROR Legacy Settings.json konnte nicht migriert werden.");
        }

        if (FileExist(FILE))
            JsonFileLoader<DZFuelZSettings>.JsonLoadFile(FILE, s_Settings);

        s_Settings.Validate();
        JsonFileLoader<DZFuelZSettings>.JsonSaveFile(FILE, s_Settings);
    }
};
