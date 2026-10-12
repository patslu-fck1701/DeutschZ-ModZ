modded class MissionServer
{
 override void OnMissionStart()
 {
  super.OnMissionStart();
  DZCR_Service.Start();
 }
 override void OnMissionFinish()
 {
  DZCR_Service.Stop();
  super.OnMissionFinish();
 }
}
