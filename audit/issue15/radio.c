class DZRMZ_RadioService
{
	static int SendHelpAlert(DZRMZ_Settings settings, string message)
	{
		if (!GetGame() || !GetGame().IsServer() || !settings || message == "")
			return 0;

		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		int receivers = 0;
		foreach (Man man : players)
		{
			PlayerBase player;
			if (!Class.CastTo(player, man) || !player.GetIdentity() || !player.IsAlive())
				continue;
			SendToPlayer(settings, player, "EIN HILFERUF", message);
			receivers++;
		}
		DZRMZ_Log.Info(string.Format("Hilferuf-Hinweis an %1 Spieler gesendet.", receivers));
		return receivers;
	}

	static bool HasQualifiedRadio(PlayerBase player, DZRMZ_Settings settings)
	{
		if (!player || !player.GetIdentity() || !settings)
			return false;

		array<EntityAI> inventoryItems = new array<EntityAI>;
		player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, inventoryItems);

		foreach (EntityAI entity : inventoryItems)
		{
			TransmitterBase transmitter;
			if (!entity || !Class.CastTo(transmitter, entity) || transmitter.IsRuined())
				continue;
			// IsReceiving() is not a reliable server-side state for handheld radios.
			// Powered-on and correctly tuned is the authoritative reception condition.
			if (!transmitter.GetCompEM() || !transmitter.GetCompEM().IsWorking())
				continue;

			float difference = Math.AbsFloat(transmitter.GetTunedFrequency() - settings.RadioFrequencyMHz);
			if (difference <= settings.FrequencyToleranceMHz)
				return true;
		}

		return false;
	}

	static int SendRadioLine(DZRMZ_Settings settings, string message)
	{
		if (!GetGame() || !GetGame().IsServer() || !settings || message == "")
			return 0;

		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		int receivers = 0;
		string title = string.Format("DeutschZ Funk %1 MHz", settings.RadioFrequencyMHz);

		foreach (Man man : players)
		{
			PlayerBase player;
			if (!Class.CastTo(player, man) || !player.IsAlive() || !HasQualifiedRadio(player, settings))
				continue;

			NotificationSystem.SendNotificationToPlayerExtended(player, settings.AnnouncementDisplaySeconds, title, message);
			receivers++;
		}

		DZRMZ_Log.Info(string.Format("Funkzeile an %1 qualifizierte Empfaenger auf %2 MHz: %3", receivers, settings.RadioFrequencyMHz, message));
		return receivers;
	}

	static void SendToPlayer(DZRMZ_Settings settings, PlayerBase player, string title, string message)
	{
		if (!GetGame() || !GetGame().IsServer() || !settings || !player || !player.GetIdentity())
			return;
		NotificationSystem.SendNotificationToPlayerExtended(player, settings.AnnouncementDisplaySeconds, title, message);
	}
}
