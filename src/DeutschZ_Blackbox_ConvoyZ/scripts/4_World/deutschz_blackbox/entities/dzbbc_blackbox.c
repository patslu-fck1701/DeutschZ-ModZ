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

class ActionDZBBC_StartBlackboxHack : ActionInteractBase
{
	void ActionDZBBC_StartBlackboxHack()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_Text = "#STR_DZBBC_ACTION_HACK_BLACKBOX";
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!player || !target)
			return false;

		Object object = target.GetObject();
		return DZBBC_Blackbox.Cast(object) != null;
	}

	override void OnExecuteServer(ActionData action_data)
	{
		if (!action_data || !action_data.m_Player || !action_data.m_Target)
			return;

		bool started = DZBBC_EventManager.GetInstance().StartBlackboxHack(action_data.m_Player, action_data.m_Target.GetObject());
		if (started)
		{
			DZBBC_Utils.Log("Blackbox hack action accepted for " + DZBBC_PlayerUtils.GetPlayerName(action_data.m_Player));
			DZBBC_ServerRPC.SendNotification(action_data.m_Player, "ConvoyZ Blackbox", "Hack gestartet. Bleibe an der Blackbox.", 7.0);
		}
		else
		{
			DZBBC_Utils.Warn("Blackbox hack action rejected for " + DZBBC_PlayerUtils.GetPlayerName(action_data.m_Player));
			DZBBC_ServerRPC.SendNotification(action_data.m_Player, "ConvoyZ Blackbox", "Hack ist in diesem Eventzustand nicht moeglich.", 7.0);
		}
	}
}
