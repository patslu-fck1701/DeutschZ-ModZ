modded class MissionServer
{
 override void OnMissionStart()
 {
  super.OnMissionStart();
  DZAC_Runtime.Start();
 }
 override void OnMissionFinish()
 {
  DZAC_Runtime.s_Settings = null;
  super.OnMissionFinish();
 }
}
