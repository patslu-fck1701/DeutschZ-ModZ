class DZRMZ_AudioService
{
	protected static ref map<string, int> s_LastPlayback = new map<string, int>;

	static string HelpId(string missionId)
	{
		if (missionId == "SIDE-EINGESCHLOSSEN-01") return "HELP_TRAPPED";
		if (missionId == "SIDE-VERLETZT-01") return "HELP_INJURED";
		if (missionId == "SIDE-BELAGERT-01") return "HELP_BESIEGED";
		if (missionId == "SIDE-ALLEIN-01") return "HELP_ALONE";
		return "";
	}

	static string StoryId(string stage)
	{
		if (stage == "01_koth_v2") return "STORY_KOTH";
		if (stage == "02_convoy") return "STORY_CONVOY";
		if (stage == "03_combination") return "STORY_DECODER";
		if (stage == "04_signal") return "STORY_SIGNAL";
		return "";
	}

	static string SoundSet(string audioId, string variant)
	{
		string suffix = "_Male_SoundSet";
		if (variant == "FEMALE") suffix = "_Female_SoundSet";
		if (audioId == "HELP_TRAPPED") return "DZRMZ_HelpTrapped" + suffix;
		if (audioId == "HELP_INJURED") return "DZRMZ_HelpInjured" + suffix;
		if (audioId == "HELP_BESIEGED") return "DZRMZ_HelpBesieged" + suffix;
		if (audioId == "HELP_ALONE") return "DZRMZ_HelpAlone" + suffix;
		if (audioId == "STORY_KOTH") return "DZRMZ_StoryKoth" + suffix;
		if (audioId == "STORY_CONVOY") return "DZRMZ_StoryConvoy" + suffix;
		if (audioId == "STORY_DECODER") return "DZRMZ_StoryDecoder" + suffix;
		if (audioId == "STORY_SIGNAL") return "DZRMZ_StorySignal" + suffix;
		return "";
	}

	static bool Play(PlayerBase player, DZRMZ_Settings settings, string audioId)
	{
		if (!GetGame() || !GetGame().IsServer() || !player || !player.GetIdentity() || !settings || audioId == "") return false;
		if (!DZRMZ_RadioService.HasQualifiedRadio(player, settings)) return false;
		string uid = player.GetIdentity().GetPlainId();
		string key = uid + "|" + audioId;
		int now = GetGame().GetTime();
		if (s_LastPlayback.Contains(key) && now - s_LastPlayback.Get(key) < 120000) return false;
		string variant = DZRMZ_Storage.GetOrChooseVoice(uid, audioId);
		if (variant == "") return false;
		string soundSet = SoundSet(audioId, variant);
		if (soundSet == "") return false;
		player.RPCSingleParam(DZRMZ_RPC_PLAY_TRANSMISSION, new Param1<string>(soundSet), true, player.GetIdentity());
		s_LastPlayback.Set(key, now);
		DZRMZ_Log.Info("[DZRMZ AUDIO] AudioId=" + audioId + " Variant=" + variant + " gezielt versendet.");
		return true;
	}
}
