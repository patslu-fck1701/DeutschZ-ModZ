class DZKOTH_NotificationClient
{
	static void OnWarningRPC(ParamsReadContext ctx)
	{
		string title;
		string text;
		float time;

		if (!ctx.Read(title))
			return;
		if (!ctx.Read(text))
			return;
		if (!ctx.Read(time))
			return;

		NotificationSystem.AddNotificationExtended(time, title, text, "");
	}

	static void OnKeycardSignalRPC(ParamsReadContext ctx)
	{
		string playerName;
		vector pos;

		if (!ctx.Read(playerName))
			return;
		if (!ctx.Read(pos))
			return;

		string text = playerName + " hat die BattlegroundZ Keycard bei X/Z " + DZKOTH_Utils.FormatXZ(pos) + " aufgenommen.";
		NotificationSystem.AddNotificationExtended(10.0, "BattlegroundZ Signal", text, "");
	}
}
