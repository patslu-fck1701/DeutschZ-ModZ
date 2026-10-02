class DZPPropertyMenu : UIScriptedMenu
{
    static DZPPropertyMenu s_Active;
    private BuildingBase m_Building;
    private TextWidget m_Title;
    private TextWidget m_Status;
    private TextWidget m_Price;
    private ButtonWidget m_Buy;
    private ButtonWidget m_Sell;
    private ButtonWidget m_Lock;
    private ButtonWidget m_Close;
    private Widget m_SumLabel;
    private Widget m_SellConfirmation;
    private ButtonWidget m_ConfirmSell;
    private ButtonWidget m_CancelSell;
    private bool m_SellPending;

    override void Update(float timeslice)
    {
        super.Update(timeslice);
        if (!g_Game || !GetUApi().GetInputByID(UAUIBack).LocalPress())
            return;

        if (m_SellConfirmation && m_SellConfirmation.IsVisible())
            ShowSellConfirmation(false);
        else
            Close();
    }

    override bool OnKeyDown(Widget w, int x, int y, int key)
    {
        if (key == KeyCode.KC_ESCAPE)
        {
            Close();
            return true;
        }
        return super.OnKeyDown(w, x, y, key);
    }

    private void ShowSellConfirmation(bool show)
    {
        m_SellConfirmation.Show(show);
        m_Buy.Enable(!show);
        m_Sell.Enable(!show && !m_SellPending);
        m_Lock.Enable(!show);
    }


    void SetBuilding(BuildingBase building)
    {
        m_Building = building;
        if (m_Building)
        {
            m_Building.m_DZP_StateReceived = false;
            Refresh();
        }
    }

    override Widget Init()
    {
        Print("[PropertyZ] DZPPropertyMenu.Init start");

        WorkspaceWidget workspace = GetGame().GetWorkspace();
        if (!workspace)
        {
            Print("[PropertyZ] ERROR: GetWorkspace returned null");
            return null;
        }

        layoutRoot = workspace.CreateWidgets("deutschz_propertyz/layout/PropertyZ/PropertyMenu.layout");
        if (!layoutRoot)
        {
            Print("[PropertyZ] ERROR: Could not load layout: deutschz_propertyz/layout/PropertyZ/PropertyMenu.layout");
            return null;
        }

        m_Title = TextWidget.Cast(layoutRoot.FindAnyWidget("Title"));
        m_Status = TextWidget.Cast(layoutRoot.FindAnyWidget("Status"));
        m_Price = TextWidget.Cast(layoutRoot.FindAnyWidget("Price"));
        m_Buy = ButtonWidget.Cast(layoutRoot.FindAnyWidget("BuyButton"));
        m_Sell = ButtonWidget.Cast(layoutRoot.FindAnyWidget("SellButton"));
        m_Lock = ButtonWidget.Cast(layoutRoot.FindAnyWidget("LockButton"));
        m_Close = ButtonWidget.Cast(layoutRoot.FindAnyWidget("CloseButton"));
        m_SumLabel = layoutRoot.FindAnyWidget("SumLabel");
        m_SellConfirmation = layoutRoot.FindAnyWidget("SellConfirmation");
        m_ConfirmSell = ButtonWidget.Cast(layoutRoot.FindAnyWidget("ConfirmSellButton"));
        m_CancelSell = ButtonWidget.Cast(layoutRoot.FindAnyWidget("CancelSellButton"));

        if (!m_Title || !m_Status || !m_Price || !m_Buy || !m_Sell || !m_Lock || !m_Close || !m_SumLabel || !m_SellConfirmation || !m_ConfirmSell || !m_CancelSell)
        {
            Print("[PropertyZ] ERROR: One or more required widgets are missing from PropertyMenu.layout");
            return layoutRoot;
        }

        SetButtonColor(m_Buy, false);
        SetButtonColor(m_Sell, false);
        SetButtonColor(m_Lock, false);
        SetButtonColor(m_Close, false);
        s_Active = this;
        Refresh();
        Print("[PropertyZ] DZPPropertyMenu.Init success");
        return layoutRoot;
    }

    override void OnShow()
    {
        super.OnShow();
        GetGame().GetUIManager().ShowCursor(true);


        Mission mission = GetGame().GetMission();
        if (mission)
            mission.PlayerControlDisable(INPUT_EXCLUDE_INVENTORY);

        Print("[PropertyZ] DZPPropertyMenu.OnShow");
    }

    override void OnHide()
    {
        Print("[PropertyZ] DZPPropertyMenu.OnHide");
        if (s_Active == this)
            s_Active = null;

        if (GetGame())
        {
            GetGame().GetUIManager().ShowCursor(false);


            Mission mission = GetGame().GetMission();
            if (mission)
                mission.PlayerControlEnable(true);
        }

        super.OnHide();
    }

    override void Refresh()
    {
        if (!layoutRoot || !m_Building || !m_Title || !m_Status || !m_Price || !m_Buy || !m_Sell || !m_Lock)
            return;

        m_Title.SetText("PropertyZ");
        if (!m_Building.m_DZP_StateReceived)
        {
            m_Status.SetText("Immobiliendaten werden geladen ...");
            m_Price.SetText("");
            m_Buy.Show(false);
            m_Sell.Show(false);
            m_Lock.Show(false);
            return;
        }

        if (!m_Building.m_DZP_Owned)
        {
            m_Status.SetText("Dieses Haus steht zum Verkauf.");
            m_Price.SetText(m_Building.m_DZP_BuyPrice.ToString() + ",00 €");
            m_Buy.Show(true);
            m_Sell.Show(false);
            m_Lock.Show(false);
        }
        else
        {
            m_Status.SetText("Eigentümer: " + m_Building.m_DZP_OwnerName);
            m_Price.SetText(m_Building.m_DZP_SellPrice.ToString() + ",00 €");
            m_Buy.Show(false);
            m_Sell.Show(m_Building.m_DZP_PlayerIsOwner);
            m_Lock.Show(m_Building.m_DZP_PlayerIsOwner);
            if (m_Building.m_DZP_PlayerIsOwner)
            {
                if (m_Building.m_DZP_LockedDoors.Count() > 0)
                    m_Lock.SetText("Alle Türen entriegeln");
                else
                    m_Lock.SetText("Alle Türen verriegeln");
            }
        }
    }

    void SetResult(bool success, string messageText)
    {
        if (!m_Status)
            return;

        m_SellPending = false;
        m_Sell.Enable(true);
        m_Status.SetText(messageText);
        if (success)
            m_Status.SetColor(ARGB(255, 80, 220, 120));
        else
            m_Status.SetColor(ARGB(255, 235, 80, 80));
    }

    override bool OnClick(Widget w, int x, int y, int button)
    {
        if (button != MouseState.LEFT || !m_Building)
            return false;

        if (w == m_Buy)
        {
            Print(string.Format("[PropertyZ] BUY clicked house=%1", m_Building.GetType()));
            GetGame().RPCSingleParam(m_Building, DZ_PROPERTYZ_RPC.BUY_PROPERTY, null, true);
            m_Status.SetText("Kauf wird serverseitig geprüft ...");
            return true;
        }

        if (w == m_CancelSell)
        {
            ShowSellConfirmation(false);
            return true;
        }
        if (w == m_ConfirmSell && m_SellConfirmation.IsVisible() && !m_SellPending)
        {
            if (!m_Building.m_DZP_StateReceived || !m_Building.m_DZP_PlayerIsOwner) return true;
            m_SellPending = true;
            ShowSellConfirmation(false);
            GetGame().RPCSingleParam(m_Building, DZ_PROPERTYZ_RPC.SELL_PROPERTY, null, true);
            m_Status.SetText("Verkauf wird serverseitig geprüft ...");
            return true;
        }
        if (w == m_Sell && !m_SellPending)
        {
            ShowSellConfirmation(true);
            return true;
        }

        if (w == m_Lock)
        {
            GetGame().RPCSingleParam(m_Building, DZ_PROPERTYZ_RPC.TOGGLE_DOOR_LOCK, null, true);
            return true;
        }

        if (w && w.GetName() == "CloseButton")
        {
            Close();
            return true;
        }

        return false;
    }

    override bool OnMouseEnter(Widget w, int x, int y)
    {
        ButtonWidget button = ButtonWidget.Cast(w);
        if (IsMenuButton(button))
        {
            SetButtonColor(button, true);
            return true;
        }

        return super.OnMouseEnter(w, x, y);
    }

    override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
    {
        ButtonWidget button = ButtonWidget.Cast(w);
        if (IsMenuButton(button))
        {
            SetButtonColor(button, false);
            return true;
        }

        return super.OnMouseLeave(w, enterW, x, y);
    }

    private bool IsMenuButton(ButtonWidget button)
    {
        return button && (button == m_Buy || button == m_Sell || button == m_Lock || button == m_Close || button == m_ConfirmSell || button == m_CancelSell);
    }

    private void SetButtonColor(ButtonWidget button, bool hovered)
    {
        if (!button)
            return;

        if (hovered)
            button.SetColor(ARGB(235, 104, 220, 0));
        else
            button.SetColor(ARGB(190, 35, 39, 42));
    }

}

