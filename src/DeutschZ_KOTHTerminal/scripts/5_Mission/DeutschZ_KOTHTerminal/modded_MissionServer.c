modded class MissionServer
{
	override void OnInit()
	{
		super.OnInit();
		DZKOTHG_ScheduleManagerInit("OnInit", 30000);
	}

	override void OnMissionStart()
	{
		super.OnMissionStart();
		DZKOTHG_ScheduleManagerInit("OnMissionStart", 5000);
	}

	protected void DZKOTHG_ScheduleManagerInit(string source, int delayMs)
	{
		if (!GetGame() || !GetGame().IsServer())
			return;

		Print("[DZKOTHG] Scheduling manager init from " + source + " in " + delayMs.ToString() + " ms.");
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.DZKOTHG_InitManager, delayMs, false);
	}

	protected void DZKOTHG_InitManager()
	{
		if (!GetGame() || !GetGame().IsServer())
			return;

		Print("[DZKOTHG] Manager init call begin.");
		DZKOTHG_Manager.GetInstance().Init();
		Print("[DZKOTHG] Manager init call end.");
	}

	override void OnMissionFinish()
	{
		super.OnMissionFinish();
	}
}
