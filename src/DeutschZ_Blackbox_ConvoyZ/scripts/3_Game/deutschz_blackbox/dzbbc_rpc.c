const int DZBBC_RPC_BASE = 536800;
const int DZBBC_RPC_MARKER_SYNC = DZBBC_RPC_BASE + 1;
const int DZBBC_RPC_HUD_SYNC = DZBBC_RPC_BASE + 2;
const int DZBBC_RPC_NOTIFICATION = DZBBC_RPC_BASE + 3;
const int DZBBC_RPC_CLIENT_FX = DZBBC_RPC_BASE + 4;
const int DZBBC_RPC_ADMIN_COMMAND = DZBBC_RPC_BASE + 5;

const int DZBBC_HUD_HIDE = 0;
const int DZBBC_HUD_ENEMIES = 1;
const int DZBBC_HUD_HACK = 2;
const int DZBBC_HUD_TRANSPORT = 3;
const int DZBBC_HUD_TERMINAL = 4;
const int DZBBC_HUD_WARNING = 5;

const int DZBBC_FX_CLEAR = 0;
const int DZBBC_FX_CRASHSITE = 1;
const int DZBBC_FX_AUDIO_FRAGMENT = 2;
const int DZBBC_FX_SIGNAL_WARNING = 3;
const int DZBBC_FX_FINAL_WAVE = 4;
const int DZBBC_FX_REWARD = 5;

class DZBBC_MarkerData
{
	string UID;
	string Text;
	vector Pos;
	int Color;
	string IconPath;
	bool Remove;

	void DZBBC_MarkerData()
	{
		UID = "";
		Text = "";
		Pos = "0 0 0";
		Color = ARGB(255, 255, 210, 0);
		IconPath = DZBBC_DEFAULT_MARKER_ICON;
		Remove = false;
	}
}
