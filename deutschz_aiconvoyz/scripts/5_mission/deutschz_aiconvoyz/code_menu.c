class deutschz_blackbox_code_menu: UIScriptedMenu
{
    protected EditBoxWidget m_CodeInput;

    override Widget Init()
    {
        layoutRoot = GetGame().GetWorkspace().CreateWidgets("gui/layouts/dialog_input_text.layout");
        m_CodeInput = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("PrimaryEditBox"));
        TextWidget.Cast(layoutRoot.FindAnyWidget("WindowLabel")).SetText("Blackbox entsperren");
        TextWidget.Cast(layoutRoot.FindAnyWidget("MessageText")).SetText("Vierstelligen Zugangscode eingeben");
        TextWidget.Cast(layoutRoot.FindAnyWidget("PrimaryLabel")).SetText("Zugangscode");
        layoutRoot.FindAnyWidget("SecondaryLabel").Show(false);
        Widget secondary = layoutRoot.FindAnyWidget("SecondaryEditBox");
        if (secondary) secondary.Show(false);
        return layoutRoot;
    }

    override bool UseKeyboard() { return true; }
    override bool UseMouse() { return true; }

    override void OnShow()
    {
        super.OnShow();
        SetFocus(m_CodeInput);
    }

    override bool OnClick(Widget w, int x, int y, int button)
    {
        if (w.GetName() == "ButtonCancel")
        {
            Close();
            return true;
        }
        if (w.GetName() == "ButtonOk")
        {
            string code = m_CodeInput.GetText();
            if (code.Length() != 4) return true;
            deutschz_aiconvoyz_blackbox box = deutschz_blackbox_code_context.SelectedBox;
            if (box) box.RPCSingleParam(deutschz_aiconvoyz_rpc.code_request, new Param1<string>(code), true);
            Close();
            return true;
        }
        return super.OnClick(w, x, y, button);
    }

    override void OnHide()
    {
        deutschz_blackbox_code_context.SelectedBox = null;
        super.OnHide();
    }
}

modded class MissionBase
{
    override UIScriptedMenu CreateScriptedMenu(int id)
    {
        if (id == deutschz_aiconvoyz_rpc.code_menu)
            return new deutschz_blackbox_code_menu();
        return super.CreateScriptedMenu(id);
    }
}
