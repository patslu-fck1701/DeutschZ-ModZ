class DZRMZ_MissionDefinition
{
	string Id;
	string Category;
	string Title;
	string ObjectiveType;
	string Description;
	string RequiredItemClass;
	string RequiredItemLabel;
	int RequiredQuantity;
	bool AllowRuinedDeliveryItems;
	float MinimumItemQuantity;
	int RequiredLiquidTypeMask;
	bool RequireStoredEnergy;
	int HoldSeconds;
	ref array<string> RadioLines;
	bool ChainStart;
	string FixedLocationId;
	string FixedDeliveryLocationId;
	string NextMissionId;
	string StartRequiredItemClass;
	bool RequiresListenConfirmation;
	int InfectedMinimum;
	int InfectedMaximum;
	string RewardCurrencyClass;
	int RewardCurrencyAmount;
	string ScenarioType;
	string RadioSoundSet;
	string NpcClass;
	bool SpawnProtectedNpc;
	int ThreatWaveCount;
	float ThreatSpawnMinimumRadius;
	float ThreatSpawnMaximumRadius;
	string AcceptText;
	string CompletionText;
	string FailureNpcDeadText;

	void DZRMZ_MissionDefinition()
	{
		Id = "";
		Category = DZRMZ_CATEGORY_CIVILIAN;
		Title = "Unbenannte Mission";
		ObjectiveType = DZRMZ_OBJECTIVE_VISIT;
		Description = "";
		RequiredItemClass = "";
		RequiredItemLabel = "";
		RequiredQuantity = 0;
		AllowRuinedDeliveryItems = false;
		MinimumItemQuantity = 0.0;
		RequiredLiquidTypeMask = LIQUID_NONE;
		RequireStoredEnergy = false;
		HoldSeconds = 0;
		RadioLines = new array<string>;
		ChainStart = false;
		FixedLocationId = "";
		FixedDeliveryLocationId = "";
		NextMissionId = "";
		StartRequiredItemClass = "";
		RequiresListenConfirmation = false;
		InfectedMinimum = 0;
		InfectedMaximum = 0;
		RewardCurrencyClass = "";
		RewardCurrencyAmount = 0;
		ScenarioType = DZRMZ_SCENARIO_TRAPPED;
		RadioSoundSet = "";
		NpcClass = "ExpansionNPCDenis";
		SpawnProtectedNpc = false;
		ThreatWaveCount = 1;
		ThreatSpawnMinimumRadius = 15.0;
		ThreatSpawnMaximumRadius = 34.0;
		AcceptText = "";
		CompletionText = "";
		FailureNpcDeadText = "Die zu schuetzende Person ist gestorben.";
	}

	bool IsValid()
	{
		if (Id == "" || Title == "" || !RadioLines || RadioLines.Count() == 0)
			return false;
		if (Category != DZRMZ_CATEGORY_CIVILIAN && Category != DZRMZ_CATEGORY_SCAVENGER && Category != DZRMZ_CATEGORY_MILITARY)
			return false;
		if (ObjectiveType != DZRMZ_OBJECTIVE_VISIT && ObjectiveType != DZRMZ_OBJECTIVE_HOLD && ObjectiveType != DZRMZ_OBJECTIVE_DELIVERY)
			return false;
		if (ObjectiveType == DZRMZ_OBJECTIVE_HOLD && HoldSeconds < 1)
			return false;
		if (ObjectiveType == DZRMZ_OBJECTIVE_DELIVERY && (RequiredItemClass == "" || RequiredQuantity < 1))
			return false;
		if (MinimumItemQuantity < 0.0 || RequiredLiquidTypeMask < LIQUID_NONE)
			return false;
		if (InfectedMinimum < 0 || InfectedMaximum < InfectedMinimum || InfectedMaximum > 30 || RewardCurrencyAmount < 0 || ThreatWaveCount < 1 || ThreatWaveCount > 3 || ThreatSpawnMinimumRadius < 0 || ThreatSpawnMaximumRadius < ThreatSpawnMinimumRadius)
			return false;
		return true;
	}
}

class DZRMZ_MissionCatalog
{
	int SchemaVersion;
	ref array<ref DZRMZ_MissionDefinition> Missions;

	void DZRMZ_MissionCatalog()
	{
		SchemaVersion = 5;
		Missions = new array<ref DZRMZ_MissionDefinition>;
	}

