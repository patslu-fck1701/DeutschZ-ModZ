class DZKOTHF_Constants
{
	static const string VERSION = "1.0.0-phase-a";
	static const string LOG_PREFIX = "[DeutschZ KotHZ Free] ";
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
