class DZKOTH_AdminDebug
{
	static void HandleRPC(PlayerIdentity sender, PlayerBase player, ParamsReadContext ctx)
	{
		if (!GetGame() || !GetGame().IsServer())
			return;

		string command;
		if (!ctx.Read(command))
			return;

		if (!sender || !DZKOTH_EventManager.GetInstance().CanUseAdminCommand(sender.GetId()))
		{
			if (player)
				DZKOTH_ServerRPC.SendWarning(player, "DZKOTH Admin", "Keine Berechtigung oder DebugCommandsEnabled=false.", 4.0);
			return;
		}

		Execute(command, player);
	}

	static void Execute(string rawCommand, PlayerBase player)
	{
		string command = rawCommand.Trim();
		command.ToLower();

		array<string> parts = new array<string>;
		command.Split(" ", parts);
		if (!parts || parts.Count() == 0)
			return;

		if (IsCommandPrefix(parts.Get(0)))
			parts.Remove(0);

		if (parts.Count() == 0 || parts.Get(0) == "state")
		{
			SendAdminReply(player, State());
			return;
		}

		string action = parts.Get(0);
		if (action == "forceboss")
		{
			ForceBoss();
			SendAdminReply(player, "ForceBoss ausgefuehrt.");
		}
		else if (action == "capture" || action == "teststart" || action == "activate")
		{
			if (DZKOTH_EventManager.GetInstance().DebugStartCapture(player))
				SendAdminReply(player, "Capture-Teststart ausgefuehrt.");
			else
				SendAdminReply(player, "Capture-Teststart fehlgeschlagen. Spieler/Instanz pruefen.");
		}
		else if (action == "cleanup")
		{
			Cleanup();
			SendAdminReply(player, "Cleanup ausgefuehrt.");
		}
		else if (action == "start")
		{
			int locationIndex = ResolveLocationIndex(parts);
			DZKOTH_EventManager.GetInstance().StartEvent(locationIndex);
			SendAdminReply(player, "StartEvent ausgefuehrt fuer index " + locationIndex.ToString());
		}
		else if (action == "stop")
		{
			DZKOTH_EventManager.GetInstance().StopEvent();
			SendAdminReply(player, "StopEvent ausgefuehrt.");
		}
		else if ((action == "progress" || action == "setprogress") && parts.Count() >= 2)
		{
			float progress = parts.Get(1).ToFloat();
			SetProgress(progress);
			SendAdminReply(player, "Progress gesetzt auf " + progress.ToString());
		}
		else
		{
			SendAdminReply(player, "Befehle: state, start [airfield|0], capture, teststart, activate, forceboss, cleanup, stop, setprogress <0-100>");
		}
	}

	static void ForceBoss()
	{
		DZKOTH_EventManager.GetInstance().DebugForceBoss();
		DZKOTH_Utils.Log("Admin debug: ForceBoss called.");
	}

	static void SetProgress(float progress)
	{
		DZKOTH_EventManager.GetInstance().DebugSetProgress(progress);
		DZKOTH_Utils.Log("Admin debug: SetProgress " + progress.ToString());
	}

	static void Cleanup()
	{
		DZKOTH_EventManager.GetInstance().CleanupEvent();
		DZKOTH_Utils.Log("Admin debug: Cleanup called.");
	}

	static string State()
	{
		return DZKOTH_EventManager.GetInstance().GetDebugState();
	}

	protected static void SendAdminReply(PlayerBase player, string text)
	{
		if (player)
			DZKOTH_ServerRPC.SendWarning(player, "DZKOTH Admin", text, 5.0);

		DZKOTH_Utils.Log("Admin debug reply: " + text);
	}

	protected static bool IsCommandPrefix(string token)
	{
		return token == DZKOTH_Const.ADMIN_COMMAND_PREFIX || token == DZKOTH_Const.ADMIN_COMMAND_PREFIX_ALT;
	}

	protected static int ResolveLocationIndex(array<string> parts)
	{
		if (!parts || parts.Count() < 2)
			return 0;

		string token = parts.Get(1);
		token.ToLower();
		if (token == "airfield" || token == "nwaf")
			return 0;

		if (token == "vmc")
			return 1;

		int index = token.ToInt();
		if (index < 0)
			return 0;

		return index;
	}
}