class DZPClientRPC
{
    private static ref DZPClientRPC s_Instance;

    static DZPClientRPC Get()
    {
        if (!s_Instance)
            s_Instance = new DZPClientRPC();
        return s_Instance;
    }

    void DZPClientRPC()
    {
        GetGame().Event_OnRPC.Insert(OnRPC);
    }

    void ~DZPClientRPC()
    {
        if (GetGame() && GetGame().Event_OnRPC)
            GetGame().Event_OnRPC.Remove(OnRPC);
    }

    void OnRPC(PlayerIdentity sender, Object target, int rpcType, ParamsReadContext ctx)
    {
        if (GetGame().IsServer())
            return;

        if (rpcType == DZ_PROPERTYZ_RPC.PROPERTY_RESPONSE)
        {
            Param2<ref Param4<string, string, int, int>, ref Param4<bool, bool, int, ref array<int>>> data;
            if (!ctx.Read(data) || !data.param1 || !data.param2 || data.param2.param3 != DZP_PROTOCOL_VERSION)
                return;

            BuildingBase building = BuildingBase.Cast(target);
            if (!building)
                return;

            building.m_DZP_OwnerId = data.param1.param1;
            building.m_DZP_OwnerName = data.param1.param2;
            building.m_DZP_BuyPrice = data.param1.param3;
            building.m_DZP_SellPrice = data.param1.param4;
            building.m_DZP_Owned = data.param2.param1;
            building.m_DZP_PlayerIsOwner = data.param2.param2;
            building.m_DZP_LockedDoors.Clear();
            building.m_DZP_LockedDoors.InsertAll(data.param2.param4);
            building.m_DZP_StateReceived = true;

            if (DZPPropertyMenu.s_Active)
                DZPPropertyMenu.s_Active.Refresh();
        }
        else if (rpcType == DZ_PROPERTYZ_RPC.RESULT_MESSAGE)
        {
            Param2<bool, string> result;
            if (!ctx.Read(result))
                return;

            if (DZPPropertyMenu.s_Active)
                DZPPropertyMenu.s_Active.SetResult(result.param1, result.param2);
        }
        else if (rpcType == DZ_PROPERTYZ_RPC.OWNED_PROPERTIES_RESPONSE)
        {
            Param2<ref Param3<ref array<string>, ref array<string>, ref array<vector>>, ref Param3<bool, float, int>> markerData;
            if (!ctx.Read(markerData) || !markerData.param1 || !markerData.param2 || markerData.param2.param3 != DZP_PROTOCOL_VERSION) return;
            DZPOwnedPropertyState.Set(markerData.param1.param1, markerData.param1.param2, markerData.param1.param3, markerData.param2.param1, markerData.param2.param2);
        }
    }
}

class DZPOwnedPropertyState
{
    static ref array<string> Ids = new array<string>;
    static ref array<string> Types = new array<string>;
    static ref array<vector> Positions = new array<vector>;
    static bool Enabled;
    static float MaxDistance;
    static int Revision;

    static bool Owns(string propertyType, vector propertyPosition)
    {
        int count = Positions.Count();
        if (Types.Count() < count) count = Types.Count();
        for (int i = 0; i < count; i++)
        {
            if (Types[i] == propertyType && vector.DistanceSq(Positions[i], propertyPosition) <= 16.0)
                return true;
        }
        return false;
    }

    static void Set(array<string> ids, array<string> types, array<vector> positions, bool enabled, float maxDistance)
    {
        Ids.Clear(); Types.Clear(); Positions.Clear();
        if (ids) Ids.InsertAll(ids);
        if (types) Types.InsertAll(types);
        if (positions) Positions.InsertAll(positions);
        Enabled = enabled;
        MaxDistance = maxDistance;
        Revision++;
        Print(string.Format("[PropertyZ] Owned property markers received: %1", Positions.Count()));
    }
}
