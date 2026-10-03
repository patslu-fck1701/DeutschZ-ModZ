class DZWelcomeZPlayerState
{
    int Version = DZWelcomeZConst.STATE_VERSION;
    bool FirstJoinCompleted;
    bool BookHintSeen;
    bool RadioHintSeen;
    bool TutorialCompleted;
    bool PendingRespawn;
    bool StorageHealthy = true;
}

class DZWelcomeZToxicState
{
    int Version;
    int State;
    string OwnerId;
    vector HospitalOne;
    vector HospitalTwo;
    bool FirstSquadDefeated;
    bool SecondSquadDefeated;
    bool RiffySquadDefeated;
    bool RewardGranted;
}

class DZWelcomeZServer
{
    protected static const string ROOT = "$profile:DeutschZ-System/deutschz_welcomez";
    protected static const string STORY_ROOT = "$profile:DeutschZ-System/deutschz_radiomissionz/event_completions";
    protected static const string TOXIC_STATE = "$profile:DeutschZ/ToxicZ/ToxicZState.json";
    protected static ref map<string, bool> s_SeenThisSession;
    protected static ref map<string, bool> s_MusicPlayedThisSession;
    protected static ref map<string, int> s_NextRequestAt;

    static void Init()
    {
        if (!s_SeenThisSession) s_SeenThisSession = new map<string, bool>;
        if (!s_MusicPlayedThisSession) s_MusicPlayedThisSession = new map<string, bool>;
        if (!s_NextRequestAt) s_NextRequestAt = new map<string, int>;
        MakeDirectory("$profile:DeutschZ-System");
        MakeDirectory(ROOT);
        MakeDirectory(ROOT + "/players");
    }

    static void HandleJoin(PlayerBase player, PlayerIdentity identity)
    {
        if (!GetGame() || !GetGame().IsServer() || !player || !identity) return;
        Init();
        string uid = identity.GetPlainId();
        DZWelcomeZPlayerState state;
        bool existed = Load(uid, state);
        int context = DZWelcomeZJoinContext.FIRST_EVER_JOIN;
        if (existed)
        {
            if (state.PendingRespawn) context = DZWelcomeZJoinContext.RESPAWN_AFTER_DEATH;
            else if (s_SeenThisSession.Contains(uid)) context = DZWelcomeZJoinContext.NORMAL_RECONNECT;
            else context = DZWelcomeZJoinContext.POST_RESTART_JOIN;
        }
        state.PendingRespawn = false;
        if (state.StorageHealthy) Save(uid, state);
        s_SeenThisSession.Set(uid, true);
        Send(player, state, context, false);
    }

    static void HandleManualRequest(PlayerBase player, PlayerIdentity sender)
    {
        if (!ValidSender(player, sender)) return;
        Init();
        string uid = sender.GetPlainId();
        int now = GetGame().GetTime();
        int nextRequest;
        if (s_NextRequestAt.Find(uid, nextRequest) && now < nextRequest) return;
        s_NextRequestAt.Set(uid, now + 1000);
        DZWelcomeZPlayerState state;
        Load(uid, state);
        Send(player, state, DZWelcomeZJoinContext.RETURNING_PLAYER, true);
    }

    static void AcknowledgeTutorial(PlayerBase player, PlayerIdentity sender)
    {
        if (!ValidSender(player, sender)) return;
        DZWelcomeZPlayerState state;
        Load(sender.GetPlainId(), state);
        if (!state.StorageHealthy) return;
        state.FirstJoinCompleted = true;
        state.BookHintSeen = true;
        state.RadioHintSeen = true;
        state.TutorialCompleted = true;
        Save(sender.GetPlainId(), state);
    }

    static void MarkDeath(PlayerBase player)
    {
        if (!GetGame() || !GetGame().IsServer() || !player || !player.GetIdentity()) return;
        DZWelcomeZPlayerState state;
        Load(player.GetIdentity().GetPlainId(), state);
        if (!state.StorageHealthy) return;
        state.PendingRespawn = true;
        Save(player.GetIdentity().GetPlainId(), state);
    }

    protected static bool ValidSender(PlayerBase player, PlayerIdentity sender)
    {
        return GetGame() && GetGame().IsServer() && player && sender && player.GetIdentity() == sender;
    }

