modded class MissionServer
{
 override void OnMissionStart()
 {
  super.OnMissionStart();
  DZWM_Service.Start();
 }
 override void OnMissionFinish()
 {
  DZWM_Service.Stop();
  super.OnMissionFinish();
 }
}
