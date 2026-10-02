class DZRB_Escort
{
	private CarScript m_Car;
	private CarScript m_Leader;
	private ref array<eAIBase> m_Guards = new array<eAIBase>();
	private eAIGroup m_Group;
	private ref array<vector> m_Trail = new array<vector>();
	private bool m_Guarding;
	private int m_NextBoard;
	private int m_BoardDeadline;
	private bool m_Failed;
	private int m_SeparatedSince;
	private float m_DriveTickSeconds;

	bool Spawn(CarScript leader, DZRB_MobileSettings settings)
	{
		m_Leader = leader;
		m_DriveTickSeconds = Math.Max(settings.RouteTickMilliseconds / 1000.0, 0.05);
		vector direction = leader.GetDirection();
		vector position = leader.GetPosition() - direction * 18;
		position[1] = GetGame().SurfaceRoadY(position[0], position[2]) + 0.25;
		m_Car = CarScript.Cast(GetGame().CreateObjectEx("Offroad_02", position, ECE_CREATEPHYSICS | ECE_PLACE_ON_SURFACE | ECE_NOPERSISTENCY_WORLD));
		if (!m_Car) return false;
		m_Car.SetOrientation(leader.GetOrientation());
		array<string> parts = {"Offroad_02_Wheel", "Offroad_02_Wheel", "Offroad_02_Wheel", "Offroad_02_Wheel", "Offroad_02_Wheel", "CarBattery", "GlowPlug", "Offroad_02_Door_1_1", "Offroad_02_Door_2_1", "Offroad_02_Door_1_2", "Offroad_02_Door_2_2", "Offroad_02_Hood", "Offroad_02_Trunk", "HeadlightH7", "HeadlightH7"};
		foreach (string part : parts)
		{
			EntityAI mountedPart = m_Car.GetInventory().CreateAttachment(part);
			if (!mountedPart)
			{
				Print("[DeutschZ_RBM] ESCORT_PART_FAILED " + part);
				Cleanup();
				return false;
			}
			mountedPart.SetAllowDamage(false);
		}
		m_Car.Fill(CarFluid.FUEL, m_Car.GetFluidCapacity(CarFluid.FUEL));
		m_Car.Fill(CarFluid.COOLANT, m_Car.GetFluidCapacity(CarFluid.COOLANT));
		m_Car.Fill(CarFluid.OIL, m_Car.GetFluidCapacity(CarFluid.OIL));
		m_Car.Fill(CarFluid.BRAKE, m_Car.GetFluidCapacity(CarFluid.BRAKE));
		m_Car.DZRB_ConfigureMobile(true, true, true, true);
		m_Car.SetAllowDamage(false);
		m_Car.DZRB_StopDriving(false);
		m_Car.DZRB_PrimeEngine();
		eAIFaction faction = new eAIFactionGuards();
		m_Group = eAIGroup.CreateGroup(faction);
		for (int i = 0; i < 2; i++)
		{
			vector guardPosition = position + Vector(4 + i * 2, 0, 0);
			guardPosition[1] = GetGame().SurfaceY(guardPosition[0], guardPosition[2]);
			eAIBase guard = eAIBase.Cast(GetGame().CreateObjectEx("eAI_SurvivorM_Mirek", guardPosition, ECE_PLACE_ON_SURFACE | ECE_NOLIFETIME));
			if (!guard) { Cleanup(); return false; }
			m_Guards.Insert(guard);
			guard.SetAllowDamage(false);
			guard.SetGroup(m_Group);
			array<string> uniform = {"Mich2001Helmet", "BalaclavaMask_Black", "TacticalShirt_Olive", "PlateCarrierVest", "CargoPants_Green", "TacticalGloves_Black", "CombatBoots_Black"};
			foreach (string garment : uniform)
				if (!guard.GetInventory().CreateAttachment(garment)) Print("[DeutschZ_RBM] GUARD_OUTFIT_FAILED class=" + garment);
			string weaponClass = "M4A1";
			string magazineClass = "Mag_STANAG_30Rnd";
			if (GetGame().ConfigIsExisting("CfgWeapons TTC_TAVOR") && GetGame().ConfigIsExisting("CfgMagazines TTC_TAVOR_Magazine_30rnd"))
			{
				weaponClass = "TTC_TAVOR";
				magazineClass = "TTC_TAVOR_Magazine_30rnd";
			}
			if (!guard.GetHumanInventory().CreateInHands(weaponClass)) Print("[DeutschZ_RBM] GUARD_WEAPON_FAILED class=" + weaponClass);
			for (int magazine = 0; magazine < 3; magazine++)
				if (!guard.GetInventory().CreateInInventory(magazineClass)) Print("[DeutschZ_RBM] GUARD_MAGAZINE_FAILED class=" + magazineClass);
			Print("[DeutschZ_RBM] GUARD_ARMED index=" + i + " weapon=" + weaponClass);
			Print("[DeutschZ_RBM] GUARD_VISIBLE index=" + i + " position=" + guard.GetPosition().ToString());
		}
		m_Group.SetWaypointBehaviour(eAIWaypointBehavior.HALT);
		m_Trail.Insert(leader.GetPosition());
		m_BoardDeadline = GetGame().GetTime() + 60000;
		Print("[DeutschZ_RBM] ESCORT_SPAWNED guards=2 vehicle=Offroad_02");
		return true;
	}

	bool Update(bool trading)
	{
		if (!m_Car || !m_Leader || m_Failed) return false;
		foreach (eAIBase aliveGuard : m_Guards)
			if (!aliveGuard || !aliveGuard.IsAlive()) { m_Failed = true; m_Car.DZRB_StopDriving(true); return false; }
		if (trading)
		{
			m_Car.DZRB_StopDriving(false);
			if (Math.AbsFloat(m_Car.GetSpeedometer()) > 0.5) return true;
			if (!m_Guarding)
			{
				m_Guarding = true;
				m_Group.ClearWaypoints();
				vector center = m_Leader.GetPosition();
				vector forward = m_Leader.GetDirection();
				vector right = Vector(forward[2], 0, -forward[0]);
				m_Group.AddWaypoint(center + forward * 7 + right * 7);
				m_Group.AddWaypoint(center - forward * 7 + right * 7);
				m_Group.AddWaypoint(center - forward * 7 - right * 7);
				m_Group.AddWaypoint(center + forward * 7 - right * 7);
				m_Group.SetWaypointBehaviour(eAIWaypointBehavior.LOOP);
				foreach (eAIBase guard : m_Guards)
				{
					guard.Notify_Transport(null, -1);
					HumanCommandVehicle command = guard.GetCommand_Vehicle();
					if (command) command.GetOutVehicle();
				}
				Print("[DeutschZ_RBM] ESCORT_GUARDING");
			}
			foreach (eAIBase armedGuard : m_Guards)
				if (armedGuard && armedGuard.CanRaiseWeapon()) armedGuard.RaiseWeapon(true);
			return true;
		}
		if (m_Guarding)
		{
			m_Guarding = false;
			m_BoardDeadline = GetGame().GetTime() + 60000;
			m_NextBoard = 0;
			m_Trail.Clear();
			m_Trail.Insert(m_Leader.GetPosition());
			m_SeparatedSince = 0;
		}
		bool seated = true;
		for (int seat = 0; seat < m_Guards.Count(); seat++)
			if (m_Car.CrewMember(seat) != m_Guards[seat]) seated = false;
		if (!seated)
		{
			m_Car.DZRB_StopDriving(false);
			if (GetGame().GetTime() > m_BoardDeadline)
			{
				m_Failed = true;
				Print("[DeutschZ_RBM] ESCORT_BOARD_TIMEOUT convoy_halted");
				return false;
			}
			if (GetGame().GetTime() >= m_NextBoard)
			{
				m_NextBoard = GetGame().GetTime() + 2000;
				for (int boarding = 0; boarding < m_Guards.Count(); boarding++)
					if (!m_Guards[boarding].GetCommand_Vehicle()) m_Guards[boarding].Notify_Transport(m_Car, boarding);
			}
			return false;
		}
		vector leaderPosition = m_Leader.GetPosition();
		if (vector.Distance(m_Trail[m_Trail.Count() - 1], leaderPosition) >= 4) m_Trail.Insert(leaderPosition);
		vector delta = leaderPosition - m_Car.GetPosition();
		delta[1] = 0;
		float gap = delta.Length();
		if (gap < 14) { m_SeparatedSince = 0; m_Car.DZRB_StopDriving(false); return true; }
		if (m_Trail.Count() > 1 && vector.Distance(m_Car.GetPosition(), m_Trail[0]) < 6) m_Trail.RemoveOrdered(0);
		m_Car.DZRB_SetDriveTarget(m_Trail[0], Math.Clamp(gap - 12, 10, 48));
		m_Car.DZRB_UpdateDrive(m_DriveTickSeconds);
		if (gap < 65) { m_SeparatedSince = 0; return true; }
		if (m_SeparatedSince == 0) m_SeparatedSince = GetGame().GetTime();
		if (GetGame().GetTime() - m_SeparatedSince < 15000) return false;
		vector recovery = leaderPosition - m_Leader.GetDirection() * 24;
		recovery[1] = GetGame().SurfaceRoadY(recovery[0], recovery[2]) + 0.25;
		m_Car.DZRB_StopDriving(true);
		m_Car.SetPosition(recovery);
		m_Car.SetOrientation(m_Leader.GetOrientation());
		SetVelocity(m_Car, vector.Zero);
		m_Trail.Clear();
		m_Trail.Insert(leaderPosition);
		m_SeparatedSince = 0;
		Print("[DeutschZ_RBM] ESCORT_REJOIN gap=" + gap.ToString());
		return true;
	}

	void Cleanup()
	{
		foreach (eAIBase guard : m_Guards)
			if (guard) GetGame().ObjectDelete(guard);
		m_Guards.Clear();
		m_Group = null;
		if (m_Car) GetGame().ObjectDelete(m_Car);
		m_Car = null;
	}
}
