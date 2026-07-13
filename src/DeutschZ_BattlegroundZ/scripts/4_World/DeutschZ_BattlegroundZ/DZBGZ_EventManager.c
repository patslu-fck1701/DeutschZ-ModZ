class DZBGZ_PlayerProgress
{
	string UID;
	string Name;
	int Stage;
	int NextSignalTime;
	bool ReaderHintSent;

	void DZBGZ_PlayerProgress()
	{
		UID = "";
		Name = "";
		Stage = 0;
		NextSignalTime = 0;
		ReaderHintSent = false;
	}
}

class DZBGZ_EventManager
{
	protected static ref DZBGZ_EventManager s_Instance;
	protected ref DZBGZ_MainConfig m_Config;
	protected ref DZBGZ_AIManager m_AI;
	protected ref array<ref DZBGZ_PlayerProgress> m_Progress;
	protected ref DZBGZ_UnlockStore m_Unlocks;
	protected Object m_Reader;
	protected bool m_Initialized;
	protected bool m_ApproachSpawned;
	protected bool m_BattlegroundSpawned;

	static DZBGZ_EventManager GetInstance()
	{
		if (!s_Instance)
			s_Instance = new DZBGZ_EventManager;

		return s_Instance;
	}

	void DZBGZ_EventManager()
	{
		m_AI = new DZBGZ_AIManager;
		m_Progress = new array<ref DZBGZ_PlayerProgress>;
		m_Unlocks = new DZBGZ_UnlockStore;
	}

	void InitServer()
	{
		if (m_Initialized || !GetGame() || !GetGame().IsServer())
			return;

		m_Config = DZBGZ_Config.LoadMain();
		LoadUnlocks();
		m_Initialized = true;
		DZBGZ_Utils.Log("Server initialized, version " + DZBGZ_VERSION);

		if (!m_Config || !m_Config.Enabled)
		{
			DZBGZ_Utils.Warn("BattlegroundZ disabled by config.");
			return;
		}

		if (m_Config.AutoSpawnReader)
			SpawnReader();

		StartTick();
	}

	void SyncPlayerState(PlayerBase player)
	{
		if (!DZBGZ_PlayerUtils.IsValidPlayer(player))
			return;

		if (IsUnlocked(DZBGZ_PlayerUtils.GetPlayerUid(player)))
			SendRevealedMarker(player);
	}

	bool ShouldBlockVehicleManagedUnitContact(Object unit)
	{
		if (!unit || !m_Config || !m_Config.Enabled || !m_AI || !m_AI.IsManagedUnit(unit))
			return false;

		float radius = m_Config.ApproachRadius;
		if (m_Config.BattlegroundRadius > radius)
			radius = m_Config.BattlegroundRadius;

		if (radius <= 0.0)
			return false;

		return vector.Distance(unit.GetPosition(), m_Config.GetBattlegroundCenter()) <= radius;
	}

	bool TryActivateReader(PlayerBase player, Object reader)
	{
		if (!DZBGZ_PlayerUtils.IsValidPlayer(player) || !reader || !m_Config)
			return false;

		float distance = vector.Distance(player.GetPosition(), reader.GetPosition());
		if (distance > m_Config.ReaderUseDistance)
		{
			DZBGZ_ServerRPC.SendNotification(player, m_Config.EventName, "Das Lesegeraet verliert dein Signal. Geh naeher ran.", 6.0);
			return false;
		}

		string uid = DZBGZ_PlayerUtils.GetPlayerUid(player);
		if (IsUnlocked(uid))
		{
			SendRevealedMarker(player);
			DZBGZ_ServerRPC.SendNotification(player, m_Config.EventName, "Der Kartenleser wurde fuer dich bereits aktiviert.", 6.0);
			return false;
		}

		EntityAI keycard = DZBGZ_PlayerUtils.FindInventoryItem(player, DZBGZ_KEYCARD_CLASSNAME);
		if (!keycard)
		{
			DZBGZ_ServerRPC.SendNotification(player, m_Config.EventName, "Der Leser reagiert nicht. Die BattlegroundZ Keycard fehlt.", 7.0);
			return false;
		}

		if (!RevealForPlayer(player))
		{
			DZBGZ_ServerRPC.SendNotification(player, m_Config.EventName, "Freischaltung konnte nicht gespeichert werden. Keycard wurde nicht verbraucht.", 7.0);
			return false;
		}

		if (!DZBGZ_PlayerUtils.DeleteInventoryItem(player, keycard))
			DZBGZ_Utils.Warn("Keycard unlock was saved, but the used keycard could not be deleted for " + DZBGZ_PlayerUtils.GetPlayerName(player));

		return true;
	}

