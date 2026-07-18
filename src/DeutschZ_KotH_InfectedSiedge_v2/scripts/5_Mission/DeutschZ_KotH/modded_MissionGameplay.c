modded class MissionGameplay
{
	override void OnEvent(EventType eventTypeId, Param params)
	{
		super.OnEvent(eventTypeId, params);

		if (eventTypeId != ChatMessageEventTypeID)
			return;

		ChatMessageEventParams chatParams = ChatMessageEventParams.Cast(params);
		DZKOTH_ClientAdminCommand.TrySendFromChat(chatParams);
	}

	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);
		DZEV_ClientProgressRenderer.Get().Update(timeslice);
	}

	override void OnMissionFinish()
	{
		DZEV_ClientProgressHud.Get().StopMusic();
		DZEV_ClientProgressHud.Get().StopSiren();
		super.OnMissionFinish();
	}
}
