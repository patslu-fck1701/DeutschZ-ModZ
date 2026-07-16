modded class PlayerBase
{
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);
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
}