	bool IsValid()
	{
		if (!Missions || Missions.Count() == 0)
			return false;
		ref map<string, bool> missionIds = new map<string, bool>;
		foreach (DZRMZ_MissionDefinition definition : Missions)
		{
			if (!definition || !definition.IsValid())
				return false;
			if (missionIds.Contains(definition.Id))
				return false;
			missionIds.Insert(definition.Id, true);
		}
		return true;
	}
}

class DZRMZ_Location
{
	string Id;
	string Name;
	ref array<float> Position;
	ref array<string> Tags;

	void DZRMZ_Location()
	{
		Id = "";
		Name = "";
		Position = new array<float>;
		Tags = new array<string>;
	}

	bool IsValid()
	{
		return Id != "" && Name != "" && Position && Position.Count() == 3 && Tags && Tags.Count() > 0;
	}

	bool HasTag(string tag)
	{
		if (!Tags)
			return false;
		return Tags.Find(tag) > -1;
	}

	vector GetGroundedPosition()
	{
		if (!IsValid())
			return "0 0 0";

		float y = Position[1];
		if (GetGame())
			y = GetGame().SurfaceY(Position[0], Position[2]);
		return Vector(Position[0], y, Position[2]);
	}
}

class DZRMZ_LocationCatalog
{
	int SchemaVersion;
	string WorldName;
	ref array<ref DZRMZ_Location> Locations;

	void DZRMZ_LocationCatalog()
	{
		SchemaVersion = 1;
		WorldName = "chernarusplus";
		Locations = new array<ref DZRMZ_Location>;
	}

	bool IsValid()
	{
		if (WorldName == "" || !Locations || Locations.Count() < 2)
			return false;
		ref map<string, bool> locationIds = new map<string, bool>;
		foreach (DZRMZ_Location location : Locations)
		{
			if (!location || !location.IsValid())
				return false;
			if (locationIds.Contains(location.Id))
				return false;
			locationIds.Insert(location.Id, true);
		}
		return true;
	}
}

class DZRMZ_ActiveMissionRuntime
{
	int SchemaVersion;
	int State;
	string DefinitionId;
	string ObjectiveLocationId;
	string DeliveryLocationId;
	string OwnerUid;
	string OwnerName;
	string MissionInstanceId;
	float SecondsUntilNextMission;
	float MissionSecondsRemaining;
	float BroadcastSecondsRemaining;
	int BroadcastLineIndex;
	float HoldProgressSeconds;
	float OwnerMissingSeconds;
	bool TransmissionHeard;
	string TransmissionListenerUid;
	bool ThreatSpawned;
	bool ThreatCleared;
	int ThreatWaveIndex;
	float ThreatWavePauseSeconds;
	bool ObjectiveInteractionDone;
	float OwnerHintSecondsRemaining;
	string CompletionId;
	bool CompletionRecorded;
	bool RewardPayoutCommitted;
	bool RewardPayoutFinished;
	int RewardItemsGranted;
	int RewardReputationGranted;

	void DZRMZ_ActiveMissionRuntime()
	{
		Reset();
	}

	void Reset()
	{
		SchemaVersion = 2;
		State = DZRMZ_EMissionState.DZRMZ_STATE_IDLE;
		DefinitionId = "";
		ObjectiveLocationId = "";
		DeliveryLocationId = "";
		OwnerUid = "";
		OwnerName = "";
		MissionInstanceId = "";
		SecondsUntilNextMission = 0.0;
		MissionSecondsRemaining = 0.0;
		BroadcastSecondsRemaining = 0.0;
		BroadcastLineIndex = 0;
		HoldProgressSeconds = 0.0;
		OwnerMissingSeconds = 0.0;
		TransmissionHeard = false;
		TransmissionListenerUid = "";
		ThreatSpawned = false;
		ThreatCleared = false;
		ThreatWaveIndex = 0;
		ThreatWavePauseSeconds = 0.0;
		ObjectiveInteractionDone = false;
		OwnerHintSecondsRemaining = 0.0;
		CompletionId = "";
		CompletionRecorded = false;
		RewardPayoutCommitted = false;
		RewardPayoutFinished = false;
		RewardItemsGranted = 0;
		RewardReputationGranted = 0;
	}
}

