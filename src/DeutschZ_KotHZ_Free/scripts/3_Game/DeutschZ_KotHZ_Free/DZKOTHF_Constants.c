class DZKOTHF_Constants
{
	static const string VERSION = "1.0.0-phase-b";
	static const string LOG_PREFIX = "[DeutschZ KotHZ Free] ";
	static const string PROFILE_ROOT = "$profile:DeutschZ";
	static const string PROFILE_MOD = "$profile:DeutschZ/KotHZ_Free";
	static const string SETTINGS_PATH = "$profile:DeutschZ/KotHZ_Free/KotHZFreeSettings.json";
	static const string FLAGPOLE_CLASSNAME = "DZKOTHF_EventFlagpole";
	static const string FLAG_CLASSNAME = "DZKOTHF_EventFlag";
}

class DZKOTHF_Log
{
	static void Info(string message)
	{
		Print(DZKOTHF_Constants.LOG_PREFIX + message);
	}

	static void Warning(string message)
	{
		Print(DZKOTHF_Constants.LOG_PREFIX + "WARNING: " + message);
	}

	static void Error(string message)
	{
		Print(DZKOTHF_Constants.LOG_PREFIX + "ERROR: " + message);
	}
}
