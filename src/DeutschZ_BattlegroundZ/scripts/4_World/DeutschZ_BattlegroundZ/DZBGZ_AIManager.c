class DZBGZ_ManagedUnit
{
	EntityAI Unit;
	string Role;
}

class DZBGZ_AIManager
{
	protected ref array<ref DZBGZ_ManagedUnit> m_Units;

	void DZBGZ_AIManager()
	{
		m_Units = new array<ref DZBGZ_ManagedUnit>;
	}

	void SpawnApproachSigns(vector center)
	{
		SpawnVanillaAI("ZmbM_SoldierNormal", center, 30.0, 65.0, "warning-infected");
		SpawnVanillaAI("ZmbM_PatrolNormal_Autumn", center, 35.0, 75.0, "warning-infected");
		SpawnVanillaAI("ZmbM_usSoldier_normal_Woodland", center, 40.0, 85.0, "warning-infected");
		DZBGZ_Utils.Log("Approach warning signs spawned.");
	}

	void SpawnBattleground(vector center)
	{
		int russian = SpawnExpansionSide(center + Vector(-35.0, 0.0, -25.0), "RussianZ", true, 3);
		int american = SpawnExpansionSide(center + Vector(35.0, 0.0, 25.0), "AmericanZ", false, 3);

		if (russian <= 0)
		{
			SpawnVanillaAI("ZmbM_SoldierNormal", center + Vector(-35.0, 0.0, -25.0), 10.0, 28.0, "RussianZ");
			SpawnVanillaAI("ZmbM_PatrolNormal_Autumn", center + Vector(-35.0, 0.0, -25.0), 10.0, 28.0, "RussianZ");
		}

		if (american <= 0)
		{
			SpawnVanillaAI("ZmbM_usSoldier_normal_Woodland", center + Vector(35.0, 0.0, 25.0), 10.0, 28.0, "AmericanZ");
			SpawnVanillaAI("ZmbM_usSoldier_Heavy_Woodland", center + Vector(35.0, 0.0, 25.0), 10.0, 28.0, "AmericanZ");
		}

		SpawnObject("M18SmokeGrenade_White", center + Vector(5.0, 0.0, 5.0), "smoke");
		SpawnObject("M18SmokeGrenade_Red", center + Vector(-8.0, 0.0, -9.0), "smoke");
		SpawnObject("Roadflare", center + Vector(14.0, 0.0, -7.0), "flare");
		DZBGZ_Utils.Log("Battleground spawned. RussianZ=" + russian.ToString() + " AmericanZ=" + american.ToString());
	}

	protected int SpawnExpansionSide(vector center, string sideName, bool east, int count)
	{
#ifdef EXPANSIONMODAI
		eAIFaction faction;
		if (east)
			faction = new eAIFactionEast();
		else
			faction = new eAIFactionWest();

		eAIGroup group = eAIGroup.CreateGroup(faction);
		if (group)
		{
			group.AddWaypoint(DZBGZ_Utils.Grounded(center));
			group.AddWaypoint(DZBGZ_Utils.RandomPointAround(center, 20.0, 45.0));
			group.AddWaypoint(DZBGZ_Utils.RandomPointAround(center, 20.0, 45.0));
			group.AddWaypoint(DZBGZ_Utils.Grounded(center));
			group.SetWaypointBehaviour(eAIWaypointBehavior.LOOP);
		}

		int spawned = 0;
		for (int i = 0; i < count; i++)
		{
			string typeName = "eAI_SurvivorM_Mirek";
			if (!east && i % 2 == 0)
				typeName = "eAI_SurvivorM_Denis";
			if (east && i % 2 == 0)
				typeName = "eAI_SurvivorM_Boris";

			if (!DZBGZ_Utils.IsCfgVehicle(typeName))
				continue;

			Object object = GetGame().CreateObjectEx(typeName, DZBGZ_Utils.RandomPointAround(center, 8.0, 26.0), ECE_SETUP | ECE_INITAI | ECE_PLACE_ON_SURFACE);
			eAIBase ai;
			if (!Class.CastTo(ai, object))
			{
				DZBGZ_Utils.DeleteObjectSafe(object);
				continue;
			}

			if (group)
				ai.SetGroup(group);

			ai.eAI_SetAccuracy(0.03, 0.12);
			ApplyLightLoadout(ai, east);
			Register(ai, sideName);
			spawned++;
		}

		return spawned;
#endif
		return 0;
	}

	protected void ApplyLightLoadout(EntityAI unit, bool east)
	{
		if (!unit || !unit.GetInventory())
			return;

		if (east)
		{
			CreateInventoryItem(unit, "MakarovIJ70");
			CreateInventoryItem(unit, "Mag_IJ70_8Rnd");
			CreateInventoryItem(unit, "Mag_IJ70_8Rnd");
			CreateInventoryItem(unit, "BandageDressing");
			return;
		}

		CreateInventoryItem(unit, "Glock19");
		CreateInventoryItem(unit, "Mag_Glock_15Rnd");
		CreateInventoryItem(unit, "Mag_Glock_15Rnd");
		CreateInventoryItem(unit, "BandageDressing");
	}

	protected void CreateInventoryItem(EntityAI unit, string className)
	{
		if (!unit || !unit.GetInventory() || className == "")
			return;

		if (!DZBGZ_Utils.IsCfgInventoryType(className))
			return;

		EntityAI item = unit.GetInventory().CreateInInventory(className);
		if (!item)
			unit.GetInventory().CreateAttachment(className);
	}

	protected EntityAI SpawnVanillaAI(string typeName, vector center, float minDistance, float maxDistance, string role)
	{
		if (!GetGame() || !DZBGZ_Utils.IsCfgVehicle(typeName))
			return null;

		Object object = GetGame().CreateObjectEx(typeName, DZBGZ_Utils.RandomPointAround(center, minDistance, maxDistance), ECE_SETUP | ECE_INITAI | ECE_PLACE_ON_SURFACE);
		EntityAI unit = EntityAI.Cast(object);
		if (!unit)
		{
			DZBGZ_Utils.DeleteObjectSafe(object);
			return null;
		}

		Register(unit, role);
		return unit;
	}

	protected Object SpawnObject(string typeName, vector pos, string role)
	{
		Object object = DZBGZ_Utils.CreateObjectSafe(typeName, pos);
		EntityAI entity = EntityAI.Cast(object);
		if (entity)
			Register(entity, role);
		return object;
	}

	protected void Register(EntityAI unit, string role)
	{
		if (!unit)
			return;

		DZBGZ_ManagedUnit managed = new DZBGZ_ManagedUnit;
		managed.Unit = unit;
		managed.Role = role;
		m_Units.Insert(managed);
	}

	bool IsManagedUnit(Object unit)
	{
		EntityAI entity = EntityAI.Cast(unit);
		if (!entity)
			return false;

		foreach (DZBGZ_ManagedUnit managed: m_Units)
		{
			if (managed && managed.Unit == entity)
				return true;
		}

		return false;
	}

	void Cleanup()
	{
		foreach (DZBGZ_ManagedUnit managed: m_Units)
		{
			if (managed && managed.Unit)
				DZBGZ_Utils.DeleteObjectSafe(managed.Unit);
		}

		m_Units.Clear();
	}
}
