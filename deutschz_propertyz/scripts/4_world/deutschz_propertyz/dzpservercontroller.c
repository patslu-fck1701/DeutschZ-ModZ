class DZPServerController
{
    private static ref DZPServerController s_Instance;

    static DZPServerController Get()
    {
        if (!s_Instance) s_Instance = new DZPServerController();
        return s_Instance;
    }

    void DZPServerController()
    {
        DZPSettingsService.Get();
        DZPPropertyStore.Get();
        DZPPropertyControllerService.Get();
        g_Game.Event_OnRPC.Insert(OnRPC);
        Print("[DeutschZ PropertyZ] Servercontroller initialisiert");
    }

    void ~DZPServerController()
    {
        if (g_Game && g_Game.Event_OnRPC) g_Game.Event_OnRPC.Remove(OnRPC);
    }

    private PlayerBase FindPlayer(PlayerIdentity identity)
    {
        if (!identity) return null;
        array<Man> players = new array<Man>;
        g_Game.GetPlayers(players);
        foreach (Man man : players)
        {
            PlayerBase player = PlayerBase.Cast(man);
            if (player && player.GetIdentity() && player.GetIdentity().GetId() == identity.GetId()) return player;
        }
        return null;
    }

    private bool Validate(PlayerBase player, BuildingBase building)
    {
        if (!player || !player.GetIdentity() || !building) return false;
        if (!DZPSettingsService.Find(building.GetType())) return false;
        return vector.Distance(player.GetPosition(), building.GetPosition()) <= DZPSettingsService.Get().InteractionDistance + 1.0;
    }

    void OnRPC(PlayerIdentity sender, Object target, int rpcType, ParamsReadContext ctx)
    {
        if (!g_Game.IsServer()) return;
        if (rpcType < DZ_PROPERTYZ_RPC.REQUEST_PROPERTY || rpcType > DZ_PROPERTYZ_RPC.REQUEST_OWNED_PROPERTIES) return;

        PlayerBase player = FindPlayer(sender);
        if (rpcType == DZ_PROPERTYZ_RPC.REQUEST_OWNED_PROPERTIES)
        {
            SendOwnedProperties(player);
            return;
        }

        BuildingBase building = BuildingBase.Cast(target);
        if (!Validate(player, building))
        {
            Print(string.Format("[DeutschZ PropertyZ] RPC rejected type=%1 target=%2 player=%3", rpcType, target, sender));
            SendMessage(sender, false, "Ungültige oder zu weit entfernte Immobilie.");
            return;
        }

        Print(string.Format("[DeutschZ PropertyZ] RPC accepted type=%1 house=%2 player=%3", rpcType, building.GetType(), sender.GetId()));

        switch (rpcType)
        {
            case DZ_PROPERTYZ_RPC.REQUEST_PROPERTY:
                SendSnapshot(player, building);
                break;
            case DZ_PROPERTYZ_RPC.BUY_PROPERTY:
                Buy(player, building);
                break;
            case DZ_PROPERTYZ_RPC.SELL_PROPERTY:
                Sell(player, building);
                break;
            case DZ_PROPERTYZ_RPC.TOGGLE_DOOR_LOCK:
                ToggleLocks(player, building);
                break;
        }
    }

    private void Buy(PlayerBase player, BuildingBase building)
    {
        DZPPropertyStore store = DZPPropertyStore.Get();
        string propertyId = store.GetId(building);
        if (!store.Begin(propertyId))
        {
            SendMessage(player.GetIdentity(), false, "Für dieses Haus läuft bereits ein Kaufvorgang.");
            return;
        }

        DZPHouseTypeSetting setting = DZPSettingsService.Find(building.GetType());
        if (store.Find(building))
        {
            store.End(propertyId);
            SendMessage(player.GetIdentity(), false, "Dieses Haus gehört bereits jemandem.");
            SendSnapshot(player, building);
            return;
        }
        if (store.CountOwned(player.GetIdentity().GetId()) >= DZPSettingsService.Get().MaxPropertiesPerPlayer)
        {
            store.End(propertyId);
            SendMessage(player.GetIdentity(), false, "Du hast bereits die maximale Anzahl an Häusern.");
            return;
        }
        if (!DZPExpansionEconomy.Debit(player, setting.BuyPrice))
        {
            store.End(propertyId);
            SendMessage(player.GetIdentity(), false, "Nicht genug Expansion-Währung.");
            return;
        }

        DZPPropertyRecord record = new DZPPropertyRecord();
        record.PropertyId = propertyId;
        record.Type = building.GetType();
        record.Position = building.GetPosition();
        record.OwnerId = player.GetIdentity().GetId();
        record.OwnerName = player.GetIdentity().GetName();
        record.OwnerPartyId = player.Expansion_GetPartyID();
        record.PurchasePrice = setting.BuyPrice;
        record.ControllerLayoutVersion = 2;
        store.Insert(record);
        if (setting.HasControllerReference)
            DZPPropertyControllerService.Get().PlaceAtConfiguredEntrance(record, building);
        else
            DZPPropertyControllerService.Get().Ensure(record, player);
        store.End(propertyId);
        DZPEventBus.OnPropertyPurchased.Invoke(record.OwnerId, record.PropertyId, record.Type, record.Position, setting.BuyPrice);
        Print(string.Format("[DeutschZ PropertyZ] PURCHASE owner=%1 property=%2 price=%3", record.OwnerId, record.PropertyId, setting.BuyPrice));
        SendMessage(player.GetIdentity(), true, "Haus erfolgreich gekauft.");
        SendSnapshot(player, building);
        SendOwnedProperties(player);
    }

    private void Sell(PlayerBase player, BuildingBase building)
    {
        DZPPropertyStore store = DZPPropertyStore.Get();
        DZPPropertyRecord record = store.Find(building);
        DZPHouseTypeSetting setting = DZPSettingsService.Find(building.GetType());
        if (!record || record.OwnerId != player.GetIdentity().GetId())
        {
            SendMessage(player.GetIdentity(), false, "Nur der Eigentümer kann dieses Haus verkaufen.");
            return;
        }
        if (!DZPPropertyControllerService.Get().IsCargoEmpty(record))
        {
            SendMessage(player.GetIdentity(), false, "Vor dem Verkauf muss das Hauslager vollständig geleert werden.");
            return;
        }
        if (!DZPExpansionEconomy.Credit(player, setting.SellPrice))
        {
            SendMessage(player.GetIdentity(), false, "Auszahlung über Expansion fehlgeschlagen.");
            return;
        }
        for (int door = 0; door < building.GetDoorCount(); door++) building.UnlockDoor(door);
        DZPPropertyControllerService.Get().Remove(record);
        store.Remove(record);
        DZPEventBus.OnPropertySold.Invoke(record.OwnerId, record.PropertyId, record.Type, record.Position, setting.SellPrice);
        Print(string.Format("[DeutschZ PropertyZ] SALE owner=%1 property=%2 price=%3", record.OwnerId, record.PropertyId, setting.SellPrice));
        SendMessage(player.GetIdentity(), true, "Haus verkauft und Auszahlung erhalten.");
        SendSnapshot(player, building);
        SendOwnedProperties(player);
    }

    private void ToggleLocks(PlayerBase player, BuildingBase building)
    {
        DZPPropertyRecord record = DZPPropertyStore.Get().Find(building);
        if (!record || record.OwnerId != player.GetIdentity().GetId())
        {
            SendMessage(player.GetIdentity(), false, "Nur der Eigentümer darf die Türen verriegeln.");
            return;
        }

        if (record.Raided)
        {
            record.Raided = false;
            record.ControllerPosition = vector.Zero;
            record.SecondControllerPosition = vector.Zero;
            DZPPropertyStore.Get().Save();
            DZPHouseTypeSetting repairSetting = DZPSettingsService.Find(building.GetType());
            if (repairSetting && repairSetting.HasControllerReference)
                DZPPropertyControllerService.Get().PlaceAtConfiguredEntrance(record, building);
            SendMessage(player.GetIdentity(), false, "Hauszugang wurde nach dem Raid erneuert. Bringe neue CodeLocks an und setze die PINs.");
            return;
        }

        DZP_PropertyDoorController controller = DZPPropertyControllerService.Get().Ensure(record, player);
        if (!controller)
        {
            SendMessage(player.GetIdentity(), false, "Hauscontroller konnte nicht erzeugt werden.");
            return;
        }
        if (!DZPPropertyControllerService.Get().HasConfiguredLocks(record))
        {
            DZPHouseTypeSetting setting = DZPSettingsService.Find(building.GetType());
            if (setting && setting.HasControllerReference)
                controller = DZPPropertyControllerService.Get().PlaceAtConfiguredEntrance(record, building);
            else
                controller = DZPPropertyControllerService.Get().PlaceAtPlayer(record, player);
            if (!controller)
            {
                SendMessage(player.GetIdentity(), false, "Hauscontroller konnte nicht am Eingang platziert werden.");
                return;
            }
            if (setting && setting.HasControllerReference)
            {
                if (setting.HasSecondControllerReference)
                    SendMessage(player.GetIdentity(), false, "Zwei Hauscontroller wurden an den referenzierten Eingängen platziert. Bringe an beiden ein CodeLock an und setze die PINs.");
                else
                    SendMessage(player.GetIdentity(), false, "Hauscontroller wurde am referenzierten Hauseingang platziert. Bringe dort das CodeLock an und setze die PIN.");
            }
            else
                SendMessage(player.GetIdentity(), false, "Hauscontroller wurde sichtbar vor dir platziert. Nimm das CodeLock in die Hand, bringe es dort an und setze die PIN.");
            return;
        }
        DZPPropertyControllerService.Get().Synchronize(record, building, player);
        SendMessage(player.GetIdentity(), true, "Die Türsteuerung übernimmt jetzt das CodeLock am Hauseingang.");
        SendSnapshot(player, building);
    }

    private void SendSnapshot(PlayerBase player, BuildingBase building)
    {
        DZPHouseTypeSetting setting = DZPSettingsService.Find(building.GetType());
        DZPPropertyRecord record = DZPPropertyStore.Get().Find(building);
        string ownerId;
        string ownerName;
        int buyPrice;
        int sellPrice;
        bool isOwned;
        bool playerIsOwner;
        ref array<int> lockedDoors = new array<int>;
        if (setting)
        {
            buyPrice = setting.BuyPrice;
            sellPrice = setting.SellPrice;
        }
        if (record)
        {
            DZPPropertyControllerService.Get().Synchronize(record, building, player);
            isOwned = true;
            ownerId = record.OwnerId;
            ownerName = record.OwnerName;
            playerIsOwner = record.OwnerId == player.GetIdentity().GetId();
            lockedDoors.InsertAll(record.LockedDoors);
        }
        Param4<string, string, int, int> values = new Param4<string, string, int, int>(ownerId, ownerName, buyPrice, sellPrice);
        Param4<bool, bool, int, ref array<int>> state = new Param4<bool, bool, int, ref array<int>>(isOwned, playerIsOwner, DZP_PROTOCOL_VERSION, lockedDoors);
        g_Game.RPCSingleParam(building, DZ_PROPERTYZ_RPC.PROPERTY_RESPONSE, new Param2<ref Param4<string, string, int, int>, ref Param4<bool, bool, int, ref array<int>>>(values, state), true, player.GetIdentity());
    }

    private DZPPropertyRecord GetOwnedRecord(PlayerBase player, BuildingBase building)
    {
        DZPPropertyRecord record = DZPPropertyStore.Get().Find(building);
        if (!record || !player || !player.GetIdentity() || record.OwnerId != player.GetIdentity().GetId()) return null;
        return record;
    }

    private void StoreHandItem(PlayerBase player, BuildingBase building)
    {
        DZPPropertyRecord property = GetOwnedRecord(player, building);
        if (!property)
        {
            SendMessage(player.GetIdentity(), false, "Nur der Eigentümer darf das Hauslager benutzen.");
            return;
        }
        EntityAI entity = player.GetHumanInventory().GetEntityInHands();
        if (!entity || !InventoryItem.Cast(entity))
        {
            SendMessage(player.GetIdentity(), false, "Nimm den einzulagernden Gegenstand zuerst in die Hände.");
            return;
        }

        DZPHouseTypeSetting setting = DZPSettingsService.Find(building.GetType());
        DZPStorageStore storage = DZPStorageStore.Get();
        DZPPropertyStorageRecord record = storage.GetRecord(property.PropertyId);
        int category = storage.ResolveCategory(entity, setting);
        DZPStoredItem stored = storage.Serialize(entity, category);
        if (!stored)
        {
            SendMessage(player.GetIdentity(), false, "Gegenstand konnte nicht gelesen werden.");
            return;
        }

        if (category == DZP_STORAGE_CATEGORY.GENERAL && storage.UsedGeneralSlots(record) + stored.UsedSlots > setting.StorageSlots)
        {
            SendMessage(player.GetIdentity(), false, "Nicht genug freie Inventarplätze im Hauslager.");
            return;
        }
        if (category == DZP_STORAGE_CATEGORY.WEAPON && storage.CountCategory(record, category) >= setting.WeaponSlots)
        {
            SendMessage(player.GetIdentity(), false, "Alle Waffenplätze sind belegt.");
            return;
        }
        if (category == DZP_STORAGE_CATEGORY.BARREL && storage.CountCategory(record, category) >= setting.BarrelSlots)
        {
            SendMessage(player.GetIdentity(), false, "Alle Fassplätze sind belegt.");
            return;
        }
        if (category == DZP_STORAGE_CATEGORY.SEA_CHEST && storage.CountCategory(record, category) >= setting.SeaChestSlots)
        {
            SendMessage(player.GetIdentity(), false, "Alle Seemannskistenplätze sind belegt.");
            return;
        }
        if (category == DZP_STORAGE_CATEGORY.WOODEN_CRATE && storage.CountCategory(record, category) >= setting.WoodenCrateSlots)
        {
            SendMessage(player.GetIdentity(), false, "Alle Holzkistenplätze sind belegt.");
            return;
        }

        record.Items.Insert(stored);
        storage.Save();
        storage.Log("STORE", player, property.PropertyId, stored.Type);
        GetGame().ObjectDelete(entity);
        SendMessage(player.GetIdentity(), true, "Gegenstand wurde im Hauslager gespeichert.");
        SendStorage(player, building);
    }

    private void WithdrawItem(PlayerBase player, BuildingBase building, string itemId)
    {
        DZPPropertyRecord property = GetOwnedRecord(player, building);
        if (!property)
        {
            SendMessage(player.GetIdentity(), false, "Nur der Eigentümer darf das Hauslager benutzen.");
            return;
        }
        DZPStorageStore storage = DZPStorageStore.Get();
        DZPPropertyStorageRecord record = storage.GetRecord(property.PropertyId, false);
        if (!record || !record.Items) return;
        foreach (DZPStoredItem item : record.Items)
        {
            if (!item || item.Id != itemId) continue;
            EntityAI restored = storage.Restore(item, player);
            if (!restored)
            {
                SendMessage(player.GetIdentity(), false, "Gegenstand konnte nicht ausgegeben werden; Datensatz bleibt erhalten.");
                return;
            }
            record.Items.RemoveItem(item);
            storage.Save();
            storage.Log("WITHDRAW", player, property.PropertyId, item.Type);
            SendMessage(player.GetIdentity(), true, "Gegenstand wurde ausgegeben.");
            SendStorage(player, building);
            return;
        }
        SendMessage(player.GetIdentity(), false, "Lagergegenstand wurde nicht gefunden.");
    }

    private void SendStorage(PlayerBase player, BuildingBase building)
    {
        DZPPropertyRecord property = GetOwnedRecord(player, building);
        if (!property) return;
        DZPHouseTypeSetting setting = DZPSettingsService.Find(building.GetType());
        DZPStorageStore storage = DZPStorageStore.Get();
        DZPPropertyStorageRecord record = storage.GetRecord(property.PropertyId);
        ref array<string> ids = new array<string>;
        ref array<string> types = new array<string>;
        ref array<int> categories = new array<int>;
        ref array<int> sizes = new array<int>;
        foreach (DZPStoredItem item : record.Items)
        {
            if (!item) continue;
            ids.Insert(item.Id);
            types.Insert(item.Type);
            categories.Insert(item.Category);
            sizes.Insert(item.UsedSlots);
        }
        Param4<ref array<string>, ref array<string>, ref array<int>, ref array<int>> items = new Param4<ref array<string>, ref array<string>, ref array<int>, ref array<int>>(ids, types, categories, sizes);
        Param4<int, int, int, int> limits = new Param4<int, int, int, int>(setting.StorageSlots, setting.WeaponSlots, setting.BarrelSlots, setting.SeaChestSlots);
        Param4<int, int, int, int> used = new Param4<int, int, int, int>(storage.UsedGeneralSlots(record), storage.CountCategory(record, DZP_STORAGE_CATEGORY.WEAPON), storage.CountCategory(record, DZP_STORAGE_CATEGORY.BARREL), storage.CountCategory(record, DZP_STORAGE_CATEGORY.SEA_CHEST));
        g_Game.RPCSingleParam(building, DZ_PROPERTYZ_RPC.STORAGE_RESPONSE, new Param3<ref Param4<ref array<string>, ref array<string>, ref array<int>, ref array<int>>, ref Param4<int, int, int, int>, ref Param4<int, int, int, int>>(items, limits, used), true, player.GetIdentity());
    }

    private void SendMessage(PlayerIdentity identity, bool success, string text)
    {
        if (!identity) return;
        g_Game.RPCSingleParam(null, DZ_PROPERTYZ_RPC.RESULT_MESSAGE, new Param2<bool, string>(success, text), true, identity);
    }

    private void SendOwnedProperties(PlayerBase player)
    {
        if (!player || !player.GetIdentity()) return;
        PlayerIdentity identity = player.GetIdentity();

        ref array<string> ids = new array<string>;
        ref array<string> types = new array<string>;
        ref array<vector> positions = new array<vector>;
        DZPPropertyStore store = DZPPropertyStore.Get();
        array<ref DZPPropertyRecord> records;
        if (store) records = store.GetAll();
        if (!records)
        {
            Print("[DeutschZ PropertyZ] Owned property marker request used empty store fallback");
            records = new array<ref DZPPropertyRecord>;
        }
        foreach (DZPPropertyRecord record : records)
        {
            if (!record || record.OwnerId != identity.GetId()) continue;
            ids.Insert(record.PropertyId);
            types.Insert(record.Type);
            positions.Insert(record.Position);
        }

        DZPSettings settings = DZPSettingsService.Get();
        bool showMarkers = true;
        float markerDistance = 5000.0;
        if (settings)
        {
            showMarkers = settings.ShowOwnedHouse3DMarkers;
            markerDistance = settings.OwnedHouseMarkerMaxDistance;
        }
        Param3<bool, float, int> markerSettings = new Param3<bool, float, int>(showMarkers, markerDistance, DZP_PROTOCOL_VERSION);
        Param3<ref array<string>, ref array<string>, ref array<vector>> properties = new Param3<ref array<string>, ref array<string>, ref array<vector>>(ids, types, positions);
        g_Game.RPCSingleParam(null, DZ_PROPERTYZ_RPC.OWNED_PROPERTIES_RESPONSE, new Param2<ref Param3<ref array<string>, ref array<string>, ref array<vector>>, ref Param3<bool, float, int>>(properties, markerSettings), true, identity);
    }
}
