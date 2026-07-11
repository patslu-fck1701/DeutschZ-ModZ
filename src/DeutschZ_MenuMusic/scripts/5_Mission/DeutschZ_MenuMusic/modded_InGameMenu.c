modded class InGameMenu
{
	protected Widget m_DZKOTHG_DeathBackdrop;

	override Widget Init()
	{
		Widget root = super.Init();
		DZKOTHG_ApplyDeathTheme();
		return root;
	}

	protected void DZKOTHG_ApplyDeathTheme()
	{
		if (!layoutRoot || !GetGame() || !GetGame().GetWorkspace())
			return;

		Man player = GetGame().GetPlayer();
		if (!player || player.GetPlayerState() == EPlayerStates.ALIVE)
			return;

		Widget vanillaVignette = layoutRoot.FindAnyWidget("vignette");
		if (vanillaVignette)
			vanillaVignette.Show(false);

		Widget vanillaLogo = layoutRoot.FindAnyWidget("dayz_logo");
		if (vanillaLogo)
			vanillaLogo.Show(false);

		Widget licensing = layoutRoot.FindAnyWidget("Licensing");
		if (licensing)
			licensing.Show(false);

		Widget separator = layoutRoot.FindAnyWidget("separator_red");
		if (separator)
			separator.SetColor(ARGB(255, 132, 240, 20));

		m_DZKOTHG_DeathBackdrop = GetGame().GetWorkspace().CreateWidgets("DeutschZ_MenuMusic/gui/dzkothg_death_backdrop.layout", layoutRoot);
		Print("[DZKOTHG][CLIENT] DeutschZ death screen active");
	}
}
