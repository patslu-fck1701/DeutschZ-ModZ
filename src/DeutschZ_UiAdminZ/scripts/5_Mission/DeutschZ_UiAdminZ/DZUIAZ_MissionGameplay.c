modded class MissionGameplay
{
	protected int m_DZUIAZ_LastDeniedNotice;

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

		if (DZUIAZ_ClientState.PendingOpen)
			return;

		DZUIAZ_ClientState.Reset();
		DZUIAZ_ClientState.PendingOpen = true;
		DZUIAZ_ClientState.AuthorizationRequestedAt = GetGame().GetTime();
		DZUIAZ_RequestAuthorization();
	}

	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);
		if (!DZUIAZ_ClientState.PendingOpen)
			return;

		if (DZUIAZ_ClientState.AuthorizationResponseReceived)
		{
			DZUIAZ_ClientState.PendingOpen = false;
			if (DZUIAZ_ClientState.Authorized)
				GetGame().GetUIManager().EnterScriptedMenu(DZUIAZ_Constants.MENU_ID, null);
			else
				DZUIAZ_ShowDenied();
		}
		else if (GetGame().GetTime() - DZUIAZ_ClientState.AuthorizationRequestedAt > 5000)
		{
			DZUIAZ_ClientState.PendingOpen = false;
			DZUIAZ_ShowDenied();
		}
	}

	protected void DZUIAZ_RequestAuthorization()
	{
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (!player) return;
		ScriptRPC rpc = new ScriptRPC;
		rpc.Write(DZUIAZ_Constants.UI_RPC_PROTOCOL_VERSION);
		rpc.Send(player, DZUIAZ_Constants.RPC_UIADMIN, true, null);
	}

	protected void DZUIAZ_ShowDenied()
	{
		int now = GetGame().GetTime();
		if (now - m_DZUIAZ_LastDeniedNotice < 5000)
			return;
		m_DZUIAZ_LastDeniedNotice = now;
		NotificationSystem.AddNotificationExtended(4.0, "DeutschZ UiAdminZ", "Keine Berechtigung fuer DeutschZ UiAdminZ.", "");
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
