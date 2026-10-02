class DZRMZ_PlayerVoiceState
{
	int SchemaVersion;
	string Trapped;
	string Injured;
	string Besieged;
	string Alone;
	string Koth;
	string Convoy;
	string Decoder;
	string Signal;

	void DZRMZ_PlayerVoiceState() { SchemaVersion = 1; }

	string GetVariant(string audioId)
	{
		if (audioId == "HELP_TRAPPED") return Trapped;
		if (audioId == "HELP_INJURED") return Injured;
		if (audioId == "HELP_BESIEGED") return Besieged;
		if (audioId == "HELP_ALONE") return Alone;
		if (audioId == "STORY_KOTH") return Koth;
		if (audioId == "STORY_CONVOY") return Convoy;
		if (audioId == "STORY_DECODER") return Decoder;
		if (audioId == "STORY_SIGNAL") return Signal;
		return "";
	}

	void SetVariant(string audioId, string variant)
	{
		if (audioId == "HELP_TRAPPED") Trapped = variant;
		else if (audioId == "HELP_INJURED") Injured = variant;
		else if (audioId == "HELP_BESIEGED") Besieged = variant;
		else if (audioId == "HELP_ALONE") Alone = variant;
		else if (audioId == "STORY_KOTH") Koth = variant;
		else if (audioId == "STORY_CONVOY") Convoy = variant;
		else if (audioId == "STORY_DECODER") Decoder = variant;
		else if (audioId == "STORY_SIGNAL") Signal = variant;
	}
}
