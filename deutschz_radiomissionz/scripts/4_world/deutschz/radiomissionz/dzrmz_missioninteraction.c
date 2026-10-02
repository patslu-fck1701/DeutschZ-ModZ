class DZRMZ_MissionMarkerService
{
	protected static const string MARKER_UID = "DZRMZ_ACTIVE_MISSION";
	protected static const string MARKER_ICON = "Questionmark";
	protected static const int MARKER_COLOR = ARGB(255, 255, 196, 0);

	static bool Create(DZRMZ_MissionDefinition definition, DZRMZ_Location objective, DZRMZ_Location delivery)
	{
		if (!definition || !objective)
			return false;

#ifdef EXPANSIONMODNAVIGATION
		ExpansionMarkerModule markerModule = ExpansionMarkerModule.GetModuleInstance();
		if (!markerModule)
		{
			DZRMZ_Log.Warn("Expansion MarkerModule ist nicht initialisiert; Missionsmarker wurde nicht erstellt.");
			return false;
		}

		vector position = objective.GetGroundedPosition();
		string label = "RadioMissionZ: " + definition.Title;
		if (definition.ObjectiveType == DZRMZ_OBJECTIVE_DELIVERY && delivery)
		{
			position = delivery.GetGroundedPosition();
			label += " - Abgabe";
		}

		markerModule.RemoveServerMarker(MARKER_UID);
		ExpansionMarkerData marker = markerModule.CreateServerMarker(label, MARKER_ICON, position, MARKER_COLOR, true, MARKER_UID);
		if (!marker)
		{
			DZRMZ_Log.Warn("Expansion-Missionsmarker konnte nicht erstellt werden.");
			return false;
		}

		marker.SetVisibility(EXPANSION_MARKER_VIS_WORLD | EXPANSION_MARKER_VIS_MAP);
		return true;
#else
		DZRMZ_Log.Warn("EXPANSIONMODNAVIGATION fehlt; Missionsmarker wurde nicht erstellt.");
		return false;
#endif
	}

	static void Remove()
	{
#ifdef EXPANSIONMODNAVIGATION
		ExpansionMarkerModule markerModule = ExpansionMarkerModule.GetModuleInstance();
		if (markerModule)
			markerModule.RemoveServerMarker(MARKER_UID);
#endif
	}
}

class ActionDZRMZ_ConfirmHelp : ActionSingleUseBase
{
	void ActionDZRMZ_ConfirmHelp()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
		m_Text = "RadioMissionZ: Mission annehmen";
	}
	override void CreateConditionComponents() { m_ConditionTarget = new CCTNone; m_ConditionItem = new CCINonRuined; }
	override bool HasTarget() { return false; }
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		TransmitterBase transmitter = TransmitterBase.Cast(item);
		return player && player.IsAlive() && !player.IsUnconscious() && transmitter && transmitter.GetCompEM() && transmitter.GetCompEM().IsWorking();
	}
	override void OnExecuteServer(ActionData action_data)
	{
		if (action_data && action_data.m_Player) DZRMZ_MissionManager.GetInstance().TryAcceptMission(action_data.m_Player);
	}
}

class ActionDZRMZ_ProvideMedicalAid : ActionSingleUseBase
{
	void ActionDZRMZ_ProvideMedicalAid() { m_CommandUID=DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE; m_CommandUIDProne=DayZPlayerConstants.CMD_ACTIONFB_INTERACT; m_StanceMask=DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH; m_LockTargetOnUse=false; m_Text="Verletzten versorgen"; }
	override void CreateConditionComponents() { m_ConditionTarget=new CCTCursor; m_ConditionItem=new CCINone; }
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!player || !target) return false;
		PlayerBase patient = PlayerBase.Cast(target.GetObject());
		return patient && patient.DZRMZ_IsMedicalAidTarget() && patient.IsAlive() && vector.Distance(player.GetPosition(), patient.GetPosition()) <= 3.0;
	}
	override void OnExecuteServer(ActionData action_data) { if(action_data && action_data.m_Target) DZRMZ_MissionManager.GetInstance().TryProvideMedicalAid(action_data.m_Player, action_data.m_Target.GetObject()); }
}
modded class ActionConstructor
{
	override void RegisterActions(TTypenameArray actions)
	{
		super.RegisterActions(actions);
		actions.Insert(ActionDZRMZ_ConfirmHelp);
		actions.Insert(ActionDZRMZ_ProvideMedicalAid);
	}
}

modded class TransmitterBase
{
	override void SetActions()
	{
		super.SetActions();
		AddAction(ActionDZRMZ_ConfirmHelp);
	}
}

modded class PlayerBase
{
	protected bool m_DZRMZ_MedicalAidTarget;
	void PlayerBase() { RegisterNetSyncVariableBool("m_DZRMZ_MedicalAidTarget"); }
	void DZRMZ_SetMedicalAidTarget(bool value) { m_DZRMZ_MedicalAidTarget = value; SetSynchDirty(); }
	bool DZRMZ_IsMedicalAidTarget() { return m_DZRMZ_MedicalAidTarget; }
	override void SetActions() { super.SetActions(); AddAction(ActionDZRMZ_ProvideMedicalAid); }
}


