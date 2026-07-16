modded class PlayerBase
{
	protected int m_DZKOTHF_LastAdminRequestTime;

	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);
		if (rpc_type == DZKOTHF_Constants.RPC_ADMIN && GetGame().IsServer())
		{
			HandleDZKOTHFAdminRPC(sender, ctx);
			return;
		}

		if (rpc_type != DZKOTHF_Constants.RPC_SYNC || GetGame().IsServer())
			return;

		int protocolVersion;
		int messageType;
		if (!ctx.Read(protocolVersion) || protocolVersion != DZKOTHF_Constants.RPC_PROTOCOL_VERSION || !ctx.Read(messageType))
			return;

		if (messageType == DZKOTHF_RPCMessage.NOTIFY)
		{
			string title;
			string message;
			if (ctx.Read(title) && ctx.Read(message))
				NotificationSystem.AddNotificationExtended(8.0, title, message, "");
		}
		else if (messageType == DZKOTHF_RPCMessage.MARKER)
		{
			bool visible;
			vector position;
			string text;
			string icon;
			if (ctx.Read(visible) && ctx.Read(position) && ctx.Read(text) && ctx.Read(icon))
				DZKOTHF_ClientState.SetMarker(visible, position, text, icon);
		}
		else if (messageType == DZKOTHF_RPCMessage.PROGRESS)
		{
			bool progressVisible;
			string eventName;
			string status;
			float progress;
			if (ctx.Read(progressVisible) && ctx.Read(eventName) && ctx.Read(status) && ctx.Read(progress))
				DZKOTHF_ClientState.SetProgress(progressVisible, eventName, status, progress);
		}
	}

	protected void HandleDZKOTHFAdminRPC(PlayerIdentity sender, ParamsReadContext ctx)
	{
		if (!sender || !GetGame())
			return;

		int now = GetGame().GetTime();
		if (now - m_DZKOTHF_LastAdminRequestTime < 500)
			return;
		m_DZKOTHF_LastAdminRequestTime = now;

		int protocolVersion;
		int action;
		if (!ctx.Read(protocolVersion) || protocolVersion != DZKOTHF_Constants.RPC_PROTOCOL_VERSION || !ctx.Read(action))
			return;

		DZKOTHF_EventController controller = DZKOTHF_EventController.GetInstance();
		if (!controller)
		{
			DZKOTHF_Log.Warning("AdminRPC rejected because the event controller is not initialized.");
			return;
		}
		DZKOTHF_Settings settings = controller.GetSettings();
		bool authorized = settings && settings.IsAdmin(sender.GetPlainId());
		string result = "Status aktualisiert";

		if (action == DZKOTHF_AdminAction.START)
		{
			if (!authorized)
				result = "Start verweigert: keine Serverberechtigung";
			else if (controller.StartEvent())
				result = "Event serverseitig gestartet";
			else
				result = "Eventstart vom Controller abgelehnt";
		}
		else if (action == DZKOTHF_AdminAction.STOP)
		{
			if (!authorized)
				result = "Stop verweigert: keine Serverberechtigung";
			else if (controller.AbortEvent("Admin Lite: " + sender.GetName()))
				result = "Kontrollierter Stop eingeleitet";
			else
				result = "Stop vom Controller abgelehnt";
		}
		else if (action != DZKOTHF_AdminAction.STATUS)
			return;

		DZKOTHF_Log.Info("AdminRPC steamId=" + sender.GetPlainId() + " action=" + action.ToString() + " authorized=" + authorized.ToString() + " result=" + result + ".");
		ScriptRPC response = new ScriptRPC;
		response.Write(DZKOTHF_Constants.RPC_PROTOCOL_VERSION);
		response.Write(authorized);
		response.Write(true);
		response.Write(controller.GetStateName());
		response.Write(controller.GetCenter());
		response.Write(controller.GetCaptureRadius());
		response.Write(controller.GetCaptureProgress());
		response.Write(controller.GetEnemyCount());
		response.Write(controller.HasRewardCrate());
		response.Write(result);
		response.Send(this, DZKOTHF_Constants.RPC_ADMIN, true, sender);
	}
}
