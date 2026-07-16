class DZKOTHF_LocationSetting
{
	bool Enabled;
	string Name;
	ref array<float> Position;
	ref array<float> Orientation;
	float CaptureRadius;

	void DZKOTHF_LocationSetting(string name = "VMC", float x = 4552.346680, float y = 317.997314, float z = 8350.974609)
	{
		Enabled = true;
		Name = name;
		Position = {x, y, z};
		Orientation = {0.0, 0.0, 0.0};
		CaptureRadius = 25.0;
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
		Locations.Insert(new DZKOTHF_LocationSetting("Kamensk", 8000.134766, 339.335907, 14637.155273));
		Locations.Insert(new DZKOTHF_LocationSetting("VMC", 4552.346680, 317.997314, 8350.974609));
		Locations.Insert(new DZKOTHF_LocationSetting("Grozovny", 3780.909912, 361.331116, 14801.656250));
		Locations.Insert(new DZKOTHF_LocationSetting("Lopatino", 2934.925049, 270.860352, 9679.623047));
		Locations.Insert(new DZKOTHF_LocationSetting("Mishkyno", 1101.278931, 182.950562, 7196.391602));
		Locations.Insert(new DZKOTHF_LocationSetting("NWAF", 4905.860352, 338.039642, 9430.402344));
		Locations.Insert(new DZKOTHF_LocationSetting("Tri-Kresta", 315.089233, 295.005219, 9339.625977));
		Locations.Insert(new DZKOTHF_LocationSetting("YRAP", 1246.114502, 173.139984, 5848.398926));
		Locations.Insert(new DZKOTHF_LocationSetting("Gorka Military", 9707.442383, 299.003113, 8890.658203));
		Locations.Insert(new DZKOTHF_LocationSetting("Krasnostav Airfield", 12138.583984, 140.082520, 12658.378906));
		Locations.Insert(new DZKOTHF_LocationSetting("Pustoshka Military", 3630.406494, 333.893066, 6921.706543));
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
		if (FileExist(DZKOTHF_Constants.LOCATIONS_PATH))
		{
			if (!JsonFileLoader<ref DZKOTHF_LocationsSettings>.LoadFile(DZKOTHF_Constants.LOCATIONS_PATH, locations, errorMessage) || !locations || !locations.Locations)
			{
				DZKOTHF_Log.Error("Locations could not be loaded; defaults are used. " + errorMessage);
				locations = new DZKOTHF_LocationsSettings;
			}
		}

		if (!JsonFileLoader<ref DZKOTHF_LocationsSettings>.SaveFile(DZKOTHF_Constants.LOCATIONS_PATH, locations, errorMessage))
			DZKOTHF_Log.Error("Locations could not be saved. " + errorMessage);
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
