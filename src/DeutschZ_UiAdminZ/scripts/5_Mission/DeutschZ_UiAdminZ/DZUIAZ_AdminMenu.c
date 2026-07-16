class DZUIAZ_AdminMenu: UIScriptedMenu
{
	protected TextWidget m_Status;
	protected TextWidget m_Details;
	protected TextWidget m_Result;
	protected ButtonWidget m_Start;
	protected ButtonWidget m_Stop;

	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("DeutschZ_UiAdminZ/gui/layouts/dzuiaz_admin_lite.layout");
		m_Status = TextWidget.Cast(layoutRoot.FindAnyWidget("DZUIAZ_Status"));
		m_Details = TextWidget.Cast(layoutRoot.FindAnyWidget("DZUIAZ_Details"));
		m_Result = TextWidget.Cast(layoutRoot.FindAnyWidget("DZUIAZ_Result"));
		m_Start = ButtonWidget.Cast(layoutRoot.FindAnyWidget("DZUIAZ_Start"));
		m_Stop = ButtonWidget.Cast(layoutRoot.FindAnyWidget("DZUIAZ_Stop"));
		Request(DZUIAZ_AdminAction.STATUS);
		return layoutRoot;
	}

	override void Update(float timeslice)
	{
		super.Update(timeslice);
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
		if (w.GetName() == "DZUIAZ_Refresh") {Request(DZUIAZ_AdminAction.STATUS); return true;}
		if (w.GetName() == "DZUIAZ_Close") {GetGame().GetUIManager().Back(); return true;}
		return super.OnClick(w, x, y, button);
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
}
