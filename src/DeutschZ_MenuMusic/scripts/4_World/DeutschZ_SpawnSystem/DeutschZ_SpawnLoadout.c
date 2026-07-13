class DeutschZ_SpawnLoadoutItem
{
	string ClassName;
	int Quantity;
	ref array<string> Attachments;

	void DeutschZ_SpawnLoadoutItem()
	{
		ClassName = "";
		Quantity = -1;
		Attachments = new array<string>;
	}

	static DeutschZ_SpawnLoadoutItem Create(string className, int quantity = -1)
	{
		DeutschZ_SpawnLoadoutItem item = new DeutschZ_SpawnLoadoutItem;
		item.ClassName = className;
		item.Quantity = quantity;
		return item;
	}
}

class DeutschZ_SpawnLoadout
{
	string Id;
	string DisplayName;
	bool Enabled;
	ref array<ref DeutschZ_SpawnLoadoutItem> Items;

	void DeutschZ_SpawnLoadout()
	{
		Id = "survivor_basic";
		DisplayName = "Ueberlebender";
		Enabled = true;
		Items = new array<ref DeutschZ_SpawnLoadoutItem>;
	}

	static bool Apply(PlayerBase player, DeutschZ_SpawnLoadoutsFile config, string loadoutId)
	{
		if (!player || !config || !config.Loadouts)
			return false;

		DeutschZ_SpawnLoadout selected;
		foreach (DeutschZ_SpawnLoadout loadout: config.Loadouts)
		{
			if (loadout && loadout.Enabled && loadout.Id == loadoutId)
			{
				selected = loadout;
				break;
			}
		}

		if (!selected)
			return false;

		foreach (DeutschZ_SpawnLoadoutItem entry: selected.Items)
		{
			if (!entry || entry.ClassName == "")
				continue;
			if (HasItem(player, entry.ClassName))
				continue;
			if (!GetGame().ConfigIsExisting("CfgVehicles " + entry.ClassName) && !GetGame().ConfigIsExisting("CfgWeapons " + entry.ClassName) && !GetGame().ConfigIsExisting("CfgMagazines " + entry.ClassName))
			{
				DZSPAWN_Log.Warn("Loadout classname missing: " + entry.ClassName);
				continue;
			}

			EntityAI created = player.GetInventory().CreateInInventory(entry.ClassName);
			if (!created)
			{
				DZSPAWN_Log.Warn("Could not create loadout item: " + entry.ClassName);
				continue;
			}

			ItemBase item = ItemBase.Cast(created);
			if (item && entry.Quantity > 0 && item.HasQuantity())
				item.SetQuantity(entry.Quantity);

			if (entry.Attachments)
			{
				foreach (string attachment: entry.Attachments)
				{
					if (attachment != "" && created.GetInventory())
						created.GetInventory().CreateAttachment(attachment);
				}
			}
		}

		DZSPAWN_Log.Info("Applied loadout " + selected.Id);
		return true;
	}

	protected static bool HasItem(PlayerBase player, string className)
	{
		if (!player || !player.GetInventory())
			return false;
		ref array<EntityAI> items = new array<EntityAI>;
		player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
		foreach (EntityAI item: items)
		{
			if (item && item.GetType() == className)
				return true;
		}
		return false;
	}
}

class DeutschZ_SpawnLoadoutsFile
{
	int Version;
	string DefaultLoadout;
	ref array<ref DeutschZ_SpawnLoadout> Loadouts;

	void DeutschZ_SpawnLoadoutsFile()
	{
		Version = 1;
		DefaultLoadout = "survivor_basic";
		Loadouts = new array<ref DeutschZ_SpawnLoadout>;

		DeutschZ_SpawnLoadout loadout = new DeutschZ_SpawnLoadout;
		loadout.Items.Insert(DeutschZ_SpawnLoadoutItem.Create("Rag", 4));
		loadout.Items.Insert(DeutschZ_SpawnLoadoutItem.Create("Apple"));

		DeutschZ_SpawnLoadoutItem phone = DeutschZ_SpawnLoadoutItem.Create("NoxZ_Phone_Device_IPhone17");
		phone.Attachments.Insert("NoxZ_PhoneBattery");
		loadout.Items.Insert(phone);
		loadout.Items.Insert(DeutschZ_SpawnLoadoutItem.Create("DeutschZ_Regeln"));
		Loadouts.Insert(loadout);
	}

	static ref DeutschZ_SpawnLoadoutsFile Load()
	{
		DeutschZ_SpawnStorage.EnsureDirectories();
		ref DeutschZ_SpawnLoadoutsFile config = new DeutschZ_SpawnLoadoutsFile;
		string error;
		if (!FileExist(DZSPAWN_Paths.LOADOUTS))
		{
			JsonFileLoader<ref DeutschZ_SpawnLoadoutsFile>.SaveFile(DZSPAWN_Paths.LOADOUTS, config, error);
			DZSPAWN_Log.Info("Created Loadouts.json");
			return config;
		}

		if (!JsonFileLoader<ref DeutschZ_SpawnLoadoutsFile>.LoadFile(DZSPAWN_Paths.LOADOUTS, config, error))
			DZSPAWN_Log.Error("Loadouts.json invalid, defaults active: " + error);

		return config;
	}
}
