class DZCourierZ_Manager
{
	static ref DZCourierZ_Manager s_Instance;
	protected ref DZCourierZ_Settings m_Settings;
	protected ref DZCourierZ_Services m_Service;
	protected DZCourierZ_State m_State = DZCourierZ_State.IDLE;
	protected EntityAI m_Case;
	protected EntityAI m_Keys;
	protected EntityAI m_CaseNPC;
	protected EntityAI m_KeyNPC;
	protected EntityAI m_FinalNPC;
	protected PlayerBase m_Carrier;
	protected PlayerBase m_RouteMarkerOwner;
	protected int m_NextStart;
	protected int m_LastTrack;
	protected int m_NextMarkerRetry;
	protected int m_StageDeadline;
	protected bool m_NPCSpawned;
	protected bool m_TargetMarkerReady;
	protected bool m_ProximityCuePlayed;
	protected bool m_FinalPromptSent;
	protected bool m_RewardCommitted;
	protected vector m_RouteMarkerPosition;
	protected DZCourierZ_State m_RouteMarkerState = DZCourierZ_State.IDLE;

	static DZCourierZ_Manager Get() { if (!s_Instance) s_Instance = new DZCourierZ_Manager; return s_Instance; }

	void DZCourierZ_Manager()
	{
		m_Settings = new DZCourierZ_Settings;
		m_Service = new DZCourierZ_Services;
		m_NextStart = GetGame().GetTime() + m_Settings.InitialDelaySeconds * 1000;
	}