class DZRMZ_HistoryEntry
{
	string MissionId;
	string MissionTitle;
	string Category;
	string Result;
	string OwnerUid;
	string OwnerName;
	string ObjectiveLocationId;
	string DeliveryLocationId;
	string CompletionId;
	bool RewardPayoutCommitted;
	bool RewardPayoutFinished;
	int RewardItemsGranted;
	int RewardReputationGranted;

	void DZRMZ_HistoryEntry()
	{
		MissionId = "";
		MissionTitle = "";
		Category = "";
		Result = "";
		OwnerUid = "";
		OwnerName = "";
		ObjectiveLocationId = "";
		DeliveryLocationId = "";
		CompletionId = "";
		RewardPayoutCommitted = false;
		RewardPayoutFinished = false;
		RewardItemsGranted = 0;
		RewardReputationGranted = 0;
	}
}

class DZRMZ_MissionHistory
{
	int SchemaVersion;
	int TotalStarted;
	int TotalCompleted;
	int TotalFailed;
	ref array<string> RecentMissionIds;
	ref array<string> RecentLocationIds;
	ref array<ref DZRMZ_HistoryEntry> Entries;

	void DZRMZ_MissionHistory()
	{
		SchemaVersion = 2;
		TotalStarted = 0;
		TotalCompleted = 0;
		TotalFailed = 0;
		RecentMissionIds = new array<string>;
		RecentLocationIds = new array<string>;
		Entries = new array<ref DZRMZ_HistoryEntry>;
	}

	void Validate()
	{
		if (SchemaVersion < 2)
			SchemaVersion = 2;
		if (!RecentMissionIds)
			RecentMissionIds = new array<string>;
		if (!RecentLocationIds)
			RecentLocationIds = new array<string>;
		if (!Entries)
			Entries = new array<ref DZRMZ_HistoryEntry>;
	}
}

class DZRMZ_DefaultFactory
{
	static int MigrateEventGrid(DZRMZ_MissionCatalog target)
	{
		if (!target || !target.Missions) return 0;
		if (target.SchemaVersion >= 5) return 0;
		int changed = 0;
		ref DZRMZ_MissionCatalog defaults = CreateMissionCatalog();
		foreach (DZRMZ_MissionDefinition candidate : defaults.Missions)
		{
			DZRMZ_MissionDefinition existing = null;
			foreach (DZRMZ_MissionDefinition current : target.Missions)
				if (current && current.Id == candidate.Id) { existing = current; break; }
			if (!existing) { target.Missions.Insert(candidate); changed++; continue; }

			// Nur neue Schema-5-Felder auffuellen; bestehende Admin-Anpassungen bleiben erhalten.
			if (existing.RadioSoundSet == "") existing.RadioSoundSet = candidate.RadioSoundSet;
			if (existing.AcceptText == "") existing.AcceptText = candidate.AcceptText;
			if (existing.CompletionText == "") existing.CompletionText = candidate.CompletionText;
			if (existing.FailureNpcDeadText == "") existing.FailureNpcDeadText = candidate.FailureNpcDeadText;
			if (existing.NpcClass == "") existing.NpcClass = candidate.NpcClass;
			if (existing.ThreatWaveCount < 1) existing.ThreatWaveCount = candidate.ThreatWaveCount;
			if (existing.ThreatSpawnMinimumRadius <= 0.0) existing.ThreatSpawnMinimumRadius = candidate.ThreatSpawnMinimumRadius;
			if (existing.ThreatSpawnMaximumRadius <= existing.ThreatSpawnMinimumRadius) existing.ThreatSpawnMaximumRadius = candidate.ThreatSpawnMaximumRadius;
			if (existing.Id == "SIDE-EINGESCHLOSSEN-01") { existing.ScenarioType = DZRMZ_SCENARIO_TRAPPED; existing.SpawnProtectedNpc = true; }
		}
		target.SchemaVersion = 5;
		return changed + 1;
	}
	protected static bool IsLiveIntegrationId(string id)
	{
		return id.IndexOf("LIVE-") == 0;
	}

	protected static bool IsObsoleteChainId(string id)
	{
		return id.IndexOf("CHAIN-") == 0;
	}

