modded class MissionServer
{
	void MissionServer()
	{
		DZUIAZ_Settings settings = DZUIAZ_SettingsLoader.Load();
		DZUIAZ_Log.Write("INFO", "Admin Lite initialized. Enabled=" + settings.Enabled.ToString() + ". Eventmods remain optional.");
	}
}
