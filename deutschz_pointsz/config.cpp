class CfgPatches
{
	class DeutschZ_PointsZ
	{
		units[]={"DeutschZ_Token_1","DeutschZ_Token_2","DeutschZ_Token_3","DeutschZ_Token_5","DeutschZ_Token_25"};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]={"DZ_Data","DZ_Gear_Consumables","DZ_Scripts","DayZExpansion_Core_Scripts"};
	};
};
class CfgMods
{
	class DeutschZ_PointsZ
	{
		dir="deutschz_pointsz";
		name="DeutschZ PointsZ";
		author="DeutschZ";
		version="1.0.0";
		type="mod";
		dependencies[]={"Game","World","Mission"};
		class defs
		{
			class gameScriptModule {value=""; files[]={"deutschz_pointsz/scripts/3_Game"};};
			class worldScriptModule {value=""; files[]={"deutschz_pointsz/scripts/4_World"};};
			class missionScriptModule {value=""; files[]={"deutschz_pointsz/scripts/5_Mission"};};
		};
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class DeutschZ_Token_Base: Inventory_Base
	{
		scope=0;
		model="\DZ\gear\consumables\PunchedCard.p3d";
		itemSize[]={1,1};
		weight=5;
		canBeSplit=1;
		isMeleeWeapon=0;
		quantityBar=1;
		varQuantityInit=1;
		varQuantityMin=0;
		varQuantityMax=50;
		varQuantityDestroyOnMin=1;
		inventorySlot[]={};
	};
	class DeutschZ_Token_1: DeutschZ_Token_Base {scope=2; displayName="$STR_DZPOINTS_TOKEN_1"; descriptionShort="$STR_DZPOINTS_TOKEN_DESC_1";};
	class DeutschZ_Token_2: DeutschZ_Token_Base {scope=2; displayName="$STR_DZPOINTS_TOKEN_2"; descriptionShort="$STR_DZPOINTS_TOKEN_DESC_2";};
	class DeutschZ_Token_3: DeutschZ_Token_Base {scope=2; displayName="$STR_DZPOINTS_TOKEN_3"; descriptionShort="$STR_DZPOINTS_TOKEN_DESC_3";};
	class DeutschZ_Token_5: DeutschZ_Token_Base {scope=2; displayName="$STR_DZPOINTS_TOKEN_5"; descriptionShort="$STR_DZPOINTS_TOKEN_DESC_5";};
	class DeutschZ_Token_25: DeutschZ_Token_Base {scope=2; displayName="$STR_DZPOINTS_TOKEN_25"; descriptionShort="$STR_DZPOINTS_TOKEN_DESC_25";};
};
