modded class MissionGameplay
{
    protected EffectSound deutschz_warningz_active_sound;
    protected bool deutschz_warningz_safezone_initialized;
    protected bool deutschz_warningz_was_in_safezone;
    protected bool deutschz_warningz_safezone_enabled = true;
    protected float deutschz_warningz_safezone_check_accumulator;
    protected bool deutschz_warningz_life_state_initialized;
    protected bool deutschz_warningz_was_alive;

    void MissionGameplay()
    {
        GetRPCManager().AddRPC(deutschz_warningz_rpc.namespace_name, deutschz_warningz_rpc.play_sound_method, this, SingleplayerExecutionType.Client);
        Print("[deutschz_warningz] Client-RPC registriert.");
    }

    override void StartLogoutMenu(int time)
    {
        super.StartLogoutMenu(time);
        if (time > 0)
            deutschz_warningz_play(deutschz_warningz_rpc.random_logout());
    }

    void play_sound(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type != CallType.Client)
            return;

        Param1<string> data;
        if (!ctx.Read(data))
            return;

        if (data.param1 == deutschz_warningz_rpc.safezone_enabled)
        {
            deutschz_warningz_safezone_enabled = true;
            Print("[deutschz_warningz] Safezone-Ueberwachung aktiviert.");
            return;
        }

        if (data.param1 == deutschz_warningz_rpc.safezone_disabled)
        {
            deutschz_warningz_safezone_enabled = false;
            deutschz_warningz_safezone_initialized = false;
            Print("[deutschz_warningz] Safezone-Ueberwachung deaktiviert.");
            return;
        }

        deutschz_warningz_play(data.param1);
    }

    protected void deutschz_warningz_play(string sound_name)
    {
        string soundset;

        switch (sound_name)
        {
            case deutschz_warningz_rpc.welcomeevents:
                soundset = "deutschz_warningz_welcomeevents_soundset";
                break;
            case deutschz_warningz_rpc.restart15min:
                soundset = "deutschz_warningz_restart15min_soundset";
                break;
            case deutschz_warningz_rpc.restart2min:
                soundset = "deutschz_warningz_restart2min_soundset";
                break;
            case deutschz_warningz_rpc.safezonewarning:
                soundset = "deutschz_warningz_safezonewarning_soundset";
                break;
            case deutschz_warningz_rpc.welcome1: soundset = "deutschz_warningz_welcome1_soundset"; break;
            case deutschz_warningz_rpc.welcome2: soundset = "deutschz_warningz_welcome2_soundset"; break;
            case deutschz_warningz_rpc.safezone1: soundset = "deutschz_warningz_safezone1_soundset"; break;
            case deutschz_warningz_rpc.safezone2: soundset = "deutschz_warningz_safezone2_soundset"; break;
            case deutschz_warningz_rpc.death1: soundset = "deutschz_warningz_death1_soundset"; break;
            case deutschz_warningz_rpc.death2: soundset = "deutschz_warningz_death2_soundset"; break;
            case deutschz_warningz_rpc.death3: soundset = "deutschz_warningz_death3_soundset"; break;
            case deutschz_warningz_rpc.logout1: soundset = "deutschz_warningz_logout1_soundset"; break;
            case deutschz_warningz_rpc.logout2: soundset = "deutschz_warningz_logout2_soundset"; break;
            case deutschz_warningz_rpc.logout3: soundset = "deutschz_warningz_logout3_soundset"; break;
            default:
                return;
        }

        vector sound_position = "0 0 0";
        PlayerBase local_player = PlayerBase.Cast(GetGame().GetPlayer());
        if (local_player)
            sound_position = local_player.GetPosition();

        deutschz_warningz_active_sound = SEffectManager.PlaySound(soundset, sound_position);
        if (deutschz_warningz_active_sound)
            Print("[deutschz_warningz] Sound gestartet: " + sound_name + " / " + soundset);
        else
            ErrorEx("[deutschz_warningz] Sound konnte nicht gestartet werden: " + sound_name + " / " + soundset);
    }

    override void OnUpdate(float timeslice)
    {
        super.OnUpdate(timeslice);

        if (!deutschz_warningz_safezone_enabled)
            return;

        deutschz_warningz_safezone_check_accumulator += timeslice;
        if (deutschz_warningz_safezone_check_accumulator < 0.5)
            return;

        deutschz_warningz_safezone_check_accumulator = 0;

        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (!player || !player.GetIdentity())
        {
            deutschz_warningz_safezone_initialized = false;
            deutschz_warningz_life_state_initialized = false;
            return;
        }

        bool is_alive = player.IsAlive();
        if (!deutschz_warningz_life_state_initialized)
        {
            deutschz_warningz_was_alive = is_alive;
            deutschz_warningz_life_state_initialized = true;
        }
        else if (deutschz_warningz_was_alive && !is_alive)
        {
            deutschz_warningz_play(deutschz_warningz_rpc.random_death());
        }
        deutschz_warningz_was_alive = is_alive;

        if (!is_alive)
        {
            deutschz_warningz_safezone_initialized = false;
            return;
        }

        bool is_in_safezone = player.Expansion_IsInSafeZone();

        if (!deutschz_warningz_safezone_initialized)
        {
            deutschz_warningz_was_in_safezone = is_in_safezone;
            deutschz_warningz_safezone_initialized = true;
            return;
        }

        if (deutschz_warningz_was_in_safezone && !is_in_safezone)
            deutschz_warningz_play(deutschz_warningz_rpc.random_safezone());

        deutschz_warningz_was_in_safezone = is_in_safezone;
    }
};
