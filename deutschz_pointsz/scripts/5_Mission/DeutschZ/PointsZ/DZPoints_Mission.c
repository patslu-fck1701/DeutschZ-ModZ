modded class MissionServer
{
	override void OnInit()
	{
		super.OnInit();
		DZPoints_Service.Initialize();
		DZPoints_TopGames.Start();
		Print("[DeutschZ PointsZ] INITIALIZED");
	}
}
