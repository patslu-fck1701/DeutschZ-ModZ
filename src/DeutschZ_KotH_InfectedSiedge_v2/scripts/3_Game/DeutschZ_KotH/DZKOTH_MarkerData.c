class DZKOTH_MarkerData
{
	string UID;
	string Text;
	vector Pos;
	int Color;
	string IconPath;
	string IconName;
	bool Marker3D;
	bool Remove;

	void DZKOTH_MarkerData()
	{
		UID = "";
		Text = "";
		Pos = "0 0 0";
		Color = ARGB(255, 255, 255, 255);
		IconPath = DZKOTH_Const.MARKER_ICON_PATH;
		IconName = DZKOTH_Const.MARKER_ICON_NAME;
		Marker3D = true;
		Remove = false;
	}
}
