class DZBGZ_Utils
{
	static void Log(string message)
	{
		Print(DZBGZ_LOG_PREFIX + message);
		DZBGZ_ProfilePaths.AppendLog(message);
	}

	static void Warn(string message)
	{
		Print(DZBGZ_LOG_PREFIX + "WARN " + message);
		DZBGZ_ProfilePaths.AppendLog("WARN " + message);
	}

	static void Error(string message)
	{
		Print(DZBGZ_LOG_PREFIX + "ERROR " + message);
		DZBGZ_ProfilePaths.AppendLog("ERROR " + message);
	}

	static ref array<float> MakeVectorArray(float x, float y, float z)
	{
		ref array<float> values = new array<float>;
		values.Insert(x);
		values.Insert(y);
		values.Insert(z);
		return values;
	}

	static vector ArrayToVector(array<float> values, vector fallback = "0 0 0")
	{
		if (!values || values.Count() < 3)
			return fallback;

		return Vector(values.Get(0), values.Get(1), values.Get(2));
	}

	static vector Grounded(vector pos)
	{
		if (GetGame())
			pos[1] = GetGame().SurfaceY(pos[0], pos[2]);

		return pos;
	}

	static vector RandomPointAround(vector center, float minDistance, float maxDistance)
	{
		if (maxDistance < minDistance)
			maxDistance = minDistance;

		float angle = Math.RandomFloatInclusive(0.0, 6.283185);
		float distance = Math.RandomFloatInclusive(minDistance, maxDistance);
		vector pos = center;
		pos[0] = center[0] + Math.Cos(angle) * distance;
		pos[2] = center[2] + Math.Sin(angle) * distance;
		return Grounded(pos);
	}

	static bool IsCfgVehicle(string typeName)
	{
		if (!GetGame() || typeName == "")
			return false;

		return GetGame().ConfigIsExisting("CfgVehicles " + typeName);
	}

	static bool IsCfgInventoryType(string typeName)
	{
		if (!GetGame() || typeName == "")
			return false;

		if (GetGame().ConfigIsExisting("CfgVehicles " + typeName))
			return true;

		if (GetGame().ConfigIsExisting("CfgMagazines " + typeName))
			return true;

		return GetGame().ConfigIsExisting("CfgWeapons " + typeName);
	}

	static void DeleteObjectSafe(Object object)
	{
		if (object && GetGame())
			GetGame().ObjectDelete(object);
	}

	static Object CreateObjectSafe(string typeName, vector pos, vector orientation = "0 0 0", int extraFlags = 0)
	{
		if (!GetGame() || typeName == "")
			return null;

		if (!IsCfgVehicle(typeName))
		{
			Warn("CfgVehicles missing: " + typeName);
			return null;
		}

		Object object = GetGame().CreateObjectEx(typeName, Grounded(pos), ECE_SETUP | ECE_PLACE_ON_SURFACE | extraFlags);
		if (!object)
		{
			Warn("CreateObjectEx failed: " + typeName);
			return null;
		}

		object.SetOrientation(orientation);
		Log("Spawned " + typeName + " at " + object.GetPosition().ToString());
		return object;
	}

}
