class DZPVPUI_LoadingVisualState
{
    static int BackgroundIndex = -1;
    static string BackgroundPath = "";
    static int LastJokeIndex = -1;
    static string CurrentJoke = "Wenn der Loot leer ist, war der Nachbar nur schneller.";
}

modded class LoadingScreen
{
    protected Widget m_DZPVPUI_LoadingOverlay;
    protected Widget m_DZPVPUI_LoadingAccent;
    protected Widget m_DZPVPUI_LoadingBottomCover;
    protected ImageWidget m_DZPVPUI_LoadingBackground;
    protected ImageWidget m_DZPVPUI_LoadingShield;
    protected ImageWidget m_DZPVPUI_LoadingWordmark;
    protected TextWidget m_DZPVPUI_LoadingJokeTitle;
    protected TextWidget m_DZPVPUI_LoadingJokeText;
    protected int m_DZPVPUI_LoadingBackgroundIndex;

    void LoadingScreen(DayZGame game)
    {
        if (!game || !game.GetLoadingWorkspace() || !m_WidgetRoot)
            return;

        m_DZPVPUI_LoadingOverlay = game.GetLoadingWorkspace().CreateWidgets("deutschz_hudz/gui/layouts/deutschz_hudz_loading.layout", m_WidgetRoot);
        if (!m_DZPVPUI_LoadingOverlay)
            return;

        if (m_TextWidgetTitle) m_TextWidgetTitle.Show(false);
        if (m_TextWidgetStatus) m_TextWidgetStatus.Show(false);
        if (m_ProgressLoading) m_ProgressLoading.Show(false);
        if (m_ProgressText) m_ProgressText.Show(false);

        m_TextWidgetStatus = TextWidget.Cast(m_DZPVPUI_LoadingOverlay.FindAnyWidget("DZPVPUI_LoadingStatus"));
        m_ProgressLoading = ProgressBarWidget.Cast(m_DZPVPUI_LoadingOverlay.FindAnyWidget("DZPVPUI_LoadingBar"));
        m_ProgressText = TextWidget.Cast(m_DZPVPUI_LoadingOverlay.FindAnyWidget("DZPVPUI_LoadingProgressText"));
        m_DZPVPUI_LoadingAccent = m_DZPVPUI_LoadingOverlay.FindAnyWidget("DZPVPUI_LoadingAccent");
        m_DZPVPUI_LoadingBottomCover = m_DZPVPUI_LoadingOverlay.FindAnyWidget("DZPVPUI_LoadingBottomCover");
        m_DZPVPUI_LoadingBackground = ImageWidget.Cast(m_DZPVPUI_LoadingOverlay.FindAnyWidget("DZPVPUI_LoadingBackground"));
        m_DZPVPUI_LoadingShield = ImageWidget.Cast(m_DZPVPUI_LoadingOverlay.FindAnyWidget("DZPVPUI_LoadingShield"));
        m_DZPVPUI_LoadingWordmark = ImageWidget.Cast(m_DZPVPUI_LoadingOverlay.FindAnyWidget("DZPVPUI_LoadingWordmark"));
        m_DZPVPUI_LoadingJokeTitle = TextWidget.Cast(m_DZPVPUI_LoadingOverlay.FindAnyWidget("DZPVPUI_LoadingJokeTitle"));
        m_DZPVPUI_LoadingJokeText = TextWidget.Cast(m_DZPVPUI_LoadingOverlay.FindAnyWidget("DZPVPUI_LoadingJokeText"));
        DZPVPUI_SelectLoadingBackground();
        DZPVPUI_SelectLoadingJoke();
        ProgressAsync.SetProgressData(m_ProgressLoading);
        DZPVPUI_HideVanillaLoadingDecoration();
        DZPVPUI_ApplyLoadingStyle();
    }

    override void Show()
    {
        super.Show();
        DZPVPUI_ApplyLoadingStyle();
    }

    override void ShowEx(DayZGame game)
    {
        super.ShowEx(game);
        DZPVPUI_ApplyLoadingStyle();
    }

    override void SetTitle(string title)
    {
        DZPVPUI_ApplyLoadingStyle();
    }

    override void SetStatus(string status)
    {
        if (m_TextWidgetStatus)
            m_TextWidgetStatus.SetText(status);
    }

    override void SetProgress(float val)
    {
        super.SetProgress(val);
        DZPVPUI_StyleProgress();
    }

    override void OnUpdate(float timeslice)
    {
        super.OnUpdate(timeslice);
        DZPVPUI_StyleProgress();
    }

    protected void DZPVPUI_ApplyLoadingStyle()
    {
        DZPVPUI_HideVanillaLoadingDecoration();
        if (m_TextWidgetTitle) m_TextWidgetTitle.Show(false);
        if (m_DZPVPUI_LoadingWordmark)
        {
            m_DZPVPUI_LoadingWordmark.Show(true);
            m_DZPVPUI_LoadingWordmark.SetColor(ARGB(255, 255, 255, 255));
        }
        if (m_TextWidgetStatus)
        {
            m_TextWidgetStatus.Show(true);
            m_TextWidgetStatus.SetColor(ARGB(255, 235, 20, 15));
            m_TextWidgetStatus.SetTextExactSize(16);
        }
        if (m_ImageLogoMid) m_ImageLogoMid.Show(false);
        if (m_ImageLogoCorner) m_ImageLogoCorner.Show(false);
        if (m_ModdedWarning) m_ModdedWarning.Show(false);
        DZPVPUI_StyleProgress();
    }

    protected void DZPVPUI_StyleProgress()
    {
        int green = ARGB(255, 84, 210, 92);
        if (m_DZPVPUI_LoadingAccent) m_DZPVPUI_LoadingAccent.SetColor(green);
        if (m_ProgressLoading) m_ProgressLoading.SetColor(green);
        if (m_DZPVPUI_LoadingJokeTitle) m_DZPVPUI_LoadingJokeTitle.SetColor(green);
        if (m_DZPVPUI_LoadingJokeText) m_DZPVPUI_LoadingJokeText.SetColor(ARGB(255, 255, 255, 255));
        if (m_ProgressText && m_ProgressLoading)
        {
            m_ProgressText.Show(false);
        }
    }

    protected void DZPVPUI_SelectLoadingJoke()
    {
        if (!m_DZPVPUI_LoadingJokeTitle || !m_DZPVPUI_LoadingJokeText)
            return;

        int jokeIndex = Math.RandomInt(0, 30);
        if (DZPVPUI_LoadingVisualState.LastJokeIndex >= 0)
        {
            while (jokeIndex == DZPVPUI_LoadingVisualState.LastJokeIndex)
                jokeIndex = Math.RandomInt(0, 30);
        }
        DZPVPUI_LoadingVisualState.LastJokeIndex = jokeIndex;

        string joke = "Wenn der Loot leer ist, war der Nachbar nur schneller.";
        switch (jokeIndex)
        {
            case 1: joke = "Ein Auto ohne Reifen bleibt ein emotionales Investment."; break;
            case 2: joke = "Wenn es klickt, war es hoffentlich nur die Tür."; break;
            case 3: joke = "Der perfekte Base-Plan beginnt mit: nur kurz provisorisch."; break;
            case 4: joke = "In Chernarus ist nur kurz looten eine Langzeitbeziehung."; break;
            case 5: joke = "Wer im Wald freundlich ruft, hat meistens bereits gezielt."; break;
            case 6: joke = "Die Batterie ist immer leer. Besonders wenn du sie brauchst."; break;
            case 7: joke = "Der Zombie war nicht schnell. Du warst nur optimistisch."; break;
            case 8: joke = "Hinter jeder guten Base steht ein Spieler ohne Nägel."; break;
            case 9: joke = "Ein Lada mit vier Reifen ist praktisch ein Endgame-Item."; break;
            case 10: joke = "Wenn du Schritte hörst: Es ist nie nur der Wind."; break;
            case 11: joke = "Der kürzeste Weg zum Loot führt meistens über deinen Tod."; break;
            case 12: joke = "Türen sind offen, verschlossen oder verdächtig."; break;
            case 13: joke = "Der Plan war perfekt, bis jemand die erste Tür öffnete."; break;
            case 14: joke = "Ich kenne den Weg bedeutet in Chernarus: Wir sind verloren."; break;
            case 15: joke = "Wer eine Dose Bohnen teilt, meint es ernst."; break;
            case 16: joke = "Das seltenste Item ist der Freund, der nichts aus deiner Kiste nimmt."; break;
            case 17: joke = "Feuer wärmt. Rauch verrät. Beides gleichzeitig ist Tradition."; break;
            case 18: joke = "Der Serverrestart kommt immer fünf Meter vor der Garage."; break;
            case 19: joke = "Der Typ ohne Hose hat vermutlich die beste Waffe."; break;
            case 20: joke = "Eine Landmine ist auch nur eine sehr direkte Wegmarkierung."; break;
            case 21: joke = "Wenn das Auto fliegt, war es kein Feature. Wahrscheinlich."; break;
            case 22: joke = "Looten ist wie Angeln, nur mit mehr Schüssen."; break;
            case 23: joke = "Der Erste am Airdrop ist selten der Letzte dort."; break;
            case 24: joke = "Vertrauen ist gut. Inventar prüfen ist DayZ."; break;
            case 25: joke = "Nur ein Zombie sind berühmte letzte Worte."; break;
            case 26: joke = "Im Zweifel: nachladen. Im größeren Zweifel: weglaufen."; break;
            case 27: joke = "Basebuilding: zehn Prozent bauen, neunzig Prozent Nägel suchen."; break;
            case 28: joke = "Wer den Schlüssel hat, findet das Auto nicht."; break;
            case 29: joke = "Friendly bedeutet meistens: Bitte bleib kurz stehen."; break;
        }

        m_DZPVPUI_LoadingJokeTitle.SetText("DEUTSCHZ JOKES");
        m_DZPVPUI_LoadingJokeTitle.SetTextExactSize(38);
        m_DZPVPUI_LoadingJokeText.SetText(joke);
        m_DZPVPUI_LoadingJokeText.SetTextExactSize(28);
        DZPVPUI_LoadingVisualState.CurrentJoke = joke;
    }

    protected void DZPVPUI_SelectLoadingBackground()
    {
        if (!m_DZPVPUI_LoadingBackground)
            return;

        int timeSalt = 0;
        if (GetGame())
            timeSalt = GetGame().GetTime();

        m_DZPVPUI_LoadingBackgroundIndex = Math.RandomInt(0, 13) + timeSalt;
        while (m_DZPVPUI_LoadingBackgroundIndex >= 13)
            m_DZPVPUI_LoadingBackgroundIndex -= 13;

        if (DZPVPUI_LoadingVisualState.BackgroundIndex >= 0 && m_DZPVPUI_LoadingBackgroundIndex == DZPVPUI_LoadingVisualState.BackgroundIndex)
        {
            m_DZPVPUI_LoadingBackgroundIndex++;
            if (m_DZPVPUI_LoadingBackgroundIndex >= 13)
                m_DZPVPUI_LoadingBackgroundIndex = 0;
        }
        string imagePath = "deutschz_hudz/gui/loading/loading_0.paa";
        switch (m_DZPVPUI_LoadingBackgroundIndex)
        {
            case 1: imagePath = "deutschz_hudz/gui/loading/loading_1.paa"; break;
            case 2: imagePath = "deutschz_hudz/gui/loading/loading_2.paa"; break;
            case 3: imagePath = "deutschz_hudz/gui/loading/loading_3.paa"; break;
            case 4: imagePath = "deutschz_hudz/gui/loading/loading_4.paa"; break;
            case 5: imagePath = "deutschz_hudz/gui/loading/loading_5.paa"; break;
            case 6: imagePath = "deutschz_hudz/gui/loading/loading_6.paa"; break;
            case 7: imagePath = "deutschz_hudz/gui/loading/loading_7.paa"; break;
            case 8: imagePath = "deutschz_hudz/gui/loading/loading_8.paa"; break;
            case 9: imagePath = "deutschz_hudz/gui/loading/loading_9.paa"; break;
            case 10: imagePath = "deutschz_hudz/gui/loading/loading_10.paa"; break;
            case 11: imagePath = "deutschz_hudz/gui/loading/loading_11.paa"; break;
            case 12: imagePath = "deutschz_hudz/gui/loading/loading_12.paa"; break;
        }

        m_DZPVPUI_LoadingBackground.LoadImageFile(0, imagePath);
        m_DZPVPUI_LoadingBackground.SetImage(0);
        m_DZPVPUI_LoadingBackground.SetColor(ARGB(255, 255, 255, 255));
        DZPVPUI_LoadingVisualState.BackgroundIndex = m_DZPVPUI_LoadingBackgroundIndex;
        DZPVPUI_LoadingVisualState.BackgroundPath = imagePath;
        if (m_DZPVPUI_LoadingBottomCover)
            m_DZPVPUI_LoadingBottomCover.Show(true);
    }

    protected void DZPVPUI_HideVanillaLoadingDecoration()
    {
        Widget widget;

        widget = m_WidgetRoot.FindAnyWidget("BottomPanel");
        if (widget) widget.SetColor(ARGB(0, 0, 0, 0));

        widget = m_WidgetRoot.FindAnyWidget("LinesImageLeft");
        if (widget) widget.Show(false);

        widget = m_WidgetRoot.FindAnyWidget("LinesRightImage");
        if (widget) widget.Show(false);

        widget = m_WidgetRoot.FindAnyWidget("hintIcon");
        if (widget) widget.Show(false);
    }
}

