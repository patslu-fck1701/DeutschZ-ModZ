modded class PlayerBase
{
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		if (rpc_type == DZBGZ_RPC_ADMIN_COMMAND)
		{
			if (GetGame() && GetGame().IsServer())
				DZBGZ_EventManager.GetInstance().HandleAdminCommand(sender, this, DZBGZ_ReadAdminCommand(ctx));
			return;
		}

		if (rpc_type == DZBGZ_RPC_MARKER_SYNC)
		{
			DZBGZ_ClientMarkerStore.Get().OnMarkerRPC(ctx);
			return;
		}

		if (rpc_type == DZBGZ_RPC_CLIENT_FX)
		{
			DZBGZ_ClientFX.OnFXRPC(ctx);
			return;
		}

		super.OnRPC(sender, rpc_type, ctx);
	}

	protected string DZBGZ_ReadAdminCommand(ParamsReadContext ctx)
	{
		string command;
		if (!ctx.Read(command))
			return "";

		return command;
	}
}
