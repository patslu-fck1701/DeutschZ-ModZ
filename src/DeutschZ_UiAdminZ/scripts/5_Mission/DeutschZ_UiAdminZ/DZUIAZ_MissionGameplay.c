modded class MissionGameplay
{
	override UIScriptedMenu CreateScriptedMenu(int id)
	{
		if (id == DZUIAZ_Constants.MENU_ID) return new DZUIAZ_AdminMenu;
		return super.CreateScriptedMenu(id);
	}

	override void OnKeyPress(int key)
	{
		super.OnKeyPress(key);
		if (key != KeyCode.KC_F7)
			return;

		UIScriptedMenu currentMenu = GetGame().GetUIManager().GetMenu();
		if (currentMenu)
		{
			if (currentMenu.GetID() == DZUIAZ_Constants.MENU_ID)
				currentMenu.Close();
			return;
		}

		DZUIAZ_ClientState.Reset();
		GetGame().GetUIManager().EnterScriptedMenu(DZUIAZ_Constants.MENU_ID, null);
	}

	override void OnMissionFinish()
	{
		UIScriptedMenu currentMenu = GetGame().GetUIManager().GetMenu();
		if (currentMenu && currentMenu.GetID() == DZUIAZ_Constants.MENU_ID)
			currentMenu.Close();
		DZUIAZ_ClientState.Reset();
		super.OnMissionFinish();
	}
}
