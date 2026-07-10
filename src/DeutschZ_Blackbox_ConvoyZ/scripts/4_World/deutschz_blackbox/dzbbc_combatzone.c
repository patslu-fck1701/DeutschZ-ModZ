class DZBBC_CombatZone : SphereTrigger
{
	protected DZBBC_EventInstance m_Instance;
	protected float m_Radius;

	void Setup(DZBBC_EventInstance instance, float radius)
	{
		m_Instance = instance;
		m_Radius = radius;
		SetCollisionSphere(radius);
	}

	override void EOnInit(IEntity other, int extra)
	{
		if (m_Radius > 0.0)
			SetCollisionSphere(m_Radius);
		else
			SetCollisionSphere(10.0);
	}

	override protected bool CanAddObjectAsInsider(Object object)
	{
		return PlayerBase.Cast(object) != null;
	}

	override protected void OnEnterServerEvent(TriggerInsider insider)
	{
		if (!m_Instance || !insider)
			return;

		PlayerBase player = PlayerBase.Cast(insider.GetObject());
		if (player)
			m_Instance.OnPlayerEntered(player);
	}

	override protected void OnStayServerEvent(TriggerInsider insider, float deltaTime)
	{
	}

	override protected void OnLeaveServerEvent(TriggerInsider insider)
	{
		if (!m_Instance || !insider)
			return;

		PlayerBase player = PlayerBase.Cast(insider.GetObject());
		if (player)
			m_Instance.OnPlayerLeft(player);
	}
}
