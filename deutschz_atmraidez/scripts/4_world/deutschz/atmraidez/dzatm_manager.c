class DZATM_Manager
{
    protected static ref DZATM_Manager s_Instance;
    protected bool m_Initialized;
    protected int m_SessionCounter;
    protected ref DZATM_Config m_Config;
    protected ref map<string, ref DZATM_HackSession> m_Sessions;
    protected ref DZATM_CooldownManager m_Cooldowns;
    protected ref DZATM_RewardProvider m_Rewards;
    protected ref DZATM_MarkerManager m_Markers;
    protected ref DZATM_NotificationManager m_Notifications;
    protected ref DZATM_AlarmManager m_Alarms;
    protected ref DZATM_ATMRegistry m_ATMs;
    protected ref map<string, int> m_AdminLastCommandAt;
    protected ref array<SmokeGrenadeBase> m_SuccessSmokes;

    void DZATM_Manager()
    {
        m_Sessions = new map<string, ref DZATM_HackSession>;
        m_AdminLastCommandAt = new map<string, int>;
        m_SuccessSmokes = new array<SmokeGrenadeBase>;
    }

    static DZATM_Manager GetInstance()
    {
        if (!s_Instance) s_Instance = new DZATM_Manager;
        return s_Instance;
    }

    static void DestroyInstance()
    {
        if (s_Instance) s_Instance.Shutdown();
        s_Instance = null;
    }

    DZATM_Config GetConfig()
    {
        if (!m_Config && GetGame() && GetGame().IsServer())
            m_Config = DZATM_Config.Load();
        return m_Config;
    }

    void Init()
    {
        if (m_Initialized || !GetGame() || !GetGame().IsServer()) return;

        DZATM_ProfilePaths.Ensure();
        m_Config = DZATM_Config.Load();
        m_Cooldowns = new DZATM_CooldownManager(m_Config.General.PersistCooldowns);
        m_Rewards = new DZATM_RewardProvider(m_Config);
        m_Markers = new DZATM_MarkerManager;
        m_Notifications = new DZATM_NotificationManager;
        m_Alarms = new DZATM_AlarmManager;
        m_ATMs = new DZATM_ATMRegistry;

        m_Initialized = true;
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Tick, DZATM_Const.SERVER_TICK_MS, true);
        // Static Expansion ATM objects can be initialized after MissionServer startup.
        // Run more than once; the reuse-radius check prevents duplicate ATMs.
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(EnsureConfiguredATMs, 5000, false);
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(EnsureConfiguredATMs, 15000, false);
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(EnsureConfiguredATMs, 30000, false);
        DZATM_Log.Info("DeutschZ ATM RaideZ " + DZATM_Const.VERSION + " initialisiert. Raid-ATM-Klasse=" + DZATM_Const.ATM_CLASS);
    }

    protected void EnsureConfiguredATMs()
    {
        if (!m_Initialized || !m_Config || !m_Config.Spawns || !m_Config.Spawns.Enabled || !m_Config.Spawns.ATMs) return;

        string atmClass = m_Config.Spawns.ATMClass;
        if (atmClass == "") atmClass = DZATM_Const.ATM_CLASS;

        string cfgPath = "CfgVehicles " + atmClass;
        if (!GetGame().ConfigIsExisting(cfgPath))
        {
            DZATM_Log.Error("ATM-Klasse fehlt in CfgVehicles: " + atmClass);
            return;
        }

        int reused = 0;
        int spawned = 0;
        int failed = 0;

        foreach (DZATM_ATMSpawnEntry entry: m_Config.Spawns.ATMs)
        {
            if (!entry || !entry.Enabled || !entry.IsValid()) continue;

            vector position = entry.GetPosition();
            vector orientation = entry.GetOrientation();
            ExpansionATMBase atm = FindExistingConfiguredATM(position, m_Config.Spawns.ReuseRadiusMeters, atmClass);
            if (atm)
            {
                RegisterATM(atm);
                reused++;
                continue;
            }

            Object created = GetGame().CreateObjectEx(atmClass, position, ECE_SETUP | ECE_CREATEPHYSICS | ECE_UPDATEPATHGRAPH);
            atm = ExpansionATMBase.Cast(created);
            if (!atm)
            {
                if (created) GetGame().ObjectDelete(created);
                failed++;
                DZATM_Log.Error("ATM konnte nicht gespawnt werden class=" + atmClass + " pos=" + position.ToString());
                continue;
            }

            atm.SetPosition(position);
            atm.SetOrientation(orientation);
            atm.Update();
            RegisterATM(atm);
            spawned++;
            DZATM_Log.Info("ATM gespawnt class=" + atmClass + " pos=" + position.ToString() + " ori=" + orientation.ToString());
        }

        DZATM_Log.Info("ATM-Spawnpruefung fertig: class=" + atmClass + " konfiguriert=" + m_Config.Spawns.ATMs.Count().ToString() + " gespawnt=" + spawned.ToString() + " wiederverwendet=" + reused.ToString() + " fehler=" + failed.ToString());
    }

    protected ExpansionATMBase FindExistingConfiguredATM(vector position, float radius, string atmClass)
    {
        array<Object> nearby = new array<Object>;
        array<CargoBase> proxyCargos = new array<CargoBase>;
        GetGame().GetObjectsAtPosition3D(position, radius, nearby, proxyCargos);

        foreach (Object obj: nearby)
        {
            ExpansionATMBase atm = ExpansionATMBase.Cast(obj);
            if (atm && atm.GetType() == atmClass)
                return atm;
        }
        return null;
    }

    void Shutdown()
    {
        if (!m_Initialized) return;
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(Tick);
        CancelAll(DZATM_Const.CANCEL_SERVER_SHUTDOWN);
        if (m_Cooldowns) m_Cooldowns.Save();
        if (m_Markers) m_Markers.RemoveAll();
        if (m_Alarms) m_Alarms.StopAll();
        StopSuccessSmokes();
        m_Initialized = false;
    }

    void RegisterATM(ExpansionATMBase atm)
    {
        if (!m_Initialized) Init();
        if (!m_ATMs || !atm || atm.GetType() != DZATM_Const.ATM_CLASS) return;
        string id = m_ATMs.Register(atm);
        if (m_Config && m_Config.Raid)
            atm.DZATM_SetHackDuration(m_Config.Raid.Phase1HackSeconds);
        bool cooldownActive = m_Cooldowns && m_Cooldowns.IsActive(id);
        atm.DZATM_SetRaidLocked(cooldownActive);
        atm.DZATM_SetRaidActionBlocked(cooldownActive);
    }

    void UnregisterATM(ExpansionATMBase atm)
    {
        if (m_ATMs) m_ATMs.Unregister(atm);
    }

    bool StartRaid(PlayerBase player, ExpansionATMBase atm)
    {
        if (!m_Initialized) Init();
        if (!m_Config || !m_Config.General.Enabled || !DZATM_PlayerUtils.IsValidPlayer(player) || !atm || atm.GetType() != DZATM_Const.ATM_CLASS) return false;

        string targetId = atm.DZATM_GetATMId();
        if (targetId == "") targetId = m_ATMs.Register(atm);

        if (m_Cooldowns.IsActive(targetId))
        {
            m_Notifications.Personal(player, "ATM RaideZ", "Dieser ATM ist noch gesperrt.");
            return false;
        }

        string playerCooldownId = "PLAYER|" + DZATM_PlayerUtils.PlayerId(player);
        if (m_Cooldowns.IsActive(playerCooldownId))
        {
            m_Notifications.Personal(player, "ATM RaideZ", "Du kannst aktuell keinen weiteren ATM ausrauben.");
            return false;
        }

        if (HasPlayerSession(DZATM_PlayerUtils.PlayerId(player))) return false;
        if (m_Config.General.PreventParallelRaidOnSameATM && HasTargetSession(targetId)) return false;
        if (!DZATM_PlayerUtils.IsWithin(player, atm, m_Config.Raid.RequiredRadiusMeters)) return false;

        ItemBase tool = DZATM_PlayerUtils.FindRaidTool(player, m_Config.Raid);
        if (!tool || tool.IsRuined()) return false;

        m_SessionCounter++;
        DZATM_HackSession session = new DZATM_HackSession;
        session.SessionId = "DZATM|" + ExpansionStatic.GetTimestamp(true).ToString() + "|" + m_SessionCounter.ToString();
        session.PlayerId = DZATM_PlayerUtils.PlayerId(player);
        session.TargetId = targetId;
        session.Player = player;
        session.Target = atm;
        session.Tool = tool;
        session.Phase1Seconds = m_Config.Raid.Phase1HackSeconds;
        session.Phase2Seconds = m_Config.Raid.Phase2GuardSeconds;
        session.RequiredRadius = m_Config.Raid.RequiredRadiusMeters;
        session.Start();

        m_Sessions.Set(session.SessionId, session);
        atm.DZATM_SetRaidLocked(true);
        atm.DZATM_SetRaidActionBlocked(false);
        StartPresentation(session);
        DZATM_Log.Info("ATM-Raub gestartet session=" + session.SessionId + " player=" + session.PlayerId + " target=" + targetId);
        return true;
    }

    void CompleteHackPhase(PlayerBase player, ExpansionATMBase atm)
    {
        DZATM_HackSession session = FindSession(player, atm);
        if (!session || session.State != DZATM_Const.STATE_ACTIVE) return;
        if (session.Phase != DZATM_Const.PHASE_HACK) return;
        if (session.GetPhaseElapsedSeconds() + 1.0 < session.Phase1Seconds) return;
        BeginGuardPhase(session);
    }

    void CancelRaid(PlayerBase player, ExpansionATMBase atm, int reason)
    {
        DZATM_HackSession session = FindSession(player, atm);
        if (session) CancelSession(session, reason);
    }

    void CancelPlayer(PlayerBase player, int reason)
    {
        if (!player) return;
        array<DZATM_HackSession> matches = new array<DZATM_HackSession>;
        foreach (string id, DZATM_HackSession session: m_Sessions)
        {
            if (session && session.Player == player) matches.Insert(session);
        }
        foreach (DZATM_HackSession match: matches) CancelSession(match, reason);
    }

    void CancelAll(int reason = DZATM_Const.CANCEL_ADMIN)
    {
        array<DZATM_HackSession> all = new array<DZATM_HackSession>;
        foreach (string id, DZATM_HackSession session: m_Sessions)
        {
            if (session) all.Insert(session);
        }
        foreach (DZATM_HackSession active: all) CancelSession(active, reason);
    }

    void Tick()
    {
        if (!m_Initialized) return;
        m_Cooldowns.Tick(1.0);
        PruneSuccessSmokes();

        array<DZATM_HackSession> invalid = new array<DZATM_HackSession>;
        array<int> reasons = new array<int>;
        array<DZATM_HackSession> completed = new array<DZATM_HackSession>;

        foreach (string id, DZATM_HackSession session: m_Sessions)
        {
            if (!session || session.State != DZATM_Const.STATE_ACTIVE) continue;

            int reason = session.Validate(m_Config);
            if (reason != DZATM_Const.CANCEL_NONE)
            {
                invalid.Insert(session);
                reasons.Insert(reason);
                continue;
            }

            session.UpdateProgress();
            EnsureSmoke(session);

            float duration = session.Phase1Seconds;
            string label = "ATM hacken - Phase 1/2";

            if (session.Phase == DZATM_Const.PHASE_GUARD)
            {
                duration = session.Phase2Seconds;
                label = "ATM sichern - Phase 2/2 (" + session.RequiredRadius.ToString() + "m)";
                if (session.Progress >= 1.0) completed.Insert(session);
            }
            DZATM_ServerRPC.SendProgress(session.Player, true, session.Progress, duration, label);
        }

        for (int i = 0; i < invalid.Count(); i++) CancelSession(invalid[i], reasons[i]);
        foreach (DZATM_HackSession done: completed)
        {
            if (done && done.State == DZATM_Const.STATE_ACTIVE) CompleteRaid(done);
        }
        RefreshATMLocks();
    }

    protected void BeginGuardPhase(DZATM_HackSession session)
    {
        if (!session || session.Phase == DZATM_Const.PHASE_GUARD) return;

        session.BeginGuardPhase();
        m_Cooldowns.Set(session.TargetId, m_Config.Raid.ATMCooldownSeconds);
        m_Cooldowns.Set("PLAYER|" + session.PlayerId, m_Config.Raid.PlayerCooldownSeconds);
        session.Target.DZATM_SetRaidLocked(true);
        session.Target.DZATM_SetRaidActionBlocked(true);

        DZATM_ServerRPC.SendProgress(session.Player, true, 0.0, session.Phase2Seconds, "ATM sichern - Phase 2/2");
        m_Notifications.Personal(session.Player, "ATM RaideZ", "Hack fertig. Bleibe bis zum Ende im Radius.");
        if (session.Phase2Seconds <= 0.0) CompleteRaid(session);
    }

    protected void CompleteRaid(DZATM_HackSession session)
    {
        if (!session || session.RewardCommitted) return;

        int reason = session.Validate(m_Config);
        if (reason != DZATM_Const.CANCEL_NONE)
        {
            CancelSession(session, reason);
            return;
        }

        session.RewardCommitted = true;

        vector atmPosition = session.Target.GetPosition();
        vector direction = vector.Direction(atmPosition, session.Player.GetPosition());
        direction[1] = 0.0;
        if (direction.Normalize() <= 0.01) direction = session.Target.GetDirection() * -1.0;
        vector payoutPosition = atmPosition + (direction * 1.5);

        int payout = m_Rewards.Grant(session.Player, payoutPosition);
        if (payout <= 0)
        {
            session.RewardCommitted = false;
            CancelSession(session, DZATM_Const.CANCEL_TARGET_INVALID);
            return;
        }

        m_Cooldowns.Set(session.TargetId, m_Config.Raid.ATMCooldownSeconds);
        m_Cooldowns.Set("PLAYER|" + session.PlayerId, m_Config.Raid.PlayerCooldownSeconds);

        if (m_Config.Raid.DestroyToolOnSuccess && session.Tool)
            session.Tool.SetHealth("", "Health", 0.0);

        session.State = DZATM_Const.STATE_COMPLETED;
        m_Notifications.Personal(session.Player, "ATM RaideZ", "ATM erfolgreich ausgeraubt. Beute: " + payout.ToString() + ".");
        if (m_Config.General.EnableGlobalNotifications)
            m_Notifications.Global("ATM-RAUB ERFOLGREICH", DZATM_PlayerUtils.PlayerName(session.Player) + " hat einen ATM ausgeraubt.", 12.0);

        StopPresentation(session, true);
        CleanupSession(session);
        DZATM_Log.Info("ATM-Raub abgeschlossen session=" + session.SessionId + " payout=" + payout.ToString());
    }

    protected void StartPresentation(DZATM_HackSession session)
    {
        DZATM_ServerRPC.SendProgress(session.Player, true, 0.0, session.Phase1Seconds, "ATM hacken - Phase 1/2");

        if (m_Config.General.EnableExpansionMarkers && m_Config.Raid.EnableMapMarker)
            m_Markers.Create(session.SessionId, "ATM RAID", session.Target.GetPosition());

        if (m_Config.Raid.EnableSiren)
            m_Alarms.Start(session.SessionId, DZATM_Const.ATM_SOUNDSET, session.Target.GetPosition(), true);

        if (m_Config.Raid.EnableBlinkLight)
        {
            vector effectPosition = session.Target.GetPosition();
            effectPosition[1] = effectPosition[1] + m_Config.Raid.EffectHeightMeters;
            string beacon = DZATM_Const.CLIENT_BEACON_SOUNDSET + "|" + m_Config.Raid.BlinkIntervalSeconds.ToString() + "|" + m_Config.Raid.BlinkRadiusMeters.ToString() + "|" + m_Config.Raid.BlinkBrightness.ToString();
            m_Alarms.Start(session.SessionId + "_beacon", beacon, effectPosition, true);
        }

        EnsureSmoke(session);

        if (m_Config.General.EnableGlobalNotifications)
            m_Notifications.Global("ATM RAID", "Ein Geldautomat wird ausgeraubt. Position ist markiert.", 12.0);
    }

    protected void EnsureSmoke(DZATM_HackSession session)
    {
        if (!session || !m_Config.Raid.EnableRedSmoke) return;
        if (session.SmokeObject && !session.SmokeObject.IsDamageDestroyed()) return;

        vector pos = session.Target.GetPosition();
        pos[1] = pos[1] + m_Config.Raid.EffectHeightMeters;
        SmokeGrenadeBase smoke = SmokeGrenadeBase.Cast(GetGame().CreateObjectEx(m_Config.Raid.RedSmokeClass, pos, ECE_NONE));
        if (smoke)
        {
            smoke.Unpin();
            session.SmokeObject = smoke;
        }
    }

    protected void StopPresentation(DZATM_HackSession session, bool success)
    {
        if (!session) return;

        if (session.SmokeObject)
        {
            SmokeGrenadeBase smoke = SmokeGrenadeBase.Cast(session.SmokeObject);
            if (smoke && smoke.GetCompEM() && smoke.GetCompEM().IsWorking())
                smoke.GetCompEM().SwitchOff();
        }
        session.SmokeObject = null;

        m_Alarms.Stop(session.SessionId);
        m_Alarms.Stop(session.SessionId + "_beacon");
        m_Markers.Remove(session.SessionId);

        if (success && m_Config.Raid.EnableGreenSmokeOnSuccess)
        {
            vector pos = session.Target.GetPosition();
            pos[1] = pos[1] + m_Config.Raid.EffectHeightMeters;
            SmokeGrenadeBase green = SmokeGrenadeBase.Cast(GetGame().CreateObjectEx(m_Config.Raid.GreenSmokeClass, pos, ECE_NONE));
            if (green)
            {
                green.Unpin();
                m_SuccessSmokes.Insert(green);
            }
        }
    }

    protected void PruneSuccessSmokes()
    {
        if (!m_SuccessSmokes) return;

        for (int i = m_SuccessSmokes.Count() - 1; i >= 0; i--)
        {
            SmokeGrenadeBase smoke = m_SuccessSmokes[i];
            if (!smoke || smoke.IsDamageDestroyed())
                m_SuccessSmokes.Remove(i);
        }
    }

    protected void StopSuccessSmokes()
    {
        if (!m_SuccessSmokes) return;

        foreach (SmokeGrenadeBase smoke: m_SuccessSmokes)
        {
            if (smoke && smoke.GetCompEM() && smoke.GetCompEM().IsWorking())
                smoke.GetCompEM().SwitchOff();
        }
        m_SuccessSmokes.Clear();
    }

    protected void CancelSession(DZATM_HackSession session, int reason)
    {
        if (!session || session.State != DZATM_Const.STATE_ACTIVE) return;

        session.State = DZATM_Const.STATE_CANCELLED;
        if (reason != DZATM_Const.CANCEL_SERVER_SHUTDOWN && reason != DZATM_Const.CANCEL_ADMIN && m_Config.Raid.DamageToolOnFail)
            DZATM_PlayerUtils.DamageTool(session.Tool, m_Config.Raid.ToolDamageOnFail);

        if (session.Player)
            m_Notifications.Personal(session.Player, "ATM RaideZ", "Raub abgebrochen: " + DZATM_Utils.CancelReasonLabel(reason));

        StopPresentation(session, false);
        CleanupSession(session);
        RefreshATMLocks();
    }

    protected void CleanupSession(DZATM_HackSession session)
    {
        if (session.Player) DZATM_ServerRPC.SendProgress(session.Player, false, 0.0, 0.0, "");
        m_Sessions.Remove(session.SessionId);
    }

    protected DZATM_HackSession FindSession(PlayerBase player, ExpansionATMBase atm)
    {
        foreach (string id, DZATM_HackSession session: m_Sessions)
        {
            if (session && session.Player == player && session.Target == atm) return session;
        }
        return null;
    }

    protected bool HasPlayerSession(string playerId)
    {
        foreach (string id, DZATM_HackSession session: m_Sessions)
        {
            if (session && session.State == DZATM_Const.STATE_ACTIVE && session.PlayerId == playerId) return true;
        }
        return false;
    }

    protected bool HasTargetSession(string targetId)
    {
        foreach (string id, DZATM_HackSession session: m_Sessions)
        {
            if (session && session.State == DZATM_Const.STATE_ACTIVE && session.TargetId == targetId) return true;
        }
        return false;
    }

    void HandleAdminCommand(PlayerIdentity sender, PlayerBase player, string command)
    {
        if (!sender || !player || !player.GetIdentity() || sender.GetPlainId() != player.GetIdentity().GetPlainId()) return;

        if (!IsAdmin(sender.GetPlainId()))
        {
            DZATM_Log.Warn("Nicht autorisierter Admin-RPC uid=" + sender.GetPlainId());
            return;
        }

        int nowMs = GetGame().GetTime();
        int lastMs;
        if (m_AdminLastCommandAt.Find(sender.GetPlainId(), lastMs) && nowMs - lastMs < 750)
            return;
        m_AdminLastCommandAt.Set(sender.GetPlainId(), nowMs);

        command.TrimInPlace();
        string result = "Unbekannter Befehl";

        if (command == "status")
        {
            result = "Status aktualisiert";
        }
        else if (command == "reload")
        {
            m_Config = DZATM_Config.Load();
            m_Rewards = new DZATM_RewardProvider(m_Config);
            m_ATMs.ApplyHackDuration(m_Config.Raid.Phase1HackSeconds);
            RefreshATMLocks();
            result = "Config neu geladen";
        }
        else if (command == "cancelall")
        {
            CancelAll(DZATM_Const.CANCEL_ADMIN);
            result = "Alle aktiven Raids abgebrochen";
        }
        else if (command == "clearnearest")
        {
            string nearest = m_ATMs.FindNearestId(player.GetPosition(), 25.0);
            bool cleared = nearest != "" && m_Cooldowns.Clear(nearest);
            result = "Naechster ATM-Cooldown geloescht=" + cleared.ToString();
            RefreshATMLocks();
        }
        else if (command == "locknearest")
        {
            string lockId = m_ATMs.FindNearestId(player.GetPosition(), 25.0);
            if (lockId != "") m_Cooldowns.Set(lockId, 86400);
            result = "Naechster ATM manuell gesperrt=" + (lockId != "").ToString();
            RefreshATMLocks();
        }
        else if (command == "unlocknearest")
        {
            string unlockId = m_ATMs.FindNearestId(player.GetPosition(), 25.0);
            if (unlockId != "") m_Cooldowns.Clear(unlockId);
            DZATM_HackSession unlockSession = FindTargetSession(unlockId);
            if (unlockSession) CancelSession(unlockSession, DZATM_Const.CANCEL_ADMIN);
            result = "Naechster ATM entsperrt=" + (unlockId != "").ToString();
            RefreshATMLocks();
        }
        else if (command == "cancelnearest")
        {
            string cancelId = m_ATMs.FindNearestId(player.GetPosition(), 25.0);
            DZATM_HackSession cancelSession = FindTargetSession(cancelId);
            if (cancelSession) CancelSession(cancelSession, DZATM_Const.CANCEL_ADMIN);
            result = "Aktiver Raid am naechsten ATM abgebrochen=" + (cancelSession != null).ToString();
        }
        else if (command == "debug_on")
        {
            DZATM_RuntimeFlags.DebugEnabled = true;
            result = "Debug aktiviert";
        }
        else if (command == "debug_off")
        {
            DZATM_RuntimeFlags.DebugEnabled = false;
            result = "Debug deaktiviert";
        }

        else
        {
            DZATM_Log.Warn("Abgewiesener Admin-Befehl uid=" + sender.GetPlainId() + " command=" + command);
            return;
        }

        string nearestId = m_ATMs.FindNearestId(player.GetPosition(), 25.0);
        if (nearestId != "")
        {
            result = result + "\nATM-ID: " + nearestId + "\nCooldown: " + Math.Ceil(m_Cooldowns.GetRemaining(nearestId)).ToString() + "s";
            DZATM_HackSession nearestSession = FindTargetSession(nearestId);
            if (nearestSession)
                result = result + "\nRaider: " + nearestSession.PlayerId + "\nPhase: " + nearestSession.Phase.ToString() + "\nRestzeit: " + GetSessionRemaining(nearestSession).ToString() + "s";
        }
        DZATM_ServerRPC.SendAdminStatus(player, m_Sessions.Count(), m_ATMs.Count(), DZATM_RuntimeFlags.DebugEnabled, result);
        DZATM_Log.Info("Admin-Befehl uid=" + sender.GetPlainId() + " command=" + command);
    }

    protected bool IsAdmin(string uid)
    {
        if (!m_Config || !m_Config.General) return false;
        if (m_Config.General.AdminUIDs.Count() == 0) return m_Config.General.AllowAdminWhenListEmpty;
        return m_Config.General.AdminUIDs.Find(uid) >= 0;
    }

    protected DZATM_HackSession FindTargetSession(string targetId)
    {
        foreach (string id, DZATM_HackSession session: m_Sessions)
            if (session && session.State == DZATM_Const.STATE_ACTIVE && session.TargetId == targetId) return session;
        return null;
    }

    protected int GetSessionRemaining(DZATM_HackSession session)
    {
        if (!session) return 0;
        float duration = session.Phase1Seconds;
        if (session.Phase == DZATM_Const.PHASE_GUARD) duration = session.Phase2Seconds;
        return Math.Ceil(Math.Max(duration - session.GetPhaseElapsedSeconds(), 0.0));
    }

    protected void RefreshATMLocks()
    {
        if (!m_ATMs || !m_Cooldowns) return;
        array<string> activeTargets = new array<string>;
        array<string> blockedRaidActions = new array<string>;
        foreach (string id, DZATM_HackSession session: m_Sessions)
        {
            if (!session || session.State != DZATM_Const.STATE_ACTIVE) continue;
            activeTargets.Insert(session.TargetId);
            if (session.Phase == DZATM_Const.PHASE_GUARD)
                blockedRaidActions.Insert(session.TargetId);
        }
        m_ATMs.UpdateLocksWithSessions(m_Cooldowns, activeTargets, blockedRaidActions);
    }
}
