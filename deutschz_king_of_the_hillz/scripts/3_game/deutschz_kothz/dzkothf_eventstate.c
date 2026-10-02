class DZKOTHF_EventState
{
	static const int IDLE = 0;
	static const int ANNOUNCED = 1;
	static const int ACTIVE = 2;
	static const int CAPTURE_COMPLETE = 3;
	static const int REWARD = 4;
	static const int CLEANUP = 5;

	static string ToString(int state)
	{
		switch (state)
		{
			case IDLE:
				return "IDLE";
			case ANNOUNCED:
				return "ANNOUNCED";
			case ACTIVE:
				return "ACTIVE";
			case CAPTURE_COMPLETE:
				return "CAPTURE_COMPLETE";
			case REWARD:
				return "REWARD";
			case CLEANUP:
				return "CLEANUP";
		}

		return "UNKNOWN(" + state.ToString() + ")";
	}
}
