#ifdef EXPANSIONMODAI
[eAIRegisterFaction(eAIFactionRussianZ)]
class eAIFactionRussianZ : eAIFactionEast
{
	override string GetDisplayName()
	{
		return "RussianZ";
	}
};

[eAIRegisterFaction(eAIFactionAmericanZ)]
class eAIFactionAmericanZ : eAIFactionWest
{
	override string GetDisplayName()
	{
		return "AmericanZ";
	}
};
#endif

class DZBBC_ExpansionBridge
{
	static bool HasServerMarkers()
	{
#ifdef EXPANSIONMODNAVIGATION
		return ExpansionMarkerModule.GetModuleInstance() != null;
#endif
		return false;
	}

	static bool UpsertServerMarker(DZBBC_MarkerData marker)
	{
#ifdef EXPANSIONMODNAVIGATION
		if (!marker || marker.UID == "")
			return false;

		ExpansionMarkerModule markerModule = ExpansionMarkerModule.GetModuleInstance();
		if (!markerModule)
			return false;

		bool marker3D = marker.UID == DZBBC_CRASHSITE_MARKER_UID || marker.UID == DZBBC_TERMINAL_MARKER_UID;
		markerModule.RemoveServerMarker(marker.UID);
		ExpansionMarkerData expansionMarker = markerModule.CreateServerMarker(GetExpansionLabel(marker), GetExpansionIcon(marker), marker.Pos, marker.Color, marker3D, marker.UID);
		if (!expansionMarker)
			return false;

		if (marker3D)
			expansionMarker.SetVisibility(EXPANSION_MARKER_VIS_WORLD | EXPANSION_MARKER_VIS_MAP);

		DZBBC_Utils.Log("Expansion marker synced " + marker.UID + " 3D=" + marker3D.ToString());
		return true;
#endif
		return false;
	}

	static bool RemoveServerMarker(string uid)
	{
#ifdef EXPANSIONMODNAVIGATION
		if (uid == "")
			return false;

		ExpansionMarkerModule markerModule = ExpansionMarkerModule.GetModuleInstance();
		if (!markerModule)
			return false;

		markerModule.RemoveServerMarker(uid);
		DZBBC_Utils.Log("Expansion marker removed " + uid);
		return true;
#endif
		return false;
	}

	static bool SendNotification(PlayerBase player, string title, string message, float time = 7.0)
	{
#ifdef EXPANSIONMODCORE
		if (!player)
			return false;

		PlayerIdentity identity = player.GetIdentity();
		if (!identity)
			return false;

		string cleanTitle = DZBBC_Utils.Localize(title);
		string cleanMessage = DZBBC_Utils.Localize(message);
		ExpansionNotification(cleanTitle, cleanMessage, "Info", ARGB(255, 46, 160, 220), time).Create(identity);
		DZBBC_Utils.Log("Expansion notification sent " + cleanTitle);
		return true;
#endif
		return false;
	}

	static bool HasAI()
	{
#ifdef EXPANSIONMODAI
		return true;
#endif
		return false;
	}

	static int SpawnAIGroup(DZBBC_AIFactionConfig faction, DZBBC_MainConfig main, vector center, array<EntityAI> spawnedUnits)
	{
#ifdef EXPANSIONMODAI
		if (!faction || !main || !spawnedUnits || !faction.UseExpansionAI || faction.ExpansionCount <= 0)
			return 0;

		if (!faction.ExpansionTypes || faction.ExpansionTypes.Count() == 0)
			return 0;

		eAIFaction aiFaction = CreateAIFaction(faction.ExpansionFaction);
		eAIGroup group = eAIGroup.CreateGroup(aiFaction);
		if (group)
		{
			vector waypointA = DZBBC_Utils.Grounded(center);
			vector waypointB = DZBBC_Utils.RandomPointAround(center, main.SpawnMinDistance, main.SpawnMaxDistance);
			vector waypointC = DZBBC_Utils.RandomPointAround(center, main.SpawnMinDistance, main.SpawnMaxDistance);
			group.AddWaypoint(waypointA);
			group.AddWaypoint(waypointB);
			group.AddWaypoint(waypointC);
			group.AddWaypoint(waypointA);
			group.SetWaypointBehaviour(eAIWaypointBehavior.LOOP);
		}

		int spawned = 0;
		for (int i = 0; i < faction.ExpansionCount; i++)
		{
			string typeName = faction.ExpansionTypes.Get(Math.RandomInt(0, faction.ExpansionTypes.Count()));
			if (typeName == "" || !DZBBC_Utils.IsCfgVehicle(typeName))
			{
				DZBBC_Utils.Warn("Expansion AI type missing in config: " + typeName);
				continue;
			}

			vector pos = DZBBC_Utils.RandomPointAround(center, main.SpawnMinDistance, main.SpawnMaxDistance);
			Object object = GetGame().CreateObjectEx(typeName, pos, ECE_SETUP | ECE_INITAI | ECE_PLACE_ON_SURFACE);
			eAIBase ai;
			if (!Class.CastTo(ai, object))
			{
				DZBBC_Utils.Warn("Expansion AI spawn failed for " + typeName);
				if (object)
					DZBBC_Utils.DeleteObjectSafe(object);
				continue;
			}

			if (group)
				ai.SetGroup(group);

			ai.eAI_SetAccuracy(faction.AccuracyMin, faction.AccuracyMax);
			ApplyExpansionLoadout(ai, faction);
			spawnedUnits.Insert(ai);
			spawned++;
			DZBBC_Utils.Log("Spawned Expansion AI " + typeName + " at " + ai.GetPosition().ToString() + " accuracy=" + faction.AccuracyMin.ToString() + "-" + faction.AccuracyMax.ToString());
		}

		if (spawned > 0)
			DZBBC_Utils.Log("Expansion AI group spawned " + faction.FactionName + ": " + spawned.ToString() + "/" + faction.ExpansionCount.ToString());

		return spawned;
#endif
		return 0;
	}

