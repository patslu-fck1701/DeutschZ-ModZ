class DZECZ_StartReceiver : House
{
	override bool IsBuilding()
	{
		return false;
	}

	override void SetActions()
	{
		super.SetActions();
		AddAction(ActionDZECZ_StartEvent);
	}
}

class DZECZ_NWAFStation : House
{
	override bool IsBuilding()
	{
		return false;
	}

	override void SetActions()
	{
		super.SetActions();
		AddAction(ActionDZECZ_ActivateStation);
	}
}

class DZECZ_TisyStation : House
{
	override bool IsBuilding()
	{
		return false;
	}

	override void SetActions()
	{
		super.SetActions();
		AddAction(ActionDZECZ_ActivateStation);
	}
}

class DZECZ_GasZoneStation : House
{
	override bool IsBuilding()
	{
		return false;
	}

	override void SetActions()
	{
		super.SetActions();
		AddAction(ActionDZECZ_ActivateStation);
	}
}

class DZECZ_FireworkBattery : FireworksLauncher
{
	protected int m_DZECZ_BatteryStep;

	void DZECZ_FireworkBattery()
	{
		m_DZECZ_BatteryStep = 0;
		RegisterNetSyncVariableInt("m_DZECZ_BatteryStep", 0, 3);
	}

	override void SetActions()
	{
		super.SetActions();
		AddAction(ActionDZECZ_PrepareBattery);
	}

	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}

	override bool CanPutInCargo(EntityAI parent)
	{
		return false;
	}

	void DZECZ_SetStep(int step)
	{
		m_DZECZ_BatteryStep = step;
		if (GetGame() && GetGame().IsServer())
			SetSynchDirty();
	}

	int DZECZ_GetStep()
	{
		return m_DZECZ_BatteryStep;
	}

	void DZECZ_Launch()
	{
		OnIgnitedThis(null);
	}
}

class DZECZ_FinalTerminal : House
{
	override bool IsBuilding()
	{
		return false;
	}

	override void SetActions()
	{
		super.SetActions();
		AddAction(ActionDZECZ_ActivateFinalTerminal);
		AddAction(ActionDZECZ_ShutdownSignal);
		AddAction(ActionDZECZ_SendSignal);
	}
}

class ActionDZECZ_StartEvent : ActionInteractBase
{
	void ActionDZECZ_StartEvent()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_Text = "Eclipse-Protokoll entschluesseln";
	}

	override void CreateConditionComponents()
	{
		m_ConditionTarget = new CCTCursor(UAMaxDistances.SMALL);
		m_ConditionItem = new CCINone;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!DZECZ_Utils.IsValidPlayer(player) || !target)
			return false;
		return DZECZ_StartReceiver.Cast(target.GetObject()) != null;
	}

	override void OnExecuteServer(ActionData action_data)
	{
		if (!action_data || !action_data.m_Player || !action_data.m_Target)
			return;
		DZECZ_EventManager.GetInstance().TryStartEvent(action_data.m_Player, action_data.m_Target.GetObject());
	}
}

class ActionDZECZ_ActivateStationCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		float duration = DZECZ_EventManager.GetInstance().GetActionDuration(m_ActionData.m_Target.GetObject());
		m_ActionData.m_ActionComponent = new CAContinuousTime(duration);
	}
}

class ActionDZECZ_ActivateStation : ActionContinuousBase
{
	void ActionDZECZ_ActivateStation()
	{
		m_CallbackClass = ActionDZECZ_ActivateStationCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_HACKTREE;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_Text = "Eclipse-Signalstation aktivieren";
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
		if (!DZECZ_Utils.IsValidPlayer(player) || !target)
			return false;
		Object object = target.GetObject();
		return DZECZ_NWAFStation.Cast(object) || DZECZ_TisyStation.Cast(object) || DZECZ_GasZoneStation.Cast(object);
	}

	override void OnStartServer(ActionData action_data)
	{
		super.OnStartServer(action_data);
		if (action_data && action_data.m_Player && action_data.m_Target)
			DZECZ_EventManager.GetInstance().StartTimedAction(action_data.m_Player, action_data.m_Target.GetObject());
	}

	override void OnEndServer(ActionData action_data)
	{
		super.OnEndServer(action_data);
		if (action_data && action_data.m_Player && action_data.m_Target)
			DZECZ_EventManager.GetInstance().CancelTimedAction(action_data.m_Player, action_data.m_Target.GetObject());
	}

	override void OnFinishProgressServer(ActionData action_data)
	{
		if (action_data && action_data.m_Player && action_data.m_Target)
			DZECZ_EventManager.GetInstance().CompleteTimedAction(action_data.m_Player, action_data.m_Target.GetObject());
	}
}

