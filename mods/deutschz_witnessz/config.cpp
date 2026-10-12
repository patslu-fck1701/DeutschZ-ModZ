class CfgPatches
{
 class deutschz_witnessz
 {
  units[]={}; weapons[]={}; requiredVersion=0.1;
  requiredAddons[]={"DZ_Data","DZ_Scripts","DayZExpansion_AI_Scripts"};
 };
};
class CfgMods
{
 class deutschz_witnessz
 {
  dir="deutschz_witnessz"; name="DeutschZ WitnessZ"; type="mod"; author="DeutschZ"; version="0.1.0";
  dependencies[]={"Game","World","Mission"};
  class defs
  {
   class gameScriptModule {value="";files[]={"deutschz_witnessz/scripts/3_game"};};
   class worldScriptModule {value="";files[]={"deutschz_witnessz/scripts/4_world"};};
   class missionScriptModule {value="";files[]={"deutschz_witnessz/scripts/5_mission"};};
  };
 };
};
