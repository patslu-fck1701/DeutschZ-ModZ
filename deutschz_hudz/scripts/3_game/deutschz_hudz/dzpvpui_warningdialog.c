class DZPVPUI_WarningDialog : UIScriptedMenu
{
    protected string m_DZPVPUI_Header;
    protected string m_DZPVPUI_Message;
    protected int m_DZPVPUI_ErrorCode;
    protected UIScriptedMenu m_DZPVPUI_Handler;
    protected ButtonWidget m_DZPVPUI_OK;
    protected ImageWidget m_DZPVPUI_OKImage;

    void Configure(string header, string message, int errorCode, UIScriptedMenu handler)
    {
        m_DZPVPUI_Header = header;
        m_DZPVPUI_Message = message;
        m_DZPVPUI_ErrorCode = errorCode;
        m_DZPVPUI_Handler = handler;

        if (layoutRoot)
        {
            TextWidget title = TextWidget.Cast(layoutRoot.FindAnyWidget("DZPVPUI_WarningTitle"));
            TextWidget body = TextWidget.Cast(layoutRoot.FindAnyWidget("DZPVPUI_WarningText"));
            if (title)
                title.SetText(m_DZPVPUI_Header);
            if (body)
                body.SetText(m_DZPVPUI_Message);
        }
    }

    override Widget Init()
    {
        layoutRoot = GetGame().GetWorkspace().CreateWidgets("deutschz_hudz/gui/layouts/deutschz_hudz_warning_dialog.layout");
        if (!layoutRoot)
            return null;

        TextWidget title = TextWidget.Cast(layoutRoot.FindAnyWidget("DZPVPUI_WarningTitle"));
        TextWidget message = TextWidget.Cast(layoutRoot.FindAnyWidget("DZPVPUI_WarningText"));
        m_DZPVPUI_OK = ButtonWidget.Cast(layoutRoot.FindAnyWidget("DZPVPUI_WarningOK"));
        m_DZPVPUI_OKImage = ImageWidget.Cast(layoutRoot.FindAnyWidget("DZPVPUI_WarningOKImage"));

        if (title)
            title.SetText(m_DZPVPUI_Header);
        if (message)
            message.SetText(m_DZPVPUI_Message);

        if (m_DZPVPUI_OK)
            SetFocus(m_DZPVPUI_OK);

        return layoutRoot;
    }

    override bool OnClick(Widget w, int x, int y, int button)
    {
        if (w != m_DZPVPUI_OK)
            return super.OnClick(w, x, y, button);

        if (m_DZPVPUI_Handler)
            m_DZPVPUI_Handler.OnModalResult(w, x, y, m_DZPVPUI_ErrorCode, DBB_OK);

        GetGame().GetUIManager().Back();
        return true;
    }

    override bool OnMouseEnter(Widget w, int x, int y)
    {
        if (w == m_DZPVPUI_OK && m_DZPVPUI_OKImage)
            m_DZPVPUI_OKImage.LoadImageFile(0, "deutschz_hudz/gui/menu_green1/component_set/buttons/wide/button_wide_hover.paa");
        return true;
    }

    override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
    {
        if (w == m_DZPVPUI_OK && m_DZPVPUI_OKImage)
            m_DZPVPUI_OKImage.LoadImageFile(0, "deutschz_hudz/gui/menu_green1/component_set/buttons/wide/button_wide_normal.paa");
        return true;
    }
}

class DZPVPUI_SystemDialogInput
{
    static void BlockMainMenuButtons(bool blocked)
    {
        WorkspaceWidget workspace = GetGame().GetWorkspace();
        if (!workspace)
            return;
        ref array<string> names = {
            "character_rotation_frame",
            "play", "choose_server", "customize_character", "custom_button1", "custom_button2", "message_button",
            "settings_button", "exit_button", "feedback_button", "tutorial_button", "tutorials", "play_video",
            "dz_nav_home", "dz_nav_server", "dz_nav_rules", "dz_nav_support", "dz_nav_events", "dz_nav_shop",
            "dz_nav_settings", "dz_nav_profile", "dz_nav_exit", "dz_music_play", "dz_music_prev", "dz_music_next",
            "dz_music_volume_down", "dz_music_mute", "dz_music_volume_up", "dz_playlist_deutschz",
            "dz_playlist_halftanz", "dz_playlist_english"
        };
        foreach (string name : names)
        {
            Widget widget = workspace.FindAnyWidget(name);
            if (!widget)
                continue;
            if (blocked)
                widget.SetFlags(WidgetFlags.IGNOREPOINTER);
            else
                widget.ClearFlags(WidgetFlags.IGNOREPOINTER);
        }
    }
}

modded class DialogueErrorProperties
{
    protected void DZPVPUI_ApplyNativeDialogStyle()
    {
        WorkspaceWidget workspace = GetGame().GetWorkspace();
        if (!workspace)
            return;

        Widget dialog = workspace.FindAnyWidget("DialogBox");
        Widget panel = workspace.FindAnyWidget("DialogBoxPanel");
        Widget separator = workspace.FindAnyWidget("SeparatorPanel");
        TextWidget caption = TextWidget.Cast(workspace.FindAnyWidget("Caption"));
        RichTextWidget body = RichTextWidget.Cast(workspace.FindAnyWidget("Text"));
        ButtonWidget ok = ButtonWidget.Cast(workspace.FindAnyWidget("bOK"));
        ButtonWidget yes = ButtonWidget.Cast(workspace.FindAnyWidget("bYes"));
        ButtonWidget no = ButtonWidget.Cast(workspace.FindAnyWidget("bNo"));
        ButtonWidget cancel = ButtonWidget.Cast(workspace.FindAnyWidget("bCancel"));

        // Palette matches deutschz_hudz_login_time.layout.
        if (dialog) dialog.SetColor(ARGB(250, 3, 4, 5));
        if (panel) panel.SetColor(ARGB(235, 3, 4, 5));
        if (separator) separator.SetColor(ARGB(255, 105, 219, 0));

        if (caption)
        {
            caption.SetColor(ARGB(255, 245, 245, 240));
            caption.SetTextExactSize(20);
        }

        if (body)
        {
            body.SetColor(ARGB(255, 209, 214, 219));
            body.SetTextExactSize(16);
        }

        if (ok) ok.SetColor(ARGB(255, 105, 219, 0));
        if (yes) yes.SetColor(ARGB(255, 105, 219, 0));
        if (no) no.SetColor(ARGB(255, 105, 219, 0));
        if (cancel) cancel.SetColor(ARGB(255, 105, 219, 0));
    }

    override void HandleError(int errorCode, string additionalInfo = "")
    {
#ifdef NO_GUI
        return;
#endif

#ifdef SERVER
        return;
#else
        // Connection loss, kick and engine error panels keep their native
        // lifecycle. Replacing them with a scripted menu can lose the panel
        // while the engine changes from gameplay to main menu.
        super.HandleError(errorCode, additionalInfo);
        DZPVPUI_SystemDialogInput.BlockMainMenuButtons(true);
        DZPVPUI_ApplyNativeDialogStyle();
#endif
    }
}
