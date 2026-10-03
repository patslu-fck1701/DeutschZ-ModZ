class DZKOTHF_LocationSetting
{
	bool Enabled;
	string Name;
	ref array<float> Position;
	ref array<float> Orientation;
	float CaptureRadius;

	void DZKOTHF_LocationSetting(string name = "VMC", float x = 4552.346680, float y = 317.997314, float z = 8350.974609, float ox = 0.0, float oy = 0.0, float oz = 0.0, float radius = 25.0)
	{
		Enabled = true;
		Name = name;
		Position = {x, y, z};
		Orientation = {ox, oy, oz};
		CaptureRadius = radius;
	}

	bool IsValid()
	{
		return Enabled && Name != "" && Position && Position.Count() == 3;
	}

	vector GetPosition()
	{
		return Vector(Position[0], Position[1], Position[2]);
	}

	vector GetOrientation()
	{
		if (!Orientation || Orientation.Count() != 3)
			return vector.Zero;
		return Vector(Orientation[0], Orientation[1], Orientation[2]);
	}
}

class DZKOTHF_LocationsSettings
{
	ref array<ref DZKOTHF_LocationSetting> Locations;

	void DZKOTHF_LocationsSettings()
	{
		Locations = new array<ref DZKOTHF_LocationSetting>;
		Locations.Insert(new DZKOTHF_LocationSetting("Myshkino Military", 1178.527222, 184.563675, 7184.000977, 0, 0, 0, 35));
		Locations.Insert(new DZKOTHF_LocationSetting("Tisy Military", 1669.084961, 451.845764, 14175.852539, 0, 0, 0, 40));
		Locations.Insert(new DZKOTHF_LocationSetting("Tisy Airfield", 1061.661621, 454.373505, 14635.067383, 0, 0, 0, 40));
		Locations.Insert(new DZKOTHF_LocationSetting("Grozowny (Küste)", 3680.81958, 363.682251, 14819.65625, 0, 0, 0, 25));
		Locations.Insert(new DZKOTHF_LocationSetting("Novaya Komando Zentrale", 4344.966797, 214.68692, 13947.355469, 0, 0, 0, 35));
		Locations.Insert(new DZKOTHF_LocationSetting("Balota Airfield (Küste)", 5089.147461, 9.475588, 2357.387695, 62.799774, 0, 0, 30));
		Locations.Insert(new DZKOTHF_LocationSetting("Grozovny Military", 3771.815918, 362.523956, 14822.856445, 0, 0, 0, 35));
		Locations.Insert(new DZKOTHF_LocationSetting("Gorka Military", 9711.21875, 298.794281, 8888.484375, 0, 0, 0, 35));
		Locations.Insert(new DZKOTHF_LocationSetting("Mountain Air Base", 567.143188, 280.748383, 8150.26123, 109.952057, -2.801387, -1.016456, 40));
		Locations.Insert(new DZKOTHF_LocationSetting("Radio Zenith", 8229.395508, 469.852234, 9026.976563, 62.799782, 0, 0, 35));
		Locations.Insert(new DZKOTHF_LocationSetting("Lopatino Military (Helicrash)", 2936.863525, 270.877014, 9680.298828, 0, 0, 0, 35));
		Locations.Insert(new DZKOTHF_LocationSetting("Downed Helicopter", 144.311996, 319.558014, 8073.836914, 0, 0, 0, 25));
		Locations.Insert(new DZKOTHF_LocationSetting("Skalisty Komando Zentrale", 13895.150391, 31.665937, 2939.197021, 0, 0, 0, 35));
		Locations.Insert(new DZKOTHF_LocationSetting("LunaPark (Küste)", 11324.776367, 1.691232, 2249.175049, 0, 0, 0, 25));
		Locations.Insert(new DZKOTHF_LocationSetting("NWAF Komando Zentrale", 4206.186035, 339.17099, 10765.483398, 0, 0, 0, 40));
		Locations.Insert(new DZKOTHF_LocationSetting("Outpost01", 130.171982, 195.248672, 6677.341797, 0, 0, 0, 35));
		Locations.Insert(new DZKOTHF_LocationSetting("Severograd Krater (Küste) #2", 8664.78125, 106.532196, 13278.74707, 0, -1.765899, 0, 30));
		Locations.Insert(new DZKOTHF_LocationSetting("Kamensk Military", 7997.521973, 339.32782, 14635.450195, 0, 0, 0, 35));
		Locations.Insert(new DZKOTHF_LocationSetting("Pulkovo Military", 5315.332031, 306.854187, 5515.727539, 0.356049, -3.680192, -5.088398, 35));
		Locations.Insert(new DZKOTHF_LocationSetting("Myshkino Komando Zentrale", 1754.822632, 289.957794, 7701.331055, 11.759870, 0, 0, 35));
		Locations.Insert(new DZKOTHF_LocationSetting("Severograd Krater (Küste)", 8631.748047, 120.071281, 13453.712891, 0, 0, 0, 30));
		Locations.Insert(new DZKOTHF_LocationSetting("Krasnostav Airfield", 12162.132813, 140.07872, 12505.953125, 0, 0, 0, 40));
		Locations.Insert(new DZKOTHF_LocationSetting("VMC Military #2", 4639.137695, 321.088898, 8381.387695, 0, 0, 0, 35));
		Locations.Insert(new DZKOTHF_LocationSetting("VMC Military", 4552.908203, 318.301666, 8350.995117, 0, 0, 0, 35));
		Locations.Insert(new DZKOTHF_LocationSetting("Tisy Radio Station", 554.090637, 502.260193, 13635.074219, 0, 0, 0, 35));
		Locations.Insert(new DZKOTHF_LocationSetting("NWAF Zeltstadt", 4253.635742, 338.894348, 11089.924805, 0, 0, 0, 40));
		Locations.Insert(new DZKOTHF_LocationSetting("Lopatino Military", 2928.781006, 271.016968, 9681.536133, 0, 0, 0, 35));
		Locations.Insert(new DZKOTHF_LocationSetting("YRAP Military Bahnhof", 1244.543823, 173.040649, 5850.441895, 0, 0, 0, 35));
	}
}

