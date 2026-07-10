modded class DynamicMusicPlayerRegistry
{
	protected override void RegisterTracksMenu()
	{
		m_TracksMenu = new array<ref DynamicMusicTrackData>();
		RegisterTrackMenu("DZKOTHG_MenuMusic_SoundSet");
		Print("[DZKOTHG][CLIENT] Menu music registered: DZKOTHG_MenuMusic_SoundSet");
	}
};
