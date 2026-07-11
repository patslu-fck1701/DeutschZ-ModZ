class DZ_KOTHTerminal : Land_Radio_PanelBig
{
	protected bool m_DZKOTHG_GateTerminal;

	void DZ_KOTHTerminal()
	{
		m_DZKOTHG_GateTerminal = true;
		RegisterNetSyncVariableBool("m_DZKOTHG_GateTerminal");

		if (GetGame() && GetGame().IsServer())
			SetAllowDamage(false);
	}

	override bool IsBuilding()
	{
		return false;
	}

	override void SetActions()
	{
		super.SetActions();
		AddAction(ActionDZKOTHGHackTerminal);
	}

	void DZKOTHG_SetGateTerminal(bool active)
	{
		m_DZKOTHG_GateTerminal = active;
		if (GetGame() && GetGame().IsServer())
			SetSynchDirty();
	}

	bool DZKOTHG_IsGateTerminal()
	{
		return m_DZKOTHG_GateTerminal;
	}
}
