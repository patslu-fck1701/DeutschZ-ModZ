class DZKOTH_ServerRPC
{
	protected static ref DZKOTHF_Settings s_Settings;
	protected static string s_ActiveMusicTrack;
	protected static int s_ActiveMusicTrackIndex = -1;
	protected static bool s_MusicPlaylistActive;
	protected static ref map<string, bool> s_MusicRecipients = new map<string, bool>;
	protected static bool s_MusicEnabled = true;
	protected static bool s_MusicLoopPlaylist = true;
	protected static bool s_MusicRandomize = true;
	protected static bool s_MusicPreventImmediateRepeat = true;
	protected static int s_MusicTrackSeconds;

	protected static DZKOTHF_Settings GetSettings()
	{
		if (!s_Settings)
			s_Settings = DZKOTHF_SettingsLoader.Load();
		return s_Settings;
	}

	static void ConfigureMusic(DZKOTH_MainConfig config)
	{
		if (!config)
			return;

		s_MusicEnabled = config.MusicEnabled;
		s_MusicLoopPlaylist = config.MusicLoopPlaylist;
		s_MusicRandomize = config.MusicRandomize;
		s_MusicPreventImmediateRepeat = config.MusicPreventImmediateRepeat;
		s_MusicTrackSeconds = Math.Clamp(config.MusicTrackSeconds, 0, 1800);
		DZKOTHF_Settings settings = GetSettings();
		if (settings)
			settings.MusicVolume = Math.Clamp(config.MusicVolume, 0.0, 0.60);
		if (!s_MusicEnabled)
		{
			StopMusicPlaylist();
			s_MusicRecipients.Clear();
		}
		DZKOTH_Utils.Log("Music configured enabled=" + s_MusicEnabled.ToString() + " volume=" + config.MusicVolume.ToString() + " radius=" + config.MusicRadius.ToString() + " loop=" + s_MusicLoopPlaylist.ToString() + " random=" + s_MusicRandomize.ToString());
	}

	static void SendMarker(PlayerBase player, DZKOTH_MarkerData marker)
	{
		if (!player || !marker || !GetGame())
			return;

		DZKOTHF_ClientBridge.SendMarker(player, GetSettings(), !marker.Remove, marker.Pos, marker.Text);
	}

	static void BroadcastMarker(DZKOTH_MarkerData marker)
	{
		if (!marker || !GetGame())
			return;

		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);

		foreach (Man man: players)
		{
			PlayerBase player = PlayerBase.Cast(man);
			if (player)
				SendMarker(player, marker);
		}
	}

	static void SendHud(PlayerBase player, int mode, string label, float current, float max)
	{
		if (!player || !GetGame())
			return;

		float progress;
		if (max > 0.0)
			progress = current / max;
		DZKOTHF_ClientBridge.SendProgress(player, GetSettings(), mode != DZKOTH_ProgressModes.HIDE, label, progress);
	}

	static void BroadcastHud(array<PlayerBase> players, int mode, string label, float current, float max)
	{
		if (!players)
			return;

		foreach (PlayerBase player: players)
		{
			if (player)
				SendHud(player, mode, label, current, max);
		}
	}

	static void SendWarning(PlayerBase player, string title, string text, float time = 7.0)
	{
		if (!player || !GetGame())
			return;

		DZKOTHF_ClientBridge.SendNotify(player, GetSettings(), title + ": " + text);
	}

	static void BroadcastWarning(string title, string text, float time = 7.0)
	{
		if (!GetGame())
			return;

		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);

		foreach (Man man: players)
		{
			PlayerBase player = PlayerBase.Cast(man);
			if (player)
				SendWarning(player, title, text, time);
		}
	}

	static void BroadcastWarningToPlayers(array<PlayerBase> players, string title, string text, float time = 7.0)
	{
		if (!players)
			return;

		foreach (PlayerBase player: players)
		{
			if (player)
				SendWarning(player, title, text, time);
		}
	}

	static void SendPlayerUIMessage(PlayerBase player, string title, string text, float time = 5.0)
	{
		if (!player || !GetGame())
			return;

		DZKOTHF_ClientBridge.SendNotify(player, GetSettings(), title + ": " + text);
	}

	static void BroadcastPlayerUIMessage(array<PlayerBase> players, string title, string text, float time = 5.0)
	{
		if (!players)
			return;

		foreach (PlayerBase player: players)
		{
			if (player)
				SendPlayerUIMessage(player, title, text, time);
		}
	}

	static void SendFX(PlayerBase player, int fx, vector pos)
	{
		if (!player || !GetGame())
			return;

		if (fx == DZKOTH_FXIds.EVENT_START)
		{
			StartMusic(player);
		}
		else if (fx == DZKOTH_FXIds.CLEAR)
		{
			StopMusic(player);
		}
	}

	static void StartMusic(PlayerBase player)
	{
		if (!s_MusicEnabled || !player || !player.GetIdentity() || !GetGame())
			return;

		string playerId = player.GetIdentity().GetPlainId();
		if (s_MusicRecipients.Contains(playerId))
			return;

		if (s_ActiveMusicTrack == "")
			SelectAndScheduleNextMusicTrack();
		if (s_ActiveMusicTrack == "")
		{
			DZKOTH_Utils.Warn("Music start skipped because no playlist track is available.");
			return;
		}

		DZKOTHF_ClientBridge.SendMusic(player, GetSettings(), true, s_ActiveMusicTrack);
		s_MusicRecipients.Set(playerId, true);
	}

	static void StopMusic(PlayerBase player)
	{
		if (!player || !player.GetIdentity() || !GetGame())
			return;

		string playerId = player.GetIdentity().GetPlainId();
		if (!s_MusicRecipients.Contains(playerId))
			return;

		DZKOTHF_ClientBridge.SendMusic(player, GetSettings(), false, "");
		s_MusicRecipients.Remove(playerId);
		if (s_MusicRecipients.Count() == 0)
			StopMusicPlaylist();
	}

	static void BroadcastFX(array<PlayerBase> players, int fx, vector pos)
	{
		if (!players)
			return;

		foreach (PlayerBase player: players)
		{
			if (player)
				SendFX(player, fx, pos);
		}
	}

	static void BroadcastFXGlobal(int fx, vector pos)
	{
		if (!GetGame())
			return;

		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);

		foreach (Man man: players)
		{
			PlayerBase player = PlayerBase.Cast(man);
			if (player)
				SendFX(player, fx, pos);
		}

		if (fx == DZKOTH_FXIds.CLEAR)
		{
			StopMusicPlaylist();
			s_MusicRecipients.Clear();
		}
	}

	static void BroadcastKeycardSignal(string playerName, vector pos)
	{
		if (!GetGame())
			return;

		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);

		foreach (Man man: players)
		{
			PlayerBase player = PlayerBase.Cast(man);
			if (!player || !player.GetIdentity())
				continue;

			DZKOTHF_ClientBridge.SendNotify(player, GetSettings(), "Streng geheimes BattlegroundZ-Dokument geborgen von " + playerName + ".");
		}
	}

	protected static void SelectAndScheduleNextMusicTrack()
	{
		if (!GetGame())
			return;
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(AdvanceMusicTrack);
		int nextIndex;
		if (s_MusicRandomize)
			nextIndex = Math.RandomInt(0, 6);
		else
			nextIndex = (s_ActiveMusicTrackIndex + 1) % 6;
		if (s_MusicPreventImmediateRepeat && nextIndex == s_ActiveMusicTrackIndex)
			nextIndex = (nextIndex + 1) % 6;
		s_ActiveMusicTrackIndex = nextIndex;
		s_ActiveMusicTrack = GetMusicTrackName(nextIndex);
		s_MusicPlaylistActive = true;
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(AdvanceMusicTrack, GetMusicTrackDurationMs(nextIndex), false);
		DZKOTH_Utils.Log("Music playlist track selected index=" + nextIndex.ToString() + " soundSet=" + s_ActiveMusicTrack);
	}

	protected static void AdvanceMusicTrack()
	{
		if (!s_MusicPlaylistActive || s_MusicRecipients.Count() == 0 || !GetGame())
		{
			StopMusicPlaylist();
			return;
		}
		if (!s_MusicLoopPlaylist)
		{
			StopMusicForAllRecipients();
			return;
		}
		SelectAndScheduleNextMusicTrack();
		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		foreach (Man man: players)
		{
			PlayerBase player = PlayerBase.Cast(man);
			if (!player || !player.GetIdentity())
				continue;
			if (s_MusicRecipients.Contains(player.GetIdentity().GetPlainId()))
				DZKOTHF_ClientBridge.SendMusic(player, GetSettings(), true, s_ActiveMusicTrack);
		}
	}

	protected static void StopMusicForAllRecipients()
	{
		if (!GetGame())
			return;
		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		foreach (Man man: players)
		{
			PlayerBase player = PlayerBase.Cast(man);
			if (player && player.GetIdentity() && s_MusicRecipients.Contains(player.GetIdentity().GetPlainId()))
				DZKOTHF_ClientBridge.SendMusic(player, GetSettings(), false, "");
		}
		s_MusicRecipients.Clear();
		StopMusicPlaylist();
	}

	protected static void StopMusicPlaylist()
	{
		s_MusicPlaylistActive = false;
		s_ActiveMusicTrack = "";
		s_ActiveMusicTrackIndex = -1;
		if (GetGame())
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(AdvanceMusicTrack);
	}

	protected static string GetMusicTrackName(int index)
	{
		return string.Format("DZKOTHF_Music%1_SoundSet", (index + 1).ToStringLen(2));
	}

	protected static int GetMusicTrackDurationMs(int index)
	{
		if (s_MusicTrackSeconds > 0)
			return s_MusicTrackSeconds * 1000;
		switch (index)
		{
			case 0: return 137880;
			case 1: return 217360;
			case 2: return 254360;
			case 3: return 235240;
			case 4: return 201120;
			case 5: return 319960;
			case 6: return 184840;
		}
		return 284880;
	}
}