	protected static string GetExpansionIcon(DZBBC_MarkerData marker)
	{
		if (!marker)
			return "Map Marker";

		if (marker.UID == DZBBC_CRASHSITE_MARKER_UID)
			return "Airdrop";

		if (marker.UID == DZBBC_TRANSPORT_MARKER_UID)
			return "Car";

		return "Map Marker";
	}

	protected static string GetExpansionLabel(DZBBC_MarkerData marker)
	{
		if (!marker)
			return "ConvoyZ Event";

		if (marker.UID == DZBBC_CRASHSITE_MARKER_UID)
			return StripStringtableKey(marker.Text, "ConvoyZ Crash Site");

		if (marker.UID == DZBBC_TERMINAL_MARKER_UID)
			return StripStringtableKey(marker.Text, "ConvoyZ Decrypt Terminal");

		if (marker.UID == DZBBC_TRANSPORT_MARKER_UID)
			return StripStringtableKey(marker.Text, "ConvoyZ DataCore Transport");

		return StripStringtableKey(marker.Text, "ConvoyZ Event");
	}

	protected static string StripStringtableKey(string text, string fallback)
	{
		if (text == "")
			return fallback;

		if (text.IndexOf("#STR_") == 0)
			return fallback;

		return text;
	}

#ifdef EXPANSIONMODAI
	protected static eAIFaction CreateAIFaction(string factionName)
	{
		if (factionName == "Russians" || factionName == "RussianZ" || factionName == "East" || factionName == "east")
			return new eAIFactionRussianZ();

		if (factionName == "Americans" || factionName == "AmericanZ" || factionName == "West" || factionName == "west")
			return new eAIFactionAmericanZ();

		if (factionName == "Mercenaries" || factionName == "mercenaries")
			return new eAIFactionMercenaries();

		if (factionName == "Guards" || factionName == "guards")
			return new eAIFactionGuards();

		if (factionName == "Passive" || factionName == "passive")
			return new eAIFactionPassive();

		return new eAIFactionRaiders();
	}

	protected static int ApplyExpansionLoadout(eAIBase ai, DZBBC_AIFactionConfig faction)
	{
		if (!ai || !faction)
			return 0;

		if (!ai.GetInventory())
			return 0;

		int added = 0;
		string loadoutFile = faction.ExpansionLoadoutFile;
		if (loadoutFile == "")
			loadoutFile = GetDefaultExpansionLoadoutFile(faction.ExpansionFaction);

		if (loadoutFile != "")
		{
			if (ExpansionHumanLoadout.Apply(ai, loadoutFile, true))
			{
				added++;
				DZBBC_Utils.Log("Expansion AI loadout file applied " + faction.FactionName + ": " + loadoutFile);
			}
			else
			{
				DZBBC_Utils.Warn("Expansion AI loadout file failed " + loadoutFile + " for " + faction.FactionName);
			}
		}

		added += ApplyExpansionExtraItems(ai, faction);
		return added;
	}

	protected static int ApplyExpansionExtraItems(eAIBase ai, DZBBC_AIFactionConfig faction)
	{
		if (!ai || !faction || !faction.ExpansionLoadout || faction.ExpansionLoadout.Count() == 0)
			return 0;

		int added = 0;
		foreach (string itemName: faction.ExpansionLoadout)
		{
			if (itemName == "")
				continue;

			if (!DZBBC_Utils.IsCfgInventoryType(itemName))
			{
				DZBBC_Utils.Warn("Expansion AI loadout item missing in config: " + itemName);
				continue;
			}

			EntityAI item = ai.GetInventory().CreateInInventory(itemName);
			if (!item)
				item = ai.GetInventory().CreateAttachment(itemName);

			if (item)
				added++;
			else
				DZBBC_Utils.Warn("Expansion AI loadout item failed " + itemName + " for " + faction.FactionName);
		}

		if (added > 0)
			DZBBC_Utils.Log("Expansion AI extra loadout applied " + faction.FactionName + ": " + added.ToString() + " item(s).");

		return added;
	}

	protected static string GetDefaultExpansionLoadoutFile(string factionName)
	{
		if (factionName == "Russians" || factionName == "RussianZ" || factionName == "East" || factionName == "east")
			return "EastLoadout";

		if (factionName == "Americans" || factionName == "AmericanZ" || factionName == "West" || factionName == "west")
			return "WestLoadout";

		if (factionName == "Mercenaries" || factionName == "mercenaries")
			return "PoliceLoadout";

		if (factionName == "Guards" || factionName == "guards")
			return "TTSKOLoadout";

		return "BanditLoadout";
	}
#endif
}
