class DZATM_RPC
{
    static void ReceiveProgress(ParamsReadContext ctx)
    {
        bool visible;
        float progress;
        float duration;
        string label;
        if (!ctx.Read(visible) || !ctx.Read(progress) || !ctx.Read(duration) || !ctx.Read(label))
            return;
        DZATM_ClientProgressState.SetProgress(visible, progress, duration, label);
    }

    static void ReceiveMessage(ParamsReadContext ctx)
    {
        string message;
        int durationMs;
        if (!ctx.Read(message) || !ctx.Read(durationMs))
            return;
        DZATM_ClientProgressState.ShowMessage(message, durationMs);
    }

    static void ReceiveEffect(ParamsReadContext ctx)
    {
        bool start;
        string effectId;
        string soundSet;
        vector position;
        bool loop;
        if (!ctx.Read(start) || !ctx.Read(effectId) || !ctx.Read(soundSet) || !ctx.Read(position) || !ctx.Read(loop))
            return;
        DZATM_ClientEffectQueue.Push(start, effectId, soundSet, position, loop);
    }

    static void ReceiveAdminStatus(ParamsReadContext ctx)
    {
        int sessions;
        int atms;
        bool debugEnabled;
        string message;
        if (!ctx.Read(sessions) || !ctx.Read(atms) || !ctx.Read(debugEnabled) || !ctx.Read(message))
            return;
        DZATM_AdminStatus.Update(sessions, atms, debugEnabled, message);
    }
}
