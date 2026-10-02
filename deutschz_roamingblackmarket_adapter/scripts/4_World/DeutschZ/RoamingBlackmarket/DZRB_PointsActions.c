class DZRB_ActionRedeemPointTokens : ActionInteractBase
{
	void DZRB_ActionRedeemPointTokens()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
		m_Text = "DeutschZ-Wertmarken einloesen";
	}

	override void CreateConditionComponents()
	{
		m_ConditionTarget = new CCTCursor(UAMaxDistances.DEFAULT);
		m_ConditionItem = new CCINone;
	}

	override bool HasTarget()
	{
		return true;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		return player && target && DZRB_ExpansionTraderService.IsActiveTrader(target.GetObject());
	}

	override void OnStartServer(ActionData action_data)
	{
		super.OnStartServer(action_data);

		PlayerBase player = action_data.m_Player;
		if (!player || !player.GetIdentity())
			return;

		array<EntityAI> inventory = new array<EntityAI>;
		player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, inventory);

		array<EntityAI> tokens = new array<EntityAI>;
		int total = 0;

		foreach (EntityAI entity : inventory)
		{
			if (!entity)
				continue;

			int value = DZPoints_TokenValues.Get(entity.GetType());

			if (value > 0)
			{
				total += value;
				tokens.Insert(entity);
			}
		}

		if (total <= 0)
		{
			NotificationSystem.SendNotificationToPlayerIdentityExtended(
				player.GetIdentity(),
				5,
				"DeutschZ Points",
				"Du hast keine Wertmarken dabei."
			);
			return;
		}

		string transactionID =
			"TOKEN-"
			+ player.GetIdentity().GetPlainId()
			+ "-"
			+ ExpansionStatic.GetTimestamp(true).ToString()
			+ "-"
			+ GetGame().GetTime().ToString();

		if (!DZPoints_Service.Credit(
			player.GetIdentity().GetPlainId(),
			total,
			"TOKEN",
			transactionID
		))
		{
			NotificationSystem.SendNotificationToPlayerIdentityExtended(
				player.GetIdentity(),
				6,
				"DeutschZ Points",
				"Einloesung fehlgeschlagen. Deine Marken wurden nicht entfernt."
			);
			return;
		}

		foreach (EntityAI token : tokens)
		{
			GetGame().ObjectDelete(token);
		}

		NotificationSystem.SendNotificationToPlayerIdentityExtended(
			player.GetIdentity(),
			7,
			"DeutschZ Points",
			total.ToString() + " Points gutgeschrieben."
		);

		Print(
			"[DeutschZ PointsZ] TOKENS_REDEEMED player="
			+ player.GetIdentity().GetPlainId()
			+ " entities="
			+ tokens.Count().ToString()
			+ " points="
			+ total.ToString()
			+ " tx="
			+ transactionID
		);
	}
}

modded class ActionConstructor
{
	override void RegisterActions(TTypenameArray actions)
	{
		super.RegisterActions(actions);
		actions.Insert(DZRB_ActionRedeemPointTokens);
	}
}

modded class PlayerBase
{
	override void SetActions(out TInputActionMap InputActionMap)
	{
		super.SetActions(InputActionMap);
		AddAction(DZRB_ActionRedeemPointTokens, InputActionMap);
	}
}
