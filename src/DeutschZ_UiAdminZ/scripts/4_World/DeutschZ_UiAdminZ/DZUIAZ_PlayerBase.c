class DZUIAZ_ServerState
{
	static ref DZUIAZ_Settings Settings;

	static void Initialize()
	{
		if (!Settings)
			Settings = DZUIAZ_SettingsLoader.Load();
	}
}

modded class PlayerBase
{
	protected int m_DZUIAZ_LastAuthorizationRequest;

	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);

		if (rpc_type == DZUIAZ_Constants.RPC_UIADMIN && GetGame().IsServer())
		{
			DZUIAZ_HandleAuthorization(sender, ctx);
			return;
		}

		if (GetGame().IsServer())
			return;

		if (rpc_type == DZUIAZ_Constants.RPC_UIADMIN)
		{
			int uiProtocol;
			if (!ctx.Read(uiProtocol) || uiProtocol != DZUIAZ_Constants.UI_RPC_PROTOCOL_VERSION)
				return;
			if (!ctx.Read(DZUIAZ_ClientState.Authorized))
				return;
			ctx.Read(DZUIAZ_ClientState.ProDevelopmentEnabled);
			ctx.Read(DZUIAZ_ClientState.AdminName);
			ctx.Read(DZUIAZ_ClientState.LicenseState);
			DZUIAZ_ClientState.AuthorizationResponseReceived = true;
			return;
		}

		if (rpc_type != DZUIAZ_Constants.RPC_EVENT_ADMIN)
			return;
		int eventProtocol;
		if (!ctx.Read(eventProtocol) || eventProtocol != DZUIAZ_Constants.EVENT_RPC_PROTOCOL_VERSION)
			return;
		bool eventAuthorized;
		if (!ctx.Read(eventAuthorized) || !ctx.Read(DZUIAZ_ClientState.ModuleAvailable))
			return;
		ctx.Read(DZUIAZ_ClientState.EventState);
		ctx.Read(DZUIAZ_ClientState.EventPosition);
		ctx.Read(DZUIAZ_ClientState.Radius);
		ctx.Read(DZUIAZ_ClientState.Progress);
		ctx.Read(DZUIAZ_ClientState.EnemyCount);
		ctx.Read(DZUIAZ_ClientState.RewardReady);
		ctx.Read(DZUIAZ_ClientState.Result);
	}

	protected void DZUIAZ_HandleAuthorization(PlayerIdentity sender, ParamsReadContext ctx)
	{
		if (!sender || !GetGame())
			return;
		PlayerIdentity playerIdentity = GetIdentity();
		if (!playerIdentity || playerIdentity.GetPlainId() != sender.GetPlainId())
		{
			DZUIAZ_Log.Write("WARNING", "AUTH identity mismatch steam64=" + sender.GetPlainId());
			return;
		}

		DZUIAZ_ServerState.Initialize();
		DZUIAZ_Settings settings = DZUIAZ_ServerState.Settings;
		if (!settings)
			return;

		int now = GetGame().GetTime();
		if (now - m_DZUIAZ_LastAuthorizationRequest < settings.RequestRateLimitMilliseconds)
		{
			DZUIAZ_Log.Write("WARNING", "AUTH rate_limited steam64=" + sender.GetPlainId());
			return;
		}
		m_DZUIAZ_LastAuthorizationRequest = now;

		int protocol;
		if (!ctx.Read(protocol) || protocol != DZUIAZ_Constants.UI_RPC_PROTOCOL_VERSION)
		{
			DZUIAZ_Log.Write("WARNING", "AUTH invalid_protocol steam64=" + sender.GetPlainId());
			return;
		}

		bool authorized = settings.IsAdmin(sender.GetPlainId());
		if (settings.AuditLogEnabled)
			DZUIAZ_Log.Write("AUDIT", "steam64=" + sender.GetPlainId() + " name=" + sender.GetName() + " action=OPEN result=" + authorized.ToString());

		ScriptRPC response = new ScriptRPC;
		response.Write(DZUIAZ_Constants.UI_RPC_PROTOCOL_VERSION);
		response.Write(authorized);
		response.Write(settings.ProDevelopmentEnabled);
		response.Write(sender.GetName());
		response.Write("PRO GESPERRT - LICENSECORE NICHT IMPLEMENTIERT");
		response.Send(this, DZUIAZ_Constants.RPC_UIADMIN, true, sender);
	}
}
