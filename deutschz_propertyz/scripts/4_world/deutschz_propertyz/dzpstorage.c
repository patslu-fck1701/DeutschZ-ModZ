class DZPStoredItem
{
    string Id;
    string Type;
    float Health;
    float Quantity;
    int AmmoCount;
    int UsedSlots;
    int Category;
    ref array<ref DZPStoredItem> Children = new array<ref DZPStoredItem>;
}

class DZPPropertyStorageRecord
{
    string PropertyId;
    ref array<ref DZPStoredItem> Items = new array<ref DZPStoredItem>;
}

class DZPPropertyStorageFile
{
    int SchemaVersion = 1;
    ref array<ref DZPPropertyStorageRecord> Storages = new array<ref DZPPropertyStorageRecord>;
}

class DZPStorageStore
{
    private static ref DZPStorageStore s_Instance;
    private static const string FILE = "$profile:DeutschZ-System\\PropertyZ\\PropertyStorage.json";
    private static const string LOG_DIR = "$profile:DeutschZ-System\\LogZ\\PropertyZ";
    private static const string LOG_FILE = "$profile:DeutschZ-System\\LogZ\\PropertyZ\\PropertyZ.log";
    private ref DZPPropertyStorageFile m_Data;

    static DZPStorageStore Get()
    {
        if (!s_Instance) s_Instance = new DZPStorageStore();
        return s_Instance;
    }

    void DZPStorageStore()
    {
        if (!FileExist("$profile:DeutschZ-System\\LogZ")) MakeDirectory("$profile:DeutschZ-System\\LogZ");
        if (!FileExist(LOG_DIR)) MakeDirectory(LOG_DIR);
        m_Data = new DZPPropertyStorageFile();
        if (FileExist(FILE)) JsonFileLoader<DZPPropertyStorageFile>.JsonLoadFile(FILE, m_Data);
        if (!m_Data.Storages) m_Data.Storages = new array<ref DZPPropertyStorageRecord>;
    }

    DZPPropertyStorageRecord GetRecord(string propertyId, bool create = true)
    {
        foreach (DZPPropertyStorageRecord record : m_Data.Storages)
            if (record && record.PropertyId == propertyId) return record;
        if (!create) return null;
        DZPPropertyStorageRecord created = new DZPPropertyStorageRecord();
        created.PropertyId = propertyId;
        m_Data.Storages.Insert(created);
        return created;
    }

    bool IsEmpty(string propertyId)
    {
        DZPPropertyStorageRecord record = GetRecord(propertyId, false);
        return !record || !record.Items || record.Items.Count() == 0;
    }

    void RemoveProperty(string propertyId)
    {
        DZPPropertyStorageRecord record = GetRecord(propertyId, false);
        if (record) m_Data.Storages.RemoveItem(record);
        Save();
    }

    void Save()
    {
        JsonFileLoader<DZPPropertyStorageFile>.JsonSaveFile(FILE, m_Data);
    }

    void Log(string action, PlayerBase player, string propertyId, string itemType)
    {
        string owner = "unknown";
        string ownerId = "unknown";
        if (player && player.GetIdentity())
        {
            owner = player.GetIdentity().GetName();
            ownerId = player.GetIdentity().GetId();
        }
        FileHandle handle = OpenFile(LOG_FILE, FileMode.APPEND);
        if (handle)
        {
            FPrintln(handle, string.Format("%1 | %2 | owner=%3 | id=%4 | property=%5 | item=%6", GetDateTime(), action, owner, ownerId, propertyId, itemType));
            CloseFile(handle);
        }
    }

    private string GetDateTime()
    {
        int year, month, day, hour, minute, second;
        GetYearMonthDay(year, month, day);
        GetHourMinuteSecond(hour, minute, second);
        return string.Format("%1-%2-%3 %4:%5:%6", year, month, day, hour, minute, second);
    }

