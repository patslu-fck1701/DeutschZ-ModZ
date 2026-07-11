modded class MissionServer
{
	override void OnInit()
	{
		super.OnInit();
		DeutschZ_SpawnManager.GetInstance().Init();
	}

	override PlayerBase CreateCharacter(PlayerIdentity identity, vector pos, ParamsReadContext ctx, string characterName)
	{
		DeutschZ_SpawnManager manager = DeutschZ_SpawnManager.GetInstance();
		string mode = manager.ConsumePendingSpawnMode(identity.GetId());
		vector selectedPosition;
		string selectedName;
		bool customSpawn = manager.TryGetRandomValidSpawn(mode, selectedPosition, selectedName);
		if (customSpawn)
		{
			pos = selectedPosition;
			Print("[DeutschZ SpawnSystem] Selected spawn " + selectedName + " at " + pos.ToString());
		}
		else
		{
			Print("[DeutschZ SpawnSystem][WARN] No valid spawn for " + mode + ", vanilla position active");
		}

		PlayerBase player = super.CreateCharacter(identity, pos, ctx, characterName);
		if (player && customSpawn)
		{
			player.SetOrientation(Vector(Math.RandomFloatInclusive(0.0, 360.0), 0.0, 0.0));
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(DZSPAWN_ApplyLoadout, 750, false, player, identity);
		}
		return player;
	}

	protected void DZSPAWN_ApplyLoadout(PlayerBase player, PlayerIdentity identity)
	{
		DeutschZ_SpawnManager.GetInstance().ApplySpawn(player, identity);
	}

	override void OnMissionFinish()
	{
		DeutschZ_SpawnManager.DestroyInstance();
		super.OnMissionFinish();
	}
}
