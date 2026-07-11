class ActionDZKOTHGHackTerminalCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		float duration = DZKOTHG_Manager.GetInstance().GetHackDurationSeconds();
		if (duration < 1.0)
			duration = 60.0;

		m_ActionData.m_ActionComponent = new CAContinuousTime(duration);
	}
}

class ActionDZKOTHGHackTerminal : ActionContinuousBase
{
	void ActionDZKOTHGHackTerminal()
	{
		m_CallbackClass = ActionDZKOTHGHackTerminalCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_Text = "KOTH-Terminal hacken";
	}

	override void CreateConditionComponents()
	{
		m_ConditionTarget = new CCTObject(UAMaxDistances.DEFAULT);
		m_ConditionItem = new CCINone;
	}

	override typename GetInputType()
	{
		return ContinuousInteractActionInput;
	}

	override bool HasTarget()
	{
		return true;
	}

	override bool HasProgress()
	{
		return true;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!player || !target)
			return false;

		if (!player.IsAlive() || player.IsUnconscious())
			return false;

		Object targetObject = DZKOTHG_GetTerminalTarget(target);

		DZ_KOTHTerminal terminal = DZ_KOTHTerminal.Cast(targetObject);
		if (!terminal)
			return false;

		if (GetGame() && GetGame().IsServer())
			return DZKOTHG_Manager.GetInstance().CanHackTerminal(player, targetObject);

		if (!DZKOTHG_IsClientInReach(player, target, targetObject))
			return false;

		return true;
	}

	override void OnStartServer(ActionData action_data)
	{
		super.OnStartServer(action_data);

		if (!action_data || !action_data.m_Player || !action_data.m_Target)
			return;

		Object terminal = DZKOTHG_GetTerminalTarget(action_data.m_Target);

		DZKOTHG_Manager.GetInstance().OnHackStarted(action_data.m_Player, terminal);
	}

	override void OnEndServer(ActionData action_data)
	{
		super.OnEndServer(action_data);

		if (!action_data || !action_data.m_Player)
			return;

		Object terminal;
		if (action_data.m_Target)
			terminal = DZKOTHG_GetTerminalTarget(action_data.m_Target);

		DZKOTHG_Manager.GetInstance().OnHackEnded(action_data.m_Player, terminal);
	}

	override void OnFinishProgressServer(ActionData action_data)
	{
		if (!action_data || !action_data.m_Player || !action_data.m_Target)
			return;

		Object terminal = DZKOTHG_GetTerminalTarget(action_data.m_Target);

		DZKOTHG_Manager.GetInstance().CompleteTerminalHack(action_data.m_Player, terminal);
	}

	protected Object DZKOTHG_GetTerminalTarget(ActionTarget target)
	{
		if (!target)
			return null;

		Object targetObject = target.GetObject();
		if (DZ_KOTHTerminal.Cast(targetObject))
			return targetObject;

		Object parentObject = target.GetParent();
		if (DZ_KOTHTerminal.Cast(parentObject))
			return parentObject;

		return targetObject;
	}

	protected bool DZKOTHG_IsClientInReach(PlayerBase player, ActionTarget target, Object targetObject)
	{
		if (!player || !target || !targetObject)
			return false;

		vector playerPos = player.GetPosition();
		vector targetPos = target.GetCursorHitPos();
		if (targetPos[0] == 0.0 && targetPos[1] == 0.0 && targetPos[2] == 0.0)
			targetPos = targetObject.GetPosition();

		if (vector.Distance(playerPos, targetPos) <= 5.0)
			return true;

		return vector.Distance(playerPos, targetObject.GetPosition()) <= 5.0;
	}
}
