class DZEV_ClientProgressState
{
	protected static bool s_Visible;
	protected static int s_Mode;
	protected static string s_Label;
	protected static float s_Current;
	protected static float s_Max;
	protected static int s_Revision;
	protected static float s_LastUpdateTime;

	static void Set(int mode, string label, float current, float max)
	{
		if (mode == DZEV_ProgressModes.HIDE)
		{
			Hide();
			return;
		}

		s_Visible = true;
		s_Mode = mode;
		s_Label = label;
		s_Current = current;
		s_Max = max;
		if (GetGame())
			s_LastUpdateTime = GetGame().GetTickTime();
		s_Revision++;
	}

	static void Hide()
	{
		s_Visible = false;
		s_Mode = DZEV_ProgressModes.HIDE;
		s_Label = "";
		s_Current = 0.0;
		s_Max = 0.0;
		s_LastUpdateTime = 0.0;
		s_Revision++;
	}

	static bool IsVisible()
	{
		return s_Visible;
	}

	static int GetMode()
	{
		return s_Mode;
	}

	static string GetLabel()
	{
		return s_Label;
	}

	static float GetCurrent()
	{
		return s_Current;
	}

	static float GetMax()
	{
		return s_Max;
	}

	static int GetRevision()
	{
		return s_Revision;
	}

	static bool IsStale(float maximumAgeSeconds)
	{
		if (!s_Visible || !GetGame() || s_LastUpdateTime <= 0.0)
			return false;

		return (GetGame().GetTickTime() - s_LastUpdateTime) > maximumAgeSeconds;
	}
}
