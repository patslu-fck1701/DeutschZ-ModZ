class DZCourierZ_Services
{
	protected ref set<string> m_Markers = new set<string>;
	protected ref array<EntityAI> m_Entities = new array<EntityAI>;
	protected ref array<EntityAI> m_Enemies = new array<EntityAI>;

	void NotifyAll(string message)
	{
		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		foreach (Man man: players) Notify(PlayerBase.Cast(man), message);
	}

	void Notify(PlayerBase player, string message)
	{
		if (!player || !player.GetIdentity()) return;
#ifdef EXPANSIONMODCORE
		ExpansionNotification("CourierZ", message, "Info", ARGB(255, 32, 170, 90), 12.0).Create(player.GetIdentity());
#else
		NotificationSystem.SendNotificationToPlayerExtended(player, 12.0, "CourierZ", message, "set:dayz_gui image:icon_info");
#endif
	}

	void PlaySound(PlayerBase player, string soundSet)
	{
		if (!player || !player.GetIdentity()) return;
		ScriptRPC rpc = new ScriptRPC;
		rpc.Write(DZCourierZ_RPCMessage.PLAY_SOUND);
		rpc.Write(soundSet);
		rpc.Send(player, DZCourierZ_Constants.RPC, true, player.GetIdentity());
	}

	void OpenRewardMenu(PlayerBase player)
	{
		if (!player || !player.GetIdentity()) return;
		ScriptRPC rpc = new ScriptRPC;
		rpc.Write(DZCourierZ_RPCMessage.OPEN_REWARD);
		rpc.Send(player, DZCourierZ_Constants.RPC, true, player.GetIdentity());
	}

	void SetRouteMarker(PlayerBase player, string label, vector position)
	{
		if (!player || !player.GetIdentity()) return;
		ScriptRPC rpc = new ScriptRPC;
		rpc.Write(DZCourierZ_RPCMessage.SET_ROUTE_MARKER);
		rpc.Write(label);
		rpc.Write(position);
		rpc.Send(player, DZCourierZ_Constants.RPC, true, player.GetIdentity());
	}

	void ClearRouteMarker(PlayerBase player)
	{
		SetRouteMarker(player, "", "0 0 0");
	}

	bool Marker(string uid, string label, vector position, bool marker3D)
	{
#ifdef EXPANSIONMODNAVIGATION
		ExpansionMarkerModule module = ExpansionMarkerModule.GetModuleInstance();
		if (!module)
		{
			DZCourierZ_Log.Warn("Expansion MarkerModule ist nicht initialisiert: " + uid);
			return false;
		}
		module.RemoveServerMarker(uid);
		ExpansionMarkerData marker = module.CreateServerMarker(label, "Deliver", position, ARGB(255, 255, 210, 0), marker3D, uid);
		if (marker)
		{
			marker.SetVisibility(EXPANSION_MARKER_VIS_WORLD | EXPANSION_MARKER_VIS_MAP);
			m_Markers.Insert(uid);
			DZCourierZ_Log.Info("Marker erstellt: uid=" + uid + " position=" + position.ToString() + " marker3D=" + marker3D.ToString());
			return true;
		}
		DZCourierZ_Log.Warn("Expansion-Marker konnte nicht erstellt werden: uid=" + uid + " position=" + position.ToString());
		return false;
#else
		DZCourierZ_Log.Warn("EXPANSIONMODNAVIGATION fehlt; Marker deaktiviert: " + uid);
		return false;
#endif
	}

	void RemoveMarker(string uid)
	{
#ifdef EXPANSIONMODNAVIGATION
		ExpansionMarkerModule module = ExpansionMarkerModule.GetModuleInstance();
		if (module) module.RemoveServerMarker(uid);
#endif
		int index = m_Markers.Find(uid);
		if (index >= 0) m_Markers.Remove(index);
	}

	EntityAI SpawnPassiveNPC(vector position)
	{
#ifdef EXPANSIONMODCORE
		int flags = ECE_CREATEPHYSICS | ECE_NOLIFETIME | ECE_PLACE_ON_SURFACE;
		Object object = ExpansionGame.CreateObjectExSafe("ExpansionNPCDenis", position, flags);
		ExpansionNPCBase npc = ExpansionNPCBase.Cast(object);
		if (!npc)
		{
			if (object) GetGame().ObjectDelete(object);
			DZCourierZ_Log.Warn("Uebergabe-NPC konnte nicht erzeugt werden.");
			return null;
		}
		npc.GetInventory().CreateAttachment("M65Jacket_Black");
		npc.GetInventory().CreateAttachment("CargoPants_Black");
		npc.GetInventory().CreateAttachment("CombatBoots_Black");
		npc.GetInventory().CreateAttachment("TacticalGloves_Black");
		npc.GetInventory().CreateAttachment("BalaclavaMask_Blackskull");
		npc.SetSynchDirty();
		npc.Update();
		m_Entities.Insert(npc);
		DZCourierZ_Log.Info("Netzwerksynchroner Uebergabe-NPC gespawnt: " + npc.GetType() + " position=" + npc.GetPosition().ToString());
		return npc;
#endif
		DZCourierZ_Log.Warn("Expansion Core fehlt; Uebergabe-NPC konnte nicht erzeugt werden.");
		return null;
	}

	int SpawnHostileAI(vector position, int count, bool weakAmbush = false)
	{
		m_Enemies.Clear();
#ifdef EXPANSIONMODAI
		eAIGroup group = eAIGroup.CreateGroup(new eAIFactionEast());
		if (!group)
		{
			DZCourierZ_Log.Warn("Expansion-AI-Gruppe fuer Gegner konnte nicht erzeugt werden.");
			return 0;
		}
		group.AddWaypoint(position);
		for (int i = 0; i < count; i++)
		{
			vector p = position + Vector(Math.RandomFloatInclusive(-18, 18), 0, Math.RandomFloatInclusive(-18, 18));
			Object object = GetGame().CreateObjectEx("eAI_SurvivorM_Boris", p, ECE_SETUP | ECE_INITAI | ECE_PLACE_ON_SURFACE);
			eAIBase ai = eAIBase.Cast(object);
			if (!ai)
			{
				if (object) GetGame().ObjectDelete(object);
				continue;
			}
			ai.SetGroup(group);
			ai.eAI_SetAccuracy(0.03, 0.12);
			if (weakAmbush)
			{
				ai.GetInventory().CreateAttachment("FOG_G99_Pants_Black");
				ai.GetInventory().CreateAttachment("FOG_G2_Hoodie_MC");
				ai.GetInventory().CreateAttachment("FOG_Combat_HikingBoots_Black");
				ai.GetInventory().CreateAttachment("FOG_Tactical_Fingerless_Gloves_Black");
				ai.GetInventory().CreateAttachment("FOG_Helmet_FAST_Bump_Black");
				ai.GetInventory().CreateAttachment("FOG_Vest_PACA_Black");
				ai.GetInventory().CreateInInventory("Glock19");
				ai.GetInventory().CreateInInventory("Mag_Glock_15Rnd");
				ai.GetInventory().CreateInInventory("Mag_Glock_15Rnd");
			}
			else if (i == 0)
			{
				string huntingRifle = "B95";
				if (Math.RandomInt(0, 2) == 1) huntingRifle = "Blaze";
				ai.GetInventory().CreateInInventory(huntingRifle);
				ai.GetInventory().CreateInInventory("AmmoBox_308Win_20Rnd");
			}
			else if (i == 1)
			{
				ai.GetInventory().CreateInInventory("M4A1");
				ai.GetInventory().CreateInInventory("Mag_STANAG_30Rnd");
				ai.GetInventory().CreateInInventory("Mag_STANAG_30Rnd");
			}
			else
			{
				ai.GetInventory().CreateInInventory("Sporter22");
				ai.GetInventory().CreateInInventory("Mag_Ruger1022_30Rnd");
				ai.GetInventory().CreateInInventory("Mag_Ruger1022_30Rnd");
			}
			m_Entities.Insert(ai);
			m_Enemies.Insert(ai);
		}
#endif
		return m_Enemies.Count();
	}

	EntityAI SpawnRewardTruck(string className, vector position, vector orientation)
	{
		if (!GetGame().IsServer() || className == "") return null;
		Object object = GetGame().CreateObjectEx(className, position, ECE_SETUP | ECE_INITAI | ECE_CREATEPHYSICS | ECE_PLACE_ON_SURFACE | ECE_NOLIFETIME);
		CarScript truck = CarScript.Cast(object);
		if (!truck)
		{
			if (object) GetGame().ObjectDelete(object);
			DZCourierZ_Log.Warn("Geheimer A2-Truck konnte nicht erzeugt werden: " + className);
			return null;
		}
		truck.SetPosition(position);
		truck.SetOrientation(orientation);
		truck.GetInventory().CreateAttachment("TruckBattery");
		truck.GetInventory().CreateAttachment("GlowPlug");
		truck.GetInventory().CreateAttachment("CarRadiator");
		truck.GetInventory().CreateAttachment("HeadlightH7");
		truck.GetInventory().CreateAttachment("HeadlightH7");
		truck.GetInventory().CreateAttachment("A2_KamAZ_Doors_Driver");
		truck.GetInventory().CreateAttachment("A2_KamAZ_Doors_Codriver");
		truck.GetInventory().CreateAttachment("A2_KamAZ_Doors_Trunk");
		for (int wheel = 0; wheel < 8; wheel++) truck.GetInventory().CreateAttachment("A2_KamAZWheel");
		truck.Fill(CarFluid.FUEL, truck.GetFluidCapacity(CarFluid.FUEL));
		truck.Fill(CarFluid.COOLANT, truck.GetFluidCapacity(CarFluid.COOLANT));
		truck.Fill(CarFluid.OIL, truck.GetFluidCapacity(CarFluid.OIL));
		truck.Fill(CarFluid.BRAKE, truck.GetFluidCapacity(CarFluid.BRAKE));
		truck.SetSynchDirty();
		DZCourierZ_Log.Info("Geheimer voll bestueckter A2-Truck gespawnt: class=" + className + " position=" + position.ToString());
		return truck;
	}

	int SpawnInfected(vector position, int count)
	{
		m_Enemies.Clear();
		array<string> types = {"ZmbM_SoldierNormal", "ZmbM_PatrolNormal_Autumn", "ZmbM_usSoldier_normal_Woodland"};
		for (int i = 0; i < count; i++)
		{
			vector p = position + Vector(Math.RandomFloatInclusive(-25, 25), 0, Math.RandomFloatInclusive(-25, 25));
			EntityAI infected = EntityAI.Cast(GetGame().CreateObjectEx(types.Get(i % types.Count()), p, ECE_SETUP | ECE_INITAI | ECE_PLACE_ON_SURFACE));
			if (!infected) continue;
			m_Entities.Insert(infected);
			m_Enemies.Insert(infected);
		}
		return m_Enemies.Count();
	}

	bool EnemiesDead()
	{
		foreach (EntityAI enemy: m_Enemies) if (enemy && enemy.IsAlive()) return false;
		return true;
	}

	void Cleanup()
	{
		array<string> ids = new array<string>;
		foreach (string id: m_Markers) ids.Insert(id);
		foreach (string markerId: ids) RemoveMarker(markerId);
		foreach (EntityAI entity: m_Entities) if (entity) GetGame().ObjectDelete(entity);
		m_Entities.Clear();
		m_Enemies.Clear();
	}
}
