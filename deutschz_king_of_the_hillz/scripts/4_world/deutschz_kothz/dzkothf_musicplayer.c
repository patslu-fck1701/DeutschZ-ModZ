class DZKOTHF_MusicPlayer
{
	protected static ref DZKOTHF_MusicPlayer s_Instance;
	protected AbstractWave m_Wave;
	protected string m_CurrentSoundSet;

	static void Play(string soundSet, float volume)
	{
		if (!s_Instance) s_Instance = new DZKOTHF_MusicPlayer;
		s_Instance.PlayInternal(soundSet, Math.Clamp(volume, 0.0, 0.60));
	}

	protected void PlayInternal(string soundSet, float volume)
	{
		if (!GetGame() || GetGame().IsServer() || soundSet == "" || volume <= 0.0) return;
		if (m_Wave && m_CurrentSoundSet == soundSet)
			return;
		StopInternal();
		SoundParams soundParams = new SoundParams(soundSet);
		if (!soundParams.IsValid())
		{
			Print("[DeutschZ KotHZ] ERROR: invalid client music SoundSet: " + soundSet);
			return;
		}
		SoundObjectBuilder soundBuilder = new SoundObjectBuilder(soundParams);
		SoundObject soundObject = soundBuilder.BuildSoundObject();
		if (!soundObject)
		{
			Print("[DeutschZ KotHZ] ERROR: client music SoundObject build failed: " + soundSet);
			return;
		}
		// Event music must not depend on DayZ's global music channel. HUD/menu mods
		// and many players set that channel to zero. Server-side proximity still
		// limits recipients to the configured music radius (default 100 m).
		soundObject.SetKind(WaveKind.WAVEENVIRONMENT);
		m_Wave = GetGame().GetSoundScene().Play2D(soundObject, soundBuilder);
		if (!m_Wave)
		{
			Print("[DeutschZ KotHZ] ERROR: client music Play2D failed: " + soundSet);
			return;
		}
		m_Wave.Loop(true);
		m_Wave.SetVolume(volume);
		m_Wave.Play();
		m_CurrentSoundSet = soundSet;
		Print("[DeutschZ KotHZ] Client event music started: " + soundSet + " volume=" + volume.ToString() + ".");
	}

	static void Stop() { if (s_Instance) s_Instance.StopInternal(); }

	protected void StopInternal()
	{
		if (m_Wave) m_Wave.Stop();
		m_Wave = null;
		m_CurrentSoundSet = "";
	}
}
