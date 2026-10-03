modded class BuildingBase
{
    bool m_DZP_StateReceived;
    string m_DZP_OwnerId;
    string m_DZP_OwnerName;
    int m_DZP_BuyPrice;
    int m_DZP_SellPrice;
    bool m_DZP_Owned;
    bool m_DZP_PlayerIsOwner;
    ref array<int> m_DZP_LockedDoors = new array<int>;

    bool DZPIsSupported()
    {
        return DZPSettingsService.Find(GetType()) != null;
    }
}
