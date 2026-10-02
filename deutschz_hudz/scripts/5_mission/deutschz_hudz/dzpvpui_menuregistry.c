modded class MissionBase
{
    override UIScriptedMenu CreateScriptedMenu(int id)
    {
        if (id == DZPVPUI_MENU_WARNING)
            return new DZPVPUI_WarningDialog;

        return super.CreateScriptedMenu(id);
    }
}
