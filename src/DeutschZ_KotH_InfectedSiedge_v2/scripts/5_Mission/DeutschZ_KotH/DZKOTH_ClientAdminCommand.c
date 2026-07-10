class DZKOTH_ClientAdminCommand
{
	static void TrySendFromChat(ChatMessageEventParams chatParams)
	{
		if (!chatParams || !GetGame() || GetGame().IsServer())
			return;

		string text = chatParams.param3;
		if (!IsAdminCommand(text))
			return;

		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (!player || !player.GetIdentity())
			return;

		if (chatParams.param2 != "" && chatParams.param2 != player.GetIdentity().GetName())
			return;

		ScriptRPC rpc = new ScriptRPC;
		rpc.Write(text);
		rpc.Send(player, DZKOTH_RPCIds.ADMIN_COMMAND, true, player.GetIdentity());
	}

	protected static bool IsAdminCommand(string text)
	{
		if (text == "")
			return false;

		if (text.IndexOf(DZKOTH_Const.ADMIN_COMMAND_PREFIX) == 0)
			return true;

		return text.IndexOf(DZKOTH_Const.ADMIN_COMMAND_PREFIX_ALT) == 0;
	}
}