class ActionDZECZ_PrepareBatteryCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		float duration = DZECZ_EventManager.GetInstance().GetActionDuration(m_ActionData.m_Target.GetObject());
		m_ActionData.m_ActionComponent = new CAContinuousTime(duration);
	}
}

class ActionDZECZ_PrepareBattery : ActionContinuousBase
{
	void ActionDZECZ_PrepareBattery()
	{
		m_CallbackClass = ActionDZECZ_PrepareBatteryCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_HACKTREE;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_Text = "Eclipse-Feuerwerk vorbereiten";
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
		if (!DZECZ_Utils.IsValidPlayer(player) || !target)
			return false;
		return DZECZ_FireworkBattery.Cast(target.GetObject()) != null;
	}

	override void OnStartServer(ActionData action_data)
	{
		super.OnStartServer(action_data);
		if (action_data && action_data.m_Player && action_data.m_Target)
			DZECZ_EventManager.GetInstance().StartTimedAction(action_data.m_Player, action_data.m_Target.GetObject());
	}

	override void OnEndServer(ActionData action_data)
	{
		super.OnEndServer(action_data);
		if (action_data && action_data.m_Player && action_data.m_Target)
			DZECZ_EventManager.GetInstance().CancelTimedAction(action_data.m_Player, action_data.m_Target.GetObject());
	}

	override void OnFinishProgressServer(ActionData action_data)
	{
		if (action_data && action_data.m_Player && action_data.m_Target)
			DZECZ_EventManager.GetInstance().CompleteTimedAction(action_data.m_Player, action_data.m_Target.GetObject());
	}
}

class ActionDZECZ_ActivateFinalTerminalCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		float duration = DZECZ_EventManager.GetInstance().GetActionDuration(m_ActionData.m_Target.GetObject());
		m_ActionData.m_ActionComponent = new CAContinuousTime(duration);
	}
}

class ActionDZECZ_ActivateFinalTerminal : ActionContinuousBase
{
	void ActionDZECZ_ActivateFinalTerminal()
	{
		m_CallbackClass = ActionDZECZ_ActivateFinalTerminalCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_HACKTREE;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
		m_Text = "Eclipse-Protokoll bestaetigen";
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
		if (!DZECZ_Utils.IsValidPlayer(player) || !target)
			return false;
		return DZECZ_FinalTerminal.Cast(target.GetObject()) != null;
	}

	override void OnStartServer(ActionData action_data)
	{
		super.OnStartServer(action_data);
		if (action_data && action_data.m_Player && action_data.m_Target)
			DZECZ_EventManager.GetInstance().StartTimedAction(action_data.m_Player, action_data.m_Target.GetObject());
	}

	override void OnEndServer(ActionData action_data)
	{
		super.OnEndServer(action_data);
		if (action_data && action_data.m_Player && action_data.m_Target)
			DZECZ_EventManager.GetInstance().CancelTimedAction(action_data.m_Player, action_data.m_Target.GetObject());
	}

	override void OnFinishProgressServer(ActionData action_data)
	{
		if (action_data && action_data.m_Player && action_data.m_Target)
			DZECZ_EventManager.GetInstance().CompleteTimedAction(action_data.m_Player, action_data.m_Target.GetObject());
	}
}

class ActionDZECZ_ShutdownSignal : ActionInteractBase
{
	void ActionDZECZ_ShutdownSignal()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_Text = "Signal abschalten";
	}

	override void CreateConditionComponents()
	{
		m_ConditionTarget = new CCTCursor(UAMaxDistances.SMALL);
		m_ConditionItem = new CCINone;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		return DZECZ_Utils.IsValidPlayer(player) && target && DZECZ_FinalTerminal.Cast(target.GetObject());
	}

	override void OnExecuteServer(ActionData action_data)
	{
		if (action_data && action_data.m_Player && action_data.m_Target)
			DZECZ_EventManager.GetInstance().ChooseEnding(action_data.m_Player, action_data.m_Target.GetObject(), false);
	}
}

class ActionDZECZ_SendSignal : ActionInteractBase
{
	void ActionDZECZ_SendSignal()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_Text = "Signal senden (riskant)";
	}

	override void CreateConditionComponents()
	{
		m_ConditionTarget = new CCTCursor(UAMaxDistances.SMALL);
		m_ConditionItem = new CCINone;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		return DZECZ_Utils.IsValidPlayer(player) && target && DZECZ_FinalTerminal.Cast(target.GetObject());
	}

	override void OnExecuteServer(ActionData action_data)
	{
		if (action_data && action_data.m_Player && action_data.m_Target)
			DZECZ_EventManager.GetInstance().ChooseEnding(action_data.m_Player, action_data.m_Target.GetObject(), true);
	}
}
