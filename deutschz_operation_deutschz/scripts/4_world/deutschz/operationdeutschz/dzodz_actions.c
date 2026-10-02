class ActionDZODZCreateMasterReader : ActionInteractBase
{
    void ActionDZODZCreateMasterReader()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
        m_Text = "Operation-DeutschZ-Freigabe uebertragen";
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (!player || !target || !DeutschZ_OperationDeutschZ_Terminal.Cast(target.GetObject()))
            return false;
        return DZODZ_PlayerItems.Find(player, "DeutschZ_BattlegroundZ_RegisteredCardReader") && DZODZ_PlayerItems.Find(player, "DeutschZ_BattlegroundZ_OperationKeyCard");
    }

    override void OnExecuteServer(ActionData action_data)
    {
        if (action_data && action_data.m_Player && action_data.m_Target && DeutschZ_OperationDeutschZ_Terminal.Cast(action_data.m_Target.GetObject()))
            DZODZ_Manager.GetInstance().CreateMasterReader(action_data.m_Player);
    }
}

modded class ActionConstructor
{
    override void RegisterActions(TTypenameArray actions)
    {
        super.RegisterActions(actions);
        actions.Insert(ActionDZODZCreateMasterReader);
    }
}

class DeutschZ_OperationDeutschZ_Terminal : BuildingBase
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionDZODZCreateMasterReader);
    }
}
