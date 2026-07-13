class DZBBC_Blackbox : SmallProtectorCase
{
	override void SetActions()
	{
		super.SetActions();
		AddAction(ActionDZBBC_StartBlackboxHack);
	}

	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}

	override bool CanPutInCargo(EntityAI parent)
	{
		return false;
	}
}

class ActionDZBBC_StartBlackboxHackCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		float duration = DZBBC_EventManager.GetInstance().GetBlackboxHackDuration();
		if (duration < 1.0)
			duration = 90.0;

		m_ActionData.m_ActionComponent = new CAContinuousTime(duration);
	}
}

class ActionDZBBC_StartBlackboxHack : ActionContinuousBase
{
	void ActionDZBBC_StartBlackboxHack()
	{
		m_CallbackClass = ActionDZBBC_StartBlackboxHackCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_HACKTREE;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_Text = "#STR_DZBBC_ACTION_HACK_BLACKBOX";
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

		Object object = target.GetObject();
		if (!DZBBC_Blackbox.Cast(object))
			return false;

		return DZBBC_EventManager.GetInstance().CanUseBlackboxAction(player, object);
	}

	override void OnStartServer(ActionData action_data)
	{
		super.OnStartServer(action_data);
		if (!action_data || !action_data.m_Player || !action_data.m_Target)
			return;

		if (!DZBBC_EventManager.GetInstance().StartBlackboxHack(action_data.m_Player, action_data.m_Target.GetObject()))
			DZBBC_Utils.Warn("Blackbox action start rejected for " + DZBBC_PlayerUtils.GetPlayerName(action_data.m_Player));
	}

	override void OnEndServer(ActionData action_data)
	{
		int endState = 0;
		if (action_data)
			endState = action_data.m_State;
		super.OnEndServer(action_data);
		if (!action_data || !action_data.m_Player || !action_data.m_Target)
			return;
		if (endState == UA_FINISHED)
			return;

		DZBBC_EventManager.GetInstance().CancelBlackboxHack(action_data.m_Player, action_data.m_Target.GetObject(), "INPUT_RELEASED_OR_ACTION_CANCELED");
	}

	override void OnFinishProgressServer(ActionData action_data)
	{
		super.OnFinishProgressServer(action_data);
		if (!action_data || !action_data.m_Player || !action_data.m_Target)
			return;

		bool completed = DZBBC_EventManager.GetInstance().CompleteBlackboxHack(action_data.m_Player, action_data.m_Target.GetObject());
		if (completed)
		{
			DZBBC_Utils.Log("Blackbox 90s action completed for " + DZBBC_PlayerUtils.GetPlayerName(action_data.m_Player));
		}
		else
		{
			DZBBC_Utils.Warn("Blackbox 90s action rejected on finish for " + DZBBC_PlayerUtils.GetPlayerName(action_data.m_Player));
			DZBBC_ServerRPC.SendNotification(action_data.m_Player, "ConvoyZ Blackbox", "Hack ist in diesem Eventzustand nicht moeglich.", 7.0);
		}
	}
}
