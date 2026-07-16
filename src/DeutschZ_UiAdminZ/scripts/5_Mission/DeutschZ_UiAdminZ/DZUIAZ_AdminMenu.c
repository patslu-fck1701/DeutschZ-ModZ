class DZUIAZ_AdminMenu: UIScriptedMenu
{
	protected TextWidget m_Status;
	protected TextWidget m_Details;
	protected TextWidget m_Result;
	protected ButtonWidget m_Start;
	protected ButtonWidget m_Stop;
	protected ButtonWidget m_Refresh;
	protected ButtonWidget m_Close;
	protected bool m_BindingsReady;
	protected bool m_InputExcluded;

	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("DeutschZ_UiAdminZ/gui/layouts/dzuiaz_admin_lite.layout");
		if (!layoutRoot)
		{
			Print("[DeutschZ UiAdminZ] ERROR: Root layout could not be loaded.");
			return null;
		}

		m_Status = TextWidget.Cast(layoutRoot.FindAnyWidget("DZUIAZ_Status"));
		m_Details = TextWidget.Cast(layoutRoot.FindAnyWidget("DZUIAZ_Details"));
		m_Result = TextWidget.Cast(layoutRoot.FindAnyWidget("DZUIAZ_Result"));
		m_Start = ButtonWidget.Cast(layoutRoot.FindAnyWidget("DZUIAZ_Start"));
		m_Stop = ButtonWidget.Cast(layoutRoot.FindAnyWidget("DZUIAZ_Stop"));
		m_Refresh = ButtonWidget.Cast(layoutRoot.FindAnyWidget("DZUIAZ_Refresh"));
		m_Close = ButtonWidget.Cast(layoutRoot.FindAnyWidget("DZUIAZ_Close"));
		m_BindingsReady = m_Status && m_Details && m_Result && m_Start && m_Stop && m_Refresh && m_Close;
		if (!m_BindingsReady)
			Print("[DeutschZ UiAdminZ] ERROR: Required Admin Lite widgets are missing from the loaded layout.");
		else
			Print("[DeutschZ UiAdminZ] Admin Lite layout and widget bindings loaded.");

		Request(DZUIAZ_AdminAction.STATUS);
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

		string access = "NUR ANZEIGE / NICHT BERECHTIGT";
		if (DZUIAZ_ClientState.Authorized)
			access = "ADMIN BESTAETIGT";
		m_Status.SetText("KOTHZ FREE: " + DZUIAZ_ClientState.EventState + " | " + access);
		m_Details.SetText("Position: " + DZUIAZ_ClientState.EventPosition.ToString() + "\nRadius: " + DZUIAZ_ClientState.Radius.ToString() + " m\nCapture: " + Math.Round(DZUIAZ_ClientState.Progress * 100).ToString() + " %\nGegner: " + DZUIAZ_ClientState.EnemyCount.ToString() + "\nReward: " + DZUIAZ_ClientState.RewardReady.ToString());
		m_Result.SetText(DZUIAZ_ClientState.Result);
		m_Start.Enable(DZUIAZ_ClientState.Authorized);
		m_Stop.Enable(DZUIAZ_ClientState.Authorized);
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (w == m_Start) {Request(DZUIAZ_AdminAction.START); return true;}
		if (w == m_Stop) {Request(DZUIAZ_AdminAction.STOP); return true;}
		if (w == m_Refresh) {Request(DZUIAZ_AdminAction.STATUS); return true;}
		if (w == m_Close) {Close(); return true;}
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

	protected void Request(int action)
	{
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (!player) return;
		ScriptRPC rpc = new ScriptRPC;
		rpc.Write(DZUIAZ_Constants.RPC_PROTOCOL_VERSION);
		rpc.Write(action);
		rpc.Send(player, DZUIAZ_Constants.RPC_ADMIN, true, null);
	}

	protected void RestoreInput()
	{
		Mission mission = GetGame().GetMission();
		if (mission && m_InputExcluded)
			mission.RemoveActiveInputExcludes({"menu"}, true);
		m_InputExcluded = false;
	}
}