	static ref DZRMZ_MissionCatalog CreateMissionCatalog()
	{
		ref DZRMZ_MissionCatalog catalog = new DZRMZ_MissionCatalog;
		AddHelpMission(catalog, "SIDE-EINGESCHLOSSEN-01", "Eingeschlossen", DZRMZ_SCENARIO_TRAPPED, "", "ExpansionNPCDenis", 8, 15, 1, 15.0, 34.0, 70, 15000);
		SetHelpTexts(catalog, "Endlich! Du hast mich gehoert. Beeil dich - die Dinger sind immer noch hier. Ich weiss nicht, wie lange ich das noch durchhalte.", "Danke! Das war verdammt knapp. Deine Belohnung gehoert dir. Ich verschwinde von hier, sobald ich wieder klar denken kann.", "Die eingeschlossene Person ist gestorben.", "Mayday... Hallo?!", "Kann mich jemand hoeren? Die Dinger stehen direkt vor dem Haus. Ich kann hier nicht mehr lange bleiben.", "Wenn mich jemand hoert und herkommt: Ich kann zahlen. Bitte bestaetigt einfach, dass jemand unterwegs ist.");
		AddHelpMission(catalog, "SIDE-VERLETZT-01", "Verletzt", DZRMZ_SCENARIO_MEDICAL_AID, "", "SurvivorM_Mirek", 2, 5, 1, 28.0, 52.0, 0, 20000);
		SetHelpTexts(catalog, "Du hast mich gehoert... gut. Ich bin noch hier. Ich brauche medizinische Versorgung. Bitte beeil dich.", "Du bist wirklich gekommen... Danke. Noch ein bisschen laenger und ich waere hier verblutet.", "Der verletzte Ueberlebende ist gestorben.", "Ich wurde angeschossen. Die Blutung ist notduerftig gestoppt.", "Ich kann kaum laufen und brauche medizinische Hilfe.", "Wenn mich jemand hoert: Bitte kommt zu mir.");
		DZRMZ_MissionDefinition medical = catalog.Missions[catalog.Missions.Count() - 1]; medical.ObjectiveType = DZRMZ_OBJECTIVE_VISIT; medical.RequiredItemClass = "BandageDressing"; medical.RequiredItemLabel = "Verbandpaeckchen"; medical.RequiredQuantity = 1;
		AddHelpMission(catalog, "SIDE-BELAGERT-01", "Belagert", DZRMZ_SCENARIO_BESIEGED, "", "ExpansionNPCDenis", 6, 10, 3, 18.0, 42.0, 90, 30000);
		SetHelpTexts(catalog, "Endlich! Ich habe dich gehoert! Die kommen von allen Seiten. Wenn du unterwegs bist, dann mach schnell!", "Scheisse... das war knapp. Ich dachte wirklich, die kommen durch. Danke. Die Belohnung gehoert dir.", "Der belagerte Ueberlebende ist gestorben.", "Hallo? Hoert mich jemand?", "Sie kommen von allen Seiten. Die Tuer haelt nicht mehr lange.", "Bitte bestaetigt, dass jemand unterwegs ist.");
		AddHelpMission(catalog, "SIDE-ALLEIN-01", "Allein zurueckgeblieben", DZRMZ_SCENARIO_RESCUE, "", "SurvivorF_Eva", 5, 9, 1, 24.0, 48.0, 45, 20000);
		SetHelpTexts(catalog, "Hallo? Ja... ich hoere dich. Bitte sag mir, dass du wirklich unterwegs bist. Ich will hier nicht alleine bleiben.", "Danke... Ich dachte wirklich, hier kommt niemand mehr. Lass uns von hier verschwinden.", "Die Ueberlebende ist gestorben.", "Wir waren zu viert. Dann wurden wir angegriffen.", "Die anderen sind tot oder verschwunden. Ich bin jetzt allein.", "Hier draussen sind noch Infizierte. Bitte findet mich.");
		return catalog;
	}

