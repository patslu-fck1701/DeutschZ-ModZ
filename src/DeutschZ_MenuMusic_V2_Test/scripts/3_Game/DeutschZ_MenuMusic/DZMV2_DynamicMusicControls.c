modded class DynamicMusicPlayer
{
	protected ref array<ref DynamicMusicTrackData> m_DZMV2_History = new array<ref DynamicMusicTrackData>();
	protected DynamicMusicTrackData m_DZMV2_PausedTrack;
	protected float m_DZMV2_PausedOffset;
	protected bool m_DZMV2_Paused;
	protected bool m_DZMV2_Muted;

	protected override void OnNextTrackSelected(DynamicMusicTrackData track, float waitTime)
	{
		DZMV2_RememberCurrent(track);
		super.OnNextTrackSelected(track, waitTime);
	}

	override void OnUpdate(float timeslice)
	{
		if (m_DZMV2_Paused)
			return;

		super.OnUpdate(timeslice);
	}

	protected void DZMV2_RememberCurrent(DynamicMusicTrackData nextTrack = null)
	{
		DynamicMusicTrackData current = m_CurrentTrack;
		if (!current || current == nextTrack)
			return;

		if (m_DZMV2_History.Count() == 0 || m_DZMV2_History.Get(m_DZMV2_History.Count() - 1) != current)
			m_DZMV2_History.Insert(current);

		while (m_DZMV2_History.Count() > 12)
			m_DZMV2_History.Remove(0);
	}

	protected void DZMV2_StopCurrent(bool remember = false)
	{
		if (remember)
			DZMV2_RememberCurrent();

		if (m_SoundPlaying)
		{
			m_SoundPlaying.GetEvents().Event_OnSoundWaveEnded.Remove(OnTrackEnded);
			m_SoundPlaying.GetEvents().Event_OnSoundWaveStopped.Remove(OnTrackStopped);
			m_SoundPlaying.Stop();
		}

		m_SoundPlaying = null;
		m_CurrentTrack = null;
	}

	protected bool DZMV2_StartTrack(DynamicMusicTrackData track, float offsetSeconds = 0.0)
	{
		if (!track || track.m_SoundSet == "")
			return false;

		SoundParams soundParams = new SoundParams(track.m_SoundSet);
		if (!soundParams.IsValid())
		{
			Print("[DZMV2][CLIENT][ERROR] Invalid menu sound set: " + track.m_SoundSet);
			return false;
		}

		SoundObjectBuilder soundBuilder = new SoundObjectBuilder(soundParams);
		SoundObject soundObject = soundBuilder.BuildSoundObject();
		soundObject.SetKind(WaveKind.WAVEMUSIC);

		m_SoundPlaying = g_Game.GetSoundScene().Play2D(soundObject, soundBuilder);
		if (!m_SoundPlaying)
			return false;

		m_CurrentTrack = track;
		m_SoundPlaying.Loop(false);
		m_SoundPlaying.GetEvents().Event_OnSoundWaveEnded.Insert(OnTrackEnded);
		m_SoundPlaying.GetEvents().Event_OnSoundWaveStopped.Insert(OnTrackStopped);

		if (offsetSeconds > 0.0)
			m_SoundPlaying.PlayWithOffset(offsetSeconds);
		else
			m_SoundPlaying.Play();

		if (m_DZMV2_Muted)
			m_SoundPlaying.SetVolumeRelative(0.0);

		return true;
	}

	bool DZMV2_TogglePause()
	{
		if (m_DZMV2_Paused)
		{
			DynamicMusicTrackData pausedTrack = m_DZMV2_PausedTrack;
			float pausedOffset = m_DZMV2_PausedOffset;
			m_DZMV2_Paused = false;
			m_DZMV2_PausedTrack = null;
			m_DZMV2_PausedOffset = 0.0;
			return DZMV2_StartTrack(pausedTrack, pausedOffset);
		}

		if (!m_SoundPlaying || !m_CurrentTrack)
		{
			SetCategory(EDynamicMusicPlayerCategory.MENU, false);
			return false;
		}

		m_DZMV2_PausedTrack = m_CurrentTrack;
		float normalizedPosition = Math.Clamp(m_SoundPlaying.GetCurrPosition(), 0.0, 1.0);
		m_DZMV2_PausedOffset = normalizedPosition * m_SoundPlaying.GetLength();
		DZMV2_StopCurrent(false);
		m_DZMV2_Paused = true;
		return true;
	}

	bool DZMV2_PlayNext()
	{
		if (!m_DynamicMusicPlayerRegistry || !m_DynamicMusicPlayerRegistry.m_TracksMenu || m_DynamicMusicPlayerRegistry.m_TracksMenu.Count() == 0)
			return false;

		DynamicMusicTrackData current = m_CurrentTrack;
		if (m_DZMV2_Paused)
			current = m_DZMV2_PausedTrack;

		if (current)
			DZMV2_RememberCurrent();

		int count = m_DynamicMusicPlayerRegistry.m_TracksMenu.Count();
		int currentIndex = m_DynamicMusicPlayerRegistry.m_TracksMenu.Find(current);
		int nextIndex = currentIndex + 1;
		if (nextIndex < 0 || nextIndex >= count)
			nextIndex = 0;

		m_DZMV2_Paused = false;
		m_DZMV2_PausedTrack = null;
		DZMV2_StopCurrent(false);
		return DZMV2_StartTrack(m_DynamicMusicPlayerRegistry.m_TracksMenu.Get(nextIndex));
	}

	bool DZMV2_PlayPrevious()
	{
		DynamicMusicTrackData previous;
		if (m_DZMV2_History.Count() > 0)
		{
			int previousIndex = m_DZMV2_History.Count() - 1;
			previous = m_DZMV2_History.Get(previousIndex);
			m_DZMV2_History.Remove(previousIndex);
		}
		else if (m_DynamicMusicPlayerRegistry && m_DynamicMusicPlayerRegistry.m_TracksMenu && m_DynamicMusicPlayerRegistry.m_TracksMenu.Count() > 0)
		{
			DynamicMusicTrackData current = m_CurrentTrack;
			if (m_DZMV2_Paused)
				current = m_DZMV2_PausedTrack;

			int currentIndex = m_DynamicMusicPlayerRegistry.m_TracksMenu.Find(current);
			int previousIndexFallback = currentIndex - 1;
			if (previousIndexFallback < 0)
				previousIndexFallback = m_DynamicMusicPlayerRegistry.m_TracksMenu.Count() - 1;
			previous = m_DynamicMusicPlayerRegistry.m_TracksMenu.Get(previousIndexFallback);
		}

		if (!previous)
			return false;

		m_DZMV2_Paused = false;
		m_DZMV2_PausedTrack = null;
		DZMV2_StopCurrent(false);
		return DZMV2_StartTrack(previous);
	}

	bool DZMV2_ToggleMute()
	{
		m_DZMV2_Muted = !m_DZMV2_Muted;
		if (m_SoundPlaying)
		{
			if (m_DZMV2_Muted)
				m_SoundPlaying.SetVolumeRelative(0.0);
			else
				m_SoundPlaying.SetVolumeRelative(1.0);
		}

		return m_DZMV2_Muted;
	}

	bool DZMV2_IsPaused()
	{
		return m_DZMV2_Paused;
	}

	bool DZMV2_IsMuted()
	{
		return m_DZMV2_Muted;
	}

	string DZMV2_GetCurrentTrackName()
	{
		DynamicMusicTrackData track = m_CurrentTrack;
		if (m_DZMV2_Paused)
			track = m_DZMV2_PausedTrack;

		if (!track)
			return "DeutschZ Radio";

		string soundSet = track.m_SoundSet;
		soundSet.Replace("DZKOTHG_MenuMusic", "TRACK ");
		soundSet.Replace("_SoundSet", "");
		return soundSet;
	}
}