modded class LoginTimeBase
{
    protected ImageWidget m_DZPVPUI_LoginBackground;
    protected Widget m_DZPVPUI_LoginBottomCover;
    protected TextWidget m_DZPVPUI_LoginBrand;
    protected TextWidget m_DZPVPUI_LoginJoke;

    override Widget Init()
    {
        layoutRoot = g_Game.GetWorkspace().CreateWidgets("deutschz_hudz/gui/layouts/deutschz_hudz_login_time.layout");
        if (!layoutRoot)
            return null;

        m_txtDescription = TextWidget.Cast(layoutRoot.FindAnyWidget("txtDescription"));
        m_txtLabel = TextWidget.Cast(layoutRoot.FindAnyWidget("txtLabel"));
        m_btnLeave = ButtonWidget.Cast(layoutRoot.FindAnyWidget("btnLeave"));
        m_DZPVPUI_LoginBackground = ImageWidget.Cast(layoutRoot.FindAnyWidget("DZPVPUI_LoginBackground"));
        m_DZPVPUI_LoginBottomCover = layoutRoot.FindAnyWidget("DZPVPUI_LoginBottomCover");
        m_DZPVPUI_LoginBrand = TextWidget.Cast(layoutRoot.FindAnyWidget("DZPVPUI_LoginBrand"));
        m_DZPVPUI_LoginJoke = TextWidget.Cast(layoutRoot.FindAnyWidget("DZPVPUI_LoginJoke"));

        if (m_DZPVPUI_LoginBackground)
        {
            m_DZPVPUI_LoginBackground.LoadImageFile(0, DZPVPUI_LoadingVisualState.BackgroundPath);
            m_DZPVPUI_LoginBackground.SetImage(0);
            m_DZPVPUI_LoginBackground.SetColor(ARGB(255, 255, 255, 255));
        }

        if (m_DZPVPUI_LoginBottomCover)
            m_DZPVPUI_LoginBottomCover.Show(false);

        if (m_txtDescription)
        {
            m_txtDescription.Show(true);
            m_txtDescription.SetTextExactSize(15);
        }
        if (m_txtLabel)
            m_txtLabel.SetTextExactSize(23);
        if (m_DZPVPUI_LoginBrand)
        {
            m_DZPVPUI_LoginBrand.SetText(DZPVPUI_SelectLoginHeadline());
            m_DZPVPUI_LoginBrand.SetTextExactSize(16);
        }
        if (m_DZPVPUI_LoginJoke)
        {
            m_DZPVPUI_LoginJoke.SetText(DZPVPUI_SelectLoginJoke());
            m_DZPVPUI_LoginJoke.SetTextExactSize(13);
        }

        Widget notificationRoot = layoutRoot.FindAnyWidget("notification_root");
        if (notificationRoot)
            notificationRoot.Show(false);

        return layoutRoot;
    }

    protected string DZPVPUI_SelectLoginHeadline()
    {
        switch (Math.RandomInt(0, 8))
        {
            case 1: return "DEIN ABENTEUER WIRD GELADEN";
            case 2: return "DEINE STORY WIRD VON DIR GEMACHT";
            case 3: return "VERBINDUNG ZU DEUTSCHZ WIRD HERGESTELLT";
            case 4: return "CHERNARUS WARTET NICHT";
            case 5: return "DEIN NAECHSTES KAPITEL BEGINNT";
            case 6: return "DEUTSCHZ BEREITET DEINE WELT VOR";
            case 7: return "GLEICH GEHT ES LOS";
        }
        return "DEUTSCHZ // DEIN ABENTEUER BEGINNT";
    }

    protected string DZPVPUI_SelectLoginJoke()
    {
        switch (Math.RandomInt(0, 12))
        {
            case 1: return "Bitte nicht ausschalten. Die Zombies werden gerade sortiert.";
            case 2: return "Wir suchen noch den einen Reifen, der wirklich passt.";
            case 3: return "Die Bohnen werden auf Zimmertemperatur gebracht.";
            case 4: return "Dein Loot wird geladen. Der vom Nachbarn leider auch.";
            case 5: return "Noch kurz warten. Chernarus zieht sich gerade die Schuhe an.";
            case 6: return "Die Tuer ist gleich offen. Vermutlich.";
            case 7: return "Wir verteilen gerade die letzten Naegel auf der Karte.";
            case 8: return "Der Lada startet schon. Emotional zumindest.";
            case 9: return "Friendly wird uebersetzt. Ergebnis weiterhin unklar.";
            case 10: return "Dein Charakter ueberlegt noch, wo er gestern gestorben ist.";
            case 11: return "Das Abenteuer laedt. Der Hunger war bereits fertig.";
        }
        return "Bitte warten. Jemand muss noch den Airdrop falsch parken.";
    }

    override void Show()
    {
        if (layoutRoot)
            layoutRoot.Show(true);
    }
}
