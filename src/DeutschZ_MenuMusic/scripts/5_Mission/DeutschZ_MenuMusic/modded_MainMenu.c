modded class MainMenu
{
	static const string DZKOTHG_SERVER_IP = "193.135.10.126";
	static const int DZKOTHG_SERVER_PORT = 20076;
	static const string DZKOTHG_WEBSITE_URL = "https://project23947.websitepublisher.ai/";
	static const string DZKOTHG_DISCORD_URL = "https://discord.gg/FHzZ7BykFk";
	static const string DZKOTHG_VOTE_URL = "https://de.top-games.net/dayz/vote/deutschz-gunz-heliz-carz-traderz-httpsdiscordggfhzz7bykfk";
	static const string DZKOTHG_MENU_LAYOUT = "DeutschZ_MenuMusic/gui/dzkothg_main_menu.layout";

	protected Widget m_DZKOTHG_VoteButton;
	protected Widget m_DZKOTHG_DiscordButton;
	protected TextWidget m_DZKOTHG_Tagline;
	protected TextWidget m_DZKOTHG_StatusText;
	protected TextWidget m_DZKOTHG_NewsTitle;
	protected MultilineTextWidget m_DZKOTHG_NewsBody;

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
		m_DZKOTHG_StatusText     = TextWidget.Cast(layoutRoot.FindAnyWidget("dz_status_text"));
		m_DZKOTHG_NewsTitle      = TextWidget.Cast(layoutRoot.FindAnyWidget("dz_news_title"));
		m_DZKOTHG_NewsBody       = MultilineTextWidget.Cast(layoutRoot.FindAnyWidget("dz_news_body"));

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
				m_ScenePC.ResetIntroCamera();
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

		if (m_ModdedWarning)
		{
			m_ModdedWarning.SetText("DeutschZModZ");
			m_ModdedWarning.Show(false);
		}

		if (GetGame().GetUIManager())
			GetGame().GetUIManager().ScreenFadeOut(0);

		SetFocus(null);
		Refresh();
		GetGame().SetLoadState(DayZLoadState.MAIN_MENU_CONTROLLER_SELECT);
	}

	protected void DZKOTHG_UpdateCustomText()
	{
		if (m_DZKOTHG_Tagline)
			m_DZKOTHG_Tagline.SetText("GUNZ | HELIZ | CARZ | TRADERZ");

		if (m_DZKOTHG_StatusText)
			m_DZKOTHG_StatusText.SetText("ONLINE " + DZKOTHG_SERVER_IP + ":" + DZKOTHG_SERVER_PORT.ToString());

		if (m_DZKOTHG_NewsTitle)
			m_DZKOTHG_NewsTitle.SetText("DEUTSCHZ LIVE");

		if (m_DZKOTHG_NewsBody)
			m_DZKOTHG_NewsBody.SetText("KOTH | CONVOYZ | EVENTS\nDiscord, Website und Vote\nlinks ueber die Buttons.\nFairplay. Kein Exploit.");
	}

	protected void DZKOTHG_ConnectToServer()
	{
		if (!GetGame())
			return;

		Print("[DZKOTHG][CLIENT] Direct Connect -> " + DZKOTHG_SERVER_IP + ":" + DZKOTHG_SERVER_PORT.ToString());
		GetGame().ConnectFromServerBrowser(DZKOTHG_SERVER_IP, DZKOTHG_SERVER_PORT, "");
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

		return super.IsFocusable(w);
	}

	override void Play()
	{
		DZKOTHG_ConnectToServer();
	}
};
