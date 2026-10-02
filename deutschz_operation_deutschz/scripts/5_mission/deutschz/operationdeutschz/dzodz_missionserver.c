modded class MissionServer
{
    override void OnMissionStart()
    {
        super.OnMissionStart();
        DZODZ_Manager.GetInstance().Initialize();
    }
}
