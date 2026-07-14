class DZCRZ_ATM: ExpansionATM_2
{
}

modded class ExpansionATM_2
{
	protected string m_DZCRZ_ATMId;
	protected float m_DZCRZ_HackDuration = 300.0;

	void ExpansionATM_2()
	{
		RegisterNetSyncVariableFloat("m_DZCRZ_HackDuration", 10.0, 3600.0);
	}

	override void EEInit()
	{
		super.EEInit();
		if (GetGame() && GetGame().IsServer())
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DZCRZ_RegisterSelf, 1000, false);
	}

	protected void DZCRZ_RegisterSelf()
	{
		if (GetGame() && GetGame().IsServer())
			DZCRZ_Manager.GetInstance().RegisterExpansionATM(this);
	}

	void DZCRZ_SetATMId(string atmId)
	{
		m_DZCRZ_ATMId = atmId;
		SetSynchDirty();
	}

	string DZCRZ_GetATMId()
	{
		return m_DZCRZ_ATMId;
	}

	void DZCRZ_AssignRandomDuration(int minimum, int maximum)
	{
		m_DZCRZ_HackDuration = DZCRZ_Utils.RandomDuration(minimum, maximum);
		SetSynchDirty();
	}

	float DZCRZ_GetHackDuration()
	{
		return m_DZCRZ_HackDuration;
	}

	override void SetActions()
	{
		super.SetActions();
		AddAction(ActionDZCRZ_RobATM);
	}
}

class ActionDZCRZ_RobATMCB: ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		float duration = 300.0;
		if (m_ActionData && m_ActionData.m_Target)
		{
			Object atmObject = m_ActionData.m_Target.GetObject();
			DZCRZ_ATM criminalATM = DZCRZ_ATM.Cast(atmObject);
			ExpansionATM_2 expansionATM = ExpansionATM_2.Cast(atmObject);
			if (criminalATM)
				duration = criminalATM.DZCRZ_GetHackDuration();
			else if (expansionATM)
				duration = expansionATM.DZCRZ_GetHackDuration();
		}
		m_ActionData.m_ActionComponent = new CAContinuousTime(duration);
	}
}

class ActionDZCRZ_RobATM: ActionContinuousBase
{
	void ActionDZCRZ_RobATM()
	{
		m_CallbackClass = ActionDZCRZ_RobATMCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_HACKTREE;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_Text = "ATM ausrauben";
	}

	override void CreateConditionComponents()
	{
		m_ConditionTarget = new CCTCursor(UAMaxDistances.DEFAULT);
		m_ConditionItem = new CCINonRuined;
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
		if (!player || !target || !item || !item.IsKindOf(DZCRZ_Const.ATM_TOOL_CLASS))
			return false;
		Object atmObject = target.GetObject();
		if (DZCRZ_ATM.Cast(atmObject) || ExpansionATM_2.Cast(atmObject))
			return true;
		return false;
	}

	override void OnStartServer(ActionData action_data)
	{
		super.OnStartServer(action_data);
		if (action_data && action_data.m_Player && action_data.m_Target)
			DZCRZ_Manager.GetInstance().StartATMHack(action_data.m_Player, action_data.m_Target.GetObject());
	}

	override void OnEndServer(ActionData action_data)
	{
		super.OnEndServer(action_data);
		if (action_data && action_data.m_Player && action_data.m_Target)
			DZCRZ_Manager.GetInstance().CancelHack(action_data.m_Player, action_data.m_Target.GetObject(), DZCRZ_Const.CANCEL_ACTION_ENDED);
	}

	override void OnFinishProgressServer(ActionData action_data)
	{
		if (action_data && action_data.m_Player && action_data.m_Target)
			DZCRZ_Manager.GetInstance().CompleteHack(action_data.m_Player, action_data.m_Target.GetObject());
	}
}
