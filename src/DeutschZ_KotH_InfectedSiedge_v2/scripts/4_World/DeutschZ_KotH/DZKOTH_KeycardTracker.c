class DZKOTH_KeycardTracker
{
	protected bool m_Announced;

	void Reset()
	{
		m_Announced = false;
	}

	bool Tick(array<PlayerBase> players, bool globalAnnouncement)
	{
		if (m_Announced || !players)
			return false;

		foreach (PlayerBase player: players)
		{
			if (!IsValidPlayer(player))
				continue;

			if (!HasKeycard(player))
				continue;

			m_Announced = true;
			Announce(player, globalAnnouncement);
			return true;
		}

		return false;
	}

	protected bool HasKeycard(PlayerBase player)
	{
		if (!player || !player.GetInventory())
			return false;

		array<EntityAI> items = new array<EntityAI>;
		player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
		foreach (EntityAI item: items)
		{
			if (item && item.GetType() == DZKOTH_Const.KEYCARD_CLASSNAME)
				return true;
		}

		return false;
	}

	protected void Announce(PlayerBase player, bool globalAnnouncement)
	{
		if (!player || !player.GetIdentity())
			return;

		string name = player.GetIdentity().GetName();
		vector pos = player.GetPosition();
		string hint = "Keycard-Fund: " + name + " bei X/Z " + DZKOTH_Utils.FormatXZ(pos) + ". Keine permanente Verfolgung aktiv.";

		if (globalAnnouncement)
			DZKOTH_ServerRPC.BroadcastWarning("BattlegroundZ Signal", hint, 10.0);
		else
			DZKOTH_ServerRPC.SendWarning(player, "BattlegroundZ Signal", hint, 10.0);

		DZKOTH_ServerRPC.BroadcastKeycardSignal(name, pos);
		DZKOTH_Utils.Log(hint);
	}

	protected bool IsValidPlayer(PlayerBase player)
	{
		return player && player.GetIdentity() && player.IsAlive();
	}
}
