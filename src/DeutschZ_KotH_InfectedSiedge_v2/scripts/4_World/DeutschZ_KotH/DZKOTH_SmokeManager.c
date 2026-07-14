class DZKOTH_EventFlagpole extends StaticFlagPole
{
	protected float m_DZKOTH_RaisedAmount;
	protected float m_DZKOTH_RaisedAmountLocal;
	protected int m_DZKOTH_SmokeState;
	protected int m_DZKOTH_SmokeStateLocal;
	protected Particle m_DZKOTH_SmokeParticle;

	void DZKOTH_EventFlagpole()
	{
		m_DZKOTH_RaisedAmount = 0.0;
		m_DZKOTH_RaisedAmountLocal = -1.0;
		m_DZKOTH_SmokeState = -1;
		m_DZKOTH_SmokeStateLocal = -2;
		RegisterNetSyncVariableFloat("m_DZKOTH_RaisedAmount");
		RegisterNetSyncVariableInt("m_DZKOTH_SmokeState");

		if (GetGame() && GetGame().IsServer())
			SetAllowDamage(false);
	}

	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);
		DZKOTH_StopSmokeParticle();
	}

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		if (m_DZKOTH_RaisedAmountLocal != m_DZKOTH_RaisedAmount)
		{
			m_DZKOTH_RaisedAmountLocal = m_DZKOTH_RaisedAmount;
			DZKOTH_UpdateVisuals();
		}

		if (m_DZKOTH_SmokeStateLocal != m_DZKOTH_SmokeState)
		{
			m_DZKOTH_SmokeStateLocal = m_DZKOTH_SmokeState;
			DZKOTH_UpdateSmokeParticle();
		}
	}

	void DZKOTH_EnsureEventFlag()
	{
		if (!GetInventory())
			return;

		FullyBuild();
		AnimateFlagEx(1.0);

		EntityAI current;
		array<EntityAI> redundantFlags = new array<EntityAI>;
		int attachmentCount = GetInventory().AttachmentCount();
		for (int attachmentIndex = 0; attachmentIndex < attachmentCount; attachmentIndex++)
		{
			EntityAI attachment = GetInventory().GetAttachmentFromIndex(attachmentIndex);
			if (!attachment || !Flag_Base.Cast(attachment))
				continue;

			if (!current && attachment.GetType() == DZKOTH_Const.FLAG_CLASSNAME)
				current = attachment;
			else
				redundantFlags.Insert(attachment);
		}

		foreach (EntityAI redundantFlag: redundantFlags)
		{
			if (redundantFlag)
				redundantFlag.Delete();
		}

		if (!current)
			current = GetInventory().CreateAttachment(DZKOTH_Const.FLAG_CLASSNAME);

		DZKOTH_SetRaisedAmount(0.0);
		DZKOTH_Utils.Log("Flagpole visual state forced: built=yes flag=" + BoolText(current != null) + " raised=no");
	}

	void DZKOTH_DeleteEventFlag()
	{
		EntityAI current = FindAttachmentBySlotName("Material_FPole_Flag");
		if (current && GetGame())
			GetGame().ObjectDelete(current);
	}

	void DZKOTH_SetSmokeState(int state)
	{
		m_DZKOTH_SmokeState = state;
		SetSynchDirty();

		if (!GetGame() || !GetGame().IsDedicatedServer())
		{
			m_DZKOTH_SmokeStateLocal = state;
			DZKOTH_UpdateSmokeParticle();
		}
	}

	void DZKOTH_ClearSmoke()
	{
		DZKOTH_SetSmokeState(-1);
		DZKOTH_StopSmokeParticle();
	}

	void DZKOTH_SetFlagRaiseProgress(float amount)
	{
		DZKOTH_SetRaisedAmount(amount);
	}

	protected void DZKOTH_SetRaisedAmount(float amount)
	{
		m_DZKOTH_RaisedAmount = Math.Clamp(amount, 0.0, 1.0);
		m_DZKOTH_RaisedAmountLocal = m_DZKOTH_RaisedAmount;
		DZKOTH_UpdateVisuals();
		SetSynchDirty();
	}

	protected void DZKOTH_UpdateVisuals()
	{
		float amount = Math.Clamp(m_DZKOTH_RaisedAmountLocal, 0.0, 1.0);
		AnimateFlagEx(1.0 - amount);
	}

	protected void DZKOTH_UpdateSmokeParticle()
	{
		if (!GetGame() || GetGame().IsDedicatedServer())
			return;

		DZKOTH_StopSmokeParticle();

		int particleId = DZKOTH_GetSmokeParticleId();
		if (particleId <= 0)
			return;

		m_DZKOTH_SmokeParticle = ParticleManager.GetInstance().PlayOnObject(particleId, this, "0 13 0", vector.Zero, true);
	}

	protected int DZKOTH_GetSmokeParticleId()
	{
		switch (m_DZKOTH_SmokeStateLocal)
		{
			case 0:
				return ParticleList.GRENADE_M18_WHITE_LOOP;
			case 1:
				return ParticleList.GRENADE_M18_GREEN_LOOP;
			case 2:
				return ParticleList.GRENADE_M18_RED_LOOP;
		}

		return 0;
	}

	protected void DZKOTH_StopSmokeParticle()
	{
		if (m_DZKOTH_SmokeParticle)
			m_DZKOTH_SmokeParticle.Stop();

		m_DZKOTH_SmokeParticle = null;
	}

	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}

	override bool CanReleaseAttachment(EntityAI attachment)
	{
		return false;
	}

	override void SetPartsAfterStoreLoad()
	{
	}

	override void SetActions()
	{
		super.SetActions();
		RemoveAction(ActionRaiseFlag);
		RemoveAction(ActionLowerFlag);
		RemoveAction(ActionFoldBaseBuildingObject);
#ifdef EXPANSIONMODBASEBUILDING
		RemoveAction(ExpansionActionEnterFlagMenu);
		RemoveAction(ExpansionActionDismantleFlag);
#endif
	}

	protected string BoolText(bool value)
	{
		if (value)
			return "true";

		return "false";
	}
}

