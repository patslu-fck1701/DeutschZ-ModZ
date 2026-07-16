modded class DynamicMusicPlayerRegistry
{
	protected override void RegisterCategorySettings()
	{
		super.RegisterCategorySettings();
		DynamicMusicPlayerSettings settings = new DynamicMusicPlayerSettings();
		settings.m_MinWaitTimeSeconds = 4.0;
		settings.m_MaxWaitTimeSeconds = 10.0;
		settings.m_PreviousTrackFadeoutSeconds = 3.5;
		m_SettingsByCategory[EDynamicMusicPlayerCategory.MENU] = settings;
	}

	protected override void RegisterTracksMenu()
	{
		m_TracksMenu = new array<ref DynamicMusicTrackData>();
		RegisterTrackMenu("DZKOTHG_MenuMusic01_SoundSet");
		RegisterTrackMenu("DZKOTHG_MenuMusic02_SoundSet");
		RegisterTrackMenu("DZKOTHG_MenuMusic03_SoundSet");
		RegisterTrackMenu("DZKOTHG_MenuMusic04_SoundSet");
		RegisterTrackMenu("DZKOTHG_MenuMusic05_SoundSet");
		RegisterTrackMenu("DZKOTHG_MenuMusic06_SoundSet");
		RegisterTrackMenu("DZKOTHG_MenuMusic07_SoundSet");
		RegisterTrackMenu("DZKOTHG_MenuMusic08_SoundSet");
		RegisterTrackMenu("DZKOTHG_MenuMusic09_SoundSet");
		RegisterTrackMenu("DZKOTHG_MenuMusic10_SoundSet");
		RegisterTrackMenu("DZKOTHG_MenuMusic11_SoundSet");
		Print("[DZKOTHG][CLIENT] Registered 11 menu tracks at max 66 percent with random start and history-based repeat protection.");
	}
};
