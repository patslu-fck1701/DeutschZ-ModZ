class CfgPatches
{
 class deutschz_airdropz
 {
  units[]={"DZADZ_RavenSupply","DZADZ_RavenMedical","DZADZ_RavenEcho","DZADZ_RavenBlack","DZADZ_RavenQuarantine","DZADZ_RavenManifest"}; weapons[]={}; requiredVersion=0.1;
  requiredAddons[]={"DZ_Data","DZ_Scripts","DZ_Gear_Consumables","deutschz_eventschedulerz","DayZExpansion_Core_Scripts","DayZExpansion_Missions_Scripts","DayZExpansion_Navigation_Scripts"};
 };
};
class CfgVehicles
{
 class ExpansionAirdropContainer_Military_GreenCamo;
 class DZADZ_RavenSupply: ExpansionAirdropContainer_Military_GreenCamo {scope=2; displayName="Versiegelte RAVEN-Luftfracht"; dzadzRaven=1;};
 class DZADZ_RavenMedical: ExpansionAirdropContainer_Military_GreenCamo {scope=2; displayName="Versiegelte RAVEN-Sanitaetsfracht"; dzadzRaven=1;};
 class DZADZ_RavenEcho: ExpansionAirdropContainer_Military_GreenCamo {scope=2; displayName="Unmarkierte RAVEN-Luftfracht"; dzadzRaven=1;};
 class DZADZ_RavenBlack: ExpansionAirdropContainer_Military_GreenCamo {scope=2; displayName="Versiegelte RAVEN-Militaerfracht"; dzadzRaven=1;};
 class DZADZ_RavenQuarantine: ExpansionAirdropContainer_Military_GreenCamo {scope=2; displayName="Versiegelte RAVEN-Quarantaenefracht"; dzadzRaven=1;};
 class Paper;
 class DZADZ_RavenManifest: Paper
 {
  scope=2; displayName="Beschaedigtes RAVEN-Frachtpapier";
  descriptionShort="RAVEN-Frachtpapier 03-771. Alter Freigabestempel, neue Transportnummer, geschwaerzter Absender.";
  model="\dz\gear\consumables\Paper.p3d";
  hiddenSelections[]={"zbytek"};
  hiddenSelectionsTextures[]={"\dz\gear\consumables\data\loot_paper_co.paa"};
 };
};
class CfgSoundShaders
{
 class DZADZ_RavenApproach_SoundShader {samples[]={{"deutschz_airdropz\sounds\Raven_Two-One_89-5MHz_FINAL",1}}; volume=1;};
 class DZADZ_RavenTarget_SoundShader {samples[]={{"deutschz_airdropz\sounds\Raven_Two-One_TargetZone_89-5MHz_FINAL",1}}; volume=1;};
};
class CfgSoundSets
{
 class DZADZ_RavenApproach_SoundSet {soundShaders[]={"DZADZ_RavenApproach_SoundShader"}; spatial=0; doppler=0; loop=0; volumeFactor=1;};
 class DZADZ_RavenTarget_SoundSet {soundShaders[]={"DZADZ_RavenTarget_SoundShader"}; spatial=0; doppler=0; loop=0; volumeFactor=1;};
};
class CfgMods
{
 class deutschz_airdropz
 {
  dir="deutschz_airdropz"; name="DeutschZ AirdropZ"; author="DeutschZ"; version="1.0.0-raven"; type="mod";
  dependencies[]={"Game","World","Mission"};
  class defs
  {
   class gameScriptModule {value=""; files[]={"deutschz_airdropz/scripts/3_Game"};};
   class worldScriptModule {value=""; files[]={"deutschz_airdropz/scripts/4_World"};};
   class missionScriptModule {value=""; files[]={"deutschz_airdropz/scripts/5_Mission"};};
  };
 };
};
