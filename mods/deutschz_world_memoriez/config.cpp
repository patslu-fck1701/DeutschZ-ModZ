class CfgPatches
{
 class deutschz_world_memoriez
 {
  units[]={}; weapons[]={}; requiredVersion=0.1;
  requiredAddons[]={"DZ_Data","DZ_Scripts","deutschz_eventschedulerz"};
 };
};
class CfgMods
{
 class deutschz_world_memoriez
 {
  dir="deutschz_world_memoriez"; name="DeutschZ World Memoriez"; type="mod";
  author="DeutschZ"; version="1.0.0";
  dependencies[]={"Game","World","Mission"};
  class defs
  {
   class gameScriptModule {value="";files[]={"deutschz_world_memoriez/scripts/3_game"};};
   class worldScriptModule {value="";files[]={"deutschz_world_memoriez/scripts/4_world"};};
   class missionScriptModule {value="";files[]={"deutschz_world_memoriez/scripts/5_mission"};};
  };
 };
};
// Same NoiseParams fields as vanilla cfgAmmo NoiseHit; no entity or spawn type.
class DZWM_AlarmNoise
{
 strength=20;
 type="sound";
};
