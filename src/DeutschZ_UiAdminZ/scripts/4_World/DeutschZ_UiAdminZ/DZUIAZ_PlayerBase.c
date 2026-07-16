modded class PlayerBase
{
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);
		if (rpc_type != DZUIAZ_Constants.RPC_ADMIN || GetGame().IsServer()) return;
		int protocol; if (!ctx.Read(protocol) || protocol != DZUIAZ_Constants.RPC_PROTOCOL_VERSION) return;
		if (!ctx.Read(DZUIAZ_ClientState.Authorized) || !ctx.Read(DZUIAZ_ClientState.ModuleAvailable)) return;
		ctx.Read(DZUIAZ_ClientState.EventState);
		ctx.Read(DZUIAZ_ClientState.EventPosition);
		ctx.Read(DZUIAZ_ClientState.Radius);
		ctx.Read(DZUIAZ_ClientState.Progress);
		ctx.Read(DZUIAZ_ClientState.EnemyCount);
		ctx.Read(DZUIAZ_ClientState.RewardReady);
		ctx.Read(DZUIAZ_ClientState.Result);
	}
}
