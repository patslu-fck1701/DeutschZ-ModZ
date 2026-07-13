class DZBGZ_CardReader : House
{
	override bool IsBuilding()
	{
		return false;
	}

	override void SetActions()
	{
		super.SetActions();
		AddAction(ActionDZBGZ_UseCardReader);
	}
}

class ActionDZBGZ_UseCardReader : ActionInteractBase
{
	void ActionDZBGZ_UseCardReader()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_Text = "BattlegroundZ Keycard einlesen";
	}

	override void CreateConditionComponents()
	{
		m_ConditionTarget = new CCTCursor(UAMaxDistances.SMALL);
		m_ConditionItem = new CCINone;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!player || !target)
			return false;

		if (!player.IsAlive() || player.IsUnconscious())
			return false;

		Object object = target.GetObject();
		return DZBGZ_CardReader.Cast(object) != null;
	}

	override void OnExecuteServer(ActionData action_data)
	{
		if (!action_data || !action_data.m_Player || !action_data.m_Target)
			return;

		DZBGZ_EventManager.GetInstance().TryActivateReader(action_data.m_Player, action_data.m_Target.GetObject());
	}
}
