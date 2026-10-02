ref deutschz_aiconvoyz_controller g_deutschz_aiconvoyz_controller;

enum deutschz_aiconvoyz_state
{
    IDLE,
    CONVOY_RUNNING,
    AMBUSH_CREW_ACTIVE,
    HOSTILE_SUPPORT_INBOUND,
    DESTINATION_TEAM_ONE_ACTIVE,
    BLACKBOX_ACTIVE,
    DECODER_RECOVERED
}

class deutschz_aiconvoyz_controller
{
    protected ref deutschz_aiconvoyz_settings m_settings;
    protected deutschz_aiconvoyz_state m_state;
    protected vector m_destination;
    protected vector m_stop_position;
    protected bool m_was_ambushed;
    protected ref array<eAIBase> m_convoy_crew = new array<eAIBase>();
    protected ref array<eAIBase> m_spawned_ai = new array<eAIBase>();
    protected ref array<ref eAIGroup> m_spawned_groups = new array<ref eAIGroup>();
    protected Object m_helicrash;
    protected deutschz_aiconvoyz_blackbox m_blackbox;
    protected ref array<EntityAI> m_crash_smokes = new array<EntityAI>();
    protected bool m_helicrash_spawned;
    protected int m_last_combat_sound_at;
    protected int m_support_seconds_remaining;
    protected ExpansionMarkerModule m_marker_module;
    protected ExpansionMarkerData m_operation_marker;
    protected bool m_reward_in_progress;

    void deutschz_aiconvoyz_controller()
    {
        m_settings = deutschz_aiconvoyz_settings_service.Load();
        m_marker_module = ExpansionMarkerModule.Cast(CF_ModuleCoreManager.Get(ExpansionMarkerModule));
        m_state = deutschz_aiconvoyz_state.IDLE;
    }

    int GetHackSeconds()
    {
        if (m_settings && m_settings.Global)
            return Math.Max(1, m_settings.Global.hack_seconds);
        return 90;
    }

    bool IsActiveBlackbox(deutschz_aiconvoyz_blackbox box)
    {
        return box && box == m_blackbox && m_state == deutschz_aiconvoyz_state.BLACKBOX_ACTIVE;
    }

    bool IsBlackboxCodeValid(string code)
    {
        return m_settings && m_settings.Global && code == m_settings.Global.blackbox_code;
    }

    bool FillBlackboxLoot(deutschz_aiconvoyz_blackbox box, PlayerBase player)
    {
        if (!GetGame().IsServer() || !IsActiveBlackbox(box) || !player || !player.IsAlive() || !player.GetIdentity() || m_reward_in_progress)
            return false;

        m_reward_in_progress = true;
        string playerId = player.GetIdentity().GetPlainId();
        string completionPath = CompletionPath("convoy", playerId);
        EntityAI decoder;
        if (!FileExist(completionPath))
        {
            decoder = player.GetInventory().CreateInInventory("Toxicz_Doc_Decoder");
            if (!decoder)
                decoder = EntityAI.Cast(GetGame().CreateObjectEx("Toxicz_Doc_Decoder", player.GetPosition(), ECE_PLACE_ON_SURFACE));
            if (!decoder)
            {
                m_reward_in_progress = false;
                Print("[DeutschZ AIConvoyZ] ERROR: Decoder grant failed; completion not committed player=" + playerId);
                return false;
            }
            Print("[DeutschZ AIConvoyZ] Decoder granted player=" + playerId);
        }
        else
            Print("[DeutschZ AIConvoyZ] Decoder already owned/completed player=" + playerId);

        if (m_settings.Global.blackbox_loot)
        {
            foreach (deutschz_aiconvoyz_loot_setting loot: m_settings.Global.blackbox_loot)
            {
                if (!loot || loot.class_name == "" || loot.count <= 0)
                    continue;
                for (int lootIndex = 0; lootIndex < loot.count; lootIndex++)
                {
                    if (Math.RandomFloatInclusive(0.0, 100.0) <= Math.Clamp(loot.chance_percent, 0.0, 100.0))
                    {
                        EntityAI reward = box.GetInventory().CreateInInventory(loot.class_name);
                        if (!reward)
                            Print("[DeutschZ AIConvoyZ] WARNING: optional blackbox loot could not be created: " + loot.class_name);
                    }
                }
            }
        }
        m_reward_in_progress = false;
        return true;
    }

