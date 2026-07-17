class DZKOTHG_LoadingEntry
{
	string Category;
	string Text;
}

class DZKOTHG_NewsEntry
{
	string Title;
	string Body;
}

class DZKOTHG_MenuContentConfig
{
	float SlideshowSeconds;
	string LogoPath;
	string HintIconPath;
	ref array<string> Screens;
	ref array<ref DZKOTHG_LoadingEntry> LoadingEntries;
	ref array<string> ServerMessages;
	ref array<ref DZKOTHG_NewsEntry> MenuNews;

	void DZKOTHG_MenuContentConfig()
	{
		SlideshowSeconds = 8.0;
		LogoPath = "DeutschZ_MenuMusic_V2_Test/gui/menu_assets/logos/Logo_Dark.paa";
		HintIconPath = "DeutschZ_MenuMusic_V2_Test/gui/menu_assets/icons/icon_status_ca.paa";
		Screens = new array<string>;
		LoadingEntries = new array<ref DZKOTHG_LoadingEntry>;
		ServerMessages = new array<string>;
		MenuNews = new array<ref DZKOTHG_NewsEntry>;
	}
}

class DZKOTHG_MenuContentStore
{
	static const string CONFIG_PATH = "DeutschZ_MenuMusic_V2_Test/data/ui/loading/loading_content.json";
	protected static ref DZKOTHG_MenuContentConfig s_Config;
	protected static int s_LastLoadingIndex = -1;
	protected static int s_ServerMessageIndex = -1;
	protected static int s_NewsIndex = -1;

	static DZKOTHG_MenuContentConfig GetConfig()
	{
		Ensure();
		return s_Config;
	}

	static void Ensure()
	{
		if (s_Config)
			return;

		s_Config = new DZKOTHG_MenuContentConfig;
		string errorMessage;
		if (!JsonFileLoader<ref DZKOTHG_MenuContentConfig>.LoadFile(CONFIG_PATH, s_Config, errorMessage))
			Print("[DZKOTHG][CLIENT][WARN] Menu content JSON failed: " + errorMessage);
	}

	static float GetSlideshowSeconds()
	{
		Ensure();
		if (s_Config.SlideshowSeconds < 6.0)
			return 6.0;
		if (s_Config.SlideshowSeconds > 12.0)
			return 12.0;
		return s_Config.SlideshowSeconds;
	}

	static string GetNextScreen(string previous = "")
	{
		Ensure();
		if (!s_Config.Screens || s_Config.Screens.Count() == 0)
			return "DeutschZ_MenuMusic_V2_Test/data/ui/loading/loading_0.paa";

		int index = Math.RandomIntInclusive(0, s_Config.Screens.Count() - 1);
		if (s_Config.Screens.Count() > 1)
		{
			for (int attempt = 0; attempt < 12 && s_Config.Screens.Get(index) == previous; attempt++)
				index = Math.RandomIntInclusive(0, s_Config.Screens.Count() - 1);
		}

		return s_Config.Screens.Get(index);
	}

	static DZKOTHG_LoadingEntry GetNextLoadingEntry()
	{
		Ensure();
		if (!s_Config.LoadingEntries || s_Config.LoadingEntries.Count() == 0)
			return null;

		int index = Math.RandomIntInclusive(0, s_Config.LoadingEntries.Count() - 1);
		if (s_Config.LoadingEntries.Count() > 1)
		{
			while (index == s_LastLoadingIndex)
				index = Math.RandomIntInclusive(0, s_Config.LoadingEntries.Count() - 1);
		}

		s_LastLoadingIndex = index;
		return s_Config.LoadingEntries.Get(index);
	}

	static string GetNextServerMessage()
	{
		Ensure();
		if (!s_Config.ServerMessages || s_Config.ServerMessages.Count() == 0)
			return "DeutschZ Live Server";

		s_ServerMessageIndex++;
		if (s_ServerMessageIndex >= s_Config.ServerMessages.Count())
			s_ServerMessageIndex = 0;
		return s_Config.ServerMessages.Get(s_ServerMessageIndex);
	}

	static DZKOTHG_NewsEntry GetNextNews()
	{
		Ensure();
		if (!s_Config.MenuNews || s_Config.MenuNews.Count() == 0)
			return null;

		s_NewsIndex++;
		if (s_NewsIndex >= s_Config.MenuNews.Count())
			s_NewsIndex = 0;
		return s_Config.MenuNews.Get(s_NewsIndex);
	}
}
