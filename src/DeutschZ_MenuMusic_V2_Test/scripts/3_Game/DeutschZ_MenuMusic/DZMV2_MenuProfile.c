class DZMV2_MenuSettings
{
	bool Enabled = true;
	float MusicVolumeCap = 0.66;
	int TrackCount = 11;
}

class DZMV2_MenuPaths
{
	static const string ROOT = "$profile:DeutschZ-System";
	static const string CONFIG_DIR = "$profile:DeutschZ-System/DeutschZ_MenuZ";
	static const string SETTINGS = "$profile:DeutschZ-System/DeutschZ_MenuZ/MenuZSettings.json";
	static const string LOG_ROOT = "$profile:DeutschZ-System/LogZ";
	static const string LOG_DIR = "$profile:DeutschZ-System/LogZ/DeutschZ_MenuZ";
	static const string LOG_FILE = "$profile:DeutschZ-System/LogZ/DeutschZ_MenuZ/MenuZ.log";
	static const string LEGACY_SETTINGS = "$profile:DeutschZ/MenuZ/MenuZSettings.json";
}

class DZMV2_MenuProfile
{
	static void EnsureDirectory(string path)
	{
		if (!FileExist(path))
			MakeDirectory(path);
	}

	static void Ensure()
	{
		EnsureDirectory(DZMV2_MenuPaths.ROOT);
		EnsureDirectory(DZMV2_MenuPaths.CONFIG_DIR);
		EnsureDirectory(DZMV2_MenuPaths.LOG_ROOT);
		EnsureDirectory(DZMV2_MenuPaths.LOG_DIR);

		if (!FileExist(DZMV2_MenuPaths.SETTINGS) && FileExist(DZMV2_MenuPaths.LEGACY_SETTINGS))
		{
			if (CopyFile(DZMV2_MenuPaths.LEGACY_SETTINGS, DZMV2_MenuPaths.SETTINGS))
				Log("Legacy settings copied to the current DeutschZ-System path.");
		}

		DZMV2_MenuSettings settings = new DZMV2_MenuSettings();
		if (!FileExist(DZMV2_MenuPaths.SETTINGS))
		{
			SaveDefaults(settings, "Settings created because no file existed.");
			return;
		}

		string error;
		if (!JsonFileLoader<ref DZMV2_MenuSettings>.LoadFile(DZMV2_MenuPaths.SETTINGS, settings, error))
		{
			string invalidCopy = DZMV2_MenuPaths.CONFIG_DIR + "/MenuZSettings.invalid.json";
			CopyFile(DZMV2_MenuPaths.SETTINGS, invalidCopy);
			settings = new DZMV2_MenuSettings();
			SaveDefaults(settings, "Damaged settings replaced; original copied to MenuZSettings.invalid.json. " + error);
			return;
		}

		Log("Settings loaded without overwriting: " + DZMV2_MenuPaths.SETTINGS);
	}

	static void SaveDefaults(DZMV2_MenuSettings settings, string reason)
	{
		string error;
		if (!JsonFileLoader<ref DZMV2_MenuSettings>.SaveFile(DZMV2_MenuPaths.SETTINGS, settings, error))
		{
			Log("ERROR: MenuZSettings.json could not be written: " + error);
			return;
		}

		Log(reason);
	}

	static void Log(string message)
	{
		Print("[DeutschZ MenuZ V2] " + message);
		FileHandle file = OpenFile(DZMV2_MenuPaths.LOG_FILE, FileMode.APPEND);
		if (file != 0)
		{
			FPrintln(file, "[DeutschZ MenuZ V2] " + message);
			CloseFile(file);
		}
	}
}
