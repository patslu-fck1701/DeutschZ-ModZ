modded class MissionGameplay
{
	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);
		DZKOTHF_ProgressRenderer.Get().Update();
	}

	override void OnMissionFinish()
	{
		DZKOTHF_ClientState.Reset();
		DZKOTHF_ProgressRenderer.Get().Cleanup();
		super.OnMissionFinish();
	}
}
