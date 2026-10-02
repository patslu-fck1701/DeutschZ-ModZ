class DZRB_DefaultRoute
{
	static const string FILE_NAME = "chernarus_north.json";

	static bool Ensure()
	{
		TStringArray existing;
		DZRB_RouteFiles.List(existing);
		if (existing.Count() > 0) return true;

		string worldName = GetGame().GetWorldName();
		worldName.ToLower();
		if (worldName != "chernarusplus")
		{
			Print("[DeutschZ_RBM] DEFAULT_ROUTE_SKIPPED world=" + worldName);
			return false;
		}

		ExpansionWorld expansionWorld;
		Class.CastTo(expansionWorld, GetDayZGame().GetExpansionGame());
		eAIRoadNetwork roadNetwork;
		if (expansionWorld) roadNetwork = expansionWorld.GetRoadNetwork();
		if (!roadNetwork)
		{
			Print("[DeutschZ_RBM] DEFAULT_ROUTE_ERROR road_network_unavailable");
			return false;
		}
		roadNetwork.Init();

		array<string> stopNames = {"Sinystok", "Topolniki", "Novaya_Petrovka", "Zaprudnoye", "Severograd", "Svergino", "Novodmitrovsk", "Svetloyarsk"};
		array<vector> anchors = {
			"1511 0 11970", "2800 0 12300", "3470 0 13060", "5013 0 12806",
			"7900 0 12600", "9510 0 14568", "11500 0 14400", "14000 0 13200"
		};

		DZRB_Route route = new DZRB_Route();
		route.Version = 1;
		route.RouteID = "Chernarus_Nordroute";
		route.Enabled = true;
		route.Loop = true;

		array<int> stopIndices = new array<int>();
		for (int segment = 0; segment < anchors.Count() - 1; segment++)
		{
			if (segment == 0) stopIndices.Insert(route.Waypoints.Count());
			if (!AppendRoadSection(roadNetwork, anchors[segment], anchors[segment + 1], route.Waypoints)) return false;
			stopIndices.Insert(route.Waypoints.Count() - 1);
		}

		// Return on the road network so Loop never creates an off-road shortcut.
		if (!AppendRoadSection(roadNetwork, anchors[anchors.Count() - 1], anchors[0], route.Waypoints)) return false;

		for (int i = 0; i < stopNames.Count(); i++)
			AddStop(route, stopNames[i], stopIndices[i]);

		string error;
		JsonFileLoader<DZRB_Route>.SaveFile(DZRB_MobileSettings.ROUTES_DIR + "/" + FILE_NAME, route, error);
		if (error != "")
		{
			Print("[DeutschZ_RBM] DEFAULT_ROUTE_CREATE_ERROR " + error);
			return false;
		}
		Print(string.Format("[DeutschZ_RBM] DEFAULT_ROUTE_CREATED waypoints=%1 stops=%2", route.Waypoints.Count(), route.TradeStops.Count()));
		return true;
	}

	private static bool AppendRoadSection(eAIRoadNetwork network, vector from, vector to, array<vector> output)
	{
		eAIRoadNode start = network.GetClosestNode(from);
		eAIRoadNode goal = network.GetClosestNode(to);
		if (!start || !goal)
		{
			Print("[DeutschZ_RBM] DEFAULT_ROUTE_ERROR road_node_missing");
			return false;
		}

		PGFilter filter = new PGFilter();
		array<eAIRoadNode> reversePath = new array<eAIRoadNode>();
		ExpansionAStar<eAIRoadNode>.Perform(start, goal, filter, reversePath);
		if (reversePath.Count() < 2)
		{
			Print("[DeutschZ_RBM] DEFAULT_ROUTE_ERROR road_path_missing from=" + from + " to=" + to);
			return false;
		}

		for (int i = reversePath.Count() - 1; i >= 0; i--)
		{
			vector point = reversePath[i].m_Position;
			if (output.Count() == 0 || vector.Distance(output[output.Count() - 1], point) > 2.0)
				output.Insert(point);
		}
		return true;
	}

	private static void AddStop(DZRB_Route route, string name, int waypointIndex)
	{
		vector parking = route.Waypoints[waypointIndex];
		vector direction = "0 0 1";
		if (waypointIndex + 1 < route.Waypoints.Count())
			direction = vector.Direction(parking, route.Waypoints[waypointIndex + 1]);
		else if (waypointIndex > 0)
			direction = vector.Direction(route.Waypoints[waypointIndex - 1], parking);
		float yaw = direction.VectorToAngles()[0];
		vector right = Vector(direction[2], 0, -direction[0]);
		right.Normalize();

		DZRB_TradeStop stop = new DZRB_TradeStop();
		stop.StopID = name;
		stop.WaypointIndex = waypointIndex;
		stop.ParkingPosition = parking;
		stop.ParkingOrientation = Vector(yaw, 0, 0);
		stop.TraderPosition = parking + right * 4.0;
		stop.StopDurationSeconds = 900;
		stop.Notification = "Der mobile Schwarzmarkt ist bei " + name + " geoeffnet.";
		route.TradeStops.Insert(stop);
	}
}