class DZKOTHF_LocationHistory
{
	int LastIndex;
	string LastName;

	void DZKOTHF_LocationHistory()
	{
		LastIndex = -1;
		LastName = "";
	}
}

class DZKOTHF_LocationLoader
{
	static ref DZKOTHF_LocationsSettings LoadLocations()
	{
		DZKOTHF_ProfilePaths.EnsureDirectories();
		ref DZKOTHF_LocationsSettings locations = new DZKOTHF_LocationsSettings;
		string errorMessage;
		bool writeLocations;
		if (FileExist(DZKOTHF_Constants.LOCATIONS_PATH))
		{
			if (!JsonFileLoader<ref DZKOTHF_LocationsSettings>.LoadFile(DZKOTHF_Constants.LOCATIONS_PATH, locations, errorMessage) || !locations || !locations.Locations)
			{
				DZKOTHF_Log.Error("Locations are corrupt and will be regenerated from defaults. " + errorMessage);
				if (!FileExist(DZKOTHF_Constants.CORRUPT_LOCATIONS_BACKUP_PATH))
				{
					if (CopyFile(DZKOTHF_Constants.LOCATIONS_PATH, DZKOTHF_Constants.CORRUPT_LOCATIONS_BACKUP_PATH))
						DZKOTHF_Log.Warning("Corrupt locations were preserved at " + DZKOTHF_Constants.CORRUPT_LOCATIONS_BACKUP_PATH + ".");
					else
						DZKOTHF_Log.Error("Corrupt locations could not be copied to the backup path before regeneration.");
				}
				locations = new DZKOTHF_LocationsSettings;
				writeLocations = true;
			}
			else
			{
				DZKOTHF_Log.Info("Valid locations loaded without rewriting: " + DZKOTHF_Constants.LOCATIONS_PATH + ".");
			}
		}
		else
		{
			writeLocations = true;
		}

		if (writeLocations)
		{
			if (!JsonFileLoader<ref DZKOTHF_LocationsSettings>.SaveFile(DZKOTHF_Constants.LOCATIONS_PATH, locations, errorMessage))
				DZKOTHF_Log.Error("Locations could not be written. " + errorMessage);
			else
				DZKOTHF_Log.Info("Locations written once because the file was missing or corrupt: " + DZKOTHF_Constants.LOCATIONS_PATH + ".");
		}
		return locations;
	}

	static ref DZKOTHF_LocationHistory LoadHistory()
	{
		DZKOTHF_ProfilePaths.EnsureDirectories();
		ref DZKOTHF_LocationHistory history = new DZKOTHF_LocationHistory;
		string errorMessage;
		if (FileExist(DZKOTHF_Constants.LOCATION_STATE_PATH))
		{
			if (!JsonFileLoader<ref DZKOTHF_LocationHistory>.LoadFile(DZKOTHF_Constants.LOCATION_STATE_PATH, history, errorMessage) || !history)
			{
				DZKOTHF_Log.Warning("Location history could not be loaded; direct-repeat protection starts fresh. " + errorMessage);
				history = new DZKOTHF_LocationHistory;
			}
		}
		return history;
	}

	static void SaveHistory(DZKOTHF_LocationHistory history)
	{
		if (!history)
			return;
		string errorMessage;
		if (!JsonFileLoader<ref DZKOTHF_LocationHistory>.SaveFile(DZKOTHF_Constants.LOCATION_STATE_PATH, history, errorMessage))
			DZKOTHF_Log.Warning("Location history could not be saved. " + errorMessage);
	}
}
