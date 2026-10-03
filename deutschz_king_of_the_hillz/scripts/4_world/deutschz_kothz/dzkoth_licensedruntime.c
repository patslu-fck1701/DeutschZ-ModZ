class DZKOTH_LicensedRuntimeProvider
{
	bool OwnsEventRuntime()
	{
		return false;
	}

	string GetLicenseStatus()
	{
		return "BASE";
	}

	void InitServer()
	{
	}

	void ShutdownServer()
	{
	}

	bool StartEvent()
	{
		return false;
	}

	bool StopEvent(string reason)
	{
		return false;
	}

	void SyncPlayer(PlayerBase player)
	{
	}
}

class DZKOTH_LicensedRuntime
{
	protected static ref DZKOTH_LicensedRuntimeProvider s_Provider;

	static bool Register(DZKOTH_LicensedRuntimeProvider provider)
	{
		if (!GetGame() || !GetGame().IsServer() || !provider)
			return false;

		if (s_Provider)
		{
			DZKOTHF_Log.Warning("Licensed runtime registration rejected: a provider is already active.");
			return false;
		}

		s_Provider = provider;
		DZKOTHF_Log.Info("Licensed runtime registered. Status=" + s_Provider.GetLicenseStatus() + ".");
		return true;
	}

	static void Unregister(DZKOTH_LicensedRuntimeProvider provider)
	{
		if (s_Provider != provider)
			return;

		DZKOTHF_Log.Info("Licensed runtime unregistered.");
		s_Provider = null;
	}

	static bool IsRegistered()
	{
		return s_Provider != null;
	}

	static bool OwnsEventRuntime()
	{
		return s_Provider && s_Provider.OwnsEventRuntime();
	}

	static string GetStatus()
	{
		if (!s_Provider)
			return "BASE";

		return s_Provider.GetLicenseStatus();
	}

	static void InitServer()
	{
		if (s_Provider)
			s_Provider.InitServer();
	}

	static void ShutdownServer()
	{
		if (s_Provider)
			s_Provider.ShutdownServer();
	}

	static bool StartEvent()
	{
		return s_Provider && s_Provider.StartEvent();
	}

	static bool StopEvent(string reason)
	{
		return s_Provider && s_Provider.StopEvent(reason);
	}

	static void SyncPlayer(PlayerBase player)
	{
		if (s_Provider)
			s_Provider.SyncPlayer(player);
	}
}
