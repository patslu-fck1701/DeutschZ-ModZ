class DZCRZ_Denomination
{
	string ClassName;
	int Value;

	void DZCRZ_Denomination(string className = "", int value = 0)
	{
		ClassName = className;
		Value = value;
	}
}

class DZCRZ_ATMPosition
{
	string Id;
	string Name;
	ref array<float> Position;
	ref array<float> Orientation;
	bool Enabled;

	void DZCRZ_ATMPosition()
	{
		Position = new array<float>;
		Orientation = new array<float>;
		Enabled = true;
	}

	vector GetPosition()
	{
		if (!Position || Position.Count() < 3)
			return vector.Zero;
		return Vector(Position[0], Position[1], Position[2]);
	}

	vector GetOrientation()
	{
		if (!Orientation || Orientation.Count() < 3)
			return vector.Zero;
		return Vector(Orientation[0], Orientation[1], Orientation[2]);
	}

	void SetPosition(vector value)
	{
		Position.Clear();
		Position.Insert(value[0]);
		Position.Insert(value[1]);
		Position.Insert(value[2]);
	}

	void SetOrientation(vector value)
	{
		Orientation.Clear();
		Orientation.Insert(value[0]);
		Orientation.Insert(value[1]);
		Orientation.Insert(value[2]);
	}
}

class DZCRZ_ATMPositionsFile
{
	ref array<ref DZCRZ_ATMPosition> Positions;

	void DZCRZ_ATMPositionsFile()
	{
		Positions = new array<ref DZCRZ_ATMPosition>;
	}
}

class DZCRZ_CooldownEntry
{
	string TargetId;
	float RemainingSeconds;
}

class DZCRZ_CooldownFile
{
	int Version = 1;
	ref array<ref DZCRZ_CooldownEntry> Entries;

	void DZCRZ_CooldownFile()
	{
		Entries = new array<ref DZCRZ_CooldownEntry>;
	}
}

class DZCRZ_RuntimeSessionEntry
{
	string SessionId;
	string PlayerId;
	string TargetId;
	int ModuleType;
	float DurationSeconds;
	float ElapsedSeconds;
}

class DZCRZ_RuntimeFile
{
	int Version = 1;
	string LastStatus = "clean";
	ref array<ref DZCRZ_RuntimeSessionEntry> ActiveSessions;

	void DZCRZ_RuntimeFile()
	{
		ActiveSessions = new array<ref DZCRZ_RuntimeSessionEntry>;
	}
}

class DZCRZ_ClientProgressState
{
	static bool Visible;
	static float Progress;
	static float Duration;
	static string Label;
	static string Message;
	static int MessageUntil;

	static void SetProgress(bool visible, float progress, float duration, string label)
	{
		Visible = visible;
		Progress = progress;
		Duration = duration;
		Label = label;
	}

	static void ShowMessage(string message, int durationMs)
	{
		Message = message;
		MessageUntil = GetGame().GetTime() + durationMs;
	}
}

class DZCRZ_ClientEffectEvent
{
	bool Start;
	string EffectId;
	string SoundSet;
	vector Position;
	bool Loop;
}

class DZCRZ_ClientEffectQueue
{
	static ref array<ref DZCRZ_ClientEffectEvent> Events = new array<ref DZCRZ_ClientEffectEvent>;

	static void Push(bool start, string effectId, string soundSet, vector position, bool loop)
	{
		DZCRZ_ClientEffectEvent eventData = new DZCRZ_ClientEffectEvent;
		eventData.Start = start;
		eventData.EffectId = effectId;
		eventData.SoundSet = soundSet;
		eventData.Position = position;
		eventData.Loop = loop;
		Events.Insert(eventData);
	}
}
