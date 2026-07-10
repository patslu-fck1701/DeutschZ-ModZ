class DZKOTH_EventTerminal : SeaChest
{
	protected bool m_DZKOTH_ActionUnlocked;

	void DZKOTH_EventTerminal()
	{
		m_DZKOTH_ActionUnlocked = false;
		RegisterNetSyncVariableBool("m_DZKOTH_ActionUnlocked");
	}

	override void SetActions()
	{
		super.SetActions();
		AddAction(ActionDeutschZBreakKotHChest);
	}

	void DZKOTH_SetActionUnlocked(bool unlocked)
	{
		m_DZKOTH_ActionUnlocked = unlocked;
		if (GetGame() && GetGame().IsServer())
			SetSynchDirty();
	}

	bool DZKOTH_CanBreak()
	{
		return m_DZKOTH_ActionUnlocked;
	}

	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}
}

class ActionDeutschZBreakKotHChestCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		float duration = DZKOTH_EventManager.GetInstance().GetChestActionDuration();
		if (duration < 1.0)
			duration = 60.0;

		m_ActionData.m_ActionComponent = new CAContinuousTime(duration);
	}
}

class ActionDeutschZBreakKotHChest : ActionContinuousBase
{
	void ActionDeutschZBreakKotHChest()
	{
		m_CallbackClass = ActionDeutschZBreakKotHChestCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_HACKTREE;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
		m_Text = "Kiste aufbrechen";
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

		if (!player.IsAlive() || player.IsUnconscious())
			return false;

		DZKOTH_EventTerminal chest = DZKOTH_EventTerminal.Cast(target.GetObject());
		if (!chest || !chest.DZKOTH_CanBreak())
			return false;

		return true;
	}

	override void OnStartServer(ActionData action_data)
	{
		super.OnStartServer(action_data);
		if (!action_data || !action_data.m_Player || !action_data.m_Target)
			return;

		DZKOTH_EventManager.GetInstance().BeginTerminalHack(action_data.m_Player, action_data.m_Target.GetObject());
	}

	override void OnEndServer(ActionData action_data)
	{
		super.OnEndServer(action_data);
		if (!action_data || !action_data.m_Player)
			return;

		Object targetObject;
		if (action_data.m_Target)
			targetObject = action_data.m_Target.GetObject();

		DZKOTH_EventManager.GetInstance().CancelTerminalHack(action_data.m_Player, targetObject);
	}

	override void OnFinishProgressServer(ActionData action_data)
	{
		if (!action_data || !action_data.m_Player || !action_data.m_Target)
			return;

		DZKOTH_EventManager.GetInstance().StartTerminalHack(action_data.m_Player, action_data.m_Target.GetObject());
	}
}

class ActionDZKOTH_StartTerminalHackCB : ActionDeutschZBreakKotHChestCB
{
}

class ActionDZKOTH_StartTerminalHack : ActionDeutschZBreakKotHChest
{
}
