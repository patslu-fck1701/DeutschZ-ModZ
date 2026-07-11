class DZBBC_EventInstance
{
	protected static const float HUD_RANGE_METERS = 500.0;
	protected ref DZBBC_ConfigBundle m_Config;
	protected DZBBC_CrashSiteConfig m_Site;
	protected ref DZBBC_CrashSiteManager m_CrashSite;
	protected ref DZBBC_AIFactionManager m_AI;
	protected ref DZBBC_BlackboxManager m_Blackbox;
	protected ref DZBBC_HackManager m_Hack;
	protected ref DZBBC_DataCoreTracker m_DataCore;
	protected ref DZBBC_TerminalManager m_Terminals;
	protected ref DZBBC_FinalDefenseManager m_FinalDefense;
	protected ref DZBBC_LootManager m_Loot;
	protected ref DZBBC_MapMarkerManager m_Markers;
	protected ref DZBBC_SoundManager m_Sound;
	protected ref array<Object> m_TestObjects;
	protected DZBBC_CombatZone m_CombatZone;
	protected int m_State;
	protected bool m_PlayerDetected;
	protected bool m_FinalDefenseStarted;
	protected float m_TransportMarkerTimer;

	void DZBBC_EventInstance(DZBBC_ConfigBundle config)
	{
		m_Config = config;
		m_CrashSite = new DZBBC_CrashSiteManager;
		m_AI = new DZBBC_AIFactionManager;
		m_Blackbox = new DZBBC_BlackboxManager;
		m_Hack = new DZBBC_HackManager;
		m_DataCore = new DZBBC_DataCoreTracker;
		m_Terminals = new DZBBC_TerminalManager;
		m_FinalDefense = new DZBBC_FinalDefenseManager;
		m_FinalDefense.Setup(m_AI);
		m_Loot = new DZBBC_LootManager;
		m_Markers = new DZBBC_MapMarkerManager;
		m_Sound = new DZBBC_SoundManager;
		m_TestObjects = new array<Object>;
		m_State = DZBBC_INACTIVE;
	}

	bool Start(int crashsiteIndex)
	{
		if (!GetGame() || !GetGame().IsServer() || !m_Config || !m_Config.Main || !m_Config.Locations)
			return false;

		if (!m_Config.Locations.CrashSites || m_Config.Locations.CrashSites.Count() == 0)
		{
			DZBBC_Utils.Error("No crashsites configured.");
			return false;
		}

		if (crashsiteIndex < 0 || crashsiteIndex >= m_Config.Locations.CrashSites.Count())
			crashsiteIndex = Math.RandomInt(0, m_Config.Locations.CrashSites.Count());

		m_Site = m_Config.Locations.CrashSites.Get(crashsiteIndex);
		if (!m_Site)
			return false;

		SetState(DZBBC_ANNOUNCED);
		DZBBC_ServerRPC.BroadcastNotification(m_Config.Main.EventName, m_Config.Messages.EventAnnounce, 10.0);

		m_CrashSite.Spawn(m_Site);
		DZBBC_ServerRPC.BroadcastFX(DZBBC_FX_CRASHSITE, m_Site.GetPosition());
		m_Terminals.SpawnTerminals(m_Config.Terminals);
		m_Markers.ShowCrashsite(m_Site);
		if (m_Terminals.GetActiveTerminalConfig())
			m_Markers.ShowTerminal(m_Terminals.GetActiveTerminalConfig());

		CreateCombatZone();
		SpawnInitialFactions();
		SetState(DZBBC_FACTION_FIGHT_ACTIVE);
		StartTick();
		DZBBC_Utils.Log("Event started at " + m_Site.Name);

		return true;
	}

	void OnPlayerEntered(PlayerBase player)
	{
		if (!DZBBC_PlayerUtils.IsValidPlayer(player))
			return;

		if (!m_PlayerDetected)
		{
			m_PlayerDetected = true;
			SetState(DZBBC_PLAYER_DETECTED);
			DZBBC_ServerRPC.BroadcastNotification(m_Config.Main.EventName, m_Config.Messages.PlayerDetected, 8.0);
		}

		m_AI.PrioritizePlayer(player);
		if (m_State == DZBBC_PLAYER_DETECTED || m_State == DZBBC_FACTION_FIGHT_ACTIVE)
			SetState(DZBBC_COMBAT_ACTIVE);

		SyncToPlayer(player);
	}

	void OnPlayerLeft(PlayerBase player)
	{
		if (player)
			DZBBC_ServerRPC.SendHud(player, DZBBC_HUD_HIDE, "", 0.0, 0.0);
	}

	bool StartBlackboxHack(PlayerBase player, Object blackbox)
	{
		if (m_State != DZBBC_BLACKBOX_SIGNAL_FOUND && m_State != DZBBC_COMBAT_COMPLETE)
			return false;

		if (!m_Blackbox.CanHack(player, blackbox, m_Config.Main))
			return false;

		if (!m_Hack.Begin(player, m_Config.Main.HackDurationSeconds))
			return false;

		SetState(DZBBC_BLACKBOX_HACK_ACTIVE);
		DZBBC_ServerRPC.BroadcastNotification("#STR_DZBBC_TITLE_BLACKBOX", DZBBC_PlayerUtils.GetPlayerName(player) + " hacks the blackbox.", 7.0);
		return true;
	}

	bool StartTerminalDecrypt(PlayerBase player, Object terminal)
	{
		if (m_State != DZBBC_DATACORE_TRANSPORT_ACTIVE && m_State != DZBBC_TERMINAL_FOUND)
			return false;

		if (!m_Terminals.Begin(player, terminal, m_DataCore, m_Config.Main))
			return false;

		SetState(DZBBC_TERMINAL_DECRYPT_ACTIVE);
		DZBBC_ServerRPC.BroadcastNotification(m_Config.Main.EventName, m_Config.Messages.TerminalStarted, 8.0);
		StartFinalDefense();
		return true;
	}

	bool CompleteTerminalDecrypt(PlayerBase player, Object terminal)
	{
		if (m_State == DZBBC_DATACORE_TRANSPORT_ACTIVE || m_State == DZBBC_TERMINAL_FOUND)
		{
			if (!StartTerminalDecrypt(player, terminal))
				return false;
		}

		if (m_State != DZBBC_TERMINAL_DECRYPT_ACTIVE && m_State != DZBBC_FINAL_DEFENSE_ACTIVE)
			return false;

		DZBBC_Utils.Log("Terminal decrypt complete by 60s action.");
		CompleteTerminal();
		return true;
	}

	void Tick()
	{
		float delta = m_Config.Main.TickSeconds;
		if (delta <= 0.0)
			delta = 1.0;

		if (m_State == DZBBC_FACTION_FIGHT_ACTIVE || m_State == DZBBC_PLAYER_DETECTED || m_State == DZBBC_COMBAT_ACTIVE)
			TickCombat();

		if (m_State == DZBBC_BLACKBOX_HACK_ACTIVE)
			m_Hack.Tick(delta, this);

		if (m_State == DZBBC_DATACORE_TRANSPORT_ACTIVE || m_State == DZBBC_TERMINAL_FOUND)
			TickDataCore(delta);

		if (m_State == DZBBC_TERMINAL_DECRYPT_ACTIVE || m_State == DZBBC_FINAL_DEFENSE_ACTIVE)
			TickTerminal(delta);
	}

	void OnHackStage(int stage, PlayerBase player)
	{
		m_Sound.BroadcastHackStage(stage, m_Blackbox.GetPosition(), m_Config.Messages);

		if (stage == 90)
			m_AI.SpawnFaction(m_Config.AIWaves.RecoveryUnit90, m_Config.Main, m_Blackbox.GetPosition());
	}

	void OnHackFailed()
	{
		DZBBC_ServerRPC.BroadcastNotification(m_Config.Main.EventName, "#STR_DZBBC_MSG_HACK_CANCELED", 6.0);
		SetState(DZBBC_BLACKBOX_SIGNAL_FOUND);
	}

	void OnHackComplete(PlayerBase player)
	{
		if (!player)
			return;

		EntityAI dataCore = m_Blackbox.GiveDataCore(player);
		if (!dataCore)
		{
			DZBBC_ServerRPC.SendNotification(player, "#STR_DZBBC_TITLE_BLACKBOX", "#STR_DZBBC_MSG_DATACORE_FAILED", 6.0);
			return;
		}

		SetState(DZBBC_BLACKBOX_HACK_COMPLETE);
		m_DataCore.Begin(player, m_Config.Main.DataCoreLifetimeSeconds);
		SetState(DZBBC_DATACORE_EXTRACTED);
		DZBBC_ServerRPC.BroadcastNotification(m_Config.Main.EventName, m_Config.Messages.DatacoreExtracted, 8.0);
		SetState(DZBBC_DATACORE_TRANSPORT_ACTIVE);
	}

	void DebugForceCombatComplete()
	{
		CompleteCombat();
	}

	void DebugForceBlackbox()
	{
		if (m_State == DZBBC_INACTIVE)
			return;

		CompleteCombat();
	}

	void DebugSetHack(float seconds)
	{
		if (!m_Hack.IsActive())
			return;

		m_Hack.SetProgressSeconds(seconds);
	}

	void DebugGiveDataCore(PlayerBase player)
	{
		if (!player)
			return;

		m_Blackbox.GiveDataCore(player);
		m_DataCore.Begin(player, m_Config.Main.DataCoreLifetimeSeconds);
		SetState(DZBBC_DATACORE_TRANSPORT_ACTIVE);
	}

	void DebugForceTerminal(PlayerBase player)
	{
		if (!player || !m_Terminals.GetActiveTerminal())
			return;

		m_DataCore.Begin(player, m_Config.Main.DataCoreLifetimeSeconds);
		m_Terminals.Begin(player, m_Terminals.GetActiveTerminal(), m_DataCore, m_Config.Main);
		SetState(DZBBC_TERMINAL_DECRYPT_ACTIVE);
		StartFinalDefense();
	}

	void DebugSetDecrypt(float percent)
	{
		m_Terminals.SetProgress(percent);
	}

	float GetDamageMultiplierForSource(EntityAI source)
	{
		return m_AI.GetDamageMultiplierForSource(source);
	}

	void SyncToPlayer(PlayerBase player)
	{
		if (!player)
			return;

		m_Markers.SyncToPlayer(player);
		if (m_Site && IsActive())
			DZBBC_ServerRPC.SendFX(player, DZBBC_FX_CRASHSITE, m_Site.GetPosition());

		if ((m_State == DZBBC_COMBAT_ACTIVE || m_State == DZBBC_FACTION_FIGHT_ACTIVE || m_State == DZBBC_PLAYER_DETECTED) && IsPlayerInHudRange(player))
			DZBBC_ServerRPC.SendHud(player, DZBBC_HUD_ENEMIES, "Verbleibende AI", m_AI.CountAliveObjective(), m_AI.CountObjectiveTotal());
		else
			DZBBC_ServerRPC.SendHud(player, DZBBC_HUD_HIDE, "", 0.0, 0.0);
	}

	void StopAsFailed()
	{
		SetState(DZBBC_FAILED);
		if (m_Config && m_Config.Messages)
			DZBBC_ServerRPC.BroadcastNotification(m_Config.Main.EventName, m_Config.Messages.Failed, 7.0);
		Cleanup();
	}

	void Cleanup()
	{
		if (GetGame())
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(Tick);

		SetState(DZBBC_CLEANUP);
		DZBBC_ServerRPC.BroadcastHud(DZBBC_HUD_HIDE, "", 0.0, 0.0);
		DZBBC_ServerRPC.BroadcastFX(DZBBC_FX_CLEAR, Vector(0.0, 0.0, 0.0));
		m_Markers.RemoveAll();
		m_AI.Cleanup();
		m_Blackbox.Cleanup();
		m_CrashSite.Cleanup();
		m_Terminals.Cleanup();
		m_Loot.Cleanup();
		m_DataCore.Reset();
		m_FinalDefense.Reset();
		CleanupTestObjects();

		if (m_CombatZone && GetGame())
			GetGame().ObjectDelete(m_CombatZone);
		m_CombatZone = null;

		SetState(DZBBC_INACTIVE);
		DZBBC_Utils.Log("Cleanup completed.");
	}

	bool IsActive()
	{
		return m_State != DZBBC_INACTIVE && m_State != DZBBC_CLEANUP;
	}

	string GetDebugState()
	{
		string site = "none";
		vector sitePos = "0 0 0";
		string terminal = "none";
		string lootOpen = "false";
		string stateText = "";

		if (m_Site)
		{
			site = m_Site.Name;
			sitePos = m_Site.GetPosition();
		}

		if (m_Terminals.GetActiveTerminalConfig())
			terminal = m_Terminals.GetActiveTerminalConfig().Name;

		if (m_Loot.IsOpened())
			lootOpen = "true";

		stateText = "state=" + DZBBC_EventStateNames.GetName(m_State);
		stateText = stateText + " site=" + site;
		stateText = stateText + " sitePos=" + sitePos.ToString();
		stateText = stateText + " activeAI=" + m_AI.CountAlive().ToString();
		stateText = stateText + " objectiveAI=" + m_AI.CountAliveObjective().ToString() + "/" + m_AI.CountObjectiveTotal().ToString();
		stateText = stateText + " hack=" + m_Hack.GetPercent().ToString();
		stateText = stateText + " carrier=" + m_DataCore.GetCarrierName();
		stateText = stateText + " transportLeft=" + m_DataCore.GetRemainingSeconds().ToString();
		stateText = stateText + " terminal=" + terminal;
		stateText = stateText + " decrypt=" + m_Terminals.GetPercent().ToString();
		stateText = stateText + " lootOpen=" + lootOpen;
		return stateText;
	}

	protected void CreateCombatZone()
	{
		vector center = m_Site.GetPosition();
		m_CombatZone = DZBBC_CombatZone.Cast(GetGame().CreateObjectEx("DZBBC_CombatZone", center, ECE_NONE));
		if (!m_CombatZone)
		{
			DZBBC_Utils.Error("Could not create combat trigger.");
			return;
		}

		float radius = m_Site.Radius;
		if (radius <= 0.0)
			radius = m_Config.Main.CombatRadius;
		m_CombatZone.Setup(this, radius);
	}

	protected void SpawnInitialFactions()
	{
		m_AI.SpawnFaction(m_Config.AIWaves.RussianZ, m_Config.Main, m_Site.GetPosition());
		m_AI.SpawnFaction(m_Config.AIWaves.AmericanZ, m_Config.Main, m_Site.GetPosition());
	}

	protected void StartTick()
	{
		float seconds = m_Config.Main.TickSeconds;
		if (seconds <= 0.0)
			seconds = 1.0;

		int tickMs = Math.Round(seconds * 1000.0);
		if (tickMs < 250)
			tickMs = 1000;

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(Tick);
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(Tick, tickMs, true);
	}

	protected void TickCombat()
	{
		int alive = m_AI.CountAliveObjective();
		int total = m_AI.CountObjectiveTotal();
		SendCombatHudInRange(alive, total);
		DZBBC_Utils.Log("Combat HUD update: objectiveAI=" + alive.ToString() + "/" + total.ToString());
		if (alive <= 0)
			CompleteCombat();
	}

	protected void CompleteCombat()
	{
		if (m_State == DZBBC_COMBAT_COMPLETE || m_State == DZBBC_BLACKBOX_SIGNAL_FOUND || m_State == DZBBC_BLACKBOX_HACK_ACTIVE)
			return;

		SetState(DZBBC_COMBAT_COMPLETE);
		DZBBC_ServerRPC.BroadcastHud(DZBBC_HUD_HIDE, "", 0.0, 0.0);
		DZBBC_ServerRPC.BroadcastNotification(m_Config.Main.EventName, m_Config.Messages.CombatComplete, 8.0);
		m_Blackbox.Spawn(m_Site);
		if (!m_Blackbox.IsSpawned())
		{
			DZBBC_Utils.Error("Blackbox phase blocked: blackbox object did not spawn.");
			return;
		}

		SetState(DZBBC_BLACKBOX_SIGNAL_FOUND);
		DZBBC_Utils.Log("Blackbox phase active: interaction unlocked at " + m_Blackbox.GetPosition().ToString());
	}

	protected void SendCombatHudInRange(float current, float max)
	{
		ref array<PlayerBase> players = DZBBC_PlayerUtils.GetOnlinePlayers();
		foreach (PlayerBase player: players)
		{
			if (IsPlayerInHudRange(player))
				DZBBC_ServerRPC.SendHud(player, DZBBC_HUD_ENEMIES, "Verbleibende AI", current, max);
			else
				DZBBC_ServerRPC.SendHud(player, DZBBC_HUD_HIDE, "", 0.0, 0.0);
		}
	}

	protected bool IsPlayerInHudRange(PlayerBase player)
	{
		if (!DZBBC_PlayerUtils.IsValidPlayer(player) || !player.IsAlive() || player.IsUnconscious() || !m_Site)
			return false;

		return vector.Distance(player.GetPosition(), m_Site.GetPosition()) <= HUD_RANGE_METERS;
	}

	protected void TickDataCore(float delta)
	{
		if (!m_DataCore.Tick(delta))
		{
			SetState(DZBBC_DATACORE_EXPIRED);
			DZBBC_ServerRPC.BroadcastNotification(m_Config.Main.EventName, "#STR_DZBBC_MSG_DATACORE_UNSTABLE", 7.0);
			Cleanup();
			return;
		}

		m_TransportMarkerTimer += delta;
		if (m_TransportMarkerTimer >= 30.0)
		{
			m_TransportMarkerTimer = 0.0;
			vector center = DZBBC_SectorUtils.GetSectorCenter(m_DataCore.GetCarrierPosition(), m_Config.Main.SectorSize);
			string label = DZBBC_Utils.Localize("#STR_DZBBC_MARKER_TRANSPORT") + ": " + DZBBC_SectorUtils.GetSectorLabel(m_DataCore.GetCarrierPosition(), m_Config.Main.SectorSize);
			m_Markers.ShowTransportHint(label, center);
		}

		if (m_Terminals.GetActiveTerminalConfig())
			SetState(DZBBC_TERMINAL_FOUND);
	}

	protected void TickTerminal(float delta)
	{
		if (!m_FinalDefenseStarted)
			StartFinalDefense();

		if (m_Terminals.Tick(delta))
			CompleteTerminal();
	}

	protected void StartFinalDefense()
	{
		if (m_FinalDefenseStarted)
			return;

		Object terminal = m_Terminals.GetActiveTerminal();
		if (!terminal)
			return;

		m_FinalDefenseStarted = true;
		SetState(DZBBC_FINAL_DEFENSE_ACTIVE);
		m_FinalDefense.Spawn(m_Config.AIWaves.FinalDefense, m_Config.Main, terminal.GetPosition());
		DZBBC_ServerRPC.BroadcastNotification(m_Config.Main.EventName, "#STR_DZBBC_MSG_FINAL_DEFENSE", 8.0);
	}

	protected void CompleteTerminal()
	{
		SetState(DZBBC_TERMINAL_DECRYPT_COMPLETE);
		m_Loot.SpawnReward(m_Site, m_Config.Loot);
		SetState(DZBBC_REWARD_UNLOCKED);
		DZBBC_ServerRPC.BroadcastNotification(m_Config.Main.EventName, m_Config.Messages.RewardUnlocked, 8.0);
		DZBBC_ServerRPC.BroadcastFX(DZBBC_FX_REWARD, m_Site.GetContainerPosition());
		SetState(DZBBC_COMPLETED);
		DZBBC_ServerRPC.BroadcastNotification(m_Config.Main.EventName, m_Config.Messages.Completed, 10.0);
		ScheduleCleanup();
	}

	protected void ScheduleCleanup()
	{
		int cleanupMs = Math.Round(m_Config.Main.CleanupDelaySeconds * 1000.0);
		if (cleanupMs < 1000)
			cleanupMs = 1000;

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(Cleanup, cleanupMs, false);
	}

	protected void SetState(int state)
	{
		m_State = state;
		DZBBC_Utils.Log("State -> " + DZBBC_EventStateNames.GetName(state));
	}

	protected void CleanupTestObjects()
	{
		foreach (Object object: m_TestObjects)
		{
			DZBBC_Utils.DeleteObjectSafe(object);
		}

		m_TestObjects.Clear();
	}
}
