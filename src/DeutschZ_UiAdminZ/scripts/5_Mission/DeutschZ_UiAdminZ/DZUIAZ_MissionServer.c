modded class MissionServer
{
	void MissionServer()
	{
		DZUIAZ_ServerState.Initialize();
		DZUIAZ_Settings settings = DZUIAZ_ServerState.Settings;
		if (settings)
			DZUIAZ_Log.Write("INFO", "UiAdminZ initialized. Enabled=" + settings.Enabled.ToString() + " ProDevelopmentEnabled=" + settings.ProDevelopmentEnabled.ToString() + ". Eventmods remain optional.");
	}
}
