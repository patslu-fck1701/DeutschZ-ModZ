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
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_HACKTREE;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
		m_Text = "KOTH-Terminal hacken";
	}

	override void CreateConditionComponents()
	{
		m_ConditionTarget = new CCTCursorNoRuinCheck(UAMaxDistances.DEFAULT);
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

		Object targetObject = target.GetObject();
		if (!targetObject && target.GetParent())
			targetObject = target.GetParent();

		DZ_KOTHTerminal terminal = DZ_KOTHTerminal.Cast(targetObject);
		if (!terminal || !terminal.DZKOTHG_IsGateTerminal())
			return false;

		if (GetGame() && GetGame().IsServer())
			return DZKOTHG_Manager.GetInstance().CanHackTerminal(player, targetObject);

		return true;
	}

	override void OnStartServer(ActionData action_data)
	{
		super.OnStartServer(action_data);

		if (!action_data || !action_data.m_Player || !action_data.m_Target)
			return;

		Object terminal = action_data.m_Target.GetObject();
		if (!terminal && action_data.m_Target.GetParent())
			terminal = action_data.m_Target.GetParent();

		DZKOTHG_Manager.GetInstance().OnHackStarted(action_data.m_Player, terminal);
	}

	override void OnEndServer(ActionData action_data)
	{
		super.OnEndServer(action_data);

		if (!action_data || !action_data.m_Player)
			return;

		Object terminal;
		if (action_data.m_Target)
		{
			terminal = action_data.m_Target.GetObject();
			if (!terminal && action_data.m_Target.GetParent())
				terminal = action_data.m_Target.GetParent();
		}

		DZKOTHG_Manager.GetInstance().OnHackEnded(action_data.m_Player, terminal);
	}

	override void OnFinishProgressServer(ActionData action_data)
	{
		if (!action_data || !action_data.m_Player || !action_data.m_Target)
			return;

		Object terminal = action_data.m_Target.GetObject();
		if (!terminal && action_data.m_Target.GetParent())
			terminal = action_data.m_Target.GetParent();

		DZKOTHG_Manager.GetInstance().CompleteTerminalHack(action_data.m_Player, terminal);
	}
}
