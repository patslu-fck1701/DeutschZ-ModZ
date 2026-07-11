modded class ActionConstructor
{
	override void RegisterActions(TTypenameArray actions)
	{
		super.RegisterActions(actions);
		actions.Insert(ActionDZBBC_StartBlackboxHack);
		actions.Insert(ActionDZBBC_StartTerminalDecrypt);
		Print(DZBBC_LOG_PREFIX + "Blackbox hack and terminal decrypt actions registered.");
	}
}
