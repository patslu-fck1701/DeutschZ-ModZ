class DZCRZ_ClientActions
{
	static void TrySendAdminCommand(ChatMessageEventParams chatParams)
	{
		if (!chatParams || !GetGame() || GetGame().IsServer())
			return;
		string text = chatParams.param3;
		if (text == "" || text.IndexOf("/dzcrz") != 0)
			return;
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (!player || !player.GetIdentity())
			return;
		if (chatParams.param2 != "" && chatParams.param2 != player.GetIdentity().GetName())
			return;
		string command = text.Substring(6, text.Length() - 6).Trim();
		DZCRZ_ServerRPC.SendAdminCommand(player, command);
	}
}

modded class MissionGameplay
{
	override void OnEvent(EventType eventTypeId, Param params)
	{
		super.OnEvent(eventTypeId, params);
		if (eventTypeId == ChatMessageEventTypeID)
			DZCRZ_ClientActions.TrySendAdminCommand(ChatMessageEventParams.Cast(params));
	}

	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);
		DZCRZ_ClientNotifications.Get().Update();
		DZCRZ_ClientEffects.Get().Update();
	}

	override void OnMissionFinish()
	{
		DZCRZ_ClientEffects.Destroy();
		DZCRZ_ClientNotifications.Destroy();
		super.OnMissionFinish();
	}
}
