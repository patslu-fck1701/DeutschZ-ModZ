class DZCRZ_VehicleHackManager
{
	bool IsLockedVehicle(Object target)
	{
#ifdef EXPANSIONMODVEHICLE
		ExpansionVehicle vehicle = ExpansionVehicle.Get(target);
		return vehicle && vehicle.IsLocked();
#endif
		return false;
	}

	bool ForceUnlock(Object target)
	{
#ifdef EXPANSIONMODVEHICLE
		ExpansionVehicle vehicle = ExpansionVehicle.Get(target);
		if (!vehicle || !vehicle.IsLocked())
			return false;
		vehicle.ForceUnlock();
		return !vehicle.IsLocked();
#endif
		return false;
	}
}

class ActionDZCRZ_HackVehicleCB: ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		float duration = 300.0;
		if (m_ActionData && m_ActionData.m_Target)
		{
			CarScript vehicle = CarScript.Cast(m_ActionData.m_Target.GetParentOrObject());
			if (vehicle)
				duration = vehicle.DZCRZ_GetHackDuration();
		}
		m_ActionData.m_ActionComponent = new CAContinuousTime(duration);
	}
}

class ActionDZCRZ_HackVehicle: ActionContinuousBase
{
	void ActionDZCRZ_HackVehicle()
	{
		m_CallbackClass = ActionDZCRZ_HackVehicleCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_HACKTREE;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_Text = "Fahrzeug hacken";
	}

	override void CreateConditionComponents()
	{
		m_ConditionTarget = new CCTNone;
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
		if (!player || !target || !item || !item.IsKindOf(DZCRZ_Const.HACK_TOOL_CLASS))
			return false;
		Object vehicleObject = target.GetParentOrObject();
		if (!CarScript.Cast(vehicleObject))
			return false;
#ifdef EXPANSIONMODVEHICLE
		ExpansionVehicle vehicle = ExpansionVehicle.Get(vehicleObject);
		return vehicle && vehicle.IsLocked();
#endif
		return false;
	}

	override void OnStartServer(ActionData action_data)
	{
		super.OnStartServer(action_data);
		if (action_data && action_data.m_Player && action_data.m_Target)
			DZCRZ_Manager.GetInstance().StartVehicleHack(action_data.m_Player, action_data.m_Target.GetParentOrObject());
	}

	override void OnEndServer(ActionData action_data)
	{
		super.OnEndServer(action_data);
		if (action_data && action_data.m_State != UA_FINISHED && action_data.m_Player && action_data.m_Target)
			DZCRZ_Manager.GetInstance().CancelHack(action_data.m_Player, action_data.m_Target.GetParentOrObject(), DZCRZ_Const.CANCEL_ACTION_ENDED);
	}

	override void OnFinishProgressServer(ActionData action_data)
	{
		super.OnFinishProgressServer(action_data);
		if (action_data && action_data.m_Player && action_data.m_Target)
			DZCRZ_Manager.GetInstance().CompleteHack(action_data.m_Player, action_data.m_Target.GetParentOrObject());
	}
}

modded class CarScript
{
	protected float m_DZCRZ_HackDuration = 300.0;

	void CarScript()
	{
		RegisterNetSyncVariableFloat("m_DZCRZ_HackDuration", 10.0, 3600.0);
	}

	override void EEInit()
	{
		super.EEInit();
		if (GetGame() && GetGame().IsServer())
		{
			DZCRZ_Config config = DZCRZ_Manager.GetInstance().GetConfig();
			if (config && config.VehicleModule)
				m_DZCRZ_HackDuration = DZCRZ_Utils.RandomDuration(config.VehicleModule.HackTimeMinSeconds, config.VehicleModule.HackTimeMaxSeconds);
			SetSynchDirty();
		}
	}

	float DZCRZ_GetHackDuration()
	{
		return m_DZCRZ_HackDuration;
	}

	override void SetActions()
	{
		super.SetActions();
		AddAction(ActionDZCRZ_HackVehicle);
	}
}
