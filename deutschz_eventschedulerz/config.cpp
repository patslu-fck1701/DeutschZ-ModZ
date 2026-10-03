class CfgPatches
{
 class deutschz_eventschedulerz
 {
  units[]={};
  weapons[]={};
  requiredVersion=0.1;
  requiredAddons[]=
  {
   "DZ_Data",
   "DZ_Scripts",
   "DayZExpansion_Core_Scripts",
   "deutschz_king_of_the_hillz",
   "deutschz_aiconvoyz",
   "DeutschZ_CourierZ",
   "deutschz_toxicz",
   "DeutschZ_BattlegroundZ",
   "DeutschZ_Operation_DeutschZ"
  };
 };
};

class CfgMods
{
 class deutschz_eventschedulerz
 {
  dir="deutschz_eventschedulerz";
  name="DeutschZ EventSchedulerZ";
  type="mod";
  dependencies[]={"Game","World"};

  class defs
  {
   class gameScriptModule
   {
    value="";
    files[]={"deutschz_eventschedulerz/scripts/3_game"};
   };

   class worldScriptModule
   {
    value="";
    files[]={"deutschz_eventschedulerz/scripts/4_world"};
   };
  };
 };
};