    int ResolveCategory(EntityAI item, DZPHouseTypeSetting setting)
    {
        if (setting.WeaponSlots > 0 && Weapon_Base.Cast(item)) return DZP_STORAGE_CATEGORY.WEAPON;
        if (setting.BarrelSlots > 0 && Barrel_ColorBase.Cast(item)) return DZP_STORAGE_CATEGORY.BARREL;
        if (setting.SeaChestSlots > 0 && SeaChest.Cast(item)) return DZP_STORAGE_CATEGORY.SEA_CHEST;
        if (setting.WoodenCrateSlots > 0 && WoodenCrate.Cast(item)) return DZP_STORAGE_CATEGORY.WOODEN_CRATE;
        return DZP_STORAGE_CATEGORY.GENERAL;
    }

    int CountCategory(DZPPropertyStorageRecord record, int category)
    {
        int count;
        foreach (DZPStoredItem item : record.Items)
            if (item && item.Category == category) count++;
        return count;
    }

    int UsedGeneralSlots(DZPPropertyStorageRecord record)
    {
        int used;
        foreach (DZPStoredItem item : record.Items)
            if (item && item.Category == DZP_STORAGE_CATEGORY.GENERAL) used += item.UsedSlots;
        return used;
    }

    DZPStoredItem Serialize(EntityAI item, int category = DZP_STORAGE_CATEGORY.GENERAL)
    {
        if (!item) return null;
        DZPStoredItem stored = new DZPStoredItem();
        stored.Id = string.Format("%1_%2", item.GetType(), Math.RandomInt(100000, 999999));
        stored.Type = item.GetType();
        stored.Health = item.GetHealth();
        stored.Category = category;
        int width = 1;
        int height = 1;
        InventoryItem inventoryItem = InventoryItem.Cast(item);
        if (inventoryItem) GetGame().GetInventoryItemSize(inventoryItem, width, height);
        stored.UsedSlots = width * height;
        ItemBase itemBase = ItemBase.Cast(item);
        if (itemBase && itemBase.HasQuantity()) stored.Quantity = itemBase.GetQuantity();
        Magazine magazine = Magazine.Cast(item);
        if (magazine) stored.AmmoCount = magazine.GetAmmoCount();
        if (item.GetInventory())
        {
            for (int i = 0; i < item.GetInventory().AttachmentCount(); i++)
            {
                EntityAI attachment = item.GetInventory().GetAttachmentFromIndex(i);
                DZPStoredItem storedAttachment = Serialize(attachment);
                if (storedAttachment) stored.Children.Insert(storedAttachment);
            }
            CargoBase cargo = item.GetInventory().GetCargo();
            if (cargo)
            {
                for (int c = 0; c < cargo.GetItemCount(); c++)
                {
                    DZPStoredItem storedCargo = Serialize(cargo.GetItem(c));
                    if (storedCargo) stored.Children.Insert(storedCargo);
                }
            }
        }
        return stored;
    }

    EntityAI Restore(DZPStoredItem stored, PlayerBase player)
    {
        if (!stored || !player) return null;
        EntityAI entity = player.GetInventory().CreateInInventory(stored.Type);
        if (!entity) entity = EntityAI.Cast(GetGame().CreateObjectEx(stored.Type, player.GetPosition(), ECE_PLACE_ON_SURFACE));
        if (!entity) return null;
        ApplyState(entity, stored);
        RestoreChildren(entity, stored.Children);
        return entity;
    }

    private void RestoreChildren(EntityAI parent, array<ref DZPStoredItem> children)
    {
        if (!parent || !children || !parent.GetInventory()) return;
        foreach (DZPStoredItem child : children)
        {
            EntityAI entity = parent.GetInventory().CreateAttachment(child.Type);
            if (!entity) entity = parent.GetInventory().CreateEntityInCargo(child.Type);
            if (!entity) continue;
            ApplyState(entity, child);
            RestoreChildren(entity, child.Children);
        }
    }

    private void ApplyState(EntityAI entity, DZPStoredItem stored)
    {
        entity.SetHealth(stored.Health);
        Magazine magazine = Magazine.Cast(entity);
        if (magazine) magazine.ServerSetAmmoCount(stored.AmmoCount);
        else
        {
            ItemBase itemBase = ItemBase.Cast(entity);
            if (itemBase && itemBase.HasQuantity()) itemBase.SetQuantity(stored.Quantity);
        }
    }
}
