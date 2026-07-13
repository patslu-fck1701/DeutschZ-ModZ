class DZECZ_Utils
{
	static void Log(string message)
	{
		Print("[DZECZ] " + message);
		DZECZ_ProfilePaths.AppendLog(message);
	}

	static void Warn(string message)
	{
		Print("[DZECZ][WARN] " + message);
		DZECZ_ProfilePaths.AppendLog("WARN " + message);
	}

	static vector Grounded(vector pos)
	{
		if (GetGame())
			pos[1] = GetGame().SurfaceY(pos[0], pos[2]);
		return pos;
	}

	static bool IsValidPlayer(PlayerBase player)
	{
		return player && player.GetIdentity() && player.IsAlive() && !player.IsUnconscious();
	}

	static string GetUID(PlayerBase player)
	{
		if (!player || !player.GetIdentity())
			return "";
		return player.GetIdentity().GetId();
	}

	static string GetName(PlayerBase player)
	{
		if (!player || !player.GetIdentity())
			return "unknown";
		return player.GetIdentity().GetName();
	}

	static bool HasBattlegroundUnlock(PlayerBase player)
	{
		string uid = GetUID(player);
		if (uid == "" || !FileExist(DZBGZ_UNLOCKS_JSON))
			return false;

		ref DZBGZ_UnlockStore store = new DZBGZ_UnlockStore;
		string errorMessage;
		if (!JsonFileLoader<ref DZBGZ_UnlockStore>.LoadFile(DZBGZ_UNLOCKS_JSON, store, errorMessage))
		{
			Warn("BattlegroundZ unlock file could not be read: " + errorMessage);
			return false;
		}

		if (!store || !store.Entries)
			return false;

		foreach (DZBGZ_UnlockEntry entry: store.Entries)
		{
			if (entry && entry.UID == uid)
				return true;
		}
		return false;
	}

	static EntityAI FindItemNear(vector center, float radius, string className)
	{
		if (!GetGame() || className == "")
			return null;

		ref array<PlayerBase> players = DZBBC_PlayerUtils.GetOnlinePlayers();
		foreach (PlayerBase player: players)
		{
			if (!IsValidPlayer(player) || vector.Distance(player.GetPosition(), center) > radius)
				continue;

			EntityAI inventoryItem = DZBBC_PlayerUtils.FindInventoryItem(player, className);
			if (inventoryItem)
				return inventoryItem;
		}

		array<Object> objects = new array<Object>;
		array<CargoBase> proxies = new array<CargoBase>;
		GetGame().GetObjectsAtPosition3D(center, radius, objects, proxies);
		foreach (Object object: objects)
		{
			EntityAI entity = EntityAI.Cast(object);
			if (entity && entity.GetType() == className)
				return entity;
		}
		return null;
	}

	static EntityAI FindGasFlareNear(vector center, float radius)
	{
		EntityAI flare = FindItemNear(center, radius, "GasZonen_Leuchtfackel");
		if (!flare)
			flare = FindItemNear(center, radius, "DZBBC_GasZoneFlare");
		return flare;
	}

	static bool HasStartItems(vector center, float radius, DZECZ_Config config)
	{
		if (!config)
			return false;
		if (config.RequiresFireworksLauncher && !FindItemNear(center, radius, "DeutschZ_FireworksLauncher"))
			return false;
		if (config.RequiresGasZoneFlare && !FindGasFlareNear(center, radius))
			return false;
		if (config.RequiresSignalMatches && !FindItemNear(center, radius, "DZBBC_SignalMatches"))
			return false;
		if (config.RequiresCodedBattery && !FindItemNear(center, radius, "DZBBC_Coded9VBattery"))
			return false;
		return true;
	}

	static bool ConsumeItem(EntityAI item)
	{
		if (!item || !GetGame())
			return false;
		string typeName = item.GetType();
		GetGame().ObjectDelete(item);
		Log("Consumed exact story item: " + typeName);
		return true;
	}

	static void DamageLauncher(EntityAI launcher)
	{
		if (!launcher)
			return;
		float health = launcher.GetHealth01("", "Health") - 0.25;
		if (health < 0.10)
			health = 0.10;
		launcher.SetHealth01("", "Health", health);
		Log("Fireworkslauncher condition reduced to " + health.ToString());
	}

	static void Send(PlayerBase player, string message, float seconds = 8.0)
	{
		if (player)
			DZBBC_ServerRPC.SendNotification(player, "Operation EclipseZ", message, seconds);
	}

	static void Broadcast(string message, float seconds = 10.0)
	{
		DZBBC_ServerRPC.BroadcastNotification("Operation EclipseZ", message, seconds);
	}

	static vector SectorCenter(vector pos, float sectorSize)
	{
		if (sectorSize < 100.0)
			sectorSize = 1000.0;
		float x = Math.Floor(pos[0] / sectorSize) * sectorSize + sectorSize * 0.5;
		float z = Math.Floor(pos[2] / sectorSize) * sectorSize + sectorSize * 0.5;
		return Grounded(Vector(x, 0, z));
	}

	static void ShowGlobalSector(string text, vector pos, float sectorSize)
	{
		DZBBC_MarkerData marker = new DZBBC_MarkerData;
		marker.UID = DZECZ_MARKER_UID;
		marker.Text = text;
		marker.Pos = SectorCenter(pos, sectorSize);
		marker.Color = ARGB(255, 160, 20, 20);
		if (!DZBBC_ExpansionBridge.UpsertServerMarker(marker))
			DZBBC_ServerRPC.BroadcastMarker(marker);
	}

	static void RemoveGlobalSector()
	{
		DZBBC_ExpansionBridge.RemoveServerMarker(DZECZ_MARKER_UID);
		DZBBC_MarkerData marker = new DZBBC_MarkerData;
		marker.UID = DZECZ_MARKER_UID;
		marker.Remove = true;
		DZBBC_ServerRPC.BroadcastMarker(marker);
	}
}
