class DZMV2_UIAssets
{
	static const string ROOT = "DeutschZ_MenuMusic_V2_Test/gui/mainmenu_v2_pack";
	static const string PANEL_TALL = ROOT + "/panels/panel_tall_default_v1_1x1.paa";
	static const string PANEL_WIDE = ROOT + "/panels/panel_wide_default_v1_1x1.paa";
	static const string PANEL_RECT = ROOT + "/panels/panel_rect_default_v1_1x1.paa";
	static const string BUTTON_DEFAULT = ROOT + "/buttons/button_shape01_default_1x1.paa";
	static const string BUTTON_HOVER = ROOT + "/buttons/button_shape01_hover_1x1.paa";
	static const string BUTTON_PRESSED = ROOT + "/buttons/button_shape01_pressed_1x1.paa";
	static const string BUTTON_ACTIVE = ROOT + "/buttons/button_shape01_active_1x1.paa";

	static string Button(string state)
	{
		if (state == "hover")
			return BUTTON_HOVER;
		if (state == "pressed")
			return BUTTON_PRESSED;
		if (state == "active")
			return BUTTON_ACTIVE;

		return BUTTON_DEFAULT;
	}

	static string Icon(string name, bool active = false)
	{
		if (name == "settings")
			name = "options";
		else if (name == "shop")
			name = "website";

		string color = "white";
		if (active)
			color = "green";

		return ROOT + "/icons/icon_" + name + "_" + color + "_1x1.paa";
	}

	static string EventCard(int variant)
	{
		if (variant < 1)
			variant = 1;
		else if (variant > 4)
			variant = 4;
		return ROOT + "/event_cards/eventcard_blank_variant_0" + variant.ToString() + "_1x1.paa";
	}
}
