class DZKOTHF_SmokeState
{
	static const int NONE = -1;
	static const int WHITE = 0;
	static const int GREEN = 1;
	static const int RED = 2;

	static string ToString(int state)
	{
		switch (state)
		{
			case WHITE:
				return "WHITE";
			case GREEN:
				return "GREEN";
			case RED:
				return "RED";
		}

		return "NONE";
	}
}
