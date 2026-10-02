class DZRB_MobileManager
{
	private static ref DZRB_MobileManager s_Instance;
	private ref DZRB_MobileSettings m_Settings;
	private ref array<ref DZRB_Route> m_Routes;
	private DZRB_Route m_Route;
	private ref DZRB_RuntimeState m_State;
	private CarScript m_Vehicle;
	private eAIBase m_TravelTrader;
	private eAIGroup m_TravelTraderGroup;
	private ref DZRB_Escort m_Escort;
	private int m_OpenUntil;
	private float m_StuckTime;
	private int m_StuckStrikes;
	private int m_StationaryChecks;
	private int m_RecoveryCooldownUntil;
	private vector m_LastMovementPosition;
	private int m_LastSaveTime;
	private int m_LastMarkerTime;
	private ref array<string> m_GreetedPlayers = new array<string>();
	private ref array<int> m_ActiveStopIndices = new array<int>();

	static DZRB_MobileManager GetInstance()
	{
		if (!s_Instance) s_Instance = new DZRB_MobileManager();
		return s_Instance;
	}

	void Initialize()
	{
		if (!GetGame().IsServer()) return;
		m_Settings = DZRB_MobileSettings.Load();
		if (!m_Settings.Enabled) return;
		m_Routes = new array<ref DZRB_Route>();
		m_State = LoadState();
		DZRB_DefaultRoute.Ensure();
		LoadRoutes();
		if (!m_Settings.Enabled)
		{
			Log("SYSTEM_DISABLED");
			return;
		}
		m_Route = SelectRoute(m_State.RouteID);
		if (!m_Route)
		{
			m_State.CurrentState = DZRB_MobileState.DZRB_INACTIVE;
			SaveState();
			Log("WAITING_FOR_ROUTE path=" + DZRB_MobileSettings.ROUTES_DIR);
			return;
		}
		StartRoute();
	}

	private void LoadRoutes()
	{
		TStringArray files;
		DZRB_RouteFiles.List(files);
		foreach (string fileName : files)
		{
			DZRB_Route route = new DZRB_Route();
			string error;
			if (!JsonFileLoader<DZRB_Route>.LoadFile(DZRB_MobileSettings.ROUTES_DIR + "/" + fileName, route, error))
			{
				Log("ROUTE_LOAD_ERROR file=" + fileName + " error=" + error);
				continue;
			}
			if (route.RouteID == "") route.RouteID = fileName;
			if (!ValidateRoute(route)) continue;
			m_Routes.Insert(route);
			Log(string.Format("ROUTE_READY id=%1 waypoints=%2 stops=%3", route.RouteID, route.Waypoints.Count(), route.TradeStops.Count()));
		}
	}

	private bool ValidateRoute(DZRB_Route route)
	{
		if (!route || !route.Enabled || !route.Waypoints || !route.TradeStops || route.Waypoints.Count() < 2) return false;
		int wideGaps = 0;
		float largestGap = 0;
		for (int i = 1; i < route.Waypoints.Count(); i++)
		{
			float gap = vector.Distance(route.Waypoints[i - 1], route.Waypoints[i]);
			if (gap > 40.0)
			{
				wideGaps++;
				largestGap = Math.Max(largestGap, gap);
			}
		}
		if (wideGaps > 0) Log(string.Format("ROUTE_GAPS id=%1 count=%2 largest=%3m", route.RouteID, wideGaps, largestGap));
		foreach (DZRB_TradeStop stop : route.TradeStops)
		{
			if (!stop || stop.StopID == "" || stop.WaypointIndex < 0 || stop.WaypointIndex >= route.Waypoints.Count())
			{
				Log("ROUTE_INVALID_STOP id=" + route.RouteID);
				return false;
			}
		}
		return true;
	}

	private DZRB_Route SelectRoute(string preferredID)
	{
		foreach (DZRB_Route configured : m_Routes)
			if (configured.RouteID == m_Settings.PreferredRouteID) return configured;
		foreach (DZRB_Route preferred : m_Routes)
			if (preferred.RouteID == preferredID) return preferred;
		if (m_Routes.Count() > 0) return m_Routes[0];
		return NULL;
	}

	private void StartRoute()
	{
		SelectStopsForLoop();
		bool freshRoute = m_State.RouteID == "" || m_State.RouteID != m_Route.RouteID;
		if (freshRoute)
			m_State = new DZRB_RuntimeState();
		if (freshRoute && m_Settings.RandomStartAtTradeStop && m_Route.TradeStops.Count() > 0)
		{
			int selectedStop = Math.RandomInt(0, m_Route.TradeStops.Count());
			m_State.WaypointIndex = m_Route.TradeStops[selectedStop].WaypointIndex;
			Log("RANDOM_START stop=" + m_Route.TradeStops[selectedStop].StopID);
		}
		m_State.RouteID = m_Route.RouteID;
		if (m_State.WaypointIndex < 0 || m_State.WaypointIndex >= m_Route.Waypoints.Count()) m_State.WaypointIndex = 0;
		vector spawnPosition = m_Route.Waypoints[m_State.WaypointIndex];
		if (!freshRoute && m_Settings.ResumeAtLastSafeWaypoint && m_State.LastValidPosition != vector.Zero)
		{
			spawnPosition = m_State.LastValidPosition;
			Log("ROUTE_RESUME waypoint=" + m_State.WaypointIndex.ToString());
		}
		spawnPosition[1] = GetGame().SurfaceRoadY(spawnPosition[0], spawnPosition[2]) + 0.25;
		m_Vehicle = CarScript.Cast(GetGame().CreateObjectEx(m_Settings.VehicleClassName, spawnPosition, ECE_CREATEPHYSICS | ECE_PLACE_ON_SURFACE | ECE_NOPERSISTENCY_WORLD));
		if (!m_Vehicle)
		{
			Log("VEHICLE_SPAWN_FAILED class=" + m_Settings.VehicleClassName);
			return;
		}
		foreach (string attachment : m_Settings.VehicleAttachments)
		{
			EntityAI mountedPart = m_Vehicle.GetInventory().CreateAttachment(attachment);
			if (attachment != "" && !mountedPart)
			{
				Log("VEHICLE_ATTACHMENT_FAILED class=" + attachment);
				GetGame().ObjectDelete(m_Vehicle);
				m_Vehicle = NULL;
				return;
			}
			if (mountedPart) mountedPart.SetAllowDamage(false);
		}
		m_Vehicle.Fill(CarFluid.FUEL, m_Vehicle.GetFluidCapacity(CarFluid.FUEL));
		m_Vehicle.Fill(CarFluid.OIL, m_Vehicle.GetFluidCapacity(CarFluid.OIL));
		m_Vehicle.Fill(CarFluid.BRAKE, m_Vehicle.GetFluidCapacity(CarFluid.BRAKE));
		m_Vehicle.Fill(CarFluid.COOLANT, m_Vehicle.GetFluidCapacity(CarFluid.COOLANT));
		m_Vehicle.DZRB_ConfigureMobile(m_Settings.VehicleInvulnerable, m_Settings.PreventCargoAccess, m_Settings.PreventPartRemoval, m_Settings.PreventVehicleEntry);
		m_Vehicle.SetAllowDamage(false);
		FaceNextWaypoint();
		SpawnTravelTrader();
		m_Escort = new DZRB_Escort();
		if (!m_Escort.Spawn(m_Vehicle, m_Settings))
		{
			Log("CONVOY_SPAWN_ABORTED escort_failed");
			GetGame().ObjectDelete(m_Vehicle);
			m_Vehicle = null;
			return;
		}
		m_Vehicle.DZRB_PrimeEngine();
		m_State.CurrentState = DZRB_MobileState.DZRB_TRAVEL;
		DZRB_StatusBridge.Set("UNTERWEGS");
		SaveState();
		Log("VEHICLE_SPAWNED class=" + m_Settings.VehicleClassName + " route=" + m_Route.RouteID);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Tick, m_Settings.RouteTickMilliseconds, true);
	}

	private void Tick()
	{
		if (!m_Vehicle || !m_Route) return;
		bool parked = m_State.CurrentState == DZRB_MobileState.DZRB_OPEN || m_State.CurrentState == DZRB_MobileState.DZRB_CLOSING;
		if (m_State.CurrentState == DZRB_MobileState.DZRB_OPEN && GetGame().GetTime() >= m_OpenUntil)
		{
			CloseStop();
			parked = false;
		}
		if (m_Escort && !m_Escort.Update(parked))
		{
			m_Vehicle.DZRB_StopDriving(false);
			return;
		}
		if (m_State.CurrentState == DZRB_MobileState.DZRB_OPEN)
		{
			if (GetGame().GetTime() >= m_OpenUntil) CloseStop();
			else ProcessGreetings();
			PeriodicWork();
			return;
		}

		if (m_State.CurrentState != DZRB_MobileState.DZRB_TRAVEL && m_State.CurrentState != DZRB_MobileState.DZRB_APPROACHING_STOP && m_State.CurrentState != DZRB_MobileState.DZRB_DEPARTING && m_State.CurrentState != DZRB_MobileState.DZRB_STUCK_RECOVERY) return;
		vector target = m_Route.Waypoints[m_State.WaypointIndex];
		vector horizontalDelta = target - m_Vehicle.GetPosition();
		horizontalDelta[1] = 0;
		float distance = horizontalDelta.Length();
		DZRB_TradeStop stop = GetStopAtWaypoint(m_State.WaypointIndex);
		float speed = m_Settings.CruiseSpeedKmh;
		if (stop && distance <= m_Settings.ApproachDistanceMeters)
		{
			m_State.CurrentState = DZRB_MobileState.DZRB_APPROACHING_STOP;
			speed = m_Settings.ApproachSpeedKmh;
		}
		m_Vehicle.DZRB_SetDriveTarget(target, speed);
		m_Vehicle.DZRB_UpdateDrive(m_Settings.RouteTickMilliseconds / 1000.0);

		if (distance <= m_Settings.WaypointReachedMeters)
		{
			m_State.LastValidPosition = target;
			m_StuckStrikes = 0;
			if (stop)
			{
				OpenStop(stop);
				return;
			}
			AdvanceWaypoint();
			if (m_State.CurrentState == DZRB_MobileState.DZRB_ROUTE_FINISHED) return;
		}
		UpdateStuck();
		PeriodicWork();
	}

	private void AdvanceWaypoint()
	{
		m_State.WaypointIndex++;
		if (m_State.WaypointIndex < m_Route.Waypoints.Count()) return;
		if (m_Route.Loop)
		{
			m_State.WaypointIndex = 0;
			m_State.CurrentState = DZRB_MobileState.DZRB_TRAVEL;
			SelectStopsForLoop();
			Log("ROUTE_LOOP id=" + m_Route.RouteID);
			return;
		}
		m_State.CurrentState = DZRB_MobileState.DZRB_ROUTE_FINISHED;
		m_Vehicle.DZRB_StopDriving(true);
		SaveState();
		Log("ROUTE_FINISHED id=" + m_Route.RouteID);
	}

	private DZRB_TradeStop GetStopAtWaypoint(int waypointIndex)
	{
		if (m_ActiveStopIndices.Find(waypointIndex) < 0) return NULL;
		foreach (DZRB_TradeStop stop : m_Route.TradeStops)
			if (stop && stop.WaypointIndex == waypointIndex) return stop;
		return NULL;
	}

	private void SelectStopsForLoop()
	{
		m_ActiveStopIndices.Clear();
		if (!m_Route || !m_Route.TradeStops || m_Route.TradeStops.Count() == 0) return;
		int target = Math.Clamp(m_Settings.RandomStopsPerLoop, 1, m_Route.TradeStops.Count());
		ref array<int> candidates = new array<int>();
		for (int i = 0; i < m_Route.TradeStops.Count(); i++) candidates.Insert(i);
		while (candidates.Count() > 0 && m_ActiveStopIndices.Count() < target)
		{
			int pick = Math.RandomInt(0, candidates.Count());
			int stopArrayIndex = candidates[pick];
			candidates.RemoveOrdered(pick);
			DZRB_TradeStop candidate = m_Route.TradeStops[stopArrayIndex];
			if (!candidate) continue;
			bool separated = true;
			foreach (int selectedIndex : m_ActiveStopIndices)
			{
				int gap = Math.AbsInt(candidate.WaypointIndex - selectedIndex);
				int wrapGap = m_Route.Waypoints.Count() - gap;
				if (Math.Min(gap, wrapGap) < m_Settings.MinStopWaypointDistance) separated = false;
			}
			if (separated) m_ActiveStopIndices.Insert(candidate.WaypointIndex);
		}
		Log(string.Format("RANDOM_STOPS_SELECTED count=%1 target=%2", m_ActiveStopIndices.Count(), target));
	}

	private void OpenStop(DZRB_TradeStop stop)
	{
		Log("STOP_REACHED waypoint=" + m_State.WaypointIndex.ToString() + " position=" + m_Vehicle.GetPosition().ToString());
		m_State.CurrentState = DZRB_MobileState.DZRB_PARKING;
		m_Vehicle.DZRB_StopDriving(true);
		vector parking = stop.ParkingPosition;
		parking[1] = GetGame().SurfaceRoadY(parking[0], parking[2]) + 0.25;
		m_Vehicle.SetPosition(parking);
		m_Vehicle.SetOrientation(stop.ParkingOrientation);
		SetVelocity(m_Vehicle, vector.Zero);
		Log("TRUCK_PARKED position=" + m_Vehicle.GetPosition().ToString() + " trader=" + stop.TraderPosition.ToString());
		RemoveTravelTrader();
		m_State.CurrentStopID = stop.StopID;
		if (!DZRB_ExpansionTraderService.OpenAt(stop.TraderPosition, stop.ParkingOrientation, m_Settings, stop.StopID))
		{
			m_State.CurrentState = DZRB_MobileState.DZRB_CLOSING;
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CloseStop, 5000, false);
			return;
		}
		m_State.CurrentState = DZRB_MobileState.DZRB_OPEN;
		DZRB_StatusBridge.Set("HANDELSBEREIT");
		m_OpenUntil = GetGame().GetTime() + Math.Max(stop.StopDurationSeconds, 60) * 1000;
		vector actualTraderPosition = DZRB_ExpansionTraderService.Position();
		Log("TRADER_READY position=" + actualTraderPosition.ToString());
		if (m_Settings.MarkerMode != "OFF" && DZRB_MarkerService.CreateAtPosition(actualTraderPosition, true)) Log("MARKER_CREATED position=" + actualTraderPosition.ToString());
		DZRB_MarkerService.BroadcastNotification("SCHWARZMARKT GEOEFFNET", "Der Schwarzmarkt hat einen neuen Standort erreicht und ist jetzt geoeffnet.");
		Log("NOTIFICATION_OPEN_SENT");
		SaveState();
		Log("MARKET_OPEN stop=" + stop.StopID);
	}

	private void CloseStop()
	{
		if (!m_Vehicle) return;
		m_State.CurrentState = DZRB_MobileState.DZRB_CLOSING;
		Log("DEPARTING waypoint=" + m_State.WaypointIndex.ToString());
		DZRB_ExpansionTraderService.Close();
		Log("MARKER_REMOVED");
		m_GreetedPlayers.Clear();
		m_State.LastCompletedStop = m_State.CurrentStopID;
		Log("MARKET_CLOSE stop=" + m_State.CurrentStopID);
		m_State.CurrentStopID = "";
		AdvanceWaypoint();
		if (m_State.CurrentState == DZRB_MobileState.DZRB_ROUTE_FINISHED) return;
		m_State.CurrentState = DZRB_MobileState.DZRB_DEPARTING;
		DZRB_StatusBridge.Set("UNTERWEGS");
		SpawnTravelTrader();
		m_Vehicle.DZRB_PrimeEngine();
		DZRB_MarkerService.BroadcastNotification("SCHWARZMARKT UNTERWEGS", "Der Schwarzmarkt schliesst und zieht weiter.");
		Log("NOTIFICATION_DEPART_SENT");
		SaveState();
	}

	private void SpawnTravelTrader()
	{
		if (!m_Vehicle || m_TravelTrader) return;
		vector position = m_Vehicle.GetPosition() + m_Vehicle.GetDirection() * 3;
		position[1] = GetGame().SurfaceY(position[0], position[2]);
		m_TravelTrader = eAIBase.Cast(GetGame().CreateObjectEx("eAI_SurvivorF_Linda", position, ECE_PLACE_ON_SURFACE | ECE_NOLIFETIME));
		if (!m_TravelTrader)
		{
			Log("TRAVEL_TRADER_SPAWN_FAILED");
			return;
		}
		m_TravelTrader.SetAllowDamage(false);
		m_TravelTraderGroup = eAIGroup.CreateGroup(new eAIFactionPassive());
		m_TravelTrader.SetGroup(m_TravelTraderGroup);
		foreach (string clothing : m_Settings.TraderOutfit)
			if (clothing != "" && !m_TravelTrader.GetInventory().CreateAttachment(clothing)) Log("TRAVEL_TRADER_OUTFIT_FAILED class=" + clothing);
		m_TravelTrader.Notify_Transport(m_Vehicle, 1);
		Log("TRAVEL_TRADER_VISIBLE seat=1");
	}

	private void RemoveTravelTrader()
	{
		if (m_TravelTrader) GetGame().ObjectDelete(m_TravelTrader);
		m_TravelTrader = null;
		m_TravelTraderGroup = null;
	}

	private void ProcessGreetings()
	{
		DZRB_TradeStop stop = GetStopAtWaypoint(m_State.WaypointIndex);
		if (!stop) return;
		ref array<Man> players = new array<Man>();
		GetGame().GetPlayers(players);
		foreach (Man man : players)
		{
			PlayerBase player = PlayerBase.Cast(man);
			if (!player || !player.IsAlive() || !player.GetIdentity()) continue;
			string id = player.GetIdentity().GetId();
			vector delta = player.GetPosition() - stop.TraderPosition;
			delta[1] = 0;
			float distance = delta.Length();
			int seen = m_GreetedPlayers.Find(id);
			if (distance > 12.0)
			{
				if (seen >= 0) m_GreetedPlayers.RemoveOrdered(seen);
				continue;
			}
			if (distance <= 10.0 && seen < 0)
			{
				m_GreetedPlayers.Insert(id);
				int variant = Math.RandomInt(0, 3);
				GetGame().RPCSingleParam(player, DZRB_AudioData.RPC_GREETING, new Param1<int>(variant), true, player.GetIdentity());
				Log("GREETING_PLAYED player=" + id + " variant=" + variant);
			}
		}
	}

	private void UpdateStuck()
	{
		int now = GetGame().GetTime();
		if (now < m_RecoveryCooldownUntil) return;
		vector currentPosition = m_Vehicle.GetPosition();
		if (m_LastMovementPosition == vector.Zero) m_LastMovementPosition = currentPosition;
		if (vector.Distance(currentPosition, m_LastMovementPosition) >= 2.0)
		{
			m_LastMovementPosition = currentPosition;
			m_StuckTime = 0;
			m_StationaryChecks = 0;
			return;
		}
		if (!m_Vehicle.EngineIsOn() || Math.AbsFloat(m_Vehicle.GetSpeedometer()) >= m_Settings.StuckSpeedKmh)
		{
			m_StuckTime = 0;
			return;
		}
		m_StuckTime += m_Settings.RouteTickMilliseconds / 1000.0;
		if (m_StuckTime < m_Settings.StuckSeconds) return;
		m_StuckTime = 0;
		m_StationaryChecks++;
		if (m_StationaryChecks < 3) return;
		m_StationaryChecks = 0;
		m_StuckStrikes++;
		m_State.CurrentState = DZRB_MobileState.DZRB_STUCK_RECOVERY;
		vector target = m_Route.Waypoints[m_State.WaypointIndex];
		vector direction = target - m_Vehicle.GetPosition();
		direction[1] = 0;
		if (direction.Length() > 0.1) m_Vehicle.SetOrientation(Vector(direction.VectorToAngles()[0], 0, 0));
		if (m_StuckStrikes == 2 && m_State.LastValidPosition != vector.Zero)
		{
			vector safe = m_State.LastValidPosition;
			safe[1] = GetGame().SurfaceRoadY(safe[0], safe[2]) + 0.25;
			m_Vehicle.SetPosition(safe);
		}
		else if (m_StuckStrikes >= 3)
		{
			DZRB_TradeStop recoveryStop = GetStopAtWaypoint(m_State.WaypointIndex);
			if (recoveryStop)
			{
				vector stopTarget = target;
				stopTarget[1] = GetGame().SurfaceRoadY(stopTarget[0], stopTarget[2]) + 0.25;
				m_Vehicle.SetPosition(stopTarget);
				m_RecoveryCooldownUntil = now + 60000;
				OpenStop(recoveryStop);
				return;
			}
			// Only advance after the truck has actually reached the current route
			// point.  Advancing the index alone produced endless fake progress.
			vector recovered = target;
			recovered[1] = GetGame().SurfaceRoadY(recovered[0], recovered[2]) + 0.25;
			m_Vehicle.SetPosition(recovered);
			m_State.LastValidPosition = target;
			AdvanceWaypoint();
			if (m_State.CurrentState == DZRB_MobileState.DZRB_ROUTE_FINISHED) return;
			m_StuckStrikes = 0;
		}
		SetVelocity(m_Vehicle, vector.Zero);
		m_LastMovementPosition = m_Vehicle.GetPosition();
		m_RecoveryCooldownUntil = now + 60000;
		m_State.CurrentState = DZRB_MobileState.DZRB_TRAVEL;
		Log(string.Format("STUCK_RECOVERY waypoint=%1 strike=%2", m_State.WaypointIndex, m_StuckStrikes));
	}

	void SyncMarker(PlayerBase player)
	{
		if (!player || !player.GetIdentity() || m_State.CurrentState != DZRB_MobileState.DZRB_OPEN) return;
		if (DZRB_ExpansionTraderService.Position() == vector.Zero) return;
		Log("MARKER_SYNC player=" + player.GetIdentity().GetPlainId());
	}

	private void PeriodicWork()
	{
		int now = GetGame().GetTime();
		if (now - m_LastSaveTime >= m_Settings.StateSaveSeconds * 1000)
		{
			m_LastSaveTime = now;
			m_State.LastPosition = m_Vehicle.GetPosition();
			SaveState();
		}
		if (m_Settings.MarkerMode == "LIVE" && m_State.CurrentState != DZRB_MobileState.DZRB_OPEN && now - m_LastMarkerTime >= m_Settings.MarkerUpdateSeconds * 1000)
		{
			m_LastMarkerTime = now;
			DZRB_MarkerService.UpdateVehicle(m_Vehicle);
		}
		else if (m_Settings.MarkerMode == "STOP_ONLY" && m_State.CurrentState != DZRB_MobileState.DZRB_OPEN)
		{
			DZRB_MarkerService.RemoveCurrent(false);
		}
	}

	private void FaceNextWaypoint()
	{
		if (!m_Vehicle || m_State.WaypointIndex >= m_Route.Waypoints.Count()) return;
		int nextIndex = m_State.WaypointIndex + 1;
		if (nextIndex >= m_Route.Waypoints.Count()) nextIndex = 0;
		vector direction = m_Route.Waypoints[nextIndex] - m_Vehicle.GetPosition();
		direction[1] = 0;
		if (direction.Length() > 0.1) m_Vehicle.SetOrientation(Vector(direction.VectorToAngles()[0], 0, 0));
	}

	private DZRB_RuntimeState LoadState()
	{
		DZRB_RuntimeState state = new DZRB_RuntimeState();
		string error;
		if (FileExist(DZRB_MobileSettings.STATE_PATH) && !JsonFileLoader<DZRB_RuntimeState>.LoadFile(DZRB_MobileSettings.STATE_PATH, state, error))
			Log("STATE_LOAD_ERROR " + error);
		return state;
	}

	private void SaveState()
	{
		string error;
		JsonFileLoader<DZRB_RuntimeState>.SaveFile(DZRB_MobileSettings.STATE_PATH, m_State, error);
		if (error != "") Log("STATE_SAVE_ERROR " + error);
	}

	private void Log(string message)
	{
		Print("[DeutschZ_RBM] " + message);
	}

	void Shutdown()
	{
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(Tick);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(CloseStop);
		DZRB_ExpansionTraderService.Close();
		RemoveTravelTrader();
		if (m_Escort) m_Escort.Cleanup();
		if (m_Vehicle) GetGame().ObjectDelete(m_Vehicle);
		m_Vehicle = null;
	}
}
