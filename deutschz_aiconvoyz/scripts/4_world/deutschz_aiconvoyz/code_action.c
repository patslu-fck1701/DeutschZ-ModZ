class deutschz_blackbox_code_context
{
    static deutschz_aiconvoyz_blackbox SelectedBox;
}

class deutschz_action_blackbox_code: ActionInteractBase
{
    void deutschz_action_blackbox_code()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
        m_Text = "Blackbox-Code eingeben";
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        deutschz_aiconvoyz_blackbox box = deutschz_aiconvoyz_blackbox_target.Resolve(target);
        return player && player.IsAlive() && box && !box.IsHacked();
    }

    override void OnExecuteClient(ActionData action_data)
    {
        if (!action_data || GetGame().GetUIManager().GetMenu()) return;
        deutschz_blackbox_code_context.SelectedBox = deutschz_aiconvoyz_blackbox_target.Resolve(action_data.m_Target);
        if (deutschz_blackbox_code_context.SelectedBox)
            GetGame().GetUIManager().EnterScriptedMenu(deutschz_aiconvoyz_rpc.code_menu, null);
    }
}
