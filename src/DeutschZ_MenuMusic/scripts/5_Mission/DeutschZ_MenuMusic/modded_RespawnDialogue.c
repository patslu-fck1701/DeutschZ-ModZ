modded class RespawnDialogue
{
	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("DeutschZ_MenuMusic/gui/dzkothg_respawn.layout");
		if (!layoutRoot)
			return super.Init();

		m_DetailsRoot = layoutRoot.FindAnyWidget("menu_details_tooltip");
		if (m_DetailsRoot)
		{
			m_DetailsLabel = TextWidget.Cast(m_DetailsRoot.FindAnyWidget("menu_details_label"));
			m_DetailsText = RichTextWidget.Cast(m_DetailsRoot.FindAnyWidget("menu_details_tooltip_content"));
		}

		m_CustomRespawn = layoutRoot.FindAnyWidget("respawn_button_custom");
		SetFocus(m_CustomRespawn);
		Print("[DZKOTHG][CLIENT] DeutschZ respawn screen active.");
		return layoutRoot;
	}

	protected void DZKOTHG_SetRespawnImage(Widget button, string imagePath)
	{
		if (!button)
			return;

		ImageWidget image = ImageWidget.Cast(button.FindAnyWidget(button.GetName() + "_image"));
		if (!image)
			return;

		image.LoadImageFile(0, imagePath);
		image.SetImage(0);
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (!w)
			return false;

		super.OnMouseEnter(w, x, y);

		if (w.GetUserID() == ID_RESPAWN_CUSTOM)
			DZKOTHG_SetRespawnImage(w, "DeutschZ_MenuMusic/gui/deathscreen_v1/03_buttons/button_respawn_hover.paa");
		else if (w.GetUserID() == ID_RESPAWN_RANDOM)
			DZKOTHG_SetRespawnImage(w, "DeutschZ_MenuMusic/gui/deathscreen_v1/03_buttons/button_zufaelliger_spawn_hover.paa");
		else if (w.GetUserID() == IDC_CANCEL)
			DZKOTHG_SetRespawnImage(w, "DeutschZ_MenuMusic/gui/deathscreen_v1/03_buttons/button_abbrechen_hover.paa");

		return true;
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (!w)
			return false;

		super.OnMouseLeave(w, enterW, x, y);

		if (w.GetUserID() == ID_RESPAWN_CUSTOM)
			DZKOTHG_SetRespawnImage(w, "DeutschZ_MenuMusic/gui/deathscreen_v1/03_buttons/button_respawn_normal.paa");
		else if (w.GetUserID() == ID_RESPAWN_RANDOM)
			DZKOTHG_SetRespawnImage(w, "DeutschZ_MenuMusic/gui/deathscreen_v1/03_buttons/button_zufaelliger_spawn_normal.paa");
		else if (w.GetUserID() == IDC_CANCEL)
			DZKOTHG_SetRespawnImage(w, "DeutschZ_MenuMusic/gui/deathscreen_v1/03_buttons/button_abbrechen_normal.paa");

		return true;
	}
}
