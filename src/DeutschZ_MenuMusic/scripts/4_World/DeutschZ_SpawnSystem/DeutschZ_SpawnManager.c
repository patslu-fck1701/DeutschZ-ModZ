class DeutschZ_SpawnManager
{
	protected static ref DeutschZ_SpawnManager s_Instance;
	protected ref DeutschZ_SpawnSettings m_Settings;
	protected ref DeutschZ_SpawnPointsFile m_Points;
	protected ref DeutschZ_SpawnLoadoutsFile m_Loadouts;
	protected ref map<string, string> m_PendingModes;
	protected ref map<string, int> m_LastRequests;
	protected bool m_Initialized;

	static DeutschZ_SpawnManager GetInstance()
	{
		if (!s_Instance)
			s_Instance = new DeutschZ_SpawnManager;
		return s_Instance;
	}

	static void DestroyInstance()
	{
		s_Instance = null;
	}

	void DeutschZ_SpawnManager()
	{
		m_PendingModes = new map<string, string>;
		m_LastRequests = new map<string, int>;
	}

	void Init()
	{
		if (m_Initialized || !GetGame() || !GetGame().IsServer())
			return;

		m_Settings = DeutschZ_SpawnSettings.Load();
		m_Points = LoadSpawnPoints();
		m_Loadouts = DeutschZ_SpawnLoadoutsFile.Load();
		m_Initialized = true;
		Print("[DeutschZ SpawnSystem] Loaded " + m_Points.SpawnPoints.Count().ToString() + " spawnpoints");
		RunSelfTest();
	}

	protected void RunSelfTest()
	{
		if (!m_Settings || !m_Settings.DebugLogs)
			return;
		vector testPosition;
		string testName;
		if (TryGetRandomValidSpawn(DZSPAWN_Mode.SAFE_RANDOM, testPosition, testName))
			Print("[DeutschZ SpawnSystem] Self-test OK: " + testName + " at " + testPosition.ToString());
		else
			Print("[DeutschZ SpawnSystem][ERROR] Self-test failed: no valid SAFE_RANDOM spawn");
	}

	DeutschZ_SpawnSettings GetSettings()
	{
		Init();
		return m_Settings;
	}

	void HandleRequest(PlayerBase player, PlayerIdentity sender, string mode)
	{
		Init();
		if (!player || !sender || !m_Settings || !m_Settings.EnableSpawnSystem)
			return;

		PlayerIdentity playerIdentity = player.GetIdentity();
		if (!playerIdentity || playerIdentity.GetId() != sender.GetId())
		{
			Deny(player, sender, "identity mismatch", 0);
			return;
		}

		if (player.IsAlive())
		{
			Deny(player, sender, "player alive", 0);
			return;
		}

		if (!DZSPAWN_Mode.IsKnown(mode) || !m_Settings.SpawnModes || !m_Settings.SpawnModes.IsEnabled(mode))
		{
			Deny(player, sender, "invalid or disabled mode", 0);
			return;
		}

		if (mode == DZSPAWN_Mode.DISABLED_KEEP_GEAR || !m_Settings.AllowKeepGear && mode == DZSPAWN_Mode.DISABLED_KEEP_GEAR)
		{
			Deny(player, sender, "keep gear disabled", 0);
			return;
		}

		string uid = sender.GetId();
		int now = GetGame().GetTime();
		int previous;
		if (m_LastRequests.Find(uid, previous))
		{
			int remainingMs = m_Settings.RespawnCooldownSeconds * 1000 - (now - previous);
			if (remainingMs > 0)
			{
				Deny(player, sender, "cooldown", Math.Ceil(remainingMs / 1000.0));
				return;
			}
		}

		SetPendingSpawnMode(uid, mode);
		m_LastRequests.Set(uid, now);
		Print("[DeutschZ SpawnSystem] Player " + uid + " requested mode " + mode);
		player.RPCSingleParam(DZSPAWN_RPC.RESPAWN_ACCEPTED, new Param1<string>(mode), true, sender);
	}

	protected void Deny(PlayerBase player, PlayerIdentity identity, string reason, int remainingSeconds)
	{
		Print("[DeutschZ SpawnSystem] Denied request: " + reason);
		if (player && identity)
			player.RPCSingleParam(DZSPAWN_RPC.RESPAWN_DENIED, new Param2<string, int>(reason, remainingSeconds), true, identity);
	}

	void SetPendingSpawnMode(string uid, string mode)
	{
		if (uid != "" && DZSPAWN_Mode.IsKnown(mode))
			m_PendingModes.Set(uid, mode);
	}

	string ConsumePendingSpawnMode(string uid)
	{
		Init();
		string mode;
		if (uid != "" && m_PendingModes.Find(uid, mode))
		{
			m_PendingModes.Remove(uid);
			return mode;
		}

		if (m_Settings)
			return m_Settings.DefaultSpawnMode;
		return DZSPAWN_Mode.SAFE_RANDOM;
	}

	vector GetRandomValidSpawn(string mode)
	{
		vector position;
		string name;
		if (TryGetRandomValidSpawn(mode, position, name))
			return position;
		return "0 0 0";
	}

	bool TryGetRandomValidSpawn(string mode, out vector position, out string pointName)
	{
		Init();
		position = "0 0 0";
		pointName = "";
		if (!m_Points || !m_Points.SpawnPoints)
			return false;

		ref array<ref DeutschZ_SpawnPoint> candidates = new array<ref DeutschZ_SpawnPoint>;
		foreach (DeutschZ_SpawnPoint point: m_Points.SpawnPoints)
		{
			if (!point || !point.Enabled)
				continue;
			if (mode == DZSPAWN_Mode.RANDOM || mode == DZSPAWN_Mode.SAFE_RANDOM || point.Mode == mode)
				candidates.Insert(point);
		}

		if (candidates.Count() == 0)
			return false;

		int start = Math.RandomInt(0, candidates.Count());
		for (int i = 0; i < candidates.Count(); i++)
		{
			DeutschZ_SpawnPoint candidate = candidates.Get((start + i) % candidates.Count());
			float radius = candidate.Radius;
			if (radius <= 0.0)
				radius = m_Settings.SpawnSearchRadius;

			for (int attempt = 0; attempt < 8; attempt++)
			{
				vector testPosition = candidate.Position;
				testPosition[0] = testPosition[0] + Math.RandomFloatInclusive(-radius, radius);
				testPosition[2] = testPosition[2] + Math.RandomFloatInclusive(-radius, radius);
				testPosition[1] = GetGame().SurfaceY(testPosition[0], testPosition[2]) + m_Settings.SpawnHeightOffset;
				if (IsSpawnValidForMode(testPosition, mode))
				{
					position = testPosition;
					pointName = candidate.Name;
					return true;
				}
			}
		}

		return false;
	}

	bool IsSpawnValid(vector position)
	{
		return IsSpawnValidForMode(position, DZSPAWN_Mode.SAFE_RANDOM);
	}

	protected bool IsSpawnValidForMode(vector position, string mode)
	{
		if (!GetGame() || !m_Settings)
			return false;

		if (m_Settings.BlockWaterSpawns && (GetGame().SurfaceIsSea(position[0], position[2]) || GetGame().SurfaceIsPond(position[0], position[2])))
			return false;

		if (m_Settings.BlockSteepTerrain)
		{
			vector normal = GetGame().SurfaceGetNormal(position[0], position[2]);
			float slope = Math.Acos(vector.Dot(normal, vector.Up)) * Math.RAD2DEG;
			if (slope > m_Settings.MaxTerrainSlope)
				return false;
		}

		ref array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		foreach (Man man: players)
		{
			PlayerBase other = PlayerBase.Cast(man);
			if (other && other.IsAlive() && vector.Distance(other.GetPosition(), position) < m_Settings.MinDistanceToPlayers)
				return false;
		}

		if (mode != DZSPAWN_Mode.TRADER_NEAR && IsNearAny(position, m_Settings.TraderPositions, m_Settings.MinDistanceToTrader))
			return false;
		if (IsNearAny(position, m_Settings.EventPositions, m_Settings.MinDistanceToEvents))
			return false;
		if (mode != DZSPAWN_Mode.TRADER_NEAR && IsNearAny(position, m_Settings.SafeZonePositions, m_Settings.MinDistanceToSafeZones))
			return false;

		return true;
	}

	protected bool IsNearAny(vector position, array<vector> zones, float minimumDistance)
	{
		if (!zones || minimumDistance <= 0.0)
			return false;
		foreach (vector zone: zones)
		{
			if (vector.Distance(position, zone) < minimumDistance)
				return true;
		}
		return false;
	}

	void ApplySpawn(PlayerBase player, PlayerIdentity identity)
	{
		Init();
		if (!player || !identity || !m_Loadouts)
			return;

		DeutschZ_SpawnLoadout.Apply(player, m_Loadouts, m_Loadouts.DefaultLoadout);
		Print("[DeutschZ SpawnSystem] Spawn completed for " + identity.GetId());
	}

	protected ref DeutschZ_SpawnPointsFile LoadSpawnPoints()
	{
		DeutschZ_SpawnStorage.EnsureDirectories();
		ref DeutschZ_SpawnPointsFile points = CreateExpansionDefaults();
		string error;
		if (!FileExist(DZSPAWN_Paths.POINTS))
		{
			JsonFileLoader<ref DeutschZ_SpawnPointsFile>.SaveFile(DZSPAWN_Paths.POINTS, points, error);
			Print("[DeutschZ SpawnSystem] Created SpawnPoints_Chernarus.json from Expansion spawn locations");
			return points;
		}

		if (!JsonFileLoader<ref DeutschZ_SpawnPointsFile>.LoadFile(DZSPAWN_Paths.POINTS, points, error))
			Print("[DeutschZ SpawnSystem][ERROR] SpawnPoints_Chernarus.json invalid, Expansion defaults active: " + error);
		return points;
	}

	protected ref DeutschZ_SpawnPointsFile CreateExpansionDefaults()
	{
		ref DeutschZ_SpawnPointsFile points = new DeutschZ_SpawnPointsFile;
		AddPoint(points, "Svetloyarsk 1", "14273.200195 2.400000 13053.299805");
		AddPoint(points, "Svetloyarsk 2", "14407.299805 2.000000 13253.000000");
		AddPoint(points, "Svetloyarsk 3", "14142.400391 3.300000 13290.200195");
		AddPoint(points, "Svetloyarsk 4", "13910.900391 4.300000 13624.900391");
		AddPoint(points, "Berezino 1", "12915.700195 3.400000 9278.200195");
		AddPoint(points, "Berezino 2", "13057.200195 2.300000 9584.480469");
		AddPoint(points, "Berezino 3", "13052.900391 6.100000 9894.700195");
		AddPoint(points, "Berezino 4", "13207.200195 2.300000 10193.700195");
		AddPoint(points, "Solnich 1", "13169.500000 3.075610 7504.029785");
		AddPoint(points, "Solnich 2", "13274.000000 1.783500 7258.810059");
		AddPoint(points, "Solnich 3", "13345.599609 1.877930 6987.359863");
		AddPoint(points, "Solnich 4", "13383.000000 2.755160 6815.890137");
		AddPoint(points, "Solnichniy 1", "13529.530273 2.251228 6455.612793");
		AddPoint(points, "Solnichniy 2", "13484.724609 1.746646 5911.094727");
		AddPoint(points, "Solnichniy 3", "13515.912109 2.679648 6117.384277");
		AddPoint(points, "Solnichniy 4", "13534.671875 1.644669 6234.750000");
		AddPoint(points, "Kamyshovo 1", "12321.939453 1.926140 3446.666748");
		AddPoint(points, "Kamyshovo 2", "12188.570313 1.727290 3422.332275");
		AddPoint(points, "Kamyshovo 3", "11992.250000 1.982081 3404.554443");
		AddPoint(points, "Kamyshovo 4", "11859.343750 1.901515 3367.714844");
		AddPoint(points, "Elektrozavodsk 1", "11099.068359 2.297676 2735.562500");
		AddPoint(points, "Elektrozavodsk 2", "10858.413086 2.911721 2328.290283");
		AddPoint(points, "Elektrozavodsk 3", "10490.914063 1.846902 1950.148438");
		AddPoint(points, "Elektrozavodsk 4", "9826.885742 1.711821 1757.374634");
		AddPoint(points, "Elektrozavodsk 5", "9428.458008 2.254453 1826.218506");
		AddPoint(points, "Elektrozavodsk 6", "9153.536133 3.421117 1914.300659");
		AddPoint(points, "Chernogorsk 1", "6044.103027 6.429995 1871.500610");
		AddPoint(points, "Chernogorsk 2", "6220.440430 1.917814 2101.123291");
		AddPoint(points, "Chernogorsk 3", "7118.122070 1.824183 2533.971924");
		AddPoint(points, "Chernogorsk 4", "7419.497070 1.768386 2576.503906");
		AddPoint(points, "Chernogorsk 5", "8139.250000 1.151711 2802.356445");
		AddPoint(points, "Balota 1", "4654.594238 1.459718 2132.866699");
		AddPoint(points, "Balota 2", "4543.990723 1.901639 2198.166260");
		AddPoint(points, "Balota 3", "4269.422852 1.289235 2245.660889");
		AddPoint(points, "Balota 4", "4111.905762 1.566264 2193.932617");
		AddPoint(points, "Komarovo 1", "3887.559082 1.595509 2207.158936");
		AddPoint(points, "Komarovo 2", "3746.655762 2.445386 2199.878174");
		AddPoint(points, "Komarovo 3", "3507.422852 2.008609 2101.454590");
		AddPoint(points, "Komarovo 4", "3366.985352 1.902521 2002.414063");
		AddPoint(points, "Kamenka 1", "2164.700195 1.728014 2049.443848");
		AddPoint(points, "Kamenka 2", "2031.425415 1.290743 2150.743408");
		AddPoint(points, "Kamenka 3", "1708.523071 1.958309 2031.263672");
		AddPoint(points, "Kamenka 4", "1563.325684 2.174132 2063.254883");
		return points;
	}

	protected void AddPoint(DeutschZ_SpawnPointsFile points, string name, vector position)
	{
		points.SpawnPoints.Insert(DeutschZ_SpawnPoint.Create(name, DZSPAWN_Mode.COAST, position));
	}
}

modded class PlayerBase
{
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);

		if (rpc_type == DZSPAWN_RPC.REQUEST_RESPAWN_MODE && GetGame().IsServer())
		{
			Param1<string> request;
			if (!ctx.Read(request) || !request)
				return;
			DeutschZ_SpawnManager.GetInstance().HandleRequest(this, sender, request.param1);
			return;
		}

		if (rpc_type == DZSPAWN_RPC.RESPAWN_ACCEPTED && GetGame().IsClient())
		{
			Param1<string> accepted;
			if (!ctx.Read(accepted) || !accepted)
				return;
			DZSPAWN_ClientState.SetAccepted(accepted.param1);
			return;
		}

		if (rpc_type == DZSPAWN_RPC.RESPAWN_DENIED && GetGame().IsClient())
		{
			Param2<string, int> denied;
			if (!ctx.Read(denied) || !denied)
				return;
			DZSPAWN_ClientState.SetDenied(denied.param1, denied.param2);
		}
	}
}
