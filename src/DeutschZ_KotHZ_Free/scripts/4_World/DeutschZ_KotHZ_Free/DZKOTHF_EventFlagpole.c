class DZKOTHF_EventFlagpole extends StaticFlagPole
{
	protected float m_DZKOTHF_CaptureProgress;
	protected float m_DZKOTHF_CaptureProgressLocal;
	protected int m_DZKOTHF_SmokeState;
	protected int m_DZKOTHF_SmokeStateLocal;
	protected Particle m_DZKOTHF_SmokeParticle;

	void DZKOTHF_EventFlagpole()
	{
		m_DZKOTHF_CaptureProgress = 0.0;
		m_DZKOTHF_CaptureProgressLocal = -1.0;
		m_DZKOTHF_SmokeState = DZKOTHF_SmokeState.NONE;
		m_DZKOTHF_SmokeStateLocal = -2;
		RegisterNetSyncVariableFloat("m_DZKOTHF_CaptureProgress");
		RegisterNetSyncVariableInt("m_DZKOTHF_SmokeState");

		if (GetGame() && GetGame().IsServer())
			SetAllowDamage(false);
	}

	override void EEDelete(EntityAI parent)
	{
		StopSmokeParticle();
		super.EEDelete(parent);
	}

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		if (m_DZKOTHF_CaptureProgressLocal != m_DZKOTHF_CaptureProgress)
		{
			m_DZKOTHF_CaptureProgressLocal = m_DZKOTHF_CaptureProgress;
			UpdateFlagVisual();
		}

		if (m_DZKOTHF_SmokeStateLocal != m_DZKOTHF_SmokeState)
		{
			m_DZKOTHF_SmokeStateLocal = m_DZKOTHF_SmokeState;
			UpdateSmokeParticle();
		}
	}

	EntityAI EnsureExactlyOneEventFlag()
	{
		if (!GetGame() || !GetGame().IsServer() || !GetInventory())
			return null;

		FullyBuild();
		array<EntityAI> flagsToDelete = new array<EntityAI>;
		EntityAI eventFlag;
		int attachmentCount = GetInventory().AttachmentCount();
		for (int index = 0; index < attachmentCount; index++)
		{
			EntityAI attachment = GetInventory().GetAttachmentFromIndex(index);
			if (!attachment || !Flag_Base.Cast(attachment))
				continue;

			if (!eventFlag && attachment.GetType() == DZKOTHF_Constants.FLAG_CLASSNAME)
				eventFlag = attachment;
			else
				flagsToDelete.Insert(attachment);
		}

		foreach (EntityAI flagToDelete: flagsToDelete)
		{
			if (flagToDelete)
				GetGame().ObjectDelete(flagToDelete);
		}

		if (!eventFlag)
			eventFlag = GetInventory().CreateInInventory(DZKOTHF_Constants.FLAG_CLASSNAME);

		SetCaptureProgress(m_DZKOTHF_CaptureProgress);
		return eventFlag;
	}

	void SetCaptureProgress(float progress)
	{
		m_DZKOTHF_CaptureProgress = Math.Clamp(progress, 0.0, 1.0);
		m_DZKOTHF_CaptureProgressLocal = m_DZKOTHF_CaptureProgress;
		UpdateFlagVisual();
		SetSynchDirty();
	}

	void SetSmokeState(int smokeState)
	{
		m_DZKOTHF_SmokeState = smokeState;
		SetSynchDirty();

		if (!GetGame() || !GetGame().IsDedicatedServer())
		{
			m_DZKOTHF_SmokeStateLocal = smokeState;
			UpdateSmokeParticle();
		}
	}

	protected void UpdateFlagVisual()
	{
		AnimateFlagEx(1.0 - Math.Clamp(m_DZKOTHF_CaptureProgressLocal, 0.0, 1.0));
	}

	protected void UpdateSmokeParticle()
	{
		if (!GetGame() || GetGame().IsDedicatedServer())
			return;

		StopSmokeParticle();
		int particleId = GetSmokeParticleId();
		if (particleId > 0)
			m_DZKOTHF_SmokeParticle = ParticleManager.GetInstance().PlayOnObject(particleId, this, "0 13 0", vector.Zero, true);
	}

	protected int GetSmokeParticleId()
	{
		switch (m_DZKOTHF_SmokeStateLocal)
		{
			case DZKOTHF_SmokeState.WHITE:
				return ParticleList.GRENADE_M18_WHITE_LOOP;
			case DZKOTHF_SmokeState.GREEN:
				return ParticleList.GRENADE_M18_GREEN_LOOP;
			case DZKOTHF_SmokeState.RED:
				return ParticleList.GRENADE_M18_RED_LOOP;
		}

		return 0;
	}

	protected void StopSmokeParticle()
	{
		if (m_DZKOTHF_SmokeParticle)
			m_DZKOTHF_SmokeParticle.Stop();

		m_DZKOTHF_SmokeParticle = null;
	}

	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}

	override bool CanReleaseAttachment(EntityAI attachment)
	{
		return false;
	}

	override void SetActions()
	{
		super.SetActions();
		RemoveAction(ActionRaiseFlag);
		RemoveAction(ActionLowerFlag);
		RemoveAction(ActionFoldBaseBuildingObject);
	}
}
