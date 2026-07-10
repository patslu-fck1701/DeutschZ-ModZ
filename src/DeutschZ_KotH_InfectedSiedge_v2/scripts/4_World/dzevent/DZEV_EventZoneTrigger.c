class DZEV_EventZoneTrigger : SphereTrigger
{
	protected DZEV_EventController m_Controller;
	protected float m_DZEV_Radius;

	void Setup(DZEV_EventController controller, float radius)
	{
		m_Controller = controller;
		m_DZEV_Radius = radius;
		SetCollisionSphere(radius);
	}

	override void EOnInit(IEntity other, int extra)
	{
		if (m_DZEV_Radius > 0.0)
			SetCollisionSphere(m_DZEV_Radius);
		else
			SetCollisionSphere(3.0);
	}

	override protected bool CanAddObjectAsInsider(Object object)
	{
		return PlayerBase.Cast(object) != null;
	}

	override protected void OnEnterServerEvent(TriggerInsider insider)
	{
		if (!m_Controller || !insider)
			return;

		PlayerBase player = PlayerBase.Cast(insider.GetObject());
		if (player)
			m_Controller.OnPlayerEntered(player);
	}

	override protected void OnStayServerEvent(TriggerInsider insider, float deltaTime)
	{
	}

	override protected void OnLeaveServerEvent(TriggerInsider insider)
	{
		if (!m_Controller || !insider)
			return;

		PlayerBase player = PlayerBase.Cast(insider.GetObject());
		if (player)
			m_Controller.OnPlayerLeft(player);
	}
}
