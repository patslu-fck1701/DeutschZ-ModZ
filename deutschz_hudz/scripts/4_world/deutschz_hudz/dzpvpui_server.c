class DZPVPUI_LeaderboardEntry
{
    string Uid;
    string Name;
    int PvpKills;
    int InfectedKills;
    int Deaths;

    int Score()
    {
        return (PvpKills * 10) + InfectedKills - Deaths;
    }
}

class DZPVPUI_LeaderboardFile
{
    int Version = 1;
    ref array<ref DZPVPUI_LeaderboardEntry> Players = new array<ref DZPVPUI_LeaderboardEntry>;
}

class DZPVPUI_ServerRepository
{
    protected static ref DZPVPUI_ServerRepository s_Instance;
    protected ref DZPVPUI_LeaderboardFile m_Leaderboard;

    static DZPVPUI_ServerRepository Get()
    {
        if (!s_Instance)
            s_Instance = new DZPVPUI_ServerRepository;
        return s_Instance;
    }

    static void Reset()
    {
        s_Instance = null;
    }

    void DZPVPUI_ServerRepository()
    {
        MakeDirectory("$profile:DeutschZ-System");
        MakeDirectory(DZPVPUI_Constants.PROFILE_DIR);
        LoadSettings();
        LoadLeaderboard();
    }

    protected void LoadSettings()
    {
        ref DZPVPUI_SettingsFile file = new DZPVPUI_SettingsFile;
        string error;
        if (FileExist(DZPVPUI_Constants.SETTINGS_PATH))
            JsonFileLoader<DZPVPUI_SettingsFile>.LoadFile(DZPVPUI_Constants.SETTINGS_PATH, file, error);
        if (!file.Free)
            file.Free = new DZPVPUI_SettingsScope;
        if (!file.Free.Global)
            file.Free.Global = new DZPVPUI_SettingsGlobal;
        file.Version = 1;
        file.Free.Global.Validate();
        DZPVPUI_State.Settings = file.Free.Global;
        JsonFileLoader<DZPVPUI_SettingsFile>.SaveFile(DZPVPUI_Constants.SETTINGS_PATH, file, error);
    }

    protected void LoadLeaderboard()
    {
        m_Leaderboard = new DZPVPUI_LeaderboardFile;
        string error;
        if (FileExist(DZPVPUI_Constants.LEADERBOARD_PATH))
            JsonFileLoader<DZPVPUI_LeaderboardFile>.LoadFile(DZPVPUI_Constants.LEADERBOARD_PATH, m_Leaderboard, error);
        if (!m_Leaderboard || !m_Leaderboard.Players)
            m_Leaderboard = new DZPVPUI_LeaderboardFile;
    }

    protected DZPVPUI_LeaderboardEntry GetOrCreate(PlayerBase player)
    {
        if (!player || !player.GetIdentity())
            return null;
        string uid = player.GetIdentity().GetId();
        foreach (DZPVPUI_LeaderboardEntry entry : m_Leaderboard.Players)
        {
            if (entry && entry.Uid == uid)
            {
                entry.Name = player.GetIdentity().GetName();
                return entry;
            }
        }
        DZPVPUI_LeaderboardEntry created = new DZPVPUI_LeaderboardEntry;
        created.Uid = uid;
        created.Name = player.GetIdentity().GetName();
        m_Leaderboard.Players.Insert(created);
        return created;
    }

    protected PlayerBase ResolvePlayer(Object killer)
    {
        PlayerBase player = PlayerBase.Cast(killer);
        EntityAI entity = EntityAI.Cast(killer);
        if (!player && entity)
            player = PlayerBase.Cast(entity.GetHierarchyRootPlayer());
        return player;
    }

    void RecordInfectedKill(Object killer)
    {
        DZPVPUI_LeaderboardEntry entry = GetOrCreate(ResolvePlayer(killer));
        if (!entry)
            return;
        entry.InfectedKills++;
        Save();
    }

    void RecordPlayerDeath(PlayerBase victim, Object killer)
    {
        DZPVPUI_LeaderboardEntry victimEntry = GetOrCreate(victim);
        if (victimEntry)
            victimEntry.Deaths++;
        PlayerBase playerKiller = ResolvePlayer(killer);
        if (playerKiller && playerKiller != victim)
        {
            DZPVPUI_LeaderboardEntry killerEntry = GetOrCreate(playerKiller);
            if (killerEntry)
                killerEntry.PvpKills++;
        }
        Save();
    }

    protected void Save()
    {
        string error;
        JsonFileLoader<DZPVPUI_LeaderboardFile>.SaveFile(DZPVPUI_Constants.LEADERBOARD_PATH, m_Leaderboard, error);
    }