	void HandleAdminCommand(PlayerIdentity identity, PlayerBase player, string command)
	{
		if (!GetGame() || !GetGame().IsServer() || command == "")
			return;

		if (!CanUseAdminCommand(identity))
		{
			DZBGZ_ServerRPC.SendNotification(player, "DZBGZ Admin", "Keine Berechtigung oder DebugCommandsEnabled=false.", 7.0);
			return;
		}

		array<string> parts = new array<string>;
		command.Split(" ", parts);
		string action = "status";
		if (parts.Count() > 1)
		{
			action = parts.Get(1);
			action.ToLower();
		}

		if (action == "start")
		{
			ForceSignalSequence(player);
			Reply(player, "Signalfolge fuer dich gestartet.");
		}
		else if (action == "reader")
		{
			SpawnReader();
			SendReaderHint(player);
			Reply(player, "Reader gespawnt und Hint gesendet.");
		}
		else if (action == "battle")
		{
			SpawnApproachAndBattle();
			Reply(player, "Battleground-Spawns ausgeloest.");
		}
		else if (action == "reveal")
		{
			RevealForPlayer(player);
			Reply(player, "Finaler Karten-Ping freigeschaltet.");
		}
		else if (action == "cleanup")
		{
			CleanupDynamic();
			Reply(player, "Dynamische BattlegroundZ-Objekte bereinigt.");
		}
		else
		{
			Reply(player, GetDebugState());
		}
	}

	protected void Tick()
	{
		if (!GetGame() || !GetGame().IsServer() || !m_Config || !m_Config.Enabled)
			return;

		ref array<PlayerBase> players = DZBGZ_PlayerUtils.GetOnlinePlayers();
		foreach (PlayerBase player: players)
		{
			TickPlayer(player);
		}
	}

	protected void TickPlayer(PlayerBase player)
	{
		if (!DZBGZ_PlayerUtils.IsValidPlayer(player))
			return;

		string uid = DZBGZ_PlayerUtils.GetPlayerUid(player);
		if (IsUnlocked(uid))
		{
			SendRevealedMarker(player);
			return;
		}

		if (!DZBGZ_PlayerUtils.HasInventoryItem(player, DZBGZ_KEYCARD_CLASSNAME))
			return;

		DZBGZ_PlayerProgress progress = GetProgress(player);
		if (!progress)
			return;

		int now = GetGame().GetTime();
		if (progress.NextSignalTime == 0)
			progress.NextSignalTime = now + Math.Round(m_Config.FirstSignalDelaySeconds * 1000.0);

		if (now >= progress.NextSignalTime)
			AdvanceSignal(player, progress);

		vector center = m_Config.GetBattlegroundCenter();
		float distance = vector.Distance(player.GetPosition(), center);
		if (distance <= m_Config.ApproachRadius && !m_ApproachSpawned)
			SpawnApproachSigns(player);

		if (distance <= m_Config.BattlegroundRadius && !m_BattlegroundSpawned)
			SpawnBattleground(player);

		if (distance <= m_Config.BattlegroundRadius && !progress.ReaderHintSent)
		{
			progress.ReaderHintSent = true;
			SendReaderHint(player);
		}
	}

