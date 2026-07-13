modded class ActionConstructor
{
	override void RegisterActions(TTypenameArray actions)
	{
		super.RegisterActions(actions);
		actions.Insert(ActionDZECZ_StartEvent);
		actions.Insert(ActionDZECZ_ActivateStation);
		actions.Insert(ActionDZECZ_PrepareBattery);
		actions.Insert(ActionDZECZ_ActivateFinalTerminal);
		actions.Insert(ActionDZECZ_ShutdownSignal);
		actions.Insert(ActionDZECZ_SendSignal);
		Print("[DZECZ] EclipseZ actions registered.");
	}
}
