class DZBBC_Utils
{
	static void Log(string message)
	{
		Print(DZBBC_LOG_PREFIX + message);
		DZBBC_ProfilePaths.AppendLog(message);
	}

	static void Warn(string message)
	{
		Print(DZBBC_LOG_PREFIX + "WARN " + message);
		DZBBC_ProfilePaths.AppendLog("WARN " + message);
	}

	static void Error(string message)
	{
		Print(DZBBC_LOG_PREFIX + "ERROR " + message);
		DZBBC_ProfilePaths.AppendLog("ERROR " + message);
	}

	static string Localize(string text)
	{
		if (text == "")
			return text;

		string key = GetStringKey(text);
		if (key == "")
			return text;

		string translated = Widget.TranslateString("#" + key);
		if (translated != "" && translated != key && translated != "#" + key)
			return translated;

		return FallbackStringtableText(key, text);
	}

	static string GetStringKey(string text)
	{
		if (text.IndexOf("#STR_") == 0)
			return text.Substring(1, text.Length() - 1);

		if (text.IndexOf("STR_") == 0)
			return text;

		return "";
	}

	static string FallbackStringtableText(string key, string original)
	{
		if (key == "STR_DZBBC_EVENT_NAME")
			return "ConvoyZ Crashsite";
		if (key == "STR_DZBBC_TITLE_BLACKBOX")
			return "Blackbox";
		if (key == "STR_DZBBC_TITLE_SIGNAL_WARNING")
			return "Signalwarnung";
		if (key == "STR_DZBBC_TITLE_RECOVERY_UNIT")
			return "Bergungseinheit";
		if (key == "STR_DZBBC_TITLE_ADMIN")
			return "ConvoyZ Admin";
		if (key == "STR_DZBBC_TITLE_TESTMODE")
			return "ConvoyZ Testmodus";
		if (key == "STR_DZBBC_MSG_EVENT_ANNOUNCE")
			return "Notruf: Ein Militaerkonvoi wurde angegriffen. Signal markiert.";
		if (key == "STR_DZBBC_MSG_PLAYER_DETECTED")
			return "Kontakt im Kampfbereich. AmericanZ und RussianZ greifen ein.";
		if (key == "STR_DZBBC_MSG_COMBAT_COMPLETE")
			return "Kampfbereich gesichert. Blackbox freigegeben.";
		if (key == "STR_DZBBC_MSG_AUDIO_30")
			return "Audiofragment rekonstruiert: Konvoi kompromittiert.";
		if (key == "STR_DZBBC_MSG_GLOBAL_WARNING")
			return "Globale Warnung: Blackbox-Hack erkannt.";
		if (key == "STR_DZBBC_MSG_HACK_CANCELED")
			return "Blackbox-Hack abgebrochen.";
		if (key == "STR_DZBBC_MSG_DATACORE_FAILED")
			return "Datenkern konnte nicht erstellt werden.";
		if (key == "STR_DZBBC_MSG_DATACORE_UNSTABLE")
			return "Datenkern wurde instabil.";
		if (key == "STR_DZBBC_MSG_RECOVERY_90")
			return "Spezial-Bergungseinheit naehert sich dem Signal.";
		if (key == "STR_DZBBC_MSG_DATACORE_EXTRACTED")
			return "Datenkern extrahiert. Stabilitaet: 30 Minuten.";
		if (key == "STR_DZBBC_MSG_TERMINAL_STARTED")
			return "Terminalentschluesselung gestartet. Finale Verteidigung aktiv.";
		if (key == "STR_DZBBC_MSG_FINAL_DEFENSE")
			return "Finale Verteidigungswelle aktiv.";
		if (key == "STR_DZBBC_MSG_REWARD_UNLOCKED")
			return "Vorratscontainer entsperrt.";
		if (key == "STR_DZBBC_MSG_COMPLETED")
			return "ConvoyZ abgeschlossen. Bergung erfolgreich.";
		if (key == "STR_DZBBC_MSG_FAILED")
			return "ConvoyZ fehlgeschlagen. Signal verloren.";
		if (key == "STR_DZBBC_ADMIN_NO_PERMISSION")
			return "Keine Berechtigung oder DebugCommandsEnabled=false.";
		if (key == "STR_DZBBC_TEST_SPAWNED")
			return "Testobjekte gespawnt. Details stehen im Server-RPT.";
		if (key == "STR_DZBBC_TEST_VISUAL_READY")
			return "Du stehst am Testspawn. Pruefe Items, Marker und Notify.";
		if (key == "STR_DZBBC_TEST_COMPLETED")
			return "Testabschluss gesetzt. Pruefe Loot, Marker und Objekte.";

		return original;
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

	static Object CreateObjectSafe(string typeName, vector pos, bool placeOnSurface = true)
	{
		if (!GetGame() || typeName == "")
			return null;

		if (!IsCfgVehicle(typeName))
		{
			Warn("CfgVehicles missing for " + typeName);
			return null;
		}

		int flags = ECE_SETUP;
		vector spawnPos = pos;
		if (placeOnSurface)
		{
			spawnPos = Grounded(pos);
			flags = flags | ECE_PLACE_ON_SURFACE;
		}

		Object object = GetGame().CreateObjectEx(typeName, spawnPos, flags);
		if (!object)
			Warn("CreateObjectEx failed for " + typeName);
		else
			Log("Spawned object " + typeName + " at " + object.GetPosition().ToString());

		return object;
	}

	static EntityAI CreateAISafe(string typeName, vector pos)
	{
		if (!GetGame() || typeName == "")
			return null;

		if (!IsCfgVehicle(typeName))
		{
			Warn("CfgVehicles missing for AI " + typeName);
			return null;
		}

		Object object = GetGame().CreateObjectEx(typeName, Grounded(pos), ECE_SETUP | ECE_INITAI | ECE_PLACE_ON_SURFACE);
		EntityAI unit = EntityAI.Cast(object);
		if (!unit)
			Warn("AI spawn failed for " + typeName);
		else
			Log("Spawned AI " + typeName + " at " + unit.GetPosition().ToString());

		return unit;
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
}
