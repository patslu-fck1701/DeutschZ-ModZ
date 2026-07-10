modded class MapMenu
{
	override void OnShow()
	{
		super.OnShow();
		DZKOTH_ClientMarkerStore.Get().RenderVanilla(m_MapWidgetInstance);
		DZEV_ClientMarkerStore.Get().RenderVanilla(m_MapWidgetInstance);
	}
}
