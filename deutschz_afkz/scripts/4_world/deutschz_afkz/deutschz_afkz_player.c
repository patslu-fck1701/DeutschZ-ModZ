modded class PlayerBase
{
    protected bool deutschz_afkz_active;

    bool deutschz_afkz_is_active()
    {
        return deutschz_afkz_active;
    }

    void deutschz_afkz_set_active(bool active)
    {
        if (!GetGame().IsServer() || deutschz_afkz_active == active)
            return;

        deutschz_afkz_active = active;
        SetAllowDamage(!active);
        SetSynchDirty();
        float sound_volume = 0.8;
        deutschz_afkz_settings settings = deutschz_afkz_manager.get().get_settings();
        if (settings)
            sound_volume = settings.SoundVolume;
        RPCSingleParam(deutschz_afkz_rpc.state, new Param2<bool, float>(active, sound_volume), true, GetIdentity());
    }

    override bool CanBeTargetedByAI(EntityAI ai)
    {
        if (deutschz_afkz_active)
            return false;
        return super.CanBeTargetedByAI(ai);
    }

    override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
    {
        if (rpc_type == deutschz_afkz_rpc.state && !GetGame().IsDedicatedServer())
        {
            Param2<bool, float> state;
            if (ctx.Read(state))
            {
                deutschz_afkz_client_state.active = state.param1;
                deutschz_afkz_client_state.sound_volume = Math.Clamp(state.param2, 0.0, 1.0);
                deutschz_afkz_client_state.pending = true;
            }
            return;
        }
        if (rpc_type == deutschz_afkz_rpc.activity && GetGame().IsServer())
        {
            deutschz_afkz_manager.get().activity(this);
            return;
        }
        super.OnRPC(sender, rpc_type, ctx);
    }

    override void EEDelete(EntityAI parent)
    {
        if (GetGame().IsServer())
            deutschz_afkz_manager.get().remove(this);
        super.EEDelete(parent);
    }
};
