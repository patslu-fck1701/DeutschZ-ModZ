modded class PlayerBase
{
    override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
    {
        if (rpc_type == DZATM_Const.RPC_ADMIN_COMMAND)
        {
            if (GetGame() && GetGame().IsServer())
            {
                string command;
                if (ctx.Read(command))
                    DZATM_Manager.GetInstance().HandleAdminCommand(sender, this, command);
            }
            return;
        }

        if (rpc_type == DZATM_Const.RPC_CLIENT_PROGRESS) { DZATM_RPC.ReceiveProgress(ctx); return; }
        if (rpc_type == DZATM_Const.RPC_CLIENT_MESSAGE) { DZATM_RPC.ReceiveMessage(ctx); return; }
        if (rpc_type == DZATM_Const.RPC_CLIENT_EFFECT) { DZATM_RPC.ReceiveEffect(ctx); return; }
        if (rpc_type == DZATM_Const.RPC_ADMIN_STATUS) { DZATM_RPC.ReceiveAdminStatus(ctx); return; }

        super.OnRPC(sender, rpc_type, ctx);
    }
}
