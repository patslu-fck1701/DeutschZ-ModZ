class DZBBC_MilitaryTerminal : GPSReceiver
{
	override void SetActions()
	{
		super.SetActions();
		AddAction(ActionDZBBC_StartTerminalDecrypt);
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

class ActionDZBBC_StartTerminalDecryptCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(60.0);
	}
}

class ActionDZBBC_StartTerminalDecrypt : ActionContinuousBase
{
	void ActionDZBBC_StartTerminalDecrypt()
	{
		m_CallbackClass = ActionDZBBC_StartTerminalDecryptCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_HACKTREE;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
		m_Text = "#STR_DZBBC_ACTION_DECRYPT_DATACORE";
	}

	override void CreateConditionComponents()
	{
		m_ConditionTarget = new CCTCursor(UAMaxDistances.SMALL);
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

		Object object = target.GetObject();
		return DZBBC_MilitaryTerminal.Cast(object) != null;
	}

	override void OnFinishProgressServer(ActionData action_data)
	{
		if (!action_data || !action_data.m_Player || !action_data.m_Target)
			return;

		DZBBC_EventManager.GetInstance().CompleteTerminalDecrypt(action_data.m_Player, action_data.m_Target.GetObject());
	}
}
