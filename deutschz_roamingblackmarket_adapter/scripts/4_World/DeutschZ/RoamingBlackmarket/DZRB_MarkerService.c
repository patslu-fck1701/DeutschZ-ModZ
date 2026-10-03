class DZRB_MarkerService
{
	static const string MARKER_UID = "DEUTSCHZ_ROAMING_BLACKMARKET";
	static const string MARKER_NAME = "DeutschZ Schwarzmarkt";
	static const string MARKER_ICON = "Deliver";

	static bool CreateAtPosition(vector position, bool traderActive)
	{
		ExpansionMarkerModule markerModule;
		if (!CF_Modules<ExpansionMarkerModule>.Get(markerModule) || !markerModule) return false;
		markerModule.RemoveServerMarker(MARKER_UID);
		return markerModule.CreateServerMarker(MARKER_NAME, MARKER_ICON, position, ARGB(255, 200, 20, 20), traderActive, MARKER_UID) != NULL;
	}

	static bool UpdateVehicle(CarScript vehicle)
	{
		if (!vehicle) return false;
		return CreateAtPosition(vehicle.GetPosition(), false);
	}

	static void RemoveCurrent(bool announce)
	{
		ExpansionMarkerModule markerModule;
		if (CF_Modules<ExpansionMarkerModule>.Get(markerModule) && markerModule)
			markerModule.RemoveServerMarker(MARKER_UID);
	}

	static void BroadcastNotification(string title, string message)
	{
		ref array<Man> players = new array<Man>();
		GetGame().GetPlayers(players);
		foreach (Man player : players)
		{
			PlayerBase playerBase = PlayerBase.Cast(player);
			if (playerBase && playerBase.GetIdentity())
				ExpansionNotification(title, message).Info(playerBase.GetIdentity());
		}
	}
}

class DZRB_ExpansionTraderService
{
	private static const string MOBILE_TRADER_FILE = "DeutschZ_MobileBlackmarket";
	private static ExpansionTraderNPCBase s_Trader;
	private static ref ExpansionMarketTraderZone s_Zone;

	static bool OpenAt(vector position, vector orientation, DZRB_MobileSettings settings, string stopID)
	{
		if (s_Trader) return true;
		// Expansion compares the NPC against the full 3D zone position.  Route
		// points are stored at Y=0, so ground the shared position before the zone
		// is registered or elevated stops can never bind to their market zone.
		position[1] = GetGame().SurfaceY(position[0], position[2]);
		ExpansionMarketSettings market = GetExpansionSettings().GetMarket();
		if (!market || !PrepareTrader(market, settings) || !PrepareZone(market, settings, position))
		{
			Print("[DeutschZ_RBM] EXPANSION_MARKET_PREPARE_FAILED stop=" + stopID);
			return false;
		}

		s_Trader = ExpansionTraderNPCBase.Cast(GetGame().CreateObjectEx(settings.TraderClassName, position, ECE_PLACE_ON_SURFACE | ECE_NOLIFETIME));
		if (!s_Trader)
		{
			Print("[DeutschZ_RBM] EXPANSION_TRADER_SPAWN_FAILED class=" + settings.TraderClassName);
			return false;
		}
		s_Trader.SetPosition(position);
		s_Trader.SetOrientation(orientation);
		s_Trader.SetAllowDamage(false);
		foreach (string clothing : settings.TraderOutfit)
		{
			if (clothing != "" && !s_Trader.GetInventory().CreateAttachment(clothing))
				Print("[DeutschZ_RBM] TRADER_OUTFIT_ATTACHMENT_FAILED class=" + clothing);
		}
		s_Trader.LoadTrader(MOBILE_TRADER_FILE);
		if (s_Trader.GetTraderObject() && s_Trader.GetTraderObject().GetTraderMarket() && s_Trader.GetTraderObject().GetTraderZone()) return true;
		Print("[DeutschZ_RBM] TRADER_BIND_FAILED stop=" + stopID);
		Close();
		return false;
	}