	void Start()
	{
		CleanupOnlineMissionItems();
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Tick, 2000, true);
		DZCourierZ_Log.Info("Manager initialisiert.");
	}

	protected void Tick()
	{
		if (!GetGame().IsServer() || !m_Settings.Enabled) return;
		if (IsActiveState() && GetGame().GetTime() >= m_StageDeadline)
		{
			AbortEvent("Der geheime Transportauftrag ist abgelaufen. Koffer und Schluessel wurden eingezogen.");
			return;
		}
		TryEnsureTargetMarker();
		if (m_State == DZCourierZ_State.IDLE && GetGame().GetTime() >= m_NextStart) BeginEvent();
		else if (m_State == DZCourierZ_State.CASE_PICKUP) TickCase();
		else if (m_State == DZCourierZ_State.KEY_PICKUP) TickKeys();
		else if (m_State == DZCourierZ_State.AI_STOP_1) TickAIStop1();
		else if (m_State == DZCourierZ_State.AI_STOP_2) TickAIStop2();
		else if (m_State == DZCourierZ_State.INFECTED_STOP) TickInfectedStop();
		else if (m_State == DZCourierZ_State.FINAL_DELIVERY) TickFinal();
		else if (m_State == DZCourierZ_State.COOLDOWN && GetGame().GetTime() >= m_NextStart) { m_State = DZCourierZ_State.IDLE; BeginEvent(); }
		UpdateRouteMarker();
		TrackCarrier();
	}

	protected void BeginEvent()
	{
		m_Service.Cleanup();
		m_Case = null; m_Keys = null; m_Carrier = null; m_CaseNPC = null; m_KeyNPC = null; m_FinalNPC = null; m_NPCSpawned = false;
		m_FinalPromptSent = false; m_RewardCommitted = false;
		m_RouteMarkerPosition = "0 0 0"; m_RouteMarkerState = DZCourierZ_State.IDLE;
		SetActiveState(DZCourierZ_State.CASE_PICKUP);
		m_TargetMarkerReady = false;
		TryEnsureTargetMarker();
		m_Service.NotifyAll("Geheimer Transportauftrag! Der Erste am Ziel erhaelt eine SPEZIELLE Aufgabe!");
		DZCourierZ_Log.Info("CASE_PICKUP gestartet.");
	}

	protected void TickCase()
	{
		PlayerBase player = ClosestPlayer(m_Settings.CasePosition, m_Settings.ActivationRadius);
		if (!player) return;
		if (!m_CaseNPC)
		{
			m_CaseNPC = m_Service.SpawnPassiveNPC(m_Settings.CasePosition);
			if (m_CaseNPC)
				m_Service.Notify(player, "Die Kontaktperson wartet am Ziel. Sprich sie mit F an.");
		}
		if (m_CaseNPC && !m_ProximityCuePlayed && vector.Distance(player.GetPosition(), m_CaseNPC.GetPosition()) <= DZCourierZ_Constants.VOICE_RADIUS)
		{
			m_ProximityCuePlayed = true;
			m_Service.PlaySound(player, "DZCourierZ_Start_SoundSet");
		}
	}

	protected bool GiveCase(PlayerBase player)
	{
		m_Case = player.GetInventory().CreateInInventory("CourierZ_ScientificCase");
		if (!m_Case)
		{
			m_Service.Notify(player, "Uebergabe nicht moeglich. Schaffe Platz im Inventar und versuche es erneut.");
			return false;
		}
		CourierZ_ScientificCase initialCase = CourierZ_ScientificCase.Cast(m_Case);
		ItemBase initialMoney;
		if (initialCase)
		{
			initialCase.Open();
			initialCase.DZCourierSetInitialFill(true);
			initialMoney = ItemBase.Cast(initialCase.GetInventory().CreateInInventory(m_Settings.RewardCurrencyClass));
			initialCase.DZCourierSetInitialFill(false);
		}
		if (!initialMoney)
		{
			DZCourierZ_Log.Warn("COURIER_CASE_FILL_FAILED money=" + m_Settings.RewardCurrencyClass + " entityCount=0 quantity=0 maxQuantity=0 reason=create_in_inventory_failed");
			GetGame().ObjectDelete(m_Case);
			m_Case = null;
			m_Service.Notify(player, "Der Kofferinhalt konnte nicht vorbereitet werden. Versuche es erneut.");
			return false;
		}
		initialMoney.SetQuantity(m_Settings.InitialCaseBanknotes);
		array<EntityAI> caseItems = new array<EntityAI>;
		initialCase.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, caseItems);
		int moneyEntityCount;
		foreach (EntityAI caseItem : caseItems)
		{
			if (caseItem && caseItem.GetType() == m_Settings.RewardCurrencyClass)
				moneyEntityCount++;
		}
		float actualQuantity = initialMoney.GetQuantity();
		float maxQuantity = initialMoney.GetQuantityMax();
		if (moneyEntityCount != 1 || actualQuantity != m_Settings.InitialCaseBanknotes)
		{
			DZCourierZ_Log.Warn("COURIER_CASE_FILL_FAILED money=" + m_Settings.RewardCurrencyClass + " entityCount=" + moneyEntityCount.ToString() + " quantity=" + actualQuantity.ToString() + " maxQuantity=" + maxQuantity.ToString() + " reason=validation_failed");
			GetGame().ObjectDelete(m_Case);
			m_Case = null;
			m_Service.Notify(player, "Der Kofferinhalt konnte nicht vollstaendig vorbereitet werden. Versuche es erneut.");
			return false;
		}
		DZCourierZ_Log.Info("COURIER_CASE_FILL_OK money=" + m_Settings.RewardCurrencyClass + " entityCount=1 quantity=" + actualQuantity.ToString() + " unitValue=100 totalValue=250000");

		int ambushCount = m_Service.SpawnHostileAI(Midpoint(m_Settings.CasePosition, m_Settings.KeyPosition), 2, true);
		m_KeyNPC = m_Service.SpawnPassiveNPC(m_Settings.KeyPosition);
		if (ambushCount != 2 || !m_KeyNPC)
		{
			AbortEvent("Der Auftrag konnte nicht vollstaendig vorbereitet werden. Er wird spaeter neu gestartet.");
			return false;
		}

		m_Carrier = player;
		m_Service.RemoveMarker("DZ_COURIERZ_TARGET");
		SetActiveState(DZCourierZ_State.KEY_PICKUP);
		m_TargetMarkerReady = false;
		TryEnsureTargetMarker();
		m_Service.Notify(player, "Verschlossener wissenschaftlicher Koffer erhalten. Besorge jetzt die Schluessel.");
		string handoverPlayer = "unknown";
		if (player.GetIdentity()) handoverPlayer = player.GetIdentity().GetPlainId();
		DZCourierZ_Log.Info("COURIER_START_HANDOVER_OK player=" + handoverPlayer + " case=CourierZ_ScientificCase");
		DZCourierZ_Log.Info("Koffer an " + handoverPlayer + " uebergeben; 2-Mann-Hinterhalt aktiv.");
		return true;
	}

	protected void TickKeys()
	{
		ResolveCarrier();
		if (m_Carrier && m_KeyNPC && !m_ProximityCuePlayed && vector.Distance(m_Carrier.GetPosition(), m_KeyNPC.GetPosition()) <= DZCourierZ_Constants.VOICE_RADIUS)
		{
			m_ProximityCuePlayed = true;
			m_Service.PlaySound(m_Carrier, "DZCourierZ_Keys_SoundSet");
		}
	}

	protected bool GiveKeys(PlayerBase player)
	{
		if (!m_Service.EnemiesDead())
		{
			m_Service.Notify(player, "Der Hinterhalt ist noch aktiv. Schalte beide Gegner aus.");
			return false;
		}
		m_Keys = m_Carrier.GetInventory().CreateInInventory("CourierZ_ScientificCaseKeys");
		if (!m_Keys)
		{
			m_Service.Notify(player, "Uebergabe nicht moeglich. Schaffe Platz im Inventar und versuche es erneut.");
			return false;
		}
		m_Service.RemoveMarker("DZ_COURIERZ_TARGET");
		SetActiveState(DZCourierZ_State.AI_STOP_1);
		m_TargetMarkerReady = false;
		TryEnsureTargetMarker();
		m_Service.NotifyAll("Eine geheime Fracht wird transportiert. Der Koffertraeger wird getrackt!");
		m_Service.Notify(m_Carrier, "Schluessel erhalten. Erreiche den ersten feindlichen Kontrollpunkt.");
		m_NPCSpawned = false;
		return true;
	}

	protected void TickAIStop1()
	{
		ResolveCarrier(); if (!m_Carrier) return;
		if (!m_NPCSpawned && vector.Distance(m_Carrier.GetPosition(), m_Settings.AIStop1Position) <= m_Settings.ActivationRadius)
		{
			if (m_Service.SpawnHostileAI(m_Settings.AIStop1Position, 3) != 3)
			{
				AbortEvent("Der erste feindliche Kontrollpunkt konnte nicht vorbereitet werden. Der Auftrag wurde beendet.");
				return;
			}
			m_NPCSpawned = true;
			DZCourierZ_Log.Info("Feindlicher Kontrollpunkt I: 3 Gegner gespawnt.");
		}
		if (!m_NPCSpawned || vector.Distance(m_Carrier.GetPosition(), m_Settings.AIStop1Position) > m_Settings.HandoverRadius || !m_Service.EnemiesDead()) return;
		m_Service.RemoveMarker("DZ_COURIERZ_TARGET");
		SetActiveState(DZCourierZ_State.AI_STOP_2);
		m_TargetMarkerReady = false;
		TryEnsureTargetMarker();
		m_Service.Notify(m_Carrier, "Erster Kontrollpunkt gesichert. Der zweite ist markiert.");
		m_NPCSpawned = false;
	}

	protected void TickAIStop2()
	{
		ResolveCarrier(); if (!m_Carrier) return;
		if (!m_NPCSpawned && vector.Distance(m_Carrier.GetPosition(), m_Settings.AIStop2Position) <= m_Settings.ActivationRadius)
		{
			if (m_Service.SpawnHostileAI(m_Settings.AIStop2Position, 3) != 3)
			{
				AbortEvent("Der zweite feindliche Kontrollpunkt konnte nicht vorbereitet werden. Der Auftrag wurde beendet.");
				return;
			}
			m_NPCSpawned = true;
			DZCourierZ_Log.Info("Feindlicher Kontrollpunkt II: 3 Gegner gespawnt.");
		}
		if (!m_NPCSpawned || vector.Distance(m_Carrier.GetPosition(), m_Settings.AIStop2Position) > m_Settings.HandoverRadius || !m_Service.EnemiesDead()) return;
		m_Service.RemoveMarker("DZ_COURIERZ_TARGET");
		SetActiveState(DZCourierZ_State.INFECTED_STOP);
		m_TargetMarkerReady = false;
		TryEnsureTargetMarker();
		m_Service.Notify(m_Carrier, "Zweiter Kontrollpunkt gesichert. Das naechste Ziel ist markiert.");
		m_NPCSpawned = false;
	}

	protected void TickInfectedStop()
	{
		ResolveCarrier(); if (!m_Carrier) return;
		if (!m_NPCSpawned && vector.Distance(m_Carrier.GetPosition(), m_Settings.InfectedStopPosition) <= m_Settings.ActivationRadius)
		{
			if (m_Service.SpawnInfected(m_Settings.InfectedStopPosition, m_Settings.InfectedCount) != m_Settings.InfectedCount)
			{
				AbortEvent("Die Infected-Horde konnte nicht vorbereitet werden. Der Auftrag wurde beendet.");
				return;
			}
			m_NPCSpawned = true;
		}
		if (!m_NPCSpawned || !m_Service.EnemiesDead()) return;
		DZCourierZ_Log.Info("Infected-Zwischenstopp abgeschlossen; alle Event-Infected sind tot.");
		m_Service.RemoveMarker("DZ_COURIERZ_TARGET");
		SetActiveState(DZCourierZ_State.FINAL_DELIVERY);
		m_TargetMarkerReady = false;
		TryEnsureTargetMarker();
		m_FinalNPC = m_Service.SpawnPassiveNPC(TerrainPosition(m_Settings.FinalPosition));
		m_Service.Notify(m_Carrier, "Die Horde ist beseitigt. Bringe die Fracht zur finalen Uebergabe.");
	}

	protected void TickFinal()
	{
		ResolveCarrier();
		if (!m_FinalNPC)
			m_FinalNPC = m_Service.SpawnPassiveNPC(TerrainPosition(m_Settings.FinalPosition));
		if (m_Carrier && m_FinalNPC && !m_FinalPromptSent && vector.Distance(m_Carrier.GetPosition(), m_FinalNPC.GetPosition()) <= DZCourierZ_Constants.VOICE_RADIUS)
		{
			m_FinalPromptSent = true;
			m_Service.PlaySound(m_Carrier, "DZCourierZ_Final_SoundSet");
			m_Service.OpenRewardMenu(m_Carrier);
			DZCourierZ_Log.Info("Finale Belohnungsauswahl fuer Koffertraeger geoeffnet.");
		}
	}

	protected EntityAI CreateRewardItem(PlayerBase player, string className)
	{
		EntityAI item = player.GetInventory().CreateInInventory(className);
		if (!item)
			item = EntityAI.Cast(GetGame().CreateObjectEx(className, player.GetPosition(), ECE_PLACE_ON_SURFACE));
		return item;
	}

	protected bool FillRewardCaseWithBanknotes(EntityAI rewardCase, int banknoteCount)
	{
		ScientificBriefcase moneyCase = ScientificBriefcase.Cast(rewardCase);
		if (!moneyCase || banknoteCount <= 0) return false;
		moneyCase.Open();
		int remaining = banknoteCount;
		while (remaining > 0)
		{
			int stackAmount = Math.Min(2500, remaining);
			ItemBase money = ItemBase.Cast(rewardCase.GetInventory().CreateInInventory(m_Settings.RewardCurrencyClass));
			if (!money) return false;
			money.SetQuantity(stackAmount);
			if (money.GetQuantity() != stackAmount) return false;
			remaining -= stackAmount;
		}
		return true;
	}

	protected bool GiveMoneyReward(PlayerBase player)
	{
		EntityAI rewardCase = CreateRewardItem(player, "CourierZ_ScientificCaseReward");
		if (!rewardCase) return false;
		if (!FillRewardCaseWithBanknotes(rewardCase, m_Settings.CashRewardBanknotes))
		{
			GetGame().ObjectDelete(rewardCase);
			return false;
		}
		DZCourierZ_Log.Info("Cash-Belohnung ausgegeben: " + m_Settings.CashRewardBanknotes.ToString() + " ExpansionBanknoteEuro.");
		return true;
	}

	void HandleRewardSelection(PlayerBase player, PlayerIdentity sender, int choice)
	{
		if (!GetGame().IsServer() || !player || !sender || player.GetIdentity() != sender) return;
		if (m_State != DZCourierZ_State.FINAL_DELIVERY || player != m_Carrier || m_RewardCommitted) return;
		if (!m_FinalNPC || vector.Distance(player.GetPosition(), m_FinalNPC.GetPosition()) > 30.0) return;
		if (!m_Case || !m_Keys || m_Case.GetHierarchyRootPlayer() != player || m_Keys.GetHierarchyRootPlayer() != player) return;
		if (choice < DZCourierZ_RewardChoice.KEEP_CASE || choice > DZCourierZ_RewardChoice.SECRET_HINT) return;

		m_RewardCommitted = true;
		bool rewardSuccess = false;
		string rewardText;
		if (choice == DZCourierZ_RewardChoice.KEEP_CASE)
		{
			EntityAI rewardCase = CreateRewardItem(player, "CourierZ_ScientificCaseReward");
			EntityAI rewardKeys = CreateRewardItem(player, "CourierZ_ScientificCaseKeysReward");
			rewardSuccess = rewardCase && rewardKeys && FillRewardCaseWithBanknotes(rewardCase, m_Settings.InitialCaseBanknotes);
			if (!rewardSuccess)
			{
				if (rewardCase) GetGame().ObjectDelete(rewardCase);
				if (rewardKeys) GetGame().ObjectDelete(rewardKeys);
			}
			rewardText = "Du behaeltst den wissenschaftlichen Koffer und seine Schluessel.";
		}
		else if (choice == DZCourierZ_RewardChoice.SECRET_ITEM)
		{
			rewardSuccess = m_Service.SpawnRewardTruck(m_Settings.SecretTruckClass, m_Settings.SecretTruckPosition, m_Settings.SecretTruckOrientation) != null;
			rewardText = "Der geheime Gegenstand wartet an der Kueste auf dich.";
		}
		else if (choice == DZCourierZ_RewardChoice.MONEY)
		{
			rewardSuccess = GiveMoneyReward(player);
			rewardText = "1.250.000 Cash wurden ausgezahlt.";
		}
		else
		{
			rewardSuccess = true;
			rewardText = m_Settings.SecretHint;
		}

		if (!rewardSuccess)
		{
			m_RewardCommitted = false;
			m_Service.Notify(player, "Die Belohnung konnte nicht ausgegeben werden. Schaffe Platz und waehle erneut.");
			m_Service.OpenRewardMenu(player);
			return;
		}

		GetGame().ObjectDelete(m_Case); GetGame().ObjectDelete(m_Keys); m_Case = null; m_Keys = null;
		m_Service.Notify(player, rewardText);
		m_Service.PlaySound(player, "DZCourierZ_Idiot_SoundSet");
		m_Service.NotifyAll("Der geheime Transportauftrag wurde abgeschlossen.");
		DZCourierZ_Log.Info("FINAL_DELIVERY abgeschlossen; Belohnungswahl=" + choice.ToString() + ".");
		RecordRadioMissionCompletion(player);
		EndEvent();
	}

	protected void RecordRadioMissionCompletion(PlayerBase player)
	{
		if(!player||!player.GetIdentity())return; string root="$profile:DeutschZ-System/deutschz_radiomissionz/event_completions"; string dir=root+"/courier";
		MakeDirectory("$profile:DeutschZ-System"); MakeDirectory("$profile:DeutschZ-System/deutschz_radiomissionz"); MakeDirectory(root); MakeDirectory(dir);
		FileHandle file=OpenFile(dir+"/"+player.GetIdentity().GetPlainId()+".done",FileMode.WRITE); if(file!=0){FPrintln(file,"courier");CloseFile(file);}
	}

	bool HandleCourierInteraction(PlayerBase player, Object target)
	{
		if (!GetGame().IsServer() || !player || !target || !player.IsAlive()) return false;
		if (vector.Distance(player.GetPosition(), target.GetPosition()) > m_Settings.HandoverRadius) return false;

		if (m_State == DZCourierZ_State.CASE_PICKUP && target == m_CaseNPC && !m_Case)
			return GiveCase(player);

		ResolveCarrier();
		if (player != m_Carrier || !m_Case) return false;

		if (m_State == DZCourierZ_State.KEY_PICKUP && target == m_KeyNPC && !m_Keys)
			return GiveKeys(player);

		if (m_State == DZCourierZ_State.FINAL_DELIVERY && target == m_FinalNPC && m_Keys)
		{
			if (m_Case.GetHierarchyRootPlayer() != player || m_Keys.GetHierarchyRootPlayer() != player)
			{
				m_Service.Notify(player, "Fuer die Uebergabe musst du Koffer und Schluessel bei dir tragen.");
				return false;
			}
			m_Service.OpenRewardMenu(player);
			return true;
		}

		return false;
	}

	protected void TrackCarrier()
	{
		if (m_State < DZCourierZ_State.AI_STOP_1 || m_State > DZCourierZ_State.FINAL_DELIVERY) return;
		ResolveCarrier(); if (!m_Case) return;
		if (GetGame().GetTime() - m_LastTrack < m_Settings.TrackUpdateSeconds * 1000) return;
		m_LastTrack = GetGame().GetTime();
		vector casePosition = m_Case.GetPosition();
		m_Service.Marker("DZ_COURIERZ_CARRIER", "Geheime Fracht", casePosition, true);
		m_Service.NotifyAll("Position der geheimen Fracht aktualisiert: " + casePosition.ToString());
	}

	protected void ResolveCarrier()
	{
		if (!m_Case) { m_Carrier = null; return; }
		m_Carrier = PlayerBase.Cast(m_Case.GetHierarchyRootPlayer());
	}

	protected PlayerBase ClosestPlayer(vector position, float radius)
	{
		array<Man> players = new array<Man>; GetGame().GetPlayers(players);
		PlayerBase closest; float best = radius;
		foreach (Man man: players)
		{
			PlayerBase player = PlayerBase.Cast(man); if (!player || !player.IsAlive()) continue;
			float distance = vector.Distance(player.GetPosition(), position);
			if (distance <= best) { best = distance; closest = player; }
		}
		return closest;
	}

	protected vector Midpoint(vector a, vector b) { return Vector((a[0] + b[0]) * 0.5, (a[1] + b[1]) * 0.5, (a[2] + b[2]) * 0.5); }

	protected vector TerrainPosition(vector position)
	{
		return Vector(position[0], GetGame().SurfaceY(position[0], position[2]) + 0.5, position[2]);
	}

	protected vector CurrentRouteTarget()
	{
		if (m_State == DZCourierZ_State.KEY_PICKUP) return m_Settings.KeyPosition;
		if (m_State == DZCourierZ_State.AI_STOP_1) return m_Settings.AIStop1Position;
		if (m_State == DZCourierZ_State.AI_STOP_2) return m_Settings.AIStop2Position;
		if (m_State == DZCourierZ_State.INFECTED_STOP) return TerrainPosition(m_Settings.InfectedStopPosition);
		if (m_State == DZCourierZ_State.FINAL_DELIVERY)
		{
			if (m_FinalNPC) return m_FinalNPC.GetPosition();
			return TerrainPosition(m_Settings.FinalPosition);
		}
		return "0 0 0";
	}

	protected void UpdateRouteMarker()
	{
		ResolveCarrier();
		if (m_RouteMarkerOwner != m_Carrier)
		{
			if (m_RouteMarkerOwner) m_Service.ClearRouteMarker(m_RouteMarkerOwner);
			m_RouteMarkerOwner = m_Carrier;
			m_RouteMarkerPosition = "0 0 0";
		}
		if (!m_Carrier || m_State < DZCourierZ_State.KEY_PICKUP || m_State > DZCourierZ_State.FINAL_DELIVERY) return;
		vector target = CurrentRouteTarget();
		if (target == "0 0 0") return;
		if (m_RouteMarkerState != m_State)
		{
			m_RouteMarkerState = m_State;
			m_RouteMarkerPosition = "0 0 0";
		}
		vector groundTarget = TerrainPosition(target);
		if (m_RouteMarkerPosition == groundTarget) return;
		if (m_RouteMarkerPosition != "0 0 0" && HorizontalDistance(m_Carrier.GetPosition(), m_RouteMarkerPosition) > m_Settings.RouteMarkerAdvanceRadius) return;

		vector start = m_Carrier.GetPosition();
		if (m_RouteMarkerPosition != "0 0 0") start = m_RouteMarkerPosition;
		float remaining = HorizontalDistance(start, target);
		if (remaining <= m_Settings.RouteMarkerSpacing)
			m_RouteMarkerPosition = TerrainPosition(target);
		else
		{
			float step = m_Settings.RouteMarkerSpacing / remaining;
			m_RouteMarkerPosition = TerrainPosition(Vector(start[0] + (target[0] - start[0]) * step, 0, start[2] + (target[2] - start[2]) * step));
		}
		m_Service.SetRouteMarker(m_Carrier, "CourierZ - Zwischenziel", m_RouteMarkerPosition);
		DZCourierZ_Log.Info("Persoenlicher Zwischenmarker aktualisiert: position=" + m_RouteMarkerPosition.ToString());
	}

	protected float HorizontalDistance(vector a, vector b)
	{
		return vector.Distance(Vector(a[0], 0, a[2]), Vector(b[0], 0, b[2]));
	}

	protected void TryEnsureTargetMarker()
	{
		if (m_TargetMarkerReady || m_State == DZCourierZ_State.IDLE || m_State == DZCourierZ_State.COOLDOWN) return;
		if (GetGame().GetTime() < m_NextMarkerRetry) return;
		m_NextMarkerRetry = GetGame().GetTime() + 10000;

		if (m_State == DZCourierZ_State.CASE_PICKUP)
			m_TargetMarkerReady = m_Service.Marker("DZ_COURIERZ_TARGET", "Geheimer Transportauftrag", m_Settings.CasePosition, false);
		else if (m_State == DZCourierZ_State.KEY_PICKUP)
			m_TargetMarkerReady = m_Service.Marker("DZ_COURIERZ_TARGET", "Schluessel besorgen", m_Settings.KeyPosition, false);
		else if (m_State == DZCourierZ_State.AI_STOP_1)
			m_TargetMarkerReady = m_Service.Marker("DZ_COURIERZ_TARGET", "Feindlicher Kontrollpunkt I", m_Settings.AIStop1Position, true);
		else if (m_State == DZCourierZ_State.AI_STOP_2)
			m_TargetMarkerReady = m_Service.Marker("DZ_COURIERZ_TARGET", "Feindlicher Kontrollpunkt II", m_Settings.AIStop2Position, true);
		else if (m_State == DZCourierZ_State.INFECTED_STOP)
			m_TargetMarkerReady = m_Service.Marker("DZ_COURIERZ_TARGET", "Infected-Zwischenstopp", TerrainPosition(m_Settings.InfectedStopPosition), true);
		else if (m_State == DZCourierZ_State.FINAL_DELIVERY)
		{
			vector finalMarkerPosition = TerrainPosition(m_Settings.FinalPosition);
			if (m_FinalNPC) finalMarkerPosition = m_FinalNPC.GetPosition();
			m_TargetMarkerReady = m_Service.Marker("DZ_COURIERZ_TARGET", "Finale Uebergabe", finalMarkerPosition, true);
		}
	}

	protected void EndEvent()
	{
		if (m_RouteMarkerOwner) m_Service.ClearRouteMarker(m_RouteMarkerOwner);
		m_RouteMarkerOwner = null;
		m_Service.Cleanup(); m_Carrier = null; m_NPCSpawned = false;
		m_TargetMarkerReady = false;
		m_RouteMarkerPosition = "0 0 0"; m_RouteMarkerState = DZCourierZ_State.IDLE;
		m_State = DZCourierZ_State.COOLDOWN;
		m_NextStart = GetGame().GetTime() + m_Settings.CooldownSeconds * 1000;
	}

	protected bool IsActiveState()
	{
		return m_State >= DZCourierZ_State.CASE_PICKUP && m_State <= DZCourierZ_State.FINAL_DELIVERY;
	}

	protected void SetActiveState(DZCourierZ_State state)
	{
		m_State = state;
		m_StageDeadline = GetGame().GetTime() + m_Settings.StageTimeoutSeconds * 1000;
		m_ProximityCuePlayed = false;
	}

	void CleanupPlayerMissionItems(PlayerBase player)
	{
		if (!player) return;
		array<EntityAI> inventory = new array<EntityAI>;
		player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, inventory);
		foreach (EntityAI item: inventory)
		{
			if (!item || item == m_Case || item == m_Keys) continue;
			if (item.GetType() == "CourierZ_ScientificCase" || item.GetType() == "CourierZ_ScientificCaseKeys")
				GetGame().ObjectDelete(item);
		}
	}

	protected void CleanupOnlineMissionItems()
	{
		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		foreach (Man man: players) CleanupPlayerMissionItems(PlayerBase.Cast(man));
	}

	protected void AbortEvent(string message)
	{
		if (m_Case) GetGame().ObjectDelete(m_Case);
		if (m_Keys) GetGame().ObjectDelete(m_Keys);
		m_Case = null; m_Keys = null;
		m_Service.NotifyAll(message);
		DZCourierZ_Log.Warn(message);
		EndEvent();
	}
}
