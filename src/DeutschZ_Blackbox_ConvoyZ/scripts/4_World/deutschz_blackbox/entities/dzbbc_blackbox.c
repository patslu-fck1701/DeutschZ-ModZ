class DZBBC_Blackbox : SmallProtectorCase
{
	override void SetActions()
	{
		super.SetActions();
		AddAction(ActionDZBBC_StartBlackboxHack);
	}
}

class ActionDZBBC_StartBlackboxHack : ActionInteractBase
{
	void ActionDZBBC_StartBlackboxHack()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_Text = "#STR_DZBBC_ACTION_HACK_BLACKBOX";
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!player || !target)
			return false;

		Object object = target.GetObject();
		return DZBBC_Blackbox.Cast(object) != null;
	}

	override void OnExecuteServer(ActionData action_data)
	{
		if (!action_data || !action_data.m_Player || !action_data.m_Target)
			return;

		DZBBC_EventManager.GetInstance().StartBlackboxHack(action_data.m_Player, action_data.m_Target.GetObject());
	}
}
