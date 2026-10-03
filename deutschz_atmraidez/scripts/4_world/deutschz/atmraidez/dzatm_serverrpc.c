class DZATM_ServerRPC
{
    static void SendAdminCommand(PlayerBase player, string command)
    {
        if (!player || command == "") return;
        ScriptRPC rpc = new ScriptRPC;
        rpc.Write(command);
        rpc.Send(player, DZATM_Const.RPC_ADMIN_COMMAND, true, null);
    }

    static void SendProgress(PlayerBase player, bool visible, float progress, float duration, string label)
    {
        if (!player || !player.GetIdentity()) return;
        ScriptRPC rpc = new ScriptRPC;
        rpc.Write(visible);
        rpc.Write(progress);
        rpc.Write(duration);
        rpc.Write(label);
        rpc.Send(player, DZATM_Const.RPC_CLIENT_PROGRESS, true, player.GetIdentity());
    }

    static void SendMessage(PlayerBase player, string message, int durationMs = 6000)
    {
        if (!player || !player.GetIdentity()) return;
        ScriptRPC rpc = new ScriptRPC;
        rpc.Write(message);
        rpc.Write(durationMs);
        rpc.Send(player, DZATM_Const.RPC_CLIENT_MESSAGE, true, player.GetIdentity());
    }

    static void SendEffect(PlayerBase player, bool start, string effectId, string soundSet, vector position, bool loop)
    {
        if (!player || !player.GetIdentity()) return;
        ScriptRPC rpc = new ScriptRPC;
        rpc.Write(start);
        rpc.Write(effectId);
        rpc.Write(soundSet);
        rpc.Write(position);
        rpc.Write(loop);
        rpc.Send(player, DZATM_Const.RPC_CLIENT_EFFECT, true, player.GetIdentity());
    }

    static void SendAdminStatus(PlayerBase player, int sessions, int atms, bool debugEnabled, string message)
    {
        if (!player || !player.GetIdentity()) return;
        ScriptRPC rpc = new ScriptRPC;
        rpc.Write(sessions);
        rpc.Write(atms);
        rpc.Write(debugEnabled);
        rpc.Write(message);
        rpc.Send(player, DZATM_Const.RPC_ADMIN_STATUS, true, player.GetIdentity());
    }
}
