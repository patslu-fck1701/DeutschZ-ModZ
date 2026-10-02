class CourierZ_ScientificCase : ScientificBriefcase
{
	protected bool m_DZCourierInitialFill;

	void DZCourierSetInitialFill(bool enabled)
	{
		m_DZCourierInitialFill = enabled;
	}

	override bool CanReceiveItemIntoCargo(EntityAI item)
	{
		if (m_DZCourierInitialFill && item && item.IsKindOf("ExpansionBanknoteEuro"))
			return true;
		return super.CanReceiveItemIntoCargo(item);
	}
}