    protected DZPVPUI_LeaderboardEntry FindBest(array<string> used)
    {
        DZPVPUI_LeaderboardEntry best;
        int bestScore = -2147483647;
        foreach (DZPVPUI_LeaderboardEntry entry : m_Leaderboard.Players)
        {
            if (!entry || used.Find(entry.Uid) >= 0)
                continue;
            if (!best || entry.Score() > bestScore)
            {
                best = entry;
                bestScore = entry.Score();
            }
        }
        return best;
    }

    string BuildLeaderboardWinner()
    {
        ref array<string> used = new array<string>;
        DZPVPUI_LeaderboardEntry best = FindBest(used);
        if (!best)
            return "";
        return best.Name + "    PVP " + best.PvpKills.ToString() + "    INF " + best.InfectedKills.ToString() + "    TOD " + best.Deaths.ToString() + "    SCORE " + best.Score().ToString();
    }

    string BuildLeaderboardSummary()
    {
        ref array<string> used = new array<string>;
        DZPVPUI_LeaderboardEntry best = FindBest(used);
        if (!best)
            return "";
        return best.InfectedKills.ToString() + "|" + best.PvpKills.ToString() + "|" + best.Deaths.ToString() + "|" + best.Score().ToString();
    }

    string BuildLeaderboard(int maximum)
    {
        string result = PadRight("RANG", 6) + PadRight("SPIELER", 24) + PadLeft("PVP", 5) + PadLeft("INF", 5) + PadLeft("TOD", 5) + PadLeft("SCORE", 7);
        ref array<string> used = new array<string>;
        for (int rank = 1; rank <= maximum; rank++)
        {
            DZPVPUI_LeaderboardEntry best = FindBest(used);
            if (!best)
                break;
            used.Insert(best.Uid);
            if (rank > 1)
            {
                string rankText = rank.ToString() + ".";
                result = result + "\n" + PadRight(rankText, 6) + PadRight(best.Name, 24) + PadLeft(best.PvpKills.ToString(), 5) + PadLeft(best.InfectedKills.ToString(), 5) + PadLeft(best.Deaths.ToString(), 5) + PadLeft(best.Score().ToString(), 7);
            }
        }
        return result;
    }

    protected string PadRight(string value, int width)
    {
        while (value.Length() < width)
            value = value + " ";
        return value;
    }

    protected string PadLeft(string value, int width)
    {
        while (value.Length() < width)
            value = " " + value;
        return value;
    }

    int GetRestartSeconds()
    {
        int hour;
        int minute;
        int second;
        GetHourMinuteSecond(hour, minute, second);
        int now = (hour * 3600) + (minute * 60) + second;
        ref array<int> restarts = {3600, 25200, 46800, 68400};
        foreach (int restart : restarts)
        {
            if (now < restart)
                return restart - now;
        }
        return (86400 - now) + 3600;
    }

	int GetAccountBalance(PlayerIdentity identity)
	{
#ifdef EXPANSIONMODMARKET
		if (!identity)
			return -1;
		ExpansionMarketModule market = ExpansionMarketModule.Cast(CF_ModuleCoreManager.Get(ExpansionMarketModule));
		if (!market)
			return -1;
		ExpansionMarketATM_Data data = market.GetPlayerATMData(identity.GetId());
		if (!data)
			return -1;
		return data.MoneyDeposited;
#endif
		return -1;
	}
}

