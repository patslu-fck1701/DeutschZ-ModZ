modded class PlayerBase
{
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		if (rpc_type == DZKOTHG_RPC_MARKER_SYNC)
		{
			DZKOTHG_ClientMarkerStore.Get().OnMarkerRPC(ctx);
			return;
		}

		super.OnRPC(sender, rpc_type, ctx);
	}
}
