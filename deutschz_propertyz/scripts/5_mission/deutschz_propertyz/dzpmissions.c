modded class MissionServer
{
    void MissionServer()
    {
        DZPServerController.Get();
    }
}

modded class MissionGameplay
{
    private ref DZPOwnedHouseMarkerManager m_DZP_MarkerManager;
    private float m_DZP_MarkerRequestDelay;
    private bool m_DZP_MarkersRequested;

    void MissionGameplay()
    {
        DZPClientRPC.Get();
        m_DZP_MarkerManager = new DZPOwnedHouseMarkerManager();
    }

    override void OnUpdate(float timeslice)
    {
        super.OnUpdate(timeslice);
        UAInput markerToggle = GetUApi().GetInputByName("UADZPropertyZMarkers");
        if (markerToggle && markerToggle.LocalPress() && m_DZP_MarkerManager)
            m_DZP_MarkerManager.Toggle();
        if (!m_DZP_MarkersRequested)
        {
            m_DZP_MarkerRequestDelay += timeslice;
            if (m_DZP_MarkerRequestDelay >= 3.0 && GetGame().GetPlayer())
            {
                GetGame().RPCSingleParam(null, DZ_PROPERTYZ_RPC.REQUEST_OWNED_PROPERTIES, null, true);
                m_DZP_MarkersRequested = true;
                Print("[PropertyZ] Requested owned property markers");
            }
        }
        if (m_DZP_MarkerManager)
            m_DZP_MarkerManager.Update(timeslice);
    }

    override void OnMissionFinish()
    {
        m_DZP_MarkerManager = null;
        super.OnMissionFinish();
    }

    override UIScriptedMenu CreateScriptedMenu(int id)
    {
        if (id == DZP_PROPERTY_MENU_ID)
            return new DZPPropertyMenu;

        return super.CreateScriptedMenu(id);
    }
}