	private static bool PrepareTrader(ExpansionMarketSettings market, DZRB_MobileSettings settings)
	{
		ExpansionMarketTrader cached = market.GetMarketTrader(MOBILE_TRADER_FILE);
		if (cached && cached.m_Items && cached.m_Items.Count() > 0)
		{
			cached.MinRequiredReputation = settings.MinRequiredReputation;
			cached.MaxRequiredReputation = settings.MaxRequiredReputation;
			cached.RequiredFaction = "";
			cached.RequiredCompletedQuestID = -1;
			cached.DisplayCurrencyValue = 1;
			cached.DisplayCurrencyName = "USD";
			cached.Save();
			Print(string.Format("[DeutschZ_RBM] TRADER_REPAIRED items=%1 quest=-1", cached.m_Items.Count()));
			return true;
		}
		ExpansionMarketTrader combined = new ExpansionMarketTrader();
		combined.m_FileName = MOBILE_TRADER_FILE;
		combined.DisplayName = "DeutschZ Mobile Blackmarket";
		combined.TraderIcon = "Deliver";
		combined.MinRequiredReputation = settings.MinRequiredReputation;
		combined.MaxRequiredReputation = settings.MaxRequiredReputation;
		combined.RequiredFaction = "";
		combined.RequiredCompletedQuestID = -1;
		combined.DisplayCurrencyValue = 1;
		combined.DisplayCurrencyName = "USD";
		combined.UseCategoryOrder = true;
		int loaded = 0;
		foreach (string fileName : settings.TraderFiles)
		{
			ExpansionMarketTrader source = market.GetMarketTrader(fileName);
			if (!source) continue;
			loaded++;
			foreach (string category : source.Categories)
				if (combined.Categories.Find(category) == -1) combined.Categories.Insert(category);
			foreach (string currency : source.Currencies)
				if (combined.Currencies.Find(currency) == -1) combined.Currencies.Insert(currency);
			foreach (string className, ExpansionMarketTraderBuySell rule : source.Items)
				if (!combined.Items.Contains(className)) combined.AddItem(className, rule);
		}
		if (loaded == 0) return false;
		combined.Finalize();
		if (!combined.m_Items || combined.m_Items.Count() == 0)
		{
			Print("[DeutschZ_RBM] TRADER_FINALIZE_EMPTY categories=" + combined.Categories.Count());
			return false;
		}
		combined.Save();
		market.AddMarketTrader(combined);
		Print(string.Format("[DeutschZ_RBM] TRADER_READY sources=%1 categories=%2 items=%3 currencies=%4", loaded, combined.Categories.Count(), combined.m_Items.Count(), combined.Currencies.Count()));
		return true;
	}

	private static bool PrepareZone(ExpansionMarketSettings market, DZRB_MobileSettings settings, vector position)
	{
		ExpansionMarketTraderZone source = ExpansionMarketTraderZone.Load(settings.TraderZoneFile);
		s_Zone = new ExpansionMarketTraderZone();
		s_Zone.m_FileName = string.Format("DeutschZ_MobileBlackmarket_%1_%2", Math.Round(position[0]), Math.Round(position[2]));
		s_Zone.m_DisplayName = "DeutschZ Mobile Blackmarket Zone";
		s_Zone.Position = position;
		s_Zone.Radius = Math.Max(settings.TraderZoneRadius, 10.0);
		if (source)
		{
			s_Zone.BuyPricePercent = source.BuyPricePercent;
			s_Zone.SellPricePercent = source.SellPricePercent;
			foreach (string className, int stock : source.Stock) s_Zone.Stock.Set(className, stock);
		}
		else
		{
			s_Zone.BuyPricePercent = 100.0;
			s_Zone.SellPricePercent = 100.0;
			Print("[DeutschZ_RBM] TRADER_ZONE_DEFAULT source_missing=" + settings.TraderZoneFile);
		}
		market.AddMarketZone(s_Zone);
		return true;
	}

	static void Close()
	{
		if (s_Trader) GetGame().ObjectDelete(s_Trader);
		s_Trader = NULL;
		DZRB_MarkerService.RemoveCurrent(false);
	}

	static vector Position()
	{
		if (s_Trader) return s_Trader.GetPosition();
		return vector.Zero;
	}

	static bool IsActiveTrader(Object object)
	{
		return object && s_Trader && object == s_Trader;
	}
}