class DZKOTH_SmokeManager
{
	static const int SMOKE_REFRESH_MS = 45000;
	static const float SMOKE_HEIGHT_OFFSET = 13.0;

	protected DZKOTH_EventFlagpole m_Flagpole;
	protected vector m_FlagpolePosition;
	protected Object m_ServerSmoke;
	protected string m_CurrentSmokeType;
	protected bool m_SmokeRefreshActive;

	void Setup(vector flagPosition, vector flagOrientation)
	{
		if (!GetGame() || m_Flagpole)
			return;

		vector pos = DZKOTH_Utils.Grounded(flagPosition);
		m_FlagpolePosition = pos;
		RemoveStaleEventFlagpoles(pos);
		int runtimeFlags = ECE_NOLIFETIME | ECE_NOPERSISTENCY_WORLD;
		m_Flagpole = TrySpawnFlagpole(pos, ECE_SETUP | ECE_CREATEPHYSICS | ECE_PLACE_ON_SURFACE | runtimeFlags);
		if (!m_Flagpole)
			m_Flagpole = TrySpawnFlagpole(pos, ECE_SETUP | ECE_PLACE_ON_SURFACE | runtimeFlags);
		if (!m_Flagpole)
			m_Flagpole = TrySpawnFlagpole(pos, runtimeFlags);
		if (!m_Flagpole)
		{
			DZKOTH_Utils.Warn("Could not spawn event flagpole at " + pos.ToString());
			return;
		}

		m_Flagpole.SetOrientation(flagOrientation);
		m_Flagpole.SetPosition(pos);
		m_Flagpole.DZKOTH_EnsureEventFlag();
		SetReady();
		DZKOTH_Utils.Log("Flagpole spawned at " + pos.ToString() + " orientation " + flagOrientation.ToString());
	}

	protected void RemoveStaleEventFlagpoles(vector pos)
	{
		if (!GetGame() || pos == vector.Zero)
			return;

		array<Object> objects = new array<Object>;
		array<CargoBase> proxies = new array<CargoBase>;
		array<Object> staleFlags = new array<Object>;
		array<Object> stalePoles = new array<Object>;
		GetGame().GetObjectsAtPosition3D(pos, 20.0, objects, proxies);

		foreach (Object object: objects)
		{
			if (!object)
				continue;

			string typeName = object.GetType();
			if (typeName == DZKOTH_Const.FLAG_CLASSNAME || typeName == "DZEV_KOTH_Flag")
				staleFlags.Insert(object);
			else if (typeName == DZKOTH_Const.FLAGPOLE_CLASSNAME || typeName == "DZEV_KOTH_Flagpole")
				stalePoles.Insert(object);
		}

		foreach (Object staleFlag: staleFlags)
		{
			if (staleFlag)
				GetGame().ObjectDelete(staleFlag);
		}

		foreach (Object stalePole: stalePoles)
		{
			if (stalePole)
				GetGame().ObjectDelete(stalePole);
		}
	}

