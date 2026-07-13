class DZCRZ_ProfilePaths
{
	static const string ROOT = "$profile:DeutschZ-System/DeutschZ_CriminalZ";
	static const string CONFIG = "$profile:DeutschZ-System/DeutschZ_CriminalZ/Config";
	static const string DATA = "$profile:DeutschZ-System/DeutschZ_CriminalZ/Data";
	static const string PERSISTENCE = "$profile:DeutschZ-System/DeutschZ_CriminalZ/Persistence";
	static const string RUNTIME = "$profile:DeutschZ-System/DeutschZ_CriminalZ/Runtime";
	static const string LOG_ROOT = "$profile:DeutschZ-System/LogZ/DeutschZ_CriminalZ";
	static const string MAIN_CONFIG = "$profile:DeutschZ-System/DeutschZ_CriminalZ/Config/DeutschZ_CriminalZ.json";
	static const string STORE_PLACEMENT_CONFIG = "$profile:DeutschZ-System/DeutschZ_CriminalZ/Config/StorePlacementSettings.json";
	static const string ATM_POSITIONS = "$profile:DeutschZ-System/DeutschZ_CriminalZ/Data/atm_positions.json";
	static const string COOLDOWNS = "$profile:DeutschZ-System/DeutschZ_CriminalZ/Persistence/cooldowns.json";
	static const string RUNTIME_STATE = "$profile:DeutschZ-System/DeutschZ_CriminalZ/Runtime/runtime.json";
	static const string LOG_FILE = "$profile:DeutschZ-System/LogZ/DeutschZ_CriminalZ/criminalz.log";

	static void Ensure()
	{
		DZCRZ_EnsureDirectory("$profile:DeutschZ-System");
		DZCRZ_EnsureDirectory(ROOT);
		DZCRZ_EnsureDirectory(CONFIG);
		DZCRZ_EnsureDirectory(DATA);
		DZCRZ_EnsureDirectory(PERSISTENCE);
		DZCRZ_EnsureDirectory(RUNTIME);
		DZCRZ_EnsureDirectory("$profile:DeutschZ-System/LogZ");
		DZCRZ_EnsureDirectory(LOG_ROOT);
	}

	protected static void DZCRZ_EnsureDirectory(string path)
	{
		if (!FileExist(path))
			MakeDirectory(path);
	}
}