	protected void AdvanceSignal(PlayerBase player, DZBGZ_PlayerProgress progress)
	{
		if (!player || !progress || !m_Config)
			return;

		if (progress.Stage == 0)
		{
			progress.Stage = 1;
			progress.NextSignalTime = GetGame().GetTime() + Math.Round(m_Config.SecondSignalDelaySeconds * 1000.0);
			DZBGZ_ServerRPC.SendFX(player, DZBGZ_FX_GLITCH_SHORT, "Ein tiefes Brummen kratzt durch deinen Kopf. Eine kaputte Karte flackert rot auf.", m_Config.GetBattlegroundCenter());
			DZBGZ_ServerRPC.SendNotification(player, m_Config.EventName, "Stoerung empfangen. Keine Quelle gefunden.", 7.0);
			return;
		}

		if (progress.Stage == 1)
		{
			progress.Stage = 2;
			progress.NextSignalTime = GetGame().GetTime() + Math.Round(m_Config.ThirdSignalDelaySeconds * 1000.0);
			DZBGZ_ServerRPC.SendFX(player, DZBGZ_FX_GLITCH_MEDIUM, "Die Karte bleibt laenger sichtbar. Roter Sektor instabil.", m_Config.GetBattlegroundCenter());
			DZBGZ_ServerRPC.SendNotification(player, m_Config.EventName, "Das Signal wiederholt sich. Es ist kein Zufall.", 8.0);
			return;
		}

		if (progress.Stage == 2)
		{
			progress.Stage = 3;
			progress.NextSignalTime = GetGame().GetTime() + Math.Round(m_Config.RiddleDelaySeconds * 1000.0);
			DZBGZ_ServerRPC.SendFX(player, DZBGZ_FX_GLITCH_MEDIUM, "Piepton erkannt. Kartenleser-Signatur passt zur Keycard.", m_Config.GetBattlegroundCenter());
			DZBGZ_ServerRPC.SendNotification(player, m_Config.EventName, "Ein kurzer Scanner-Ton liegt unter dem Rauschen.", 8.0);
			return;
		}

		if (progress.Stage == 3)
		{
			progress.Stage = 4;
			progress.NextSignalTime = 0;
			string riddle = "Dort, wo zwei Kriege sich kreuzten, aber keiner gewann... suche nicht den Eingang. Suche nach dem, was dich erkennt.";
			DZBGZ_ServerRPC.SendFX(player, DZBGZ_FX_RIDDLE, riddle, m_Config.GetBattlegroundCenter());
			DZBGZ_ServerRPC.SendNotification(player, m_Config.EventName, riddle, 12.0);
		}
	}

	protected void ForceSignalSequence(PlayerBase player)
	{
		if (!DZBGZ_PlayerUtils.IsValidPlayer(player))
			return;

		DZBGZ_PlayerProgress progress = GetProgress(player);
		if (!progress)
			return;

		progress.Stage = 0;
		progress.NextSignalTime = GetGame().GetTime() + 1000;
	}

	protected void SpawnReader()
	{
		if (!m_Config || m_Reader)
			return;

		m_Reader = DZBGZ_Utils.CreateObjectSafe(DZBGZ_READER_CLASSNAME, m_Config.GetReaderPosition(), m_Config.GetReaderOrientation());
		if (m_Reader)
			DZBGZ_Utils.Log("Card reader ready at " + m_Reader.GetPosition().ToString());
	}

	protected void SpawnApproachSigns(PlayerBase player)
	{
		m_ApproachSpawned = true;
		m_AI.SpawnApproachSigns(m_Config.GetBattlegroundCenter());
		DZBGZ_ServerRPC.SendNotification(player, m_Config.EventName, "Du bist nah genug. Die Umgebung reagiert.", 8.0);
	}

	protected void SpawnBattleground(PlayerBase player)
	{
		m_BattlegroundSpawned = true;
		m_AI.SpawnBattleground(m_Config.GetBattlegroundCenter());
		DZBGZ_ServerRPC.BroadcastNotification(m_Config.EventName, "Schuesse im Zielgebiet. RussianZ und AmericanZ sind bereits dort.", 9.0);
	}

