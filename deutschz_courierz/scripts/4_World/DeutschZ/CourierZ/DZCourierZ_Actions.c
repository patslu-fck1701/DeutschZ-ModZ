class ActionCourierZInteract : ActionInteractBase
{
	void ActionCourierZInteract()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
		m_Text = "CourierZ Auftrag fortsetzen";
	}

	override void CreateConditionComponents() { m_ConditionTarget = new CCTCursor(UAMaxDistances.DEFAULT); m_ConditionItem = new CCINone; }
	override bool HasTarget() { return true; }

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!player || !target || !target.GetObject()) return false;
		return target.GetObject().IsKindOf("ExpansionNPCDenis");
	}

	override void OnStartServer(ActionData action_data)
	{
		super.OnStartServer(action_data);
		if (!action_data || !action_data.m_Player || !action_data.m_Target) return;
		DZCourierZ_Manager.Get().HandleCourierInteraction(action_data.m_Player, action_data.m_Target.GetObject());
	}
}

modded class ActionConstructor
{
	override void RegisterActions(TTypenameArray actions)
	{
		super.RegisterActions(actions);
		actions.Insert(ActionCourierZInteract);
	}
}

modded class PlayerBase
{
	override void SetActions(out TInputActionMap InputActionMap)
	{
		super.SetActions(InputActionMap);
		AddAction(ActionCourierZInteract, InputActionMap);
	}
}
