const int DZBGZ_RPC_BASE = 539300;
const int DZBGZ_RPC_MARKER_SYNC = DZBGZ_RPC_BASE + 1;
const int DZBGZ_RPC_CLIENT_FX = DZBGZ_RPC_BASE + 2;
const int DZBGZ_RPC_ADMIN_COMMAND = DZBGZ_RPC_BASE + 3;

const int DZBGZ_FX_CLEAR = 0;
const int DZBGZ_FX_GLITCH_SHORT = 1;
const int DZBGZ_FX_GLITCH_MEDIUM = 2;
const int DZBGZ_FX_RIDDLE = 3;
const int DZBGZ_FX_READER_ACCEPTED = 4;

class DZBGZ_MarkerData
{
	string UID;
	string Text;
	vector Pos;
	int Color;
	string IconPath;
	bool Remove;

	void DZBGZ_MarkerData()
	{
		UID = "";
		Text = "";
		Pos = "0 0 0";
		Color = ARGB(255, 220, 20, 20);
		IconPath = DZBGZ_DEFAULT_MARKER_ICON;
		Remove = false;
	}
}
