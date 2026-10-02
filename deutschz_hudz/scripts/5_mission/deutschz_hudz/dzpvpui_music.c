class DZPVPUI_MusicPlayer
{
    protected AbstractWave m_Wave;
    protected int m_TrackIndex;
    protected int m_PlaylistIndex;
    protected float m_Volume;
    protected float m_VolumeBeforeMute;
    protected bool m_Playing;
    protected bool m_ShouldPlay;
    protected ref array<string> m_SoundSets;
    protected ref array<string> m_Names;
    protected ref array<int> m_Playlists;

    void DZPVPUI_MusicPlayer()
    {
        m_SoundSets = new array<string>;
        m_Names = new array<string>;
        m_Playlists = new array<int>;
        ref array<string> deutschzNames = {"deutschz - koth laut timer", "deutschz bra - 2 minuten [afk]", "deutschz bra - chernarus allein", "deutschz bra - chernarus bei nacht", "deutschz chill - chernarus brennt 2", "deutschz chill - chernarus brennt", "deutschz chill - chernarus gear", "deutschz chill - ganz normal", "deutschz chill - nacht gehoert unz", "deutschz gang - black g waggon", "deutschz gang - deutschz", "deutschz gang - heli ueberm wald", "deutschz gang - kein respawn", "deutschz gang - maztaz&mazakkazz", "deutschz gang - nachtfahrt nach tisy", "deutschz gang - nachtschicht", "deutschz gang - roter rauch gute laune", "mc knochen - bestimmt desync", "mc knochen - convoy nacht", "mc knochen - deutsch z", "mc knochen - deutschz restart", "mc knochen - gruenes licht", "mc knochen - hundertprozentig desync", "mc knochen - ich bin der king", "mc knochen - kothz keiner schlaeft", "mc knochen - leben fuer die nacht", "mc knochen - nur kurz [afk]", "mc knochen - server sagt nein", "mc knochen deutschz leauft"};
        ref array<string> englishNames = {"Blackstreet - No Diggity feat Dr Dre & Queen Pen", "deutschz - black & gold", "deutschz - no safe zone", "deutschz - welcome to deutschz", "DJ ICEK - Clap Dat feat Nicki Minaj Cardi B Tyga Quavo", "DJ ICEK - Hit It From The Back feat Tyga Rubi Rose Lil Wayne", "Dr Dre - Bang Bang feat Hittman & KnocTurnal", "Dr Dre - Forgot About Dre feat Eminem", "Dr Dre - Still DRE", "G-Unit - Poppin Them Thangs", "Rapking - SIP feat 6IX9INE Tyga Nicki Minaj Blueface", "Rubi Rose - Crystal feat Wiz Khalifa & Flo Milli", "Rubi Rose - Pretty MF", "Tyga - SEXY I Best of Tyga", "Tyga & Nicki Minaj - Life Remix", "Tyga & Rubi Rose - Insane Remix"};
        ref array<string> halftanzNames = {"halftan - blutpfad nach walhal 2", "halftan - blutpfad nach walhal", "halftan - die chronicken von deutschz 2", "halftan - die chronicken von deutschz"};
        AddPlaylist(0, "deutschz", deutschzNames);
        AddPlaylist(1, "english", englishNames);
        AddPlaylist(2, "halftanz", halftanzNames);
        DZPVPUI_State.Preferences.Validate(DZPVPUI_State.Settings.MusicMaximumVolume);
        m_PlaylistIndex = DZPVPUI_State.Preferences.PlaylistIndex;
        m_TrackIndex = RandomTrackInPlaylist(m_PlaylistIndex, false);
        m_Volume = DZPVPUI_State.Preferences.MusicVolume;
        m_VolumeBeforeMute = m_Volume;
    }

    protected void AddPlaylist(int playlist, string prefix, array<string> names)
    {
        for (int i = 0; i < names.Count(); i++)
        {
            string number = i.ToString();
            if (i < 10)
                number = "0" + number;
            m_SoundSets.Insert("dzpvpui_" + prefix + "_" + number + "_set");
            m_Names.Insert(names.Get(i));
            m_Playlists.Insert(playlist);
        }
    }

    protected int FirstTrackInPlaylist(int playlist)
    {
        for (int i = 0; i < m_Playlists.Count(); i++)
            if (m_Playlists.Get(i) == playlist)
                return i;
        return 0;
    }

    protected int RandomTrackInPlaylist(int playlist, bool avoidCurrent)
    {
        ref array<int> candidates = new array<int>;
        for (int i = 0; i < m_Playlists.Count(); i++)
        {
            if (m_Playlists.Get(i) == playlist && (!avoidCurrent || i != m_TrackIndex))
                candidates.Insert(i);
        }
        if (candidates.Count() == 0)
            return FirstTrackInPlaylist(playlist);
        return candidates.Get(Math.RandomInt(0, candidates.Count()));
    }

    void SetPlaylist(int playlist)
    {
        m_PlaylistIndex = Math.Clamp(playlist, 0, 2);
        m_TrackIndex = RandomTrackInPlaylist(m_PlaylistIndex, false);
        DZPVPUI_State.Preferences.PlaylistIndex = m_PlaylistIndex;
        DZPVPUI_State.Preferences.TrackIndex = m_TrackIndex;
        Play();
    }

    int GetPlaylistIndex() { return m_PlaylistIndex; }
    float GetVolume() { return m_Volume; }
    string GetTrackName() { return m_Names.Get(m_TrackIndex); }
    float GetTrackLength()
    {
        if (!m_Wave || !m_Wave.IsHeaderLoaded())
            return 0.0;
        return m_Wave.GetLength();
    }
    float GetTrackProgress()
    {
        if (!m_Wave || !m_Wave.IsHeaderLoaded())
            return 0.0;
        return Math.Clamp(m_Wave.GetCurrPosition(), 0.0, 1.0);
    }
    string GetPlaylistName()
    {
        if (m_PlaylistIndex == 1) return "english";
        if (m_PlaylistIndex == 2) return "halftanz";
        return "deutschz";
    }

    void Destroy()
    {
        StopInternal(true);
        m_SoundSets = null;
        m_Names = null;
        m_Playlists = null;
    }

    void Play()
    {
        m_ShouldPlay = true;
        StartCurrentTrack();
    }

    protected void StartCurrentTrack()
    {
        StopInternal(false);
        SoundParams soundParams = new SoundParams(m_SoundSets.Get(m_TrackIndex));
        if (!soundParams.IsValid())
        {
            ErrorEx("[DZPVPUI] Ungueltiges SoundSet: " + m_SoundSets.Get(m_TrackIndex));
            return;
        }
        SoundObjectBuilder builder = new SoundObjectBuilder(soundParams);
        SoundObject soundObject = builder.BuildSoundObject();
        if (!soundObject)
        {
            ErrorEx("[DZPVPUI][MUSIC] SoundObject konnte nicht gebaut werden: " + m_SoundSets.Get(m_TrackIndex));
            return;
        }
        soundObject.SetKind(WaveKind.WAVEUI);
        m_Wave = GetGame().GetSoundScene().Play2D(soundObject, builder);
        if (!m_Wave)
        {
            ErrorEx("[DZPVPUI][MUSIC] Play2D lieferte keine Wave: " + m_SoundSets.Get(m_TrackIndex));
            return;
        }
        m_Wave.Loop(false);
        m_Wave.GetEvents().Event_OnSoundWaveEnded.Insert(OnEnded);
        m_Wave.GetEvents().Event_OnSoundWaveStopped.Insert(OnStopped);
        m_Wave.SetVolume(m_Volume);
        m_Wave.Play();
        m_Playing = true;
        DZPVPUI_State.Preferences.TrackIndex = m_TrackIndex;
        Print("[DZPVPUI][MUSIC] Wiedergabe gestartet: " + m_SoundSets.Get(m_TrackIndex));
    }

    void TogglePlayPause()
    {
        if (m_ShouldPlay)
            Pause();
        else
            Play();
    }

    void Pause()
    {
        StopInternal(true);
    }

    void Previous()
    {
        int candidate = m_TrackIndex - 1;
        if (candidate < 0 || m_Playlists.Get(candidate) != m_PlaylistIndex)
        {
            candidate = m_Playlists.Count() - 1;
            while (candidate > 0 && m_Playlists.Get(candidate) != m_PlaylistIndex)
                candidate--;
        }
        m_TrackIndex = candidate;
        Play();
    }

    void ToggleMute()
    {
        if (m_Volume > 0.001)
        {
            m_VolumeBeforeMute = m_Volume;
            m_Volume = 0.0;
        }
        else
        {
            m_Volume = Math.Clamp(m_VolumeBeforeMute, 0.0, DZPVPUI_State.Settings.MusicMaximumVolume);
        }
        DZPVPUI_State.Preferences.MusicVolume = m_Volume;
        if (m_Wave)
            m_Wave.SetVolume(m_Volume);
    }

    void Next()
    {
        m_TrackIndex = RandomTrackInPlaylist(m_PlaylistIndex, true);
        Play();
    }

    bool WantsPlayback()
    {
        return m_ShouldPlay;
    }

    void RecoverAfterMenu()
    {
        if (m_ShouldPlay && !m_Wave)
            StartCurrentTrack();
    }

    void ChangeVolume(float delta)
    {
        m_Volume = Math.Clamp(m_Volume + delta, 0.0, DZPVPUI_State.Settings.MusicMaximumVolume);
        DZPVPUI_State.Preferences.MusicVolume = m_Volume;
        if (m_Wave)
            m_Wave.SetVolume(m_Volume);
    }

    string GetStatus()
    {
        int percent = Math.Round(m_Volume * 100.0);
        string state = "PAUSE";
        if (m_ShouldPlay)
            state = "PLAY";
        return GetPlaylistName() + " // " + m_Names.Get(m_TrackIndex) + " // " + state + " // " + percent.ToString() + "%";
    }

    protected void StopInternal(bool clearIntent)
    {
        if (clearIntent)
            m_ShouldPlay = false;
        if (!m_Wave)
        {
            m_Playing = false;
            return;
        }
        m_Wave.GetEvents().Event_OnSoundWaveEnded.Remove(OnEnded);
        m_Wave.GetEvents().Event_OnSoundWaveStopped.Remove(OnStopped);
        m_Wave.Stop();
        m_Wave = null;
        m_Playing = false;
    }

    protected void OnEnded()
    {
        if (m_Wave)
            m_Wave.GetEvents().Event_OnSoundWaveStopped.Remove(OnStopped);
        m_Wave = null;
        m_Playing = false;
        if (m_ShouldPlay)
            Next();
    }

    protected void OnStopped()
    {
        m_Wave = null;
        m_Playing = false;
    }
}

class DZPVPUI_MenuMusicSession
{
    protected static ref DZPVPUI_MusicPlayer s_Player;

    static DZPVPUI_MusicPlayer GetPlayer()
    {
        if (!s_Player)
            s_Player = new DZPVPUI_MusicPlayer();
        return s_Player;
    }

    static void Destroy()
    {
        if (s_Player)
            s_Player.Destroy();
        s_Player = null;
    }
}
