class DZBBC_Config
{
	static void EnsureProfileDirectory()
	{
		DZBBC_ProfilePaths.Ensure();
	}

	static ref DZBBC_ConfigBundle LoadAll()
	{
		EnsureProfileDirectory();
		ref DZBBC_ConfigBundle bundle = new DZBBC_ConfigBundle;
		bundle.Main = LoadMain();
		bundle.Locations = LoadLocations();
		bundle.AIWaves = LoadAIWaves();
		bundle.Loot = LoadLoot();
		bundle.Terminals = LoadTerminals();
		bundle.Messages = LoadMessages();
		return bundle;
	}

	static ref DZBBC_MainConfig LoadMain()
	{
		ref DZBBC_MainConfig config = new DZBBC_MainConfig;
		string errorMessage;
		if (FileExist(DZBBC_CONFIG_JSON))
		{
			if (!JsonFileLoader<ref DZBBC_MainConfig>.LoadFile(DZBBC_CONFIG_JSON, config, errorMessage))
				DZBBC_Utils.Warn("Main config load failed, defaults active: " + errorMessage);
		}
		else
		{
			JsonFileLoader<ref DZBBC_MainConfig>.SaveFile(DZBBC_CONFIG_JSON, config, errorMessage);
		}
		if (config.TestModeTeleportRetryCount <= 0)
			config.TestModeTeleportRetryCount = 4;
		if (config.TestModeTeleportRetrySeconds <= 0)
			config.TestModeTeleportRetrySeconds = 4;
		return config;
	}

	static ref DZBBC_LocationsConfig LoadLocations()
	{
		ref DZBBC_LocationsConfig config = new DZBBC_LocationsConfig;
		string errorMessage;
		if (FileExist(DZBBC_LOCATIONS_JSON))
		{
			if (!JsonFileLoader<ref DZBBC_LocationsConfig>.LoadFile(DZBBC_LOCATIONS_JSON, config, errorMessage))
				DZBBC_Utils.Warn("Locations load failed, defaults active: " + errorMessage);
		}
		else
		{
			JsonFileLoader<ref DZBBC_LocationsConfig>.SaveFile(DZBBC_LOCATIONS_JSON, config, errorMessage);
		}
		if (config)
		{
			bool migrated = config.EnsureRequiredLocations();
			foreach (DZBBC_CrashSiteConfig site: config.CrashSites)
			{
				if (site)
					migrated = site.EnsureDefaultScene() || migrated;
			}

			if (migrated)
				JsonFileLoader<ref DZBBC_LocationsConfig>.SaveFile(DZBBC_LOCATIONS_JSON, config, errorMessage);
		}
		return config;
	}

	static ref DZBBC_AIWavesConfig LoadAIWaves()
	{
		ref DZBBC_AIWavesConfig config = new DZBBC_AIWavesConfig;
		string errorMessage;
		if (FileExist(DZBBC_AI_WAVES_JSON))
		{
			if (!JsonFileLoader<ref DZBBC_AIWavesConfig>.LoadFile(DZBBC_AI_WAVES_JSON, config, errorMessage))
				DZBBC_Utils.Warn("AI waves load failed, defaults active: " + errorMessage);
		}
		else
		{
			JsonFileLoader<ref DZBBC_AIWavesConfig>.SaveFile(DZBBC_AI_WAVES_JSON, config, errorMessage);
		}
		if (config)
		{
			config.Normalize();
			JsonFileLoader<ref DZBBC_AIWavesConfig>.SaveFile(DZBBC_AI_WAVES_JSON, config, errorMessage);
		}
		return config;
	}

	static ref DZBBC_LootConfig LoadLoot()
	{
		ref DZBBC_LootConfig config = new DZBBC_LootConfig;
		string errorMessage;
		if (FileExist(DZBBC_LOOT_JSON))
		{
			if (!JsonFileLoader<ref DZBBC_LootConfig>.LoadFile(DZBBC_LOOT_JSON, config, errorMessage))
				DZBBC_Utils.Warn("Loot load failed, defaults active: " + errorMessage);
		}
		else
		{
			JsonFileLoader<ref DZBBC_LootConfig>.SaveFile(DZBBC_LOOT_JSON, config, errorMessage);
		}
		if (config && config.EnsureStoryLootDefaults())
		{
			JsonFileLoader<ref DZBBC_LootConfig>.SaveFile(DZBBC_LOOT_JSON, config, errorMessage);
			DZBBC_Utils.Log("Added SignalMatches and Coded9VBattery to the existing ConvoyZ loot profile.");
		}
		return config;
	}

	static ref DZBBC_TerminalsConfig LoadTerminals()
	{
		ref DZBBC_TerminalsConfig config = new DZBBC_TerminalsConfig;
		string errorMessage;
		if (FileExist(DZBBC_TERMINALS_JSON))
		{
			if (!JsonFileLoader<ref DZBBC_TerminalsConfig>.LoadFile(DZBBC_TERMINALS_JSON, config, errorMessage))
				DZBBC_Utils.Warn("Terminals load failed, defaults active: " + errorMessage);
		}
		else
		{
			JsonFileLoader<ref DZBBC_TerminalsConfig>.SaveFile(DZBBC_TERMINALS_JSON, config, errorMessage);
		}
		if (config && config.NormalizeLiveTerminals())
		{
			JsonFileLoader<ref DZBBC_TerminalsConfig>.SaveFile(DZBBC_TERMINALS_JSON, config, errorMessage);
			DZBBC_Utils.Log("Terminal config normalized: Green Mountain removed, Airfield/VMC enforced.");
		}
		return config;
	}

	static ref DZBBC_MessagesConfig LoadMessages()
	{
		ref DZBBC_MessagesConfig config = new DZBBC_MessagesConfig;
		string errorMessage;
		if (FileExist(DZBBC_MESSAGES_JSON))
		{
			if (!JsonFileLoader<ref DZBBC_MessagesConfig>.LoadFile(DZBBC_MESSAGES_JSON, config, errorMessage))
				DZBBC_Utils.Warn("Messages load failed, defaults active: " + errorMessage);
		}
		else
		{
			JsonFileLoader<ref DZBBC_MessagesConfig>.SaveFile(DZBBC_MESSAGES_JSON, config, errorMessage);
		}
		return config;
	}
}