modded class PlayerBase
{
    protected int m_DZPVPUI_LastLeaderboardRequest;
    protected int m_DZPVPUI_LastStatusRequest;

    void DZPVPUI_RequestLeaderboard()
    {
        if (!GetGame() || GetGame().IsServer())
            return;
        ScriptRPC rpc = new ScriptRPC;
        rpc.Write(DZPVPUI_Constants.PROTOCOL);
        rpc.Send(this, DZPVPUI_Constants.RPC_LEADERBOARD_REQUEST, true, null);
    }

    void DZPVPUI_RequestStatus()
    {
        if (!GetGame() || GetGame().IsServer())
            return;
        ScriptRPC rpc = new ScriptRPC;
        rpc.Write(DZPVPUI_Constants.PROTOCOL);
        rpc.Send(this, DZPVPUI_Constants.RPC_STATUS_REQUEST, false, null);
    }

    override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
    {
        if (rpc_type == DZPVPUI_Constants.RPC_STATUS_REQUEST)
        {
            if (!GetGame() || !GetGame().IsServer() || !sender || !GetIdentity() || sender.GetId() != GetIdentity().GetId())
                return;
            int statusProtocol;
            if (!ctx.Read(statusProtocol) || statusProtocol != DZPVPUI_Constants.PROTOCOL)
                return;
            int statusNow = GetGame().GetTime();
            if (m_DZPVPUI_LastStatusRequest > 0 && statusNow - m_DZPVPUI_LastStatusRequest < 750)
                return;
            m_DZPVPUI_LastStatusRequest = statusNow;
            ScriptRPC statusResponse = new ScriptRPC;
            statusResponse.Write(DZPVPUI_Constants.PROTOCOL);
            statusResponse.Write(Math.Round(Math.Clamp(GetHealth01("", "Health"), 0.0, 1.0) * 100.0));
            statusResponse.Write(Math.Round(Math.Clamp(GetHealth01("", "Blood"), 0.0, 1.0) * 100.0));
            float energyMaximum = Math.Max(GetStatEnergy().GetMax(), 1.0);
            float waterMaximum = Math.Max(GetStatWater().GetMax(), 1.0);
            statusResponse.Write(Math.Round(Math.Clamp(GetStatEnergy().Get() / energyMaximum, 0.0, 1.0) * 100.0));
            statusResponse.Write(Math.Round(Math.Clamp(GetStatWater().Get() / waterMaximum, 0.0, 1.0) * 100.0));
            statusResponse.Send(this, DZPVPUI_Constants.RPC_STATUS_RESPONSE, false, sender);
            return;
        }
        if (rpc_type == DZPVPUI_Constants.RPC_STATUS_RESPONSE)
        {
            if (!GetGame() || GetGame().IsServer())
                return;
            int statusResponseProtocol;
            int healthPercent;
            int bloodPercent;
            int energyPercent;
            int waterPercent;
            if (ctx.Read(statusResponseProtocol) && statusResponseProtocol == DZPVPUI_Constants.PROTOCOL && ctx.Read(healthPercent) && ctx.Read(bloodPercent) && ctx.Read(energyPercent) && ctx.Read(waterPercent))
            {
                DZPVPUI_State.HealthPercent = Math.Clamp(healthPercent, 0, 100);
                DZPVPUI_State.BloodPercent = Math.Clamp(bloodPercent, 0, 100);
                DZPVPUI_State.EnergyPercent = Math.Clamp(energyPercent, 0, 100);
                DZPVPUI_State.WaterPercent = Math.Clamp(waterPercent, 0, 100);
            }
            return;
        }
        if (rpc_type == DZPVPUI_Constants.RPC_LEADERBOARD_REQUEST)
        {
            if (!GetGame() || !GetGame().IsServer() || !sender || !GetIdentity() || sender.GetId() != GetIdentity().GetId())
                return;
            int protocol;
            if (!ctx.Read(protocol) || protocol != DZPVPUI_Constants.PROTOCOL)
                return;
            int now = GetGame().GetTime();
            if (m_DZPVPUI_LastLeaderboardRequest > 0 && now - m_DZPVPUI_LastLeaderboardRequest < 1000)
                return;
            m_DZPVPUI_LastLeaderboardRequest = now;
            ScriptRPC response = new ScriptRPC;
            response.Write(DZPVPUI_Constants.PROTOCOL);
            response.Write(DZPVPUI_ServerRepository.Get().BuildLeaderboardWinner());
            response.Write(DZPVPUI_ServerRepository.Get().BuildLeaderboardSummary());
            response.Write(DZPVPUI_ServerRepository.Get().BuildLeaderboard(15));
            response.Write(DZPVPUI_ServerRepository.Get().GetRestartSeconds());
            response.Write(DZPVPUI_ServerRepository.Get().GetAccountBalance(sender));
            response.Send(this, DZPVPUI_Constants.RPC_LEADERBOARD_RESPONSE, true, sender);
            return;
        }
        if (rpc_type == DZPVPUI_Constants.RPC_LEADERBOARD_RESPONSE)
        {
            if (!GetGame() || GetGame().IsServer())
                return;
            int responseProtocol;
            string winnerText;
            string summaryText;
            string text;
            int restartSeconds;
            int accountBalance;
            if (ctx.Read(responseProtocol) && responseProtocol == DZPVPUI_Constants.PROTOCOL && ctx.Read(winnerText) && ctx.Read(summaryText) && ctx.Read(text) && ctx.Read(restartSeconds) && ctx.Read(accountBalance))
            {
                DZPVPUI_State.LeaderboardWinnerText = winnerText;
                DZPVPUI_State.LeaderboardSummary = summaryText;
                DZPVPUI_State.LeaderboardText = text;
                DZPVPUI_State.RestartSeconds = restartSeconds;
                DZPVPUI_State.RestartSyncTime = GetGame().GetTime();
                DZPVPUI_State.AccountBalance = accountBalance;
                string playerUid;
                if (GetIdentity())
                    playerUid = GetIdentity().GetId();
                DZPVPUI_ClientDisplayCacheStore.SaveServerSnapshot(playerUid);
            }
            return;
        }
        super.OnRPC(sender, rpc_type, ctx);
    }

    override void EEKilled(Object killer)
    {
        if (GetGame() && GetGame().IsServer())
            DZPVPUI_ServerRepository.Get().RecordPlayerDeath(this, killer);
        super.EEKilled(killer);
    }
}

modded class ZombieBase
{
    override void EEKilled(Object killer)
    {
        if (GetGame() && GetGame().IsServer())
            DZPVPUI_ServerRepository.Get().RecordInfectedKill(killer);
        super.EEKilled(killer);
    }
}
