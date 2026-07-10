const int DZKOTHG_RPC_MARKER_SYNC = 92734001;

class DZKOTHG_State
{
	static const int IDLE = 0;
	static const int PRE_NOTIFY = 1;
	static const int WAITING_FOR_TERMINAL = 2;
	static const int HACKING = 3;
	static const int ACTIVATED = 4;
	static const int FOXY_RUNNING = 5;
	static const int FINISHED = 6;
	static const int ABORTED = 7;
}

class DZKOTHG_MarkerData
{
	string UID;
	string Text;
	vector Pos;
	int Color;
	string IconPath;
	bool Remove;

	void DZKOTHG_MarkerData()
	{
		UID = "";
		Text = "";
		Pos = "0 0 0";
		Color = ARGB(255, 255, 196, 0);
		IconPath = "dz/gear/navigation/data/map_transmitter_ca.paa";
		Remove = false;
	}
}
