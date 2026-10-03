class deutschz_afkz_record
{
    PlayerBase player;
    int last_activity_ms;
    bool preparing;
};

class deutschz_afkz_manager
{
    protected static ref deutschz_afkz_manager instance;
    protected ref array<ref deutschz_afkz_record> records;
    protected ref deutschz_afkz_settings settings;
    protected bool running;

    static deutschz_afkz_manager get()
    {
        if (!instance)
            instance = new deutschz_afkz_manager();
        return instance;
    }

    void deutschz_afkz_manager()
    {
        records = new array<ref deutschz_afkz_record>();
    }

    void start()
    {
        if (!GetGame().IsServer() || running)
            return;
        settings = deutschz_afkz_settings_loader.load();
        running = true;
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(tick, 1000, true);
        Print("[deutschz_afkz] Servermanager gestartet. AFK-Sekunden=" + settings.AfkSeconds.ToString());
    }

    deutschz_afkz_settings get_settings()
    {
        return settings;
    }

    void stop()
    {
        if (!running)
            return;
        running = false;
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(tick);
        foreach (deutschz_afkz_record record: records)
        {
            if (record && record.player)
                deactivate(record.player, false);
        }
        records.Clear();
    }

    void activity(PlayerBase player)
    {
        if (!player || !player.IsAlive())
            return;
        deutschz_afkz_record record = find_or_create(player);
        record.last_activity_ms = GetGame().GetTime();
        record.preparing = false;
        if (player.deutschz_afkz_is_active())
            deactivate(player, true);
    }

    void remove(PlayerBase player)
    {
        for (int i = records.Count() - 1; i >= 0; i--)
        {
            if (records[i] && records[i].player == player)
            {
                player.deutschz_afkz_set_active(false);
                records.Remove(i);
            }
        }
    }

    protected deutschz_afkz_record find_or_create(PlayerBase player)
    {
        foreach (deutschz_afkz_record existing: records)
        {
            if (existing && existing.player == player)
                return existing;
        }
        deutschz_afkz_record record = new deutschz_afkz_record();
        record.player = player;
        record.last_activity_ms = GetGame().GetTime();
        records.Insert(record);
        return record;
    }

    protected void tick()
    {
        if (!running)
            return;
        int now = GetGame().GetTime();
        foreach (deutschz_afkz_record record: records)
        {
            if (!record || !record.player || !record.player.IsAlive())
                continue;
            if (record.player.deutschz_afkz_is_active())
                continue;
            if (now - record.last_activity_ms < settings.AfkSeconds * 1000)
                continue;
            prepare_or_activate(record);
        }
    }

    protected void prepare_or_activate(deutschz_afkz_record record)
    {
        PlayerBase player = record.player;
        HumanCommandVehicle command = player.GetCommand_Vehicle();
        if (!command)
        {
            activate(player);
            return;
        }

        Transport transport = command.GetTransport();
        if (!transport)
            return;
        record.preparing = true;

#ifdef RFFS_HELI
        RFFSHeli_base rffs = RFFSHeli_base.Cast(transport);
        if (rffs)
        {
            land_rffs(player, rffs);
            return;
        }
#endif

#ifdef EXPANSIONMODVEHICLE
        ExpansionHelicopterScript expansion_heli = ExpansionHelicopterScript.Cast(transport);
        if (expansion_heli)
        {
            land_expansion(player, expansion_heli);
            return;
        }
#endif

        CarScript car = CarScript.Cast(transport);
        if (car)
        {
            stop_ground_vehicle(player, car);
            return;
        }

        Print("[deutschz_afkz] Unbekannter Transporttyp; AFK-Aktivierung sicher ausgesetzt: " + transport.GetType());
    }

    protected void stop_ground_vehicle(PlayerBase player, CarScript car)
    {
        vector velocity = GetVelocity(car) * 0.55;
        SetVelocity(car, velocity);
        if (velocity.Length() > settings.GroundVehicleStopSpeed)
            return;
        car.EngineStop();
        safe_get_out(player, car);
    }

#ifdef RFFS_HELI
    protected void land_rffs(PlayerBase player, RFFSHeli_base heli)
    {
        vector velocity = heli.GetVelocityAdjusted();
        float horizontal = Math.Sqrt(velocity[0] * velocity[0] + velocity[2] * velocity[2]);
        velocity[0] = velocity[0] * 0.65;
        velocity[2] = velocity[2] * 0.65;
        if (heli.GetAGLAltitude() > settings.HelicopterLandingHeight)
            velocity[1] = -settings.HelicopterDescentSpeed;
        else
            velocity[1] = Math.Max(velocity[1], -0.15);
        heli.SetVelocityAdjusted(velocity);
        if (heli.GetAGLAltitude() <= settings.HelicopterLandingHeight && horizontal <= settings.HelicopterHorizontalStopSpeed)
        {
            heli.SetVelocityAdjusted(vector.Zero);
            safe_get_out(player, heli);
        }
    }
#endif

#ifdef EXPANSIONMODVEHICLE
    protected void land_expansion(PlayerBase player, ExpansionHelicopterScript heli)
    {
        if (!heli.IsAutoHover())
            heli.SwitchAutoHover();
        vector velocity = GetVelocity(heli);
        float horizontal = Math.Sqrt(velocity[0] * velocity[0] + velocity[2] * velocity[2]);
        float ground_y = GetGame().SurfaceY(heli.GetPosition()[0], heli.GetPosition()[2]);
        float agl = heli.GetPosition()[1] - ground_y;
        velocity[0] = velocity[0] * 0.65;
        velocity[2] = velocity[2] * 0.65;
        if (agl > settings.HelicopterLandingHeight)
            velocity[1] = -settings.HelicopterDescentSpeed;
        else
            velocity[1] = Math.Max(velocity[1], -0.15);
        SetVelocity(heli, velocity);
        if (agl <= settings.HelicopterLandingHeight && horizontal <= settings.HelicopterHorizontalStopSpeed)
        {
            SetVelocity(heli, vector.Zero);
            safe_get_out(player, heli);
        }
    }
#endif

    protected void safe_get_out(PlayerBase player, Transport transport)
    {
        int seat = transport.CrewMemberIndex(player);
        if (seat < 0)
            return;
        transport.CrewGetOut(seat);
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(activate, 750, false, player);
    }

    protected void activate(PlayerBase player)
    {
        if (!player || !player.IsAlive() || player.GetCommand_Vehicle())
            return;
        player.deutschz_afkz_set_active(true);
        Print("[deutschz_afkz] AFK aktiviert: " + player.GetIdentity().GetName());
    }

    protected void deactivate(PlayerBase player, bool notify)
    {
        if (!player)
            return;
        player.deutschz_afkz_set_active(false);
        if (notify)
            NotificationSystem.SendNotificationToPlayerExtended(player, 6.0, "AFK beendet", "Du bist wieder verletzbar.");
        Print("[deutschz_afkz] AFK aufgehoben.");
    }
};