	protected DZKOTH_EventFlagpole TrySpawnFlagpole(vector pos, int flags)
	{
		Object object = GetGame().CreateObjectEx(DZKOTH_Const.FLAGPOLE_CLASSNAME, pos, flags);
		DZKOTH_EventFlagpole flagpole = DZKOTH_EventFlagpole.Cast(object);
		if (!flagpole && object)
			GetGame().ObjectDelete(object);

		if (flagpole)
			DZKOTH_Utils.Log("Flagpole spawn path accepted flags=" + flags.ToString() + " at " + pos.ToString());

		return flagpole;
	}

	void SetReady()
	{
		SetSmoke("M18SmokeGrenade_White");
	}

	void SetCapture()
	{
		SetSmoke("M18SmokeGrenade_Green");
	}

	void SetCompleted()
	{
		SetSmoke("M18SmokeGrenade_Red");
	}

	void StopSmokeOnly()
	{
		if (m_Flagpole)
			m_Flagpole.DZKOTH_ClearSmoke();

		StopSmokeRefresh();
		ClearServerSmoke();
	}

	void SetFlagRaiseProgress(float progress)
	{
		if (!m_Flagpole)
			return;

		m_Flagpole.DZKOTH_SetFlagRaiseProgress(progress);
	}

	void Cleanup()
	{
		if (m_Flagpole)
			m_Flagpole.DZKOTH_ClearSmoke();

		StopSmokeRefresh();
		ClearServerSmoke();
	}

	void DeleteFlagpole()
	{
		Cleanup();
		vector oldPosition = m_FlagpolePosition;

		if (m_Flagpole && GetGame())
		{
			oldPosition = m_Flagpole.GetPosition();
			m_Flagpole.DZKOTH_DeleteEventFlag();
			GetGame().ObjectDelete(m_Flagpole);
		}

		m_Flagpole = null;
		m_FlagpolePosition = vector.Zero;
		if (oldPosition != vector.Zero && GetGame())
		{
			RemoveStaleEventFlagpoles(oldPosition);
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(RemoveStaleEventFlagpoles, 250, false, oldPosition);
		}
	}

	protected void SetSmoke(string smokeType)
	{
		if (!m_Flagpole)
			return;

		m_CurrentSmokeType = smokeType;

		if (smokeType == "M18SmokeGrenade_White")
			m_Flagpole.DZKOTH_SetSmokeState(0);
		else if (smokeType == "M18SmokeGrenade_Green")
			m_Flagpole.DZKOTH_SetSmokeState(1);
		else if (smokeType == "M18SmokeGrenade_Red")
			m_Flagpole.DZKOTH_SetSmokeState(2);

		SpawnServerSmoke(smokeType);
		StartSmokeRefresh();
		DZKOTH_Utils.Log("Persistent KotH smoke active: " + smokeType + " at " + m_Flagpole.GetPosition().ToString());
	}

	protected void SpawnServerSmoke(string smokeType)
	{
		ClearServerSmoke();
		if (!GetGame() || !m_Flagpole)
			return;

		vector smokePos = m_Flagpole.GetPosition() + Vector(0, SMOKE_HEIGHT_OFFSET, 0);
		m_ServerSmoke = GetGame().CreateObjectEx(smokeType, smokePos, ECE_NONE);
		SmokeGrenadeBase smoke = SmokeGrenadeBase.Cast(m_ServerSmoke);
		if (smoke)
		{
			smoke.SetPosition(smokePos);
			smoke.SetSmokeGrenadeState(ESmokeGrenadeState.START);
		}
	}

	protected void StartSmokeRefresh()
	{
		if (!GetGame() || m_SmokeRefreshActive)
			return;

		m_SmokeRefreshActive = true;
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(RefreshServerSmoke);
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(RefreshServerSmoke, SMOKE_REFRESH_MS, true);
	}

	protected void StopSmokeRefresh()
	{
		m_SmokeRefreshActive = false;
		if (GetGame())
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(RefreshServerSmoke);
	}

	protected void RefreshServerSmoke()
	{
		if (!m_SmokeRefreshActive || !m_Flagpole || m_CurrentSmokeType == "")
			return;

		SpawnServerSmoke(m_CurrentSmokeType);
		DZKOTH_Utils.Log("Persistent KotH smoke refreshed: " + m_CurrentSmokeType);
	}

	protected void ClearServerSmoke()
	{
		if (m_ServerSmoke && GetGame())
			GetGame().ObjectDelete(m_ServerSmoke);

		m_ServerSmoke = null;
	}
}
