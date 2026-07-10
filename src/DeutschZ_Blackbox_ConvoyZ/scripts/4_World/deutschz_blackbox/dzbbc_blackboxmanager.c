class DZBBC_BlackboxManager
{
	protected ItemBase m_Blackbox;
	protected bool m_Unlocked;

	void Spawn(DZBBC_CrashSiteConfig site)
	{
		if (!site)
			return;

		m_Blackbox = ItemBase.Cast(DZBBC_Utils.CreateObjectSafe(DZBBC_BLACKBOX_CLASSNAME, site.GetBlackboxPosition()));
		m_Unlocked = m_Blackbox != null;
		if (m_Unlocked)
			DZBBC_Utils.Log("Blackbox spawned at " + m_Blackbox.GetPosition().ToString());
		else
			DZBBC_Utils.Warn("Blackbox spawn failed.");
	}

	bool CanHack(PlayerBase player, Object target, DZBBC_MainConfig main)
	{
		if (!m_Unlocked || !DZBBC_PlayerUtils.IsValidPlayer(player) || !target || !m_Blackbox || target != m_Blackbox)
			return false;

		if (main && vector.Distance(player.GetPosition(), m_Blackbox.GetPosition()) > main.PlayerInteractionDistance)
			return false;

		return true;
	}

	EntityAI GiveDataCore(PlayerBase player)
	{
		if (!player || !player.GetInventory())
			return null;

		EntityAI dataCore = player.GetInventory().CreateInInventory(DZBBC_DATACORE_CLASSNAME);
		if (!dataCore && GetGame())
			dataCore = EntityAI.Cast(GetGame().CreateObjectEx(DZBBC_DATACORE_CLASSNAME, DZBBC_Utils.Grounded(player.GetPosition()), ECE_SETUP));

		return dataCore;
	}

	bool IsSpawned()
	{
		return m_Blackbox != null;
	}

	vector GetPosition()
	{
		if (m_Blackbox)
			return m_Blackbox.GetPosition();

		return "0 0 0";
	}

	void Cleanup()
	{
		DZBBC_Utils.DeleteObjectSafe(m_Blackbox);
		m_Blackbox = null;
		m_Unlocked = false;
	}
}
