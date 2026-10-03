class DZPPropertyRecord
{
    string PropertyId;
    string Type;
    vector Position;
    string OwnerId;
    string OwnerName;
    int OwnerPartyId = -1;
    int PurchasePrice;
    vector ControllerPosition;
    vector SecondControllerPosition;
    int ControllerLayoutVersion;
    bool Raided;
    ref array<int> LockedDoors = new array<int>;
}

class DZPPropertyStoreFile
{
    int SchemaVersion = 2;
    ref array<ref DZPPropertyRecord> Properties = new array<ref DZPPropertyRecord>;
}

class DZPPropertyStore
{
    private static ref DZPPropertyStore s_Instance;
    private static const string FILE = "$profile:DeutschZ-System\\PropertyZ\\Properties.json";
    private ref DZPPropertyStoreFile m_Data;
    private ref map<string, bool> m_Busy = new map<string, bool>;

    static DZPPropertyStore Get()
    {
        if (!s_Instance) s_Instance = new DZPPropertyStore();
        return s_Instance;
    }

    void DZPPropertyStore()
    {
        DZPSettingsService.Get();
        m_Data = new DZPPropertyStoreFile();
        if (FileExist(FILE)) JsonFileLoader<DZPPropertyStoreFile>.JsonLoadFile(FILE, m_Data);
        if (!m_Data.Properties) m_Data.Properties = new array<ref DZPPropertyRecord>;
    }

    string GetId(BuildingBase building)
    {
        if (!building) return "";
        vector p = building.GetPosition();
        return string.Format("%1_%2_%3_%4", building.GetType(), Math.Round(p[0] * 10), Math.Round(p[1] * 10), Math.Round(p[2] * 10));
    }

    DZPPropertyRecord Find(BuildingBase building)
    {
        string id = GetId(building);
        foreach (DZPPropertyRecord record : m_Data.Properties)
            if (record && record.PropertyId == id) return record;
        return null;
    }

    int CountOwned(string playerId)
    {
        int count;
        foreach (DZPPropertyRecord record : m_Data.Properties)
            if (record && record.OwnerId == playerId) count++;
        return count;
    }

    bool Begin(string propertyId)
    {
        if (m_Busy.Contains(propertyId)) return false;
        m_Busy.Set(propertyId, true);
        return true;
    }

    void End(string propertyId)
    {
        m_Busy.Remove(propertyId);
    }

    void Insert(DZPPropertyRecord record)
    {
        m_Data.Properties.Insert(record);
        Save();
    }

    void Remove(DZPPropertyRecord record)
    {
        m_Data.Properties.RemoveItem(record);
        Save();
    }

    void Save()
    {
        JsonFileLoader<DZPPropertyStoreFile>.JsonSaveFile(FILE, m_Data);
    }

    array<ref DZPPropertyRecord> GetAll()
    {
        if (!m_Data)
            m_Data = new DZPPropertyStoreFile();
        if (!m_Data.Properties)
            m_Data.Properties = new array<ref DZPPropertyRecord>;
        return m_Data.Properties;
    }
}
