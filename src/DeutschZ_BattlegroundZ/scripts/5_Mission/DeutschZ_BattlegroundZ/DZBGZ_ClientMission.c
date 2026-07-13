class DZBGZ_ClientAdminCommands
{
	static void TrySendAdminCommand(ChatMessageEventParams chatParams)
	{
		if (!chatParams || !GetGame() || GetGame().IsServer())
			return;

		string text = chatParams.param3;
		if (text == "" || text.IndexOf(DZBGZ_ADMIN_COMMAND_PREFIX) != 0)
			return;

		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (!player || !player.GetIdentity())
			return;

		if (chatParams.param2 != "" && chatParams.param2 != player.GetIdentity().GetName())
			return;

		ScriptRPC rpc = new ScriptRPC;
		rpc.Write(text);
		rpc.Send(player, DZBGZ_RPC_ADMIN_COMMAND, true, player.GetIdentity());
	}
}

class DZBGZ_MapMenuRenderer
{
	static void RenderVanilla(MapWidget mapWidget)
	{
		if (!mapWidget)
			return;

		array<ref DZBGZ_MarkerData> markers = DZBGZ_ClientMarkerStore.Get().GetMarkers();
		foreach (DZBGZ_MarkerData marker: markers)
		{
			if (!marker)
				continue;

			string iconPath = marker.IconPath;
			if (iconPath == "")
				iconPath = DZBGZ_DEFAULT_MARKER_ICON;

			mapWidget.AddUserMark(marker.Pos, marker.Text, marker.Color, iconPath);
		}
	}
}

modded class MissionGameplay
{
	override void OnEvent(EventType eventTypeId, Param params)
	{
		super.OnEvent(eventTypeId, params);

		if (eventTypeId != ChatMessageEventTypeID)
			return;

		ChatMessageEventParams chatParams = ChatMessageEventParams.Cast(params);
		DZBGZ_ClientAdminCommands.TrySendAdminCommand(chatParams);
	}

	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);
		DZBGZ_ClientFX.Update();
	}
}

modded class MapMenu
{
	override void OnShow()
	{
		super.OnShow();
		DZBGZ_MapMenuRenderer.RenderVanilla(m_MapWidgetInstance);
	}
}