    protected static void Send(PlayerBase player, DZWelcomeZPlayerState state, int context, bool manual)
    {
        string uid = player.GetIdentity().GetPlainId();
        bool tutorial = !state.TutorialCompleted || manual;
        bool music = !s_MusicPlayedThisSession.Contains(uid);
        if (music) s_MusicPlayedThisSession.Set(uid, true);
        DZWelcomeZPayload payload = BuildPayload(uid, context, tutorial, music);
        ScriptRPC rpc = new ScriptRPC;
        rpc.Write(payload.Context);
        rpc.Write(payload.ShowTutorial);
        rpc.Write(payload.PlayMusic);
        rpc.Write(payload.Heading);
        rpc.Write(payload.CurrentStatus);
        rpc.Write(payload.NextStep);
        rpc.Write(payload.Progress);
        rpc.Write(payload.Detail);
        rpc.Write(payload.EventStatus);
        rpc.Write(payload.EventNext);
        rpc.Write(payload.StoryStatus);
        rpc.Send(player, DZWelcomeZConst.RPC_RESPONSE, true, player.GetIdentity());
        if (!manual && context == DZWelcomeZJoinContext.FIRST_EVER_JOIN)
            NotificationSystem.SendNotificationToPlayerExtended(player, 10.0, "DEUTSCHZ", "Willkommen in Chernarus. Dein Weg beginnt hier. B öffnet dein DeutschZ-Buch. RadioMissionZ: 89,5 MHz.");
        else if (!manual && context == DZWelcomeZJoinContext.RESPAWN_AFTER_DEATH)
            NotificationSystem.SendNotificationToPlayerExtended(player, 8.0, "WILLKOMMEN ZURÜCK", "Dein Missionsfortschritt wurde geladen. Weitere Informationen: B.");
    }

