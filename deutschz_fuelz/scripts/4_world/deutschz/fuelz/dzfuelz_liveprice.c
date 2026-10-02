class DZFuelZLivePriceCallback : RestCallback
{
    override void OnSuccess(string data, int dataSize)
    {
        DZFuelZLivePriceService.HandleResponse(data, dataSize);
    }

    override void OnError(int errorCode)
    {
        DZFuelZLivePriceService.HandleFailure("HTTP error " + errorCode);
    }

    override void OnTimeout()
    {
        DZFuelZLivePriceService.HandleFailure("HTTP timeout");
    }
};

class DZFuelZLivePriceService
{
    private static const string CACHE_FILE = "$profile:DeutschZ-System/deutschz_fuelz/PriceCache.json";
    private static const string LEGACY_CACHE_FILE = "$profile:DeutschZ_FuelZ/PriceCache.json";
    private static const int UPDATE_INTERVAL_MS = 86400000;
    private static float s_PricePerLiter;

    static void Start()
    {
        DZFuelZSettings settings = DZFuelZSettingsService.Get();
        s_PricePerLiter = settings.PricePerLiter;
        LoadCache();

        if (!settings.LivePriceEnabled)
        {
            Print(string.Format("[FuelZ] Live price disabled, using %1 EUR/l", s_PricePerLiter));
            return;
        }

        if (settings.TankerkoenigApiKey == "")
        {
            Print(string.Format("[FuelZ] Live price API key missing, using cached/default %1 EUR/l", s_PricePerLiter));
            return;
        }

        RequestPrice();
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(RequestPrice, UPDATE_INTERVAL_MS, true);
    }

    static float GetPricePerLiter()
    {
        if (s_PricePerLiter <= 0)
            return RoundToCents(DZFuelZSettingsService.Get().PricePerLiter);
        return RoundToCents(s_PricePerLiter);
    }

    protected static float RoundToCents(float price)
    {
        return Math.Round(price * 100.0) / 100.0;
    }

    protected static void LoadCache()
    {
        if (!FileExist(CACHE_FILE) && FileExist(LEGACY_CACHE_FILE))
        {
            if (CopyFile(LEGACY_CACHE_FILE, CACHE_FILE)) Print("[FuelZ] Legacy PriceCache.json nach DeutschZ-System migriert; Quelle bleibt als Backup erhalten.");
            else Print("[FuelZ] ERROR Legacy PriceCache.json konnte nicht migriert werden.");
        }
        if (!FileExist(CACHE_FILE))
            return;

        DZFuelZPriceCache cache = new DZFuelZPriceCache;
        JsonFileLoader<DZFuelZPriceCache>.JsonLoadFile(CACHE_FILE, cache);
        if (cache.PricePerLiter >= 0.5 && cache.PricePerLiter <= 5.0)
            s_PricePerLiter = cache.PricePerLiter;
    }

    protected static void SaveCache(float price)
    {
        DZFuelZPriceCache cache = new DZFuelZPriceCache;
        cache.PricePerLiter = price;
        cache.Source = "Tankerkönig / MTS-K";
        JsonFileLoader<DZFuelZPriceCache>.JsonSaveFile(CACHE_FILE, cache);
    }

    static void RequestPrice()
    {
        DZFuelZSettings settings = DZFuelZSettingsService.Get();
        if (!settings.LivePriceEnabled || settings.TankerkoenigApiKey == "")
            return;

        RestApi restApi = CreateRestApi();
        if (!restApi)
        {
            HandleFailure("REST API unavailable");
            return;
        }

        RestContext restContext = restApi.GetRestContext("https://creativecommons.tankerkoenig.de/json/");
        if (!restContext)
        {
            HandleFailure("REST context unavailable");
            return;
        }

        string requestPath = string.Format("list.php?lat=%1&lng=%2&rad=%3&sort=dist&type=%4&apikey=%5", settings.LivePriceLatitude, settings.LivePriceLongitude, settings.LivePriceRadiusKm, settings.LiveFuelType, settings.TankerkoenigApiKey);
        restContext.GET(new DZFuelZLivePriceCallback, requestPath);
    }

    static void HandleResponse(string data, int dataSize)
    {
        if (dataSize <= 0 || data == "")
        {
            HandleFailure("empty response");
            return;
        }

        DZFuelZLivePriceResponse response = new DZFuelZLivePriceResponse;
        JsonFileLoader<DZFuelZLivePriceResponse>.JsonLoadData(data, response);
        if (!response.ok || !response.stations)
        {
            HandleFailure("invalid API response");
            return;
        }

        float total = 0;
        int count = 0;
        foreach (DZFuelZLivePriceStation station : response.stations)
        {
            if (station && station.isOpen && station.price >= 0.5 && station.price <= 5.0)
            {
                total += station.price;
                count++;
            }
        }

        if (count == 0)
        {
            HandleFailure("no valid open-station prices");
            return;
        }

        float price = RoundToCents((total / count) * DZFuelZSettingsService.Get().LivePriceMultiplier);
        if (price < 0.5 || price > 5.0)
        {
            HandleFailure("calculated price outside safety range");
            return;
        }

        s_PricePerLiter = price;
        SaveCache(price);
        Print(string.Format("[FuelZ] Live E10 price updated: %1 EUR/l from %2 open stations", price, count));
    }

    static void HandleFailure(string reason)
    {
        Print(string.Format("[FuelZ] Live price update failed (%1), keeping %2 EUR/l", reason, GetPricePerLiter()));
    }
};
