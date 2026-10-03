class DZKOTH_UnlockRuntimeProvider extends DZKOTH_LicensedRuntimeProvider
{
	override bool OwnsEventRuntime()
	{
		return true;
	}

	override string GetLicenseStatus()
	{
		return "FULL_PRO_UNIFIED";
	}

	override void InitServer()
	{
		DZKOTH_EventManager.GetInstance().InitServer();
	}

	override void ShutdownServer()
	{
		DZKOTH_EventManager.GetInstance().StopEvent();
	}

	override bool StartEvent()
	{
		return DZKOTH_EventManager.GetInstance().StartEvent(-1);
	}

	override bool StopEvent(string reason)
	{
		DZKOTH_Utils.Log("Licensed runtime stop requested. Reason=" + reason);
		DZKOTH_EventManager.GetInstance().StopEvent();
		return true;
	}

	override void SyncPlayer(PlayerBase player)
	{
		DZKOTH_EventManager.GetInstance().SyncStateToPlayer(player);
	}
}

class DZKOTH_UnlockRuntime
{
	protected static ref DZKOTH_UnlockRuntimeProvider s_Provider;

	static void Register()
	{
		if (!GetGame() || !GetGame().IsServer() || s_Provider)
			return;

		s_Provider = new DZKOTH_UnlockRuntimeProvider;
		if (!DZKOTH_LicensedRuntime.Register(s_Provider))
		{
			s_Provider = null;
			DZKOTH_Utils.Error("Licensed runtime registration failed.");
		}
	}

	static void Unregister()
	{
		if (!s_Provider)
			return;

		DZKOTH_LicensedRuntime.Unregister(s_Provider);
		s_Provider = null;
	}
}
