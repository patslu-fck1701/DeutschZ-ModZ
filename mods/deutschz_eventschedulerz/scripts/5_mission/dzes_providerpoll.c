modded class MissionServer
{
 override void OnMissionStart()
 {
  super.OnMissionStart();
  if (!GetGame() || !GetGame().IsServer()) return;
  GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(DZES_MissionProviderPoll);
  GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DZES_MissionProviderPoll,10000,true);
  Print("[EventSchedulerZ] KOTH_MISSION_POLL_STARTED interval=10s");
 }
 protected void DZES_MissionProviderPoll()
 {
  DZKOTH_EventManager.GetInstance().StartEvent();
 }
 override void OnMissionFinish()
 {
  if (GetGame()) GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(DZES_MissionProviderPoll);
  super.OnMissionFinish();
 }
}
