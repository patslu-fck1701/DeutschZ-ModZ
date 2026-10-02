modded class LogoutMenu
{
	protected TextWidget m_DZPVPUI_LogoutTime;
	protected TextWidget m_DZPVPUI_LogoutDescription;
	protected TextWidget m_DZPVPUI_LogoutJoke;
	protected ButtonWidget m_DZPVPUI_LogoutNow;
	protected ButtonWidget m_DZPVPUI_LogoutCancel;
	protected int m_DZPVPUI_LogoutSeconds;
	protected ref array<string> m_DZPVPUI_LogoutJokes;

	override Widget Init()
	{
		Widget vanillaRoot = super.Init();
		if (vanillaRoot)
			vanillaRoot.Unlink();

		layoutRoot = g_Game.GetWorkspace().CreateWidgets("deutschz_hudz/gui/layouts/deutschz_hudz_logout.layout");
		m_DZPVPUI_LogoutTime = TextWidget.Cast(layoutRoot.FindAnyWidget("txtLogoutTime"));
		m_DZPVPUI_LogoutDescription = TextWidget.Cast(layoutRoot.FindAnyWidget("txtDescription"));
		m_DZPVPUI_LogoutJoke = TextWidget.Cast(layoutRoot.FindAnyWidget("dzpvpui_logout_joke"));
		m_DZPVPUI_LogoutNow = ButtonWidget.Cast(layoutRoot.FindAnyWidget("bLogoutNow"));
		m_DZPVPUI_LogoutCancel = ButtonWidget.Cast(layoutRoot.FindAnyWidget("bCancel"));

		m_DZPVPUI_LogoutJokes = new array<string>();
		m_DZPVPUI_LogoutJokes.Insert("Bis gleich. Wir wissen beide, dass du wiederkommst.");
		m_DZPVPUI_LogoutJokes.Insert("Dein Loot bleibt hier. Deine Ausrede darf mit.");
		m_DZPVPUI_LogoutJokes.Insert("Chernarus merkt sich, wer zuerst ausloggt.");
		m_DZPVPUI_LogoutJokes.Insert("Flucht ist auch eine Taktik. Nur keine gute.");
		m_DZPVPUI_LogoutJokes.Insert("Alt+F4 ist kein Evakuierungsplan.");
		m_DZPVPUI_LogoutJokes.Insert("Dein Charakter zaehlt runter. Die Zombies nicht.");
		m_DZPVPUI_LogoutJokes.Insert("DeutschZ macht keine Pause. Nur du.");
		m_DZPVPUI_LogoutJokes.Insert("Abmelden erfolgreich vorbereitet. Vermisst wirst du spaeter.");

		if (m_DZPVPUI_LogoutJoke && m_DZPVPUI_LogoutJokes.Count() > 0)
			m_DZPVPUI_LogoutJoke.SetText(m_DZPVPUI_LogoutJokes.Get(Math.RandomInt(0, m_DZPVPUI_LogoutJokes.Count())));

		UpdateInfo();
		Print("[DZPVPUI][LOGOUT] DeutschZ logout dialog initialized");
		return layoutRoot;
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (w == m_DZPVPUI_LogoutNow)
		{
			Print("[DZPVPUI][LOGOUT] immediate exit requested");
			g_Game.GetMission().AbortMission();
			return true;
		}

		if (w == m_DZPVPUI_LogoutCancel)
		{
			Print("[DZPVPUI][LOGOUT] countdown cancelled");
			Hide();
			Cancel();
			return true;
		}

		return super.OnClick(w, x, y, button);
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (w == m_DZPVPUI_LogoutNow)
			DZPVPUI_SetLogoutButtonImage("dzpvpui_logout_now_image", "deutschz_hudz/gui/menu_green1/component_set/buttons/wide_red/wide_red_hover.paa");
		else if (w == m_DZPVPUI_LogoutCancel)
			DZPVPUI_SetLogoutButtonImage("dzpvpui_logout_cancel_image", "deutschz_hudz/gui/menu_green1/component_set/buttons/wide/button_wide_hover.paa");

		return super.OnMouseEnter(w, x, y);
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (w == m_DZPVPUI_LogoutNow)
			DZPVPUI_SetLogoutButtonImage("dzpvpui_logout_now_image", "deutschz_hudz/gui/menu_green1/component_set/buttons/wide_red/wide_red_normal.paa");
		else if (w == m_DZPVPUI_LogoutCancel)
			DZPVPUI_SetLogoutButtonImage("dzpvpui_logout_cancel_image", "deutschz_hudz/gui/menu_green1/component_set/buttons/wide/button_wide_normal.paa");

		return super.OnMouseLeave(w, enterW, x, y);
	}

	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		if (button == MouseState.LEFT)
		{
			if (w == m_DZPVPUI_LogoutNow)
				DZPVPUI_SetLogoutButtonImage("dzpvpui_logout_now_image", "deutschz_hudz/gui/menu_green1/component_set/buttons/wide_red/wide_red_pressed.paa");
			else if (w == m_DZPVPUI_LogoutCancel)
				DZPVPUI_SetLogoutButtonImage("dzpvpui_logout_cancel_image", "deutschz_hudz/gui/menu_green1/component_set/buttons/wide/button_wide_pressed.paa");
		}

		return super.OnMouseButtonDown(w, x, y, button);
	}

	override void SetLogoutTime()
	{
		if (m_DZPVPUI_LogoutTime)
			m_DZPVPUI_LogoutTime.SetText(" ");
	}

	override void SetTime(int time)
	{
		m_DZPVPUI_LogoutSeconds = time;
		if (m_DZPVPUI_LogoutTime)
			m_DZPVPUI_LogoutTime.SetText("VERLASSEN IN " + time.ToString() + " SEKUNDEN");
	}

	override void UpdateTime()
	{
		if (m_DZPVPUI_LogoutSeconds > 0)
			SetTime(--m_DZPVPUI_LogoutSeconds);
		else
			Exit();
	}

	override void UpdateInfo()
	{
		if (!m_DZPVPUI_LogoutDescription)
			return;

		PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());
		if (player && (player.IsRestrained() || player.IsUnconscious()))
			m_DZPVPUI_LogoutDescription.SetText("Dein Charakter bleibt schutzlos auf dem Server. Das ist jetzt wirklich mutig.");
		else
			m_DZPVPUI_LogoutDescription.SetText("Der Charakter bleibt waehrend des Countdowns auf dem Server.");
	}

	protected void DZPVPUI_SetLogoutButtonImage(string widgetName, string imagePath)
	{
		if (!layoutRoot)
			return;

		ImageWidget image = ImageWidget.Cast(layoutRoot.FindAnyWidget(widgetName));
		if (!image)
			return;

		image.LoadImageFile(0, imagePath);
		image.SetImage(0);
	}
}
