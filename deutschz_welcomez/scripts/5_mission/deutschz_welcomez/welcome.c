class DZWelcomeZPanel : ScriptedWidgetEventHandler
{
    protected Widget m_Root;
    protected ButtonWidget m_CloseButton;
    protected TextWidget m_Heading;
    protected TextWidget m_Status;
    protected TextWidget m_Next;
    protected TextWidget m_Progress;
    protected TextWidget m_Detail;
    protected TextWidget m_EventStatus;
    protected TextWidget m_EventNext;
    protected TextWidget m_StoryStatus;
    protected AbstractWave m_Music;

    bool IsOpen() { return m_Root != null; }

    protected bool HasExternalMenu()
    {
        if (GetGame() && GetGame().GetUIManager().GetMenu()) return true;
#ifdef EXPANSIONMODSPAWNSELECTION
        if (GetDayZExpansion() && GetDayZExpansion().GetExpansionUIManager().GetMenu()) return true;
#endif
        return false;
    }

    bool Show(DZWelcomeZPayload payload)
    {
        if (!payload) return false;
        bool alreadyOpen = m_Root != null;
        if (!m_Root) m_Root = GetGame().GetWorkspace().CreateWidgets("deutschz_welcomez/gui/layouts/welcome.layout");
        if (!m_Root) { ErrorEx("[WelcomeZ] Layout konnte nicht geladen werden."); return false; }
        m_CloseButton = ButtonWidget.Cast(m_Root.FindAnyWidget("CloseButton"));
        m_Heading = TextWidget.Cast(m_Root.FindAnyWidget("Heading"));
        m_Status = TextWidget.Cast(m_Root.FindAnyWidget("StatusText"));
        m_Next = TextWidget.Cast(m_Root.FindAnyWidget("NextText"));
        m_Progress = TextWidget.Cast(m_Root.FindAnyWidget("ProgressText"));
        m_Detail = TextWidget.Cast(m_Root.FindAnyWidget("DetailText"));
        m_EventStatus = TextWidget.Cast(m_Root.FindAnyWidget("EventStatusText"));
        m_EventNext = TextWidget.Cast(m_Root.FindAnyWidget("EventNextText"));
        m_StoryStatus = TextWidget.Cast(m_Root.FindAnyWidget("StoryStatusText"));
        if (!m_CloseButton || !m_Heading || !m_Status || !m_Next || !m_Progress || !m_Detail || !m_EventStatus || !m_EventNext || !m_StoryStatus)
        {
            ErrorEx("[WelcomeZ] Pflichtwidget fehlt."); Close(); return false;
        }
        m_CloseButton.SetHandler(this);
        m_Root.SetHandler(this);
        m_Heading.SetText(payload.Heading);
        m_Status.SetText(payload.CurrentStatus);
        m_Next.SetText(payload.NextStep);
        m_Progress.SetText(payload.Progress);
        m_Detail.SetText(payload.Detail);
        m_EventStatus.SetText(payload.EventStatus);
        m_EventNext.SetText(payload.EventNext);
        m_StoryStatus.SetText(payload.StoryStatus);
        if (payload.PlayMusic) m_MusicPending = true;
        if (alreadyOpen) return true;
        GetGame().GetInput().ChangeGameFocus(1, INPUT_DEVICE_MOUSE);
        GetGame().GetUIManager().ShowUICursor(true);
        SetFocus(m_CloseButton);
        int duration = 45000;
        if (payload.ShowTutorial) duration = 90000;
        GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(Close, duration, false);
        return true;
    }

    override bool OnClick(Widget w, int x, int y, int button)
    {
        if (w == m_CloseButton) { Close(); return true; }
        return false;
    }

    override bool OnKeyDown(Widget w, int x, int y, int key)
    {
        if (key == KeyCode.KC_ESCAPE && IsOpen())
        {
            Close();
            StopMusic();
            return true;
        }
        return false;
    }

    void Close()
    {
        if (GetGame()) GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(Close);
        bool wasOpen = m_Root != null;
        if (m_Root) { m_Root.Unlink(); m_Root = null; }
        m_CloseButton = null; m_Heading = null; m_Status = null; m_Next = null; m_Progress = null; m_Detail = null; m_EventStatus = null; m_EventNext = null; m_StoryStatus = null;
        if (wasOpen && GetGame())
        {
            GetGame().GetInput().ChangeGameFocus(-1, INPUT_DEVICE_MOUSE);
            if (HasExternalMenu())
                GetGame().GetUIManager().ShowUICursor(true);
            else
            {
                SetFocus(null);
                GetGame().GetUIManager().ShowUICursor(false);
            }
        }
    }

    protected void StartMusic()
    {
        StopMusic();
        string soundSet = "deutschz_welcomez_music_soundset";
        int selection = Math.RandomInt(0, 3);
        if (selection == 1) soundSet = "deutschz_welcomez_morozov_soundset";
        if (selection == 2) soundSet = "deutschz_welcomez_king_soundset";
        SoundParams soundParams = new SoundParams(soundSet);
        if (!soundParams.IsValid()) { ErrorEx("[WelcomeZ] Ungültiges SoundSet."); return; }
        SoundObjectBuilder builder = new SoundObjectBuilder(soundParams);
        SoundObject soundObject = builder.BuildSoundObject();
        if (!soundObject) return;
        soundObject.SetKind(WaveKind.WAVEUI);
        m_Music = GetGame().GetSoundScene().Play2D(soundObject, builder);
        if (m_Music) { m_Music.SetVolume(0.75); m_Music.Play(); }
    }

    protected bool m_MusicPending;

    void StopMusic()
    {
        m_MusicPending = false;
        if (m_Music) { m_Music.Stop(); m_Music = null; }
    }

    void ReleaseMusic()
    {
        if (!m_MusicPending) return;
        m_MusicPending = false;
        StartMusic();
        Print("[WelcomeZ] Startmusik nach Willkommen freigegeben.");
    }

    void Shutdown()
    {
        Close();
        StopMusic();
    }
}

