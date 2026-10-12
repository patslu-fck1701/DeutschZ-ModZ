class CfgPatches
{
 class deutschz_adaptive_camouflage
 {
  units[]={}; weapons[]={}; requiredVersion=0.1;
  requiredAddons[]={"DZ_Data","DZ_Scripts","DayZExpansion_AI_Scripts"};
 };
};
class CfgMods
{
 class deutschz_adaptive_camouflage
 {
  dir="deutschz_adaptive_camouflage"; name="DeutschZ Adaptive Camouflage";
  type="mod"; author="DeutschZ"; version="0.1.0";
  dependencies[]={"Game","World","Mission"};
  class defs
  {
   class gameScriptModule {value="";files[]={"deutschz_adaptive_camouflage/scripts/3_game"};};
   class worldScriptModule {value="";files[]={"deutschz_adaptive_camouflage/scripts/4_world"};};
   class missionScriptModule {value="";files[]={"deutschz_adaptive_camouflage/scripts/5_mission"};};
  };
 };
};
