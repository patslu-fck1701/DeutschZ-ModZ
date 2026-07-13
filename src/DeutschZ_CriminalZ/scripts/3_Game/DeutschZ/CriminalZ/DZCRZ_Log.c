class DZCRZ_Log
{
	static void Info(string message)
	{
		Write("INFO", message);
	}

	static void Warn(string message)
	{
		Write("WARN", message);
	}

	static void Error(string message)
	{
		Write("ERROR", message);
	}

	static void Debug(string message)
	{
		if (DZCRZ_RuntimeFlags.DebugEnabled)
			Write("DEBUG", message);
	}

	protected static void Write(string level, string message)
	{
		string line = DZCRZ_Const.PREFIX + "[" + level + "] " + message;
		Print(line);
		if (!GetGame() || !GetGame().IsServer())
			return;
		DZCRZ_ProfilePaths.Ensure();
		FileHandle file = OpenFile(DZCRZ_ProfilePaths.LOG_FILE, FileMode.APPEND);
		if (file != 0)
		{
			FPrintln(file, line);
			CloseFile(file);
		}
	}
}

class DZCRZ_RuntimeFlags
{
	static bool DebugEnabled = true;
}