    AIConvoyEventParams Prepare(AIConvoyEventParams params)
    {
        if (!params || !m_settings || !m_settings.Global || !m_settings.Global.enabled)
            return params;
        if (!params.Waypoints || params.Waypoints.Count() == 0)
        {
            Print("[DeutschZ AIConvoyZ] ERROR: convoy has no destination waypoint");
            return params;
        }

        m_destination = FindLandPosition(params.Waypoints[params.Waypoints.Count() - 1], params.Waypoints[0]);
        return params;
    }

    void OnStarted(AIConvoyEventParams params)
    {
        if (m_state != deutschz_aiconvoyz_state.IDLE || m_spawned_ai.Count() > 0 || m_spawned_groups.Count() > 0 || m_blackbox || m_operation_marker)
        {
            Print("[DeutschZ AIConvoyZ] Stale encounter found before new convoy start; cleanup required");
            CleanupEncounter();
        }
        m_was_ambushed = false;
        m_state = deutschz_aiconvoyz_state.CONVOY_RUNNING;
        Print("[DeutschZ AIConvoyZ] STATE CONVOY_RUNNING route=" + params.Name + " destination=" + m_destination.ToString());
    }

    void OnStopped(vector position)
    {
        if (m_state != deutschz_aiconvoyz_state.CONVOY_RUNNING)
            return;

        m_was_ambushed = true;
        m_stop_position = FindLandPosition(position, m_destination);
        m_state = deutschz_aiconvoyz_state.AMBUSH_CREW_ACTIVE;
        Print("[DeutschZ AIConvoyZ] STATE AMBUSH_CREW_ACTIVE position=" + m_stop_position.ToString());
        AnnounceAmbush(m_stop_position, "Feindliche Unterstuetzung im Anmarsch");
        StartHostileSupportCountdown();
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CaptureConvoyCrew, Math.Max(1, m_settings.Global.crew_capture_delay_seconds) * 1000, false);
    }

    protected void CaptureConvoyCrew()
    {
        if (m_state != deutschz_aiconvoyz_state.AMBUSH_CREW_ACTIVE && m_state != deutschz_aiconvoyz_state.HOSTILE_SUPPORT_INBOUND)
            return;

        m_convoy_crew.Clear();
        array<Object> objects = new array<Object>();
        array<CargoBase> proxy = new array<CargoBase>();
        GetGame().GetObjectsAtPosition3D(m_stop_position, Math.Max(20.0, m_settings.Global.convoy_crew_capture_radius), objects, proxy);
        foreach (Object object: objects)
        {
            eAIBase ai = eAIBase.Cast(object);
            if (ai && ai.IsAlive())
                m_convoy_crew.Insert(ai);
        }

        Print("[DeutschZ AIConvoyZ] Convoy crew captured: " + m_convoy_crew.Count().ToString() + " living AI");
        StartCombatFollowup();
    }

    void OnEnded()
    {
        if (m_was_ambushed)
        {
            Print("[DeutschZ AIConvoyZ] Dependency convoy ended after player ambush; DeutschZ phase remains active");
            return;
        }

        if (!HasPlayerWithin(m_destination, m_settings.Global.destination_player_radius))
        {
            m_state = deutschz_aiconvoyz_state.IDLE;
            Print("[DeutschZ AIConvoyZ] Destination reached; no player within 300 m, no battle spawned");
            return;
        }

        int teamOne = SpawnTeam(m_destination, "AmericanZ", m_settings.Global.team_one_ai_count, m_settings.Global.team_one_loadout, -32.0);
        if (teamOne > 0)
        {
            m_state = deutschz_aiconvoyz_state.DESTINATION_TEAM_ONE_ACTIVE;
            Print("[DeutschZ AIConvoyZ] STATE DESTINATION_TEAM_ONE_ACTIVE team1=" + teamOne.ToString());
            AnnounceAmbush(m_destination, "AmericanZ sichern den angekommenen AI-Convoy");
            StartCombatFollowup();
        }
        else
            Print("[DeutschZ AIConvoyZ] ERROR: destination team could not be spawned");
    }

    protected int SpawnTeam(vector center, string factionName, int count, string loadout, float offsetX, vector objective = "0 0 0")
    {
        if (count <= 0)
            return 0;

        eAIFaction faction = eAIFaction.Create(factionName);
        if (!faction)
        {
            Print("[DeutschZ AIConvoyZ] ERROR: faction unavailable: " + factionName);
            return 0;
        }

        eAIGroup group = eAIGroup.CreateGroup(faction);
        if (!group)
            return 0;
        group.SetName("DeutschZ AIConvoyZ " + factionName);
        if (objective != "0 0 0")
        {
            group.AddWaypoint(objective);
            group.SetWaypointBehaviour(eAIWaypointBehavior.HALT);
        }
        else
            group.SetWaypointBehaviour(eAIWaypointBehavior.HALT);
        m_spawned_groups.Insert(group);

        int spawned;
        for (int index = 0; index < count; index++)
        {
            vector desired = center + Vector(offsetX + Math.RandomFloatInclusive(-8, 8), 0, Math.RandomFloatInclusive(-14, 14));
            vector position = FindLandPosition(desired, center);
            eAIBase ai = eAIBase.Cast(GetGame().CreateObject(eAISurvivor.GetQuasiRandom(), position));
            if (!ai)
                continue;
            ai.SetGroup(group);
            ExpansionHumanLoadout.Apply(ai, loadout, false);
            m_spawned_ai.Insert(ai);
            spawned++;
        }
        return spawned;
    }

    protected int GetHostileSupportSeconds()
    {
        if (m_settings && m_settings.Global && m_settings.Global.hostile_support_arrival_seconds > 0)
            return m_settings.Global.hostile_support_arrival_seconds;
        return 180;
    }

    protected void StartHostileSupportCountdown()
    {
        m_support_seconds_remaining = GetHostileSupportSeconds();
        m_state = deutschz_aiconvoyz_state.HOSTILE_SUPPORT_INBOUND;
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(HostileSupportCountdownTick);
        BroadcastSupportCountdown();
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(HostileSupportCountdownTick, 10000, true);
        Print("[DeutschZ AIConvoyZ] STATE HOSTILE_SUPPORT_INBOUND countdown=" + m_support_seconds_remaining.ToString() + "s");
    }

    protected void HostileSupportCountdownTick()
    {
        if (m_state != deutschz_aiconvoyz_state.HOSTILE_SUPPORT_INBOUND)
        {
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(HostileSupportCountdownTick);
            return;
        }

        m_support_seconds_remaining = Math.Max(0, m_support_seconds_remaining - 10);
        if (m_support_seconds_remaining > 0)
        {
            BroadcastSupportCountdown();
            return;
        }

        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(HostileSupportCountdownTick);
        Broadcast("AI-Convoy Funk", "Mayday, Mayday, wir stuerzen ab!");
        Print("[DeutschZ AIConvoyZ] MAYDAY countdown completed; spawning helicrash");
        SpawnHelicrash();
    }

    protected void BroadcastSupportCountdown()
    {
        int minutes = m_support_seconds_remaining / 60;
        int seconds = m_support_seconds_remaining % 60;
        string secondsText = seconds.ToString();
        if (seconds < 10)
            secondsText = "0" + secondsText;
        Broadcast("AI-Convoy", "Feindliche Unterstuetzung im Anmarsch - " + minutes.ToString() + ":" + secondsText);
    }

    protected void StartCombatFollowup()
    {
        m_helicrash_spawned = false;
        m_last_combat_sound_at = 0;
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(CombatFollowupTick);
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CombatFollowupTick, 2000, true);
    }

    protected void CombatFollowupTick()
    {
        SendCombatSound();
    }

    protected void SendCombatSound()
    {
        int now = GetGame().GetTime();
        if (now - m_last_combat_sound_at < 10000)
            return;
        m_last_combat_sound_at = now;

        vector center = m_destination;
        if (m_was_ambushed)
            center = m_stop_position;
        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);
        foreach (Man man: players)
        {
            if (!man || !man.GetIdentity())
                continue;
            float maxDistance = Math.Max(1.0, m_settings.Global.combat_sound_max_distance);
            float distance = vector.Distance(man.GetPosition(), center);
            if (distance > maxDistance)
                continue;
            float volume = Math.Clamp(1.0 - (distance / maxDistance), m_settings.Global.combat_sound_min_volume, 1.0);
            vector audiblePosition = GetAudibleSoundPosition(man.GetPosition(), center, 75.0);
            GetRPCManager().SendRPC(deutschz_aiconvoyz_rpc.namespace_name, deutschz_aiconvoyz_rpc.combat_sound_method, new Param3<vector, float, int>(audiblePosition, volume, Math.RandomIntInclusive(0, 1)), true, man.GetIdentity());
        }
    }

    protected void SpawnHelicrash()
    {
        if (m_helicrash_spawned)
            return;
        vector crashPosition = m_settings.Global.helicrash_position;
        string crashClass = m_settings.Global.helicrash_class;
        if (!GetGame().ConfigIsExisting("CfgVehicles " + crashClass))
        {
            Print("[DeutschZ AIConvoyZ] WARNING: missing configured wreck class " + crashClass + "; using Wreck_UH1Y fallback");
            crashClass = "Wreck_UH1Y";
        }
        if (!GetGame().ConfigIsExisting("CfgVehicles " + crashClass))
        {
            Print("[DeutschZ AIConvoyZ] ERROR: fallback wreck class is unavailable: " + crashClass);
            return;
        }
        m_helicrash = GetGame().CreateObjectEx(crashClass, crashPosition, ECE_NOPERSISTENCY_WORLD);
        if (!m_helicrash)
        {
            Print("[DeutschZ AIConvoyZ] ERROR: wreck creation failed");
            return;
        }
        m_helicrash.SetPosition(crashPosition);
        m_helicrash.SetOrientation(m_settings.Global.helicrash_orientation);
        m_blackbox = deutschz_aiconvoyz_blackbox.Cast(GetGame().CreateObjectEx("deutschz_aiconvoyz_blackbox", m_settings.Global.blackbox_position, ECE_NOPERSISTENCY_WORLD));
        if (!m_blackbox)
        {
            GetGame().ObjectDelete(m_helicrash);
            m_helicrash = null;
            Print("[DeutschZ AIConvoyZ] ERROR: blackbox creation failed");
            return;
        }
        m_blackbox.SetPosition(m_settings.Global.blackbox_position);
        m_blackbox.SetOrientation(m_settings.Global.blackbox_orientation);
        m_helicrash_spawned = true;
        SpawnCrashSmoke(crashPosition);
        SendCrashSound(crashPosition);

        m_state = deutschz_aiconvoyz_state.BLACKBOX_ACTIVE;
        Broadcast("AI-Convoy", "Helicrash geortet: Blackbox sichern: 90 Sekunden hacken oder Zugangscode eingeben");
        int teamTwo = SpawnTeam(crashPosition, "RussianZ", m_settings.Global.team_two_ai_count, m_settings.Global.team_two_loadout, 45.0, crashPosition);
        if (teamTwo > 0)
        {
            Print("[DeutschZ AIConvoyZ] STATE BLACKBOX_ACTIVE team2=" + teamTwo.ToString());
        }
        else
            Print("[DeutschZ AIConvoyZ] ERROR: RussianZ recovery team could not be spawned");
        Print("[DeutschZ AIConvoyZ] HELICRASH spawned after support countdown at " + crashPosition.ToString());
    }

    protected void SpawnCrashSmoke(vector position)
    {
        EntityAI smokeEntity = EntityAI.Cast(GetGame().CreateObjectEx("RDG2SmokeGrenade_Black", position, ECE_PLACE_ON_SURFACE | ECE_NOPERSISTENCY_WORLD));
        Grenade_Base smoke = Grenade_Base.Cast(smokeEntity);
        if (!smoke)
            return;
        m_crash_smokes.Insert(smokeEntity);
        smoke.Unpin();
    }

    protected void SendCrashSound(vector position)
    {
        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);
        foreach (Man man: players)
        {
            if (!man || !man.GetIdentity())
                continue;
            float maxDistance = Math.Max(1.0, m_settings.Global.combat_sound_max_distance);
            float distance = vector.Distance(man.GetPosition(), position);
            if (distance > maxDistance)
                continue;
            float volume = Math.Clamp(1.0 - (distance / maxDistance), m_settings.Global.combat_sound_min_volume, 1.0);
            vector audiblePosition = GetAudibleSoundPosition(man.GetPosition(), position, 150.0);
            GetRPCManager().SendRPC(deutschz_aiconvoyz_rpc.namespace_name, deutschz_aiconvoyz_rpc.crash_sound_method, new Param2<vector, float>(audiblePosition, volume), true, man.GetIdentity());
        }
    }

    protected vector GetAudibleSoundPosition(vector listener, vector source, float maxSourceDistance)
    {
        float distance = vector.Distance(listener, source);
        if (distance <= maxSourceDistance || distance <= 0.01)
            return source;

        float ratio = maxSourceDistance / distance;
        return Vector(listener[0] + ((source[0] - listener[0]) * ratio), listener[1] + ((source[1] - listener[1]) * ratio), listener[2] + ((source[2] - listener[2]) * ratio));
    }

    void OnBlackboxHacked(deutschz_aiconvoyz_blackbox box, PlayerBase player)
    {
        string playerId = "unknown";
        if (player && player.GetIdentity())
            playerId = player.GetIdentity().GetPlainId();
        m_state = deutschz_aiconvoyz_state.DECODER_RECOVERED;
        Print("[DeutschZ AIConvoyZ] Hack completed; STATE DECODER_RECOVERED player=" + playerId);
        Broadcast("AI-Convoy", "Blackbox gehackt: Convoy Decoder geborgen");
        if (RecordRadioMissionCompletion(player, "convoy"))
            GrantHardlineReputation(player, 250);
    }

    protected string CompletionPath(string eventId, string playerId)
    {
        return "$profile:DeutschZ-System/deutschz_radiomissionz/event_completions/" + eventId + "/" + playerId + ".done";
    }

    protected bool RecordRadioMissionCompletion(PlayerBase player, string eventId)
    {
        if (!player || !player.GetIdentity()) return false;
        MakeDirectory("$profile:DeutschZ-System");
        MakeDirectory("$profile:DeutschZ-System/deutschz_radiomissionz");
        MakeDirectory("$profile:DeutschZ-System/deutschz_radiomissionz/event_completions");
        MakeDirectory("$profile:DeutschZ-System/deutschz_radiomissionz/event_completions/" + eventId);
        string path = CompletionPath(eventId, player.GetIdentity().GetPlainId());
        if (FileExist(path))
        {
            Print("[DeutschZ AIConvoyZ] Completion already committed player=" + player.GetIdentity().GetPlainId());
            return false;
        }
        FileHandle file = OpenFile(path, FileMode.WRITE);
        if (file == 0) return false;
        FPrintln(file, eventId); CloseFile(file);
        Print("[DeutschZ AIConvoyZ] Completion committed player=" + player.GetIdentity().GetPlainId());
        return true;
    }

    protected void GrantHardlineReputation(PlayerBase player, int points)
    {
        if (!player || !player.GetIdentity() || points <= 0) return;
        #ifdef EXPANSIONMODHARDLINE
        int before = player.Expansion_GetReputation();
        if (before >= 0 && player.Expansion_SetReputation(before + points, true))
            Print("[DeutschZ AIConvoyZ] Hardline reward granted points=" + points.ToString() + " player=" + player.GetIdentity().GetPlainId());
        else
            Print("[DeutschZ AIConvoyZ] WARNING: Hardline reward failed player=" + player.GetIdentity().GetPlainId());
        #else
        Print("[DeutschZ AIConvoyZ] WARNING: Hardline unavailable; completion remains valid");
        #endif
    }

    protected void AnnounceAmbush(vector position, string text)
    {
        if (m_marker_module)
        {
            if (m_operation_marker)
                m_marker_module.RemoveServerMarker(m_operation_marker.GetUID());
            m_operation_marker = m_marker_module.CreateServerMarker("AI-Convoy Ueberfall", "Helicopter", position, ARGB(255, 220, 45, 45), true, "deutschz_aiconvoyz_operation");
        }
        Broadcast("AI-Convoy Ueberfall", text);
    }

    protected void Broadcast(string title, string text)
    {
        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);
        foreach (Man man: players)
        {
            if (man && man.GetIdentity())
                NotificationSystem.SendNotificationToPlayerIdentityExtended(man.GetIdentity(), 10, title, text);
        }
    }

    protected bool HasPlayerWithin(vector position, float radius)
    {
        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);
        foreach (Man man: players)
        {
            if (man && man.IsAlive() && vector.Distance(man.GetPosition(), position) <= radius)
                return true;
        }
        return false;
    }

    protected void NotifyNearby(vector position, float radius, string title, string text)
    {
        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);
        foreach (Man man: players)
        {
            if (man && man.GetIdentity() && vector.Distance(man.GetPosition(), position) <= radius)
                NotificationSystem.SendNotificationToPlayerIdentityExtended(man.GetIdentity(), 8, title, text);
        }
    }

    protected vector FindLandPosition(vector desired, vector fallback)
    {
        if (!GetGame().SurfaceIsSea(desired[0], desired[2]) && !GetGame().SurfaceIsPond(desired[0], desired[2]))
        {
            desired[1] = GetGame().SurfaceY(desired[0], desired[2]);
            return desired;
        }
        fallback[1] = GetGame().SurfaceY(fallback[0], fallback[2]);
        return fallback;
    }

    void CleanupEncounter()
    {
        Print("[DeutschZ AIConvoyZ] Cleanup started");
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(CaptureConvoyCrew);
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(CombatFollowupTick);
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(HostileSupportCountdownTick);
        foreach (eAIGroup group: m_spawned_groups)
        {
            if (group)
                group.ClearAI(true);
        }
        m_spawned_groups.Clear();
        m_spawned_ai.Clear();
        m_convoy_crew.Clear();
        if (m_helicrash)
            GetGame().ObjectDelete(m_helicrash);
        if (m_blackbox)
            GetGame().ObjectDelete(m_blackbox);
        foreach (EntityAI smokeEntity: m_crash_smokes)
        {
            if (smokeEntity)
                GetGame().ObjectDelete(smokeEntity);
        }
        m_crash_smokes.Clear();
        m_helicrash = null;
        m_blackbox = null;
        m_helicrash_spawned = false;
        if (m_marker_module && m_operation_marker)
            m_marker_module.RemoveServerMarker(m_operation_marker.GetUID());
        m_operation_marker = null;
        m_was_ambushed = false;
        m_reward_in_progress = false;
        m_state = deutschz_aiconvoyz_state.IDLE;
        Print("[DeutschZ AIConvoyZ] Cleanup finished");
    }
}

modded class AIConvoyManager
{
    override protected AIConvoyEventParams AssembleEventForRoute(AIConvoyRoute route)
    {
        AIConvoyEventParams params = super.AssembleEventForRoute(route);
        if (!g_deutschz_aiconvoyz_controller)
            g_deutschz_aiconvoyz_controller = new deutschz_aiconvoyz_controller();
        return g_deutschz_aiconvoyz_controller.Prepare(params);
    }

    override void NotifyConvoyStarted(AIConvoyEventParams params)
    {
        super.NotifyConvoyStarted(params);
        if (g_deutschz_aiconvoyz_controller)
            g_deutschz_aiconvoyz_controller.OnStarted(params);
    }

    override void NotifyConvoyStopped(vector stopPos)
    {
        super.NotifyConvoyStopped(stopPos);
        if (g_deutschz_aiconvoyz_controller)
            g_deutschz_aiconvoyz_controller.OnStopped(stopPos);
    }

    override void OnEventEnded(AIConvoyEvent ev)
    {
        if (g_deutschz_aiconvoyz_controller)
            g_deutschz_aiconvoyz_controller.OnEnded();
        super.OnEventEnded(ev);
    }
}
