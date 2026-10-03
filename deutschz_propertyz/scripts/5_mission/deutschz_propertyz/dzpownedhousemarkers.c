class DZPOwnedHouseMarkerVisual
{
    Widget Root;
    TextWidget Label;
}

class DZPOwnedHouseMarkerManager
{
    private ref array<ref DZPOwnedHouseMarkerVisual> m_Visuals = new array<ref DZPOwnedHouseMarkerVisual>;
    private int m_Revision = -1;
    private float m_UpdateAccumulator;
    private bool m_UserEnabled = true;

    void ~DZPOwnedHouseMarkerManager()
    {
        Clear();
    }

    void Update(float timeslice)
    {
        if (m_Revision != DZPOwnedPropertyState.Revision)
            Rebuild();

        m_UpdateAccumulator += timeslice;
        if (m_UpdateAccumulator < 0.05) return;
        m_UpdateAccumulator = 0;

        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        bool blocked = !player || !m_UserEnabled || !DZPOwnedPropertyState.Enabled || (GetGame().GetUIManager() && GetGame().GetUIManager().GetMenu());
        for (int i = 0; i < m_Visuals.Count(); i++)
        {
            DZPOwnedHouseMarkerVisual visual = m_Visuals[i];
            if (!visual || !visual.Root) continue;
            if (blocked || i >= DZPOwnedPropertyState.Positions.Count())
            {
                visual.Root.Show(false);
                continue;
            }
            Position(visual, DZPOwnedPropertyState.Positions[i], player.GetPosition());
        }
    }

    void Toggle()
    {
        m_UserEnabled = !m_UserEnabled;
        if (!m_UserEnabled)
        {
            foreach (DZPOwnedHouseMarkerVisual visual : m_Visuals)
                if (visual && visual.Root) visual.Root.Show(false);
        }
        Print("[PropertyZ] Owned property 3D markers user toggle=" + m_UserEnabled.ToString());
    }

    private void Rebuild()
    {
        Clear();
        m_Revision = DZPOwnedPropertyState.Revision;
        for (int i = 0; i < DZPOwnedPropertyState.Positions.Count(); i++)
        {
            Widget root = GetGame().GetWorkspace().CreateWidgets("deutschz_propertyz/layout/PropertyZ/OwnedHouseMarker.layout");
            if (!root) continue;
            DZPOwnedHouseMarkerVisual visual = new DZPOwnedHouseMarkerVisual;
            visual.Root = root;
            visual.Label = TextWidget.Cast(root.FindAnyWidget("OwnedHouseMarkerText"));
            root.Show(false);
            m_Visuals.Insert(visual);
        }
    }

    private void Position(DZPOwnedHouseMarkerVisual visual, vector housePosition, vector playerPosition)
    {
        float distance = vector.Distance(playerPosition, housePosition);
        if (DZPOwnedPropertyState.MaxDistance > 0 && distance > DZPOwnedPropertyState.MaxDistance)
        {
            visual.Root.Show(false);
            return;
        }

        vector markerPosition = housePosition;
        markerPosition[1] = markerPosition[1] + 4.0;
        vector screen = GetGame().GetScreenPos(markerPosition);
        int screenWidth;
        int screenHeight;
        GetScreenSize(screenWidth, screenHeight);
        if (screen[2] <= 0 || screen[0] <= 0 || screen[0] >= screenWidth || screen[1] <= 0 || screen[1] >= screenHeight)
        {
            visual.Root.Show(false);
            return;
        }

        if (visual.Label)
            visual.Label.SetText(string.Format("MEIN HAUS | %1 m", Math.Round(distance)));
        visual.Root.SetPos(Math.Round(screen[0] - 100), Math.Round(screen[1] - 18));
        visual.Root.Show(true);
    }

    private void Clear()
    {
        foreach (DZPOwnedHouseMarkerVisual visual : m_Visuals)
            if (visual && visual.Root) visual.Root.Unlink();
        m_Visuals.Clear();
    }
}
