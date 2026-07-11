class DZKOTH_EventInstance
{
	protected ref DZKOTH_ConfigBundle m_Config;
	protected ref DZKOTH_LocationConfig m_Location;
	protected ref array<PlayerBase> m_PlayersInside;
	protected ref DZKOTH_MapMarkerManager m_Markers;
	protected ref DZKOTH_SmokeManager m_Smoke;
	protected ref DZKOTH_WaveManager m_Waves;
	protected ref DZKOTH_BossManager m_Boss;
	protected ref DZKOTH_LootManager m_Loot;
	protected ref DZKOTH_KeycardTracker m_KeycardTracker;
	protected ref DZKOTH_TerminalHackManager m_TerminalHack;
	protected DZKOTH_CaptureZone m_Trigger;
	protected int m_State;
	protected float m_CaptureProgress;
	protected string m_CaptureOwnerUid;
	protected string m_CaptureGroupId;
	protected int m_CaptureExpansionPartyId;
	protected bool m_CapturePausedByEnemy;
	protected int m_ZombieGoal;
	protected int m_LastZombieKilledCount;
	protected int m_LastLoggedCaptureTen;
	protected PlayerBase m_CaptureStarter;
	protected bool m_WaveOneSpawned;
	protected bool m_WaveTwoSpawned;
	protected bool m_WaveThreeSpawned;
	protected bool m_WaveFourSpawned;
	protected bool m_WaveFiveSpawned;
	protected bool m_BossWarningSent;
	protected bool m_BossDeathHandled;

	void DZKOTH_EventInstance(DZKOTH_ConfigBundle config, DZKOTH_LocationConfig location)
	{
		m_Config = config;
		m_Location = location;
		m_PlayersInside = new array<PlayerBase>;
		m_Markers = new DZKOTH_MapMarkerManager;
		m_Smoke = new DZKOTH_SmokeManager;
		m_Waves = new DZKOTH_WaveManager;
		m_Boss = new DZKOTH_BossManager;
		m_Loot = new DZKOTH_LootManager;
		m_KeycardTracker = new DZKOTH_KeycardTracker;
		m_TerminalHack = new DZKOTH_TerminalHackManager;
		m_State = DZKOTH_States.INACTIVE;
		m_CaptureExpansionPartyId = -1;
		m_LastLoggedCaptureTen = -1;
	}

	bool Start()
	{
		if (!GetGame() || !GetGame().IsServer() || !m_Config || !m_Config.Main || !m_Location)
			return false;

		vector center = DZKOTH_Utils.Grounded(m_Location.GetPosition());
		m_Trigger = DZKOTH_CaptureZone.Cast(GetGame().CreateObjectEx("DZKOTH_CaptureZone", center, ECE_NONE));
		if (!m_Trigger)
		{
			DZKOTH_Utils.Error("Could not create capture trigger.");
			return false;
		}

		float radius = GetCaptureRadius();
		m_Trigger.Setup(this, radius);
		RemoveLegacySpeakersNear(center);

		m_Smoke.Setup(m_Location.GetFlagPosition(), m_Location.GetFlagOrientation());
		m_Smoke.SetReady();
		m_Smoke.SetFlagRaiseProgress(0.0);

		m_State = DZKOTH_States.WAITING_FOR_PLAYER;
		m_Markers.ShowReady(m_Location);
		DZKOTH_ServerRPC.BroadcastWarning("DeutschZ KotH wurde aktiviert.", "Betrete die Zone und halte den Mast.", 10.0);
		StartTick();

		DZKOTH_Utils.Log("Event started: " + m_Location.Name);
		return true;
	}

	protected void RemoveLegacySpeakersNear(vector center)
	{
		if (!GetGame() || !GetGame().IsServer())
			return;

		array<Object> objects = new array<Object>;
		array<CargoBase> proxies = new array<CargoBase>;
		GetGame().GetObjectsAtPosition3D(center, 100.0, objects, proxies);

		int removed = 0;
		foreach (Object object: objects)
		{
			if (!object)
				continue;

			string typeName = object.GetType();
			if (typeName != "NoxZ_Static_Speaker" && typeName != "NoxZ_Speaker")
				continue;

			GetGame().ObjectDelete(object);
			removed++;
		}

		if (removed > 0)
			DZKOTH_Utils.Log("Removed " + removed.ToString() + " legacy NoxZ speaker(s) from the active KotH area.");
	}

	void OnPlayerEntered(PlayerBase player)
	{
		if (!IsValidPlayer(player))
			return;

		AddPlayerInside(player);
		SyncStateToPlayer(player);

		if (m_State == DZKOTH_States.WAITING_FOR_PLAYER)
			BeginCapture(player);
	}

	void OnPlayerLeft(PlayerBase player)
	{
		if (!player)
			return;

		CancelTerminalHack(player);
		RemovePlayerInside(player);
		DZKOTH_ServerRPC.SendHud(player, DZKOTH_ProgressModes.HIDE, "", 0.0, 0.0);
	}

	void Tick()
	{
		CleanupPlayerList();

		if (m_State == DZKOTH_States.ZOMBIE_PREPHASE)
			TickZombiePrephase();
		else if (m_State == DZKOTH_States.WAITING_FOR_TERMINAL_HACK)
			BroadcastChestHudToOnlinePlayers();
		else if (m_State == DZKOTH_States.TERMINAL_HACK_ACTIVE)
			TickTerminalHack();
		else if (m_State == DZKOTH_States.WAITING_FOR_PLAYER)
			BroadcastCaptureReadyHudToNearbyPlayers();
		else if (m_State == DZKOTH_States.CAPTURING)
			TickCapture();
		else if (m_State == DZKOTH_States.REWARD_ACTIVE)
			TickRewardPhase();
	}

	void SyncStateToPlayer(PlayerBase player)
	{
		if (!player)
			return;

		m_Markers.SyncToPlayer(player);

		if (m_State == DZKOTH_States.ZOMBIE_PREPHASE)
		{
			SendZombieHud(player);
		}
		else if (m_State == DZKOTH_States.WAITING_FOR_TERMINAL_HACK || m_State == DZKOTH_States.TERMINAL_HACK_ACTIVE)
		{
			SyncTerminalHudToPlayer(player);
		}
		else if (IsCaptureState())
		{
			DZKOTH_ServerRPC.SendHud(player, DZKOTH_ProgressModes.CAPTURE, GetCaptureHudLabel(), m_CaptureProgress, 100.0);
		}
		else if (m_State == DZKOTH_States.REWARD_ACTIVE || m_State == DZKOTH_States.COMPLETED)
		{
			DZKOTH_ServerRPC.SendHud(player, DZKOTH_ProgressModes.CAPTURE, "KotH abgeschlossen", 100.0, 100.0);
		}
		else if (m_State == DZKOTH_States.WAITING_FOR_PLAYER)
		{
			DZKOTH_ServerRPC.SendHud(player, DZKOTH_ProgressModes.CAPTURE, "Fahnenmast aktiviert. Zone halten.", 0.0, 100.0);
		}
		else
		{
			DZKOTH_ServerRPC.SendHud(player, DZKOTH_ProgressModes.HIDE, "", 0.0, 0.0);
		}
	}

	bool IsPositionInsideActiveZone(vector pos)
	{
		if (!m_Location || m_State == DZKOTH_States.INACTIVE || m_State == DZKOTH_States.CLEANUP)
			return false;

		return vector.Distance(pos, m_Location.GetPosition()) <= GetCaptureRadius();
	}

	bool IsManagedInfected(Object object)
	{
		if (m_Waves && m_Waves.IsManagedInfected(object))
			return true;

		return m_Boss && m_Boss.IsBoss(EntityAI.Cast(object));
	}

	float GetDamageMultiplierForSource(EntityAI source)
	{
		if (!source)
			return 1.0;

		if (m_Boss)
		{
			float bossMultiplier = m_Boss.GetDamageMultiplier(source);
			if (bossMultiplier > 1.0)
				return bossMultiplier;
		}

		if (m_Waves)
			return m_Waves.GetDamageMultiplier(source);

		return 1.0;
	}

	void CleanupEvent(bool returnToReady = false)
	{
		bool notifyAbort = m_State != DZKOTH_States.INACTIVE && m_State != DZKOTH_States.COMPLETED && m_State != DZKOTH_States.REWARD_ACTIVE;
		if (notifyAbort)
			DZKOTH_ServerRPC.BroadcastWarning("DeutschZ KotH", "Event abgebrochen.", 7.0);

		vector cleanupPos = vector.Zero;
		if (m_Location)
			cleanupPos = m_Location.GetPosition();
		DZKOTH_ServerRPC.BroadcastFXGlobal(DZKOTH_FXIds.CLEAR, cleanupPos);

		if (GetGame())
		{
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(Tick);
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(ScheduledCleanupEvent);
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(HideHudForOnlinePlayers);
		}

		HideHudForOnlinePlayers();
		if (m_Waves)
			m_Waves.Cleanup();
		if (m_Boss)
			m_Boss.Cleanup();
		if (m_Loot)
			m_Loot.Cleanup();
		if (m_TerminalHack)
			m_TerminalHack.Cleanup();
		if (m_KeycardTracker)
			m_KeycardTracker.Reset();
		if (m_Smoke)
			m_Smoke.DeleteFlagpole();
		if (m_Markers)
			m_Markers.Remove();

		ResetCapture();
		m_State = DZKOTH_States.INACTIVE;
		DZKOTH_Utils.Log("Cleanup complete");

		if (returnToReady)
			Start();
	}

	void Destroy()
	{
		CleanupEvent(false);

		if (m_Trigger && GetGame())
			GetGame().ObjectDelete(m_Trigger);
		m_Trigger = null;

		m_State = DZKOTH_States.INACTIVE;
	}

	void ScheduledCleanupEvent()
	{
		Destroy();
		DZKOTH_EventManager.GetInstance().ScheduleNextEvent();
	}

	int GetState()
	{
		return m_State;
	}

	float GetCaptureProgress()
	{
		return m_CaptureProgress;
	}

	float GetTerminalHackProgress()
	{
		if (!m_TerminalHack || !m_Config || !m_Config.Main)
			return 0.0;

		return m_TerminalHack.GetProgressPercent(m_Config.Main);
	}

	void DebugForceBoss()
	{
		DZKOTH_Utils.Warn("ForceBoss maps to KotH completion in the current Vybor KotH flow.");
		CompleteCapturePhase();
	}

	void DebugForceTerminalHack()
	{
		if (!m_TerminalHack)
			return;

		if (m_State == DZKOTH_States.ZOMBIE_PREPHASE)
			OnZombiePrephaseComplete();

		m_TerminalHack.ForceComplete(this);
		DZKOTH_Utils.Log("Admin debug: chest action forced.");
	}

	void DebugSetTerminalHack(float progress)
	{
		if (!m_TerminalHack || !m_Config || !m_Config.Main)
			return;

		if (m_State == DZKOTH_States.ZOMBIE_PREPHASE)
			OnZombiePrephaseComplete();

		m_TerminalHack.SetProgressPercent(progress, m_Config.Main);
		if (progress >= 100.0)
			m_TerminalHack.ForceComplete(this);
		else
			SyncTerminalHudToPlayersInside();
	}

	void DebugSetProgress(float progress)
	{
		if (m_State < DZKOTH_States.WAITING_FOR_PLAYER)
			DebugForceTerminalHack();

		if (m_State == DZKOTH_States.WAITING_FOR_PLAYER)
			ActivateCaptureGate(null);

		m_CaptureProgress = Math.Clamp(progress, 0.0, 100.0);
		if (m_Smoke)
			m_Smoke.SetFlagRaiseProgress(m_CaptureProgress * 0.01);

		if (m_CaptureProgress >= 100.0)
			CompleteCapturePhase();
		else
			BroadcastCaptureHudToPlayersInside();
	}

	bool DebugStartCapture(PlayerBase player)
	{
		if (!IsValidPlayer(player))
			return false;

		if (m_State == DZKOTH_States.ZOMBIE_PREPHASE)
			OnZombiePrephaseComplete();

		if (m_State < DZKOTH_States.WAITING_FOR_PLAYER)
			DebugForceTerminalHack();

		AddPlayerInside(player);
		if (m_State == DZKOTH_States.WAITING_FOR_PLAYER)
		{
			BeginCapture(player);
			return true;
		}

		if (IsCaptureState())
		{
			SyncStateToPlayer(player);
			return true;
		}

		return false;
	}

	bool BeginTerminalHack(PlayerBase player, Object terminal)
	{
		if (!RequiresTerminalHack())
			return false;

		if (m_State != DZKOTH_States.WAITING_FOR_TERMINAL_HACK)
			return false;

		if (!m_TerminalHack.Begin(player, terminal, m_Config.Main))
			return false;

		m_State = DZKOTH_States.TERMINAL_HACK_ACTIVE;
		StartTick();
		SyncTerminalHudToPlayersInside();
		return true;
	}

	bool CancelTerminalHack(PlayerBase player, Object terminal = null)
	{
		if (!m_TerminalHack || m_State != DZKOTH_States.TERMINAL_HACK_ACTIVE)
			return false;

		if (!m_TerminalHack.Cancel(player, terminal))
			return false;

		m_State = DZKOTH_States.WAITING_FOR_TERMINAL_HACK;
		BroadcastChestHudToOnlinePlayers();
		return true;
	}

	bool StartTerminalHack(PlayerBase player, Object terminal)
	{
		if (!RequiresTerminalHack())
			return false;

		if (m_State != DZKOTH_States.TERMINAL_HACK_ACTIVE)
			return false;

		if (!m_TerminalHack.CompleteFromAction(player, terminal, m_Config.Main))
			return false;

		m_State = DZKOTH_States.TERMINAL_HACK_COMPLETE;
		DZKOTH_ServerRPC.BroadcastHud(m_PlayersInside, DZKOTH_ProgressModes.CAPTURE, "Kiste wird aufgebrochen: 100%", 100.0, 100.0);
		OnTerminalHackComplete(player);
		return true;
	}

	void OnTerminalHackComplete(PlayerBase hacker)
	{
		if (!RequiresTerminalHack())
			return;

		if (m_State >= DZKOTH_States.WAITING_FOR_PLAYER)
			return;

		m_State = DZKOTH_States.TERMINAL_HACK_COMPLETE;
		DZKOTH_ServerRPC.BroadcastPlayerUIMessage(CollectPlayersInHudRange(), "Fahnenmast aktiviert.", "Halte die Stellung und hisse die Flagge.", 5.0);
		ActivateCaptureGate(hacker);
	}

	protected void StartZombiePrephase()
	{
		m_State = DZKOTH_States.ZOMBIE_PREPHASE;
		m_LastZombieKilledCount = 0;
		m_ZombieGoal = m_Config.Main.ZombieCount;
		m_Waves.Cleanup();
		m_Waves.SpawnWaveForPlayers(null, m_Config.Waves.WaveOne, m_Config.Main, m_Location.GetFlagPosition(), m_ZombieGoal);

		int spawned = m_Waves.CountSpawned();
		if (spawned > 0)
			m_ZombieGoal = spawned;
		else
			DZKOTH_Utils.Warn("No zombies spawned for prephase; unlocking chest to avoid a stuck event.");

		BroadcastZombieHudToOnlinePlayers();
		if (spawned <= 0)
			OnZombiePrephaseComplete();
	}

	protected void TickZombiePrephase()
	{
		int killed = m_Waves.CountDead();
		if (killed < 0)
			killed = 0;
		if (killed > m_ZombieGoal)
			killed = m_ZombieGoal;

		if (killed > m_LastZombieKilledCount)
		{
			for (int killIndex = m_LastZombieKilledCount + 1; killIndex <= killed; killIndex++)
				DZKOTH_Utils.Log("Zombie killed " + killIndex.ToString() + "/" + m_ZombieGoal.ToString());

			m_LastZombieKilledCount = killed;
		}

		BroadcastZombieHudToOnlinePlayers();

		if (m_ZombieGoal <= 0 || killed >= m_ZombieGoal)
			OnZombiePrephaseComplete();
	}

	protected void OnZombiePrephaseComplete()
	{
		if (m_State != DZKOTH_States.ZOMBIE_PREPHASE)
			return;

		m_State = DZKOTH_States.WAITING_FOR_TERMINAL_HACK;
		m_TerminalHack.SetActionUnlocked(true);
		DZKOTH_Utils.Log("Chest action unlocked");
		DZKOTH_ServerRPC.BroadcastPlayerUIMessage(CollectPlayersInHudRange(), m_Config.Main.EventName, "Zone gesichert. Hacke die Versorgungskiste, um den Fahnenmast zu aktivieren.", 5.0);
		BroadcastChestHudToOnlinePlayers();
	}

	protected void BeginCapture(PlayerBase starter)
	{
		if (!IsValidPlayer(starter))
			return;

		if (RequiresTerminalHack() && m_State < DZKOTH_States.WAITING_FOR_PLAYER)
		{
			DZKOTH_ServerRPC.SendPlayerUIMessage(starter, m_Config.Main.EventName, "Kiste zuerst aufbrechen.", 4.0);
			return;
		}

		ResetCapture();
		m_State = DZKOTH_States.CAPTURING;
		m_CaptureOwnerUid = DZKOTH_GroupResolver.GetPlayerUid(starter);
		m_CaptureGroupId = DZKOTH_GroupResolver.GetPlayerGroupId(m_CaptureOwnerUid);
		m_CaptureExpansionPartyId = DZKOTH_GroupResolver.GetExpansionPartyId(starter);
		m_CaptureStarter = starter;
		m_LastLoggedCaptureTen = -1;
		m_WaveOneSpawned = false;
		m_WaveTwoSpawned = false;
		m_WaveThreeSpawned = false;
		m_WaveFourSpawned = false;
		m_WaveFiveSpawned = false;
		m_BossWarningSent = false;
		m_BossDeathHandled = false;

		m_Smoke.SetReady();
		m_Smoke.SetFlagRaiseProgress(0.0);
		m_Markers.ShowCapture(m_Location);
		SpawnWaveAroundPlayer(starter, m_Config.Waves.WaveOne);
		m_WaveOneSpawned = true;
		DZKOTH_ServerRPC.BroadcastWarningToPlayers(CollectPlayersInHudRange(), m_Config.Main.EventName, "Capture laeuft. Flagge wird gehisst.", 7.0);
		DZKOTH_ServerRPC.BroadcastFX(CollectPlayersInHudRange(), DZKOTH_FXIds.EVENT_START, m_Location.GetPosition());
		BroadcastCaptureHudToNearbyPlayers();
		StartTick();
	}

	protected void ActivateCaptureGate(PlayerBase preferredStarter)
	{
		if (!m_Smoke)
			return;

		m_Smoke.Setup(m_Location.GetFlagPosition(), m_Location.GetFlagOrientation());
		m_Smoke.SetReady();
		m_Smoke.SetFlagRaiseProgress(0.0);
		m_State = DZKOTH_States.WAITING_FOR_PLAYER;
		DZKOTH_Utils.Log("State changed to WAITING_FOR_PLAYER at " + m_Location.Name);

		if (IsValidPlayer(preferredStarter) && m_PlayersInside.Find(preferredStarter) >= 0)
		{
			BeginCapture(preferredStarter);
			return;
		}

		foreach (PlayerBase player: m_PlayersInside)
		{
			if (IsValidPlayer(player))
			{
				BeginCapture(player);
				return;
			}
		}

		BroadcastCaptureReadyHudToOnlinePlayers();
	}

	protected void TickTerminalHack()
	{
		if (!m_TerminalHack || !m_Config || !m_Config.Main)
			return;

		float delta = m_Config.Main.TickSeconds;
		if (delta <= 0.0)
			delta = 1.0;

		if (m_TerminalHack.Tick(delta, this, m_Config.Main))
			return;

		if (!m_TerminalHack.IsActive())
		{
			m_State = DZKOTH_States.WAITING_FOR_TERMINAL_HACK;
			BroadcastChestHudToOnlinePlayers();
			return;
		}

		SyncTerminalHudToPlayersInside();
	}

	protected void TickCapture()
	{
		int validCount = CountValidPlayersInside();
		if (validCount < m_Config.Main.RequirePlayersInsideZone)
		{
			DecayOrWaitForPlayers();
			return;
		}

		if (IsCaptureBlockedByEnemy())
		{
			if (!m_CapturePausedByEnemy)
			{
				m_CapturePausedByEnemy = true;
				DZKOTH_ServerRPC.BroadcastPlayerUIMessage(m_PlayersInside, m_Config.Main.EventName, "Capture pausiert: Gegner in der Zone.", 4.0);
			}

			DZKOTH_ServerRPC.BroadcastHud(CollectPlayersInHudRange(), DZKOTH_ProgressModes.CAPTURE, "KotH-Eroberung pausiert: Gegner in der Zone", m_CaptureProgress, 100.0);
			return;
		}

		if (m_CapturePausedByEnemy)
		{
			m_CapturePausedByEnemy = false;
			DZKOTH_ServerRPC.BroadcastPlayerUIMessage(m_PlayersInside, m_Config.Main.EventName, "Capture laeuft weiter.", 4.0);
		}

		float multiplier = 1.0;
		if (m_Config.Main.AllowMultiplePlayersToSpeedUpCapture)
			multiplier = validCount;

		float captureSeconds = m_Config.Main.CaptureTimeSeconds;
		if (captureSeconds < 1.0)
			captureSeconds = 1.0;

		m_CaptureProgress = Math.Clamp(m_CaptureProgress + ((m_Config.Main.TickSeconds * multiplier / captureSeconds) * 100.0), 0.0, 100.0);
		m_Smoke.SetFlagRaiseProgress(m_CaptureProgress * 0.01);
		LogCaptureProgress();
		HandleProgressMilestones();
		BroadcastCaptureHudToNearbyPlayers();

		if (m_CaptureProgress >= 100.0)
			CompleteCapturePhase();
	}

	protected void DecayOrWaitForPlayers()
	{
		if (m_Config.Main.ProgressLossWhenEmpty && m_PlayersInside.Count() == 0 && m_CaptureProgress > 0.0)
		{
			m_CaptureProgress = Math.Clamp(m_CaptureProgress - (m_Config.Main.ProgressLossPerSecond * m_Config.Main.TickSeconds), 0.0, 100.0);
			m_Smoke.SetFlagRaiseProgress(m_CaptureProgress * 0.01);
		}

		DZKOTH_ServerRPC.BroadcastHud(CollectPlayersInHudRange(), DZKOTH_ProgressModes.CAPTURE, "KotH-Eroberung pausiert: Kein Spieler", m_CaptureProgress, 100.0);
	}

	protected void CompleteCapturePhase()
	{
		if (m_State == DZKOTH_States.REWARD_ACTIVE || m_State == DZKOTH_States.COMPLETED || m_State == DZKOTH_States.CLEANUP)
			return;

		m_CaptureProgress = 100.0;
		m_State = DZKOTH_States.CAPTURE_COMPLETE;
		m_Smoke.SetFlagRaiseProgress(1.0);
		m_Smoke.SetCompleted();
		m_Markers.ShowCompleted(m_Location);
		ref array<PlayerBase> nearbyPlayers = CollectPlayersInHudRange();
		DZKOTH_ServerRPC.BroadcastHud(nearbyPlayers, DZKOTH_ProgressModes.CAPTURE, "KotH abgeschlossen", 100.0, 100.0);
		DZKOTH_ServerRPC.BroadcastFX(nearbyPlayers, DZKOTH_FXIds.FIREWORKS, m_Location.GetPosition());
		DZKOTH_ServerRPC.BroadcastWarning("DeutschZ KotH", GetWinnerName() + " gewinnt KotH.", 10.0);

		m_State = DZKOTH_States.REWARD_ACTIVE;
		DZKOTH_Utils.Log("Event completed");
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(SpawnRewardSequenceAtFlag, 3000, false);

		int cleanupMs = m_Config.Main.CleanupDelayMinutes * 60000;
		if (cleanupMs < 1000)
			cleanupMs = 1000;
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ScheduledCleanupEvent, cleanupMs, false);
	}

	protected void SpawnRewardSequenceAtFlag()
	{
		if (!m_Location || m_State != DZKOTH_States.REWARD_ACTIVE)
			return;

		if (m_Smoke)
			m_Smoke.DeleteFlagpole();

		if (m_Loot)
			m_Loot.SpawnRewardCrate(m_Location, m_Config.Loot, m_Config.Main);

		vector rewardPos = m_Location.GetFlagPosition();
		SpawnRewardFireworks(rewardPos);
		SpawnBossForWinner();
		DZKOTH_Utils.Log("Reward sequence spawned at former flagpole position " + rewardPos.ToString());
	}

	protected bool IsCaptureBlockedByEnemy()
	{
		if (!m_Config || !m_Config.Main || !m_Config.Main.EnemyPlayersBlockCapture)
			return false;

		bool hasCaptureTeamMember = false;
		foreach (PlayerBase player: m_PlayersInside)
		{
			if (!IsValidPlayer(player))
				continue;

			if (DZKOTH_GroupResolver.IsCaptureTeamMember(player, m_CaptureOwnerUid, m_CaptureGroupId, m_CaptureExpansionPartyId))
			{
				hasCaptureTeamMember = true;
				continue;
			}

			return true;
		}

		return !hasCaptureTeamMember;
	}

	protected bool IsCaptureState()
	{
		return m_State == DZKOTH_States.CAPTURING;
	}

	protected bool RequiresTerminalHack()
	{
		return m_Config && m_Config.Main && m_Config.Main.RequireTerminalHackBeforeCapture;
	}

	protected float GetCaptureRadius()
	{
		float radius = 35.0;
		if (m_Location && m_Location.Radius > 0.0)
			radius = m_Location.Radius;
		else if (m_Config && m_Config.Main && m_Config.Main.CaptureRadius > 0.0)
			radius = m_Config.Main.CaptureRadius;

		return radius;
	}

	protected string GetCaptureHudLabel()
	{
		return "KotH-Eroberung laeuft: " + Math.Round(m_CaptureProgress).ToString() + "%";
	}

	protected void HandleProgressMilestones()
	{
		if (!m_Config || !m_Config.Waves)
			return;

		if (!m_WaveTwoSpawned && m_Config.Waves.WaveTwo && m_CaptureProgress >= m_Config.Waves.WaveTwo.TriggerProgress)
		{
			m_WaveTwoSpawned = true;
			SpawnWaveAroundPlayer(GetBestCapturePlayer(), m_Config.Waves.WaveTwo);
			DZKOTH_ServerRPC.BroadcastPlayerUIMessage(CollectPlayersInHudRange(), m_Config.Main.EventName, "Polizei-Infizierte ruecken an.", 4.0);
		}

		if (!m_WaveThreeSpawned && m_Config.Waves.WaveThree && m_CaptureProgress >= m_Config.Waves.WaveThree.TriggerProgress)
		{
			m_WaveThreeSpawned = true;
			SpawnWaveAroundPlayer(GetBestCapturePlayer(), m_Config.Waves.WaveThree);
			DZKOTH_ServerRPC.BroadcastPlayerUIMessage(CollectPlayersInHudRange(), m_Config.Main.EventName, "Verstaerkte Militaer-Infizierte greifen an.", 4.0);
		}

		if (!m_WaveFourSpawned && m_Config.Waves.WaveFour && m_CaptureProgress >= m_Config.Waves.WaveFour.TriggerProgress)
		{
			m_WaveFourSpawned = true;
			SpawnWaveAroundPlayer(GetBestCapturePlayer(), m_Config.Waves.WaveFour);
			DZKOTH_ServerRPC.BroadcastPlayerUIMessage(CollectPlayersInHudRange(), m_Config.Main.EventName, "Schwere Militaer-Infizierte umstellen die Zone.", 4.0);
		}

		if (!m_WaveFiveSpawned && m_Config.Waves.WaveFive && m_CaptureProgress >= m_Config.Waves.WaveFive.TriggerProgress)
		{
			m_WaveFiveSpawned = true;
			SpawnWaveAroundPlayer(GetBestCapturePlayer(), m_Config.Waves.WaveFive);
			DZKOTH_ServerRPC.BroadcastPlayerUIMessage(CollectPlayersInHudRange(), m_Config.Main.EventName, "Die letzte Infiziertenwelle ist da.", 4.0);
		}

		if (!m_BossWarningSent && m_CaptureProgress >= 80.0)
		{
			m_BossWarningSent = true;
			DZKOTH_ServerRPC.BroadcastFX(CollectPlayersInHudRange(), DZKOTH_FXIds.BOSS_WARNING, m_Location.GetPosition());
			DZKOTH_ServerRPC.BroadcastPlayerUIMessage(CollectPlayersInHudRange(), "DeutschZ KotH", "Ein uralter Schrei erschuettert den Huegel.", 5.0);
		}
	}

	protected void SpawnWaveAroundPlayer(PlayerBase player, DZKOTH_WaveConfig wave)
	{
		if (!m_Waves || !wave)
			return;

		m_Waves.SpawnWaveForPlayers(null, wave, m_Config.Main, m_Location.GetFlagPosition());
	}

	protected PlayerBase GetBestCapturePlayer()
	{
		if (IsValidPlayer(m_CaptureStarter))
			return m_CaptureStarter;

		foreach (PlayerBase player: m_PlayersInside)
		{
			if (IsValidPlayer(player))
				return player;
		}

		return null;
	}

	protected string GetWinnerName()
	{
		PlayerBase winner = GetBestCapturePlayer();
		if (winner && winner.GetIdentity())
			return winner.GetIdentity().GetName();

		return "Ein Ueberlebender";
	}

	protected void SpawnBossForWinner()
	{
		if (!m_Boss || !m_Config || !m_Config.Main)
			return;

		PlayerBase winner = GetBestCapturePlayer();
		bool spawned = false;
		if (IsValidPlayer(winner))
			spawned = m_Boss.SpawnBossNearPlayer(winner, m_Location, m_Config.Main);
		else
			spawned = m_Boss.SpawnBoss(m_Location, m_Config.Main);

		if (!spawned)
			return;

		m_Markers.ShowBoss(m_Location);
		DZKOTH_ServerRPC.BroadcastFX(CollectPlayersInHudRange(), DZKOTH_FXIds.BOSS_SPAWN, m_Boss.GetPosition());
		DZKOTH_ServerRPC.BroadcastPlayerUIMessage(CollectPlayersInHudRange(), "Achtung!", "BosZ Mumie erschienen.", 6.0);
		BroadcastBossHudToNearbyPlayers();
	}

	protected void SpawnRewardFireworks(vector center)
	{
		if (!GetGame())
			return;

		for (int i = 0; i < 3; i++)
		{
			float angle = (2.094395 * i) + Math.RandomFloatInclusive(-0.25, 0.25);
			vector pos = center + Vector(Math.Cos(angle) * 15.0, 0, Math.Sin(angle) * 15.0);
			pos = DZKOTH_Utils.Grounded(pos);
			Object object = GetGame().CreateObjectEx(DZKOTH_Const.FIREWORKS_BATTERY_CLASSNAME, pos, ECE_PLACE_ON_SURFACE);
			EntityAI battery = EntityAI.Cast(object);
			if (battery)
			{
				battery.SetOrientation(Vector(Math.RandomFloatInclusive(0.0, 359.0), 0, 0));
				GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(IgniteRewardFirework, 1200 + (i * 700), false, battery);
				GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(CleanupRewardFirework, 120000, false, battery);
			}
		}
	}

	protected void IgniteRewardFirework(EntityAI battery)
	{
		if (!battery)
			return;

		battery.OnIgnitedThis(null);
	}

	protected void CleanupRewardFirework(EntityAI battery)
	{
		if (battery && GetGame())
			GetGame().ObjectDelete(battery);
	}

	protected void TickRewardPhase()
	{
		if (!m_Boss)
			return;

		if (m_Boss.IsAlive())
		{
			BroadcastBossHudToNearbyPlayers();
			return;
		}

		if (!m_BossDeathHandled)
		{
			m_BossDeathHandled = true;
			DZKOTH_ServerRPC.BroadcastFX(CollectPlayersInHudRange(), DZKOTH_FXIds.BOSS_DEATH, m_Location.GetPosition());
			DZKOTH_ServerRPC.BroadcastPlayerUIMessage(CollectPlayersInHudRange(), "DeutschZ KotH", "BosZ Mumie wurde erledigt.", 5.0);
		}
	}

	protected void DeleteFlagpoleAfterWin()
	{
		if (m_Smoke)
			m_Smoke.DeleteFlagpole();
	}

	protected void LogCaptureProgress()
	{
		int currentTen = Math.Floor(m_CaptureProgress / 10.0);
		if (currentTen <= m_LastLoggedCaptureTen || currentTen <= 0)
			return;

		m_LastLoggedCaptureTen = currentTen;
		DZKOTH_Utils.Log("Capture progress " + (currentTen * 10).ToString() + "%");
	}

	protected void StartTick()
	{
		if (!GetGame() || !m_Config || !m_Config.Main)
			return;

		int tickMs = Math.Round(m_Config.Main.TickSeconds * 1000.0);
		if (tickMs < 250)
			tickMs = 1000;

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(Tick);
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(Tick, tickMs, true);
	}

	protected void AddPlayerInside(PlayerBase player)
	{
		if (m_PlayersInside.Find(player) < 0)
			m_PlayersInside.Insert(player);
	}

	protected void RemovePlayerInside(PlayerBase player)
	{
		int index = m_PlayersInside.Find(player);
		if (index >= 0)
			m_PlayersInside.Remove(index);
	}

	protected void CleanupPlayerList()
	{
		for (int i = m_PlayersInside.Count() - 1; i >= 0; i--)
		{
			PlayerBase player = m_PlayersInside.Get(i);
			if (!IsValidPlayer(player))
				m_PlayersInside.Remove(i);
		}
	}

	protected int CountValidPlayersInside()
	{
		int count = 0;
		foreach (PlayerBase player: m_PlayersInside)
		{
			if (IsValidPlayer(player))
				count++;
		}

		return count;
	}

	protected bool IsValidPlayer(PlayerBase player)
	{
		return player && player.GetIdentity() && player.IsAlive() && !player.IsUnconscious();
	}

	protected void ResetCapture()
	{
		m_CaptureProgress = 0.0;
		m_CaptureOwnerUid = "";
		m_CaptureGroupId = "";
		m_CaptureExpansionPartyId = -1;
		m_CapturePausedByEnemy = false;
		m_LastLoggedCaptureTen = -1;
		m_CaptureStarter = null;
		m_WaveOneSpawned = false;
		m_WaveTwoSpawned = false;
		m_WaveThreeSpawned = false;
		m_WaveFourSpawned = false;
		m_WaveFiveSpawned = false;
		m_BossWarningSent = false;
		m_BossDeathHandled = false;
	}

	protected ref array<PlayerBase> CollectOnlinePlayers()
	{
		ref array<PlayerBase> players = new array<PlayerBase>;
		if (!GetGame())
			return players;

		array<Man> mans = new array<Man>;
		GetGame().GetPlayers(mans);
		foreach (Man man: mans)
		{
			PlayerBase player = PlayerBase.Cast(man);
			if (player)
				players.Insert(player);
		}

		return players;
	}

	protected ref array<PlayerBase> CollectPlayersInHudRange()
	{
		ref array<PlayerBase> players = new array<PlayerBase>;
		if (!m_Location)
			return players;

		float radius = 500.0;
		if (m_Config && m_Config.Main && m_Config.Main.ProgressHudRadius > 0.0)
			radius = m_Config.Main.ProgressHudRadius;

		vector center = m_Location.GetPosition();
		ref array<PlayerBase> onlinePlayers = CollectOnlinePlayers();
		foreach (PlayerBase player: onlinePlayers)
		{
			if (player && vector.Distance(player.GetPosition(), center) <= radius)
				players.Insert(player);
		}

		return players;
	}

	protected void HideHudForOnlinePlayers()
	{
		ref array<PlayerBase> onlinePlayers = CollectOnlinePlayers();
		foreach (PlayerBase player: onlinePlayers)
		{
			if (player)
				DZKOTH_ServerRPC.SendHud(player, DZKOTH_ProgressModes.HIDE, "", 0.0, 0.0);
		}
	}

	protected void BroadcastZombieHudToOnlinePlayers()
	{
		ref array<PlayerBase> onlinePlayers = CollectOnlinePlayers();
		foreach (PlayerBase player: onlinePlayers)
		{
			if (player)
				SendZombieHud(player);
		}
	}

	protected void SendZombieHud(PlayerBase player)
	{
		if (!player)
			return;

		int killed = m_Waves.CountDead();
		if (killed < 0)
			killed = 0;
		if (killed > m_ZombieGoal)
			killed = m_ZombieGoal;

		string label = "KotH sichern: " + killed.ToString() + "/" + m_ZombieGoal.ToString() + " Infizierte erledigt";
		DZKOTH_ServerRPC.SendHud(player, DZKOTH_ProgressModes.CAPTURE, label, killed, m_ZombieGoal);
	}

	protected void BroadcastChestHudToOnlinePlayers()
	{
		ref array<PlayerBase> onlinePlayers = CollectOnlinePlayers();
		foreach (PlayerBase player: onlinePlayers)
		{
			if (player)
				SyncTerminalHudToPlayer(player);
		}
	}

	protected void BroadcastCaptureReadyHudToOnlinePlayers()
	{
		BroadcastCaptureReadyHudToNearbyPlayers();
	}

	protected void BroadcastCaptureReadyHudToNearbyPlayers()
	{
		ref array<PlayerBase> nearbyPlayers = CollectPlayersInHudRange();
		foreach (PlayerBase player: nearbyPlayers)
		{
			if (player)
				DZKOTH_ServerRPC.SendHud(player, DZKOTH_ProgressModes.CAPTURE, "Fahnenmast aktiviert. Zone halten.", 0.0, 100.0);
		}
	}

	protected void BroadcastCaptureHudToPlayersInside()
	{
		BroadcastCaptureHudToNearbyPlayers();
	}

	protected void BroadcastCaptureHudToNearbyPlayers()
	{
		DZKOTH_ServerRPC.BroadcastHud(CollectPlayersInHudRange(), DZKOTH_ProgressModes.CAPTURE, GetCaptureHudLabel(), m_CaptureProgress, 100.0);
	}

	protected void BroadcastBossHudToNearbyPlayers()
	{
		if (!m_Boss)
			return;

		DZKOTH_ServerRPC.BroadcastHud(CollectPlayersInHudRange(), DZKOTH_ProgressModes.BOSS, "BosZ Mumie", m_Boss.GetHealth(), m_Boss.GetMaxHealth());
	}

	protected void SyncTerminalHudToPlayersInside()
	{
		foreach (PlayerBase player: m_PlayersInside)
		{
			if (player)
				SyncTerminalHudToPlayer(player);
		}
	}

	protected void SyncTerminalHudToPlayer(PlayerBase player)
	{
		if (!player || !m_TerminalHack || !m_Config || !m_Config.Main)
			return;

		float progress = m_TerminalHack.GetProgressPercent(m_Config.Main);
		string label = "Zone gesichert. Kiste vorbereiten.";
		if (m_State == DZKOTH_States.TERMINAL_HACK_ACTIVE)
			label = "Kiste wird aufgebrochen: " + Math.Round(progress).ToString() + "%";

		DZKOTH_ServerRPC.SendHud(player, DZKOTH_ProgressModes.CAPTURE, label, progress, 100.0);
	}

	string GetDebugState()
	{
		string text = "state=" + m_State.ToString() + " progress=" + m_CaptureProgress.ToString();
		text = text + " zombies=" + m_LastZombieKilledCount.ToString() + "/" + m_ZombieGoal.ToString();
		if (m_TerminalHack && m_Config && m_Config.Main)
		{
			text = text + " chestSpawned=" + BoolText(m_TerminalHack.IsSpawned());
			text = text + " chestUnlocked=" + BoolText(m_TerminalHack.IsActionUnlocked());
			text = text + " chestActive=" + BoolText(m_TerminalHack.IsActive());
			text = text + " chestProgress=" + m_TerminalHack.GetProgressPercent(m_Config.Main).ToString();
			text = text + " chestHacker=" + m_TerminalHack.GetHackerName();
			text = text + " captureUnlocked=" + BoolText(m_State >= DZKOTH_States.WAITING_FOR_PLAYER);
		}

		return text;
	}

	protected string BoolText(bool value)
	{
		if (value)
			return "ja";

		return "nein";
	}
}
