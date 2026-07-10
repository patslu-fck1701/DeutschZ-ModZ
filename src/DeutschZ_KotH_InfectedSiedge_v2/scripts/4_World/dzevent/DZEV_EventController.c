class DZEV_EventController
{
	private static ref DZEV_EventController s_Instance;

	protected ref DZEV_EventConfig m_Config;
	protected ref array<PlayerBase> m_PlayersInside;
	protected ref array<Object> m_SpawnedObjects;
	protected DZEV_EventZoneTrigger m_Trigger;
	protected DZEV_KOTH_Flagpole m_Flagpole;
	protected EntityAI m_Boss;
	protected int m_State;
	protected float m_CaptureProgress;
	protected string m_CaptureOwnerUid;
	protected string m_CaptureGroupId;
	protected bool m_CapturePausedByEnemy;
	protected bool m_WaveOneSpawned;
	protected bool m_WaveTwoSpawned;
	protected bool m_WaveThreeSpawned;
	protected bool m_Initialized;

	static DZEV_EventController GetInstance()
	{
		if (!s_Instance)
			s_Instance = new DZEV_EventController;

		return s_Instance;
	}

	void DZEV_EventController()
	{
		m_PlayersInside = new array<PlayerBase>;
		m_SpawnedObjects = new array<Object>;
		m_State = DZEV_States.READY;
		m_CaptureOwnerUid = "";
		m_CaptureGroupId = "";
		m_CapturePausedByEnemy = false;
	}

	void InitServer()
	{
		if (m_Initialized || !GetGame() || !GetGame().IsServer())
			return;

		m_Config = DZEV_Persistence.LoadConfig();
		DZEV_GroupManager.GetInstance().InitServer();
		SpawnWorldObjects();
		m_Initialized = true;
		Print("[DZEV] Event controller ready: " + m_Config.EventName);
	}

	void OnPlayerEntered(PlayerBase player)
	{
		if (!player || !player.GetIdentity())
			return;

		AddPlayerInside(player);
		SyncStateToPlayer(player);

		if (m_State == DZEV_States.READY)
			BeginCapture(player);
		else if (m_State == DZEV_States.CAPTURING && !m_WaveOneSpawned)
			SpawnWaveAroundPlayer(player, m_Config.WaveOneTypes, m_Config.WaveOneCount);
	}

	void OnPlayerLeft(PlayerBase player)
	{
		if (!player)
			return;

		RemovePlayerInside(player);
		DZEV_MarkerSender.SendProgress(player, DZEV_ProgressModes.HIDE, "", 0, 0);
	}

	bool IsPositionInActiveEventZone(vector pos)
	{
		if (!m_Config)
			return false;

		if (m_State == DZEV_States.READY || m_State == DZEV_States.CLEANUP)
			return false;

		return vector.Distance(pos, m_Config.Center) <= m_Config.Radius;
	}

	bool ShouldBlockVehicleZombieContact(Object zombie)
	{
		if (!zombie)
			return false;

		return IsPositionInActiveEventZone(zombie.GetPosition());
	}

	protected void SpawnWorldObjects()
	{
		vector center = Grounded(m_Config.Center);

		if (!m_Trigger)
		{
			m_Trigger = DZEV_EventZoneTrigger.Cast(GetGame().CreateObjectEx("DZEV_EventZoneTrigger", center, ECE_NONE));
			if (m_Trigger)
				m_Trigger.Setup(this, m_Config.Radius);
		}

		if (!m_Flagpole)
		{
			m_Flagpole = DZEV_KOTH_Flagpole.Cast(GetGame().CreateObjectEx("DZEV_KOTH_Flagpole", center, ECE_PLACE_ON_SURFACE));
			if (m_Flagpole)
			{
				m_Flagpole.SetOrientation("0 0 0");
				m_Flagpole.DZEV_EnsureEventFlag();
				m_Flagpole.DZEV_SetSmokeState(DZEV_States.READY);
			}
		}

		BroadcastMarker("DZEV_KOTH_MAIN", "K.o.t.H bereit", center, ARGB(255, 255, 210, 0), "Flag", true, false);
	}

	protected void BeginCapture(PlayerBase starter)
	{
		m_State = DZEV_States.CAPTURING;
		m_CaptureProgress = 0.0;
		m_CaptureOwnerUid = GetPlayerUid(starter);
		m_CaptureGroupId = DZEV_GroupManager.GetInstance().GetPlayerGroupId(m_CaptureOwnerUid);
		m_CapturePausedByEnemy = false;
		m_WaveOneSpawned = true;
		m_WaveTwoSpawned = false;
		m_WaveThreeSpawned = false;

		if (m_Flagpole)
			m_Flagpole.DZEV_SetSmokeState(DZEV_States.CAPTURING);

		BroadcastMarker("DZEV_KOTH_MAIN", "K.o.t.H Capture laeuft", Grounded(m_Config.Center), ARGB(255, 255, 210, 0), "Flag", true, false);
		BroadcastNotify("DeutschZ K.o.t.H", "Capture gestartet. Infected Siege beginnt.", 0);
		BroadcastFX(DZEV_FXIds.MUSIC_START, Grounded(m_Config.Center));

		SpawnWaveForPlayers(m_Config.WaveOneTypes, m_Config.WaveOneCount);

		int tickMs = m_Config.TickSeconds;
		tickMs = tickMs * 1000;
		if (tickMs < 250)
			tickMs = 1000;

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(Tick);
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(Tick, tickMs, true);
	}

	protected void Tick()
	{
		CleanupPlayerList();

		if (m_State == DZEV_States.CAPTURING)
		{
			if (m_PlayersInside.Count() == 0)
				return;

			if (IsCaptureBlockedByEnemy())
			{
				if (!m_CapturePausedByEnemy)
				{
					m_CapturePausedByEnemy = true;
					BroadcastNotify("DeutschZ K.o.t.H", "Capture pausiert: Gegner in der Zone.", 0, 5.0);
				}

				BroadcastProgress(DZEV_ProgressModes.CAPTURE, "Capture pausiert", m_CaptureProgress, 100.0);
				return;
			}

			if (m_CapturePausedByEnemy)
			{
				m_CapturePausedByEnemy = false;
				BroadcastNotify("DeutschZ K.o.t.H", "Capture laeuft weiter.", 0, 4.0);
			}

			m_CaptureProgress = Math.Clamp(m_CaptureProgress + ((m_Config.TickSeconds / m_Config.CaptureSeconds) * 100.0), 0.0, 100.0);

			if (m_CaptureProgress >= 33.0 && !m_WaveTwoSpawned)
			{
				m_WaveTwoSpawned = true;
				BroadcastNotify("DeutschZ K.o.t.H", "33 Prozent: weniger, aber doppelt starke Infected.", 0);
				SpawnWaveForPlayers(m_Config.WaveTwoTypes, m_Config.WaveTwoCount);
			}

			if (m_CaptureProgress >= 66.0 && !m_WaveThreeSpawned)
			{
				m_WaveThreeSpawned = true;
				BroadcastNotify("DeutschZ K.o.t.H", "66 Prozent: schwere langsame Infected brechen durch.", 0);
				SpawnWaveForPlayers(m_Config.WaveThreeTypes, m_Config.WaveThreeCount);
			}

			BroadcastProgress(DZEV_ProgressModes.CAPTURE, "Capture", m_CaptureProgress, 100.0);

			if (m_CaptureProgress >= 100.0)
				StartBossPhase();
		}
		else if (m_State == DZEV_States.BOSS)
		{
			TickBoss();
		}
	}

	protected void StartBossPhase()
	{
		m_State = DZEV_States.BOSS;
		m_CaptureProgress = 100.0;

		if (m_Flagpole)
			m_Flagpole.DZEV_SetSmokeState(DZEV_States.BOSS);

		vector center = Grounded(m_Config.Center);
		BroadcastMarker("DZEV_KOTH_MAIN", "Bozz Zombie freigesetzt", center, ARGB(255, 220, 20, 20), "Skull 3", true, false);
		BroadcastProgress(DZEV_ProgressModes.HIDE, "", 0, 0);
		BroadcastFX(DZEV_FXIds.FIREWORKS, center);
		BroadcastFX(DZEV_FXIds.BOSS_ALARM, center);
		BroadcastNotify("Bozz Zombie Freigesetzt", "Mummy langsam, extrem stark, 7500 HP.", 0, 9.0);

		vector bossPos = Grounded(center + "8 0 8");
		m_Boss = EntityAI.Cast(GetGame().CreateObjectEx("DZEV_Boss_Mummy", bossPos, ECE_PLACE_ON_SURFACE | ECE_INITAI | ECE_EQUIP_ATTACHMENTS));
		if (m_Boss)
		{
			m_Boss.SetHealth("", "Health", m_Config.BossHealth);
			m_SpawnedObjects.Insert(m_Boss);
		}
	}

	protected void TickBoss()
	{
		if (!m_Boss || !m_Boss.IsAlive())
		{
			CompleteEvent();
			return;
		}

		float hp = m_Boss.GetHealth("", "Health");
		if (hp < 0.0)
			hp = 0.0;
		BroadcastProgress(DZEV_ProgressModes.BOSS, "Bozz Zombie", hp, m_Config.BossHealth);
	}

	protected void CompleteEvent()
	{
		m_State = DZEV_States.COMPLETED;
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(Tick);

		vector center = Grounded(m_Config.Center);
		if (m_Flagpole)
			m_Flagpole.DZEV_SetSmokeState(DZEV_States.COMPLETED);

		BroadcastMarker("DZEV_KOTH_MAIN", "K.o.t.H abgeschlossen", center, ARGB(255, 220, 20, 20), "Flag", true, false);
		BroadcastProgress(DZEV_ProgressModes.HIDE, "", 0, 0);
		BroadcastFX(DZEV_FXIds.MUSIC_STOP, center);
		BroadcastNotify("DeutschZ K.o.t.H", "Event abgeschlossen. Cleanup in 5 Minuten.", 0, 10.0);
		SpawnRewards(center);

		int cleanupDelayMs = m_Config.CleanupDelaySeconds;
		cleanupDelayMs = cleanupDelayMs * 1000;
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(CleanupEvent, cleanupDelayMs, false);
	}

	protected void CleanupEvent()
	{
		m_State = DZEV_States.CLEANUP;
		m_CaptureOwnerUid = "";
		m_CaptureGroupId = "";
		m_CapturePausedByEnemy = false;

		foreach (Object obj: m_SpawnedObjects)
		{
			if (obj && GetGame())
				GetGame().ObjectDelete(obj);
		}

		m_SpawnedObjects.Clear();
		m_Boss = null;
		m_CaptureProgress = 0.0;
		m_WaveOneSpawned = false;
		m_WaveTwoSpawned = false;
		m_WaveThreeSpawned = false;
		m_State = DZEV_States.READY;

		if (m_Flagpole)
			m_Flagpole.DZEV_SetSmokeState(DZEV_States.READY);

		BroadcastMarker("DZEV_KOTH_MAIN", "K.o.t.H bereit", Grounded(m_Config.Center), ARGB(255, 255, 210, 0), "Flag", true, false);
		Print("[DZEV] Event cleanup done, zone ready again.");
	}

	protected void SpawnRewards(vector center)
	{
		Object chest = GetGame().CreateObjectEx("DZEV_KOTH_SeaChest", Grounded(center + "2 0 2"), ECE_PLACE_ON_SURFACE);
		if (chest)
			m_SpawnedObjects.Insert(chest);

		Object keycard = GetGame().CreateObjectEx("DZEV_KOTH_KeyCard", Grounded(center + "2.5 0 1.5"), ECE_PLACE_ON_SURFACE);
		if (keycard)
			m_SpawnedObjects.Insert(keycard);
	}

	protected void SpawnWaveForPlayers(array<string> types, int count)
	{
		foreach (PlayerBase player: m_PlayersInside)
		{
			if (player)
				SpawnWaveAroundPlayer(player, types, count);
		}
	}

	protected void SpawnWaveAroundPlayer(PlayerBase player, array<string> types, int count)
	{
		if (!player || !types || types.Count() == 0 || count <= 0)
			return;

		for (int i = 0; i < count; i++)
		{
			string type = types.Get(Math.RandomInt(0, types.Count()));
			vector spawnPos = RandomSpawnAround(player.GetPosition());
			EntityAI infected = EntityAI.Cast(GetGame().CreateObjectEx(type, spawnPos, ECE_PLACE_ON_SURFACE | ECE_INITAI | ECE_EQUIP_ATTACHMENTS));
			if (infected)
				m_SpawnedObjects.Insert(infected);
		}
	}

	protected vector RandomSpawnAround(vector origin)
	{
		float angle = Math.RandomFloatInclusive(0.0, 6.28318);
		float distance = Math.RandomFloatInclusive(m_Config.SpawnMinDistance, m_Config.SpawnMaxDistance);
		vector pos = origin + Vector(Math.Cos(angle) * distance, 0, Math.Sin(angle) * distance);
		return Grounded(pos);
	}

	protected vector Grounded(vector pos)
	{
		if (GetGame())
			pos[1] = GetGame().SurfaceY(pos[0], pos[2]);

		return pos;
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
			if (!player || !player.IsAlive() || !player.GetIdentity())
				m_PlayersInside.Remove(i);
		}
	}

	protected bool IsCaptureBlockedByEnemy()
	{
		bool hasCaptureTeamMember = false;

		foreach (PlayerBase player: m_PlayersInside)
		{
			if (!player || !player.GetIdentity())
				continue;

			if (IsCaptureTeamMember(player))
			{
				hasCaptureTeamMember = true;
				continue;
			}

			return true;
		}

		return !hasCaptureTeamMember;
	}

	protected bool IsCaptureTeamMember(PlayerBase player)
	{
		if (!player || !player.GetIdentity())
			return false;

		string uid = player.GetIdentity().GetId();
		if (uid != "" && uid == m_CaptureOwnerUid)
			return true;

		if (m_CaptureGroupId != "" && DZEV_GroupManager.GetInstance().GetPlayerGroupId(uid) == m_CaptureGroupId)
			return true;

		return false;
	}

	protected string GetPlayerUid(PlayerBase player)
	{
		if (!player || !player.GetIdentity())
			return "";

		return player.GetIdentity().GetId();
	}

	void SyncStateToPlayer(PlayerBase player)
	{
		if (!player || !m_Config)
			return;

		vector center = Grounded(m_Config.Center);
		int color = ARGB(255, 255, 210, 0);
		string text = "K.o.t.H bereit";
		string icon = "Flag";

		if (m_State == DZEV_States.CAPTURING)
		{
			color = ARGB(255, 255, 210, 0);
			text = "K.o.t.H Capture laeuft";
		}
		else if (m_State == DZEV_States.BOSS)
		{
			color = ARGB(255, 220, 20, 20);
			text = "Bozz Zombie freigesetzt";
			icon = "Skull 3";
		}
		else if (m_State == DZEV_States.COMPLETED)
		{
			color = ARGB(255, 220, 20, 20);
			text = "K.o.t.H abgeschlossen";
		}

		DZEV_MarkerData marker = new DZEV_MarkerData;
		marker.UID = "DZEV_KOTH_MAIN";
		marker.Text = text;
		marker.Pos = center;
		marker.Color = color;
		marker.IconName = icon;
		marker.Marker3D = true;
		DZEV_MarkerSender.SendMarker(player, marker);

		if (m_State == DZEV_States.CAPTURING)
		{
			string progressLabel = "Capture";
			if (m_CapturePausedByEnemy)
				progressLabel = "Capture pausiert";

			DZEV_MarkerSender.SendProgress(player, DZEV_ProgressModes.CAPTURE, progressLabel, m_CaptureProgress, 100.0);
		}
		else if (m_State == DZEV_States.BOSS && m_Boss)
			DZEV_MarkerSender.SendProgress(player, DZEV_ProgressModes.BOSS, "Bozz Zombie", m_Boss.GetHealth("", "Health"), m_Config.BossHealth);
	}

	protected void BroadcastMarker(string uid, string text, vector pos, int color, string icon, bool marker3D, bool remove)
	{
		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);

		foreach (Man man: players)
		{
			PlayerBase player = PlayerBase.Cast(man);
			if (!player)
				continue;

			DZEV_MarkerData marker = new DZEV_MarkerData;
			marker.UID = uid;
			marker.Text = text;
			marker.Pos = pos;
			marker.Color = color;
			marker.IconName = icon;
			marker.Marker3D = marker3D;
			marker.Remove = remove;
			DZEV_MarkerSender.SendMarker(player, marker);
		}
	}

	protected void BroadcastProgress(int mode, string label, float current, float max)
	{
		foreach (PlayerBase player: m_PlayersInside)
		{
			if (player)
				DZEV_MarkerSender.SendProgress(player, mode, label, current, max);
		}
	}

	protected void BroadcastFX(int fx, vector pos)
	{
		foreach (PlayerBase player: m_PlayersInside)
		{
			if (player)
				DZEV_MarkerSender.SendFX(player, fx, pos);
		}
	}

	protected void BroadcastNotify(string title, string text, int color, float time = 7.0)
	{
		foreach (PlayerBase player: m_PlayersInside)
		{
			if (player)
				DZEV_MarkerSender.Notify(player, title, text, color, time);
		}
	}
}
