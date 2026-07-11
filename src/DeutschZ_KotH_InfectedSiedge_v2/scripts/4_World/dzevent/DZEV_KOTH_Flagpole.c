class DZEV_KOTH_Flagpole : TerritoryFlag
{
	protected Object m_DZEV_Smoke;

	void DZEV_EnsureEventFlag()
	{
		if (!GetInventory())
			return;

		EntityAI current = FindAttachmentBySlotName("Material_FPole_Flag");
		if (!current)
			GetInventory().CreateAttachment("DZEV_KOTH_Flag");
	}

	void DZEV_SetSmokeState(int state)
	{
		string smokeType = "M18SmokeGrenade_White";
		if (state == DZEV_States.CAPTURING)
			smokeType = "M18SmokeGrenade_Green";
		else if (state == DZEV_States.BOSS || state == DZEV_States.COMPLETED)
			smokeType = "M18SmokeGrenade_Red";

		DZEV_ClearSmoke();

		vector smokePos = GetPosition() + "0 7.2 0";
		m_DZEV_Smoke = GetGame().CreateObjectEx(smokeType, smokePos, ECE_NONE);

		SmokeGrenadeBase smoke = SmokeGrenadeBase.Cast(m_DZEV_Smoke);
		if (smoke)
		{
			smoke.SetPosition(smokePos);
			smoke.SetSmokeGrenadeState(ESmokeGrenadeState.START);
		}
	}

	void DZEV_ClearSmoke()
	{
		if (m_DZEV_Smoke && GetGame())
			GetGame().ObjectDelete(m_DZEV_Smoke);

		m_DZEV_Smoke = null;
	}
}
