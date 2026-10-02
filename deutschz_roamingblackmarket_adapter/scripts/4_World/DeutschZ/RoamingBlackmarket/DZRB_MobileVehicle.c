modded class CarScript
{
	protected bool m_DZRB_MobileVehicle;
	protected bool m_DZRB_Autopilot;
	protected bool m_DZRB_Invulnerable;
	protected bool m_DZRB_BlockCargo;
	protected bool m_DZRB_BlockParts;
	protected bool m_DZRB_BlockEntry;
	protected vector m_DZRB_Target;
	protected float m_DZRB_TargetSpeed;
	protected float m_DZRB_Integral;

	void CarScript()
	{
		RegisterNetSyncVariableBool("m_DZRB_MobileVehicle");
		RegisterNetSyncVariableBool("m_DZRB_BlockEntry");
		RegisterNetSyncVariableBool("m_DZRB_BlockCargo");
		RegisterNetSyncVariableBool("m_DZRB_BlockParts");
	}

	void DZRB_ConfigureMobile(bool invulnerable, bool blockCargo, bool blockParts, bool blockEntry)
	{
		m_DZRB_MobileVehicle = true;
		m_DZRB_Invulnerable = invulnerable;
		m_DZRB_BlockCargo = blockCargo;
		m_DZRB_BlockParts = blockParts;
		m_DZRB_BlockEntry = blockEntry;
		SetSynchDirty();
	}

	bool DZRB_IsEntryBlocked()
	{
		return m_DZRB_MobileVehicle && m_DZRB_BlockEntry;
	}

	void DZRB_SetDriveTarget(vector target, float speedKmh)
	{
		m_DZRB_Target = target;
		m_DZRB_TargetSpeed = Math.Max(speedKmh, 0.0);
		m_DZRB_Autopilot = true;
	}

	void DZRB_StopDriving(bool stopEngine)
	{
		m_DZRB_Autopilot = false;
		m_DZRB_Integral = 0;
		SetThrottle(0);
		SetSteering(0);
		SetBrake(1.0);
		SetHandbrake(1.0);
		SetBrakesActivateWithoutDriver(true);
		if (stopEngine && EngineIsOn()) EngineStop();
	}

	void DZRB_PrimeEngine()
	{
		if (!GetGame().IsServer()) return;
		dBodyActive(this, ActiveState.ACTIVE);
		DisableSimulation(false);
		if (!EngineIsOn()) EngineStart();
	}

	override void OnInput(float dt)
	{
		super.OnInput(dt);
		DZRB_UpdateDrive(dt);
	}

	// OnInput is not called reliably for an unoccupied vehicle. The mission
	// manager therefore invokes this server-side control step on every route tick.
	void DZRB_UpdateDrive(float dt)
	{
		if (!GetGame().IsServer() || !m_DZRB_MobileVehicle || !m_DZRB_Autopilot) return;

		dBodyActive(this, ActiveState.ACTIVE);
		DisableSimulation(false);
		SetBrakesActivateWithoutDriver(false);
		SetHandbrake(0);
		if (!EngineIsOn())
		{
			EngineStart();
			return;
		}

		if (GearboxGetType() != CarGearboxType.MANUAL && GearboxGetMode() != CarAutomaticGearboxMode.D)
			ShiftTo(CarAutomaticGearboxMode.D);
		if (GearboxGetType() == CarGearboxType.MANUAL && GetGear() < CarGear.FIRST)
			ShiftTo(CarGear.FIRST);

		vector desired = m_DZRB_Target - GetPosition();
		desired[1] = 0;
		float steering;
		if (desired.Length() > 0.01)
		{
			float desiredYaw = desired.VectorToAngles()[0];
			float currentYaw = GetOrientation()[0];
			float yawError = desiredYaw - currentYaw;
			while (yawError > 180.0) yawError -= 360.0;
			while (yawError < -180.0) yawError += 360.0;
			steering = Math.Clamp(yawError / 38.0, -1.0, 1.0);
		}
		SetSteering(steering);

		float cornerFactor = 1.0 - Math.Min(Math.AbsFloat(steering) * 0.65, 0.7);
		float requestedSpeed = m_DZRB_TargetSpeed * cornerFactor;
		float error = requestedSpeed - GetSpeedometer();
		m_DZRB_Integral = Math.Clamp(m_DZRB_Integral + error * dt, -8.0, 12.0);

		if (error > 0.5)
		{
			SetBrake(0);
			SetThrottle(Math.Clamp(error * 0.07 + m_DZRB_Integral * 0.04, 0.18, 1.0));
		}
		else if (error < -2.0)
		{
			SetThrottle(0);
			SetBrake(Math.Clamp(Math.AbsFloat(error) * 0.06, 0.2, 0.65));
		}
		else
		{
			SetThrottle(0.10);
			SetBrake(0);
		}
	}

	override bool EEOnDamageCalculated(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		if (m_DZRB_MobileVehicle && m_DZRB_Invulnerable) return false;
		return super.EEOnDamageCalculated(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
	}

	override bool CanDisplayCargo()
	{
		if (m_DZRB_MobileVehicle && m_DZRB_BlockCargo) return false;
		return super.CanDisplayCargo();
	}

	override bool CanReleaseCargo(EntityAI cargo)
	{
		if (m_DZRB_MobileVehicle && m_DZRB_BlockCargo) return false;
		return super.CanReleaseCargo(cargo);
	}

	override bool CanReleaseAttachment(EntityAI attachment)
	{
		if (m_DZRB_MobileVehicle && m_DZRB_BlockParts) return false;
		return super.CanReleaseAttachment(attachment);
	}
}

modded class ActionGetInTransport
{
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		CarScript car;
		if (target && Class.CastTo(car, target.GetObject()) && car.DZRB_IsEntryBlocked()) return false;
		return super.ActionCondition(player, target, item);
	}
}
