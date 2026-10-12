class CfgPatches
{
 class deutschz_carraidez
 {
  units[]={}; weapons[]={}; requiredVersion=0.1;
  requiredAddons[]={"DZ_Data","DZ_Scripts","DayZExpansion_AI_Scripts","DZ_Vehicles_Wheeled","deutschz_eventschedulerz"};
 };
};
class CfgMods
{
 class deutschz_carraidez
 {
  dir="deutschz_carraidez"; name="DeutschZ CarRaidZ"; type="mod"; author="DeutschZ"; version="0.1.0";
  dependencies[]={"Game","World","Mission"};
  class defs
  {
   class gameScriptModule {value="";files[]={"deutschz_carraidez/scripts/3_game"};};
   class worldScriptModule {value="";files[]={"deutschz_carraidez/scripts/4_world"};};
   class missionScriptModule {value="";files[]={"deutschz_carraidez/scripts/5_mission"};};
  };
 };
};
