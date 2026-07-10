modded class LoadingScreen
{
	protected static ref TStringArray DZKOTHG_LoadingImages;
	protected static ref TStringArray DZKOTHG_LoadingJokes;
	protected string m_DZKOTHG_CurrentJoke;

	override void Show()
	{
		super.Show();
		DZKOTHG_ApplyLoadingScreen();
	}

	override void ShowEx(DayZGame game)
	{
		super.ShowEx(game);
		DZKOTHG_ApplyLoadingScreen();
	}

	override void SetTitle(string title)
	{
		DZKOTHG_ApplyJokeWidgets();
	}

	override void SetStatus(string status)
	{
		DZKOTHG_ApplyJokeWidgets();
	}

	protected void DZKOTHG_ApplyLoadingScreen()
	{
		DZKOTHG_EnsureLoadingData();

		string imagePath = DZKOTHG_LoadingImages.GetRandomElement();
		m_DZKOTHG_CurrentJoke = DZKOTHG_LoadingJokes.GetRandomElement();
		DZKOTHG_DisableVanillaHintPanel();

		if (m_ImageWidgetBackground)
		{
			m_ImageWidgetBackground.LoadMaskTexture("DeutschZ_MenuMusic/data/ui/loading/null.paa");
			m_ImageWidgetBackground.LoadImageFile(0, imagePath);
			m_ImageWidgetBackground.SetImage(0);
			m_ImageWidgetBackground.SetColor(ARGB(255, 255, 255, 255));
			m_ImageWidgetBackground.Show(true);
		}

		if (m_ImageBackground)
		{
			m_ImageBackground.LoadImageFile(0, imagePath);
			m_ImageBackground.SetImage(0);
			m_ImageBackground.SetColor(ARGB(255, 255, 255, 255));
		}

		if (m_ImageLogoMid)
			m_ImageLogoMid.Show(false);

		if (m_ImageLogoCorner)
			m_ImageLogoCorner.Show(false);

		if (m_ModdedWarning)
			m_ModdedWarning.Show(false);

		DZKOTHG_ApplyJokeWidgets();

		Print("[DZKOTHG][CLIENT] LoadingScreen image active: " + imagePath);
	}

	protected void DZKOTHG_ApplyJokeWidgets()
	{
		if (m_TextWidgetTitle)
		{
			m_TextWidgetTitle.Show(true);
			m_TextWidgetTitle.SetPos(0, 0.175);
			m_TextWidgetTitle.SetText("DeutschZ Jokes");
		}

		if (m_TextWidgetStatus)
		{
			m_TextWidgetStatus.Show(true);
			m_TextWidgetStatus.SetPos(0, 0.147);
			m_TextWidgetStatus.SetText(DZKOTHG_GetCurrentJoke());
		}
	}

	protected void DZKOTHG_DisableVanillaHintPanel()
	{
		if (m_WidgetRoot)
		{
			Widget hintFrame = m_WidgetRoot.FindAnyWidget("hint_frame");
			if (hintFrame)
				hintFrame.Show(false);

			Widget hintFrameAlt = m_WidgetRoot.FindAnyWidget("hint_frame0");
			if (hintFrameAlt)
				hintFrameAlt.Show(false);
		}

	}

	protected string DZKOTHG_GetCurrentJoke()
	{
		DZKOTHG_EnsureLoadingData();

		if (m_DZKOTHG_CurrentJoke == "")
			m_DZKOTHG_CurrentJoke = DZKOTHG_LoadingJokes.GetRandomElement();

		return m_DZKOTHG_CurrentJoke;
	}

	protected static void DZKOTHG_EnsureLoadingData()
	{
		if (!DZKOTHG_LoadingImages)
		{
			DZKOTHG_LoadingImages = new TStringArray;
			DZKOTHG_LoadingImages.Insert("DeutschZ_MenuMusic/data/ui/loading/loading_0.paa");
			DZKOTHG_LoadingImages.Insert("DeutschZ_MenuMusic/data/ui/loading/loading_1.paa");
			DZKOTHG_LoadingImages.Insert("DeutschZ_MenuMusic/data/ui/loading/loading_2.paa");
		}

		if (!DZKOTHG_LoadingJokes)
		{
			DZKOTHG_LoadingJokes = new TStringArray;
			DZKOTHG_LoadingJokes.Insert("Wenn der Loot leer ist, war es bestimmt nur ein sehr schneller Nachbar.");
			DZKOTHG_LoadingJokes.Insert("Chernarus-Regel 1: Wer rennt, hat entweder Hunger oder schlechte Freunde.");
			DZKOTHG_LoadingJokes.Insert("Der perfekte Base-Plan beginnt immer mit: Das ist nur kurz provisorisch.");
			DZKOTHG_LoadingJokes.Insert("Ein Auto ohne Reifen ist in DayZ immer noch ein emotionales Investment.");
			DZKOTHG_LoadingJokes.Insert("Wenn es klickt, war es hoffentlich nur die Tuer.");
			DZKOTHG_LoadingJokes.Insert("DeutschZ Tipp: Erst nachladen, dann Held spielen.");
			DZKOTHG_LoadingJokes.Insert("Der Wald macht keine Geraeusche. Ausser er macht doch welche.");
			DZKOTHG_LoadingJokes.Insert("Jede Abkuerzung ist sicher, bis sie es nicht mehr ist.");
			DZKOTHG_LoadingJokes.Insert("Wer in Cherno hupt, unterschreibt meistens nur schneller.");
			DZKOTHG_LoadingJokes.Insert("Ein leerer Magen macht keinen Laerm. Dein Magen schon.");
			DZKOTHG_LoadingJokes.Insert("Wenn die Tuer offen war, war es nie der Wind.");
			DZKOTHG_LoadingJokes.Insert("Base-Regel: Erst abschliessen, dann angeben.");
			DZKOTHG_LoadingJokes.Insert("Der beste Fluchtplan beginnt mit vollen Schuhen.");
			DZKOTHG_LoadingJokes.Insert("Wer den Heli hoert, sollte den Kopf benutzen.");
			DZKOTHG_LoadingJokes.Insert("Loot ist wie Vertrauen: selten da, wenn man es braucht.");
			DZKOTHG_LoadingJokes.Insert("Wenn der Trader laechelt, hast du zu billig verkauft.");
			DZKOTHG_LoadingJokes.Insert("Eine volle Mag ist besser als ein guter Vorsatz.");
			DZKOTHG_LoadingJokes.Insert("DeutschZ Regel: Freundlich sein, aber nachladen.");
			DZKOTHG_LoadingJokes.Insert("Wer am Feuer singt, leuchtet auch fuer andere.");
			DZKOTHG_LoadingJokes.Insert("KOTH beginnt nicht am Huegel. KOTH beginnt im Kopf.");
			DZKOTHG_LoadingJokes.Insert("Ein Code Lock schuetzt nur vor Leuten ohne Geduld.");
			DZKOTHG_LoadingJokes.Insert("Die beste Tarnung ist manchmal einfach nicht reden.");
			DZKOTHG_LoadingJokes.Insert("Ein Plan ohne Benzin ist nur Wanddeko.");
			DZKOTHG_LoadingJokes.Insert("Wenn es zu ruhig ist, fehlt dir nur die Information.");
			DZKOTHG_LoadingJokes.Insert("Rote Zone, gruene Hoffnung, leere Taschen.");
			DZKOTHG_LoadingJokes.Insert("Wer Wasser sucht, findet meistens erst Probleme.");
			DZKOTHG_LoadingJokes.Insert("Bei DeutschZ gilt: Erst ueberleben, dann diskutieren.");
			DZKOTHG_LoadingJokes.Insert("Der letzte Reifen ist immer der teuerste.");
		}
	}
};

