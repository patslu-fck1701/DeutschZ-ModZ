class DZEV_ClientProgressHud
{
	protected static const float MUSIC_FULL_VOLUME_RADIUS = 25.0;
	protected static const float MUSIC_MAX_DISTANCE = 100.0;
	protected static const float MUSIC_MAX_VOLUME = 0.45;
	protected static const int MUSIC_DISTANCE_TICK_MS = 500;

	private static ref DZEV_ClientProgressHud s_Instance;

	protected Widget m_Root;
	protected TextWidget m_Title;
	protected TextWidget m_Value;
	protected ProgressBarWidget m_Progress;
	protected Widget m_RedFlash;
	protected EffectSound m_Music;
	protected EffectSound m_Siren;
	protected vector m_MusicPosition;
	protected bool m_MusicRequested;

	static DZEV_ClientProgressHud Get()
	{
		if (!s_Instance)
			s_Instance = new DZEV_ClientProgressHud;

		return s_Instance;
	}

	void ShowProgress(int mode, string label, float current, float max)
	{
		if (!GetGame() || GetGame().IsDedicatedServer())
			return;

		if (mode == DZEV_ProgressModes.HIDE)
		{
			Hide();
			return;
		}

		EnsureWidgets();
		if (!m_Root)
			return;

		if (max <= 0)
			max = 1;

		float percent = Math.Clamp((current / max) * 100.0, 0.0, 100.0);
		string valueText;

		if (mode == DZEV_ProgressModes.BOSS)
			valueText = Math.Round(current).ToString() + " / " + Math.Round(max).ToString() + " HP";
		else
			valueText = Math.Round(percent).ToString() + " %";

		if (m_Title)
			m_Title.SetText(label);
		if (m_Value)
			m_Value.SetText(valueText);
		if (m_Progress)
			m_Progress.SetCurrent(percent);
		m_Root.Show(true);
	}

	void Hide()
	{
		if (m_Title)
			m_Title.SetText("");
		if (m_Value)
			m_Value.SetText("");
		if (m_Root)
			m_Root.Show(false);
	}

	void PlayMusic(vector pos)
	{
		if (!GetGame())
			return;

		StopMusic();
		m_MusicPosition = pos;
		m_MusicRequested = true;
		UpdateMusicAttenuation();
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(UpdateMusicAttenuation, MUSIC_DISTANCE_TICK_MS, true);
	}

	void PlayEventStart(vector pos)
	{
		PlayMusic(pos);
		PlayStartSiren(pos);
	}

	void PlayStartSiren(vector pos)
	{
		if (!GetGame())
			return;

		StopSiren();
		m_Siren = SEffectManager.PlaySound("UndergroundDoor_Alarm_Loop_SoundSet", pos, 0, 0, true);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(StopSiren, 7000, false);
	}

	void PlayAncientScream(vector pos)
	{
		if (!GetGame())
			return;

		StopSiren();
		m_Siren = SEffectManager.PlaySound("ZmbM_Mummy_DisturbedIdle_Soundset", pos, 0, 0, false);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(StopSiren, 7000, false);
	}

	void PlayBossSpawnEffect(vector pos)
	{
		PlayBossAlarm(pos);
	}

	void StopMusic()
	{
		m_MusicRequested = false;
		m_MusicPosition = vector.Zero;
		if (GetGame())
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(UpdateMusicAttenuation);

		DestroyMusicEffect();
	}

	protected void UpdateMusicAttenuation()
	{
		if (!m_MusicRequested || !GetGame())
			return;

		Man localPlayer = GetGame().GetPlayer();
		if (!localPlayer)
		{
			DestroyMusicEffect();
			return;
		}

		float distance = vector.Distance(localPlayer.GetPosition(), m_MusicPosition);
		if (distance > MUSIC_MAX_DISTANCE)
		{
			DestroyMusicEffect();
			return;
		}

		if (!m_Music)
			m_Music = SEffectManager.PlaySound("DZKOTH_EventMusic_SoundSet", m_MusicPosition, 0, 0, true);

		if (!m_Music)
			return;

		float relativeVolume = 1.0;
		if (distance > MUSIC_FULL_VOLUME_RADIUS)
			relativeVolume = 1.0 - ((distance - MUSIC_FULL_VOLUME_RADIUS) / (MUSIC_MAX_DISTANCE - MUSIC_FULL_VOLUME_RADIUS));

		m_Music.SetSoundVolume(Math.Clamp(relativeVolume, 0.0, 1.0) * MUSIC_MAX_VOLUME);
	}

	protected void DestroyMusicEffect()
	{
		if (m_Music)
			SEffectManager.DestroySound(m_Music);

		m_Music = null;
	}

	void PlayBossAlarm(vector pos)
	{
		if (!GetGame())
			return;

		StopSiren();
		m_Siren = SEffectManager.PlaySound("UndergroundDoor_Alarm_Loop_SoundSet", pos, 0, 0, true);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(StopSiren, 9500, false);
	}

	void StopSiren()
	{
		if (m_Siren)
			SEffectManager.DestroySound(m_Siren);

		m_Siren = null;
	}

	void PlayFireworks(vector pos)
	{
		if (!ParticleManager.GetInstance())
			return;

		vector basePos = pos + "0 38 0";
		ParticleManager.GetInstance().PlayInWorld(ParticleList.FIREWORKS_EXPLOSION_RED, basePos + "0 0 0");
		ParticleManager.GetInstance().PlayInWorld(ParticleList.FIREWORKS_EXPLOSION_GREEN, basePos + "6 5 3");
		ParticleManager.GetInstance().PlayInWorld(ParticleList.FIREWORKS_EXPLOSION_BLUE, basePos + "-5 7 -2");
		ParticleManager.GetInstance().PlayInWorld(ParticleList.FIREWORKS_EXPLOSION_YELLOW, basePos + "2 10 -6");
	}

	protected void FlashRed()
	{
		if (!GetGame())
			return;

		EnsureWidgets();
		if (!m_RedFlash)
			return;

		m_RedFlash.Show(true);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(HideFlash, 650, false);
	}

	protected void HideFlash()
	{
		if (m_RedFlash)
			m_RedFlash.Show(false);
	}

	protected void EnsureWidgets()
	{
		if (m_Root || !GetGame() || GetGame().IsDedicatedServer() || !GetGame().GetWorkspace())
			return;

		m_Root = GetGame().GetWorkspace().CreateWidgets("DeutschZ_KotH_InfectedSiedge_v2/gui/layouts/dzevent/dzev_progress.layout");
		if (!m_Root)
			return;

		m_Title = TextWidget.Cast(m_Root.FindAnyWidget("DZEV_Title"));
		m_Value = TextWidget.Cast(m_Root.FindAnyWidget("DZEV_Value"));
		m_Progress = ProgressBarWidget.Cast(m_Root.FindAnyWidget("DZEV_ProgressBar"));
		m_RedFlash = m_Root.FindAnyWidget("DZEV_RedFlash");
		m_Root.Show(false);
	}
}
