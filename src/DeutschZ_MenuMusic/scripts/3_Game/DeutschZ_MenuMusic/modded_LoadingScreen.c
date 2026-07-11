modded class LoadingScreen
{
	protected float m_DZKOTHG_SlideshowTime;
	protected string m_DZKOTHG_CurrentImage;
	protected ref DZKOTHG_LoadingEntry m_DZKOTHG_CurrentEntry;
	protected Widget m_DZKOTHG_LoadingOverlay;
	protected ImageWidget m_DZKOTHG_LoadingLogo;

	void LoadingScreen(DayZGame game)
	{
		if (!game || !game.GetLoadingWorkspace() || !m_WidgetRoot)
			return;

		m_DZKOTHG_LoadingOverlay = game.GetLoadingWorkspace().CreateWidgets("DeutschZ_MenuMusic/gui/dzkothg_loading_overlay.layout", m_WidgetRoot);
		if (!m_DZKOTHG_LoadingOverlay)
			return;

		if (m_TextWidgetTitle)
			m_TextWidgetTitle.Show(false);
		if (m_TextWidgetStatus)
			m_TextWidgetStatus.Show(false);
		if (m_ProgressLoading)
			m_ProgressLoading.Show(false);
		if (m_ProgressText)
			m_ProgressText.Show(false);

		m_TextWidgetTitle = TextWidget.Cast(m_DZKOTHG_LoadingOverlay.FindAnyWidget("DZKOTHG_LoadingTitle"));
		m_TextWidgetStatus = TextWidget.Cast(m_DZKOTHG_LoadingOverlay.FindAnyWidget("DZKOTHG_LoadingStatus"));
		m_ProgressLoading = ProgressBarWidget.Cast(m_DZKOTHG_LoadingOverlay.FindAnyWidget("DZKOTHG_LoadingBar"));
		m_ProgressText = TextWidget.Cast(m_DZKOTHG_LoadingOverlay.FindAnyWidget("DZKOTHG_LoadingProgressText"));
		m_DZKOTHG_LoadingLogo = ImageWidget.Cast(m_DZKOTHG_LoadingOverlay.FindAnyWidget("DZKOTHG_LoadingLogo"));
		ProgressAsync.SetProgressData(m_ProgressLoading);
	}

	override void Show()
	{
		super.Show();
		DZKOTHG_ApplyLoadingScreen(true);
	}

	override void ShowEx(DayZGame game)
	{
		super.ShowEx(game);
		DZKOTHG_ApplyLoadingScreen(true);
	}

	override void SetTitle(string title)
	{
		DZKOTHG_ApplyContentWidgets();
	}

	override void SetStatus(string status)
	{
		DZKOTHG_ApplyContentWidgets();
	}

	override void SetProgress(float val)
	{
		super.SetProgress(val);
		DZKOTHG_StyleProgress();
	}

	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);
		m_DZKOTHG_SlideshowTime += timeslice;
		if (m_DZKOTHG_SlideshowTime >= DZKOTHG_MenuContentStore.GetSlideshowSeconds())
		{
			m_DZKOTHG_SlideshowTime = 0.0;
			DZKOTHG_ApplyLoadingScreen(false);
		}

		DZKOTHG_StyleProgress();
	}

	protected void DZKOTHG_ApplyLoadingScreen(bool resetTimer)
	{
		if (resetTimer)
			m_DZKOTHG_SlideshowTime = 0.0;

		m_DZKOTHG_CurrentImage = DZKOTHG_MenuContentStore.GetNextScreen(m_DZKOTHG_CurrentImage);
		m_DZKOTHG_CurrentEntry = DZKOTHG_MenuContentStore.GetNextLoadingEntry();
		DZKOTHG_DisableVanillaHintPanel();

		if (m_ImageWidgetBackground)
		{
			m_ImageWidgetBackground.LoadMaskTexture("DeutschZ_MenuMusic/data/ui/loading/null.paa");
			m_ImageWidgetBackground.LoadImageFile(0, m_DZKOTHG_CurrentImage, true);
			m_ImageWidgetBackground.SetImage(0);
			m_ImageWidgetBackground.SetColor(ARGB(255, 255, 255, 255));
			m_ImageWidgetBackground.Show(true);
		}

		if (m_ImageBackground)
		{
			m_ImageBackground.LoadImageFile(0, m_DZKOTHG_CurrentImage, true);
			m_ImageBackground.SetImage(0);
			m_ImageBackground.SetColor(ARGB(255, 255, 255, 255));
		}

		DZKOTHG_ApplyBranding();
		DZKOTHG_ApplyContentWidgets();
		DZKOTHG_StyleProgress();
		Print("[DZKOTHG][CLIENT] Loading image active: " + m_DZKOTHG_CurrentImage);
	}

	protected void DZKOTHG_ApplyBranding()
	{
		DZKOTHG_MenuContentConfig config = DZKOTHG_MenuContentStore.GetConfig();
		if (!config)
			return;

		if (m_ImageLogoMid)
			m_ImageLogoMid.Show(false);
		if (m_ImageLogoCorner)
			m_ImageLogoCorner.Show(false);

		if (m_DZKOTHG_LoadingLogo)
		{
			m_DZKOTHG_LoadingLogo.LoadImageFile(0, config.LogoPath);
			m_DZKOTHG_LoadingLogo.SetImage(0);
			m_DZKOTHG_LoadingLogo.Show(true);
		}

		if (m_ModdedWarning)
			m_ModdedWarning.Show(false);

		if (m_WidgetRoot)
		{
			ImageWidget hintIcon = ImageWidget.Cast(m_WidgetRoot.FindAnyWidget("hintIcon"));
			if (hintIcon)
			{
				hintIcon.LoadImageFile(0, config.HintIconPath);
				hintIcon.SetImage(0);
				hintIcon.Show(true);
			}
		}
	}

	protected void DZKOTHG_ApplyContentWidgets()
	{
		if (!m_DZKOTHG_CurrentEntry)
			m_DZKOTHG_CurrentEntry = DZKOTHG_MenuContentStore.GetNextLoadingEntry();

		if (m_TextWidgetTitle)
		{
			m_TextWidgetTitle.Show(true);
			string title = "DEUTSCHZ";
			if (m_DZKOTHG_CurrentEntry)
				title = "DEUTSCHZ " + m_DZKOTHG_CurrentEntry.Category;
			m_TextWidgetTitle.SetText(title);
			m_TextWidgetTitle.SetColor(DZKOTHG_UITheme.BrandGreen());
		}

		if (m_TextWidgetStatus)
		{
			m_TextWidgetStatus.Show(true);
			string status = "Willkommen bei DeutschZ.";
			if (m_DZKOTHG_CurrentEntry)
				status = m_DZKOTHG_CurrentEntry.Text;
			m_TextWidgetStatus.SetText(status);
			m_TextWidgetStatus.SetColor(DZKOTHG_UITheme.PrimaryText());
		}
	}

	protected void DZKOTHG_StyleProgress()
	{
		if (m_ProgressLoading)
			m_ProgressLoading.SetColor(DZKOTHG_UITheme.BrandGreen());

		if (m_ProgressText && m_ProgressLoading)
		{
			float normalizedProgress = Math.Clamp(m_ProgressLoading.GetCurrent(), 0.0, 1.0);
			int progressPercent = Math.Round(normalizedProgress * 100.0);
			m_ProgressText.Show(true);
			m_ProgressText.SetColor(DZKOTHG_UITheme.BrandGreen());
			m_ProgressText.SetText("DEUTSCHZ LAEDT  " + progressPercent.ToString() + "%");
		}
	}

	protected void DZKOTHG_DisableVanillaHintPanel()
	{
		if (!m_WidgetRoot)
			return;

		Widget hintFrame = m_WidgetRoot.FindAnyWidget("hint_frame");
		if (hintFrame)
			hintFrame.Show(false);

		Widget hintFrameAlt = m_WidgetRoot.FindAnyWidget("hint_frame0");
		if (hintFrameAlt)
			hintFrameAlt.Show(false);
	}
}