modded class LoginTimeBase extends UIScriptedMenu
{
	override void Show()
	{
		super.Show();
		DZKOTHG_ApplyLoginBackground();
	}

	protected void DZKOTHG_ApplyLoginBackground()
	{
		if (!layoutRoot)
			return;

		DZKOTHG_DisableLoginHintPanel();
		DZKOTHG_EnsureLoginScreens();
		ImageWidget background = ImageWidget.Cast(layoutRoot.FindAnyWidget("Background"));
		if (!background)
			background = ImageWidget.Cast(layoutRoot.FindAnyWidget("ImageBackground"));

		if (background)
		{
			string imagePath = DZKOTHG_LoadingScreenStore.GetRandomScreen();
			background.LoadImageFile(0, imagePath);
			Print("[DZKOTHG][CLIENT] LoginTime image active: " + imagePath);
		}
	}

	protected void DZKOTHG_EnsureLoginScreens()
	{
		DZKOTHG_LoadingScreenStore.Ensure();
	}

	protected void DZKOTHG_DisableLoginHintPanel()
	{
		Widget hintFrame = layoutRoot.FindAnyWidget("hint_frame0");
		if (hintFrame)
			hintFrame.Show(false);

		Widget hintFrameAlt = layoutRoot.FindAnyWidget("hint_frame");
		if (hintFrameAlt)
			hintFrameAlt.Show(false);

	}
};

modded class LoginQueueBase extends UIScriptedMenu
{
	override void Show()
	{
		super.Show();
		DZKOTHG_ApplyQueueBackground();
	}

	protected void DZKOTHG_ApplyQueueBackground()
	{
		if (!layoutRoot)
			return;

		DZKOTHG_DisableQueueHintPanel();
		DZKOTHG_LoadingScreenStore.Ensure();
		ImageWidget background = ImageWidget.Cast(layoutRoot.FindAnyWidget("Background"));
		if (!background)
			background = ImageWidget.Cast(layoutRoot.FindAnyWidget("ImageBackground"));

		if (background)
		{
			string imagePath = DZKOTHG_LoadingScreenStore.GetRandomScreen();
			background.LoadImageFile(0, imagePath);
			Print("[DZKOTHG][CLIENT] LoginQueue image active: " + imagePath);
		}
	}

	protected void DZKOTHG_DisableQueueHintPanel()
	{
		Widget hintFrame = layoutRoot.FindAnyWidget("hint_frame0");
		if (hintFrame)
			hintFrame.Show(false);

		Widget hintFrameAlt = layoutRoot.FindAnyWidget("hint_frame");
		if (hintFrameAlt)
			hintFrameAlt.Show(false);

	}
};

class DZKOTHG_LoadingScreenStore
{
	protected static ref TStringArray s_Screens;

	static void Ensure()
	{
		if (s_Screens)
			return;

		s_Screens = new TStringArray;
		s_Screens.Insert("DeutschZ_MenuMusic/data/ui/loading/loading_0.paa");
		s_Screens.Insert("DeutschZ_MenuMusic/data/ui/loading/loading_1.paa");
		s_Screens.Insert("DeutschZ_MenuMusic/data/ui/loading/loading_2.paa");
	}

	static string GetRandomScreen()
	{
		Ensure();
		return s_Screens.GetRandomElement();
	}
};
