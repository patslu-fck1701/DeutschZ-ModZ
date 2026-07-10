class DZBBC_DataCoreTracker
{
	protected PlayerBase m_Carrier;
	protected string m_CarrierUid;
	protected string m_CarrierName;
	protected float m_RemainingSeconds;
	protected bool m_Active;

	void Begin(PlayerBase player, float lifetimeSeconds)
	{
		m_Carrier = player;
		m_CarrierUid = DZBBC_PlayerUtils.GetPlayerUid(player);
		m_CarrierName = DZBBC_PlayerUtils.GetPlayerName(player);
		m_RemainingSeconds = lifetimeSeconds;
		if (m_RemainingSeconds <= 0.0)
			m_RemainingSeconds = 1800.0;
		m_Active = true;
	}

	bool Tick(float deltaSeconds)
	{
		if (!m_Active)
			return false;

		m_RemainingSeconds -= deltaSeconds;
		if (m_RemainingSeconds <= 0.0 || !DZBBC_PlayerUtils.IsValidPlayer(m_Carrier))
		{
			m_Active = false;
			return false;
		}

		DZBBC_ServerRPC.SendHud(m_Carrier, DZBBC_HUD_TRANSPORT, "#STR_DZBBC_MARKER_TRANSPORT", m_RemainingSeconds, 1800.0);
		return true;
	}

	bool IsCarrier(PlayerBase player)
	{
		return player && m_Active && DZBBC_PlayerUtils.GetPlayerUid(player) == m_CarrierUid;
	}

	PlayerBase GetCarrier()
	{
		return m_Carrier;
	}

	vector GetCarrierPosition()
	{
		if (m_Carrier)
			return m_Carrier.GetPosition();

		return "0 0 0";
	}

	float GetRemainingSeconds()
	{
		return m_RemainingSeconds;
	}

	string GetCarrierName()
	{
		return m_CarrierName;
	}

	void Reset()
	{
		m_Active = false;
		m_Carrier = null;
		m_CarrierUid = "";
		m_CarrierName = "";
		m_RemainingSeconds = 0.0;
	}
}
