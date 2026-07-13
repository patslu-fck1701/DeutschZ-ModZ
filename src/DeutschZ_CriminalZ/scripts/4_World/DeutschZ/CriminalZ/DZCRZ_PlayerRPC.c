modded class PlayerBase
{
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		if (rpc_type == DZCRZ_Const.RPC_ADMIN_COMMAND)
		{
			if (GetGame() && GetGame().IsServer())
			{
				string command;
				if (ctx.Read(command))
					DZCRZ_Manager.GetInstance().HandleAdminCommand(sender, this, command);
			}
			return;
		}
		if (rpc_type == DZCRZ_Const.RPC_CLIENT_PROGRESS)
		{
			DZCRZ_RPC.ReceiveProgress(ctx);
			return;
		}
		if (rpc_type == DZCRZ_Const.RPC_CLIENT_MESSAGE)
		{
			DZCRZ_RPC.ReceiveMessage(ctx);
			return;
		}
		if (rpc_type == DZCRZ_Const.RPC_CLIENT_EFFECT)
		{
			DZCRZ_RPC.ReceiveEffect(ctx);
			return;
		}
		super.OnRPC(sender, rpc_type, ctx);
	}
}