    protected static DZWelcomeZPayload BuildPayload(string uid, int context, bool tutorial, bool music)
    {
        DZWelcomeZPayload result = new DZWelcomeZPayload;
        result.Context = context; result.ShowTutorial = tutorial; result.PlayMusic = music;
        result.EventStatus = "WELTEVENTS (ROTATION)\n\nEVENTSTATUS NICHT VERFUEGBAR";
        result.EventNext = "";
        result.StoryStatus = "STORYEVENT: NICHT VERFÜGBAR";
        ref array<string> schedulerStatus = new array<string>;
        int schedulerCall = g_Game.GameScript.CallFunctionParams(null, "DZES_FillWelcomeStatus", null, new Param1<ref array<string>>(schedulerStatus));
        if (schedulerCall && schedulerStatus.Count() >= 3)
        {
            result.EventStatus = "WELTEVENTS (ROTATION)\n\n" + schedulerStatus[0];
            result.EventNext = schedulerStatus[1];
            result.StoryStatus = schedulerStatus[2] + "\n\nNEBENAKTIVITAETEN\nATM RaidZ\nPropertyZ\nRadioMissionZ";
        }
        ref array<string> rbmStatus = new array<string>;
        int rbmCall = g_Game.GameScript.CallFunctionParams(null, "DZRB_FillWelcomeStatus", null, new Param1<ref array<string>>(rbmStatus));
        if (rbmCall && rbmStatus.Count() > 0) result.EventStatus = result.EventStatus + "\n\n" + rbmStatus[0];
        if (context == DZWelcomeZJoinContext.FIRST_EVER_JOIN) result.Heading = "WILLKOMMEN BEI DEUTSCHZ";
        else if (context == DZWelcomeZJoinContext.RESPAWN_AFTER_DEATH) result.Heading = "WILLKOMMEN ZURÜCK";
        else result.Heading = "DEUTSCHZ PLAYER-HUB";
        bool koth = Done("koth", uid); bool convoy = Done("convoy", uid); bool combined = Done("combined", uid);
        bool toxicStarted = Done("toxic_started", uid); bool toxic = Done("toxic", uid); bool atm = Done("atm", uid);
        bool courier = Done("courier", uid); bool battleground = Done("battleground", uid); bool operation = Done("operation", uid);
        result.Progress = Mark(koth, "King of the HillZ") + "\n" + Mark(convoy, "ConvoyZ") + "\n" + Mark(combined, "Signal entschlüsselt") + "\n" + Mark(toxicStarted, "ToxicZ aktiviert") + "\n" + Mark(toxic, "ToxicZ abgeschlossen") + "\n" + Mark(atm, "ATMRaideZ") + "\n" + Mark(courier, "CourierZ") + "\n" + Mark(battleground, "BattlegroundZ") + "\n" + Mark(operation, "Operation DeutschZ");
        DZWelcomeZToxicState toxicState;
        if (LoadToxicState(toxicState) && toxicState.OwnerId == uid && toxicState.State > 0 && toxicState.State < 10) BuildToxicStep(toxicState.State, result);
        else if (!koth) { result.CurrentStatus = "WOHER KOMMST DU?"; result.NextStep = "Höre auf 89,5 MHz und gewinne King of the HillZ.\nSichere das erste T-17-Dokument."; }
        else if (!convoy) { result.CurrentStatus = "GEHEIMES DOKUMENT GEFUNDEN"; result.NextStep = "Halte Ausschau nach ConvoyZ.\nTransport Sieben könnte den passenden Decoder führen."; }
        else if (!combined) { result.CurrentStatus = "ZWEI TEILE EINER SPUR"; result.NextStep = "Kombiniere ToxicZ Secret Document und ToxicZ Document Decoder."; }
        else if (!toxicStarted) { result.CurrentStatus = "SIGNAL GEFUNDEN"; result.NextStep = "Aktiviere den ToxicZ Signal Marker und achte auf die nächste Übertragung."; }
        else if (!toxic) { result.CurrentStatus = "AKTUELLE OPERATION: TOXICZ"; result.NextStep = "Folge dem aktiven ToxicZ-Marker und den Funksprüchen.\nDer Serverzustand bestimmt das aktuelle Ziel."; }
        else if (!atm) { result.CurrentStatus = "TOXICZ ABGESCHLOSSEN"; result.NextStep = "Folge der Geldspur zum nächsten ATMRaideZ-Ereignis."; }
        else if (!courier) { result.CurrentStatus = "DIE GELDSPUR"; result.NextStep = "Finde den wissenschaftlichen CourierZ-Transport."; }
        else if (!battleground) { result.CurrentStatus = "DER ROTE SEKTOR"; result.NextStep = "Betritt BattlegroundZ und sichere den Operationscode."; }
        else if (!operation) { result.CurrentStatus = "LETZTER ZUGANG"; result.NextStep = "Nutze die Operations-KeyCard bei Operation DeutschZ."; }
        else { result.CurrentStatus = "STORYABSCHNITT ABGESCHLOSSEN"; result.NextStep = "Bleib auf 89,5 MHz.\nNicht jede Übertragung ist das, was sie vorgibt zu sein."; }
        if (tutorial) result.Detail = "WILLKOMMEN AUF DEUTSCHZ\nViel Spaß auf unserem Server!\n\nDEIN BUCH  [ B ]\nÖffne mit B Hinweise, Missionen und Storyinformationen.\n\nRADIO MISSIONZ  89,5 MHz\nAchte auf Funksprüche, Missionsmarker und Fragezeichen.\n\nWICHTIG\nAuch in der Willkommensmusik sind Hinweise versteckt.\nHöre genau hin und bewahre ungewöhnliche Missionsgegenstände auf.\n\nKing of the HillZ, ConvoyZ, BattlegroundZ, ToxicZ\nund Operation DeutschZ gehören zu deiner Geschichte.";
        else result.Detail = "RADIO MISSIONZ: 89,5 MHz\nDEUTSCHZ-BUCH: [ B ]\n\nAuch in der Musik sind Hinweise versteckt.\nHöre genau hin!\n\nViel Spaß auf DeutschZ!";
        return result;
    }

    protected static string FormatCountdown(int seconds)
    {
        if (seconds < 0) seconds = 0;
        int hours = seconds / 3600;
        int minutes = (seconds % 3600) / 60;
        int remaining = seconds % 60;
        return string.Format("%1:%2:%3", PadTime(hours), PadTime(minutes), PadTime(remaining));
    }

    protected static string PadTime(int value)
    {
        if (value < 10) return "0" + value.ToString();
        return value.ToString();
    }

