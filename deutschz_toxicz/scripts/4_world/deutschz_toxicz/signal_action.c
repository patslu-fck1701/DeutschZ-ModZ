class DZToxicZActionActivateSignal: ActionSingleUseBase
{
    void DZToxicZActionActivateSignal()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_ITEM_ON;
        m_FullBody = false;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_PRONE;
        m_Text = "ToxicZ-Signal aktivieren";
    }

    override void CreateConditionComponents()
    {
        m_ConditionItem = new CCINonRuined;
        m_ConditionTarget = new CCTSelf;
    }

    override bool HasTarget()
    {
        return false;
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        return player && player.IsAlive() && item && item.GetType() == "ToxicZ_Signal_Marker";
    }

    override void OnExecuteServer(ActionData action_data)
    {
        if (!action_data || !action_data.m_Player || !action_data.m_MainItem || !g_DZToxicZ) return;
        g_DZToxicZ.ActivateSignalMarker(action_data.m_Player, action_data.m_MainItem);
    }
}

modded class ToxicZ_Signal_Marker
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(DZToxicZActionActivateSignal);
    }
}

modded class ActionConstructor
{
    override void RegisterActions(TTypenameArray actions)
    {
        super.RegisterActions(actions);
        actions.Insert(DZToxicZActionActivateSignal);
    }
}
