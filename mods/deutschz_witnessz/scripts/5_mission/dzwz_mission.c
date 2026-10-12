modded class MissionServer
{
 override void OnMissionStart()
 {
  super.OnMissionStart();
  DZWZ_Service.Start();
 }
 override void OnMissionFinish()
 {
  DZWZ_Service.Stop();
  super.OnMissionFinish();
 }
}