    protected static void BuildToxicStep(int state, DZWelcomeZPayload result)
    {
        result.CurrentStatus = "AKTUELLE OPERATION: TOXICZ";
        switch (state)
        {
            case 1: result.NextStep = "Untersuche die aktivierte Signalquelle."; break;
            case 2: result.NextStep = "Suche das erste markierte Hospital und sichere Morozovs Akte."; break;
            case 3: result.NextStep = "Erreiche das zweite Hospital und vervollständige deine NBC-Ausrüstung."; break;
            case 4: result.NextStep = "Bereite deine NBC-Ausrüstung vor und nähere dich Riffy."; break;
            case 5: result.NextStep = "Überlebe den Kampf bei Riffy und untersuche die Signalquelle."; break;
            case 6: result.NextStep = "Berge die Blackbox von Transport Sieben."; break;
            case 7: result.NextStep = "Bringe Blackbox und Decoder zur Decoderstation."; break;
            case 8: result.NextStep = "Warte auf die letzte Übertragung aus Protokoll T-17."; break;
            default: result.NextStep = "Folge dem aktiven ToxicZ-Marker und den aktuellen Funksprüchen."; break;
        }
    }

    protected static string Mark(bool done, string label)
    {
        if (done)
            return "✓ " + label;
        else
            return "→ " + label;
    }

    protected static bool Done(string eventId, string uid)
    {
        bool exists = FileExist(STORY_ROOT + "/" + eventId + "/" + uid + ".done");
        return exists;
    }
    protected static bool LoadToxicState(out DZWelcomeZToxicState state)
    {
        state = null; if (!FileExist(TOXIC_STATE)) return false; string error;
        if (!JsonFileLoader<DZWelcomeZToxicState>.LoadFile(TOXIC_STATE, state, error) || !state || state.Version != 1) { Print("[WelcomeZ] ToxicZ-Zustand nicht lesbar: " + error); return false; }
        return true;
    }
    protected static bool Load(string uid, out DZWelcomeZPlayerState state)
    {
        Init(); state = new DZWelcomeZPlayerState; string path = ROOT + "/players/" + uid + ".json"; if (!FileExist(path)) return false;
        string error; DZWelcomeZPlayerState loaded;
        if (!JsonFileLoader<DZWelcomeZPlayerState>.LoadFile(path, loaded, error) || !loaded) { state.StorageHealthy = false; state.TutorialCompleted = true; Print("[WelcomeZ] Spielerzustand beschädigt; Datei bleibt unverändert. UID=" + uid + " Fehler=" + error); return true; }
        if (loaded.Version < 1 || loaded.Version > DZWelcomeZConst.STATE_VERSION) { state.StorageHealthy = false; state.TutorialCompleted = true; Print("[WelcomeZ] Nicht unterstützte Zustandsversion; Datei bleibt unverändert. UID=" + uid); return true; }
        loaded.Version = DZWelcomeZConst.STATE_VERSION; state = loaded; return true;
    }
    protected static void Save(string uid, DZWelcomeZPlayerState state)
    {
        if (!state) return; Init(); state.Version = DZWelcomeZConst.STATE_VERSION; string error;
        if (!JsonFileLoader<DZWelcomeZPlayerState>.SaveFile(ROOT + "/players/" + uid + ".json", state, error)) Print("[WelcomeZ] ERROR Zustand nicht gespeichert. UID=" + uid + " Fehler=" + error);
    }
}

modded class PlayerBase
{
    override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
    {
        if (rpc_type == DZWelcomeZConst.RPC_RESPONSE) { if (GetGame() && !GetGame().IsServer()) DZWelcomeZClientState.Receive(ctx); return; }
        if (rpc_type == DZWelcomeZConst.RPC_REQUEST) { if (GetGame() && GetGame().IsServer()) DZWelcomeZServer.HandleManualRequest(this, sender); return; }
        if (rpc_type == DZWelcomeZConst.RPC_TUTORIAL_ACK) { if (GetGame() && GetGame().IsServer()) DZWelcomeZServer.AcknowledgeTutorial(this, sender); return; }
        super.OnRPC(sender, rpc_type, ctx);
    }
    override void EEKilled(Object killer)
    {
        if (GetGame() && GetGame().IsServer()) DZWelcomeZServer.MarkDeath(this);
        super.EEKilled(killer);
    }
}