	protected static void AddHelpMission(DZRMZ_MissionCatalog catalog, string id, string title, string scenario, string soundSet, string npcClass, int minInfected, int maxInfected, int waves, float spawnMin, float spawnMax, int holdSeconds, int reward)
	{
		ref DZRMZ_MissionDefinition d = new DZRMZ_MissionDefinition;
		d.Id=id; d.Category=DZRMZ_CATEGORY_CIVILIAN; d.Title=title; d.ObjectiveType=DZRMZ_OBJECTIVE_HOLD; d.Description="Hilferuf auf 89,5 MHz"; d.HoldSeconds=holdSeconds; d.ChainStart=true; d.RequiresListenConfirmation=true;
		d.ScenarioType=scenario; d.RadioSoundSet=soundSet; d.NpcClass=npcClass; d.SpawnProtectedNpc=true; d.InfectedMinimum=minInfected; d.InfectedMaximum=maxInfected; d.ThreatWaveCount=waves; d.ThreatSpawnMinimumRadius=spawnMin; d.ThreatSpawnMaximumRadius=spawnMax;
		d.RewardCurrencyClass="ExpansionBanknoteEuro"; d.RewardCurrencyAmount=reward; catalog.Missions.Insert(d);
	}
	protected static void SetHelpTexts(DZRMZ_MissionCatalog catalog, string acceptText, string completionText, string failureText, string line1, string line2, string line3)
	{
		DZRMZ_MissionDefinition d = catalog.Missions[catalog.Missions.Count() - 1];
		d.AcceptText=acceptText; d.CompletionText=completionText; d.FailureNpcDeadText=failureText;
		d.RadioLines.Insert(line1); d.RadioLines.Insert(line2); d.RadioLines.Insert(line3);
	}
	static ref DZRMZ_LocationCatalog CreateLocationCatalog()
	{
		ref DZRMZ_LocationCatalog catalog = new DZRMZ_LocationCatalog;

		AddLocation(catalog, "CHERNO", "Chernogorsk", 6650, 2560, true, true, false, true);
		AddLocation(catalog, "ELEKTRO", "Elektrozavodsk", 10460, 2440, true, true, false, true);
		AddLocation(catalog, "BEREZINO", "Berezino", 12250, 9500, true, true, false, true);
		AddLocation(catalog, "SVETLO", "Svetlojarsk", 13900, 13250, true, true, false, true);
		AddLocation(catalog, "NOVO", "Novodmitrovsk", 11500, 14350, true, true, false, true);
		AddLocation(catalog, "SEVERO", "Severograd", 7900, 12650, true, true, false, true);
		AddLocation(catalog, "ZELENO", "Zelenogorsk", 2700, 5300, true, true, false, true);
		AddLocation(catalog, "GORKA", "Gorka", 9600, 8900, true, true, false, true);
		AddLocation(catalog, "STAROYE", "Staroye", 10150, 5500, true, false, false, true);
		AddLocation(catalog, "KAMY", "Kamyshovo", 12100, 3500, true, false, false, true);
		AddLocation(catalog, "SOLNICH", "Solnichniy", 13300, 6250, true, true, false, true);
		AddLocation(catalog, "KABANINO", "Kabanino", 5300, 8600, true, true, false, true);
		AddLocation(catalog, "VYBOR", "Vybor", 3800, 8900, true, true, false, true);
		AddLocation(catalog, "NOVAYA", "Novaya Petrovka", 3450, 13000, true, true, false, true);
		AddLocation(catalog, "STARY", "Stary Sobor", 6100, 7800, true, true, false, true);

		AddLocation(catalog, "CHERNAYA", "Chernaya Polana", 12100, 13700, false, true, false, true);
		AddLocation(catalog, "POLANA", "Polana", 10700, 8100, true, true, false, true);
		AddLocation(catalog, "DUBROVKA", "Dubrovka", 10450, 9900, true, true, false, true);
		AddLocation(catalog, "KRASNO", "Krasnostav", 11200, 12250, true, true, false, true);
		AddLocation(catalog, "LUMBER", "Berezino Sawmill", 12700, 9700, false, true, false, false);
		AddLocation(catalog, "BALOTA_INDUSTRY", "Balota Industrie", 4500, 2350, false, true, false, false);
		AddLocation(catalog, "SOL_QUARRY", "Solnichniy Steinbruch", 12900, 8300, false, true, false, false);
		AddLocation(catalog, "MOGI", "Mogilevka", 7550, 5150, true, true, false, true);
		AddLocation(catalog, "DUBKY", "Dubky", 6500, 3600, true, true, false, true);
		AddLocation(catalog, "KOMAROVO", "Komarovo", 3650, 2450, true, true, false, true);

		AddLocation(catalog, "TISY", "Tisy Militaerbasis", 1600, 14000, false, false, true, false);
		AddLocation(catalog, "NWAF", "Nordwest-Flugfeld", 4600, 10200, false, true, true, false);
		AddLocation(catalog, "BALOTA_AF", "Balota Flugfeld", 4800, 2500, false, false, true, false);
		AddLocation(catalog, "PAVLOVO_MB", "Pavlovo Militaerbasis", 2200, 3350, false, false, true, false);
		AddLocation(catalog, "ZELENO_MB", "Zelenogorsk Militaerbasis", 2500, 5100, false, false, true, false);
		AddLocation(catalog, "TROITSKOE", "Troitskoe Militaerposten", 7500, 14700, false, false, true, false);
		AddLocation(catalog, "KAMENSK_MB", "Kamensk Militaerbasis", 7850, 14550, false, false, true, false);
		AddLocation(catalog, "MYSHKINO", "Myshkino Zeltlager", 1150, 7300, false, true, true, false);
		AddLocation(catalog, "GREEN_MOUNTAIN", "Green Mountain", 3700, 6000, false, true, true, false);
		AddLocation(catalog, "KRASNO_AF", "Krasnostav Flugfeld", 12100, 12600, false, false, true, false);
		AddLocation(catalog, "SKALISTY", "Skalisty Insel", 13600, 2900, true, true, true, false);

		return catalog;
	}

