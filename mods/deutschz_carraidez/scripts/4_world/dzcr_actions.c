class ActionDZCRInvestigate: ActionInteractBase
{
 void ActionDZCRInvestigate()
 {
  m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
  m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
  m_Text = "Fahrzeug-Raid untersuchen / sichern";
 }
 override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
 {
  if (!player || !target) return false;
  CarScript car = CarScript.Cast(target.GetObject());
  return car && (car.DZCR_Stage() == 1 || car.DZCR_Stage() == 3);
 }
 override void OnExecuteServer(ActionData action_data)
 {
  if (!DZCR_Service.Instance || !action_data || !action_data.m_Target) return;
  DZCR_Service.Instance.Use(action_data.m_Player, CarScript.Cast(action_data.m_Target.GetObject()));
 }
}
modded class ActionConstructor
{
 override void RegisterActions(TTypenameArray actions)
 {
  super.RegisterActions(actions);
  actions.Insert(ActionDZCRInvestigate);
 }
}
modded class CarScript
{
 protected int m_DZCR_Stage;
 void CarScript()
 {
  RegisterNetSyncVariableInt("m_DZCR_Stage", 0, 4);
 }
 int DZCR_Stage() {return m_DZCR_Stage;}
 void DZCR_SetStage(int stage)
 {
  if (!GetGame().IsServer()) return;
  m_DZCR_Stage = stage;
  SetSynchDirty();
 }
 override void SetActions()
 {
  super.SetActions();
  AddAction(ActionDZCRInvestigate);
 }
}
