modded class ExpansionAirdropContainerBase_Server
{
 protected bool m_DZADZ_Sealed;
 protected int m_DZADZ_SealSeconds = 30;
 void ExpansionAirdropContainerBase_Server()
 {
  m_DZADZ_Sealed = ConfigGetBool("dzadzRaven");
  RegisterNetSyncVariableBool("m_DZADZ_Sealed");
  RegisterNetSyncVariableInt("m_DZADZ_SealSeconds",5,120);
 }
 bool DZADZ_IsSealed()
 {
  return m_DZADZ_Sealed;
 }
 int DZADZ_GetSealSeconds()
 {
  return m_DZADZ_SealSeconds;
 }
 void DZADZ_SetSealSeconds(int seconds)
 {
  m_DZADZ_SealSeconds = Math.Clamp(seconds, 5, 120);
  SetSynchDirty();
 }
 void DZADZ_SetSealed(bool isSealed)
 {
  if (!GetGame().IsServer()) return;
  m_DZADZ_Sealed = isSealed; SetSynchDirty();
 }
 override bool IsInventoryVisible()
 {
  if (m_DZADZ_Sealed) return false;
  return super.IsInventoryVisible();
 }
 override bool CanReleaseCargo(EntityAI cargo)
 {
  if (m_DZADZ_Sealed) return false;
  return super.CanReleaseCargo(cargo);
 }
}

class ActionDZADZ_UnsealCB: ActionContinuousBaseCB
{
 override void CreateActionComponent()
 {
  int seconds=30;
  ExpansionAirdropContainerBase_Server container=ActionDZADZ_Unseal.Resolve(m_ActionData.m_Target);
  if(container) seconds=container.DZADZ_GetSealSeconds();
  m_ActionData.m_ActionComponent = new CAContinuousTime(seconds);
 }
}
class DZADZ_ActionData: ActionData
{
 float StartHealth;
 bool Completed;
}
class ActionDZADZ_Unseal: ActionContinuousBase
{
 void ActionDZADZ_Unseal()
 {
  m_CallbackClass = ActionDZADZ_UnsealCB;
  m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
  m_FullBody = true;
  m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
  m_Text = "Transportversiegelung aufbrechen";
 }
 override ActionData CreateActionData()
 {
  return new DZADZ_ActionData;
 }
 override typename GetInputType()
 {
  return ContinuousInteractActionInput;
 }
 override void CreateConditionComponents()
 {
  m_ConditionItem = new CCINone;
  m_ConditionTarget = new CCTCursor(4.0);
 }
 static ExpansionAirdropContainerBase_Server Resolve(ActionTarget target)
 {
  if (!target) return null;
  ExpansionAirdropContainerBase_Server container = ExpansionAirdropContainerBase_Server.Cast(target.GetObject());
  if (!container) container = ExpansionAirdropContainerBase_Server.Cast(target.GetParent());
  return container;
 }
 override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
 {
  ExpansionAirdropContainerBase_Server container = Resolve(target);
  if (!player || !player.IsAlive() || player.IsUnconscious() || !container || !container.DZADZ_IsSealed()) return false;
  if (vector.Distance(player.GetPosition(), container.GetPosition()) > 5.0) return false;
#ifdef SERVER
  return DZADZ_RavenManager.Get().CanUnseal(container);
#else
  return true;
#endif
 }
 override void OnStartServer(ActionData action_data)
 {
  super.OnStartServer(action_data);
#ifdef SERVER
  DZADZ_ActionData state = DZADZ_ActionData.Cast(action_data);
  if (state && action_data.m_Player)
  {
   state.StartHealth = action_data.m_Player.GetHealth();
   state.Completed = false;
  }
  ExpansionAirdropContainerBase_Server container = Resolve(action_data.m_Target);
  if (container) DZADZ_RavenManager.Get().BeginUnseal(action_data.m_Player, container);
#endif
 }
 override bool ActionConditionContinue(ActionData action_data)
 {
  if (!super.ActionConditionContinue(action_data)) return false;
  DZADZ_ActionData state = DZADZ_ActionData.Cast(action_data);
  if (!state || !action_data.m_Player) return false;
  if (GetGame().IsServer() && action_data.m_Player.GetHealth() < state.StartHealth) return false;
  return true;
 }
 override void OnFinishProgressServer(ActionData action_data)
 {
#ifdef SERVER
  DZADZ_ActionData state = DZADZ_ActionData.Cast(action_data);
  ExpansionAirdropContainerBase_Server container = Resolve(action_data.m_Target);
  if (state && action_data.m_Player && action_data.m_Player.GetHealth() >= state.StartHealth && container)
  {
   state.Completed = true;
   DZADZ_RavenManager.Get().CompleteUnseal(action_data.m_Player, container);
  }
#endif
 }
 override void OnEndServer(ActionData action_data)
 {
  super.OnEndServer(action_data);
#ifdef SERVER
  DZADZ_ActionData state = DZADZ_ActionData.Cast(action_data);
  ExpansionAirdropContainerBase_Server container = Resolve(action_data.m_Target);
  if (state && !state.Completed && container && container.DZADZ_IsSealed())
   DZADZ_RavenManager.Get().ReportUnsealFailure(action_data.m_Player, container);
#endif
 }
}
modded class ActionConstructor
{
 override void RegisterActions(TTypenameArray actions)
 {
  super.RegisterActions(actions);
  actions.Insert(ActionDZADZ_Unseal);
 }
}
modded class PlayerBase
{
 override void SetActions(out TInputActionMap InputActionMap)
 {
  super.SetActions(InputActionMap);
  AddAction(ActionDZADZ_Unseal, InputActionMap);
 }
}
modded class ExpansionAirdropContainerManager
{
 override void CreateServerMarker()
 {
  if (m_Container && m_Container.ConfigGetBool("dzadzRaven")) return;
  super.CreateServerMarker();
 }
}
