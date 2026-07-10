class DZEV_EventBus
{
	private static ref ScriptInvoker s_OnClientProgress;
	private static ref ScriptInvoker s_OnClientMarker;

	static ScriptInvoker OnClientProgress()
	{
		if (!s_OnClientProgress)
			s_OnClientProgress = new ScriptInvoker;

		return s_OnClientProgress;
	}

	static ScriptInvoker OnClientMarker()
	{
		if (!s_OnClientMarker)
			s_OnClientMarker = new ScriptInvoker;

		return s_OnClientMarker;
	}
}
