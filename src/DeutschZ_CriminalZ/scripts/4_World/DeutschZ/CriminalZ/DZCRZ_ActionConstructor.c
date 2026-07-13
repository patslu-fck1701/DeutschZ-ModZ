modded class ActionConstructor
{
	override void RegisterActions(TTypenameArray actions)
	{
		super.RegisterActions(actions);
		actions.Insert(ActionDZCRZ_RobATM);
		actions.Insert(ActionDZCRZ_HackVehicle);
	}
}
