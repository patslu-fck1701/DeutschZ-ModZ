modded class MissionMainMenu
{
    override void OnMissionStart()
    {
        super.OnMissionStart();
        DZPVPUI_StopAutomaticMenuMusic();
        GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(DZPVPUI_StopAutomaticMenuMusic, 250, false);
        GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(DZPVPUI_StopAutomaticMenuMusic, 1000, false);
    }

    override void OnMissionFinish()
    {
        DZPVPUI_MenuMusicSession.Destroy();
        super.OnMissionFinish();
    }

    protected void DZPVPUI_StopAutomaticMenuMusic()
    {
        DynamicMusicPlayer player = GetDynamicMusicPlayer();
        if (!player)
            return;

        DynamicMusicPlayerCategoryPlaybackData playback = new DynamicMusicPlayerCategoryPlaybackData();
        playback.m_Category = EDynamicMusicPlayerCategory.NONE;
        playback.m_FadeOut = true;
        player.SetCategory(playback);
    }
};
