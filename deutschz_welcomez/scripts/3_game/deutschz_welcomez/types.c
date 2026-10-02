class DZWelcomeZConst
{
    static const int RPC_REQUEST = 91759501;
    static const int RPC_RESPONSE = 91759502;
    static const int RPC_TUTORIAL_ACK = 91759503;
    static const int STATE_VERSION = 2;
}

enum DZWelcomeZJoinContext
{
    FIRST_EVER_JOIN,
    NORMAL_RECONNECT,
    POST_RESTART_JOIN,
    RESPAWN_AFTER_DEATH,
    RETURNING_PLAYER
}

class DZWelcomeZPayload
{
    int Context;
    bool ShowTutorial;
    bool PlayMusic;
    string Heading;
    string CurrentStatus;
    string NextStep;
    string Progress;
    string Detail;
    string EventStatus;
    string EventNext;
    string StoryStatus;
}

class DZWelcomeZClientState
{
    static ref DZWelcomeZPayload Payload;
    static int Revision;

    static void Receive(ParamsReadContext ctx)
    {
        Payload = new DZWelcomeZPayload;
        if (!ctx.Read(Payload.Context) || !ctx.Read(Payload.ShowTutorial) || !ctx.Read(Payload.PlayMusic) || !ctx.Read(Payload.Heading) || !ctx.Read(Payload.CurrentStatus) || !ctx.Read(Payload.NextStep) || !ctx.Read(Payload.Progress) || !ctx.Read(Payload.Detail) || !ctx.Read(Payload.EventStatus) || !ctx.Read(Payload.EventNext) || !ctx.Read(Payload.StoryStatus))
        {
            Payload = null;
            return;
        }
        Revision++;
    }
}
