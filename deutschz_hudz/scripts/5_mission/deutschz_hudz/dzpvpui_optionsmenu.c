modded class OptionsMenu
{
    static const int DZPVPUI_OPTIONS_GREEN = ARGB(255, 61, 255, 46);
    static const int DZPVPUI_OPTIONS_GREEN_MUTED = ARGB(255, 104, 220, 0);
    static const int DZPVPUI_OPTIONS_DARK = ARGB(235, 5, 12, 8);
    static const int DZPVPUI_OPTIONS_DARK_SOLID = ARGB(255, 3, 8, 5);
    static const int DZPVPUI_OPTIONS_TEXT = ARGB(255, 238, 244, 236);
    static const string DZPVPUI_OPTIONS_LAYOUT = "deutschz_hudz/gui/layouts/deutschz_hudz_options_branding.layout";

    protected Widget m_DZPVPUI_OptionsBranding;

    override Widget Init()
    {
        Widget root = super.Init();
        if (!root)
            return root;

        m_DZPVPUI_OptionsBranding = GetGame().GetWorkspace().CreateWidgets(DZPVPUI_OPTIONS_LAYOUT, root);
        if (!m_DZPVPUI_OptionsBranding)
            Print("[DZPVPUI][OPTIONS][ERROR] Branding layout missing: " + DZPVPUI_OPTIONS_LAYOUT);

        DZPVPUI_ApplyOptionsStyle(root);
        DZPVPUI_ApplyTabStyle(m_ActiveTabIdx);
        Print("[DZPVPUI][OPTIONS] DeutschZ style ready");
        return root;
    }

    override void OnShow()
    {
        super.OnShow();
        DZPVPUI_ApplyOptionsStyle(layoutRoot);
        DZPVPUI_ApplyTabStyle(m_ActiveTabIdx);
    }

    override void OnTabSwitch(int tab)
    {
        super.OnTabSwitch(tab);
        DZPVPUI_ApplyTabStyle(tab);
    }

    protected void DZPVPUI_ApplyOptionsStyle(Widget root)
    {
        if (!root)
            return;

        DZPVPUI_SetWidgetColor(root, "Tab_Control_Container", DZPVPUI_OPTIONS_DARK);
        DZPVPUI_SetWidgetColor(root, "settings_details", DZPVPUI_OPTIONS_DARK);
        DZPVPUI_SetWidgetColor(root, "settings_details_header", DZPVPUI_OPTIONS_DARK_SOLID);
        DZPVPUI_SetWidgetColor(root, "settings_details_body_in", DZPVPUI_OPTIONS_DARK);
        DZPVPUI_SetWidgetColor(root, "play_panel_root", DZPVPUI_OPTIONS_DARK);
        DZPVPUI_SetWidgetColor(root, "bottom", DZPVPUI_OPTIONS_DARK);
        DZPVPUI_SetWidgetColor(root, "top", DZPVPUI_OPTIONS_DARK);
        DZPVPUI_SetWidgetColor(root, "separator_red", DZPVPUI_OPTIONS_GREEN);

        TextWidget title = TextWidget.Cast(root.FindAnyWidget("SettingsTextWidget"));
        if (title)
        {
            title.Show(false);
        }

        TextWidget version = TextWidget.Cast(root.FindAnyWidget("version"));
        if (version)
            version.SetColor(DZPVPUI_OPTIONS_GREEN_MUTED);

        DZPVPUI_StyleActionButton(root, "apply");
        DZPVPUI_StyleActionButton(root, "back");
        DZPVPUI_StyleActionButton(root, "reset");
        DZPVPUI_StyleActionButton(root, "defaults");

        Widget apply = root.FindAnyWidget("apply");
        Widget back = root.FindAnyWidget("back");
        if (apply)
            DZPVPUI_ColorNamedChildren(apply, DZPVPUI_OPTIONS_GREEN);
        if (back)
            DZPVPUI_ColorNamedChildren(back, DZPVPUI_OPTIONS_TEXT);
    }

    protected void DZPVPUI_StyleTree(Widget parent)
    {
        Widget child = parent.GetChildren();
        while (child)
        {
            string name = child.GetName();
            if (name.Contains("_header"))
            {
                child.SetColor(DZPVPUI_OPTIONS_DARK_SOLID);
                ButtonWidget headerButton = ButtonWidget.Cast(child);
                if (headerButton)
                    headerButton.SetTextColor(DZPVPUI_OPTIONS_GREEN);
                DZPVPUI_ColorDirectTextChildren(child, DZPVPUI_OPTIONS_GREEN);
            }

            TextWidget text = TextWidget.Cast(child);
            if (text && name != "SettingsTextWidget" && name != "version")
            {
                int textColor = DZPVPUI_OPTIONS_TEXT;
                Widget textParent = child.GetParent();
                if (textParent && textParent.GetName().Contains("_header"))
                    textColor = DZPVPUI_OPTIONS_GREEN;
                text.SetColor(textColor);
            }

            ButtonWidget normalButton = ButtonWidget.Cast(child);
            if (normalButton && !name.Contains("_header"))
                normalButton.SetTextColor(DZPVPUI_OPTIONS_TEXT);

            RichTextWidget richText = RichTextWidget.Cast(child);
            if (richText)
            {
                int richTextColor = DZPVPUI_OPTIONS_TEXT;
                Widget richTextParent = child.GetParent();
                if (richTextParent && richTextParent.GetName().Contains("_header"))
                    richTextColor = DZPVPUI_OPTIONS_GREEN;
                richText.SetColor(richTextColor);
            }

            DZPVPUI_StyleTree(child);
            child = child.GetSibling();
        }
    }

    protected void DZPVPUI_ColorDirectTextChildren(Widget parent, int color)
    {
        Widget child = parent.GetChildren();
        while (child)
        {
            TextWidget text = TextWidget.Cast(child);
            if (text)
                text.SetColor(color);
            RichTextWidget richText = RichTextWidget.Cast(child);
            if (richText)
                richText.SetColor(color);
            child = child.GetSibling();
        }
    }

    protected void DZPVPUI_ApplyTabStyle(int selected)
    {
        if (!layoutRoot)
            return;

        for (int i = 0; i < 12; i++)
        {
            Widget tab = layoutRoot.FindAnyWidget("Tab_Control_" + i.ToString());
            if (!tab)
                continue;

            TextWidget title = TextWidget.Cast(tab.FindAnyWidget("Tab_Control_" + i.ToString() + "_Title"));
            if (i == selected)
            {
                tab.SetColor(DZPVPUI_OPTIONS_DARK_SOLID);
                if (title)
                    title.SetColor(DZPVPUI_OPTIONS_GREEN);
            }
            else
            {
                tab.SetColor(ARGB(190, 5, 12, 8));
                if (title)
                    title.SetColor(DZPVPUI_OPTIONS_TEXT);
            }
        }
    }

    protected void DZPVPUI_StyleActionButton(Widget root, string name)
    {
        ButtonWidget button = ButtonWidget.Cast(root.FindAnyWidget(name));
        if (!button)
            return;
        button.SetColor(DZPVPUI_OPTIONS_DARK);
        button.SetTextColor(DZPVPUI_OPTIONS_TEXT);
    }

    protected void DZPVPUI_SetWidgetColor(Widget root, string name, int color)
    {
        Widget widget = root.FindAnyWidget(name);
        if (widget)
            widget.SetColor(color);
    }

    protected void DZPVPUI_ColorNamedChildren(Widget w, int color)
    {
        TextWidget text1 = TextWidget.Cast(w.FindAnyWidget(w.GetName() + "_text"));
        TextWidget text2 = TextWidget.Cast(w.FindAnyWidget(w.GetName() + "_label"));
        TextWidget text3 = TextWidget.Cast(w.FindAnyWidget(w.GetName() + "_text_1"));
        ImageWidget image = ImageWidget.Cast(w.FindAnyWidget(w.GetName() + "_image"));
        Widget option = w.FindAnyWidget(w.GetName() + "_option_wrapper");
        Widget optionLabel = w.FindAnyWidget("option_label");

        if (text1)
            text1.SetColor(color);
        if (text2)
            text2.SetColor(color);
        if (text3)
            text3.SetColor(color);
        if (image)
            image.SetColor(color);
        if (option)
            option.SetColor(color);
        if (optionLabel)
            optionLabel.SetColor(color);
    }
}
