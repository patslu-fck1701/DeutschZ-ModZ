class DZECZ_EventManager
{
	protected static ref DZECZ_EventManager s_Instance;
	protected bool m_Initialized;
	protected ref DZECZ_Config m_Config;
	protected ref DZECZ_Progress m_Progress;
	protected Object m_Receiver;
	protected Object m_ActiveObject;
	protected Object m_FinalTerminal;
	protected ref array<Object> m_SpawnedObjects;
	protected ref array<EntityAI> m_Threats;
	protected string m_ActionActorUID;
	protected Object m_ActionTarget;
	protected int m_ActionStartedAt;
	protected float m_ActionDuration;
	protected int m_ThreatPhaseKey;
	protected bool m_RiskyEndingPending;
	protected vector m_FinalPosition;

	void DZECZ_EventManager()
	{
		m_Config = new DZECZ_Config;
		m_Progress = new DZECZ_Progress;
		m_SpawnedObjects = new array<Object>;
		m_Threats = new array<EntityAI>;
		m_ActionActorUID = "";
		m_ActionStartedAt = 0;
		m_ActionDuration = 0.0;
		m_ThreatPhaseKey = -1;
		m_RiskyEndingPending = false;
		m_FinalPosition = "0 0 0";
	}

	static DZECZ_EventManager GetInstance()
	{
		if (!s_Instance)
			s_Instance = new DZECZ_EventManager;
		return s_Instance;
	}

	void InitServer()
	{
		if (m_Initialized || !GetGame() || !GetGame().IsServer())
			return;

		m_Initialized = true;
		m_Config = DZECZ_Config.Load();
		LoadProgress();
		SpawnReceiver();
		RestoreProgress();
		DZECZ_Utils.Log("Server initialized. Version=" + DZECZ_VERSION + " state=" + m_Progress.State.ToString());
	}

	float GetActionDuration(Object target)
	{
		if (!m_Config)
			m_Config = new DZECZ_Config;

		if (DZECZ_NWAFStation.Cast(target))
			return m_Config.StationActivationSecondsNWAF;
		if (DZECZ_TisyStation.Cast(target))
			return m_Config.StationActivationSecondsTisy;
		if (DZECZ_GasZoneStation.Cast(target))
			return m_Config.StationActivationSecondsGasZone;
		if (DZECZ_FinalTerminal.Cast(target))
			return m_Config.FinalTerminalSeconds;

		DZECZ_FireworkBattery battery = DZECZ_FireworkBattery.Cast(target);
		if (battery)
		{
			if (battery.DZECZ_GetStep() == 0)
				return m_Config.FireworkBatteryInsertBatterySeconds;
			if (battery.DZECZ_GetStep() == 1)
				return m_Config.FireworkBatterySyncSeconds;
			return m_Config.FireworkBatteryIgnitionSeconds;
		}

		return 1.0;
	}

	void TryStartEvent(PlayerBase player, Object target)
	{
		if (!CanUse(player, target) || target != m_Receiver)
			return;

		if (!m_Config || !m_Config.Enabled)
		{
			DZECZ_Utils.Send(player, "Das Eclipse-Protokoll ist deaktiviert.");
			return;
		}

		if (m_Progress.State != DZECZ_INACTIVE)
		{
			DZECZ_Utils.Send(player, "Operation EclipseZ laeuft bereits.");
			return;
		}

		if (m_Config.RequiresBattlegroundZUnlock && !DZECZ_Utils.HasBattlegroundUnlock(player))
		{
			DZECZ_Utils.Send(player, "Zugriff verweigert: BattlegroundZ-Freischaltung fehlt.");
			return;
		}

		if (!DZECZ_Utils.HasStartItems(target.GetPosition(), m_Config.GroupItemCheckRadius, m_Config))
		{
			DZECZ_Utils.Send(player, "Im 10-Meter-Teamradius fehlen Storygegenstaende.");
			return;
		}

		m_Progress.Reset();
		m_Progress.ActivatorUID = DZECZ_Utils.GetUID(player);
		BuildRandomStationOrder();
		SetState(DZECZ_RIDDLE_STARTED);
		DZECZ_Utils.Broadcast("Drei tote Stimmen antworten: Wo Fluegel sterben, Stahl im Norden rostet und Gift den Boden frisst.", 15.0);
		DZECZ_Utils.Send(player, "Das Signal erkennt dich. Die Reihenfolge der drei Stationen wurde entschluesselt.", 12.0);
		SpawnCurrentStation();
	}

	bool StartTimedAction(PlayerBase player, Object target)
	{
		if (!CanUse(player, target) || target != m_ActiveObject)
			return false;

		if (m_ActionTarget)
		{
			DZECZ_Utils.Send(player, "Dieses Eclipse-System wird bereits bedient.");
			return false;
		}

		if (!ValidateStateForTarget(target))
		{
			DZECZ_Utils.Send(player, "Das Objekt reagiert in dieser Phase nicht.");
			return false;
		}

		if (DZECZ_FinalTerminal.Cast(target) && m_Config.RequiresBattlegroundZUnlock && !DZECZ_Utils.HasBattlegroundUnlock(player))
		{
			DZECZ_Utils.Send(player, "Das Hauptterminal erkennt deine BattlegroundZ-Freischaltung nicht.");
			return false;
		}

		if (!FindRequiredItem(target))
		{
			DZECZ_Utils.Send(player, GetMissingItemMessage(target));
			return false;
		}

		m_ActionActorUID = DZECZ_Utils.GetUID(player);
		m_ActionTarget = target;
		m_ActionStartedAt = GetGame().GetTime();
		m_ActionDuration = GetActionDuration(target);
		BeginThreatPhase(target);
		DZECZ_Utils.Log("Timed action started actor=" + DZECZ_Utils.GetName(player) + " target=" + target.GetType() + " duration=" + m_ActionDuration.ToString());
		return true;
	}

	void CancelTimedAction(PlayerBase player, Object target)
	{
		if (!IsActiveAction(player, target))
			return;

		bool finalAborted = DZECZ_FinalTerminal.Cast(target) != null;
		ClearActiveAction();
		if (finalAborted)
		{
			SetState(DZECZ_FINAL_ZONE_ACTIVE);
			DZECZ_Utils.Send(player, "Terminalabbruch: Fortschritt auf 0 zurueckgesetzt.");
		}
		DZECZ_Utils.Log("Timed action cancelled by " + DZECZ_Utils.GetName(player));
	}

	bool CompleteTimedAction(PlayerBase player, Object target)
	{
		if (!IsActiveAction(player, target) || !CanUse(player, target) || !ValidateStateForTarget(target))
			return RejectFinish(player, "Aktion wurde serverseitig verworfen.");

		int elapsed = GetGame().GetTime() - m_ActionStartedAt;
		int required = Math.Round(m_ActionDuration * 1000.0) - 1500;
		if (required < 0)
			required = 0;
		if (elapsed < required)
			return RejectFinish(player, "Aktion war zu kurz und wurde zurueckgesetzt.");

		EntityAI requiredItem = FindRequiredItem(target);
		if (!requiredItem)
			return RejectFinish(player, GetMissingItemMessage(target));

		ClearActiveAction();
		DZECZ_Utils.Log("Timed action completed actor=" + DZECZ_Utils.GetName(player) + " target=" + target.GetType() + " elapsedMs=" + elapsed.ToString());

		if (DZECZ_NWAFStation.Cast(target) || DZECZ_TisyStation.Cast(target) || DZECZ_GasZoneStation.Cast(target))
		{
			CompleteStation(player, target, requiredItem);
			return true;
		}

		if (DZECZ_FireworkBattery.Cast(target))
		{
			CompleteBatteryStep(player, DZECZ_FireworkBattery.Cast(target), requiredItem);
			return true;
		}

		if (DZECZ_FinalTerminal.Cast(target))
		{
			SetState(DZECZ_FINAL_CHOICE_ACTIVE);
			DZECZ_Utils.Broadcast("Das Eclipse-Hauptterminal ist offen. Die Verteidiger muessen fallen, bevor das Signal entschieden wird.", 12.0);
			return true;
		}

		return false;
	}

	void ChooseEnding(PlayerBase player, Object target, bool risky)
	{
		if (!CanUse(player, target) || target != m_FinalTerminal || m_Progress.State != DZECZ_FINAL_CHOICE_ACTIVE)
			return;

		if (m_Config.RequiresBattlegroundZUnlock && !DZECZ_Utils.HasBattlegroundUnlock(player))
		{
			DZECZ_Utils.Send(player, "Nur ein erkannter Storytraeger darf den letzten Befehl geben.");
			return;
		}

		int alive = CountAliveThreats();
		if (alive > 0)
		{
			DZECZ_Utils.Send(player, "Noch " + alive.ToString() + " Eclipse-Verteidiger aktiv.");
			return;
		}

		if (!risky)
		{
			DZECZ_Utils.Broadcast("Eclipse-Signal wurde abgeschaltet. Chernarus faellt zurueck in Stille.", 15.0);
			CompleteReward(false);
			return;
		}

		m_RiskyEndingPending = true;
		SetState(DZECZ_FINAL_DEFENSE_ACTIVE);
		DZECZ_Utils.Broadcast("Eclipse-Signal wurde gesendet. Etwas hat geantwortet.", 15.0);
		SpawnInfected(m_FinalPosition, "ZmbM_SoldierNormal", 8, 12.0, 35.0, false);
		SpawnExpansionFactions(m_FinalPosition, 4);
		SpawnGuardian(m_FinalPosition);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CheckRiskyEnding, 5000, false);
	}

	bool ShouldBlockVehicleContact(Object unit)
	{
		if (!unit || m_Progress.State == DZECZ_INACTIVE || m_Progress.State == DZECZ_COMPLETED)
			return false;
		foreach (EntityAI threat: m_Threats)
		{
			if (threat && threat == unit)
				return true;
		}
		return false;
	}

	protected bool CanUse(PlayerBase player, Object target)
	{
		if (!DZECZ_Utils.IsValidPlayer(player) || !target || !m_Config)
			return false;
		return vector.Distance(player.GetPosition(), target.GetPosition()) <= m_Config.PlayerInteractionDistance + 0.75;
	}

	protected bool IsActiveAction(PlayerBase player, Object target)
	{
		return player && target && m_ActionTarget == target && m_ActionActorUID == DZECZ_Utils.GetUID(player);
	}

	protected bool RejectFinish(PlayerBase player, string message)
	{
		ClearActiveAction();
		DZECZ_Utils.Send(player, message);
		DZECZ_Utils.Warn("Timed action finish rejected: " + message);
		return false;
	}

	protected void ClearActiveAction()
	{
		m_ActionActorUID = "";
		m_ActionTarget = null;
		m_ActionStartedAt = 0;
		m_ActionDuration = 0.0;
	}

	protected bool ValidateStateForTarget(Object target)
	{
		if (target != m_ActiveObject)
			return false;
		if (DZECZ_NWAFStation.Cast(target) || DZECZ_TisyStation.Cast(target) || DZECZ_GasZoneStation.Cast(target))
			return m_Progress.CurrentStationOrderIndex < 3;
		if (DZECZ_FireworkBattery.Cast(target))
			return m_Progress.State == DZECZ_FIREWORK_BATTERY_REVEALED || m_Progress.State == DZECZ_FIREWORK_BATTERY_ARMING;
		if (DZECZ_FinalTerminal.Cast(target))
			return m_Progress.State == DZECZ_FINAL_ZONE_ACTIVE || m_Progress.State == DZECZ_FINAL_TERMINAL_ACTIVE || m_Progress.State == DZECZ_FINAL_DEFENSE_ACTIVE;
		return false;
	}

	protected EntityAI FindRequiredItem(Object target)
	{
		if (!target || !m_Config)
			return null;
		vector center = target.GetPosition();
		float radius = m_Config.GroupItemCheckRadius;

		if (DZECZ_NWAFStation.Cast(target))
			return DZECZ_Utils.FindItemNear(center, radius, "DZBBC_Coded9VBattery");
		if (DZECZ_TisyStation.Cast(target))
			return DZECZ_Utils.FindItemNear(center, radius, "DeutschZ_FireworksLauncher");
		if (DZECZ_GasZoneStation.Cast(target))
			return DZECZ_Utils.FindGasFlareNear(center, radius);
		if (DZECZ_FinalTerminal.Cast(target))
			return EntityAI.Cast(target);

		DZECZ_FireworkBattery battery = DZECZ_FireworkBattery.Cast(target);
		if (battery)
		{
			if (battery.DZECZ_GetStep() == 0)
				return DZECZ_Utils.FindItemNear(center, radius, "DZBBC_Coded9VBattery");
			if (battery.DZECZ_GetStep() == 1)
				return DZECZ_Utils.FindItemNear(center, radius, "DeutschZ_FireworksLauncher");
			if (battery.DZECZ_GetStep() == 2)
				return DZECZ_Utils.FindItemNear(center, radius, "DZBBC_SignalMatches");
		}
		return null;
	}

	protected string GetMissingItemMessage(Object target)
	{
		if (DZECZ_NWAFStation.Cast(target))
			return "Codierte 9V-Batterie im Teamradius fehlt.";
		if (DZECZ_TisyStation.Cast(target))
			return "DeutschZ Fireworkslauncher im Teamradius fehlt.";
		if (DZECZ_GasZoneStation.Cast(target))
			return "Gaszonen-Leuchtfackel im Teamradius fehlt.";
		DZECZ_FireworkBattery battery = DZECZ_FireworkBattery.Cast(target);
		if (battery && battery.DZECZ_GetStep() == 0)
			return "Codierte 9V-Batterie im Teamradius fehlt.";
		if (battery && battery.DZECZ_GetStep() == 1)
			return "DeutschZ Fireworkslauncher im Teamradius fehlt.";
		if (battery && battery.DZECZ_GetStep() == 2)
			return "DZBBC SignalMatches im Teamradius fehlen.";
		return "Voraussetzung fehlt.";
	}

	protected void CompleteStation(PlayerBase player, Object target, EntityAI requiredItem)
	{
		int stationId = GetStationId(target);
		if (stationId == 2 && m_Config.ConsumeGasZoneFlare)
			DZECZ_Utils.ConsumeItem(requiredItem);

		string fragment = "Alpha";
		if (m_Progress.CurrentStationOrderIndex == 1)
			fragment = "Bravo";
		if (m_Progress.CurrentStationOrderIndex == 2)
			fragment = "Charlie";

		SetStationCompleteState(m_Progress.CurrentStationOrderIndex);
		DZECZ_Utils.Broadcast("Eclipse Fragment " + fragment + " wurde reaktiviert. Kein Traeger wird verfolgt.", 10.0);
		CleanupThreats();
		DeleteObject(target);
		m_ActiveObject = null;
		m_Progress.CurrentStationOrderIndex++;
		SaveProgress();

		if (m_Progress.CurrentStationOrderIndex < 3)
		{
			SpawnCurrentStation();
			return;
		}

		SetState(DZECZ_ALL_FRAGMENTS_COMPLETE);
		DZECZ_Utils.Broadcast("Drei alte Signale antworten. Letzter Abgleich an der FeuerwerkBattery erforderlich.", 14.0);
		SpawnFireworkBattery();
	}

	protected void CompleteBatteryStep(PlayerBase player, DZECZ_FireworkBattery battery, EntityAI requiredItem)
	{
		int step = battery.DZECZ_GetStep();
		if (step == 0)
		{
			if (m_Config.ConsumeCodedBattery)
				DZECZ_Utils.ConsumeItem(requiredItem);
			m_Progress.BatteryStep = 1;
			battery.DZECZ_SetStep(1);
			SetState(DZECZ_FIREWORK_BATTERY_ARMING);
			DZECZ_Utils.Send(player, "Stromkreis aktiv. Jetzt den Fireworkslauncher synchronisieren.");
			return;
		}

		if (step == 1)
		{
			if (m_Config.DamageFireworksLauncher)
				DZECZ_Utils.DamageLauncher(requiredItem);
			m_Progress.BatteryStep = 2;
			battery.DZECZ_SetStep(2);
			SetState(DZECZ_FIREWORK_BATTERY_ARMING);
			DZECZ_Utils.Send(player, "Signal synchronisiert. SignalMatches zur Zuendung vorbereiten.");
			return;
		}

		if (step == 2)
		{
			if (m_Config.ConsumeSignalMatches)
				DZECZ_Utils.ConsumeItem(requiredItem);
			m_Progress.BatteryStep = 3;
			battery.DZECZ_SetStep(3);
			SetState(DZECZ_FIREWORK_COUNTDOWN);
			DZECZ_Utils.Broadcast("Eclipse-Signalstart in 30 Sekunden. Verlasse den unmittelbaren Bereich.", 12.0);
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(FinishFireworkCountdown, Math.Round(m_Config.FireworkCountdownSeconds * 1000.0), false);
		}
	}

	protected void FinishFireworkCountdown()
	{
		if (m_Progress.State != DZECZ_FIREWORK_COUNTDOWN)
			return;

		DZECZ_FireworkBattery battery = DZECZ_FireworkBattery.Cast(m_ActiveObject);
		if (battery)
			battery.DZECZ_Launch();

		SetState(DZECZ_ECLIPSE_SIGNAL_SENT);
		DZECZ_Utils.Broadcast("Ein codiertes Feuerwerk wurde ueber Chernarus gesichtet. Alte Empfaengerstationen reagieren.", 15.0);
		SelectFinalLocation();
		SpawnFinalTerminal();
	}

	protected void SpawnReceiver()
	{
		vector pos = DZECZ_Utils.Grounded(DZECZ_Config.ToVector(m_Config.ReceiverPosition));
		m_Receiver = GetGame().CreateObjectEx("DZECZ_StartReceiver", pos, ECE_PLACE_ON_SURFACE);
		if (m_Receiver)
			DZECZ_Utils.Log("Start receiver spawned at " + pos.ToString());
		else
			DZECZ_Utils.Warn("Start receiver spawn failed.");
	}

	protected void BuildRandomStationOrder()
	{
		m_Progress.StationOrder.Clear();
		m_Progress.StationOrder.Insert(0);
		m_Progress.StationOrder.Insert(1);
		m_Progress.StationOrder.Insert(2);
		for (int i = m_Progress.StationOrder.Count() - 1; i > 0; i--)
		{
			int swapIndex = Math.RandomIntInclusive(0, i);
			int value = m_Progress.StationOrder[i];
			m_Progress.StationOrder[i] = m_Progress.StationOrder[swapIndex];
			m_Progress.StationOrder[swapIndex] = value;
		}
		SaveProgress();
	}

	protected void SpawnCurrentStation()
	{
		if (!m_Progress.StationOrder || m_Progress.CurrentStationOrderIndex >= m_Progress.StationOrder.Count())
			return;

		int stationId = m_Progress.StationOrder[m_Progress.CurrentStationOrderIndex];
		string className = "DZECZ_NWAFStation";
		vector position = DZECZ_Config.ToVector(m_Config.NWAFPosition);
		string sectorText = "Altes NWAF-Signal: Nordwestlicher Luftwaffenbereich";
		if (stationId == 1)
		{
			className = "DZECZ_TisyStation";
			position = DZECZ_Config.ToVector(m_Config.TisyPosition);
			sectorText = "Tisy-Relais: geschaetzter Ursprung Nordsektor";
		}
		else if (stationId == 2)
		{
			className = "DZECZ_GasZoneStation";
			position = DZECZ_Config.ToVector(m_Config.GasZonePosition);
			sectorText = "Kontaminiertes Signallicht: Dekontaminationsfenster instabil";
		}

		position = DZECZ_Utils.Grounded(position);
		m_ActiveObject = SpawnManagedObject(className, position);
		m_ThreatPhaseKey = -1;
		SetStationActiveState(m_Progress.CurrentStationOrderIndex);
		DZECZ_Utils.ShowGlobalSector(sectorText, position, m_Config.GlobalSectorHintSizeMeters);
		DZECZ_Utils.Log("Station order index=" + m_Progress.CurrentStationOrderIndex.ToString() + " stationId=" + stationId.ToString() + " spawned at " + position.ToString());
	}

	protected void SpawnFireworkBattery()
	{
		vector position = DZECZ_Utils.Grounded(DZECZ_Config.ToVector(m_Config.FireworkBatteryPosition));
		m_ActiveObject = SpawnManagedObject("DZECZ_FireworkBattery", position);
		DZECZ_FireworkBattery battery = DZECZ_FireworkBattery.Cast(m_ActiveObject);
		if (battery)
			battery.DZECZ_SetStep(m_Progress.BatteryStep);
		m_ThreatPhaseKey = -1;
		SetState(DZECZ_FIREWORK_BATTERY_REVEALED);
		DZECZ_Utils.ShowGlobalSector("Eclipse FeuerwerkBattery - grober Suchsektor", position, m_Config.GlobalSectorHintSizeMeters);
	}

	protected void SelectFinalLocation()
	{
		if (!m_Config.FinalPositions || m_Config.FinalPositions.Count() == 0)
		{
			m_Progress.FinalLocationIndex = 0;
			m_FinalPosition = DZECZ_Config.ToVector(m_Config.TisyPosition);
		}
		else
		{
			m_Progress.FinalLocationIndex = Math.RandomInt(0, m_Config.FinalPositions.Count());
			m_FinalPosition = DZECZ_Config.ToVector(m_Config.FinalPositions[m_Progress.FinalLocationIndex]);
		}
		m_FinalPosition = DZECZ_Utils.Grounded(m_FinalPosition);
		SaveProgress();
	}

	protected void SpawnFinalTerminal()
	{
		if (m_FinalPosition == "0 0 0")
			SelectFinalLocation();
		m_FinalTerminal = SpawnManagedObject("DZECZ_FinalTerminal", m_FinalPosition);
		m_ActiveObject = m_FinalTerminal;
		m_ThreatPhaseKey = -1;
		SetState(DZECZ_FINAL_ZONE_ACTIVE);
		DZECZ_Utils.ShowGlobalSector("Eclipse-Protokoll: grober finaler Zielsektor", m_FinalPosition, m_Config.GlobalSectorHintSizeMeters);
		DZECZ_Utils.Broadcast("Eclipse-Protokoll teilweise aktiviert. Endsignal instabil. Ein grober Zielsektor ist sichtbar.", 14.0);

		PlayerBase activator = FindPlayer(m_Progress.ActivatorUID);
		if (activator)
		{
			vector hint = DZECZ_Utils.SectorCenter(m_FinalPosition, m_Config.ActivatorHintSizeMeters);
			DZECZ_Utils.Send(activator, "Persoenlicher 400-m-Suchsektor: X " + Math.Round(hint[0]).ToString() + " / Z " + Math.Round(hint[2]).ToString(), 15.0);
		}
	}

	protected void BeginThreatPhase(Object target)
	{
		int phaseKey = m_Progress.State * 10 + m_Progress.CurrentStationOrderIndex + m_Progress.BatteryStep;
		if (m_ThreatPhaseKey == phaseKey)
			return;
		m_ThreatPhaseKey = phaseKey;

		vector position = target.GetPosition();
		if (DZECZ_NWAFStation.Cast(target))
		{
			SpawnInfected(position, "ZmbM_CitizenASkinny_Base", 8, 10.0, 28.0, false);
			SpawnInfected(position, "ZmbM_SoldierNormal", 3, 12.0, 30.0, false);
			SpawnInfected(position, "ZmbM_Mummy", 1, 16.0, 24.0, true);
		}
		else if (DZECZ_TisyStation.Cast(target))
		{
			SpawnExpansionFactions(position, 3);
		}
		else if (DZECZ_GasZoneStation.Cast(target))
		{
			SpawnInfected(position, "ZmbM_NBC_Grey", 10, 10.0, 28.0, false);
			SpawnGuardian(position);
		}
		else if (DZECZ_FinalTerminal.Cast(target))
		{
			SetState(DZECZ_FINAL_DEFENSE_ACTIVE);
			SpawnFinalWaveOne();
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SpawnFinalWaveTwo, 30000, false);
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SpawnFinalWaveThree, 60000, false);
		}
	}

	protected void SpawnFinalWaveOne()
	{
		SpawnInfected(m_FinalPosition, "ZmbM_CitizenASkinny_Base", 12, 14.0, 34.0, false);
		SpawnInfected(m_FinalPosition, "ZmbM_SoldierNormal", 4, 18.0, 36.0, false);
	}

	protected void SpawnFinalWaveTwo()
	{
		if (m_Progress.State != DZECZ_FINAL_DEFENSE_ACTIVE || !m_ActionTarget)
			return;
		SpawnExpansionFactions(m_FinalPosition, 3);
		DZECZ_Utils.Broadcast("Eclipse-Verteidigung: Fraktionsreste erreichen den finalen Sektor.", 8.0);
	}

	protected void SpawnFinalWaveThree()
	{
		if (m_Progress.State != DZECZ_FINAL_DEFENSE_ACTIVE || !m_ActionTarget)
			return;
		SpawnGuardian(m_FinalPosition);
		DZECZ_Utils.Broadcast("Eclipse-Waechter erkannt.", 8.0);
	}

	protected void SpawnExpansionFactions(vector center, int countPerFaction)
	{
		DZBBC_MainConfig main = new DZBBC_MainConfig;
		main.SpawnMinDistance = 18.0;
		main.SpawnMaxDistance = 38.0;

		DZBBC_AIFactionConfig russian = new DZBBC_AIFactionConfig;
		russian.FactionName = "RussianZ";
		russian.Count = 0;
		russian.UseExpansionAI = true;
		russian.ExpansionCount = countPerFaction;
		russian.ExpansionFaction = "RussianZ";
		russian.ApplyConvoyDifficulty(0.03, 0.12, false);
		SpawnExpansionFaction(russian, main, center);

		DZBBC_AIFactionConfig american = new DZBBC_AIFactionConfig;
		american.FactionName = "AmericanZ";
		american.Count = 0;
		american.UseExpansionAI = true;
		american.ExpansionCount = countPerFaction;
		american.ExpansionFaction = "AmericanZ";
		american.ApplyConvoyDifficulty(0.03, 0.12, false);
		SpawnExpansionFaction(american, main, center);
	}

	protected void SpawnExpansionFaction(DZBBC_AIFactionConfig faction, DZBBC_MainConfig main, vector center)
	{
		array<EntityAI> spawned = new array<EntityAI>;
		DZBBC_ExpansionBridge.SpawnAIGroup(faction, main, center, spawned);
		foreach (EntityAI unit: spawned)
		{
			if (unit)
				m_Threats.Insert(unit);
		}
	}

	protected void SpawnInfected(vector center, string typeName, int count, float minDistance, float maxDistance, bool slow)
	{
		for (int i = 0; i < count; i++)
		{
			vector position = RandomPoint(center, minDistance, maxDistance);
			Object object = GetGame().CreateObjectEx(typeName, position, ECE_SETUP | ECE_INITAI | ECE_PLACE_ON_SURFACE);
			EntityAI unit = EntityAI.Cast(object);
			if (!unit)
			{
				if (object)
					GetGame().ObjectDelete(object);
				DZECZ_Utils.Warn("Threat spawn failed: " + typeName);
				continue;
			}
			m_Threats.Insert(unit);
			if (slow)
			{
				DayZInfected infected = DayZInfected.Cast(unit);
				if (infected && infected.GetInputController())
					infected.GetInputController().OverrideMovementSpeed(true, 0.75);
			}
		}
	}

	protected void SpawnGuardian(vector center)
	{
		SpawnInfected(center, "DZECZ_EclipseGuardian", 1, 18.0, 28.0, true);
	}

	protected vector RandomPoint(vector center, float minDistance, float maxDistance)
	{
		float angle = Math.RandomFloat(0.0, Math.PI2);
		float distance = Math.RandomFloat(minDistance, maxDistance);
		vector position = Vector(center[0] + Math.Sin(angle) * distance, center[1], center[2] + Math.Cos(angle) * distance);
		return DZECZ_Utils.Grounded(position);
	}

	protected int CountAliveThreats()
	{
		int alive = 0;
		foreach (EntityAI threat: m_Threats)
		{
			if (threat && threat.IsAlive() && threat.GetHealth("", "Health") > 0.0)
				alive++;
		}
		return alive;
	}

	protected void CheckRiskyEnding()
	{
		if (!m_RiskyEndingPending || m_Progress.State != DZECZ_FINAL_DEFENSE_ACTIVE)
			return;
		if (CountAliveThreats() > 0)
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CheckRiskyEnding, 5000, false);
			return;
		}
		m_RiskyEndingPending = false;
		CompleteReward(true);
	}

	protected void CompleteReward(bool risky)
	{
		if (m_Progress.RewardSpawned)
			return;

		m_Progress.RewardSpawned = true;
		SetState(DZECZ_REWARD_UNLOCKED);
		vector cratePosition = DZECZ_Utils.Grounded(m_FinalPosition + "3 0 3");
		EntityAI crate = EntityAI.Cast(SpawnManagedObject("DZECZ_EclipseRewardCrate", cratePosition));
		if (crate)
			FillRewardCrate(crate, risky);
		CleanupThreats();
		DZECZ_Utils.RemoveGlobalSector();
		SetState(DZECZ_COMPLETED);
		DZECZ_Utils.Broadcast("Operation EclipseZ abgeschlossen. Die Sicherheitskiste bleibt 15 Minuten bestehen.", 15.0);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CleanupEvent, Math.Round(m_Config.CleanupDelaySeconds * 1000.0), false);
	}

	protected void FillRewardCrate(EntityAI crate, bool risky)
	{
		if (!crate || !crate.GetInventory())
			return;

		CreateCargo(crate, "DZECZ_EclipseToken");
		ref array<string> weapons = {"M4A1", "AKM", "SVD", "FAL", "VSS"};
		ref array<string> armor = {"PlateCarrierVest", "GhillieSuit_Tan", "BallisticHelmet_Black", "NVGoggles"};
		ref array<string> survival = {"NBCJacketGray", "NBCPantsGray", "NBCBootsGray", "GasMask_Filter", "EpoxyPutty", "ElectronicRepairKit", "LeatherSewingKit", "SalineBagIV"};
		ref array<string> building = {"NailBox", "MetalPlate", "BarbedWire", "CombinationLock4", "CamoNet"};

		CreateRandomCargo(crate, weapons);
		CreateRandomCargo(crate, armor);
		for (int i = 0; i < 4; i++)
			CreateRandomCargo(crate, survival);
		for (int j = 0; j < 3; j++)
			CreateRandomCargo(crate, building);
		CreateCargo(crate, "Ammo_556x45");
		CreateCargo(crate, "Ammo_762x39");
		CreateCargo(crate, "Ammo_762x54");

		if (risky)
		{
			CreateRandomCargo(crate, weapons);
			CreateRandomCargo(crate, armor);
			CreateCargo(crate, "Ammo_308Win");
			CreateCargo(crate, "Plastic_Explosive");
			CreateCargo(crate, "RemoteDetonator");
		}
	}

	protected void CreateRandomCargo(EntityAI crate, array<string> pool)
	{
		if (!pool || pool.Count() == 0)
			return;
		CreateCargo(crate, pool[Math.RandomInt(0, pool.Count())]);
	}

	protected void CreateCargo(EntityAI crate, string className)
	{
		if (!crate || !crate.GetInventory() || className == "")
			return;
		EntityAI item = crate.GetInventory().CreateInInventory(className);
		if (!item)
			DZECZ_Utils.Warn("Reward item could not be created: " + className);
	}

	void CleanupEvent()
	{
		ClearActiveAction();
		CleanupThreats();
		DZECZ_Utils.RemoveGlobalSector();
		foreach (Object object: m_SpawnedObjects)
		{
			if (object)
				GetGame().ObjectDelete(object);
		}
		m_SpawnedObjects.Clear();
		m_ActiveObject = null;
		m_FinalTerminal = null;
		m_FinalPosition = "0 0 0";
		m_RiskyEndingPending = false;
		m_ThreatPhaseKey = -1;
		m_Progress.Reset();
		SaveProgress();
		DZECZ_Utils.Log("Event cleanup complete; receiver remains ready.");
	}

	protected void CleanupThreats()
	{
		foreach (EntityAI threat: m_Threats)
		{
			if (threat)
				GetGame().ObjectDelete(threat);
		}
		m_Threats.Clear();
	}

	protected Object SpawnManagedObject(string className, vector position)
	{
		Object object = GetGame().CreateObjectEx(className, DZECZ_Utils.Grounded(position), ECE_PLACE_ON_SURFACE);
		if (object)
			m_SpawnedObjects.Insert(object);
		else
			DZECZ_Utils.Warn("Object spawn failed: " + className);
		return object;
	}

	protected void DeleteObject(Object object)
	{
		if (object && GetGame())
			GetGame().ObjectDelete(object);
	}

	protected int GetStationId(Object target)
	{
		if (DZECZ_TisyStation.Cast(target))
			return 1;
		if (DZECZ_GasZoneStation.Cast(target))
			return 2;
		return 0;
	}

	protected void SetStationActiveState(int index)
	{
		if (index == 0)
			SetState(DZECZ_STATION_ALPHA_ACTIVE);
		else if (index == 1)
			SetState(DZECZ_STATION_BRAVO_ACTIVE);
		else
			SetState(DZECZ_STATION_CHARLIE_ACTIVE);
	}

	protected void SetStationCompleteState(int index)
	{
		if (index == 0)
			SetState(DZECZ_STATION_ALPHA_COMPLETE);
		else if (index == 1)
			SetState(DZECZ_STATION_BRAVO_COMPLETE);
		else
			SetState(DZECZ_STATION_CHARLIE_COMPLETE);
	}

	protected void SetState(int state)
	{
		m_Progress.State = state;
		SaveProgress();
		DZECZ_Utils.Log("State -> " + state.ToString());
	}

	protected void SaveProgress()
	{
		if (!m_Progress)
			return;
		MakeDirectory(DZECZ_PROFILE_DIR);
		string errorMessage;
		if (!JsonFileLoader<ref DZECZ_Progress>.SaveFile(DZECZ_PROGRESS_PATH, m_Progress, errorMessage))
			DZECZ_Utils.Warn("Progress save failed: " + errorMessage);
	}

	protected void LoadProgress()
	{
		m_Progress = new DZECZ_Progress;
		if (!FileExist(DZECZ_PROGRESS_PATH))
			return;
		string errorMessage;
		if (!JsonFileLoader<ref DZECZ_Progress>.LoadFile(DZECZ_PROGRESS_PATH, m_Progress, errorMessage))
		{
			DZECZ_Utils.Warn("Progress load failed: " + errorMessage);
			m_Progress = new DZECZ_Progress;
		}
		if (!m_Progress.StationOrder)
			m_Progress.StationOrder = new array<int>;
	}

	protected void RestoreProgress()
	{
		if (!m_Progress || m_Progress.State == DZECZ_INACTIVE)
			return;

		if (m_Progress.State == DZECZ_COMPLETED || m_Progress.State == DZECZ_REWARD_UNLOCKED || m_Progress.State == DZECZ_CLEANUP || m_Progress.State == DZECZ_FAILED)
		{
			CleanupEvent();
			return;
		}

		if (m_Progress.State >= DZECZ_STATION_ALPHA_ACTIVE && m_Progress.State <= DZECZ_STATION_CHARLIE_COMPLETE)
		{
			SpawnCurrentStation();
			return;
		}

		if (m_Progress.State >= DZECZ_ALL_FRAGMENTS_COMPLETE && m_Progress.State <= DZECZ_FIREWORK_COUNTDOWN)
		{
			SpawnFireworkBattery();
			if (m_Progress.BatteryStep >= 3)
			{
				SetState(DZECZ_FIREWORK_COUNTDOWN);
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(FinishFireworkCountdown, 5000, false);
			}
			return;
		}

		if (m_Progress.FinalLocationIndex >= 0 && m_Config.FinalPositions && m_Progress.FinalLocationIndex < m_Config.FinalPositions.Count())
			m_FinalPosition = DZECZ_Utils.Grounded(DZECZ_Config.ToVector(m_Config.FinalPositions[m_Progress.FinalLocationIndex]));
		else
			SelectFinalLocation();
		SpawnFinalTerminal();
	}

	protected PlayerBase FindPlayer(string uid)
	{
		if (uid == "")
			return null;
		ref array<PlayerBase> players = DZBBC_PlayerUtils.GetOnlinePlayers();
		foreach (PlayerBase player: players)
		{
			if (DZECZ_Utils.GetUID(player) == uid)
				return player;
		}
		return null;
	}
}