modded class MissionGameplay
{
    protected ref DZWelcomeZPanel m_DZWelcomeZPanel;
    protected int m_DZWelcomeZRevision;
    protected bool m_DZWelcomeZWarningFinished;
    protected int m_DZWelcomeZNextEventRefresh;
    protected EffectSound m_DZWelcomeZWarningSound;

    override void OnKeyPress(int key)
    {
        if (key == KeyCode.KC_ESCAPE && m_DZWelcomeZPanel && m_DZWelcomeZPanel.IsOpen())
        {
            m_DZWelcomeZPanel.Close();
            m_DZWelcomeZPanel.StopMusic();
            return;
        }
        super.OnKeyPress(key);
    }

    protected bool DZWelcomeZHasBlockingMenu()
    {
        if (GetGame().GetUIManager().GetMenu()) return true;
#ifdef EXPANSIONMODSPAWNSELECTION
        if (GetDayZExpansion() && GetDayZExpansion().GetExpansionUIManager().GetMenu()) return true;
#endif
        return false;
    }

    protected bool DZWelcomeZCanPresent()
    {
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (!player || !player.IsPlayerSelected() || !player.IsAlive() || player.IsUnconscious()) return false;
        return !DZWelcomeZHasBlockingMenu();
    }

#ifdef deutschz_warningz
    override void deutschz_warningz_play(string sound_name)
    {
        super.deutschz_warningz_play(sound_name);
        if (sound_name != "welcomeevents" && sound_name != "welcome1" && sound_name != "welcome2") return;
        m_DZWelcomeZWarningFinished = false;
        if (m_DZWelcomeZWarningSound) m_DZWelcomeZWarningSound.Event_OnSoundWaveEnded.Remove(DZWelcomeZWarningEnded);
        m_DZWelcomeZWarningSound = deutschz_warningz_active_sound;
        if (m_DZWelcomeZWarningSound)
        {
            m_DZWelcomeZWarningSound.Event_OnSoundWaveEnded.Insert(DZWelcomeZWarningEnded);
            Print("[WelcomeZ] Warte auf Ende der WarningZ-Begruessung.");
        }
    }
#endif

    void DZWelcomeZWarningEnded(EffectSound finishedSound)
    {
        if (finishedSound != m_DZWelcomeZWarningSound) return;
        m_DZWelcomeZWarningFinished = true;
        if (m_DZWelcomeZWarningSound) m_DZWelcomeZWarningSound.Event_OnSoundWaveEnded.Remove(DZWelcomeZWarningEnded);
        m_DZWelcomeZWarningSound = null;
    }

    override void OnUpdate(float timeslice)
    {
        super.OnUpdate(timeslice);
        if (!m_DZWelcomeZPanel) m_DZWelcomeZPanel = new DZWelcomeZPanel;
        bool canPresent = DZWelcomeZCanPresent();
        if (m_DZWelcomeZPanel.IsOpen() && !canPresent) m_DZWelcomeZPanel.Close();
        if (canPresent && DZWelcomeZClientState.Revision != m_DZWelcomeZRevision && DZWelcomeZClientState.Payload)
        {
            m_DZWelcomeZRevision = DZWelcomeZClientState.Revision;
            if (m_DZWelcomeZPanel.Show(DZWelcomeZClientState.Payload) && DZWelcomeZClientState.Payload.ShowTutorial)
            {
                PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
                if (player) player.RPCSingleParam(DZWelcomeZConst.RPC_TUTORIAL_ACK, null, true, null);
            }
        }
        if (canPresent && m_DZWelcomeZPanel.IsOpen() && GetGame().GetTime() >= m_DZWelcomeZNextEventRefresh)
        {
            m_DZWelcomeZNextEventRefresh = GetGame().GetTime() + 10000;
            PlayerBase refreshPlayer = PlayerBase.Cast(GetGame().GetPlayer());
            if (refreshPlayer) refreshPlayer.RPCSingleParam(DZWelcomeZConst.RPC_REQUEST, null, true, null);
        }
#ifndef deutschz_warningz
        m_DZWelcomeZWarningFinished = true;
#endif
        if (!m_DZWelcomeZWarningSound) m_DZWelcomeZWarningFinished = true;
        if (m_DZWelcomeZWarningFinished) m_DZWelcomeZPanel.ReleaseMusic();
        UAInput book = GetUApi().GetInputByName("UADZWelcomeZBook");
        if (canPresent && book && book.LocalPress())
        {
            if (m_DZWelcomeZPanel.IsOpen()) m_DZWelcomeZPanel.Close();
            else
            {
                PlayerBase current = PlayerBase.Cast(GetGame().GetPlayer());
                if (current) current.RPCSingleParam(DZWelcomeZConst.RPC_REQUEST, null, true, null);
            }
        }
    }

    void ~MissionGameplay()
    {
        if (m_DZWelcomeZWarningSound) m_DZWelcomeZWarningSound.Event_OnSoundWaveEnded.Remove(DZWelcomeZWarningEnded);
        if (m_DZWelcomeZPanel) m_DZWelcomeZPanel.Shutdown();
        m_DZWelcomeZPanel = null;
    }
}

modded class MissionServer
{
    override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
    {
        super.InvokeOnConnect(player, identity);
        if (GetGame() && GetGame().IsServer() && player && identity)
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DZWelcomeZServer.HandleJoin, 5000, false, player, identity);
    }
}
