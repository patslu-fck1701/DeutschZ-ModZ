modded class MissionServer
{
	override void OnMissionStart()
	{
		super.OnMissionStart();
		DZCourierZ_Manager.Get().Start();
	}

	override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
	{
		super.InvokeOnConnect(player, identity);
		DZCourierZ_Manager.Get().CleanupPlayerMissionItems(player);
	}
}

modded class MissionGameplay
{
	override UIScriptedMenu CreateScriptedMenu(int id)
	{
		if (id == DZCourierZ_Constants.REWARD_MENU_ID)
			return new DZCourierZ_RewardMenu;
		return super.CreateScriptedMenu(id);
	}
}
