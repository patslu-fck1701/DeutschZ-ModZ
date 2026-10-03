modded class MainMenu
{
	static const string DZHUDZ_SERVER_IP = "193.135.10.99";
	static const int DZHUDZ_SERVER_PORT = 20284;
	static const int DZHUDZ_SERVER_QUERY_PORT = 20285;
	static const string DZKOTHG_WEBSITE_URL = "https://project23947.websitepublisher.ai/";
	static const string DZKOTHG_DISCORD_URL = "https://discord.gg/FHzZ7BykFk";
	static const string DZKOTHG_VOTE_URL = "https://de.top-games.net/dayz/vote/deutschz-gunz-heliz-carz-traderz-httpsdiscordggfhzz7bykfk";
	static const string DZKOTHG_RULES_URL = "https://project23947.websitepublisher.ai/wiki.html";
	static const string DZKOTHG_EVENTS_URL = "https://project23947.websitepublisher.ai/kothz.html";
	static const string DZKOTHG_SHOP_URL = "https://project23947.websitepublisher.ai/";
	static const string DZKOTHG_MENU_LAYOUT = "deutschz_hudz/gui/layouts/deutschz_hudz_main_menu.layout";

	protected Widget m_DZKOTHG_VoteButton;
	protected Widget m_DZKOTHG_DiscordButton;
	protected ImageWidget m_DZKOTHG_Background;
	protected TextWidget m_DZKOTHG_Tagline;
	protected MultilineTextWidget m_DZKOTHG_DescriptionBody;
	protected TextWidget m_DZKOTHG_StatusText;
	protected TextWidget m_DZKOTHG_NewsTitle;
	protected TextWidget m_DZKOTHG_ServerStatusTitle;
	protected TextWidget m_DZKOTHG_ServerStatusBody;
	protected MultilineTextWidget m_DZKOTHG_NewsBody;
	protected TextWidget m_DZKOTHG_LiveStrip;
	protected TextWidget m_DZKOTHG_CharacterHint;
	protected TextWidget m_DZKOTHG_SquadTitle;
	protected ref array<TextWidget> m_DZKOTHG_SquadMembers;
	protected TextWidget m_DZKOTHG_MoneyPrivateValue;
	protected TextWidget m_DZKOTHG_MoneyGroupValue;
	protected ref array<TextWidget> m_DZKOTHG_LeaderboardNumbers;
	protected ref array<TextWidget> m_DZKOTHG_EventTitles;
	protected int m_DZKOTHG_DynamicPage;
	protected float m_DZKOTHG_ContentTimer;
	protected float m_DZKOTHG_GroupTimer;
	protected Widget m_DZKOTHG_NavHome;
	protected Widget m_DZKOTHG_NavServer;
	protected Widget m_DZKOTHG_NavRules;
	protected Widget m_DZKOTHG_NavSupport;
	protected Widget m_DZKOTHG_NavEvents;
	protected Widget m_DZKOTHG_NavShop;
	protected Widget m_DZKOTHG_NavSettings;
	protected Widget m_DZKOTHG_NavProfile;
	protected Widget m_DZKOTHG_NavExit;
	protected Widget m_DZKOTHG_MusicPlay;
	protected Widget m_DZKOTHG_MusicNext;
	protected Widget m_DZKOTHG_MusicPrev;
	protected Widget m_DZKOTHG_MusicVolumeDown;
	protected Widget m_DZKOTHG_MusicMute;
	protected Widget m_DZKOTHG_MusicVolumeUp;
	protected Widget m_DZKOTHG_MusicVolumeLevelClip;
	protected TextWidget m_DZKOTHG_TrackTitle;
	protected TextWidget m_DZKOTHG_MusicTime;
	protected Widget m_DZKOTHG_MusicProgressFill;
	protected float m_DZKOTHG_MusicUiTimer;
	protected Widget m_DZKOTHG_PlaylistDeutschZ;
	protected Widget m_DZKOTHG_PlaylistEnglish;
	protected Widget m_DZKOTHG_PlaylistHalftanZ;
	protected ref DZPVPUI_MusicPlayer m_DZKOTHG_PlaylistController;
	protected bool m_DZKOTHG_MusicPaused = true;
	protected bool m_DZKOTHG_MusicMuted;
	protected float m_DZKOTHG_MusicVolumeBeforeMute = 0.66;
	protected bool m_DZKOTHG_ServerStatusKnown;
	protected bool m_DZKOTHG_ServerOnline;
	protected bool m_DZKOTHG_ServerStatusPending;
	protected bool m_DZKOTHG_SystemDialogBlocked;
	protected int m_DZKOTHG_ServerPlayers;
	protected int m_DZKOTHG_ServerMaxPlayers;
	protected static const float DZKOTHG_MUSIC_VOLUME_MAX = 0.66;
	protected static const float DZKOTHG_MUSIC_VOLUME_STEP = 0.10;

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

		DZPVPUI_ClientDisplayCacheStore.Load();
		DZKOTHG_SelectMenuBackground();
		DZKOTHG_BootVanillaMenuState();
		DZKOTHG_StopNativeMenuMusic();
		DZKOTHG_InitPlaylistController();
		DZKOTHG_RequestServerStatus();
		DZKOTHG_UpdateCustomText();
		DZKOTHG_UpdateExpansionGroup();
		DZKOTHG_UpdateLeaderboardSummary();
		DZKOTHG_ApplyInitialButtonStates();
		DZKOTHG_SetNavButtonState(m_DZKOTHG_NavHome, "active");
		DZKOTHG_UpdateVolumeDisplay();

		Print("[DZKOTHG][CLIENT] MainMenu Init OK");
		return layoutRoot;
	}

	void ~MainMenu()
	{
		OnlineServices.m_ServersAsyncInvoker.Remove(DZKOTHG_OnServerStatusLoaded);
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
		m_DZKOTHG_Background     = ImageWidget.Cast(layoutRoot.FindAnyWidget("dz_background"));
		m_DZKOTHG_Tagline        = TextWidget.Cast(layoutRoot.FindAnyWidget("dz_tagline_text"));
		m_DZKOTHG_DescriptionBody = MultilineTextWidget.Cast(layoutRoot.FindAnyWidget("dz_description_body"));
		m_DZKOTHG_StatusText     = TextWidget.Cast(layoutRoot.FindAnyWidget("dz_status_text"));
		m_DZKOTHG_NewsTitle      = TextWidget.Cast(layoutRoot.FindAnyWidget("dz_news_title"));
		m_DZKOTHG_ServerStatusTitle = TextWidget.Cast(layoutRoot.FindAnyWidget("dz_server_title"));
		m_DZKOTHG_ServerStatusBody = TextWidget.Cast(layoutRoot.FindAnyWidget("dz_server_status_body"));
		m_DZKOTHG_NewsBody       = MultilineTextWidget.Cast(layoutRoot.FindAnyWidget("dz_news_body"));
		m_DZKOTHG_LiveStrip      = TextWidget.Cast(layoutRoot.FindAnyWidget("dz_live_strip_text"));
		m_DZKOTHG_CharacterHint  = TextWidget.Cast(layoutRoot.FindAnyWidget("dz_character_hint"));
		m_DZKOTHG_SquadTitle = TextWidget.Cast(layoutRoot.FindAnyWidget("dz_squad_title"));
		m_DZKOTHG_SquadMembers = new array<TextWidget>();
		for (int squadIndex = 0; squadIndex < 5; squadIndex++)
			m_DZKOTHG_SquadMembers.Insert(TextWidget.Cast(layoutRoot.FindAnyWidget("dz_squad_member_" + squadIndex.ToString())));
		m_DZKOTHG_MoneyPrivateValue = TextWidget.Cast(layoutRoot.FindAnyWidget("dz_money_private_value"));
		m_DZKOTHG_MoneyGroupValue = TextWidget.Cast(layoutRoot.FindAnyWidget("dz_money_group_value"));
		m_DZKOTHG_LeaderboardNumbers = new array<TextWidget>();
		for (int leaderboardIndex = 0; leaderboardIndex < 4; leaderboardIndex++)
			m_DZKOTHG_LeaderboardNumbers.Insert(TextWidget.Cast(layoutRoot.FindAnyWidget("dz_leaderboard_number_" + leaderboardIndex.ToString())));
		m_DZKOTHG_EventTitles = new array<TextWidget>();
		m_DZKOTHG_EventTitles.Insert(TextWidget.Cast(layoutRoot.FindAnyWidget("dz_event_koth_title")));
		m_DZKOTHG_EventTitles.Insert(TextWidget.Cast(layoutRoot.FindAnyWidget("dz_event_convoy_title")));
		m_DZKOTHG_EventTitles.Insert(TextWidget.Cast(layoutRoot.FindAnyWidget("dz_event_groundzero_title")));
		m_DZKOTHG_EventTitles.Insert(TextWidget.Cast(layoutRoot.FindAnyWidget("dz_event_operation_title")));
		m_DZKOTHG_NavHome        = layoutRoot.FindAnyWidget("dz_nav_home");
		m_DZKOTHG_NavServer      = layoutRoot.FindAnyWidget("dz_nav_server");
		m_DZKOTHG_NavRules       = layoutRoot.FindAnyWidget("dz_nav_rules");
		m_DZKOTHG_NavSupport     = layoutRoot.FindAnyWidget("dz_nav_support");
		m_DZKOTHG_NavEvents      = layoutRoot.FindAnyWidget("dz_nav_events");
		m_DZKOTHG_NavShop        = layoutRoot.FindAnyWidget("dz_nav_shop");
		m_DZKOTHG_NavSettings    = layoutRoot.FindAnyWidget("dz_nav_settings");
		m_DZKOTHG_NavProfile     = layoutRoot.FindAnyWidget("dz_nav_profile");
		m_DZKOTHG_NavExit        = layoutRoot.FindAnyWidget("dz_nav_exit");
		m_DZKOTHG_MusicPlay      = layoutRoot.FindAnyWidget("dz_music_play");
		m_DZKOTHG_MusicNext      = layoutRoot.FindAnyWidget("dz_music_next");
		m_DZKOTHG_MusicPrev      = layoutRoot.FindAnyWidget("dz_music_prev");
		m_DZKOTHG_MusicVolumeDown = layoutRoot.FindAnyWidget("dz_music_volume_down");
		m_DZKOTHG_MusicMute       = layoutRoot.FindAnyWidget("dz_music_mute");
		m_DZKOTHG_MusicVolumeUp   = layoutRoot.FindAnyWidget("dz_music_volume_up");
		m_DZKOTHG_MusicVolumeLevelClip = layoutRoot.FindAnyWidget("dz_volume_level_clip");
		m_DZKOTHG_TrackTitle = TextWidget.Cast(layoutRoot.FindAnyWidget("dz_track_title"));
		m_DZKOTHG_MusicTime = TextWidget.Cast(layoutRoot.FindAnyWidget("dz_music_time"));
		m_DZKOTHG_MusicProgressFill = layoutRoot.FindAnyWidget("dz_music_progress_fill");
		m_DZKOTHG_PlaylistDeutschZ = layoutRoot.FindAnyWidget("dz_playlist_deutschz");
		m_DZKOTHG_PlaylistEnglish = layoutRoot.FindAnyWidget("dz_playlist_english");
		m_DZKOTHG_PlaylistHalftanZ = layoutRoot.FindAnyWidget("dz_playlist_halftanz");

		if (m_DZKOTHG_Tagline)
			m_DZKOTHG_Tagline.SetColor(DZKOTHG_UITheme.PrimaryText());
		if (m_DZKOTHG_DescriptionBody)
			m_DZKOTHG_DescriptionBody.SetColor(DZKOTHG_UITheme.PrimaryText());
		if (m_DZKOTHG_StatusText)
			m_DZKOTHG_StatusText.SetColor(DZKOTHG_UITheme.BrandGreen());
		if (m_DZKOTHG_NewsTitle)
			m_DZKOTHG_NewsTitle.SetColor(DZKOTHG_UITheme.BrandGreen());
		if (m_DZKOTHG_ServerStatusBody)
			m_DZKOTHG_ServerStatusBody.SetColor(DZKOTHG_UITheme.PrimaryText());
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

	protected void DZKOTHG_SelectMenuBackground()
	{
		if (!m_DZKOTHG_Background)
			return;
		// Die native 3D-Menüszene samt Charakter bleibt vollständig frei sichtbar.
		m_DZKOTHG_Background.Show(false);
	}

	protected bool DZKOTHG_ValidateRequiredWidgets()
	{
		if (!m_Play || !m_ChooseServer || !m_CustomizeCharacter || !m_SettingsButton || !m_Exit || !m_CharacterRotationFrame)
			return false;

		if (!layoutRoot.FindAnyWidget("character_stats_root"))
			return false;

		if (!m_DZKOTHG_MusicPlay || !m_DZKOTHG_MusicPrev || !m_DZKOTHG_MusicNext || !m_DZKOTHG_MusicVolumeDown || !m_DZKOTHG_MusicMute || !m_DZKOTHG_MusicVolumeUp)
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
			m_ModdedWarning.Show(true);

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
		MultilineTextWidget summaryIdentity = MultilineTextWidget.Cast(layoutRoot.FindAnyWidget("dz_summary_identity"));
		MultilineTextWidget summaryFaction = MultilineTextWidget.Cast(layoutRoot.FindAnyWidget("dz_summary_faction"));
		MultilineTextWidget summaryBalance = MultilineTextWidget.Cast(layoutRoot.FindAnyWidget("dz_summary_balance"));
		MultilineTextWidget summaryKills = MultilineTextWidget.Cast(layoutRoot.FindAnyWidget("dz_summary_kills"));
		MultilineTextWidget summaryPlaytime = MultilineTextWidget.Cast(layoutRoot.FindAnyWidget("dz_summary_playtime"));
		if (summaryIdentity)
			summaryIdentity.SetText("DeutschZer\nUeberlebender");
		if (summaryFaction)
			summaryFaction.SetText("Fraktion\nNeutral");
		if (summaryBalance)
			summaryBalance.SetText("Kontostand\nEUR --");
		if (summaryKills)
			summaryKills.SetText("Kills\n--");
		if (summaryPlaytime)
			summaryPlaytime.SetText("Spielzeit\n--");

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
			m_DZKOTHG_StatusText.SetText("SERVER  |  " + DZHUDZ_SERVER_IP + ":" + DZHUDZ_SERVER_PORT.ToString());
			m_DZKOTHG_StatusText.SetTextExactSize(18);
		}

		if (m_DZKOTHG_NewsTitle)
			m_DZKOTHG_NewsTitle.SetText("STATISTIKEN");
		if (m_DZKOTHG_NewsBody)
			m_DZKOTHG_NewsBody.SetText("Am Leben                         2h 34m 57s\nSpieler Kills                                  0\nInfizierte Kills                               0\nReisedistanz                             955 m\nWeitester Treffer                           0 m");

		if (m_DZKOTHG_ServerStatusBody)
			DZKOTHG_ApplyServerStatus();

		DZKOTHG_UpdateDynamicEvents();
		DZKOTHG_UpdateLiveStrip();
		DZKOTHG_UpdateCharacterHint();
		m_DZKOTHG_DynamicPage++;
	}

	protected void DZKOTHG_RequestServerStatus()
	{
		m_DZKOTHG_ServerStatusKnown = false;
		m_DZKOTHG_ServerOnline = false;
		m_DZKOTHG_ServerStatusPending = true;
		OnlineServices.m_ServersAsyncInvoker.Remove(DZKOTHG_OnServerStatusLoaded);
		OnlineServices.m_ServersAsyncInvoker.Insert(DZKOTHG_OnServerStatusLoaded, EScriptInvokerInsertFlags.NONE);
		OnlineServices.ClearCurrentServerInfo();
		DZKOTHG_SendServerStatusQuery();
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(DZKOTHG_SendServerStatusQuery, 2500, false);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(DZKOTHG_SendServerStatusQuery, 5000, false);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(DZKOTHG_SendServerStatusQuery, 7500, false);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(DZKOTHG_OnServerStatusTimeout, 12000, false);
		DZKOTHG_ApplyServerStatus();
	}

	protected void DZKOTHG_SendServerStatusQuery()
	{
		if (!m_DZKOTHG_ServerStatusPending)
			return;
		GetServersInput statusQuery = new GetServersInput;
		statusQuery.m_Page = 0;
		statusQuery.m_RowsPerPage = 20;
		statusQuery.m_Platform = 1;
		statusQuery.SetFavorited(true);
		statusQuery.AddFavourite(DZHUDZ_SERVER_IP, DZHUDZ_SERVER_QUERY_PORT);
		OnlineServices.LoadServers(statusQuery);
	}

	protected void DZKOTHG_OnServerStatusLoaded(GetServersResult resultList, EBiosError error, string response)
	{
		if (!m_DZKOTHG_ServerStatusPending)
			return;
		if (error == EBiosError.OK && resultList)
		{
			foreach (GetServersResultRow result : resultList.m_Results)
			{
				if (result && result.GetIP() == DZHUDZ_SERVER_IP && (result.m_HostPort == DZHUDZ_SERVER_PORT || result.m_SteamQueryPort == DZHUDZ_SERVER_QUERY_PORT))
				{
					m_DZKOTHG_ServerStatusKnown = true;
					m_DZKOTHG_ServerOnline = true;
					m_DZKOTHG_ServerPlayers = result.m_CurrentNumberPlayers;
					m_DZKOTHG_ServerMaxPlayers = result.m_MaxPlayers;
					m_DZKOTHG_ServerStatusPending = false;
					OnlineServices.m_ServersAsyncInvoker.Remove(DZKOTHG_OnServerStatusLoaded);
					DZKOTHG_ApplyServerStatus();
					return;
				}
			}
		}
	}

	protected void DZKOTHG_OnServerStatusTimeout()
	{
		if (!m_DZKOTHG_ServerStatusPending)
			return;
		m_DZKOTHG_ServerStatusPending = false;
		m_DZKOTHG_ServerStatusKnown = true;
		m_DZKOTHG_ServerOnline = false;
		m_DZKOTHG_ServerPlayers = 0;
		m_DZKOTHG_ServerMaxPlayers = 0;
		OnlineServices.m_ServersAsyncInvoker.Remove(DZKOTHG_OnServerStatusLoaded);
		DZKOTHG_ApplyServerStatus();
	}

	protected void DZKOTHG_ApplyServerStatus()
	{
		string stateText = "STATUS NICHT VERFUEGBAR";
		int stateColor = ARGB(255, 116, 124, 119);
		if (m_DZKOTHG_ServerStatusKnown)
		{
			stateText = "OFFLINE";
			stateColor = ARGB(255, 220, 45, 45);
		}
		if (m_DZKOTHG_ServerOnline)
		{
			stateText = "ONLINE  |  " + m_DZKOTHG_ServerPlayers.ToString() + " / " + m_DZKOTHG_ServerMaxPlayers.ToString() + " SPIELER";
			stateColor = DZKOTHG_UITheme.BrandGreen();
		}

		if (m_DZKOTHG_StatusText)
		{
			m_DZKOTHG_StatusText.SetText(stateText + "  |  " + DZHUDZ_SERVER_IP + ":" + DZHUDZ_SERVER_PORT.ToString());
			m_DZKOTHG_StatusText.SetColor(stateColor);
		}
		if (m_DZKOTHG_ServerStatusBody)
		{
			m_DZKOTHG_ServerStatusBody.SetText("LIVE  |  COMMUNITY UPDATE\n\n" + stateText + "\n" + DZHUDZ_SERVER_IP + ":" + DZHUDZ_SERVER_PORT.ToString() + "\n\nEVENTSYSTEME\nKotHZ, ConvoyZ und weitere DeutschZ-Systeme");
			m_DZKOTHG_ServerStatusBody.SetColor(stateColor);
		}
		if (m_DZKOTHG_ServerStatusTitle)
			m_DZKOTHG_ServerStatusTitle.SetColor(stateColor);
	}

	protected void DZKOTHG_UpdateDynamicEvents()
	{
		if (!m_DZKOTHG_EventTitles || m_DZKOTHG_EventTitles.Count() < 4)
			return;

		ref array<string> names = {"CONVOYZ", "KOTHZ", "BATTLEGROUNDZ", "CRIMINALZ"};

		for (int i = 0; i < 4; i++)
		{
			if (m_DZKOTHG_EventTitles.Get(i))
				m_DZKOTHG_EventTitles.Get(i).SetText(names.Get(i));
		}
	}

	protected void DZKOTHG_UpdateCharacterHint()
	{
		if (!m_DZKOTHG_CharacterHint)
			return;

		m_DZKOTHG_CharacterHint.SetText("");
	}

	protected void DZKOTHG_UpdateLiveStrip()
	{
		if (!m_DZKOTHG_LiveStrip)
			return;

		int hour;
		int minute;
		int second;
		GetHourMinuteSecond(hour, minute, second);
		string minuteText = minute.ToString();
		if (minute < 10)
			minuteText = "0" + minuteText;

		m_DZKOTHG_LiveStrip.SetText("EVENTSTATUS WIRD GELADEN  |  SERVER LIVE  |  DEUTSCHZ COMMUNITY  |  SERVERZEIT " + hour.ToString() + ":" + minuteText);
	}

	override void Update(float timeslice)
	{
		super.Update(timeslice);
		Widget systemDialog = GetGame().GetWorkspace().FindAnyWidget("DialogBox");
		bool systemDialogVisible = systemDialog && systemDialog.IsVisible();
		if (systemDialogVisible != m_DZKOTHG_SystemDialogBlocked)
		{
			m_DZKOTHG_SystemDialogBlocked = systemDialogVisible;
			DZPVPUI_SystemDialogInput.BlockMainMenuButtons(systemDialogVisible);
		}
		if (m_ModdedWarning)
			m_ModdedWarning.Show(true);

		m_DZKOTHG_ContentTimer += timeslice;
		m_DZKOTHG_GroupTimer += timeslice;
		m_DZKOTHG_MusicUiTimer += timeslice;
		if (m_DZKOTHG_MusicUiTimer >= 0.25)
		{
			m_DZKOTHG_MusicUiTimer = 0.0;
			DZKOTHG_UpdateMusicDisplay();
		}
		if (m_DZKOTHG_ContentTimer >= 8.0)
		{
			m_DZKOTHG_ContentTimer = 0.0;
			DZKOTHG_UpdateCustomText();
		}
		if (m_DZKOTHG_GroupTimer >= 2.0)
		{
			m_DZKOTHG_GroupTimer = 0.0;
			DZKOTHG_UpdateExpansionGroup();
			DZKOTHG_UpdateLeaderboardSummary();
		}
	}

	protected void DZKOTHG_UpdateLeaderboardSummary()
	{
		foreach (TextWidget numberWidget : m_DZKOTHG_LeaderboardNumbers)
		{
			if (numberWidget)
				numberWidget.SetText("--");
		}
		if (DZPVPUI_State.LeaderboardSummary == "")
			return;
		array<string> values = new array<string>;
		DZPVPUI_State.LeaderboardSummary.Split("|", values);
		if (values.Count() != 4)
			return;
		for (int valueIndex = 0; valueIndex < 4; valueIndex++)
		{
			TextWidget valueWidget = m_DZKOTHG_LeaderboardNumbers.Get(valueIndex);
			if (valueWidget)
				valueWidget.SetText(values.Get(valueIndex));
		}
	}

	protected void DZKOTHG_UpdateExpansionGroup()
	{
		DZPVPUI_ClientDisplayCacheStore.Load();
		DZPVPUI_ClientDisplayCache displayCache = DZPVPUI_ClientDisplayCacheStore.Cache;
		string initialTitle = "KEINE GRUPPE  0 / 0";
		if (displayCache && displayCache.GroupTitle != "")
			initialTitle = displayCache.GroupTitle;
		if (m_DZKOTHG_SquadTitle)
			m_DZKOTHG_SquadTitle.SetText(initialTitle);
		if (m_DZKOTHG_MoneyPrivateValue)
			m_DZKOTHG_MoneyPrivateValue.SetText(DZKOTHG_FormatMoney(DZPVPUI_State.AccountBalance));
		if (m_DZKOTHG_MoneyGroupValue)
		{
			if (displayCache)
				m_DZKOTHG_MoneyGroupValue.SetText(DZKOTHG_FormatMoney(displayCache.GroupBalance));
			else
				m_DZKOTHG_MoneyGroupValue.SetText("--");
		}
		foreach (TextWidget memberWidget : m_DZKOTHG_SquadMembers)
		{
			if (memberWidget)
			{
				memberWidget.SetText("");
				memberWidget.SetColor(ARGB(255, 116, 124, 119));
			}
		}
		if (displayCache)
		{
			int cachedMemberCount = Math.Min(displayCache.GroupMemberNames.Count(), m_DZKOTHG_SquadMembers.Count());
			for (int cachedIndex = 0; cachedIndex < cachedMemberCount; cachedIndex++)
			{
				TextWidget cachedWidget = m_DZKOTHG_SquadMembers.Get(cachedIndex);
				if (!cachedWidget)
					continue;
				cachedWidget.SetText(displayCache.GroupMemberNames.Get(cachedIndex));
				if (cachedIndex < displayCache.GroupMemberOnline.Count() && displayCache.GroupMemberOnline.Get(cachedIndex) == 1)
					cachedWidget.SetColor(DZKOTHG_UITheme.BrandGreen());
			}
		}

#ifdef EXPANSIONMODGROUPS
		ExpansionPartyModule partyModule;
		if (!CF_Modules<ExpansionPartyModule>.Get(partyModule) || !partyModule.GetParty())
			return;

		ExpansionPartyData party = partyModule.GetParty();
		array<ref ExpansionPartyPlayerData> members = party.GetPlayers();
		if (!members)
			return;

		int onlineCount;
		foreach (ExpansionPartyPlayerData member : members)
		{
			if (member && PlayerBase.Expansion_IsOnline(member.GetID()))
				onlineCount++;
		}

		string partyName = party.GetPartyName();
		if (partyName == "")
			partyName = "GRUPPE";
		if (m_DZKOTHG_SquadTitle)
			m_DZKOTHG_SquadTitle.SetText(partyName + "  " + onlineCount.ToString() + " / " + members.Count().ToString());
		if (m_DZKOTHG_MoneyGroupValue)
			m_DZKOTHG_MoneyGroupValue.SetText(DZKOTHG_FormatMoney(party.GetMoneyDeposited()));
		array<string> cachedNames = new array<string>;
		array<int> cachedOnline = new array<int>;

		int visibleMembers = Math.Min(members.Count(), m_DZKOTHG_SquadMembers.Count());
		for (int memberIndex = 0; memberIndex < visibleMembers; memberIndex++)
		{
			ExpansionPartyPlayerData memberData = members.Get(memberIndex);
			TextWidget nameWidget = m_DZKOTHG_SquadMembers.Get(memberIndex);
			if (!memberData || !nameWidget)
				continue;
			string memberName = memberData.GetName();
			if (memberName.Length() > 8)
				memberName = memberName.Substring(0, 8);
			nameWidget.SetText(memberName);
			bool memberOnline = PlayerBase.Expansion_IsOnline(memberData.GetID());
			cachedNames.Insert(memberName);
			if (memberOnline)
				cachedOnline.Insert(1);
			else
				cachedOnline.Insert(0);
			if (memberOnline)
				nameWidget.SetColor(DZKOTHG_UITheme.BrandGreen());
			else
				nameWidget.SetColor(ARGB(255, 116, 124, 119));
		}
		DZPVPUI_ClientDisplayCacheStore.SaveGroupSnapshot(partyName + "  " + onlineCount.ToString() + " / " + members.Count().ToString(), party.GetMoneyDeposited(), cachedNames, cachedOnline);
#endif
	}

	protected string DZKOTHG_FormatMoney(int amount)
	{
		if (amount < 0)
			return "--";
		string raw = amount.ToString();
		string formatted;
		int digits = raw.Length();
		for (int digitIndex = 0; digitIndex < digits; digitIndex++)
		{
			if (digitIndex > 0 && (digits - digitIndex) % 3 == 0)
				formatted += ".";
			formatted += raw.Substring(digitIndex, 1);
		}
		return formatted + " EUR";
	}

	protected string DZKOTHG_FormatMusicTime(float secondsValue)
	{
		int secondsTotal = Math.Max(0, Math.Round(secondsValue));
		int minutes = secondsTotal / 60;
		int seconds = secondsTotal % 60;
		string secondsText = seconds.ToString();
		if (seconds < 10)
			secondsText = "0" + secondsText;
		return minutes.ToString() + ":" + secondsText;
	}

	protected void DZKOTHG_UpdateMusicDisplay()
	{
		if (!m_DZKOTHG_PlaylistController)
			return;

		float length = m_DZKOTHG_PlaylistController.GetTrackLength();
		float progress = m_DZKOTHG_PlaylistController.GetTrackProgress();
		if (m_DZKOTHG_TrackTitle)
			m_DZKOTHG_TrackTitle.SetText(m_DZKOTHG_PlaylistController.GetTrackName());
		if (m_DZKOTHG_MusicTime)
			m_DZKOTHG_MusicTime.SetText(DZKOTHG_FormatMusicTime(length * progress) + " / " + DZKOTHG_FormatMusicTime(length));
		if (m_DZKOTHG_MusicProgressFill)
			m_DZKOTHG_MusicProgressFill.SetSize(245.0 * progress, 6.0);
	}

	protected void DZKOTHG_ConnectToServer()
	{
		if (!GetGame())
			return;

		Print("[DZKOTHG][CLIENT] Direct Connect -> " + DZHUDZ_SERVER_IP + ":" + DZHUDZ_SERVER_PORT.ToString());
		GetGame().ConnectFromServerBrowserEx(DZHUDZ_SERVER_IP, DZHUDZ_SERVER_PORT, DZHUDZ_SERVER_QUERY_PORT, "");
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
		image.SetColor(ARGB(255, 255, 255, 255));
		image.SetAlpha(1.0);
		image.Show(true);
	}

	protected void DZKOTHG_SetNavIcon(Widget button, string name, bool hovered)
	{
		if (!button)
			return;

		string widgetName = "dz_nav_" + name + "_icon";
		if (name == "power")
			widgetName = "dz_nav_exit_icon";
		ImageWidget icon = ImageWidget.Cast(layoutRoot.FindAnyWidget(widgetName));
		if (!icon)
			return;

		icon.LoadImageFile(0, DZKOTHG_GetPatrickIconPath(name));
		icon.SetImage(0);
		if (hovered)
			icon.SetAlpha(1.0);
		else
			icon.SetAlpha(0.82);
	}

	protected string DZKOTHG_GetPatrickIconPath(string name)
	{
		string iconName = name;
		if (name == "events") iconName = "calendar";
		else if (name == "rules") iconName = "info";
		else if (name == "shop") iconName = "website";
		else if (name == "profile") iconName = "character";

		return "deutschz_hudz/gui/menu_green1/asset_pack/icons/dz_ui_icon_" + iconName + "_green.paa";
	}

	protected void DZKOTHG_SetNavButtonState(Widget button, string state)
	{
		string name = "";
		string layoutName = "";
		if (button == m_DZKOTHG_NavHome)
			name = "home";
		else if (button == m_DZKOTHG_NavServer)
			name = "server";
		else if (button == m_DZKOTHG_NavRules)
			name = "rules";
		else if (button == m_DZKOTHG_NavSupport)
			name = "support";
		else if (button == m_DZKOTHG_NavEvents)
			name = "events";
		else if (button == m_DZKOTHG_NavShop)
			name = "shop";
		else if (button == m_DZKOTHG_NavSettings)
			name = "settings";
		else if (button == m_DZKOTHG_NavProfile)
			name = "profile";
		else if (button == m_DZKOTHG_NavExit)
			name = "power";

		if (name == "")
			return;

		layoutName = name;
		if (button == m_DZKOTHG_NavExit)
			layoutName = "exit";

		string widgetName = "dz_nav_" + layoutName + "_icon";
		ImageWidget icon = ImageWidget.Cast(layoutRoot.FindAnyWidget(widgetName));
		if (!icon)
			return;

		icon.LoadImageFile(0, DZKOTHG_GetPatrickIconPath(name));
		icon.SetImage(0);
		icon.Show(true);
		icon.SetColor(ARGB(255, 255, 255, 255));
		if (state == "disabled")
			icon.SetAlpha(0.35);
		else if (state == "pressed")
			icon.SetAlpha(0.68);
		else if (state == "hover" || state == "active")
			icon.SetAlpha(1.0);
		else
			icon.SetAlpha(0.82);

		TextWidget navLabel = TextWidget.Cast(layoutRoot.FindAnyWidget("dz_nav_" + layoutName + "_text"));
		if (navLabel)
		{
			if (button == m_DZKOTHG_NavExit)
			{
				if (state == "hover" || state == "focus" || state == "selected" || state == "active")
					navLabel.SetColor(ARGB(255, 255, 78, 66));
				else
					navLabel.SetColor(ARGB(255, 220, 45, 45));
			}
			else if (state == "hover" || state == "focus" || state == "selected" || state == "active")
				navLabel.SetColor(ARGB(255, 104, 220, 0));
			else if (state == "disabled")
				navLabel.SetColor(ARGB(255, 255, 255, 255));
			else
				navLabel.SetColor(DZKOTHG_UITheme.BrandGreen());
		}

		if (button == m_DZKOTHG_NavExit)
			icon.SetColor(ARGB(255, 220, 45, 45));
	}

	protected void DZKOTHG_SetMainButtonState(Widget button, string state)
	{
		string focusState = "cancel";
		string colorName = "green";
		bool permanentRed = (button == m_Play || button == m_Exit);
		if (permanentRed)
			colorName = "red";

		if (state == "hover" || state == "focus" || state == "selected" || state == "active")
		{
			focusState = "success";
		}
		else if (state == "pressed")
			focusState = "warning";
		else if (state == "disabled" || state == "error")
		{
			focusState = "error";
			colorName = "red";
		}

		string labelName = "";
		if (button == m_Play)
			labelName = "play_label";
		else if (button == m_ChooseServer)
			labelName = "choose_server_label";
		else if (button == m_CustomizeCharacter)
			labelName = "customize_label";
		else if (button == m_DZKOTHG_VoteButton)
			labelName = "vote_label";
		else if (button == m_DZKOTHG_DiscordButton)
			labelName = "discord_label";
		else if (button == m_MessageButton)
			labelName = "website_label";
		else if (button == m_Exit)
			labelName = "exit_label";

		TextWidget label;
		if (labelName != "")
			label = TextWidget.Cast(layoutRoot.FindAnyWidget(labelName));
		if (label)
		{
			if (permanentRed)
			{
				if (state == "hover" || state == "focus" || state == "selected" || state == "active")
					label.SetColor(ARGB(255, 255, 78, 66));
				else
					label.SetColor(ARGB(255, 255, 183, 170));
			}
			else if (state == "hover" || state == "focus" || state == "selected" || state == "active")
				label.SetColor(ARGB(255, 104, 220, 0));
			else if (state == "disabled" || state == "error")
				label.SetColor(ARGB(255, 255, 255, 255));
			else
				label.SetColor(DZKOTHG_UITheme.BrandGreen());
		}
	}

	protected string DZKOTHG_GetCompactFrameState(string state)
	{
		if (state == "hover" || state == "focus" || state == "selected" || state == "active")
			return "success";
		if (state == "pressed")
			return "warning";
		if (state == "disabled")
			return "error";
		return "cancel";
	}

	protected string DZKOTHG_GetTileState(string state)
	{
		if (state == "hover" || state == "focus" || state == "selected")
			return "hover";
		if (state == "pressed")
			return "pressed";
		if (state == "active")
			return "active";
		if (state == "disabled")
			return "disabled";
		return "normal";
	}

	protected void DZKOTHG_SetMusicControlState(Widget button, string state)
	{
		if (button == m_DZKOTHG_MusicPlay)
		{
			if (m_DZKOTHG_MusicPaused)
				DZKOTHG_SetMenuImage("dz_music_play_icon", "deutschz_hudz/gui/icons/music/icon_music_play_green_256px_success.paa");
			else
				DZKOTHG_SetMenuImage("dz_music_play_icon", "deutschz_hudz/gui/icons/music/dz_ui_icon_pause_red.paa");
		}
		else if (button == m_DZKOTHG_MusicNext)
		{
			DZKOTHG_SetMenuImage("dz_music_next_icon", "deutschz_hudz/gui/icons/music/icon_music_next_green_256px_success.paa");
		}
		else if (button == m_DZKOTHG_MusicPrev)
		{
			DZKOTHG_SetMenuImage("dz_music_prev_icon", "deutschz_hudz/gui/icons/music/icon_music_prev_green_256px_success.paa");
		}
		else if (button == m_DZKOTHG_MusicVolumeDown)
			return;
		else if (button == m_DZKOTHG_MusicVolumeUp)
			return;
		else if (button == m_DZKOTHG_MusicMute)
		{
			if (m_DZKOTHG_MusicMuted)
				DZKOTHG_SetMenuImage("dz_music_mute_icon", "deutschz_hudz/gui/icons/music/icon_volume_green_192px_normal.paa");
			else
				DZKOTHG_SetMenuImage("dz_music_mute_icon", "deutschz_hudz/gui/icons/music/icon_volume_green_192px_normal.paa");
		}
	}

	protected void DZKOTHG_UpdateVolumeDisplay()
	{
		if (!m_DZKOTHG_MusicVolumeLevelClip || !m_DZKOTHG_PlaylistController)
			return;

		float level = Math.Clamp(m_DZKOTHG_PlaylistController.GetVolume() / DZKOTHG_MUSIC_VOLUME_MAX, 0.0, 1.0);
		m_DZKOTHG_MusicVolumeLevelClip.SetSize(96.0 * level, 56.0);
	}

	protected void DZKOTHG_ApplyInitialButtonStates()
	{
		DZKOTHG_SetMenuImage("play_icon", "deutschz_hudz/gui/menu_green1/asset_pack/icons/dz_ui_icon_play_green.paa");
		DZKOTHG_SetMenuImage("choose_server_icon", "deutschz_hudz/gui/menu_green1/asset_pack/icons/dz_ui_icon_search_green.paa");
		DZKOTHG_SetMenuImage("customize_icon", "deutschz_hudz/gui/menu_green1/asset_pack/icons/dz_ui_icon_character_green.paa");
		DZKOTHG_SetMenuImage("vote_icon", "deutschz_hudz/gui/menu_green1/asset_pack/icons/dz_ui_icon_vote_green.paa");
		DZKOTHG_SetMenuImage("discord_icon", "deutschz_hudz/gui/menu_green1/asset_pack/icons/dz_ui_icon_discord_green.paa");
		DZKOTHG_SetMenuImage("website_icon", "deutschz_hudz/gui/menu_green1/asset_pack/icons/dz_ui_icon_website_green.paa");
		DZKOTHG_SetMenuImage("exit_icon", "deutschz_hudz/gui/menu_green1/asset_pack/icons/dz_ui_icon_power_green.paa");
		DZKOTHG_SetMainButtonState(m_Play, "normal");
		DZKOTHG_SetMainButtonState(m_ChooseServer, "normal");
		DZKOTHG_SetMainButtonState(m_CustomizeCharacter, "normal");
		DZKOTHG_SetMainButtonState(m_DZKOTHG_VoteButton, "normal");
		DZKOTHG_SetMainButtonState(m_DZKOTHG_DiscordButton, "normal");
		DZKOTHG_SetMainButtonState(m_MessageButton, "normal");
		DZKOTHG_SetMainButtonState(m_Exit, "normal");
		DZKOTHG_SetMusicControlState(m_DZKOTHG_MusicPlay, "normal");
		DZKOTHG_SetMusicControlState(m_DZKOTHG_MusicPrev, "normal");
		DZKOTHG_SetMusicControlState(m_DZKOTHG_MusicNext, "normal");
		DZKOTHG_SetMusicControlState(m_DZKOTHG_MusicVolumeDown, "normal");
		DZKOTHG_SetMusicControlState(m_DZKOTHG_MusicMute, "normal");
		DZKOTHG_SetMusicControlState(m_DZKOTHG_MusicVolumeUp, "normal");
		DZKOTHG_SetNavButtonState(m_DZKOTHG_NavServer, "normal");
		DZKOTHG_SetNavButtonState(m_DZKOTHG_NavRules, "normal");
		DZKOTHG_SetNavButtonState(m_DZKOTHG_NavSupport, "normal");
		DZKOTHG_SetNavButtonState(m_DZKOTHG_NavEvents, "normal");
		DZKOTHG_SetNavButtonState(m_DZKOTHG_NavShop, "normal");
		DZKOTHG_SetNavButtonState(m_DZKOTHG_NavSettings, "normal");
		DZKOTHG_SetNavButtonState(m_DZKOTHG_NavProfile, "normal");
		DZKOTHG_SetNavButtonState(m_DZKOTHG_NavExit, "normal");
	}

	protected void DZKOTHG_RequestMenuMusic(bool fadeCurrent)
	{
		if (!GetGame() || !GetGame().GetMission())
		{
			Print("[DZKOTHG][MUSIC][ERROR] Mission unavailable");
			return;
		}

		DynamicMusicPlayer player = GetGame().GetMission().GetDynamicMusicPlayer();
		if (!player)
		{
			Print("[DZKOTHG][MUSIC][ERROR] DynamicMusicPlayer unavailable");
			return;
		}

		DynamicMusicPlayerCategoryPlaybackData playback = new DynamicMusicPlayerCategoryPlaybackData();
		playback.m_Category = EDynamicMusicPlayerCategory.MENU;
		playback.m_FadeOut = fadeCurrent;
		player.SetCategory(playback);
		Print("[DZKOTHG][MUSIC] MENU requested fadeCurrent=" + fadeCurrent);
	}

	protected void DZKOTHG_InitPlaylistController()
	{
		m_DZKOTHG_PlaylistController = DZPVPUI_MenuMusicSession.GetPlayer();
		if (!m_DZKOTHG_PlaylistController.WantsPlayback())
			m_DZKOTHG_PlaylistController.Play();
		else
			m_DZKOTHG_PlaylistController.RecoverAfterMenu();
		m_DZKOTHG_MusicPaused = !m_DZKOTHG_PlaylistController.WantsPlayback();
		DZKOTHG_UpdatePlaylistTabs();
		Print("[DZKOTHG][MUSIC] DeutschZ playlist controller ready; paused=" + m_DZKOTHG_MusicPaused);
	}

	protected void DZKOTHG_UpdatePlaylistTabs()
	{
		int activePlaylist = 0;
		if (m_DZKOTHG_PlaylistController)
			activePlaylist = m_DZKOTHG_PlaylistController.GetPlaylistIndex();

		DZKOTHG_SetPlaylistTab("dz_playlist_deutschz_text", activePlaylist == 0);
		DZKOTHG_SetPlaylistTab("dz_playlist_english_text", activePlaylist == 1);
		DZKOTHG_SetPlaylistTab("dz_playlist_halftanz_text", activePlaylist == 2);
	}

	protected void DZKOTHG_SetPlaylistTab(string widgetName, bool active)
	{
		TextWidget label = TextWidget.Cast(layoutRoot.FindAnyWidget(widgetName));
		if (!label)
			return;
		if (active)
			label.SetColor(ARGB(255, 105, 220, 0));
		else
			label.SetColor(ARGB(255, 74, 81, 77));
	}

	protected void DZKOTHG_StopNativeMenuMusic()
	{
		if (!GetGame() || !GetGame().GetMission())
			return;

		DynamicMusicPlayer nativePlayer = GetGame().GetMission().GetDynamicMusicPlayer();
		if (!nativePlayer)
			return;

		DynamicMusicPlayerCategoryPlaybackData stopNative = new DynamicMusicPlayerCategoryPlaybackData();
		stopNative.m_Category = EDynamicMusicPlayerCategory.NONE;
		stopNative.m_FadeOut = true;
		nativePlayer.SetCategory(stopNative);
	}

	protected void DZKOTHG_StartNextMenuTrack()
	{
		DZKOTHG_RequestMenuMusic(false);
	}

	protected void DZKOTHG_SkipMenuTrack()
	{
		if (!GetGame() || !GetGame().GetMission())
			return;

		DynamicMusicPlayer player = GetGame().GetMission().GetDynamicMusicPlayer();
		if (!player)
			return;

		DynamicMusicPlayerCategoryPlaybackData stopPlayback = new DynamicMusicPlayerCategoryPlaybackData();
		stopPlayback.m_Category = EDynamicMusicPlayerCategory.NONE;
		stopPlayback.m_FadeOut = false;
		player.SetCategory(stopPlayback);

		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(DZKOTHG_StartNextMenuTrack, 100, false);
	}

	protected void DZKOTHG_ToggleMenuMusic()
	{
		if (!GetGame() || !GetGame().GetMission())
		{
			Print("[DZKOTHG][MUSIC][ERROR] Toggle without mission");
			return;
		}

		DynamicMusicPlayer player = GetGame().GetMission().GetDynamicMusicPlayer();
		if (!player)
		{
			Print("[DZKOTHG][MUSIC][ERROR] Toggle without player");
			return;
		}

		DynamicMusicPlayerCategoryPlaybackData playback = new DynamicMusicPlayerCategoryPlaybackData();
		if (m_DZKOTHG_MusicPaused)
		{
			playback.m_Category = EDynamicMusicPlayerCategory.MENU;
			playback.m_FadeOut = false;
			m_DZKOTHG_MusicPaused = false;
		}
		else
		{
			playback.m_Category = EDynamicMusicPlayerCategory.NONE;
			playback.m_FadeOut = true;
			m_DZKOTHG_MusicPaused = true;
		}

		player.SetCategory(playback);
		DZKOTHG_SetMusicControlState(m_DZKOTHG_MusicPlay, "normal");
		Print("[DZKOTHG][MUSIC] paused=" + m_DZKOTHG_MusicPaused);
	}

	protected void DZKOTHG_ToggleMenuMusicMute()
	{
		if (!GetGame() || !GetGame().GetSoundScene())
			return;

		if (m_DZKOTHG_MusicMuted)
		{
			GetGame().GetSoundScene().SetMusicVolume(Math.Clamp(m_DZKOTHG_MusicVolumeBeforeMute, 0.0, DZKOTHG_MUSIC_VOLUME_MAX), 0.2);
			m_DZKOTHG_MusicMuted = false;
		}
		else
		{
			m_DZKOTHG_MusicVolumeBeforeMute = Math.Clamp(GetGame().GetSoundScene().GetMusicVolume(), 0.0, DZKOTHG_MUSIC_VOLUME_MAX);
			GetGame().GetSoundScene().SetMusicVolume(0.0, 0.2);
			m_DZKOTHG_MusicMuted = true;
		}

		DZKOTHG_SetMusicControlState(m_DZKOTHG_MusicMute, "normal");
	}

	protected void DZKOTHG_AdjustMenuMusicVolume(float delta)
	{
		if (!GetGame() || !GetGame().GetSoundScene())
			return;

		float currentVolume = GetGame().GetSoundScene().GetMusicVolume();
		if (m_DZKOTHG_MusicMuted && delta > 0.0)
		{
			currentVolume = m_DZKOTHG_MusicVolumeBeforeMute;
			m_DZKOTHG_MusicMuted = false;
		}

		float newVolume = Math.Clamp(currentVolume + delta, 0.0, DZKOTHG_MUSIC_VOLUME_MAX);
		GetGame().GetSoundScene().SetMusicVolume(newVolume, 0.15);
		m_DZKOTHG_MusicMuted = newVolume <= 0.001;
		if (!m_DZKOTHG_MusicMuted)
			m_DZKOTHG_MusicVolumeBeforeMute = newVolume;

		DZKOTHG_SetMusicControlState(m_DZKOTHG_MusicMute, "normal");
	}

	protected void DZKOTHG_RestoreMenuMusicVolume()
	{
		if (!m_DZKOTHG_MusicMuted || !GetGame() || !GetGame().GetSoundScene())
			return;

		GetGame().GetSoundScene().SetMusicVolume(Math.Clamp(m_DZKOTHG_MusicVolumeBeforeMute, 0.0, DZKOTHG_MUSIC_VOLUME_MAX), 0.2);
		m_DZKOTHG_MusicMuted = false;
	}

	override void OnShow()
	{
		super.OnShow();
		DZKOTHG_StopNativeMenuMusic();
		if (m_DZKOTHG_PlaylistController)
		{
			m_DZKOTHG_PlaylistController.RecoverAfterMenu();
			m_DZKOTHG_MusicPaused = !m_DZKOTHG_PlaylistController.WantsPlayback();
			DZKOTHG_SetMusicControlState(m_DZKOTHG_MusicPlay, "normal");
		}
	}

	override void OnHide()
	{
		// Untermenues blenden MainMenu aus. Die Session bleibt bis zum wirklichen
		// Ende der MainMenu-Mission erhalten und setzt Wiedergabe danach fort.
		super.OnHide();
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (w == m_Play)
			DZKOTHG_SetMainButtonState(w, "hover");
		else if (w == m_ChooseServer)
			DZKOTHG_SetMainButtonState(w, "hover");
		else if (w == m_CustomizeCharacter)
			DZKOTHG_SetMainButtonState(w, "hover");
		else if (w == m_DZKOTHG_VoteButton)
			DZKOTHG_SetMainButtonState(w, "hover");
		else if (w == m_SettingsButton)
			DZKOTHG_SetMenuImage("settings_image", "deutschz_hudz/gui/menu_green1/asset_pack/icons/dz_ui_icon_settings_green.paa");
		else if (w == m_Exit)
			DZKOTHG_SetMainButtonState(w, "hover");
		else if (w == m_DZKOTHG_DiscordButton)
			DZKOTHG_SetMainButtonState(w, "hover");
		else if (w == m_MessageButton)
			DZKOTHG_SetMainButtonState(w, "hover");
		else if (w == m_DZKOTHG_MusicPlay || w == m_DZKOTHG_MusicNext || w == m_DZKOTHG_MusicPrev || w == m_DZKOTHG_MusicVolumeDown || w == m_DZKOTHG_MusicMute || w == m_DZKOTHG_MusicVolumeUp)
			DZKOTHG_SetMusicControlState(w, "hover");
		else if (w == m_PrevCharacter)
			DZKOTHG_SetMenuImage("prev_img", "deutschz_hudz/gui/menu_green1/asset_pack/icons/dz_ui_icon_back_green.paa");
		else if (w == m_NextCharacter)
			DZKOTHG_SetMenuImage("next_img", "deutschz_hudz/gui/menu_green1/asset_pack/icons/dz_ui_icon_forward_green.paa");
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
			DZKOTHG_SetNavIcon(w, "power", true);

		return super.OnMouseEnter(w, x, y);
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (w == m_Play)
			DZKOTHG_SetMainButtonState(w, "normal");
		else if (w == m_ChooseServer)
			DZKOTHG_SetMainButtonState(w, "normal");
		else if (w == m_CustomizeCharacter)
			DZKOTHG_SetMainButtonState(w, "normal");
		else if (w == m_DZKOTHG_VoteButton)
			DZKOTHG_SetMainButtonState(w, "normal");
		else if (w == m_SettingsButton)
			DZKOTHG_SetMenuImage("settings_image", "deutschz_hudz/gui/menu_green1/asset_pack/icons/dz_ui_icon_settings_green.paa");
		else if (w == m_Exit)
			DZKOTHG_SetMainButtonState(w, "normal");
		else if (w == m_DZKOTHG_DiscordButton)
			DZKOTHG_SetMainButtonState(w, "normal");
		else if (w == m_MessageButton)
			DZKOTHG_SetMainButtonState(w, "normal");
		else if (w == m_DZKOTHG_MusicPlay || w == m_DZKOTHG_MusicNext || w == m_DZKOTHG_MusicPrev || w == m_DZKOTHG_MusicVolumeDown || w == m_DZKOTHG_MusicMute || w == m_DZKOTHG_MusicVolumeUp)
			DZKOTHG_SetMusicControlState(w, "normal");
		else if (w == m_PrevCharacter)
			DZKOTHG_SetMenuImage("prev_img", "deutschz_hudz/gui/menu_green1/asset_pack/icons/dz_ui_icon_back_green.paa");
		else if (w == m_NextCharacter)
			DZKOTHG_SetMenuImage("next_img", "deutschz_hudz/gui/menu_green1/asset_pack/icons/dz_ui_icon_forward_green.paa");
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
			DZKOTHG_SetNavIcon(w, "power", false);

		return super.OnMouseLeave(w, enterW, x, y);
	}

	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		if (button == MouseState.LEFT)
		{
			DZKOTHG_SetMainButtonState(w, "pressed");
			DZKOTHG_SetMusicControlState(w, "pressed");
			DZKOTHG_SetNavButtonState(w, "pressed");
		}

		if (w && w == m_CharacterRotationFrame)
		{
			if (m_ScenePC)
				m_ScenePC.CharacterRotationStart();

			return true;
		}

		return super.OnMouseButtonDown(w, x, y, button);
	}

	override bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		if (button == MouseState.LEFT)
		{
			DZKOTHG_SetMainButtonState(w, "hover");
			DZKOTHG_SetMusicControlState(w, "hover");
			DZKOTHG_SetNavButtonState(w, "hover");
		}

		return super.OnMouseButtonUp(w, x, y, button);
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

		if (w == m_DZKOTHG_MusicPlay)
		{
			Print("[DZKOTHG][MUSIC] click play");
			if (m_DZKOTHG_PlaylistController)
			{
				m_DZKOTHG_PlaylistController.TogglePlayPause();
				m_DZKOTHG_MusicPaused = !m_DZKOTHG_PlaylistController.WantsPlayback();
			}
			DZKOTHG_SetMusicControlState(m_DZKOTHG_MusicPlay, "normal");
			return true;
		}

		if (w == m_DZKOTHG_PlaylistDeutschZ || w == m_DZKOTHG_PlaylistEnglish || w == m_DZKOTHG_PlaylistHalftanZ)
		{
			int playlist = 0;
			if (w == m_DZKOTHG_PlaylistEnglish) playlist = 1;
			else if (w == m_DZKOTHG_PlaylistHalftanZ) playlist = 2;
			if (m_DZKOTHG_PlaylistController)
			{
				m_DZKOTHG_PlaylistController.SetPlaylist(playlist);
				m_DZKOTHG_MusicPaused = false;
			}
			DZKOTHG_UpdatePlaylistTabs();
			DZKOTHG_SetMusicControlState(m_DZKOTHG_MusicPlay, "normal");
			return true;
		}

		if (w == m_DZKOTHG_MusicPrev)
		{
			Print("[DZKOTHG][MUSIC] click previous");
			if (m_DZKOTHG_PlaylistController)
			{
				m_DZKOTHG_PlaylistController.Previous();
				m_DZKOTHG_MusicPaused = !m_DZKOTHG_PlaylistController.WantsPlayback();
			}
			DZKOTHG_SetMusicControlState(m_DZKOTHG_MusicPlay, "normal");
			return true;
		}

		if (w == m_DZKOTHG_MusicNext)
		{
			Print("[DZKOTHG][MUSIC] click next");
			if (m_DZKOTHG_PlaylistController)
			{
				m_DZKOTHG_PlaylistController.Next();
				m_DZKOTHG_MusicPaused = !m_DZKOTHG_PlaylistController.WantsPlayback();
			}
			DZKOTHG_SetMusicControlState(m_DZKOTHG_MusicPlay, "normal");
			return true;
		}

		if (w == m_DZKOTHG_MusicVolumeDown)
		{
			Print("[DZKOTHG][MUSIC] click volume down");
			if (m_DZKOTHG_PlaylistController)
				m_DZKOTHG_PlaylistController.ChangeVolume(-DZKOTHG_MUSIC_VOLUME_STEP);
			DZKOTHG_UpdateVolumeDisplay();
			return true;
		}

		if (w == m_DZKOTHG_MusicMute)
		{
			Print("[DZKOTHG][MUSIC] click mute");
			if (m_DZKOTHG_PlaylistController)
				m_DZKOTHG_PlaylistController.ToggleMute();
			m_DZKOTHG_MusicMuted = !m_DZKOTHG_MusicMuted;
			DZKOTHG_SetMusicControlState(m_DZKOTHG_MusicMute, "normal");
			DZKOTHG_UpdateVolumeDisplay();
			return true;
		}

		if (w == m_DZKOTHG_MusicVolumeUp)
		{
			Print("[DZKOTHG][MUSIC] click volume up");
			if (m_DZKOTHG_PlaylistController)
				m_DZKOTHG_PlaylistController.ChangeVolume(DZKOTHG_MUSIC_VOLUME_STEP);
			DZKOTHG_UpdateVolumeDisplay();
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
			if (m_DZKOTHG_PlaylistController)
				m_DZKOTHG_PlaylistController.TogglePlayPause();
			m_DZKOTHG_MusicPaused = !m_DZKOTHG_MusicPaused;
			DZKOTHG_SetMusicControlState(m_DZKOTHG_MusicPlay, "normal");
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

		if (w == m_DZKOTHG_VoteButton || w == m_DZKOTHG_DiscordButton || w == m_Exit || w == m_PlayVideo || w == m_Feedback || w == m_DZKOTHG_MusicPlay || w == m_DZKOTHG_MusicNext || w == m_DZKOTHG_MusicPrev || w == m_DZKOTHG_MusicVolumeDown || w == m_DZKOTHG_MusicMute || w == m_DZKOTHG_MusicVolumeUp)
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