	protected static void AddMission(DZRMZ_MissionCatalog catalog, string id, string category, string title, string objectiveType, string description, string requiredItemClass, string requiredItemLabel, int requiredQuantity, int holdSeconds, string line1, string line2, string line3)
	{
		ref DZRMZ_MissionDefinition definition = new DZRMZ_MissionDefinition;
		definition.Id = id;
		definition.Category = category;
		definition.Title = title;
		definition.ObjectiveType = objectiveType;
		definition.Description = description;
		definition.RequiredItemClass = requiredItemClass;
		definition.RequiredItemLabel = requiredItemLabel;
		definition.RequiredQuantity = requiredQuantity;
		definition.HoldSeconds = holdSeconds;
		if (requiredItemClass == "WaterBottle")
		{
			definition.MinimumItemQuantity = 500.0;
			definition.RequiredLiquidTypeMask = LIQUID_GROUP_DRINKWATER;
		}
		else if (requiredItemClass == "Battery9V")
		{
			definition.RequireStoredEnergy = true;
		}
		else if (requiredItemClass == "DuctTape")
		{
			definition.MinimumItemQuantity = 10.0;
		}
		else if (requiredItemClass == "EpoxyPutty")
		{
			definition.MinimumItemQuantity = 25.0;
		}
		definition.RadioLines.Insert(line1);
		definition.RadioLines.Insert(line2);
		definition.RadioLines.Insert(line3);
		catalog.Missions.Insert(definition);
	}

	protected static void AddChainMission(DZRMZ_MissionCatalog catalog, string id, string title, string fixedLocationId, string nextMissionId, bool chainStart, string startRequiredItemClass, int holdSeconds, string line1, string line2, string line3)
	{
		ref DZRMZ_MissionDefinition definition = new DZRMZ_MissionDefinition;
		definition.Id = id;
		definition.Category = DZRMZ_CATEGORY_MILITARY;
		definition.Title = title;
		definition.ObjectiveType = DZRMZ_OBJECTIVE_HOLD;
		definition.Description = "Untersuche und sichere den angegebenen Einsatzort. Die Missionskette setzt erst nach erfolgreichem Abschluss fort.";
		definition.HoldSeconds = holdSeconds;
		definition.ChainStart = chainStart;
		definition.FixedLocationId = fixedLocationId;
		definition.NextMissionId = nextMissionId;
		definition.StartRequiredItemClass = startRequiredItemClass;
		definition.RadioLines.Insert(line1);
		definition.RadioLines.Insert(line2);
		definition.RadioLines.Insert(line3);
		catalog.Missions.Insert(definition);
	}

	protected static void AddLocation(DZRMZ_LocationCatalog catalog, string id, string name, float x, float z, bool civilian, bool scavenger, bool military, bool delivery)
	{
		ref DZRMZ_Location location = new DZRMZ_Location;
		location.Id = id;
		location.Name = name;
		location.Position.Insert(x);
		location.Position.Insert(0.0);
		location.Position.Insert(z);
		if (civilian)
			location.Tags.Insert(DZRMZ_CATEGORY_CIVILIAN);
		if (scavenger)
			location.Tags.Insert(DZRMZ_CATEGORY_SCAVENGER);
		if (military)
			location.Tags.Insert(DZRMZ_CATEGORY_MILITARY);
		if (delivery)
			location.Tags.Insert(DZRMZ_TAG_DELIVERY);
		catalog.Locations.Insert(location);
	}
}


