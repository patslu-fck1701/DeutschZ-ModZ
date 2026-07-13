modded class ActionConstructor
{
	override void RegisterActions(TTypenameArray actions)
	{
		super.RegisterActions(actions);
		actions.Insert(ActionDZBGZ_UseCardReader);
		Print(DZBGZ_LOG_PREFIX + "Card reader action registered.");
	}
}
