modded class MissionGameplay
{
    void MissionGameplay()
    {
        GetRPCManager().AddRPC(deutschz_aiconvoyz_rpc.namespace_name, deutschz_aiconvoyz_rpc.crash_sound_method, this, SingleplayerExecutionType.Client);
        GetRPCManager().AddRPC(deutschz_aiconvoyz_rpc.namespace_name, deutschz_aiconvoyz_rpc.combat_sound_method, this, SingleplayerExecutionType.Client);
    }

    void play_crash_sound(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type != CallType.Client) return;
        Param2<vector, float> data;
        if (!ctx.Read(data)) return;
        EffectSound sound = SEffectManager.PlaySound("HeliCrash_Distant_SoundSet", data.param1);
        if (sound) sound.SetSoundVolume(data.param2);
    }

    void play_combat_sound(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type != CallType.Client) return;
        Param3<vector, float, int> data;
        if (!ctx.Read(data)) return;
        string soundSet = "M4A1_Shot_SoundSet";
        if (data.param3 == 1) soundSet = "AKM_Shot_SoundSet";
        EffectSound sound = SEffectManager.PlaySound(soundSet, data.param1);
        if (sound) sound.SetSoundVolume(data.param2);
    }
}
