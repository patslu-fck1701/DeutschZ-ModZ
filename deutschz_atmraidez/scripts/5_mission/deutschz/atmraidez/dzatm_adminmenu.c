class DZATM_AdminMenu: UIScriptedMenu
{
    protected TextWidget m_Status;
    protected TextWidget m_Message;
    protected ButtonWidget m_StatusButton;
    protected ButtonWidget m_ReloadButton;
    protected ButtonWidget m_CancelAllButton;
    protected ButtonWidget m_ClearNearestButton;
    protected ButtonWidget m_CancelNearestButton;
    protected ButtonWidget m_LockNearestButton;
    protected ButtonWidget m_UnlockNearestButton;
    protected ButtonWidget m_DebugButton;
    protected ButtonWidget m_CloseButton;

    override Widget Init()
    {
        layoutRoot = GetGame().GetWorkspace().CreateWidgets("deutschz_atmraidez/gui/layouts/dzatm_admin.layout");
        m_Status = TextWidget.Cast(layoutRoot.FindAnyWidget("DZATM_AdminStatusText"));
        m_Message = TextWidget.Cast(layoutRoot.FindAnyWidget("DZATM_AdminMessage"));
        m_StatusButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("DZATM_Status"));
        m_ReloadButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("DZATM_Reload"));
        m_CancelAllButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("DZATM_CancelAll"));
        m_ClearNearestButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("DZATM_ClearNearest"));
        m_CancelNearestButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("DZATM_CancelNearest"));
        m_LockNearestButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("DZATM_LockNearest"));
        m_UnlockNearestButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("DZATM_UnlockNearest"));
        m_DebugButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("DZATM_Debug"));
        m_CloseButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("DZATM_Close"));

        GetGame().GetInput().ChangeGameFocus(1);
        Send("status");
        return layoutRoot;
    }

    override void Update(float timeslice)
    {
        super.Update(timeslice);
        if (m_Status)
            m_Status.SetText("Aktive Raids: " + DZATM_AdminStatus.ActiveSessions.ToString() + "\nRegistrierte Expansion-ATMs: " + DZATM_AdminStatus.RegisteredATMs.ToString() + "\nDebug: " + DZATM_AdminStatus.DebugEnabled.ToString());
        if (m_Message)
            m_Message.SetText(DZATM_AdminStatus.LastMessage);
    }

    override bool OnClick(Widget w, int x, int y, int button)
    {
        if (w == m_StatusButton) { Send("status"); return true; }
        if (w == m_ReloadButton) { Send("reload"); return true; }
        if (w == m_CancelAllButton) { Send("cancelall"); return true; }
        if (w == m_ClearNearestButton) { Send("clearnearest"); return true; }
        if (w == m_CancelNearestButton) { Send("cancelnearest"); return true; }
        if (w == m_LockNearestButton) { Send("locknearest"); return true; }
        if (w == m_UnlockNearestButton) { Send("unlocknearest"); return true; }
        if (w == m_DebugButton)
        {
            if (DZATM_AdminStatus.DebugEnabled) Send("debug_off");
            else Send("debug_on");
            return true;
        }
        if (w == m_CloseButton)
        {
            Close();
            return true;
        }
        return super.OnClick(w, x, y, button);
    }

    protected void Send(string command)
    {
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (player)
            DZATM_ServerRPC.SendAdminCommand(player, command);
    }

    override void OnHide()
    {
        super.OnHide();
        GetGame().GetInput().ResetGameFocus();
    }
}

modded class MissionGameplay
{
    override UIScriptedMenu CreateScriptedMenu(int id)
    {
        if (id == DZATM_Const.ADMIN_MENU_ID)
            return new DZATM_AdminMenu;
        return super.CreateScriptedMenu(id);
    }

    override void OnEvent(EventType eventTypeId, Param params)
    {
        super.OnEvent(eventTypeId, params);
        if (eventTypeId != ChatMessageEventTypeID) return;

        ChatMessageEventParams chatParams = ChatMessageEventParams.Cast(params);
        if (!chatParams || chatParams.param3 != "/atmadmin") return;

        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (!player || !player.GetIdentity()) return;
        if (chatParams.param2 != "" && chatParams.param2 != player.GetIdentity().GetName()) return;

        GetGame().GetUIManager().EnterScriptedMenu(DZATM_Const.ADMIN_MENU_ID, null);
    }
}
