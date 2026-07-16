class DZKOTHF_MusicPlayer
{
	protected static ref DZKOTHF_MusicPlayer s_Instance;
	protected AbstractWave m_Wave;

	static void Play(string soundSet, float volume)
	{
		if (!s_Instance) s_Instance = new DZKOTHF_MusicPlayer;
		s_Instance.PlayInternal(soundSet, Math.Clamp(volume, 0.0, 0.45));
	}

	protected void PlayInternal(string soundSet, float volume)
	{
		StopInternal();
		if (!GetGame() || GetGame().IsServer() || soundSet == "" || volume <= 0.0) return;
		SoundParams soundParams = new SoundParams(soundSet);
		if (!soundParams.IsValid()) return;
		SoundObjectBuilder soundBuilder = new SoundObjectBuilder(soundParams);
		SoundObject soundObject = soundBuilder.BuildSoundObject();
		if (!soundObject) return;
		soundObject.SetKind(WaveKind.WAVEMUSIC);
		m_Wave = GetGame().GetSoundScene().Play2D(soundObject, soundBuilder);
		if (!m_Wave) return;
		m_Wave.Loop(false);
		m_Wave.SetVolumeRelative(volume);
		m_Wave.Play();
		Print("[DeutschZ KotHZ Free] Client music started: " + soundSet + " volume=" + volume.ToString() + ".");
	}

	static void Stop() { if (s_Instance) s_Instance.StopInternal(); }

	protected void StopInternal()
	{
		if (m_Wave) m_Wave.Stop();
		m_Wave = null;
	}
}
