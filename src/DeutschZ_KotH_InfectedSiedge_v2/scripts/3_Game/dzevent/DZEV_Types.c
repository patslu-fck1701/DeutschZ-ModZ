class DZEV_MarkerData
{
	string UID;
	string Text;
	vector Pos;
	int Color;
	string IconName;
	bool Marker3D;
	bool Remove;

	void DZEV_MarkerData()
	{
		UID = "";
		Text = "";
		Pos = "0 0 0";
		Color = ARGB(255, 255, 255, 255);
		IconName = "Flag";
		Marker3D = true;
		Remove = false;
	}
}

class DZEV_EventConfig
{
	string EventId;
	string EventName;
	vector Center;
	float Radius;
	float CaptureSeconds;
	float TickSeconds;
	float CleanupDelaySeconds;
	float SpawnMinDistance;
	float SpawnMaxDistance;
	int WaveOneCount;
	int WaveTwoCount;
	int WaveThreeCount;
	float BossHealth;
	ref array<string> WaveOneTypes;
	ref array<string> WaveTwoTypes;
	ref array<string> WaveThreeTypes;

	void DZEV_EventConfig()
	{
		EventId = "DeutschZ_KOTH_InfectedSiege_v2";
		EventName = "DeutschZ K.o.t.H Infected Siege";
		Center = "7500 0 7500";
		Radius = 90.0;
		CaptureSeconds = 300.0;
		TickSeconds = 1.0;
		CleanupDelaySeconds = 300.0;
		SpawnMinDistance = 18.0;
		SpawnMaxDistance = 38.0;
		WaveOneCount = 12;
		WaveTwoCount = 7;
		WaveThreeCount = 5;
		BossHealth = 7500.0;

		WaveOneTypes = new array<string>;
		WaveOneTypes.Insert("ZmbM_CitizenASkinny_Blue");
		WaveOneTypes.Insert("ZmbM_JoggerSkinny_Red");
		WaveOneTypes.Insert("ZmbM_HandymanNormal_Green");

		WaveTwoTypes = new array<string>;
		WaveTwoTypes.Insert("DZEV_Infected_Stage2");

		WaveThreeTypes = new array<string>;
		WaveThreeTypes.Insert("DZEV_Infected_Stage3");
	}
}

class DZEV_GroupRecord
{
	string GroupId;
	string OwnerId;
	ref array<string> Members;

	void DZEV_GroupRecord()
	{
		GroupId = "";
		OwnerId = "";
		Members = new array<string>;
	}

	bool HasMember(string uid)
	{
		if (uid == "" || !Members)
			return false;

		return Members.Find(uid) >= 0;
	}
}

class DZEV_GroupsSave
{
	ref array<ref DZEV_GroupRecord> Groups;

	void DZEV_GroupsSave()
	{
		Groups = new array<ref DZEV_GroupRecord>;
	}
}
