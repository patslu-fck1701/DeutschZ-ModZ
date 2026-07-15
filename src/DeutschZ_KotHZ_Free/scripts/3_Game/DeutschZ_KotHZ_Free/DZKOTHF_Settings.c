class DZKOTHF_Settings
{
	bool Enabled;
	bool AutoStart;
	int AutoStartDelaySeconds;
	int AnnounceSeconds;
	float CaptureRadius;
	int CaptureTimeSeconds;
	int CaptureTickMilliseconds;
	int CompletionCleanupDelaySeconds;
	ref array<float> EventPosition;
	ref array<float> EventOrientation;

	void DZKOTHF_Settings()
	{
		Enabled = true;
		AutoStart = true;
		AutoStartDelaySeconds = 3;
		AnnounceSeconds = 10;
		CaptureRadius = 25.0;
		CaptureTimeSeconds = 300;
		CaptureTickMilliseconds = 1000;
		CompletionCleanupDelaySeconds = 10;
		EventPosition = {4552.346680, 317.997314, 8350.974609};
		EventOrientation = {0.0, 0.0, 0.0};
	}

	void Validate()
	{
		AutoStartDelaySeconds = Math.Clamp(AutoStartDelaySeconds, 1, 3600);
		AnnounceSeconds = Math.Clamp(AnnounceSeconds, 1, 3600);
		CaptureRadius = Math.Clamp(CaptureRadius, 5.0, 250.0);
		CaptureTimeSeconds = Math.Clamp(CaptureTimeSeconds, 10, 7200);
		CaptureTickMilliseconds = Math.Clamp(CaptureTickMilliseconds, 250, 5000);
		CompletionCleanupDelaySeconds = Math.Clamp(CompletionCleanupDelaySeconds, 1, 600);

		if (!EventPosition || EventPosition.Count() != 3)
			EventPosition = {4552.346680, 317.997314, 8350.974609};

		if (!EventOrientation || EventOrientation.Count() != 3)
			EventOrientation = {0.0, 0.0, 0.0};
	}

	vector GetEventPosition()
	{
		return Vector(EventPosition[0], EventPosition[1], EventPosition[2]);
	}

	vector GetEventOrientation()
	{
		return Vector(EventOrientation[0], EventOrientation[1], EventOrientation[2]);
	}
}

class DZKOTHF_SettingsLoader
{
	static ref DZKOTHF_Settings Load()
	{
		EnsureDirectories();

		ref DZKOTHF_Settings settings = new DZKOTHF_Settings;
		string errorMessage;
		if (FileExist(DZKOTHF_Constants.SETTINGS_PATH))
		{
			if (!JsonFileLoader<ref DZKOTHF_Settings>.LoadFile(DZKOTHF_Constants.SETTINGS_PATH, settings, errorMessage) || !settings)
			{
				DZKOTHF_Log.Error("Settings could not be loaded; safe defaults are used. " + errorMessage);
				settings = new DZKOTHF_Settings;
			}
		}

		settings.Validate();
		if (!JsonFileLoader<ref DZKOTHF_Settings>.SaveFile(DZKOTHF_Constants.SETTINGS_PATH, settings, errorMessage))
			DZKOTHF_Log.Error("Settings could not be saved. " + errorMessage);
		else
			DZKOTHF_Log.Info("Settings loaded from " + DZKOTHF_Constants.SETTINGS_PATH + ".");

		return settings;
	}

	protected static void EnsureDirectories()
	{
		if (!FileExist(DZKOTHF_Constants.PROFILE_ROOT))
			MakeDirectory(DZKOTHF_Constants.PROFILE_ROOT);

		if (!FileExist(DZKOTHF_Constants.PROFILE_MOD))
			MakeDirectory(DZKOTHF_Constants.PROFILE_MOD);
	}
}