	protected void SpawnApproachAndBattle()
	{
		if (!m_ApproachSpawned)
		{
			m_ApproachSpawned = true;
			m_AI.SpawnApproachSigns(m_Config.GetBattlegroundCenter());
		}

		if (!m_BattlegroundSpawned)
		{
			m_BattlegroundSpawned = true;
			m_AI.SpawnBattleground(m_Config.GetBattlegroundCenter());
		}
	}

	protected void SendReaderHint(PlayerBase player)
	{
		if (!player || !m_Config)
			return;

		DZBGZ_MarkerData marker = new DZBGZ_MarkerData;
		marker.UID = DZBGZ_READER_MARKER_UID;
		marker.Text = "BattlegroundZ Kartenleser";
		marker.Pos = DZBGZ_Utils.Grounded(m_Config.GetReaderMarkerPosition());
		marker.Color = ARGB(255, 255, 60, 30);
		DZBGZ_ServerRPC.SendMarker(player, marker);
		DZBGZ_ServerRPC.SendNotification(player, m_Config.EventName, "Das Geraet ist in der Naehe. Suche nach dem, was dich erkennt.", 8.0);
	}

	protected bool RevealForPlayer(PlayerBase player)
	{
		if (!DZBGZ_PlayerUtils.IsValidPlayer(player) || !m_Config)
			return false;

		string uid = DZBGZ_PlayerUtils.GetPlayerUid(player);
		if (!MarkUnlocked(uid, DZBGZ_PlayerUtils.GetPlayerName(player)))
			return false;

		SendRevealedMarker(player);
		RemoveReaderHint(player);
		DZBGZ_ServerRPC.SendFX(player, DZBGZ_FX_READER_ACCEPTED, "Du hast nicht den Ort gefunden. Du hast bestaetigt, dass du ihn sehen darfst.", m_Config.GetRevealedPingPosition());
		DZBGZ_ServerRPC.SendNotification(player, m_Config.EventName, "BattlegroundZ Ping dauerhaft freigeschaltet.", 10.0);
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(CleanupDynamic, Math.Round(m_Config.CleanupDelaySeconds * 1000.0), false);
		return true;
	}

	protected void SendRevealedMarker(PlayerBase player)
	{
		if (!player || !m_Config)
			return;

		DZBGZ_MarkerData marker = new DZBGZ_MarkerData;
		marker.UID = DZBGZ_MARKER_UID;
		marker.Text = "BattlegroundZ | Roter Sektor";
		marker.Pos = DZBGZ_Utils.Grounded(m_Config.GetRevealedPingPosition());
		marker.Color = ARGB(255, 220, 0, 0);
		DZBGZ_ServerRPC.SendMarker(player, marker);
	}

	protected void RemoveReaderHint(PlayerBase player)
	{
		if (!player)
			return;

		DZBGZ_MarkerData marker = new DZBGZ_MarkerData;
		marker.UID = DZBGZ_READER_MARKER_UID;
		marker.Remove = true;
		DZBGZ_ServerRPC.SendMarker(player, marker);
	}

	protected DZBGZ_PlayerProgress GetProgress(PlayerBase player)
	{
		string uid = DZBGZ_PlayerUtils.GetPlayerUid(player);
		if (uid == "")
			return null;

		foreach (DZBGZ_PlayerProgress progress: m_Progress)
		{
			if (progress && progress.UID == uid)
				return progress;
		}

		DZBGZ_PlayerProgress created = new DZBGZ_PlayerProgress;
		created.UID = uid;
		created.Name = DZBGZ_PlayerUtils.GetPlayerName(player);
		m_Progress.Insert(created);
		DZBGZ_Utils.Log("Tracking keycard player " + created.Name);
		return created;
	}

	protected bool IsUnlocked(string uid)
	{
		if (uid == "" || !m_Unlocks || !m_Unlocks.Entries)
			return false;

		foreach (DZBGZ_UnlockEntry entry: m_Unlocks.Entries)
		{
			if (entry && entry.UID == uid && entry.Revealed)
				return true;
		}

		return false;
	}

