modded class MissionServer
{
    protected ref deutschz_warningz_settings deutschz_warningz_server_settings;
    protected float deutschz_warningz_elapsed_seconds;
    protected bool deutschz_warningz_sent_15;
    protected bool deutschz_warningz_sent_2;

    void MissionServer()
    {
        deutschz_warningz_server_settings = deutschz_warningz_settings.load();
        deutschz_warningz_elapsed_seconds = deutschz_warningz_server_settings.restart_offset_minutes * 60;
        Print("[deutschz_warningz] Server initialisiert. Restartintervall=" + deutschz_warningz_server_settings.restart_interval_minutes.ToString() + " Minuten.");
    }

    override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
    {
        super.InvokeOnConnect(player, identity);

        if (!deutschz_warningz_server_settings)
            return;

        string safezone_state = deutschz_warningz_rpc.safezone_disabled;
        if (deutschz_warningz_server_settings.enable_safezonewarning)
            safezone_state = deutschz_warningz_rpc.safezone_enabled;

        GetRPCManager().SendRPC(deutschz_warningz_rpc.namespace_name, deutschz_warningz_rpc.play_sound_method, new Param1<string>(safezone_state), true, identity);
        Print("[deutschz_warningz] Safezone-Status an " + identity.GetName() + " gesendet: " + safezone_state);

        if (!deutschz_warningz_server_settings.enable_welcome)
            return;

        int delay_ms = deutschz_warningz_server_settings.welcome_delay_seconds * 1000;
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(deutschz_warningz_send_welcome, delay_ms, false, identity);
    }

    protected void deutschz_warningz_send_welcome(PlayerIdentity identity)
    {
        if (!identity)
            return;

        string sound_name = deutschz_warningz_rpc.random_welcome();
        GetRPCManager().SendRPC(deutschz_warningz_rpc.namespace_name, deutschz_warningz_rpc.play_sound_method, new Param1<string>(sound_name), true, identity);
        Print("[deutschz_warningz] Willkommen-Sound an " + identity.GetName() + " gesendet.");
    }

    protected void deutschz_warningz_broadcast(string sound_name)
    {
        array<Man> players = {};
        GetGame().GetPlayers(players);

        foreach (Man entry: players)
        {
            PlayerBase player = PlayerBase.Cast(entry);
            if (!player || !player.GetIdentity())
                continue;

            GetRPCManager().SendRPC(deutschz_warningz_rpc.namespace_name, deutschz_warningz_rpc.play_sound_method, new Param1<string>(sound_name), true, player.GetIdentity());
        }

        Print("[deutschz_warningz] Broadcast gesendet: " + sound_name + " / Spieler=" + players.Count().ToString());
    }

    override void OnUpdate(float timeslice)
    {
        super.OnUpdate(timeslice);

        if (!deutschz_warningz_server_settings)
            return;

        deutschz_warningz_elapsed_seconds += timeslice;

        float interval_seconds = deutschz_warningz_server_settings.restart_interval_minutes * 60;
        float remaining_seconds = interval_seconds - deutschz_warningz_elapsed_seconds;

        bool should_send_15 = deutschz_warningz_server_settings.enable_restart15min;
        should_send_15 = should_send_15 && !deutschz_warningz_sent_15;
        should_send_15 = should_send_15 && remaining_seconds <= 900;
        should_send_15 = should_send_15 && remaining_seconds > 120;
        if (should_send_15)
        {
            deutschz_warningz_sent_15 = true;
            deutschz_warningz_broadcast(deutschz_warningz_rpc.restart15min);
        }

        bool should_send_2 = deutschz_warningz_server_settings.enable_restart2min;
        should_send_2 = should_send_2 && !deutschz_warningz_sent_2;
        should_send_2 = should_send_2 && remaining_seconds <= 120;
        should_send_2 = should_send_2 && remaining_seconds > 0;
        if (should_send_2)
        {
            deutschz_warningz_sent_2 = true;
            deutschz_warningz_broadcast(deutschz_warningz_rpc.restart2min);
        }

        if (deutschz_warningz_elapsed_seconds >= interval_seconds)
        {
            deutschz_warningz_elapsed_seconds = 0;
            deutschz_warningz_sent_15 = false;
            deutschz_warningz_sent_2 = false;
        }
    }
};
