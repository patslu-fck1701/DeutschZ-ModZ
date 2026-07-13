class DZBBC_ManagedAI
{
	EntityAI Unit;
	string Faction;
	float DamageMultiplier;
	bool ExpansionObjective;
}

class DZBBC_AIFactionManager
{
	protected ref array<ref DZBBC_ManagedAI> m_Units;

	void DZBBC_AIFactionManager()
	{
		m_Units = new array<ref DZBBC_ManagedAI>;
	}

	void SpawnFaction(DZBBC_AIFactionConfig faction, DZBBC_MainConfig main, vector center)
	{
		if (!faction || !main)
			return;

		array<EntityAI> expansionUnits = new array<EntityAI>;
		int expansionSpawned = DZBBC_ExpansionBridge.SpawnAIGroup(faction, main, center, expansionUnits);
		foreach (EntityAI expansionUnit: expansionUnits)
		{
			RegisterManagedUnit(expansionUnit, faction, true);
		}

		if (!faction.Types || faction.Types.Count() == 0 || faction.Count <= 0)
		{
			DZBBC_Utils.Log("Faction spawned " + faction.FactionName + ": zombies=0 expansionAI=" + expansionSpawned.ToString());
			return;
		}

		int spawned = 0;
		for (int i = 0; i < faction.Count; i++)
		{
			int typeIndex = Math.RandomInt(0, faction.Types.Count());
			string typeName = faction.Types.Get(typeIndex);
			vector pos = DZBBC_Utils.RandomPointAround(center, main.SpawnMinDistance, main.SpawnMaxDistance);
			EntityAI unit = DZBBC_Utils.CreateAISafe(typeName, pos);
			if (!unit)
				continue;

			ApplyLoadout(unit, faction);
			RegisterManagedUnit(unit, faction, false);
			spawned++;
		}

		DZBBC_Utils.Log("Faction spawned " + faction.FactionName + ": zombies=" + spawned.ToString() + "/" + faction.Count.ToString() + " expansionAI=" + expansionSpawned.ToString());
	}

	protected void RegisterManagedUnit(EntityAI unit, DZBBC_AIFactionConfig faction, bool expansionObjective)
	{
		if (!unit || !faction)
			return;

		DZBBC_ManagedAI managed = new DZBBC_ManagedAI;
		managed.Unit = unit;
		managed.Faction = faction.FactionName;
		managed.DamageMultiplier = faction.DamageMultiplier;
		managed.ExpansionObjective = expansionObjective;
		m_Units.Insert(managed);
	}

	protected int ApplyLoadout(EntityAI unit, DZBBC_AIFactionConfig faction)
	{
		if (!unit || !faction || !faction.Loadout || faction.Loadout.Count() == 0)
			return 0;

		if (!unit.GetInventory())
			return 0;

		int added = 0;
		foreach (string itemName: faction.Loadout)
		{
			if (itemName == "")
				continue;

			if (!DZBBC_Utils.IsCfgInventoryType(itemName))
			{
				DZBBC_Utils.Warn("AI loadout item missing in config: " + itemName);
				continue;
			}

			EntityAI item = unit.GetInventory().CreateAttachment(itemName);
			if (!item)
				item = unit.GetInventory().CreateInInventory(itemName);

			if (item)
				added++;
			else
				DZBBC_Utils.Warn("AI loadout item failed " + itemName + " for " + faction.FactionName);
		}

		if (added > 0)
			DZBBC_Utils.Log("AI loadout applied " + faction.FactionName + ": " + added.ToString() + " item(s).");

		return added;
	}

	void PrioritizePlayer(PlayerBase player)
	{
		if (!player)
			return;

		DZBBC_Utils.Log("Vanilla AI priority requested for player " + DZBBC_PlayerUtils.GetPlayerName(player) + ". Native infected AI will handle aggro.");
	}

	int CountAlive()
	{
		int count = 0;
		foreach (DZBBC_ManagedAI managed: m_Units)
		{
			if (IsManagedUnitAlive(managed))
				count++;
		}

		return count;
	}

	int CountAliveObjective()
	{
		if (HasExpansionObjective())
			return CountAliveExpansionObjective();

		return CountAlive();
	}

	int CountObjectiveTotal()
	{
		int total = CountExpansionObjectiveTotal();
		if (total > 0)
			return total;

		return CountAlive();
	}

	protected bool HasExpansionObjective()
	{
		return CountExpansionObjectiveTotal() > 0;
	}

	bool IsManagedUnit(Object unit)
	{
		EntityAI entity = EntityAI.Cast(unit);
		if (!entity)
			return false;

		foreach (DZBBC_ManagedAI managed: m_Units)
		{
			if (managed && managed.Unit == entity)
				return true;
		}

		return false;
	}

	protected int CountAliveExpansionObjective()
	{
		int count = 0;
		foreach (DZBBC_ManagedAI managed: m_Units)
		{
			if (managed && managed.ExpansionObjective && IsManagedUnitAlive(managed))
				count++;
		}

		return count;
	}

	protected bool IsManagedUnitAlive(DZBBC_ManagedAI managed)
	{
		if (!managed || !managed.Unit)
			return false;

		if (!managed.Unit.IsAlive())
			return false;

		if (managed.Unit.GetHealth("", "Health") <= 0.0)
			return false;

		return true;
	}

	protected int CountExpansionObjectiveTotal()
	{
		int count = 0;
		foreach (DZBBC_ManagedAI managed: m_Units)
		{
			if (managed && managed.ExpansionObjective)
				count++;
		}

		return count;
	}

	float GetDamageMultiplierForSource(EntityAI source)
	{
		if (!source)
			return 1.0;

		foreach (DZBBC_ManagedAI managed: m_Units)
		{
			if (managed && managed.Unit == source)
				return managed.DamageMultiplier;
		}

		return 1.0;
	}

	void Cleanup()
	{
		foreach (DZBBC_ManagedAI managed: m_Units)
		{
			if (managed && managed.Unit)
				DZBBC_Utils.DeleteObjectSafe(managed.Unit);
		}

		m_Units.Clear();
	}
}