modded class LoginTimeBase
{
	protected float m_DZKOTHG_SlideshowTime;
	protected string m_DZKOTHG_CurrentImage;
	protected string m_DZKOTHG_ServerStatus;

	override void Show()
	{
		super.Show();
		DZKOTHG_RefreshLoginScreen(true);
	}

	override void Update(float timeslice)
	{
		super.Update(timeslice);
		m_DZKOTHG_SlideshowTime += timeslice;
		if (m_DZKOTHG_SlideshowTime >= DZKOTHG_MenuContentStore.GetSlideshowSeconds())
		{
			m_DZKOTHG_SlideshowTime = 0.0;
			DZKOTHG_RefreshLoginScreen(false);
		}
	}

	override void SetStatus(string status)
	{
		m_DZKOTHG_ServerStatus = status;
		DZKOTHG_UpdateLoginMessage();
	}

	protected void DZKOTHG_RefreshLoginScreen(bool resetTimer)
	{
		if (!layoutRoot)
			return;

		if (resetTimer)
			m_DZKOTHG_SlideshowTime = 0.0;

		m_DZKOTHG_CurrentImage = DZKOTHG_MenuContentStore.GetNextScreen(m_DZKOTHG_CurrentImage);
		DZKOTHG_ApplyLoginBranding(layoutRoot, m_DZKOTHG_CurrentImage);
		DZKOTHG_UpdateLoginMessage();
	}

	protected void DZKOTHG_UpdateLoginMessage()
	{
		if (!m_txtDescription)
			return;

		string text = m_DZKOTHG_ServerStatus;
		if (text != "")
			text = text + "\n";
		m_txtDescription.SetText(text + DZKOTHG_MenuContentStore.GetNextServerMessage());
	}

	protected void DZKOTHG_ApplyLoginBranding(Widget root, string imagePath)
	{
		ImageWidget background = ImageWidget.Cast(root.FindAnyWidget("Background"));
		if (!background)
			background = ImageWidget.Cast(root.FindAnyWidget("ImageBackground"));
		if (background)
		{
			background.LoadImageFile(0, imagePath, true);
			background.SetImage(0);
		}

		DZKOTHG_DialogStyler.Apply(root);
	}
}

