class DZATM_Denomination
{
    string ClassName;
    int Value;

    void DZATM_Denomination(string className = "", int value = 0)
    {
        ClassName = className;
        Value = value;
    }
}

class DZATM_CooldownEntry
{
    string TargetId;
    int ExpiresAtUTC;
    // Version-1 migration only. New files use ExpiresAtUTC.
    float RemainingSeconds;
}

class DZATM_CooldownFile
{
    int Version = 2;
    ref array<ref DZATM_CooldownEntry> Entries;

    void DZATM_CooldownFile()
    {
        Entries = new array<ref DZATM_CooldownEntry>;
    }
}

class DZATM_ClientProgressState
{
    static bool Visible;
    static float Progress;
    static float Duration;
    static string Label;
    static string Message;
    static int MessageUntil;

    static void SetProgress(bool visible, float progress, float duration, string label)
    {
        Visible = visible;
        Progress = progress;
        Duration = duration;
        Label = label;
    }

    static void ShowMessage(string message, int durationMs)
    {
        Message = message;
        MessageUntil = GetGame().GetTime() + durationMs;
    }
}

class DZATM_ClientEffectEvent
{
    bool Start;
    string EffectId;
    string SoundSet;
    vector Position;
    bool Loop;
}

class DZATM_ClientEffectQueue
{
    static ref array<ref DZATM_ClientEffectEvent> Events = new array<ref DZATM_ClientEffectEvent>;

    static void Push(bool start, string effectId, string soundSet, vector position, bool loop)
    {
        DZATM_ClientEffectEvent eventData = new DZATM_ClientEffectEvent;
        eventData.Start = start;
        eventData.EffectId = effectId;
        eventData.SoundSet = soundSet;
        eventData.Position = position;
        eventData.Loop = loop;
        Events.Insert(eventData);
    }
}

class DZATM_AdminStatus
{
    static int ActiveSessions;
    static int RegisteredATMs;
    static bool DebugEnabled;
    static string LastMessage;

    static void Update(int sessions, int atms, bool debugEnabled, string message)
    {
        ActiveSessions = sessions;
        RegisteredATMs = atms;
        DebugEnabled = debugEnabled;
        LastMessage = message;
    }
}
