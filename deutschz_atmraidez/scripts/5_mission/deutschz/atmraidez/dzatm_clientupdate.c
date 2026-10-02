modded class MissionGameplay
{
    override void OnUpdate(float timeslice)
    {
        super.OnUpdate(timeslice);
        DZATM_ClientNotifications.Get().Update();
        DZATM_ClientEffects.Get().Update();
    }

    override void OnMissionFinish()
    {
        DZATM_ClientEffects.Destroy();
        DZATM_ClientNotifications.Destroy();
        super.OnMissionFinish();
    }
}
