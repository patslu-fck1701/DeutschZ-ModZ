modded class MissionServer
{
    override void OnInit()
    {
        super.OnInit();
        if (!g_DZToxicZ) g_DZToxicZ=new DZToxicZController;
        g_DZToxicZ.Start();
    }
    override void OnMissionFinish()
    {
        if (g_DZToxicZ) g_DZToxicZ.Stop();
        super.OnMissionFinish();
    }
}
modded class MissionGameplay
{
    void MissionGameplay()
    {
        GetRPCManager().AddRPC(DZToxicZRPC.NS,DZToxicZRPC.AUDIO,this,SingleplayerExecutionType.Client);
    }
    void Audio(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type != CallType.Client) return;
        Param1<string> data;
        if (!ctx.Read(data)) return;
        DZToxicZAudio.Play(data.param1);
    }
}
