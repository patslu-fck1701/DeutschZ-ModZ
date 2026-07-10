class DZKOTH_Utils
{
	static void Log(string message)
	{
		Print(DZKOTH_Const.LOG_PREFIX + message);
	}

	static void Warn(string message)
	{
		Print(DZKOTH_Const.LOG_PREFIX + "WARN: " + message);
	}

	static void Error(string message)
	{
		Print(DZKOTH_Const.LOG_PREFIX + "ERROR: " + message);
	}

	static vector Grounded(vector pos)
	{
		if (!GetGame())
			return pos;

		pos[1] = GetGame().SurfaceY(pos[0], pos[2]);
		return pos;
	}

	static vector ArrayToVector(array<float> values, vector fallback = "0 0 0")
	{
		if (!values || values.Count() < 3)
			return fallback;

		return Vector(values.Get(0), values.Get(1), values.Get(2));
	}

	static ref array<float> MakeVectorArray(float x, float y, float z)
	{
		ref array<float> values = new array<float>;
		values.Insert(x);
		values.Insert(y);
		values.Insert(z);
		return values;
	}

	static string FormatXZ(vector pos)
	{
		return Math.Round(pos[0]).ToString() + " / " + Math.Round(pos[2]).ToString();
	}
}