	protected bool MarkUnlocked(string uid, string name)
	{
		if (uid == "")
			return false;

		if (!m_Unlocks)
			m_Unlocks = new DZBGZ_UnlockStore;

		foreach (DZBGZ_UnlockEntry entry: m_Unlocks.Entries)
		{
			if (entry && entry.UID == uid)
			{
				string oldName = entry.Name;
				bool oldRevealed = entry.Revealed;
				entry.Name = name;
				entry.Revealed = true;
				if (SaveUnlocks())
					return true;

				entry.Name = oldName;
				entry.Revealed = oldRevealed;
				return false;
			}
		}

		DZBGZ_UnlockEntry created = new DZBGZ_UnlockEntry;
		created.UID = uid;
		created.Name = name;
		created.Revealed = true;
		m_Unlocks.Entries.Insert(created);
		if (SaveUnlocks())
			return true;

		m_Unlocks.Entries.RemoveItem(created);
		return false;
	}

	protected void LoadUnlocks()
	{
		DZBGZ_ProfilePaths.Ensure();

		m_Unlocks = new DZBGZ_UnlockStore;
		string errorMessage;
		if (FileExist(DZBGZ_UNLOCKS_JSON))
		{
			if (!JsonFileLoader<ref DZBGZ_UnlockStore>.LoadFile(DZBGZ_UNLOCKS_JSON, m_Unlocks, errorMessage))
				DZBGZ_Utils.Warn("Could not load unlocks. " + errorMessage);
		}
		else
		{
			SaveUnlocks();
		}
	}

	protected bool SaveUnlocks()
	{
		if (!m_Unlocks)
			m_Unlocks = new DZBGZ_UnlockStore;

		string errorMessage;
		if (!JsonFileLoader<ref DZBGZ_UnlockStore>.SaveFile(DZBGZ_UNLOCKS_JSON, m_Unlocks, errorMessage))
		{
			DZBGZ_Utils.Warn("Could not save unlocks. " + errorMessage);
			return false;
		}

		return true;
	}

	protected void StartTick()
	{
		float seconds = m_Config.TickSeconds;
		if (seconds <= 0.0)
			seconds = 10.0;

		int tickMs = Math.Round(seconds * 1000.0);
		if (tickMs < 1000)
			tickMs = 1000;

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(Tick);
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(Tick, tickMs, true);
	}

	protected bool CanUseAdminCommand(PlayerIdentity identity)
	{
		if (!identity || !m_Config || !m_Config.DebugCommandsEnabled)
			return false;

		if (!m_Config.AdminUIDs || m_Config.AdminUIDs.Count() == 0)
			return m_Config.AllowAdminWhenListEmpty;

		return m_Config.AdminUIDs.Find(identity.GetId()) >= 0;
	}

	protected void Reply(PlayerBase player, string text)
	{
		DZBGZ_Utils.Log("Admin: " + text);
		DZBGZ_ServerRPC.SendNotification(player, "DZBGZ Admin", text, 7.0);
	}

	void CleanupDynamic()
	{
		if (m_AI)
			m_AI.Cleanup();

		m_ApproachSpawned = false;
		m_BattlegroundSpawned = false;
		DZBGZ_Utils.Log("Dynamic cleanup completed. Reader and persistent unlocks remain.");
	}

	string GetDebugState()
	{
		string reader = "no";
		if (m_Reader)
			reader = m_Reader.GetPosition().ToString();

		string text = "version=" + DZBGZ_VERSION;
		text = text + " reader=" + reader;
		text = text + " approach=" + m_ApproachSpawned.ToString();
		text = text + " battle=" + m_BattlegroundSpawned.ToString();
		text = text + " tracked=" + m_Progress.Count().ToString();
		text = text + " unlocks=" + m_Unlocks.Entries.Count().ToString();
		return text;
	}
}