modded class LoginQueueBase
{
	protected float m_DZKOTHG_SlideshowTime;
	protected string m_DZKOTHG_CurrentImage;
	protected string m_DZKOTHG_CurrentMessage;

	override void Show()
	{
		super.Show();
		DZKOTHG_RefreshQueueScreen(true);
	}

	override void Update(float timeslice)
	{
		super.Update(timeslice);
		m_DZKOTHG_SlideshowTime += timeslice;
		if (m_DZKOTHG_SlideshowTime >= DZKOTHG_MenuContentStore.GetSlideshowSeconds())
		{
			m_DZKOTHG_SlideshowTime = 0.0;
			DZKOTHG_RefreshQueueScreen(false);
		}
	}

	override void SetPosition(int position)
	{
		super.SetPosition(position);
		DZKOTHG_UpdateQueueMessage();
	}

	protected void DZKOTHG_RefreshQueueScreen(bool resetTimer)
	{
		if (!layoutRoot)
			return;

		if (resetTimer)
			m_DZKOTHG_SlideshowTime = 0.0;

		m_DZKOTHG_CurrentImage = DZKOTHG_MenuContentStore.GetNextScreen(m_DZKOTHG_CurrentImage);
		m_DZKOTHG_CurrentMessage = DZKOTHG_MenuContentStore.GetNextServerMessage();
		ImageWidget background = ImageWidget.Cast(layoutRoot.FindAnyWidget("Background"));
		if (!background)
			background = ImageWidget.Cast(layoutRoot.FindAnyWidget("ImageBackground"));
		if (background)
		{
			background.LoadImageFile(0, m_DZKOTHG_CurrentImage, true);
			background.SetImage(0);
		}

		DZKOTHG_DialogStyler.Apply(layoutRoot);
		DZKOTHG_UpdateQueueMessage();
	}

	protected void DZKOTHG_UpdateQueueMessage()
	{
		if (!m_txtNote)
			return;

		string text = "WARTESCHLANGE";
		if (m_iPosition >= 0)
			text = text + " | PLATZ " + m_iPosition.ToString();
		if (m_DZKOTHG_CurrentMessage != "")
			text = text + "\n" + m_DZKOTHG_CurrentMessage;
		m_txtNote.SetText(text);
	}
}

class DZKOTHG_DialogStyler
{
	static void Apply(Widget root)
	{
		if (!root)
			return;

		DZKOTHG_MenuContentConfig config = DZKOTHG_MenuContentStore.GetConfig();
		Widget hintFrame = root.FindAnyWidget("hint_frame0");
		if (hintFrame)
			hintFrame.Show(false);
		Widget hintFrameAlt = root.FindAnyWidget("hint_frame");
		if (hintFrameAlt)
			hintFrameAlt.Show(false);

		ImageWidget hintIcon = ImageWidget.Cast(root.FindAnyWidget("hintIcon"));
		if (hintIcon && config)
		{
			hintIcon.LoadImageFile(0, config.HintIconPath);
			hintIcon.SetImage(0);
			hintIcon.Show(true);
		}

		Widget separator = root.FindAnyWidget("SeparatorPanel");
		if (separator)
			separator.SetColor(DZKOTHG_UITheme.BrandGreen());
	}
}
