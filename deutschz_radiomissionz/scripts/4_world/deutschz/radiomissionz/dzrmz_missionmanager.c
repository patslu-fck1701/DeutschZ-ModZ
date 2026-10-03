class DZRMZ_MissionManager
{
	protected static ref DZRMZ_MissionManager s_Instance;

	protected bool m_Initialized;
	protected bool m_Running;
	protected ref DZRMZ_Settings m_Settings;
	protected ref DZRMZ_MissionCatalog m_MissionCatalog;
	protected ref DZRMZ_LocationCatalog m_LocationCatalog;
	protected ref DZRMZ_ActiveMissionRuntime m_Runtime;
	protected ref DZRMZ_MissionHistory m_History;
	protected DZRMZ_MissionDefinition m_ActiveDefinition;
	protected DZRMZ_Location m_ObjectiveLocation;
	protected DZRMZ_Location m_DeliveryLocation;
	protected float m_TickSeconds;
	protected float m_RuntimeSaveElapsed;
	protected bool m_PayoutCommitOwnedByProcess;
	protected bool m_RecoveryPayoutWarningLogged;
	protected ref array<Object> m_MissionThreats = new array<Object>;
	protected Object m_ProtectedNpc;
	protected bool m_ProtectionActivated;
	protected ref map<string, bool> m_HelpListeners = new map<string, bool>;

	static DZRMZ_MissionManager GetInstance()
	{
		if (!s_Instance)
			s_Instance = new DZRMZ_MissionManager;
		return s_Instance;
	}

	static void DestroyInstance()
	{
		if (s_Instance)
			s_Instance.Shutdown();
		s_Instance = null;
	}

	void Init()
	{
		if (m_Initialized || !GetGame() || !GetGame().IsServer())
			return;

		DZRMZ_ProfilePaths.Ensure();
		m_Settings = DZRMZ_Storage.LoadSettings();
		m_MissionCatalog = DZRMZ_Storage.LoadMissions();
		m_LocationCatalog = DZRMZ_Storage.LoadLocations();
		m_Runtime = DZRMZ_Storage.LoadRuntime();
		m_History = DZRMZ_Storage.LoadHistory();
		m_Initialized = true;

		if (!m_Settings || !m_MissionCatalog || !m_MissionCatalog.IsValid() || !m_LocationCatalog || !m_LocationCatalog.IsValid() || !m_Runtime || !m_History)
		{
			DZRMZ_Log.Error("Initialisierung abgebrochen: mindestens ein erforderlicher Datenbestand ist nicht verfuegbar oder ungueltig.");
			return;
		}

		if (!m_Settings.Enabled)
		{
			DZRMZ_Log.Info("System ist in RadioMissionConfig.json deaktiviert.");
			return;
		}

		string activeWorld = GetGame().GetWorldName();
		string expectedWorld = m_Settings.WorldName;
		activeWorld.ToLower();
		expectedWorld.ToLower();
		if (m_Settings.DisableOnWorldMismatch && activeWorld != expectedWorld)
		{
			DZRMZ_Log.Error(string.Format("World-Mismatch: aktiv=%1, konfiguriert=%2. System startet aus Sicherheitsgruenden nicht.", activeWorld, expectedWorld));
			return;
		}
		string catalogWorld = m_LocationCatalog.WorldName;
		catalogWorld.ToLower();
		if (catalogWorld != expectedWorld || catalogWorld != activeWorld)
		{
			DZRMZ_Log.Error(string.Format("Locations.json passt nicht zur Welt: aktiv=%1, Settings=%2, Locations=%3. System startet nicht.", activeWorld, expectedWorld, catalogWorld));
			return;
		}
		NormalizeRecentHistory();

		if (!RestoreRuntimeReferences())
		{
			m_Runtime.Reset();
			m_Runtime.SecondsUntilNextMission = m_Settings.FirstMissionDelaySeconds;
			DZRMZ_Storage.SaveRuntime(m_Runtime);
		}
		else if (m_Runtime.State == DZRMZ_EMissionState.DZRMZ_STATE_IDLE && m_Runtime.SecondsUntilNextMission <= 0.0)
		{
			m_Runtime.SecondsUntilNextMission = m_Settings.FirstMissionDelaySeconds;
			DZRMZ_Storage.SaveRuntime(m_Runtime);
		}

		m_TickSeconds = m_Settings.ServerTickMilliseconds / 1000.0;
		m_RuntimeSaveElapsed = 0.0;
		m_PayoutCommitOwnedByProcess = false;
		m_RecoveryPayoutWarningLogged = false;
		m_Running = true;
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Tick, m_Settings.ServerTickMilliseconds, true);
		if (m_Runtime.State == DZRMZ_EMissionState.DZRMZ_STATE_ACTIVE && m_Runtime.OwnerUid != "")
		{
			DZRMZ_MissionMarkerService.Create(m_ActiveDefinition, m_ObjectiveLocation, m_DeliveryLocation);
			if (m_ActiveDefinition.InfectedMaximum > 0) m_Runtime.ThreatSpawned = false;
		}

		DZRMZ_Log.Info(string.Format("Version %1 gestartet: %2 Missionen, %3 Orte, Funk %4 MHz, Zustand %5.", DZRMZ_VERSION, m_MissionCatalog.Missions.Count(), m_LocationCatalog.Locations.Count(), m_Settings.RadioFrequencyMHz, m_Runtime.State));
	}

	void Shutdown()
	{
		if (GetGame())
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(Tick);

		m_Running = false;
		if (m_Runtime)
			DZRMZ_Storage.SaveRuntime(m_Runtime);
		if (m_History)
			DZRMZ_Storage.SaveHistory(m_History);
		DZRMZ_MissionMarkerService.Remove();
		DZRMZ_Storage.ResetRuntimeState();
		CleanupMissionThreats();
		m_ActiveDefinition = null;
		m_ObjectiveLocation = null;
		m_DeliveryLocation = null;
		m_PayoutCommitOwnedByProcess = false;
		m_RecoveryPayoutWarningLogged = false;
		m_Initialized = false;
	}

	protected void Tick()
	{
		if (!m_Running || !GetGame() || !GetGame().IsServer() || !m_Runtime || !m_Settings)
			return;
		DZRMZ_StoryDirector.Tick(m_Settings);

		switch (m_Runtime.State)
		{
			case DZRMZ_EMissionState.DZRMZ_STATE_IDLE:
				TickIdle(m_TickSeconds);
				break;
			case DZRMZ_EMissionState.DZRMZ_STATE_BROADCASTING:
				TickBroadcasting(m_TickSeconds);
				break;
			case DZRMZ_EMissionState.DZRMZ_STATE_ACTIVE:
				TickActive(m_TickSeconds);
				break;
			case DZRMZ_EMissionState.DZRMZ_STATE_COOLDOWN:
				TickCooldown(m_TickSeconds);
				break;
			case DZRMZ_EMissionState.DZRMZ_STATE_COMPLETING:
				TickCompleting(m_TickSeconds);
				break;
			default:
				m_Runtime.Reset();
				m_Runtime.SecondsUntilNextMission = m_Settings.FirstMissionDelaySeconds;
				break;
		}

		m_RuntimeSaveElapsed += m_TickSeconds;
		if (m_RuntimeSaveElapsed >= m_Settings.RuntimeSaveIntervalSeconds)
		{
			m_RuntimeSaveElapsed = 0.0;
			DZRMZ_Storage.SaveRuntime(m_Runtime);
		}
	}

	protected void TickIdle(float deltaSeconds)
	{
		m_Runtime.SecondsUntilNextMission -= deltaSeconds;
		if (m_Runtime.SecondsUntilNextMission <= 0.0)
			StartNextMission();
	}

	protected void TickBroadcasting(float deltaSeconds)
	{
		if (!m_ActiveDefinition || !m_ObjectiveLocation || !m_ActiveDefinition.RadioLines || m_ActiveDefinition.RadioLines.Count() == 0)
		{
			FailMission("Aktive Missionsdaten fehlen");
			return;
		}

		if (m_ActiveDefinition.RequiresListenConfirmation)
		{
			DZRMZ_RadioService.SendHelpAlert(m_Settings, "Eine gestoerte Uebertragung wurde auf 89,5 MHz aufgefangen. Schalte dein Funkgeraet ein und stelle 89,5 MHz ein. Der Funkspruch wird automatisch abgespielt.");
			m_Runtime.State = DZRMZ_EMissionState.DZRMZ_STATE_ACTIVE;
			m_Runtime.MissionSecondsRemaining = m_Settings.MissionTimeoutSeconds;
			m_Runtime.OwnerHintSecondsRemaining = 0.0;
			DZRMZ_Storage.SaveRuntime(m_Runtime);
			TransmitToQualifiedPlayers();
			return;
		}

		m_Runtime.BroadcastSecondsRemaining -= deltaSeconds;
		if (m_Runtime.BroadcastSecondsRemaining > 0.0)
			return;

		if (m_Runtime.BroadcastLineIndex < m_ActiveDefinition.RadioLines.Count())
		{
			string message = ExpandRadioLine(m_ActiveDefinition.RadioLines[m_Runtime.BroadcastLineIndex]);
			DZRMZ_RadioService.SendRadioLine(m_Settings, message);
			m_Runtime.BroadcastLineIndex++;
			m_Runtime.BroadcastSecondsRemaining = m_Settings.BroadcastStepDelaySeconds;
		}

		if (m_Runtime.BroadcastLineIndex >= m_ActiveDefinition.RadioLines.Count())
		{
			m_Runtime.State = DZRMZ_EMissionState.DZRMZ_STATE_ACTIVE;
			m_Runtime.MissionSecondsRemaining = m_Settings.MissionTimeoutSeconds;
			m_Runtime.BroadcastSecondsRemaining = 0.0;
			m_Runtime.OwnerHintSecondsRemaining = m_Settings.OwnerHintIntervalSeconds;
			DZRMZ_RadioService.SendRadioLine(m_Settings, "Auftrag offen: Funkgeraet in die Haende nehmen und 'Mission annehmen' waehlen.");
			DZRMZ_Log.Info(string.Format("Mission %1 ist aktiv bei %2.", m_ActiveDefinition.Id, m_ObjectiveLocation.Name));
			DZRMZ_Storage.SaveRuntime(m_Runtime);
		}
	}

	protected void TickActive(float deltaSeconds)
	{
		if (!m_ActiveDefinition || !m_ObjectiveLocation)
		{
			FailMission("Aktive Missionsdaten fehlen");
			return;
		}

		m_Runtime.MissionSecondsRemaining -= deltaSeconds;
		if (m_Runtime.MissionSecondsRemaining <= 0.0)
		{
			FailMission("Zeitlimit abgelaufen");
			return;
		}

		if (m_Runtime.OwnerUid == "")
		{
			if (m_ActiveDefinition.RequiresListenConfirmation)
			{
				TransmitToQualifiedPlayers();
				return;
			}
			m_Runtime.OwnerHintSecondsRemaining -= deltaSeconds;

			if (m_Runtime.OwnerHintSecondsRemaining <= 0.0)
			{
				DZRMZ_RadioService.SendRadioLine(m_Settings, string.Format("Auftrag weiterhin offen: %1 bei %2. Funkgeraet in die Haende nehmen und Mission annehmen.", m_ActiveDefinition.Title, m_ObjectiveLocation.Name));
				m_Runtime.OwnerHintSecondsRemaining = m_Settings.OwnerHintIntervalSeconds;
			}
			return;
		}

		ProcessOwnershipAndObjective(deltaSeconds);
	}

	protected void TransmitToQualifiedPlayers()
	{
		if (!m_ActiveDefinition || !m_ActiveDefinition.RequiresListenConfirmation || !m_Settings)
			return;
		array<Man> listeners = new array<Man>;
		GetGame().GetPlayers(listeners);
		foreach (Man listener : listeners)
		{
			PlayerBase radioPlayer = PlayerBase.Cast(listener);
			if (radioPlayer && radioPlayer.GetIdentity() && !m_HelpListeners.Contains(radioPlayer.GetIdentity().GetPlainId()) && DZRMZ_RadioService.HasQualifiedRadio(radioPlayer, m_Settings))
				ListenToTransmission(radioPlayer);
		}
	}

	protected void TickCooldown(float deltaSeconds)
	{
		m_Runtime.SecondsUntilNextMission -= deltaSeconds;
		if (m_Runtime.SecondsUntilNextMission <= 0.0)
			StartNextMission();
	}

	protected void TickCompleting(float deltaSeconds)
	{
		if (!m_Runtime || !m_Settings || !m_History || !m_ActiveDefinition || !m_ObjectiveLocation)
		{
			DZRMZ_Log.Error("Abschlusszustand ist unvollstaendig. Auszahlung wird aus Sicherheitsgruenden nicht ausgefuehrt.");
			return;
		}

		if (m_Runtime.CompletionId == "")
			m_Runtime.CompletionId = BuildCompletionId();
		if (m_Runtime.CompletionId == "")
		{
			DZRMZ_Log.Error("Keine stabile CompletionId erzeugbar. Auszahlung bleibt gesperrt.");
			return;
		}

		// Der COMPLETING-Zustand muss auf Platte stehen, bevor Historie oder
		// Auszahlung angefasst werden. Bei einem Schreibfehler bleibt die
		// Mission absichtlich in diesem sicheren Zustand.
		if (!DZRMZ_Storage.SaveRuntime(m_Runtime))
			return;

		DZRMZ_HistoryEntry completionEntry = EnsureCompletionHistoryEntry();
		if (!completionEntry)
		{
			DZRMZ_Log.Error("Completion-Historie konnte nicht im Speicher angelegt werden. Auszahlung bleibt gesperrt.");
			return;
		}
		if (!DZRMZ_Storage.SaveHistory(m_History))
			return;
		m_Runtime.CompletionRecorded = true;
		if (!DZRMZ_Storage.SaveRuntime(m_Runtime))
			return;
		if (!ReconcilePayoutMarkers(completionEntry))
			return;

		PlayerBase owner = FindOnlinePlayer(m_Runtime.OwnerUid);
		bool rewardsRequested = m_Settings.HardlineRewardsEnabled || m_ActiveDefinition.RewardCurrencyAmount > 0 || (m_Settings.SuccessRewardsEnabled && m_Settings.SuccessRewards && m_Settings.SuccessRewards.Count() > 0);
		bool payoutOmittedBecauseOwnerMissing = false;

		if (!m_Runtime.RewardPayoutCommitted)
		{
			if (rewardsRequested && (!owner || !owner.GetIdentity()))
			{
				m_Runtime.OwnerMissingSeconds += deltaSeconds;
				if (m_Runtime.OwnerMissingSeconds < m_Settings.OwnerReleaseAfterDisconnectSeconds)
					return;
				payoutOmittedBecauseOwnerMissing = true;
			}
			else
			{
				m_Runtime.OwnerMissingSeconds = 0.0;
			}

			// At-most-once-Commit: Erst wenn dieser Marker erfolgreich persistent
			// ist, darf GrantSuccessRewards im aktuellen Prozess laufen.
			m_Runtime.RewardPayoutCommitted = true;
			if (!DZRMZ_Storage.SaveRuntime(m_Runtime))
			{
				m_Runtime.RewardPayoutCommitted = false;
				return;
			}
			m_PayoutCommitOwnedByProcess = true;
			completionEntry.RewardPayoutCommitted = true;
			if (!DZRMZ_Storage.SaveHistory(m_History))
				return;
		}

		if (!m_Runtime.RewardPayoutFinished)
		{
			if (!m_PayoutCommitOwnedByProcess)
			{
				// Ein persistierter Commit ohne Finished-Marker bedeutet: Der alte
				// Prozess kann bereits Items erzeugt haben. Eine Wiederholung waere
				// ein Dupe. Deshalb wird im Zweifel nicht erneut ausgezahlt.
				m_Runtime.RewardItemsGranted = -1;
				m_Runtime.RewardPayoutFinished = true;
			}
			else if (payoutOmittedBecauseOwnerMissing)
			{
				m_Runtime.RewardItemsGranted = 0;
				m_Runtime.RewardPayoutFinished = true;
			}
			else
			{
				m_Runtime.RewardItemsGranted = DZRMZ_RewardService.GrantMissionReward(owner, m_ActiveDefinition, m_Settings);
				m_Runtime.RewardReputationGranted = GrantHardlineReputation(owner);
				m_Runtime.RewardPayoutFinished = true;
			}
		}

		completionEntry.RewardPayoutCommitted = m_Runtime.RewardPayoutCommitted;
		completionEntry.RewardPayoutFinished = m_Runtime.RewardPayoutFinished;
		completionEntry.RewardItemsGranted = m_Runtime.RewardItemsGranted;
		completionEntry.RewardReputationGranted = m_Runtime.RewardReputationGranted;
		if (!DZRMZ_Storage.SaveRuntime(m_Runtime))
			return;
		if (!DZRMZ_Storage.SaveHistory(m_History))
			return;
		if (m_Runtime.RewardItemsGranted < 0 && !m_RecoveryPayoutWarningLogged)
		{
			DZRMZ_Log.Error("Recovery erkannte einen begonnenen, aber nicht bestaetigten Reward-Payout. Keine Wiederholung; Admin-Pruefung erforderlich. CompletionId=" + m_Runtime.CompletionId);
			m_RecoveryPayoutWarningLogged = true;
		}
		if (payoutOmittedBecauseOwnerMissing)
			DZRMZ_Log.Warn("Reward-Payout nach Ablauf der Wiederverbindungsfrist ohne Empfaenger abgeschlossen. CompletionId=" + m_Runtime.CompletionId);
		if (m_ActiveDefinition.Id == "LIVE-OPERATION-02" && owner)
		{
			bool operationCompleted = false;
			int operationCall = g_Game.GameScript.CallFunctionParams(null, "DZODZ_CompleteOperation", operationCompleted, new Param1<PlayerBase>(owner));
			if (operationCall == 0 || !operationCompleted)
			{
				DZRMZ_Log.Error("Operation-DeutschZ-Finale konnte nicht bestaetigt werden; Completion bleibt zur Admin-Pruefung protokolliert.");
				return;
			}
		}
		AnnounceCompletedMission(owner, payoutOmittedBecauseOwnerMissing);
		BeginCooldown();
	}

	protected void StartNextMission()
	{
		DZRMZ_MissionDefinition definition = SelectMissionDefinition();
		StartMissionDefinition(definition);
	}

	protected void StartMissionDefinition(DZRMZ_MissionDefinition definition)
	{
		if (!definition)
		{
			DZRMZ_Log.Error("Keine gueltige Missionsdefinition verfuegbar. Neuer Versuch in 300 Sekunden.");
			m_Runtime.State = DZRMZ_EMissionState.DZRMZ_STATE_IDLE;
			m_Runtime.SecondsUntilNextMission = 300.0;
			return;
		}

		DZRMZ_Location objective;
		if (definition.FixedLocationId != "")
			objective = FindLocation(definition.FixedLocationId);
		else
			objective = SelectLocation(definition.Category, "");
		if (!objective)
		{
			DZRMZ_Log.Error("Keine passende Zielposition fuer Kategorie " + definition.Category + ".");
			m_Runtime.State = DZRMZ_EMissionState.DZRMZ_STATE_IDLE;
			m_Runtime.SecondsUntilNextMission = 300.0;
			return;
		}

		DZRMZ_Location delivery;
		if (definition.ObjectiveType == DZRMZ_OBJECTIVE_DELIVERY)
		{
			if (definition.FixedDeliveryLocationId != "")
				delivery = FindLocation(definition.FixedDeliveryLocationId);
			else
				delivery = SelectLocation(DZRMZ_TAG_DELIVERY, objective.Id);
			if (!delivery)
			{
				DZRMZ_Log.Error("Keine vom Ziel getrennte Abgabeposition verfuegbar.");
				m_Runtime.State = DZRMZ_EMissionState.DZRMZ_STATE_IDLE;
				m_Runtime.SecondsUntilNextMission = 300.0;
				return;
			}
		}

		m_Runtime.Reset();
		m_HelpListeners.Clear();
		m_Runtime.State = DZRMZ_EMissionState.DZRMZ_STATE_BROADCASTING;
		m_Runtime.DefinitionId = definition.Id;
		m_Runtime.ObjectiveLocationId = objective.Id;
		m_Runtime.MissionInstanceId = string.Format("%1-%2-%3-%4-%5", m_History.TotalStarted + 1, Math.RandomInt(100000, 1000000), Math.RandomInt(100000, 1000000), definition.Id, objective.Id);
		if (delivery)
			m_Runtime.DeliveryLocationId = delivery.Id;
		m_Runtime.BroadcastLineIndex = 0;
		m_Runtime.BroadcastSecondsRemaining = 0.0;
		m_Runtime.MissionSecondsRemaining = m_Settings.MissionTimeoutSeconds;

		m_ActiveDefinition = definition;
		m_ObjectiveLocation = objective;
		m_DeliveryLocation = delivery;
		m_History.TotalStarted++;
		RememberRecent(m_History.RecentMissionIds, definition.Id, m_Settings.RecentMissionMemory);
		RememberRecent(m_History.RecentLocationIds, objective.Id, m_Settings.RecentLocationMemory);
		if (delivery)
			RememberRecent(m_History.RecentLocationIds, delivery.Id, m_Settings.RecentLocationMemory);

		DZRMZ_Storage.SaveHistory(m_History);
		DZRMZ_Storage.SaveRuntime(m_Runtime);
		DZRMZ_Log.Info(string.Format("Mission %1 (%2) ausgewaehlt: Ziel=%3, Abgabe=%4.", definition.Id, definition.Category, objective.Name, GetDeliveryName()));
	}

	protected void ProcessOwnershipAndObjective(float deltaSeconds)
	{
		if (m_Runtime.OwnerUid == "")
			return;

		PlayerBase owner = FindOnlinePlayer(m_Runtime.OwnerUid);
		if (!owner || !owner.GetIdentity())
		{
			m_Runtime.OwnerMissingSeconds += deltaSeconds;
			if (m_Runtime.OwnerMissingSeconds >= m_Settings.OwnerReleaseAfterDisconnectSeconds)
				ReleaseOwnership("Auftragseigentuemer nicht mehr erreichbar");
			return;
		}

		if (!owner.IsAlive())
		{
			ReleaseOwnership("Auftragseigentuemer ist gefallen");
			return;
		}
		if (owner.IsUnconscious())
		{
			if (m_ActiveDefinition.ObjectiveType == DZRMZ_OBJECTIVE_HOLD)
				m_Runtime.HoldProgressSeconds = 0.0;
			return;
		}

		m_Runtime.OwnerMissingSeconds = 0.0;
		m_Runtime.OwnerHintSecondsRemaining -= deltaSeconds;

		float objectiveDistance = vector.Distance(owner.GetPosition(), m_ObjectiveLocation.GetGroundedPosition());
		if (!m_Runtime.ThreatSpawned && objectiveDistance <= 300.0)
			SpawnMissionThreats();
		if (m_ActiveDefinition.ScenarioType == DZRMZ_SCENARIO_MEDICAL_AID)
		{
			EntityAI patient = EntityAI.Cast(m_ProtectedNpc);
			if (m_Runtime.ThreatSpawned && (!patient || !patient.IsAlive())) { FailMission(m_ActiveDefinition.FailureNpcDeadText); return; }
			if (patient && objectiveDistance <= m_Settings.ObjectiveRadiusMeters && !m_ProtectionActivated)
			{
				patient.SetAllowDamage(true);
				m_ProtectionActivated = true;
				DZRMZ_RadioService.SendToPlayer(m_Settings, owner, m_ActiveDefinition.Title, GetHelpArrivalText());
			}
			return;
		}

		if (m_ActiveDefinition.ObjectiveType == DZRMZ_OBJECTIVE_VISIT)
		{
			if (vector.Distance(owner.GetPosition(), m_ObjectiveLocation.GetGroundedPosition()) <= m_Settings.ObjectiveRadiusMeters)
				CompleteMission();
		}
		else if (m_ActiveDefinition.ObjectiveType == DZRMZ_OBJECTIVE_HOLD)
		{
			ProcessHoldObjective(owner, deltaSeconds);
		}
		else if (m_ActiveDefinition.ObjectiveType == DZRMZ_OBJECTIVE_DELIVERY)
		{
			ProcessDeliveryObjective(owner);
		}
	}

	void TryAcceptMission(PlayerBase player)
	{
		if (!player || !player.GetIdentity() || !player.IsAlive() || player.IsUnconscious())
			return;
		if (!m_Running || !m_Runtime || !m_Settings || !m_ActiveDefinition || !m_ObjectiveLocation)
		{
			DZRMZ_RadioService.SendToPlayer(m_Settings, player, "DeutschZ RadioMissionZ", "Aktuell ist keine Mission zur Annahme bereit.");
			return;
		}
		if (m_Runtime.State != DZRMZ_EMissionState.DZRMZ_STATE_ACTIVE)
		{
			DZRMZ_RadioService.SendToPlayer(m_Settings, player, "DeutschZ RadioMissionZ", "Die Funkmeldung laeuft noch oder die Mission ist bereits beendet.");
			return;
		}
		if (m_Runtime.OwnerUid != "")
		{
			DZRMZ_RadioService.SendToPlayer(m_Settings, player, "DeutschZ RadioMissionZ", "Diese Mission wurde bereits von einem anderen Ueberlebenden angenommen.");
			return;
		}
		if (!DZRMZ_RadioService.HasQualifiedRadio(player, m_Settings))
		{
			DZRMZ_RadioService.SendToPlayer(m_Settings, player, "DeutschZ RadioMissionZ", string.Format("Funkgeraet einschalten und auf %1 MHz stellen.", m_Settings.RadioFrequencyMHz));
			return;
		}
		if (m_ActiveDefinition.RequiresListenConfirmation && !m_HelpListeners.Contains(player.GetIdentity().GetPlainId()))
			ListenToTransmission(player);

		if (!PlayerHasStartRequirement(player, m_ActiveDefinition))
		{
			DZRMZ_RadioService.SendToPlayer(m_Settings, player, "Funknachweis fehlt", "Du benoetigst den unbeschaedigten Einsatznachweis: " + m_ActiveDefinition.StartRequiredItemClass);
			return;
		}
		ClaimMission(player);
	}

	void ListenToTransmission(PlayerBase player)
	{
		if (!player || !player.GetIdentity() || !m_Settings) return;
		if (!m_Runtime || !m_ActiveDefinition || m_Runtime.State != DZRMZ_EMissionState.DZRMZ_STATE_ACTIVE)
		{
			DZRMZ_RadioService.SendToPlayer(m_Settings, player, "89,5 MHz", "Derzeit ist kein Hilferuf zum Abhoeren aktiv.");
			return;
		}
		if (!DZRMZ_RadioService.HasQualifiedRadio(player, m_Settings))
		{
			DZRMZ_RadioService.SendToPlayer(m_Settings, player, "89,5 MHz", "Funkgeraet einschalten und auf 89,5 MHz stellen.");
			return;
		}
		string dialogue = "";
		foreach (string line : m_ActiveDefinition.RadioLines) { if (dialogue != "") dialogue += "\n"; dialogue += line; }
		string context = GetHelpSituationText();
		if (context != "") dialogue += "\n\nEINSATZINFO (TEXT): " + context;
		string audioId = DZRMZ_AudioService.HelpId(m_ActiveDefinition.Id);
		if (audioId != "") DZRMZ_AudioService.Play(player, m_Settings, audioId);
		DZRMZ_RadioService.SendToPlayer(m_Settings, player, "89,5 MHz - " + m_ActiveDefinition.Title, dialogue);
		m_Runtime.TransmissionHeard = true;
		m_Runtime.TransmissionListenerUid = player.GetIdentity().GetPlainId();
		m_HelpListeners.Set(m_Runtime.TransmissionListenerUid, true);
		DZRMZ_Storage.SaveRuntime(m_Runtime);
	}

	protected string GetHelpSituationText()
	{
		if (!m_ActiveDefinition || !m_ObjectiveLocation) return "";
		string place = m_ObjectiveLocation.Name;
		if (m_ActiveDefinition.Id == "SIDE-EINGESCHLOSSEN-01") return string.Format("Bei %1 meldet sich jemand aus der Naehe eines Hauses. Hinter seiner Stimme hoerst du Angst: Draussen sammeln sich Infizierte, und niemand antwortet ihm. Er verspricht %2 Euro, wenn er lebend herauskommt. Wer hat ihn dort zurueckgelassen?", place, m_ActiveDefinition.RewardCurrencyAmount);
		if (m_ActiveDefinition.Id == "SIDE-VERLETZT-01") return string.Format("Ein angeschossener Mann bei %1 haelt die Blutung kaum noch auf. Er spricht von einem Schuss, nicht von einem Biss. Der Schuetze ist verschwunden; Infizierte sind noch dort. Fuer seine Rettung stehen %2 Euro bereit.", place, m_ActiveDefinition.RewardCurrencyAmount);
		if (m_ActiveDefinition.Id == "SIDE-BELAGERT-01") return string.Format("Bei %1 wird eine Person von Infizierten eingekreist. Ihr Funkruf bricht immer wieder ab. Es wirkt nicht wie ein einzelner Angriff: Weitere Gruppen bewegen sich auf dieselbe Stelle zu. Fuer die Sicherung sind %2 Euro zugesagt.", place, m_ActiveDefinition.RewardCurrencyAmount);
		if (m_ActiveDefinition.Id == "SIDE-ALLEIN-01") return string.Format("Vier Menschen waren bei %1 unterwegs. Nach einem Angriff blieb nur eine Stimme auf 89,5 MHz uebrig; die anderen sind tot oder verschwunden. Die Ueberlebende bittet um Hilfe, bevor auch ihr Signal verstummt. Belohnung: %2 Euro.", place, m_ActiveDefinition.RewardCurrencyAmount);
		return "";
	}

	protected string GetHelpObjectiveText()
	{
		if (!m_ActiveDefinition) return "";
		if (m_ActiveDefinition.Id == "SIDE-EINGESCHLOSSEN-01") return string.Format("Ziel: Beseitige die Missions-Infizierten, halte die Schutzperson am Leben und sichere den Bereich %1 Sekunden lang.", m_ActiveDefinition.HoldSeconds);
		if (m_ActiveDefinition.Id == "SIDE-VERLETZT-01") return "Ziel: Bringe ein verwendbares Verbandpaeckchen mit. Erreiche den Verletzten und waehle in seiner Naehe 'Verletzten versorgen'.";
		if (m_ActiveDefinition.Id == "SIDE-BELAGERT-01") return string.Format("Ziel: Wehre %1 Infiziertenwellen ab, halte die Schutzperson am Leben und sichere den Bereich danach %2 Sekunden lang.", m_ActiveDefinition.ThreatWaveCount, m_ActiveDefinition.HoldSeconds);
		if (m_ActiveDefinition.Id == "SIDE-ALLEIN-01") return string.Format("Ziel: Beseitige die Missions-Infizierten, halte die Ueberlebende am Leben und sichere den Bereich %1 Sekunden lang.", m_ActiveDefinition.HoldSeconds);
		return "";
	}

	protected string GetHelpArrivalText()
	{
		if (!m_ActiveDefinition) return "Die Schutzperson ist jetzt verwundbar. Beseitige alle Missions-Infizierten; erst danach zaehlt die Sicherungszeit.";
		if (m_ActiveDefinition.Id == "SIDE-EINGESCHLOSSEN-01") return "Du bist am Haus. Die Infizierten stehen zwischen dir und der eingeschlossenen Person. Sie ist jetzt verwundbar: Raeume den Bereich und halte die Stellung, bis sie sicher fortkann.";
		if (m_ActiveDefinition.Id == "SIDE-VERLETZT-01") return "Du hast den Verletzten erreicht. Er lebt, aber die Blutung laesst ihm keine Zeit. Sichere den Zugang und versorge ihn mit einem Verbandpaeckchen aus deinem Inventar.";
		if (m_ActiveDefinition.Id == "SIDE-BELAGERT-01") return "Die erste Gruppe ist da. Hinter ihr bewegen sich weitere Infizierte auf die Schutzperson zu. Lass sie nicht sterben; nach der letzten Welle muss der Bereich noch gesichert werden.";
		if (m_ActiveDefinition.Id == "SIDE-ALLEIN-01") return "Du findest die letzte Ueberlebende der Gruppe. Die Infizierten sind noch in ihrer Naehe. Sie ist jetzt verwundbar: Befreie sie und halte den Bereich, bis der Weg frei ist.";
		return "Die Schutzperson ist jetzt verwundbar. Beseitige alle Missions-Infizierten; erst danach zaehlt die Sicherungszeit.";
	}

	protected string GetHelpAftermathText()
	{
		if (!m_ActiveDefinition) return "";
		if (m_ActiveDefinition.Id == "SIDE-EINGESCHLOSSEN-01") return "NACHTRAG: Die gerettete Person wusste nicht, warum sich so viele Infizierte ausgerechnet an diesem Haus sammelten. Auf 89,5 MHz bleiben weitere Notrufe offen.";
		if (m_ActiveDefinition.Id == "SIDE-VERLETZT-01") return "NACHTRAG: Der Verletzte wurde nicht von einem Infizierten getroffen. Wer auf ihn schoss, war bei deiner Ankunft fort. Sein Fall bleibt in der Einsatzakte offen.";
		if (m_ActiveDefinition.Id == "SIDE-BELAGERT-01") return "NACHTRAG: Die Angriffe kamen in mehreren Wellen. Ob die Infizierten zufaellig dort auftauchten oder von etwas angelockt wurden, bleibt ungeklaert.";
		if (m_ActiveDefinition.Id == "SIDE-ALLEIN-01") return "NACHTRAG: Von den drei Vermissten fehlt weiter jede Spur. Die Gerettete erinnert sich nur an einen Namen, der vor dem Angriff fiel: Transport Sieben.";
		return "";
	}

	bool TryProvideMedicalAid(PlayerBase player, Object target)
	{
		if (!player || !player.GetIdentity() || !target || target != m_ProtectedNpc || !m_Runtime || !m_ActiveDefinition) return false;
		if (m_Runtime.State != DZRMZ_EMissionState.DZRMZ_STATE_ACTIVE || m_Runtime.OwnerUid != player.GetIdentity().GetPlainId()) return false;
		if (m_ActiveDefinition.ScenarioType != DZRMZ_SCENARIO_MEDICAL_AID || m_Runtime.ObjectiveInteractionDone) return false;
		if (vector.Distance(player.GetPosition(), target.GetPosition()) > 3.0) return false;
		EntityAI patient = EntityAI.Cast(target); if (!patient || !patient.IsAlive()) return false;
		if (DZRMZ_InventoryService.CountValidDeliveryItems(player, m_ActiveDefinition) < m_ActiveDefinition.RequiredQuantity)
		{
			DZRMZ_RadioService.SendToPlayer(m_Settings, player, "Medizin fehlt", "Du brauchst ein verwendbares Verbandpaeckchen im Inventar."); return false;
		}
		if (!DZRMZ_InventoryService.ConsumeValidDeliveryItems(player, m_ActiveDefinition)) return false;
		m_Runtime.ObjectiveInteractionDone = true; DZRMZ_Storage.SaveRuntime(m_Runtime);
		CompleteMission(); return true;
	}
	protected void ClaimMission(PlayerBase player)
	{
		m_Runtime.OwnerUid = player.GetIdentity().GetPlainId();
		m_Runtime.OwnerName = player.GetIdentity().GetName();
		m_Runtime.OwnerMissingSeconds = 0.0;
		m_Runtime.OwnerHintSecondsRemaining = 0.0;
		m_Runtime.HoldProgressSeconds = 0.0;

		string instructions = m_ActiveDefinition.Description;
		if (m_ActiveDefinition.ObjectiveType == DZRMZ_OBJECTIVE_HOLD)
			instructions = string.Format("Halte den Bereich %1 Sekunden lang. Verlaesst du die Zone, beginnt die Sicherung erneut.", m_ActiveDefinition.HoldSeconds);
		else if (m_ActiveDefinition.ObjectiveType == DZRMZ_OBJECTIVE_DELIVERY)
			instructions = string.Format("Bringe %1 x %2 nach %3. Die Gegenstaende muessen in deinem Inventar sein.", m_ActiveDefinition.RequiredQuantity, m_ActiveDefinition.RequiredItemLabel, GetDeliveryName());

		string acceptance = m_ActiveDefinition.AcceptText;
		string objective = GetHelpObjectiveText();
		if (objective != "") acceptance += "\n\nEINSATZINFO (TEXT): " + objective;
		DZRMZ_RadioService.SendToPlayer(m_Settings, player, "89,5 MHz", acceptance);
		if (DZRMZ_MissionMarkerService.Create(m_ActiveDefinition, m_ObjectiveLocation, m_DeliveryLocation))
			DZRMZ_RadioService.SendToPlayer(m_Settings, player, "Missionsmarker gesetzt", "Das Missionsziel wurde auf Karte und HUD markiert.");
		else
			DZRMZ_RadioService.SendToPlayer(m_Settings, player, "Missionsmarker nicht verfuegbar", "Die Mission ist aktiv; nutze die Ortsangabe aus dem Funkspruch.");
		DZRMZ_Log.Info(string.Format("Mission %1 angenommen durch %2 (%3).", m_ActiveDefinition.Id, m_Runtime.OwnerName, m_Runtime.OwnerUid));
		// Szenarioobjekte werden bei Annaeherung erzeugt, damit sie bei langer Anfahrt nicht verschwinden.
		DZRMZ_Storage.SaveRuntime(m_Runtime);
	}

	protected void ProcessHoldObjective(PlayerBase owner, float deltaSeconds)
	{
		float distance = vector.Distance(owner.GetPosition(), m_ObjectiveLocation.GetGroundedPosition());
		if (distance > m_Settings.ObjectiveRadiusMeters)
		{
			if (m_Runtime.HoldProgressSeconds > 0.0) { m_Runtime.HoldProgressSeconds = 0.0; DZRMZ_RadioService.SendToPlayer(m_Settings, owner, "Sicherung abgebrochen", "Du hast den Missionsbereich verlassen. Die Haltezeit wurde zurueckgesetzt."); }
			return;
		}
		if (!m_Runtime.ThreatSpawned) { SpawnMissionThreats(); return; }
		if (!m_ProtectionActivated)
		{
			EntityAI protectedEntity = EntityAI.Cast(m_ProtectedNpc); if (protectedEntity) protectedEntity.SetAllowDamage(true);
			m_ProtectionActivated = true;
			DZRMZ_RadioService.SendToPlayer(m_Settings, owner, m_ActiveDefinition.Title, GetHelpArrivalText());
		}
		EntityAI protectedPerson = EntityAI.Cast(m_ProtectedNpc);
		if (m_ActiveDefinition.SpawnProtectedNpc && (!protectedPerson || !protectedPerson.IsAlive())) { FailMission(m_ActiveDefinition.FailureNpcDeadText); return; }
		if (CountLivingMissionThreats() > 0) { m_Runtime.HoldProgressSeconds = 0.0; return; }
		if (m_ActiveDefinition.ScenarioType == DZRMZ_SCENARIO_BESIEGED && m_Runtime.ThreatWaveIndex < m_ActiveDefinition.ThreatWaveCount)
		{
			if (m_Runtime.ThreatWavePauseSeconds <= 0.0) { m_Runtime.ThreatWavePauseSeconds = 8.0; DZRMZ_RadioService.SendToPlayer(m_Settings, owner, "Kurze Ruhe", "Die naechste Welle kommt. Bleib bei der Schutzperson."); }
			m_Runtime.ThreatWavePauseSeconds -= deltaSeconds;
			if (m_Runtime.ThreatWavePauseSeconds <= 0.0) SpawnThreatWave();
			return;
		}
		m_Runtime.ThreatCleared = true;
		m_Runtime.HoldProgressSeconds += deltaSeconds;
		if (m_Runtime.HoldProgressSeconds >= m_ActiveDefinition.HoldSeconds) { CompleteMission(); return; }
		if (m_Runtime.OwnerHintSecondsRemaining <= 0.0)
		{
			int remaining = m_ActiveDefinition.HoldSeconds - m_Runtime.HoldProgressSeconds; if (remaining < 0) remaining = 0;
			DZRMZ_RadioService.SendToPlayer(m_Settings, owner, "Stellung halten", string.Format("Noch %1 Sekunden bis zur Sicherung.", remaining));
			m_Runtime.OwnerHintSecondsRemaining = m_Settings.OwnerHintIntervalSeconds;
		}
	}

	protected void SpawnMissionThreats()
	{
		CleanupMissionThreats();
		m_Runtime.ThreatSpawned = true; m_Runtime.ThreatWaveIndex = 0; m_Runtime.ThreatWavePauseSeconds = 0.0;
		vector center = m_ObjectiveLocation.GetGroundedPosition();
		vector npcPosition = FindScenarioNpcPosition(center);
		if (m_ActiveDefinition.SpawnProtectedNpc && npcPosition != vector.Zero)
		{
#ifdef EXPANSIONMODCORE
			if (m_ActiveDefinition.NpcClass.IndexOf("ExpansionNPC") == 0) m_ProtectedNpc = ExpansionGame.CreateObjectExSafe(m_ActiveDefinition.NpcClass, npcPosition, ECE_CREATEPHYSICS | ECE_NOLIFETIME | ECE_NOPERSISTENCY_WORLD | ECE_PLACE_ON_SURFACE);
			else
#endif
			m_ProtectedNpc = GetGame().CreateObjectEx(m_ActiveDefinition.NpcClass, npcPosition, ECE_CREATEPHYSICS | ECE_NOLIFETIME | ECE_NOPERSISTENCY_WORLD | ECE_PLACE_ON_SURFACE);
			EntityAI protectedEntity = EntityAI.Cast(m_ProtectedNpc); if (protectedEntity) protectedEntity.SetAllowDamage(false);
			if (m_ActiveDefinition.ScenarioType == DZRMZ_SCENARIO_MEDICAL_AID)
			{
				PlayerBase patient = PlayerBase.Cast(m_ProtectedNpc);
				if (patient) patient.DZRMZ_SetMedicalAidTarget(true);
			}
		}
		m_ProtectionActivated = false;
		SpawnThreatWave();
		DZRMZ_Log.Info(string.Format("Szenario %1 vorbereitet: Welle %2/%3, %4 Infizierte, Schutzperson=%5.", m_ActiveDefinition.ScenarioType, m_Runtime.ThreatWaveIndex, m_ActiveDefinition.ThreatWaveCount, m_MissionThreats.Count(), m_ProtectedNpc != null));
		DZRMZ_Storage.SaveRuntime(m_Runtime);
	}

	protected void SpawnThreatWave()
	{
		for (int n = m_MissionThreats.Count() - 1; n >= 0; n--)
		{
			EntityAI oldThreat = EntityAI.Cast(m_MissionThreats[n]);
			if (!oldThreat || !oldThreat.IsAlive()) m_MissionThreats.RemoveOrdered(n);
		}
		int count = Math.RandomIntInclusive(m_ActiveDefinition.InfectedMinimum, m_ActiveDefinition.InfectedMaximum);
		vector center = m_ObjectiveLocation.GetGroundedPosition();
		for (int i = 0; i < count; i++)
		{
			vector pos = FindSafeMissionPosition(center, m_ActiveDefinition.ThreatSpawnMinimumRadius, m_ActiveDefinition.ThreatSpawnMaximumRadius);
			if (pos == vector.Zero) continue;
			Object infected = GetGame().CreateObjectEx("ZmbM_CitizenASkinny_Base", pos, ECE_PLACE_ON_SURFACE | ECE_NOPERSISTENCY_WORLD | ECE_CREATEPHYSICS);
			if (infected) m_MissionThreats.Insert(infected);
		}
		m_Runtime.ThreatWaveIndex++; m_Runtime.ThreatWavePauseSeconds = 0.0;
		DZRMZ_Log.Info(string.Format("Missionswelle %1/%2 erzeugt: %3 Infizierte.", m_Runtime.ThreatWaveIndex, m_ActiveDefinition.ThreatWaveCount, count));
	}

	protected vector FindScenarioNpcPosition(vector center)
	{
		if (m_ActiveDefinition.ScenarioType == DZRMZ_SCENARIO_TRAPPED || m_ActiveDefinition.ScenarioType == DZRMZ_SCENARIO_BESIEGED)
		{
			array<Object> objects = new array<Object>; array<CargoBase> proxy = new array<CargoBase>; GetGame().GetObjectsAtPosition3D(center, 80.0, objects, proxy);
			foreach (Object obj : objects) if (obj && obj.IsInherited(Building)) { vector nearBuilding = FindSafeMissionPosition(obj.GetPosition(), 3.0, 10.0); if (nearBuilding != vector.Zero) return nearBuilding; }
		}
		return FindSafeMissionPosition(center, 3.0, 10.0);
	}
	protected vector FindSafeMissionPosition(vector center, float minimumDistance, float maximumDistance)
	{
		for (int attempt = 0; attempt < 40; attempt++)
		{
			vector pos = center + Vector(Math.RandomFloatInclusive(-maximumDistance, maximumDistance), 0, Math.RandomFloatInclusive(-maximumDistance, maximumDistance));
			float distance = vector.Distance(pos, center);
			if (distance < minimumDistance || distance > maximumDistance) continue;
			if (GetGame().SurfaceIsSea(pos[0], pos[2]) || GetGame().SurfaceIsPond(pos[0], pos[2])) continue;
			pos[1] = GetGame().SurfaceY(pos[0], pos[2]);
			array<vector> samples = new array<vector>;
			samples.Insert(pos + "0.75 0 0.75"); samples.Insert(pos + "-0.75 0 0.75"); samples.Insert(pos + "0.75 0 -0.75"); samples.Insert(pos + "-0.75 0 -0.75");
			if (GetGame().GetHighestSurfaceYDifference(samples) > 1.25) continue;
			array<Object> excluded = new array<Object>; array<Object> collided = new array<Object>;
			if (GetGame().IsBoxColliding(pos + "0 0.9 0", vector.Zero, "0.9 1.8 0.9", excluded, collided)) continue;
			return pos;
		}
		return vector.Zero;
	}

	protected int CountLivingMissionThreats()
	{
		int alive = 0;
		foreach (Object object : m_MissionThreats) { ZombieBase zombie = ZombieBase.Cast(object); if (zombie && zombie.IsAlive()) alive++; }
		return alive;
	}

	protected void CleanupMissionThreats()
	{
		foreach (Object object : m_MissionThreats) if (object) GetGame().ObjectDelete(object);
		m_MissionThreats.Clear();
		if (m_ProtectedNpc) GetGame().ObjectDelete(m_ProtectedNpc);
		m_ProtectedNpc = null;
		m_ProtectionActivated = false;
	}

	protected void ProcessDeliveryObjective(PlayerBase owner)
	{
		if (!m_DeliveryLocation)
		{
			FailMission("Abgabeposition fehlt");
			return;
		}

		if (vector.Distance(owner.GetPosition(), m_DeliveryLocation.GetGroundedPosition()) > m_Settings.DeliveryRadiusMeters)
			return;

		int available = DZRMZ_InventoryService.CountValidDeliveryItems(owner, m_ActiveDefinition);
		if (available >= m_ActiveDefinition.RequiredQuantity)
		{
			if (m_Settings.ConsumeDeliveryItems && !DZRMZ_InventoryService.ConsumeValidDeliveryItems(owner, m_ActiveDefinition))
			{
				DZRMZ_Log.Warn("Liefergegenstaende konnten trotz vorheriger Zaehlung nicht verbraucht werden.");
				return;
			}
			CompleteMission();
			return;
		}

		if (m_Runtime.OwnerHintSecondsRemaining <= 0.0)
		{
			DZRMZ_RadioService.SendToPlayer(m_Settings, owner, "Lieferung unvollstaendig", string.Format("Vorhanden: %1 von %2 x %3.", available, m_ActiveDefinition.RequiredQuantity, m_ActiveDefinition.RequiredItemLabel));
			m_Runtime.OwnerHintSecondsRemaining = m_Settings.OwnerHintIntervalSeconds;
		}
	}

	protected void ReleaseOwnership(string reason)
	{
		CleanupMissionThreats();
		string previousOwner = m_Runtime.OwnerName;
		DZRMZ_MissionMarkerService.Remove();
		m_Runtime.OwnerUid = "";
		m_Runtime.OwnerName = "";
		m_Runtime.OwnerMissingSeconds = 0.0;
		m_Runtime.OwnerHintSecondsRemaining = 0.0;
		m_Runtime.HoldProgressSeconds = 0.0;
		DZRMZ_RadioService.SendRadioLine(m_Settings, string.Format("Auftrag %1 ist wieder offen. Grund: %2.", m_ActiveDefinition.Title, reason));
		DZRMZ_Log.Warn(string.Format("Eigentuemer %1 freigegeben: %2.", previousOwner, reason));
		DZRMZ_Storage.SaveRuntime(m_Runtime);
	}

	protected void CompleteMission()
	{
		if (!m_ActiveDefinition || !m_ObjectiveLocation || !m_Runtime || m_Runtime.State == DZRMZ_EMissionState.DZRMZ_STATE_COMPLETING)
			return;

		DZRMZ_MissionMarkerService.Remove();
		CleanupMissionThreats();
		m_Runtime.State = DZRMZ_EMissionState.DZRMZ_STATE_COMPLETING;
		m_Runtime.CompletionId = BuildCompletionId();
		m_Runtime.CompletionRecorded = false;
		m_Runtime.RewardPayoutCommitted = false;
		m_Runtime.RewardPayoutFinished = false;
		m_Runtime.RewardItemsGranted = 0;
		m_Runtime.RewardReputationGranted = 0;
		m_Runtime.OwnerMissingSeconds = 0.0;
		m_PayoutCommitOwnedByProcess = false;
		m_RecoveryPayoutWarningLogged = false;

		if (!DZRMZ_Storage.SaveRuntime(m_Runtime))
		{
			DZRMZ_Log.Error("Abschluss konnte nicht persistent vorgemerkt werden. Reward-Payout bleibt gesperrt.");
			return;
		}
		TickCompleting(0.0);
	}

	protected void FailMission(string reason)
	{
		CleanupMissionThreats();
		DZRMZ_MissionMarkerService.Remove();
		string title = "Unbekannte Mission";
		if (m_ActiveDefinition)
			title = m_ActiveDefinition.Title;
		m_History.TotalFailed++;
		AddHistoryEntry("FAILED: " + reason);
		DZRMZ_RadioService.SendRadioLine(m_Settings, string.Format("Mission fehlgeschlagen: '%1' wurde beendet. Grund: %2.", title, reason));
		DZRMZ_Log.Warn(string.Format("Mission fehlgeschlagen: %1 (%2).", title, reason));
		BeginCooldown(false);
	}

	protected void BeginCooldown(bool continueChain = true)
	{
		DZRMZ_MissionMarkerService.Remove();
		string nextMissionId = "";
		if (continueChain && m_ActiveDefinition)
			nextMissionId = m_ActiveDefinition.NextMissionId;

		if (nextMissionId != "")
		{
			DZRMZ_MissionDefinition nextDefinition = FindDefinition(nextMissionId);
			if (nextDefinition)
			{
				DZRMZ_Log.Info(string.Format("Missionskette setzt mit %1 fort.", nextMissionId));
				StartMissionDefinition(nextDefinition);
				return;
			}
			DZRMZ_Log.Error(string.Format("Folgemission %1 fehlt. Kette wird sicher beendet.", nextMissionId));
		}

		int cooldown = m_Settings.MissionCooldownMinSeconds;
		if (m_Settings.MissionCooldownMaxSeconds > m_Settings.MissionCooldownMinSeconds)
			cooldown = Math.RandomInt(m_Settings.MissionCooldownMinSeconds, m_Settings.MissionCooldownMaxSeconds + 1);

		m_Runtime.Reset();
		m_Runtime.State = DZRMZ_EMissionState.DZRMZ_STATE_COOLDOWN;
		m_Runtime.SecondsUntilNextMission = cooldown;
		m_ActiveDefinition = null;
		m_ObjectiveLocation = null;
		m_DeliveryLocation = null;
		m_PayoutCommitOwnedByProcess = false;
		m_RecoveryPayoutWarningLogged = false;
		DZRMZ_Storage.SaveHistory(m_History);
		DZRMZ_Storage.SaveRuntime(m_Runtime);
	}

	protected string BuildCompletionId()
	{
		if (!m_Runtime || !m_ActiveDefinition || !m_ObjectiveLocation || !m_History)
			return "";
		string instanceId = m_Runtime.MissionInstanceId;
		if (instanceId == "")
			instanceId = string.Format("legacy-%1-%2-%3-%4", m_History.TotalStarted, m_ActiveDefinition.Id, m_ObjectiveLocation.Id, m_Runtime.DeliveryLocationId);
		return instanceId + "|" + m_Runtime.OwnerUid;
	}

	protected DZRMZ_HistoryEntry EnsureCompletionHistoryEntry()
	{
		if (!m_History || !m_History.Entries || !m_Runtime || m_Runtime.CompletionId == "")
			return null;

		foreach (DZRMZ_HistoryEntry existingEntry : m_History.Entries)
		{
			if (existingEntry && existingEntry.CompletionId == m_Runtime.CompletionId)
				return existingEntry;
		}

		ref DZRMZ_HistoryEntry entry = new DZRMZ_HistoryEntry;
		entry.Result = "COMPLETED";
		entry.OwnerUid = m_Runtime.OwnerUid;
		entry.OwnerName = m_Runtime.OwnerName;
		entry.ObjectiveLocationId = m_Runtime.ObjectiveLocationId;
		entry.DeliveryLocationId = m_Runtime.DeliveryLocationId;
		entry.CompletionId = m_Runtime.CompletionId;
		entry.MissionId = m_ActiveDefinition.Id;
		entry.MissionTitle = m_ActiveDefinition.Title;
		entry.Category = m_ActiveDefinition.Category;
		entry.RewardReputationGranted = 0;
		m_History.TotalCompleted++;
		m_History.Entries.Insert(entry);
		while (m_History.Entries.Count() > 100)
			m_History.Entries.RemoveOrdered(0);
		DZRMZ_Storage.SaveHistory(m_History);
		return entry;
	}

	protected bool ReconcilePayoutMarkers(DZRMZ_HistoryEntry completionEntry)
	{
		if (!completionEntry || !m_Runtime || !m_History)
			return false;

		bool runtimeChanged = false;
		bool historyChanged = false;
		if (completionEntry.RewardPayoutCommitted && !m_Runtime.RewardPayoutCommitted)
		{
			m_Runtime.RewardPayoutCommitted = true;
			runtimeChanged = true;
		}
		if (m_Runtime.RewardPayoutCommitted && !completionEntry.RewardPayoutCommitted)
		{
			completionEntry.RewardPayoutCommitted = true;
			historyChanged = true;
		}
		if (completionEntry.RewardPayoutFinished && !m_Runtime.RewardPayoutFinished)
		{
			m_Runtime.RewardPayoutFinished = true;
			m_Runtime.RewardItemsGranted = completionEntry.RewardItemsGranted;
			m_Runtime.RewardReputationGranted = completionEntry.RewardReputationGranted;
			runtimeChanged = true;
		}
		if (m_Runtime.RewardPayoutFinished && !completionEntry.RewardPayoutFinished)
		{
			completionEntry.RewardPayoutFinished = true;
			completionEntry.RewardItemsGranted = m_Runtime.RewardItemsGranted;
			completionEntry.RewardReputationGranted = m_Runtime.RewardReputationGranted;
			historyChanged = true;
		}

		if (runtimeChanged && !DZRMZ_Storage.SaveRuntime(m_Runtime))
			return false;
		if (historyChanged && !DZRMZ_Storage.SaveHistory(m_History))
			return false;
		return true;
	}

	protected void AnnounceCompletedMission(PlayerBase owner, bool payoutOmittedBecauseOwnerMissing)
	{
		string ownerName = m_Runtime.OwnerName;
		if (ownerName == "")
			ownerName = "Unbekannter Ueberlebender";

		if (owner)
		{
			string conclusion = m_ActiveDefinition.CompletionText;
			string aftermath = GetHelpAftermathText();
			if (aftermath != "") conclusion += "\n\nEINSATZAKTE (TEXT): " + aftermath;
			if (conclusion != "") DZRMZ_RadioService.SendToPlayer(m_Settings, owner, "89,5 MHz", conclusion);
			string payoutMessage;
			if (m_Runtime.RewardItemsGranted < 0)
				payoutMessage = "Mission abgeschlossen. Nach einem Neustart war der Auszahlungsstatus nicht eindeutig; zum Schutz vor Duplikaten wurde nicht erneut ausgezahlt. Bitte einen Admin kontaktieren.";
			else if (payoutOmittedBecauseOwnerMissing)
				payoutMessage = "Mission abgeschlossen. Die Wiederverbindungsfrist fuer die direkte Belohnung ist abgelaufen.";
			else
				payoutMessage = string.Format("Mission abgeschlossen. %1 Belohnungsgegenstaende wurden direkt vergeben.", m_Runtime.RewardItemsGranted);
			DZRMZ_RadioService.SendToPlayer(m_Settings, owner, "DeutschZ Mission erfolgreich", payoutMessage);
		}

		DZRMZ_RadioService.SendRadioLine(m_Settings, string.Format("Bestaetigt: %1 hat '%2' bei %3 abgeschlossen.", ownerName, m_ActiveDefinition.Title, m_ObjectiveLocation.Name));
		DZRMZ_Log.Info(string.Format("Mission %1 erfolgreich durch %2. CompletionId=%3, RewardCommitted=%4, RewardFinished=%5, Items=%6, Hardline=%7.", m_ActiveDefinition.Id, ownerName, m_Runtime.CompletionId, m_Runtime.RewardPayoutCommitted, m_Runtime.RewardPayoutFinished, m_Runtime.RewardItemsGranted, m_Runtime.RewardReputationGranted));
	}

	protected int GrantHardlineReputation(PlayerBase owner)
	{
		if (!owner || !m_Settings || !m_Settings.HardlineRewardsEnabled || !m_ActiveDefinition)
			return 0;

		int points = m_Settings.SideMissionReputation;
		if (m_ActiveDefinition.Id.IndexOf("LIVE-") == 0)
			points = m_Settings.EventMissionReputation;
		if (m_ActiveDefinition.ChainStart && m_ActiveDefinition.StartRequiredItemClass != "")
			points = m_Settings.EventGateReputation;
		if (m_ActiveDefinition.Id == "LIVE-OPERATION-02")
			points = m_Settings.FinalOperationReputation;
		if (points <= 0)
			return 0;

	#ifdef EXPANSIONMODHARDLINE
		if (!owner.GetIdentity() || !GetExpansionSettings().GetHardline().UseReputation || owner.Expansion_GetReputation() < 0)
			return 0;
		int before = owner.Expansion_GetReputation();
		if (!owner.Expansion_SetReputation(before + points, true))
		{
			DZRMZ_Log.Error("Hardline-Speicherung fehlgeschlagen. CompletionId=" + m_Runtime.CompletionId);
			return -1;
		}
		points = owner.Expansion_GetReputation() - before;
		DZRMZ_Log.Info(string.Format("Hardline: %1 Reputation an %2 fuer %3 vergeben.", points, m_Runtime.OwnerName, m_ActiveDefinition.Id));
		return points;
	#else
		DZRMZ_Log.Warn("Hardline-Reputation ist konfiguriert, aber EXPANSIONMODHARDLINE ist nicht geladen.");
		return 0;
	#endif
	}

	protected void AddHistoryEntry(string result)
	{
		ref DZRMZ_HistoryEntry entry = new DZRMZ_HistoryEntry;
		entry.Result = result;
		entry.OwnerUid = m_Runtime.OwnerUid;
		entry.OwnerName = m_Runtime.OwnerName;
		entry.ObjectiveLocationId = m_Runtime.ObjectiveLocationId;
		entry.DeliveryLocationId = m_Runtime.DeliveryLocationId;
		if (m_ActiveDefinition)
		{
			entry.MissionId = m_ActiveDefinition.Id;
			entry.MissionTitle = m_ActiveDefinition.Title;
			entry.Category = m_ActiveDefinition.Category;
		}

		m_History.Entries.Insert(entry);
		while (m_History.Entries.Count() > 100)
			m_History.Entries.RemoveOrdered(0);
	}

	protected bool RestoreRuntimeReferences()
	{
		if (!m_Runtime)
			return false;
		if (m_Runtime.State == DZRMZ_EMissionState.DZRMZ_STATE_IDLE || m_Runtime.State == DZRMZ_EMissionState.DZRMZ_STATE_COOLDOWN)
			return true;

		m_ActiveDefinition = FindDefinition(m_Runtime.DefinitionId);
		m_ObjectiveLocation = FindLocation(m_Runtime.ObjectiveLocationId);
		m_DeliveryLocation = null;
		if (m_Runtime.DeliveryLocationId != "")
			m_DeliveryLocation = FindLocation(m_Runtime.DeliveryLocationId);
		if (!m_ActiveDefinition || !m_ObjectiveLocation)
			return false;
		if (m_ActiveDefinition.ObjectiveType == DZRMZ_OBJECTIVE_DELIVERY && !m_DeliveryLocation)
			return false;
		return true;
	}

	protected DZRMZ_MissionDefinition FindDefinition(string id)
	{
		if (!m_MissionCatalog || id == "")
			return null;
		foreach (DZRMZ_MissionDefinition definition : m_MissionCatalog.Missions)
		{
			if (definition && definition.Id == id)
				return definition;
		}
		return null;
	}

	protected DZRMZ_Location FindLocation(string id)
	{
		if (!m_LocationCatalog || id == "")
			return null;
		foreach (DZRMZ_Location location : m_LocationCatalog.Locations)
		{
			if (location && location.Id == id)
				return location;
		}
		return null;
	}

	protected DZRMZ_MissionDefinition SelectMissionDefinition()
	{
		bool hasChainMetadata = false;
		foreach (DZRMZ_MissionDefinition metadata : m_MissionCatalog.Missions)
		{
			if (metadata && metadata.ChainStart)
				hasChainMetadata = true;
		}
		ref array<ref DZRMZ_MissionDefinition> candidates = new array<ref DZRMZ_MissionDefinition>;
		foreach (DZRMZ_MissionDefinition definition : m_MissionCatalog.Missions)
		{
			if (definition && definition.IsValid() && definition.ChainStart && IsStartRequirementAvailable(definition) && m_History.RecentMissionIds.Find(definition.Id) == -1)
				candidates.Insert(definition);
		}

		if (candidates.Count() == 0)
		{
			foreach (DZRMZ_MissionDefinition fallbackDefinition : m_MissionCatalog.Missions)
			{
				if (fallbackDefinition && fallbackDefinition.IsValid() && fallbackDefinition.ChainStart && IsStartRequirementAvailable(fallbackDefinition))
					candidates.Insert(fallbackDefinition);
			}
		}

		// Never start a chain's middle chapter when its entry requirement is unavailable.
		if (candidates.Count() == 0 && !hasChainMetadata)
		{
			foreach (DZRMZ_MissionDefinition legacyDefinition : m_MissionCatalog.Missions)
			{
				if (legacyDefinition && legacyDefinition.IsValid() && IsStartRequirementAvailable(legacyDefinition) && m_History.RecentMissionIds.Find(legacyDefinition.Id) == -1)
					candidates.Insert(legacyDefinition);
			}

			if (candidates.Count() == 0)
			{
				foreach (DZRMZ_MissionDefinition legacyFallbackDefinition : m_MissionCatalog.Missions)
				{
					if (legacyFallbackDefinition && legacyFallbackDefinition.IsValid() && IsStartRequirementAvailable(legacyFallbackDefinition))
						candidates.Insert(legacyFallbackDefinition);
				}
			}

			if (candidates.Count() > 0)
				DZRMZ_Log.Warn("Keine verwendbare ChainStart-Markierung gefunden; kompatible Einzelmission aus dem vorhandenen Katalog wird gestartet.");
		}

		if (candidates.Count() == 0)
			return null;
		return candidates[Math.RandomInt(0, candidates.Count())];
	}

	protected bool IsStartRequirementAvailable(DZRMZ_MissionDefinition definition)
	{
		if (!definition || definition.StartRequiredItemClass == "")
			return true;

		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		foreach (Man man : players)
		{
			PlayerBase player;
			if (!Class.CastTo(player, man) || !player.IsAlive() || !player.GetIdentity())
				continue;

			if (PlayerHasStartRequirement(player, definition))
				return true;
		}
		return false;
	}

	protected bool PlayerHasStartRequirement(PlayerBase player, DZRMZ_MissionDefinition definition)
	{
		if (!player || !definition)
			return false;
		if (definition.StartRequiredItemClass == "")
			return true;
		array<EntityAI> inventory = new array<EntityAI>;
		player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, inventory);
		foreach (EntityAI item : inventory)
		{
			if (item && !item.IsRuined() && item.IsKindOf(definition.StartRequiredItemClass))
				return true;
		}
		return false;
	}

	protected DZRMZ_Location SelectLocation(string requiredTag, string excludedId)
	{
		ref array<ref DZRMZ_Location> candidates = new array<ref DZRMZ_Location>;
		foreach (DZRMZ_Location location : m_LocationCatalog.Locations)
		{
			if (location && location.Id != excludedId && location.HasTag(requiredTag) && m_History.RecentLocationIds.Find(location.Id) == -1)
				candidates.Insert(location);
		}

		if (candidates.Count() == 0)
		{
			foreach (DZRMZ_Location fallbackLocation : m_LocationCatalog.Locations)
			{
				if (fallbackLocation && fallbackLocation.Id != excludedId && fallbackLocation.HasTag(requiredTag))
					candidates.Insert(fallbackLocation);
			}
		}

		if (candidates.Count() == 0)
			return null;
		return candidates[Math.RandomInt(0, candidates.Count())];
	}

	protected PlayerBase FindOnlinePlayer(string uid)
	{
		if (uid == "")
			return null;

		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		foreach (Man man : players)
		{
			PlayerBase player;
			if (Class.CastTo(player, man) && player.GetIdentity() && player.GetIdentity().GetPlainId() == uid)
				return player;
		}
		return null;
	}

	protected string ExpandRadioLine(string templateText)
	{
		string result = templateText;
		string itemLabel = m_ActiveDefinition.RequiredItemLabel;
		if (itemLabel == "")
			itemLabel = m_ActiveDefinition.RequiredItemClass;
		result.Replace("{MISSION}", m_ActiveDefinition.Title);
		result.Replace("{LOCATION}", m_ObjectiveLocation.Name);
		result.Replace("{DELIVERY}", GetDeliveryName());
		result.Replace("{ITEM}", itemLabel);
		result.Replace("{COUNT}", m_ActiveDefinition.RequiredQuantity.ToString());
		result.Replace("{HOLD}", m_ActiveDefinition.HoldSeconds.ToString());
		result.Replace("{FREQUENCY}", m_Settings.RadioFrequencyMHz.ToString());
		return result;
	}

	protected string GetDeliveryName()
	{
		if (m_DeliveryLocation)
			return m_DeliveryLocation.Name;
		return "keine";
	}

	protected void RememberRecent(array<string> history, string id, int maximum)
	{
		if (!history || id == "")
			return;
		if (maximum <= 0)
		{
			history.Clear();
			return;
		}
		int existingIndex = history.Find(id);
		if (existingIndex > -1)
			history.RemoveOrdered(existingIndex);
		history.Insert(id);
		while (history.Count() > maximum)
			history.RemoveOrdered(0);
	}

	protected void NormalizeRecentHistory()
	{
		if (!m_Settings || !m_History)
			return;

		if (m_Settings.RecentMissionMemory <= 0)
			m_History.RecentMissionIds.Clear();
		else
		{
			while (m_History.RecentMissionIds.Count() > m_Settings.RecentMissionMemory)
				m_History.RecentMissionIds.RemoveOrdered(0);
		}

		if (m_Settings.RecentLocationMemory <= 0)
			m_History.RecentLocationIds.Clear();
		else
		{
			while (m_History.RecentLocationIds.Count() > m_Settings.RecentLocationMemory)
				m_History.RecentLocationIds.RemoveOrdered(0);
		}
		DZRMZ_Storage.SaveHistory(m_History);
	}
}






