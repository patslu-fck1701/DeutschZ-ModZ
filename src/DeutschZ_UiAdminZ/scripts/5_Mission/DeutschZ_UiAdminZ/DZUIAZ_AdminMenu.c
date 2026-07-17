class DZUIAZ_AdminMenu: UIScriptedMenu
{
	protected TextWidget m_HeaderStatus;
	protected TextWidget m_Status;
	protected TextWidget m_Details;
	protected TextWidget m_Result;
	protected TextWidget m_LicenseInfo;
	protected ButtonWidget m_Start;
	protected ButtonWidget m_Stop;
	protected ButtonWidget m_Refresh;
	protected ButtonWidget m_Close;
	protected ImageWidget m_StartArt;
	protected ImageWidget m_StopArt;
	protected bool m_BindingsReady;
	protected bool m_InputExcluded;
	protected bool m_ActionStateKnown;
	protected bool m_LastActionState;

	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("DeutschZ_UiAdminZ/gui/layouts/dzuiaz_admin_pro.layout");
		if (!layoutRoot)
		{
			Print("[DeutschZ UiAdminZ] ERROR: Admin Pro root layout could not be loaded.");
			return null;
		}

		m_HeaderStatus = TextWidget.Cast(layoutRoot.FindAnyWidget("DZUIAZ_HeaderStatus"));
		m_Status = TextWidget.Cast(layoutRoot.FindAnyWidget("DZUIAZ_Status"));
		m_Details = TextWidget.Cast(layoutRoot.FindAnyWidget("DZUIAZ_Details"));
		m_Result = TextWidget.Cast(layoutRoot.FindAnyWidget("DZUIAZ_Result"));
		m_LicenseInfo = TextWidget.Cast(layoutRoot.FindAnyWidget("DZUIAZ_LicenseInfo"));
		m_Start = ButtonWidget.Cast(layoutRoot.FindAnyWidget("DZUIAZ_Start"));
		m_Stop = ButtonWidget.Cast(layoutRoot.FindAnyWidget("DZUIAZ_Stop"));
		m_Refresh = ButtonWidget.Cast(layoutRoot.FindAnyWidget("DZUIAZ_Refresh"));
		m_Close = ButtonWidget.Cast(layoutRoot.FindAnyWidget("DZUIAZ_Close"));
		m_StartArt = ImageWidget.Cast(layoutRoot.FindAnyWidget("DZUIAZ_StartArt"));
		m_StopArt = ImageWidget.Cast(layoutRoot.FindAnyWidget("DZUIAZ_StopArt"));
		m_BindingsReady = m_HeaderStatus && m_Status && m_Details && m_Result && m_LicenseInfo && m_Start && m_Stop && m_Refresh && m_Close && m_StartArt && m_StopArt;
		if (!m_BindingsReady)
		{
			Print("[DeutschZ UiAdminZ] ERROR: Required Admin Pro widget binding is missing.");
			return layoutRoot;
		}

		Print("[DeutschZ UiAdminZ] Admin Pro layout A-F and widget bindings loaded.");
		RequestEvent(DZUIAZ_AdminAction.STATUS);
		return layoutRoot;
	}

	override void OnShow()
	{
		super.OnShow();
		Mission mission = GetGame().GetMission();
		if (mission && !m_InputExcluded)
		{
			mission.AddActiveInputExcludes({"menu"});
			m_InputExcluded = true;
		}
	}

	override void OnHide()
	{
		RestoreInput();
		super.OnHide();
	}

	override void Cleanup()
	{
		RestoreInput();
		super.Cleanup();
	}

	override void Update(float timeslice)
	{
		super.Update(timeslice);
		if (GetUApi().GetInputByID(UAUIBack).LocalPress())
		{
			Close();
			return;
		}
		if (!m_BindingsReady)
			return;

		string moduleState = "EVENTMOD NICHT GELADEN";
		if (DZUIAZ_ClientState.ModuleAvailable)
			moduleState = "KOTHZ FREE: " + DZUIAZ_ClientState.EventState;
		m_HeaderStatus.SetText("SERVER ONLINE | " + moduleState + " | ADMIN " + DZUIAZ_ClientState.AdminName);
		m_Status.SetText(moduleState);
		if (DZUIAZ_ClientState.ModuleAvailable)
			m_Details.SetText("Position: " + DZUIAZ_ClientState.EventPosition.ToString() + "\nRadius: " + DZUIAZ_ClientState.Radius.ToString() + " m\nCapture: " + Math.Round(DZUIAZ_ClientState.Progress * 100).ToString() + " %\nGegner: " + DZUIAZ_ClientState.EnemyCount.ToString() + "\nReward: " + DZUIAZ_ClientState.RewardReady.ToString());
		else
			m_Details.SetText("KEINE DATEN\nUiAdminZ bleibt ohne Eventmod lauffaehig.");
		m_Result.SetText(DZUIAZ_ClientState.Result);

		string devState = "AUS";
		if (DZUIAZ_ClientState.ProDevelopmentEnabled)
			devState = "AN (LOKALE ENTWICKLUNG)";
		m_LicenseInfo.SetText("FREE AKTIV\nStatus, Start, Stop und Diagnose\n\n" + DZUIAZ_ClientState.LicenseState + "\nKeine Lizenzdaten vorhanden\n\nDev-Flag: " + devState);

		bool eventActionsEnabled = DZUIAZ_ClientState.Authorized && DZUIAZ_ClientState.ModuleAvailable;
		m_Start.Enable(eventActionsEnabled);
		m_Stop.Enable(eventActionsEnabled);
		if (!m_ActionStateKnown || eventActionsEnabled != m_LastActionState)
		{
			if (eventActionsEnabled)
			{
				m_StartArt.LoadImageFile(0, "DeutschZ_UiAdminZ/gui/admin_pack/buttons/event_starten_normal.paa");
				m_StopArt.LoadImageFile(0, "DeutschZ_UiAdminZ/gui/admin_pack/buttons/event_stoppen_normal.paa");
			}
			else
			{
				m_StartArt.LoadImageFile(0, "DeutschZ_UiAdminZ/gui/admin_pack/buttons/event_starten_disabled.paa");
				m_StopArt.LoadImageFile(0, "DeutschZ_UiAdminZ/gui/admin_pack/buttons/event_stoppen_disabled.paa");
			}
			m_StartArt.SetImage(0);
			m_StopArt.SetImage(0);
			m_LastActionState = eventActionsEnabled;
			m_ActionStateKnown = true;
		}
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (w == m_Start)
		{
			RequestEvent(DZUIAZ_AdminAction.START);
			return true;
		}
		if (w == m_Stop)
		{
			RequestEvent(DZUIAZ_AdminAction.STOP);
			return true;
		}
		if (w == m_Refresh)
		{
			RequestEvent(DZUIAZ_AdminAction.STATUS);
			return true;
		}
		if (w == m_Close)
		{
			Close();
			return true;
		}
		return super.OnClick(w, x, y, button);
	}

	override bool OnKeyPress(Widget w, int x, int y, int key)
	{
		if (key == KeyCode.KC_F7)
		{
			Close();
			return true;
		}
		return super.OnKeyPress(w, x, y, key);
	}

	protected void RequestEvent(int action)
	{
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (!player)
			return;
		ScriptRPC rpc = new ScriptRPC;
		rpc.Write(DZUIAZ_Constants.EVENT_RPC_PROTOCOL_VERSION);
		rpc.Write(action);
		rpc.Send(player, DZUIAZ_Constants.RPC_EVENT_ADMIN, true, null);
	}

	protected void RestoreInput()
	{
		Mission mission = GetGame().GetMission();
		if (mission && m_InputExcluded)
			mission.RemoveActiveInputExcludes({"menu"}, true);
		m_InputExcluded = false;
	}
}
