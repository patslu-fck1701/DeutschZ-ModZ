modded class MainMenu
{
	static const string DZKOTHG_SERVER_IP = "193.135.10.126";
	static const int DZKOTHG_SERVER_PORT = 20076;
	static const string DZKOTHG_WEBSITE_URL = "https://project23947.websitepublisher.ai/";
	static const string DZKOTHG_DISCORD_URL = "https://discord.gg/FHzZ7BykFk";
	static const string DZKOTHG_VOTE_URL = "https://de.top-games.net/dayz/vote/deutschz-gunz-heliz-carz-traderz-httpsdiscordggfhzz7bykfk";
	static const string DZKOTHG_RULES_URL = "https://project23947.websitepublisher.ai/wiki.html";
	static const string DZKOTHG_EVENTS_URL = "https://project23947.websitepublisher.ai/kothz.html";
	static const string DZKOTHG_SHOP_URL = "https://project23947.websitepublisher.ai/shop.html";
	static const string DZKOTHG_MENU_LAYOUT = "DeutschZ_MenuMusic/gui/dzkothg_main_menu.layout";

	protected Widget m_DZKOTHG_VoteButton;
	protected Widget m_DZKOTHG_DiscordButton;
	protected TextWidget m_DZKOTHG_Tagline;
	protected MultilineTextWidget m_DZKOTHG_DescriptionBody;
	protected TextWidget m_DZKOTHG_StatusText;
	protected TextWidget m_DZKOTHG_NewsTitle;
	protected MultilineTextWidget m_DZKOTHG_NewsBody;
	protected float m_DZKOTHG_ContentTimer;
	protected Widget m_DZKOTHG_NavHome;
	protected Widget m_DZKOTHG_NavServer;
	protected Widget m_DZKOTHG_NavRules;
	protected Widget m_DZKOTHG_NavSupport;
	protected Widget m_DZKOTHG_NavEvents;
	protected Widget m_DZKOTHG_NavShop;
	protected Widget m_DZKOTHG_NavSettings;
	protected Widget m_DZKOTHG_NavProfile;
	protected Widget m_DZKOTHG_NavExit;

	override Widget Init()
	{
		Print("[DZKOTHG][CLIENT] MainMenu Init start");

		if (!GetGame() || !GetGame().GetWorkspace())
			return super.Init();

		layoutRoot = GetGame().GetWorkspace().CreateWidgets(DZKOTHG_MENU_LAYOUT);
		if (!layoutRoot)
		{
			Print("[DZKOTHG][CLIENT][ERROR] Custom menu layout missing: " + DZKOTHG_MENU_LAYOUT);
			return super.Init();
		}

		DZKOTHG_FindWidgets();
		if (!DZKOTHG_ValidateRequiredWidgets())
		{
			Print("[DZKOTHG][CLIENT][ERROR] Custom menu widgets missing, using vanilla menu.");
			layoutRoot.Unlink();
			layoutRoot = null;
			return super.Init();
		}

		DZKOTHG_BootVanillaMenuState();
		DZKOTHG_UpdateCustomText();

		Print("[DZKOTHG][CLIENT] MainMenu Init OK");
		return layoutRoot;
	}

	protected void DZKOTHG_FindWidgets()
	{
		m_Play                   = layoutRoot.FindAnyWidget("play");
		m_ChooseServer           = layoutRoot.FindAnyWidget("choose_server");
		m_CustomizeCharacter     = layoutRoot.FindAnyWidget("customize_character");
		m_PlayVideo              = layoutRoot.FindAnyWidget("play_video");
		m_Feedback               = layoutRoot.FindAnyWidget("feedback_button");
		m_Tutorials              = layoutRoot.FindAnyWidget("tutorials");
		m_TutorialButton         = layoutRoot.FindAnyWidget("tutorial_button");
		m_MessageButton          = layoutRoot.FindAnyWidget("message_button");
		m_SettingsButton         = layoutRoot.FindAnyWidget("settings_button");
		m_Exit                   = layoutRoot.FindAnyWidget("exit_button");
		m_PrevCharacter          = layoutRoot.FindAnyWidget("prev_character");
		m_NextCharacter          = layoutRoot.FindAnyWidget("next_character");
		m_CharacterRotationFrame = layoutRoot.FindAnyWidget("character_rotation_frame");
		m_DlcFrame               = layoutRoot.FindAnyWidget("dlc_Frame");
		m_NewsCarouselFrame      = layoutRoot.FindAnyWidget("carousel_Frame");

		m_DZKOTHG_VoteButton     = layoutRoot.FindAnyWidget("custom_button1");
		m_DZKOTHG_DiscordButton  = layoutRoot.FindAnyWidget("custom_button2");
		m_DZKOTHG_Tagline        = TextWidget.Cast(layoutRoot.FindAnyWidget("dz_tagline_text"));
		m_DZKOTHG_DescriptionBody = MultilineTextWidget.Cast(layoutRoot.FindAnyWidget("dz_description_body"));
		m_DZKOTHG_StatusText     = TextWidget.Cast(layoutRoot.FindAnyWidget("dz_status_text"));
		m_DZKOTHG_NewsTitle      = TextWidget.Cast(layoutRoot.FindAnyWidget("dz_news_title"));
		m_DZKOTHG_NewsBody       = MultilineTextWidget.Cast(layoutRoot.FindAnyWidget("dz_news_body"));
		m_DZKOTHG_NavHome        = layoutRoot.FindAnyWidget("dz_nav_home");
		m_DZKOTHG_NavServer      = layoutRoot.FindAnyWidget("dz_nav_server");
		m_DZKOTHG_NavRules       = layoutRoot.FindAnyWidget("dz_nav_rules");
		m_DZKOTHG_NavSupport     = layoutRoot.FindAnyWidget("dz_nav_support");
		m_DZKOTHG_NavEvents      = layoutRoot.FindAnyWidget("dz_nav_events");
		m_DZKOTHG_NavShop        = layoutRoot.FindAnyWidget("dz_nav_shop");
		m_DZKOTHG_NavSettings    = layoutRoot.FindAnyWidget("dz_nav_settings");
		m_DZKOTHG_NavProfile     = layoutRoot.FindAnyWidget("dz_nav_profile");
		m_DZKOTHG_NavExit        = layoutRoot.FindAnyWidget("dz_nav_exit");

		if (m_DZKOTHG_Tagline)
			m_DZKOTHG_Tagline.SetColor(DZKOTHG_UITheme.PrimaryText());
		if (m_DZKOTHG_DescriptionBody)
			m_DZKOTHG_DescriptionBody.SetColor(DZKOTHG_UITheme.PrimaryText());
		if (m_DZKOTHG_StatusText)
			m_DZKOTHG_StatusText.SetColor(DZKOTHG_UITheme.BrandGreen());
		if (m_DZKOTHG_NewsTitle)
			m_DZKOTHG_NewsTitle.SetColor(DZKOTHG_UITheme.BrandGreen());
		if (m_DZKOTHG_NewsBody)
			m_DZKOTHG_NewsBody.SetColor(DZKOTHG_UITheme.PrimaryText());

		m_Version                = TextWidget.Cast(layoutRoot.FindAnyWidget("version"));
		m_ModdedWarning          = TextWidget.Cast(layoutRoot.FindAnyWidget("ModdedWarning"));
		m_PlayerName             = TextWidget.Cast(layoutRoot.FindAnyWidget("character_name_text"));
		m_LastPlayedTooltip      = layoutRoot.FindAnyWidget("last_server_info");

		if (m_LastPlayedTooltip)
		{
			m_LastPlayedTooltip.Show(false);
			m_LastPlayedTooltipLabel = m_LastPlayedTooltip.FindAnyWidget("last_server_info_label");
			m_LastPlayedTooltipName  = TextWidget.Cast(m_LastPlayedTooltip.FindAnyWidget("last_server_info_name"));
			m_LastPlayedTooltipIP    = TextWidget.Cast(m_LastPlayedTooltip.FindAnyWidget("last_server_info_ip"));
			m_LastPlayedTooltipPort  = TextWidget.Cast(m_LastPlayedTooltip.FindAnyWidget("last_server_info_port"));
		}
	}

	protected bool DZKOTHG_ValidateRequiredWidgets()
	{
		if (!m_Play || !m_ChooseServer || !m_CustomizeCharacter || !m_SettingsButton || !m_Exit || !m_CharacterRotationFrame)
			return false;

		if (!layoutRoot.FindAnyWidget("character_stats_root"))
			return false;

		return true;
	}

	protected void DZKOTHG_BootVanillaMenuState()
	{
		m_LastPlayedTooltipTimer = new WidgetFadeTimer();
		m_Stats = new MainMenuStats(layoutRoot.FindAnyWidget("character_stats_root"));
		m_Mission = MissionMainMenu.Cast(GetGame().GetMission());
		m_LastFocusedButton = m_Play;

		if (m_Mission)
		{
			m_ScenePC = m_Mission.GetIntroScenePC();
			if (m_ScenePC)
			{
				m_ScenePC.ResetIntroCamera();
				DZKOTHG_ZoomCharacterOut();
			}
		}

		if (m_PlayVideo)
			m_PlayVideo.Show(false);

		if (m_DlcFrame)
			m_DlcFrame.Show(false);

		if (m_NewsCarouselFrame)
			m_NewsCarouselFrame.Show(false);

		string version;
		GetGame().GetVersion(version);

		if (m_Version)
			m_Version.SetText("DeutschZ Menu | DayZ " + version);

		if (GetGame().GetUIManager())
			GetGame().GetUIManager().ScreenFadeOut(0);

		SetFocus(null);
		Refresh();

		if (m_ModdedWarning)
		{
			m_ModdedWarning.SetText("DeutschZ - Server #1");
			m_ModdedWarning.SetColor(DZKOTHG_UITheme.BrandRed());
			m_ModdedWarning.SetTextExactSize(18);
			m_ModdedWarning.Show(true);
		}

		if (m_DZKOTHG_NewsTitle)
			m_DZKOTHG_NewsTitle.SetTextExactSize(20);
		if (m_DZKOTHG_NewsBody)
			m_DZKOTHG_NewsBody.SetTextExactSize(14);

		GetGame().SetLoadState(DayZLoadState.MAIN_MENU_CONTROLLER_SELECT);
	}

	protected void DZKOTHG_ZoomCharacterOut()
	{
		if (!m_ScenePC || !m_ScenePC.GetIntroCamera() || !m_ScenePC.GetIntroCharacter())
			return;

		IntroSceneCharacter introCharacter = m_ScenePC.GetIntroCharacter();
		if (!introCharacter.GetCharacterObj())
			return;

		Camera introCamera = m_ScenePC.GetIntroCamera();
		vector characterPosition = introCharacter.GetPosition();
		vector cameraOffset = introCamera.GetPosition() - characterPosition;
		introCamera.SetPosition(characterPosition + (cameraOffset * 1.08));
		introCamera.LookAt(characterPosition + Vector(0, 1, 0));
	}

	protected void DZKOTHG_UpdateCustomText()
	{
		if (m_DZKOTHG_Tagline)
		{
			m_DZKOTHG_Tagline.SetText("DEUTSCHZ COMMUNITY");
			m_DZKOTHG_Tagline.SetTextExactSize(22);
		}

		if (m_DZKOTHG_DescriptionBody)
		{
			m_DZKOTHG_DescriptionBody.SetText("Survival, Events und Community auf Chernarus.\nDein Server. Deine Regeln. Dein Ueberleben.");
			m_DZKOTHG_DescriptionBody.SetTextExactSize(16);
		}

		if (m_DZKOTHG_StatusText)
		{
			m_DZKOTHG_StatusText.SetText("ONLINE | " + DZKOTHG_SERVER_IP + ":" + DZKOTHG_SERVER_PORT.ToString());
			m_DZKOTHG_StatusText.SetTextExactSize(15);
		}

		DZKOTHG_NewsEntry news = DZKOTHG_MenuContentStore.GetNextNews();
		if (news)
		{
			if (m_DZKOTHG_NewsTitle)
				m_DZKOTHG_NewsTitle.SetText("SERVERSTATUS");
			if (m_DZKOTHG_NewsBody)
				m_DZKOTHG_NewsBody.SetText("ONLINE | " + DZKOTHG_SERVER_IP + ":" + DZKOTHG_SERVER_PORT.ToString() + "\n" + news.Title + " | " + news.Body);
		}
	}

	override void Update(float timeslice)
	{
		super.Update(timeslice);
		m_DZKOTHG_ContentTimer += timeslice;
		if (m_DZKOTHG_ContentTimer >= 8.0)
		{
			m_DZKOTHG_ContentTimer = 0.0;
			DZKOTHG_UpdateCustomText();
		}
	}

	protected void DZKOTHG_ConnectToServer()
	{
		if (!GetGame())
			return;

		Print("[DZKOTHG][CLIENT] Direct Connect -> " + DZKOTHG_SERVER_IP + ":" + DZKOTHG_SERVER_PORT.ToString());
		GetGame().ConnectFromServerBrowser(DZKOTHG_SERVER_IP, DZKOTHG_SERVER_PORT, "");
	}

	protected void DZKOTHG_SetMenuImage(string widgetName, string imagePath)
	{
		if (!layoutRoot)
			return;

		ImageWidget image = ImageWidget.Cast(layoutRoot.FindAnyWidget(widgetName));
		if (!image)
			return;

		image.LoadImageFile(0, imagePath);
		image.SetImage(0);
	}

	protected void DZKOTHG_SetNavIcon(Widget button, string name, bool hovered)
	{
		if (!button)
			return;
		string state = "normal";
		if (hovered)
			state = "hover";
		DZKOTHG_SetMenuImage("dz_nav_" + name + "_icon", "DeutschZ_MenuMusic/gui/menu_assets/icons/03_icon_" + name + "_" + state + ".paa");
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (w == m_Play)
			DZKOTHG_SetMenuImage("play_image", "DeutschZ_MenuMusic/gui/darkglass_v1/02_buttons_text/play_now_hover.paa");
		else if (w == m_ChooseServer)
			DZKOTHG_SetMenuImage("choose_server_image", "DeutschZ_MenuMusic/gui/darkglass_v1/02_buttons_text/server_suchen_hover.paa");
		else if (w == m_CustomizeCharacter)
			DZKOTHG_SetMenuImage("customize_image", "DeutschZ_MenuMusic/gui/darkglass_v1/02_buttons_text/charakter_hover.paa");
		else if (w == m_DZKOTHG_VoteButton)
			DZKOTHG_SetMenuImage("vote_image", "DeutschZ_MenuMusic/gui/darkglass_v1/02_buttons_text/vote_hover.paa");
		else if (w == m_SettingsButton)
			DZKOTHG_SetMenuImage("settings_image", "DeutschZ_MenuMusic/gui/menu_assets/icons/03_icon_settings_hover.paa");
		else if (w == m_Exit)
			DZKOTHG_SetMenuImage("exit_image", "DeutschZ_MenuMusic/gui/menu_assets/icons/03_icon_power_hover.paa");
		else if (w == m_DZKOTHG_DiscordButton)
			DZKOTHG_SetMenuImage("discord_image", "DeutschZ_MenuMusic/gui/darkglass_v1/02_buttons_text/discord_hover.paa");
		else if (w == m_MessageButton)
			DZKOTHG_SetMenuImage("website_image", "DeutschZ_MenuMusic/gui/darkglass_v1/02_buttons_text/website_hover.paa");
		else if (w == m_PrevCharacter)
			DZKOTHG_SetMenuImage("prev_img", "DeutschZ_MenuMusic/gui/menu_assets/arrows/19_arrow_left_hover.paa");
		else if (w == m_NextCharacter)
			DZKOTHG_SetMenuImage("next_img", "DeutschZ_MenuMusic/gui/menu_assets/arrows/20_arrow_right_hover.paa");
		else if (w == m_DZKOTHG_NavHome)
			DZKOTHG_SetNavIcon(w, "home", true);
		else if (w == m_DZKOTHG_NavServer)
			DZKOTHG_SetNavIcon(w, "server", true);
		else if (w == m_DZKOTHG_NavRules)
			DZKOTHG_SetNavIcon(w, "rules", true);
		else if (w == m_DZKOTHG_NavSupport)
			DZKOTHG_SetNavIcon(w, "support", true);
		else if (w == m_DZKOTHG_NavEvents)
			DZKOTHG_SetNavIcon(w, "events", true);
		else if (w == m_DZKOTHG_NavShop)
			DZKOTHG_SetNavIcon(w, "shop", true);
		else if (w == m_DZKOTHG_NavSettings)
			DZKOTHG_SetNavIcon(w, "settings", true);
		else if (w == m_DZKOTHG_NavProfile)
			DZKOTHG_SetNavIcon(w, "profile", true);
		else if (w == m_DZKOTHG_NavExit)
			DZKOTHG_SetMenuImage("dz_nav_exit_icon", "DeutschZ_MenuMusic/gui/menu_assets/icons/03_icon_power_hover.paa");

		return super.OnMouseEnter(w, x, y);
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (w == m_Play)
			DZKOTHG_SetMenuImage("play_image", "DeutschZ_MenuMusic/gui/darkglass_v1/02_buttons_text/play_now_normal.paa");
		else if (w == m_ChooseServer)
			DZKOTHG_SetMenuImage("choose_server_image", "DeutschZ_MenuMusic/gui/darkglass_v1/02_buttons_text/server_suchen_normal.paa");
		else if (w == m_CustomizeCharacter)
			DZKOTHG_SetMenuImage("customize_image", "DeutschZ_MenuMusic/gui/darkglass_v1/02_buttons_text/charakter_normal.paa");
		else if (w == m_DZKOTHG_VoteButton)
			DZKOTHG_SetMenuImage("vote_image", "DeutschZ_MenuMusic/gui/darkglass_v1/02_buttons_text/vote_normal.paa");
		else if (w == m_SettingsButton)
			DZKOTHG_SetMenuImage("settings_image", "DeutschZ_MenuMusic/gui/menu_assets/icons/03_icon_settings_normal.paa");
		else if (w == m_Exit)
			DZKOTHG_SetMenuImage("exit_image", "DeutschZ_MenuMusic/gui/menu_assets/icons/03_icon_power_normal.paa");
		else if (w == m_DZKOTHG_DiscordButton)
			DZKOTHG_SetMenuImage("discord_image", "DeutschZ_MenuMusic/gui/darkglass_v1/02_buttons_text/discord_normal.paa");
		else if (w == m_MessageButton)
			DZKOTHG_SetMenuImage("website_image", "DeutschZ_MenuMusic/gui/darkglass_v1/02_buttons_text/website_normal.paa");
		else if (w == m_PrevCharacter)
			DZKOTHG_SetMenuImage("prev_img", "DeutschZ_MenuMusic/gui/menu_assets/arrows/19_arrow_left_normal.paa");
		else if (w == m_NextCharacter)
			DZKOTHG_SetMenuImage("next_img", "DeutschZ_MenuMusic/gui/menu_assets/arrows/20_arrow_right_normal.paa");
		else if (w == m_DZKOTHG_NavHome)
			DZKOTHG_SetNavIcon(w, "home", false);
		else if (w == m_DZKOTHG_NavServer)
			DZKOTHG_SetNavIcon(w, "server", false);
		else if (w == m_DZKOTHG_NavRules)
			DZKOTHG_SetNavIcon(w, "rules", false);
		else if (w == m_DZKOTHG_NavSupport)
			DZKOTHG_SetNavIcon(w, "support", false);
		else if (w == m_DZKOTHG_NavEvents)
			DZKOTHG_SetNavIcon(w, "events", false);
		else if (w == m_DZKOTHG_NavShop)
			DZKOTHG_SetNavIcon(w, "shop", false);
		else if (w == m_DZKOTHG_NavSettings)
			DZKOTHG_SetNavIcon(w, "settings", false);
		else if (w == m_DZKOTHG_NavProfile)
			DZKOTHG_SetNavIcon(w, "profile", false);
		else if (w == m_DZKOTHG_NavExit)
			DZKOTHG_SetMenuImage("dz_nav_exit_icon", "DeutschZ_MenuMusic/gui/menu_assets/icons/03_icon_power_normal.paa");

		return super.OnMouseLeave(w, enterW, x, y);
	}

	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		if (w && w == m_CharacterRotationFrame)
		{
			if (m_ScenePC)
				m_ScenePC.CharacterRotationStart();

			return true;
		}

		return super.OnMouseButtonDown(w, x, y, button);
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (!w || button != MouseState.LEFT)
			return super.OnClick(w, x, y, button);

		if (w == m_Play)
		{
			m_LastFocusedButton = m_Play;
			DZKOTHG_ConnectToServer();
			return true;
		}

		if (w == m_ChooseServer)
		{
			m_LastFocusedButton = m_ChooseServer;
			OpenMenuServerBrowser();
			return true;
		}

		if (w == m_CustomizeCharacter)
		{
			OpenMenuCustomizeCharacter();
			return true;
		}

		if (w == m_TutorialButton)
		{
			OpenTutorials();
			return true;
		}

		if (w == m_MessageButton)
		{
			GetGame().OpenURL(DZKOTHG_WEBSITE_URL);
			return true;
		}

		if (w == m_SettingsButton)
		{
			OpenSettings();
			return true;
		}

		if (w == m_Exit)
		{
			Exit();
			return true;
		}

		if (w == m_PrevCharacter)
		{
			PreviousCharacter();
			return true;
		}

		if (w == m_NextCharacter)
		{
			NextCharacter();
			return true;
		}

		if (w == m_PlayVideo)
		{
			m_LastFocusedButton = m_PlayVideo;
			PlayVideo();
			return true;
		}

		if (w == m_Tutorials)
		{
			m_LastFocusedButton = m_Tutorials;
			OpenTutorials();
			return true;
		}

		if (w == m_Feedback)
		{
			GetGame().OpenURL(DZKOTHG_WEBSITE_URL);
			return true;
		}

		if (w == m_DZKOTHG_VoteButton)
		{
			GetGame().OpenURL(DZKOTHG_VOTE_URL);
			return true;
		}

		if (w == m_DZKOTHG_DiscordButton)
		{
			GetGame().OpenURL(DZKOTHG_DISCORD_URL);
			return true;
		}

		if (w == m_DZKOTHG_NavHome)
		{
			DZKOTHG_UpdateCustomText();
			return true;
		}

		if (w == m_DZKOTHG_NavServer)
		{
			OpenMenuServerBrowser();
			return true;
		}

		if (w == m_DZKOTHG_NavRules)
		{
			GetGame().OpenURL(DZKOTHG_RULES_URL);
			return true;
		}

		if (w == m_DZKOTHG_NavSupport)
		{
			GetGame().OpenURL(DZKOTHG_DISCORD_URL);
			return true;
		}

		if (w == m_DZKOTHG_NavEvents)
		{
			GetGame().OpenURL(DZKOTHG_EVENTS_URL);
			return true;
		}

		if (w == m_DZKOTHG_NavShop)
		{
			GetGame().OpenURL(DZKOTHG_SHOP_URL);
			return true;
		}

		if (w == m_DZKOTHG_NavSettings)
		{
			OpenSettings();
			return true;
		}

		if (w == m_DZKOTHG_NavProfile)
		{
			OpenMenuCustomizeCharacter();
			return true;
		}

		if (w == m_DZKOTHG_NavExit)
		{
			Exit();
			return true;
		}

		return super.OnClick(w, x, y, button);
	}

	override bool IsFocusable(Widget w)
	{
		if (!w)
			return false;

		if (w == m_Play || w == m_ChooseServer || w == m_CustomizeCharacter || w == m_TutorialButton || w == m_MessageButton || w == m_SettingsButton)
			return true;

		if (w == m_DZKOTHG_VoteButton || w == m_DZKOTHG_DiscordButton || w == m_Exit || w == m_PlayVideo || w == m_Feedback)
			return true;

		if (w == m_NewsMain || w == m_NewsSec1 || w == m_NewsSec2 || w == m_PrevCharacter || w == m_NextCharacter)
			return true;

		if (w == m_DZKOTHG_NavHome || w == m_DZKOTHG_NavServer || w == m_DZKOTHG_NavRules || w == m_DZKOTHG_NavSupport || w == m_DZKOTHG_NavEvents || w == m_DZKOTHG_NavShop || w == m_DZKOTHG_NavSettings || w == m_DZKOTHG_NavProfile || w == m_DZKOTHG_NavExit)
			return true;

		return super.IsFocusable(w);
	}

	override void Play()
	{
		DZKOTHG_ConnectToServer();
	}
};
