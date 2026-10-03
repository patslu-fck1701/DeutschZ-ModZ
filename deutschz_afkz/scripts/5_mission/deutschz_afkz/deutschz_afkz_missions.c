modded class MissionServer
{
    override void OnMissionStart()
    {
        super.OnMissionStart();
        deutschz_afkz_manager.get().start();
    }

    void ~MissionServer()
    {
        deutschz_afkz_manager.get().stop();
    }
};

modded class MissionGameplay
{
    protected vector deutschz_afkz_last_camera_direction;
    protected vector deutschz_afkz_last_player_position;
    protected float deutschz_afkz_sample_elapsed;
    protected bool deutschz_afkz_client_active;
    protected AbstractWave deutschz_afkz_wave;

    override void OnMissionStart()
    {
        super.OnMissionStart();
        deutschz_afkz_last_camera_direction = GetGame().GetCurrentCameraDirection();
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (player)
        {
            deutschz_afkz_last_player_position = player.GetPosition();
            deutschz_afkz_send_activity(player);
        }
    }

    override void OnUpdate(float timeslice)
    {
        super.OnUpdate(timeslice);
        if (deutschz_afkz_client_state.pending)
        {
            deutschz_afkz_client_state.pending = false;
            deutschz_afkz_apply_client_state(deutschz_afkz_client_state.active);
        }
        deutschz_afkz_sample_elapsed += timeslice;
        if (deutschz_afkz_sample_elapsed < 0.25)
            return;
        deutschz_afkz_sample_elapsed = 0;

        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (!player || !player.IsAlive())
            return;

        bool active_input = deutschz_afkz_has_input();
        vector camera_direction = GetGame().GetCurrentCameraDirection();
        if (vector.DistanceSq(camera_direction, deutschz_afkz_last_camera_direction) > 0.000001)
            active_input = true;
        if (vector.DistanceSq(player.GetPosition(), deutschz_afkz_last_player_position) > 0.01)
            active_input = true;

        deutschz_afkz_last_camera_direction = camera_direction;
        deutschz_afkz_last_player_position = player.GetPosition();
        if (active_input)
            deutschz_afkz_send_activity(player);

        if (deutschz_afkz_client_active)
            deutschz_afkz_ensure_active_state(player);
    }

    protected bool deutschz_afkz_has_input()
    {
        array<string> inputs = {
            "UAMoveForward", "UAMoveBack", "UAMoveLeft", "UAMoveRight", "UATurbo", "UAGetOver",
            "UAFire", "UAADSToggle", "UARaiseWeapon", "UAAction", "UAActionTarget", "UAInventory",
            "UAGear", "UAChat", "UAMap", "UAVehicleMoveForward", "UAVehicleMoveBackward",
            "UAVehicleSteerLeft", "UAVehicleSteerRight", "UAHeliCollectiveRaise", "UAHeliCollectiveLower",
            "UAUISelect", "UAUIBack", "UAUIDragNDrop"
        };
        foreach (string input_name: inputs)
        {
            UAInput input = GetUApi().GetInputByName(input_name);
            if (input && (input.LocalPress() || Math.AbsFloat(input.LocalValue()) > 0.01))
                return true;
        }
        return false;
    }

    protected void deutschz_afkz_send_activity(PlayerBase player)
    {
        player.RPCSingleParam(deutschz_afkz_rpc.activity, new Param1<int>(GetGame().GetTime()), true, null);
        if (deutschz_afkz_client_active)
            deutschz_afkz_apply_client_state(false);
    }

    void deutschz_afkz_apply_client_state(bool active)
    {
        deutschz_afkz_client_active = active;
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (active)
        {
            deutschz_afkz_ensure_active_state(player);
            return;
        }
        if (deutschz_afkz_wave)
        {
            deutschz_afkz_wave.GetEvents().Event_OnSoundWaveEnded.Remove(deutschz_afkz_on_sound_ended);
            deutschz_afkz_wave.GetEvents().Event_OnSoundWaveStopped.Remove(deutschz_afkz_on_sound_stopped);
            deutschz_afkz_wave.Stop();
            deutschz_afkz_wave = null;
        }
    }

    protected void deutschz_afkz_ensure_active_state(PlayerBase player)
    {
        if (!player || !player.IsAlive())
            return;

        EmoteManager emote_manager = player.GetEmoteManager();
        if (emote_manager && !emote_manager.IsEmotePlaying())
            deutschz_afkz_start_dance(player);

        if (!deutschz_afkz_wave)
            deutschz_afkz_start_sound();
    }

    protected void deutschz_afkz_start_sound()
    {
        SoundParams sound_params = new SoundParams("deutschz_afkz_dance_soundset");
        if (!sound_params.IsValid())
        {
            ErrorEx("[deutschz_afkz] Ungueltiges SoundSet: deutschz_afkz_dance_soundset");
            return;
        }
        SoundObjectBuilder builder = new SoundObjectBuilder(sound_params);
        SoundObject sound_object = builder.BuildSoundObject();
        if (!sound_object)
        {
            ErrorEx("[deutschz_afkz] SoundObject konnte nicht gebaut werden: deutschz_afkz_dance_soundset");
            return;
        }
        sound_object.SetKind(WaveKind.WAVEUI);
        deutschz_afkz_wave = GetGame().GetSoundScene().Play2D(sound_object, builder);
        if (!deutschz_afkz_wave)
        {
            ErrorEx("[deutschz_afkz] Play2D lieferte keine Wave: deutschz_afkz_dance_soundset");
            return;
        }
        deutschz_afkz_wave.Loop(true);
        deutschz_afkz_wave.GetEvents().Event_OnSoundWaveEnded.Insert(deutschz_afkz_on_sound_ended);
        deutschz_afkz_wave.GetEvents().Event_OnSoundWaveStopped.Insert(deutschz_afkz_on_sound_stopped);
        deutschz_afkz_wave.SetVolume(deutschz_afkz_client_state.sound_volume);
        deutschz_afkz_wave.Play();
        Print("[deutschz_afkz] AFK-Sound gestartet. Lautstaerke=" + deutschz_afkz_client_state.sound_volume.ToString());
    }

    protected void deutschz_afkz_on_sound_ended()
    {
        if (deutschz_afkz_wave)
            deutschz_afkz_wave.GetEvents().Event_OnSoundWaveStopped.Remove(deutschz_afkz_on_sound_stopped);
        deutschz_afkz_wave = null;
        Print("[deutschz_afkz] AFK-Sound beendet; Wiedergabe wird bei aktivem AFK-Status erneuert.");
    }

    protected void deutschz_afkz_on_sound_stopped()
    {
        deutschz_afkz_wave = null;
        Print("[deutschz_afkz] AFK-Sound gestoppt; Wiedergabe wird bei aktivem AFK-Status erneuert.");
    }

    protected void deutschz_afkz_start_dance(PlayerBase player)
    {
        if (!player || !player.GetEmoteManager())
            return;
        player.GetEmoteManager().CreateEmoteCBFromMenu(EmoteConstants.ID_EMOTE_DANCE, true);
    }
};
