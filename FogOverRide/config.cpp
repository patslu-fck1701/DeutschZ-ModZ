class CfgPatches
{
	class FOG_Damage_OverRides
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data",
			"FOG_MOD_Bags",
			"FOG_MOD_Clothes",
			"FOG_MOD_Gear",
			"FOG_MOD_Vests",
			"FOG_MOD_Slots"
		};
	};
};
class CfgMods
{
	class HAVOC_FOG_Override
	{
		dir="fogoverride";
		picture="";
		action="";
		hideName=1;
		hidePicture=1;
		name="fogoverride";
		credits="";
		author="IRON";
		authorID="";
		version="1.0";
		extra=0;
		type="mod";
	};
};
class CfgVehicles
{
	class Clothing;
	class Inventory_Base;
	class Container_Base;
	class Transmitter_Base;

	// DeutschZ backpack balancing: 10-column grids, capped at 20 rows for a usable inventory view.
	// Bags without a weapon slot receive Shoulder. Existing weapon capacity is doubled
	// with a second distinct compatible slot (duplicate slot names are not valid).
	class FOG_Bag_6ShTBag_ColorBase: Clothing { itemsCargoSize[]={10,40}; attachments[]={"Chemlight","WalkieTalkie","Backpack_1","Shoulder"}; };
	class FOG_Bag_ArcteryxLEAF_ColorBase: Clothing { itemsCargoSize[]={10,40}; attachments[]={"Chemlight","WalkieTalkie","Backpack_1","Shoulder"}; };
	class FOG_Bergen_Rucksack_ColorBase: Clothing { itemsCargoSize[]={10,45}; attachments[]={"Chemlight","WalkieTalkie","Backpack_1","Shoulder"}; };
	class FOG_Bag_CommsBag_Base: Clothing { itemsCargoSize[]={10,30}; attachments[]={"Chemlight","WalkieTalkie","Backpack_1","Shoulder"}; };
	class FOG_Bag_CommsBag_Transmitter_Base: Transmitter_Base { itemsCargoSize[]={10,30}; attachments[]={"CarBattery","Chemlight","WalkieTalkie","Backpack_1","Shoulder"}; };
	class FOG_Bag_DrawBridge_ColorBase: Clothing { itemsCargoSize[]={10,35}; attachments[]={"Chemlight","WalkieTalkie","Backpack_1","Shoulder"}; };
	class FOG_Bag_F4Terminator_ColorBase: Clothing { itemsCargoSize[]={10,50}; attachments[]={"Chemlight","WalkieTalkie","Backpack_1","Shoulder"}; };
	class FOG_Bag_LBT1475A_ColorBase: Clothing { itemsCargoSize[]={10,45}; attachments[]={"Chemlight","WalkieTalkie","Backpack_1","Shoulder"}; };
	class FOG_Bag_MRASAP_ColorBase: Clothing { itemsCargoSize[]={10,20}; attachments[]={"Chemlight","WalkieTalkie","Backpack_1","Shoulder"}; };
	class FOG_Bag_BlackJack_ColorBase: Clothing { itemsCargoSize[]={10,50}; attachments[]={"Shoulder","Melee","FOG_big_patch","FOG_ifak_vest","Belt_Back","Belt_Right","Belt_Left","Chemlight","WalkieTalkie","Backpack_1"}; };
	class FOG_Bag_SATL_ColorBase: Clothing { itemsCargoSize[]={10,50}; attachments[]={"Pistol","Shoulder","FOG_big_patch","FOG_ifak_vest","Belt_Back","Belt_Left","Chemlight","FOG_navagation_slot","WalkieTalkie","Backpack_1"}; };
	class FOG_Bag_SSOBag_ColorBase: Clothing { itemsCargoSize[]={10,40}; attachments[]={"Chemlight","WalkieTalkie","Backpack_1","Shoulder"}; };
	class FOG_Bag_Trizip_ColorBase: Clothing { itemsCargoSize[]={10,10}; attachments[]={"Shoulder"}; };

	// Clothing cargo: exactly 100 slots.
	class FOG_FlightSuit_ColorBase: Clothing { itemsCargoSize[]={10,10}; };
	class FOG_ArcticPants_ColorBase: Clothing { itemsCargoSize[]={10,10}; };
	class FOG_AthleticShorts_Base: Clothing { itemsCargoSize[]={10,10}; };
	class FOG_FieldPants_ColorBase: Clothing { itemsCargoSize[]={10,10}; };
	class FOG_FieldShorts_ColorBase: Clothing { itemsCargoSize[]={10,10}; };
	class FOG_Pants_Crye_G3_ColorBase: Clothing { itemsCargoSize[]={10,10}; };
	class FOG_Pants_FRACU_ColorBase: Clothing { itemsCargoSize[]={10,10}; };
	class FOG_Pants_Crye_G2_ColorBase: Clothing { itemsCargoSize[]={10,10}; };
	class FOG_Pants_Crye_G2_Holster_ColorBase: Clothing { itemsCargoSize[]={10,10}; };
	class FOG_G99_Pants_ColorBase: Clothing { itemsCargoSize[]={10,10}; };
	class FOG_Gorka_Pants_ColorBase: Clothing { itemsCargoSize[]={10,10}; };
	class FOG_L9_PatagoniaPants_ColorBase: Clothing { itemsCargoSize[]={10,10}; };
	class FOG_MTF_Pants_ColorBase: Clothing { itemsCargoSize[]={10,10}; };
	class FOG_UFP_Pants_ColorBase: Clothing { itemsCargoSize[]={10,10}; };
	class FOG_AEM01_Jacket_ColorBase: Clothing { itemsCargoSize[]={10,10}; };
	class FOG_Jacket_ArcteryxGryphon_ColorBase: Clothing { itemsCargoSize[]={10,10}; };
	class FOG_Shirt_ArcticJacket_ColorBase: Clothing { itemsCargoSize[]={10,10}; };
	class FOG_ButtonUp_ColorBase: Clothing { itemsCargoSize[]={10,10}; };
	class FOG_Cronen_Shirt_ColorBase: Clothing { itemsCargoSize[]={10,10}; };
	class FOG_Crye_G2_Shirt_ColorBase: Clothing { itemsCargoSize[]={10,10}; };
	class FOG_DrugRug_ColorBase: Clothing { itemsCargoSize[]={10,10}; };
	class FOG_511_Shirt_ColorBase: Clothing { itemsCargoSize[]={10,10}; };
	class FOG_Crye_G3_Shirt_ColorBase: Clothing { itemsCargoSize[]={10,10}; };
	class FOG_Enhanced_PCU_ColorBase: Clothing { itemsCargoSize[]={10,10}; };
	class FOG_HeavyRiders_Jacket_ColorBase: Clothing { itemsCargoSize[]={10,10}; };
	class FOG_Hoodie_Jacket_ColorBase: Clothing { itemsCargoSize[]={10,10}; };
	class FOG_Shirt_ISOF_Jacket_ColorBase: Clothing { itemsCargoSize[]={10,10}; };
	class FOG_Shirt_Tactical_Coat_Base: Clothing { itemsCargoSize[]={10,10}; };
	class FOG_Shirt_Polo_ColorBase: Clothing { itemsCargoSize[]={10,10}; };
	class FOG_Jacket_SoftShell_ColorBase: Clothing { itemsCargoSize[]={10,10}; };
	class FOG_Tactical_Hoodie_ColorBase: Clothing { itemsCargoSize[]={10,10}; };
	class FOG_Tshirt_ColorBase: Clothing { itemsCargoSize[]={10,10}; };
	class FOG_UFJ_Shirt_ColorBase: Clothing { itemsCargoSize[]={10,10}; };
	class FOG_Shirt_FRACU_Top_ColorBase: Clothing { itemsCargoSize[]={10,10}; };
	class FOG_G3_Hoodie_ColorBase: Clothing { itemsCargoSize[]={10,10}; };
	class FOG_Gorka_Jacket_ColorBase: Clothing { itemsCargoSize[]={10,10}; };
	class FOG_Hooded_Parka_ColorBase: Clothing { itemsCargoSize[]={10,10}; };
	class FOG_L9Patagonia_ColorBase: Clothing { itemsCargoSize[]={10,10}; };
	class FOG_MTF_Shirt_ColorBase: Clothing { itemsCargoSize[]={10,10}; };
	class FOG_Tactical_Fleece_ColorBase: Clothing { itemsCargoSize[]={10,10}; };
	class FOG_Shirt_UrbanUtilityJacket_Base: Clothing { itemsCargoSize[]={10,10}; };
	class FOG_RugbyShirt_ColorBase: Clothing { itemsCargoSize[]={10,10}; };
	class FOG_WoobieHoodie_ColorBase: Clothing { itemsCargoSize[]={10,10}; };

	// General-purpose pouches and cargo backpanels: exactly 50 cargo slots.
	class FOG_Pouch_Dump_Base: Inventory_Base { itemsCargoSize[]={5,6}; };
	class FOG_Pouch_Admin_Small_Base: Inventory_Base { itemsCargoSize[]={4,5}; };
	class FOG_Pouch_Admin_Spiritus_Base: Inventory_Base { itemsCargoSize[]={5,6}; };
	class FOG_Pouch_Tall_Spiritus_Base: Inventory_Base { itemsCargoSize[]={6,8}; };
	class FOG_Pouch_Mutant_Base: Inventory_Base { itemsCargoSize[]={6,8}; };
	class FOG_Pouch_JSTA_Base: Inventory_Base { itemsCargoSize[]={6,8}; };
	class FOG_Pouch_Belly_Base: Container_Base { itemsCargoSize[]={5,6}; };
	class FOG_Pouch_FannyPack_Base: Container_Base { itemsCargoSize[]={4,5}; };
	class FOG_Pouch_Belly_Spiritus_Base: Container_Base { itemsCargoSize[]={5,6}; };
	class FOG_Pouch_FerroDangler_Base: Container_Base { itemsCargoSize[]={5,6}; };
	class FOG_Pouch_Belly_Lunchbox_Base: Container_Base { itemsCargoSize[]={6,8}; };
	class FOG_PouchAdmin_Chest_Base: Inventory_Base { itemsCargoSize[]={5,6}; };
	class FOG_TV110T_AdminPouchBase: Container_Base { itemsCargoSize[]={4,5}; };
	class FOG_TV110T_UtilityPouchBase: Container_Base { itemsCargoSize[]={6,8}; };
	class FOG_Panel_CryeZipon_Base: Container_Base { itemsCargoSize[]={8,8}; };
	class FOG_JPC_Panel_Flag_Base: Container_Base { itemsCargoSize[]={8,8}; };
	class FOG_FC_Panel_Flag_Base: Container_Base { itemsCargoSize[]={8,8}; };
	class FOG_Panel_FerroBanger_Base: Container_Base { itemsCargoSize[]={8,10}; };
	class FOG_Panel_GMR_MiniMap_Base: Container_Base { itemsCargoSize[]={8,10}; };
	class FOG_Panel_MapPackMed_ColorBase: Inventory_Base { itemsCargoSize[]={10,10}; };
	class FOG_Vest_AVS_Base: Clothing
	{
		descriptionShort = "AVS: scalable load-bearing armor system with good protection and moderate coverage.";
		weight=8500;
		attachments[] = {"WalkieTalkie","Chemlight","FOG_big_patch","FOG_tourniquet","VestPouch","FOG_admin_small","FOG_VestSlotFR","FOG_gren_pouch","FOG_ifak_vest","FOG_vest_belly","FOG_vest_panel"};
		class DamageSystem
		{
			class GlobalArmor
			{
				class Projectile
				{
					class Health
					{
						damage=0.32;
					};
					class Blood
					{
						damage=0.25;
					};
					class Shock
					{
						damage=0.45;
					};
				};
				class Melee
				{
					class Health
					{
						damage=0.45;
					};
					class Blood
					{
						damage=0.40;
					};
					class Shock
					{
						damage=0.42;
					};
				};
				class Infected
				{
					class Health
					{
						damage=0.45;
					};
					class Blood
					{
						damage=0.40;
					};
					class Shock
					{
						damage=0.42;
					};
				};
			};
		};
	};
	class FOG_Vest_JPC2_ColorBase: Clothing
	{
		displayName="JPC 2.0";
		descriptionShort = "JPC 2.0: lightweight plate carrier balancing mobility and rifle protection.";
		weight=6400;
		attachments[] = {"WalkieTalkie","FOG_big_patch","FOG_tourniquet","VestPouch","FOG_admin_small","FOG_VestSlotFR","FOG_MRB_singlemag","FOG_gren_pouch","FOG_ifak_vest","FOG_vest_belly","FOG_vest_panel"};
		class DamageSystem
		{
			class GlobalArmor
			{
				class Projectile
				{
					class Health
					{
						damage=0.40;
					};
					class Blood
					{
						damage=0.35;
					};
					class Shock
					{
						damage=0.55;
					};
				};
				class Melee
				{
					class Health
					{
						damage=0.65;
					};
					class Blood
					{
						damage=0.60;
					};
					class Shock
					{
						damage=0.60;
					};
				};
				class Infected
				{
					class Health
					{
						damage=0.65;
					};
					class Blood
					{
						damage=0.60;
					};
					class Shock
					{
						damage=0.60;
					};
				};
			};
		};
	};
	class FOG_Vest_Plateframe_ColorBase: Clothing
	{
		displayName="S&S Plateframe Carrier";
		descriptionShort = "S&S PlateFrame: ultra-light plate carrier with limited coverage and high mobility.";
		weight=6000;
		attachments[] = {"WalkieTalkie","Chemlight","FOG_big_patch","FOG_tourniquet","VestPouch","FOG_admin_small","FOG_VestSlotFR","FOG_MRB_singlemag","FOG_gren_pouch","FOG_ifak_vest","FOG_vest_belly","FOG_vest_panel"};
		class DamageSystem
		{
			class GlobalArmor
			{
				class Projectile
				{
					class Health
					{
						damage=0.40;
					};
					class Blood
					{
						damage=0.35;
					};
					class Shock
					{
						damage=0.55;
					};
				};
				class Melee
				{
					class Health
					{
						damage=0.65;
					};
					class Blood
					{
						damage=0.60;
					};
					class Shock
					{
						damage=0.60;
					};
				};
				class Infected
				{
					class Health
					{
						damage=0.65;
					};
					class Blood
					{
						damage=0.60;
					};
					class Shock
					{
						damage=0.60;
					};
				};
				class FragGrenade
				{
					class Health
					{
						damage=0.55;
					};
					class Blood
					{
						damage=0.45;
					};
					class Shock
					{
						damage=0.50;
					};
				};
			};
		};
	};
	class FOG_Vest_DPC_ColorBase: Clothing
	{
		displayName="Dynamic Principles Plate Carrier";
		descriptionShort = "DPC: load-bearing plate carrier with side-plate support and extended equipment capacity.";
		weight=8800;
		attachments[] = {"WalkieTalkie","VestPouch","FOG_big_patch","FOG_vest_panel","FOG_tourniquet","FOG_vest_belly"};
		class DamageSystem
		{
			class GlobalArmor
			{
				class Projectile
				{
					class Health
					{
						damage=0.32;
					};
					class Blood
					{
						damage=0.25;
					};
					class Shock
					{
						damage=0.45;
					};
				};
				class Melee
				{
					class Health
					{
						damage=0.45;
					};
					class Blood
					{
						damage=0.40;
					};
					class Shock
					{
						damage=0.42;
					};
				};
				class Infected
				{
					class Health
					{
						damage=0.45;
					};
					class Blood
					{
						damage=0.40;
					};
					class Shock
					{
						damage=0.42;
					};
				};
			};
		};
	};
	class FOG_Vest_CPC_Base: Clothing
	{
		displayName="Cage Plate Carrier CPC";
		descriptionShort = "CPC: plate carrier with extended soft-armor and side-protection capability.";
		weight=11000;
		attachments[] = {"WalkieTalkie","FOG_big_patch","FOG_tourniquet","VestPouch","FOG_admin_small","FOG_VestSlotFR","FOG_gren_pouch","FOG_ifak_vest","FOG_vest_belly","FOG_vest_panel"};
		class DamageSystem
		{
			class GlobalArmor
			{
				class Projectile
				{
					class Health
					{
						damage=0.28;
					};
					class Blood
					{
						damage=0.20;
					};
					class Shock
					{
						damage=0.40;
					};
				};
				class Melee
				{
					class Health
					{
						damage=0.30;
					};
					class Blood
					{
						damage=0.25;
					};
					class Shock
					{
						damage=0.34;
					};
				};
				class Infected
				{
					class Health
					{
						damage=0.30;
					};
					class Blood
					{
						damage=0.25;
					};
					class Shock
					{
						damage=0.34;
					};
				};
			};
		};
	};
	class FOG_Vest_FCPC_ColorBase: Clothing
	{
		displayName = "FCPC V5 Plate Carrier";
		descriptionShort = "FCPC V5: lightweight modular plate carrier with limited coverage.";
		weight=6200;
		attachments[] = {"WalkieTalkie","FOG_big_patch","FOG_tourniquet","VestPouch","FOG_admin_small","FOG_VestSlotFR","FOG_gren_pouch","FOG_ifak_vest","FOG_vest_belly","FOG_vest_panel"};
		class DamageSystem
		{
			class GlobalArmor
			{
				class Projectile
				{
					class Health
					{
						damage=0.40;
					};
					class Blood
					{
						damage=0.35;
					};
					class Shock
					{
						damage=0.55;
					};
				};
				class Melee
				{
					class Health
					{
						damage=0.65;
					};
					class Blood
					{
						damage=0.60;
					};
					class Shock
					{
						damage=0.60;
					};
				};
				class Infected
				{
					class Health
					{
						damage=0.65;
					};
					class Blood
					{
						damage=0.60;
					};
					class Shock
					{
						damage=0.60;
					};
				};
			};
		};
	};
	class FOG_Vest_JPC_Base: Clothing
	{
		descriptionShort = "JPC: compact plate carrier with integrated magazine capacity.";
		weight=6800;
		attachments[] = {"WalkieTalkie","FOG_big_patch","FOG_tourniquet","VestPouch","FOG_admin_small","FOG_VestSlotFR","FOG_gren_pouch","FOG_ifak_vest","FOG_vest_belly","FOG_vest_panel"};
		class DamageSystem
		{
			class GlobalArmor
			{
				class Projectile
				{
					class Health
					{
						damage=0.36;
					};
					class Blood
					{
						damage=0.30;
					};
					class Shock
					{
						damage=0.50;
					};
				};
				class Melee
				{
					class Health
					{
						damage=0.55;
					};
					class Blood
					{
						damage=0.50;
					};
					class Shock
					{
						damage=0.50;
					};
				};
				class Infected
				{
					class Health
					{
						damage=0.55;
					};
					class Blood
					{
						damage=0.50;
					};
					class Shock
					{
						damage=0.50;
					};
				};
			};
		};
	};
	class FOG_Vest_Osprey_ColorBase: Clothing
	{
		descriptionShort = "Osprey body armor: heavy full-coverage system with strong ballistic and fragmentation protection.";
		weight=13000;
		attachments[] = {"WalkieTalkie","FOG_big_patch","FOG_tourniquet","Belt_Back","VestHolster","VestPouch","FOG_admin_small","FOG_VestSlotFR","FOG_gren_pouch","FOG_ifak_vest","FOG_vest_belly","FOG_vest_panel"};
		class DamageSystem
		{
			class GlobalArmor
			{
				class Projectile
				{
					class Health
					{
						damage=0.25;
					};
					class Blood
					{
						damage=0.18;
					};
					class Shock
					{
						damage=0.36;
					};
				};
				class Melee
				{
					class Health
					{
						damage=0.25;
					};
					class Blood
					{
						damage=0.20;
					};
					class Shock
					{
						damage=0.30;
					};
				};
				class Infected
				{
					class Health
					{
						damage=0.25;
					};
					class Blood
					{
						damage=0.20;
					};
					class Shock
					{
						damage=0.30;
					};
				};
			};
		};
	};
	class FOG_Vest_TacTec_Base: Clothing
	{
		descriptionShort = "TacTec: robust plate carrier with balanced protection, ventilation and mobility.";
		weight=7200;
		attachments[] = {"WalkieTalkie","Chemlight","FOG_big_patch","FOG_tourniquet","VestPouch","FOG_admin_small","FOG_VestSlotFR","FOG_gren_pouch","FOG_ifak_vest","FOG_vest_belly","FOG_vest_panel"};
		class DamageSystem
		{
			class GlobalArmor
			{
				class Projectile
				{
					class Health
					{
						damage=0.36;
					};
					class Blood
					{
						damage=0.30;
					};
					class Shock
					{
						damage=0.50;
					};
				};
				class Melee
				{
					class Health
					{
						damage=0.55;
					};
					class Blood
					{
						damage=0.50;
					};
					class Shock
					{
						damage=0.50;
					};
				};
				class Infected
				{
					class Health
					{
						damage=0.55;
					};
					class Blood
					{
						damage=0.50;
					};
					class Shock
					{
						damage=0.50;
					};
				};
			};
		};
	};
	class FOG_Vest_Gen4_ColorBase: Clothing
	{
		descriptionShort = "IOTV Gen IV: very heavy full-coverage armor system with the highest overall protection.";
		weight=14500;
		attachments[] = {"WalkieTalkie","Chemlight","FOG_big_patch","FOG_tourniquet","VestHolster","VestPouch","FOG_admin_small","FOG_VestSlotFR","FOG_gren_pouch","FOG_ifak_vest","Belt_Left","FOG_vest_panel"};
		class DamageSystem
		{
			class GlobalArmor
			{
				class Projectile
				{
					class Health
					{
						damage=0.25;
					};
					class Blood
					{
						damage=0.18;
					};
					class Shock
					{
						damage=0.36;
					};
				};
				class Melee
				{
					class Health
					{
						damage=0.25;
					};
					class Blood
					{
						damage=0.20;
					};
					class Shock
					{
						damage=0.30;
					};
				};
				class Infected
				{
					class Health
					{
						damage=0.25;
					};
					class Blood
					{
						damage=0.20;
					};
					class Shock
					{
						damage=0.30;
					};
				};
			};
		};
	};
	class FOG_Vest_LBT6094_Base: Clothing
	{
		descriptionShort = "LBT-6094: full-featured plate carrier supporting front, rear and side armor.";
		weight=9000;
		attachments[] = {"WalkieTalkie","FOG_big_patch","FOG_tourniquet","VestPouch","FOG_admin_small","FOG_VestSlotFR","FOG_gren_pouch","FOG_ifak_vest","FOG_vest_belly","FOG_vest_panel"};
		class DamageSystem
		{
			class GlobalArmor
			{
				class Projectile
				{
					class Health
					{
						damage=0.32;
					};
					class Blood
					{
						damage=0.25;
					};
					class Shock
					{
						damage=0.45;
					};
				};
				class Melee
				{
					class Health
					{
						damage=0.45;
					};
					class Blood
					{
						damage=0.40;
					};
					class Shock
					{
						damage=0.42;
					};
				};
				class Infected
				{
					class Health
					{
						damage=0.45;
					};
					class Blood
					{
						damage=0.40;
					};
					class Shock
					{
						damage=0.42;
					};
				};
			};
		};
	};
	class FOG_Vest_LV119_ColorBase: Clothing
	{
		descriptionShort = "LV-119: low-profile modular plate carrier with limited coverage.";
		weight=6500;
		attachments[] = {"WalkieTalkie","VestPouch","FOG_big_patch","FOG_vest_panel","FOG_tourniquet","FOG_vest_belly"};
		class DamageSystem
		{
			class GlobalArmor
			{
				class Projectile
				{
					class Health
					{
						damage=0.40;
					};
					class Blood
					{
						damage=0.35;
					};
					class Shock
					{
						damage=0.55;
					};
				};
				class Melee
				{
					class Health
					{
						damage=0.65;
					};
					class Blood
					{
						damage=0.60;
					};
					class Shock
					{
						damage=0.60;
					};
				};
				class Infected
				{
					class Health
					{
						damage=0.65;
					};
					class Blood
					{
						damage=0.60;
					};
					class Shock
					{
						damage=0.60;
					};
				};
			};
		};
	};
	class FOG_Vest_LVMBAV_ColorBase: Clothing
	{
		descriptionShort = "LV-MBAV: plate carrier with additional soft-armor coverage.";
		weight=10500;
		attachments[] = {};
		class DamageSystem
		{
			class GlobalArmor
			{
				class Projectile
				{
					class Health
					{
						damage=0.28;
					};
					class Blood
					{
						damage=0.20;
					};
					class Shock
					{
						damage=0.40;
					};
				};
				class Melee
				{
					class Health
					{
						damage=0.30;
					};
					class Blood
					{
						damage=0.25;
					};
					class Shock
					{
						damage=0.34;
					};
				};
				class Infected
				{
					class Health
					{
						damage=0.30;
					};
					class Blood
					{
						damage=0.25;
					};
					class Shock
					{
						damage=0.34;
					};
				};
			};
		};
	};
	class FOG_Vest_MMAC_Base: Clothing
	{
		descriptionShort = "MMAC: multi-mission plate carrier with side-plate support and integrated storage.";
		weight=8800;
		attachments[] = {"WalkieTalkie","FOG_big_patch","FOG_tourniquet","VestPouch","FOG_admin_small","FOG_VestSlotFR","FOG_gren_pouch","FOG_ifak_vest","FOG_vest_belly","FOG_vest_panel"};
		class DamageSystem
		{
			class GlobalArmor
			{
				class Projectile
				{
					class Health
					{
						damage=0.32;
					};
					class Blood
					{
						damage=0.25;
					};
					class Shock
					{
						damage=0.45;
					};
				};
				class Melee
				{
					class Health
					{
						damage=0.45;
					};
					class Blood
					{
						damage=0.40;
					};
					class Shock
					{
						damage=0.42;
					};
				};
				class Infected
				{
					class Health
					{
						damage=0.45;
					};
					class Blood
					{
						damage=0.40;
					};
					class Shock
					{
						damage=0.42;
					};
				};
			};
		};
	};
	class FOG_Vest_PACA_ColorBase: Clothing
	{
		descriptionShort = "PACA soft armor: protection against fragments and lower-energy threats; no rifle plate protection.";
		weight=2800;
		attachments[] = {};
		class DamageSystem
		{
			class GlobalArmor
			{
				class Projectile
				{
					class Health
					{
						damage=0.70;
					};
					class Blood
					{
						damage=0.55;
					};
					class Shock
					{
						damage=0.75;
					};
				};
				class Melee
				{
					class Health
					{
						damage=0.45;
					};
					class Blood
					{
						damage=0.45;
					};
					class Shock
					{
						damage=0.45;
					};
				};
				class Infected
				{
					class Health
					{
						damage=0.45;
					};
					class Blood
					{
						damage=0.45;
					};
					class Shock
					{
						damage=0.45;
					};
				};
			};
		};
	};
	class FOG_Vest_Thor_ColorBase: Clothing
	{
		descriptionShort = "THOR: modular armor carrier with combined hard- and soft-armor coverage.";
		weight=11500;
		attachments[] = {"WalkieTalkie","Chemlight","FOG_big_patch","FOG_tourniquet","VestHolster","VestPouch","FOG_admin_small","FOG_VestSlotFR","FOG_gren_pouch","FOG_ifak_vest","FOG_vest_panel"};
		class DamageSystem
		{
			class GlobalArmor
			{
				class Projectile
				{
					class Health
					{
						damage=0.28;
					};
					class Blood
					{
						damage=0.20;
					};
					class Shock
					{
						damage=0.40;
					};
				};
				class Melee
				{
					class Health
					{
						damage=0.30;
					};
					class Blood
					{
						damage=0.25;
					};
					class Shock
					{
						damage=0.34;
					};
				};
				class Infected
				{
					class Health
					{
						damage=0.30;
					};
					class Blood
					{
						damage=0.25;
					};
					class Shock
					{
						damage=0.34;
					};
				};
			};
		};
	};
	class FOG_TV110T_ColorBase: Clothing
	{
		descriptionShort = "TV-110: robust load-bearing plate carrier with integrated pouches.";
		weight=7800;
		attachments[] = {"WalkieTalkie","FOG_big_patch","FOG_tourniquet","VestPouch","TV110utilitypouch","TV110adminpouch","TV110fastmag","TV110medkit","TV110grenadepouch","FOG_vest_belly","FOG_vest_panel"};
		class DamageSystem
		{
			class GlobalArmor
			{
				class Projectile
				{
					class Health
					{
						damage=0.36;
					};
					class Blood
					{
						damage=0.30;
					};
					class Shock
					{
						damage=0.50;
					};
				};
				class Melee
				{
					class Health
					{
						damage=0.55;
					};
					class Blood
					{
						damage=0.50;
					};
					class Shock
					{
						damage=0.50;
					};
				};
				class Infected
				{
					class Health
					{
						damage=0.55;
					};
					class Blood
					{
						damage=0.50;
					};
					class Shock
					{
						damage=0.50;
					};
				};
			};
		};
	};
	class Mich2001Helmet;
	class FOG_Helmet_Airframe_Base: Mich2001Helmet
	{
		descriptionShort = "Ballistic helmet: HG2 handgun and fragmentation protection; rifle hits overmatch.";
		class DamageSystem
		{
			class GlobalArmor
			{
				class Projectile
				{
					class Health
					{
						damage=0.65;
					};
					class Blood
					{
						damage=0.55;
					};
					class Shock
					{
						damage=0.75;
					};
				};
				class Melee
				{
					class Health
					{
						damage=0.35;
					};
					class Blood
					{
						damage=0.40;
					};
					class Shock
					{
						damage=0.40;
					};
				};
				class Infected
				{
					class Health
					{
						damage=0.35;
					};
					class Blood
					{
						damage=0.40;
					};
					class Shock
					{
						damage=0.40;
					};
				};
				class FragGrenade
				{
					class Health
					{
						damage=0.45;
					};
					class Blood
					{
						damage=0.35;
					};
					class Shock
					{
						damage=0.55;
					};
				};
			};
		};
	};
	class FOG_Helmet_FAST_Bump_Base: Mich2001Helmet
	{
		descriptionShort = "FAST Bump: impact helmet without ballistic protection.";
		class DamageSystem
		{
			class GlobalArmor
			{
				class Projectile
				{
					class Health
					{
						damage=1.00;
					};
					class Blood
					{
						damage=1.00;
					};
					class Shock
					{
						damage=1.00;
					};
				};
				class Melee
				{
					class Health
					{
						damage=0.40;
					};
					class Blood
					{
						damage=0.45;
					};
					class Shock
					{
						damage=0.45;
					};
				};
				class Infected
				{
					class Health
					{
						damage=0.40;
					};
					class Blood
					{
						damage=0.45;
					};
					class Shock
					{
						damage=0.45;
					};
				};
				class FragGrenade
				{
					class Health
					{
						damage=0.85;
					};
					class Blood
					{
						damage=0.80;
					};
					class Shock
					{
						damage=0.85;
					};
				};
			};
		};
	};
	class FOG_Helmet_FASTMT_Base: Mich2001Helmet
	{
		descriptionShort = "Ballistic helmet: HG2 handgun and fragmentation protection; rifle hits overmatch.";
		class DamageSystem
		{
			class GlobalArmor
			{
				class Projectile
				{
					class Health
					{
						damage=0.65;
					};
					class Blood
					{
						damage=0.55;
					};
					class Shock
					{
						damage=0.75;
					};
				};
				class Melee
				{
					class Health
					{
						damage=0.35;
					};
					class Blood
					{
						damage=0.40;
					};
					class Shock
					{
						damage=0.40;
					};
				};
				class Infected
				{
					class Health
					{
						damage=0.35;
					};
					class Blood
					{
						damage=0.40;
					};
					class Shock
					{
						damage=0.40;
					};
				};
				class FragGrenade
				{
					class Health
					{
						damage=0.45;
					};
					class Blood
					{
						damage=0.35;
					};
					class Shock
					{
						damage=0.55;
					};
				};
			};
		};
	};
	class FOG_Helmet_FAST_SF_Base: Mich2001Helmet
	{
		descriptionShort = "Ballistic helmet: HG2 handgun and fragmentation protection; rifle hits overmatch.";
		class DamageSystem
		{
			class GlobalArmor
			{
				class Projectile
				{
					class Health
					{
						damage=0.65;
					};
					class Blood
					{
						damage=0.55;
					};
					class Shock
					{
						damage=0.75;
					};
				};
				class Melee
				{
					class Health
					{
						damage=0.35;
					};
					class Blood
					{
						damage=0.40;
					};
					class Shock
					{
						damage=0.40;
					};
				};
				class Infected
				{
					class Health
					{
						damage=0.35;
					};
					class Blood
					{
						damage=0.40;
					};
					class Shock
					{
						damage=0.40;
					};
				};
				class FragGrenade
				{
					class Health
					{
						damage=0.45;
					};
					class Blood
					{
						damage=0.35;
					};
					class Shock
					{
						damage=0.55;
					};
				};
			};
		};
	};
	class FOG_Helmet_MICH2000_Base: Mich2001Helmet
	{
		descriptionShort = "Ballistic helmet: HG2 handgun and fragmentation protection; rifle hits overmatch.";
		class DamageSystem
		{
			class GlobalArmor
			{
				class Projectile
				{
					class Health
					{
						damage=0.65;
					};
					class Blood
					{
						damage=0.55;
					};
					class Shock
					{
						damage=0.75;
					};
				};
				class Melee
				{
					class Health
					{
						damage=0.35;
					};
					class Blood
					{
						damage=0.40;
					};
					class Shock
					{
						damage=0.40;
					};
				};
				class Infected
				{
					class Health
					{
						damage=0.35;
					};
					class Blood
					{
						damage=0.40;
					};
					class Shock
					{
						damage=0.40;
					};
				};
				class FragGrenade
				{
					class Health
					{
						damage=0.45;
					};
					class Blood
					{
						damage=0.35;
					};
					class Shock
					{
						damage=0.55;
					};
				};
			};
		};
	};
	class FOG_Helmet_AJs_FAST_SF_Base: FOG_Helmet_FAST_SF_Base
	{
		descriptionShort = "Ballistic helmet: HG2 handgun and fragmentation protection; rifle hits overmatch.";
		class DamageSystem
		{
			class GlobalArmor
			{
				class Projectile
				{
					class Health
					{
						damage=0.65;
					};
					class Blood
					{
						damage=0.55;
					};
					class Shock
					{
						damage=0.75;
					};
				};
				class Melee
				{
					class Health
					{
						damage=0.35;
					};
					class Blood
					{
						damage=0.40;
					};
					class Shock
					{
						damage=0.40;
					};
				};
				class Infected
				{
					class Health
					{
						damage=0.35;
					};
					class Blood
					{
						damage=0.40;
					};
					class Shock
					{
						damage=0.40;
					};
				};
				class FragGrenade
				{
					class Health
					{
						damage=0.45;
					};
					class Blood
					{
						damage=0.35;
					};
					class Shock
					{
						damage=0.55;
					};
				};
			};
		};
	};
	class FOG_Helmet_FAST_FTHS_MC: FOG_Helmet_FAST_SF_Base
	{
		descriptionShort = "Ballistic helmet: HG2 handgun and fragmentation protection; rifle hits overmatch.";
		class DamageSystem
		{
			class GlobalArmor
			{
				class Projectile
				{
					class Health
					{
						damage=0.65;
					};
					class Blood
					{
						damage=0.55;
					};
					class Shock
					{
						damage=0.75;
					};
				};
				class Melee
				{
					class Health
					{
						damage=0.35;
					};
					class Blood
					{
						damage=0.40;
					};
					class Shock
					{
						damage=0.40;
					};
				};
				class Infected
				{
					class Health
					{
						damage=0.35;
					};
					class Blood
					{
						damage=0.40;
					};
					class Shock
					{
						damage=0.40;
					};
				};
				class FragGrenade
				{
					class Health
					{
						damage=0.45;
					};
					class Blood
					{
						damage=0.35;
					};
					class Shock
					{
						damage=0.55;
					};
				};
			};
		};
	};
	class FOG_Helmet_MTEK_Flux_Base: Mich2001Helmet
	{
		descriptionShort = "Ballistic helmet: HG2 handgun and fragmentation protection; rifle hits overmatch.";
		class DamageSystem
		{
			class GlobalArmor
			{
				class Projectile
				{
					class Health
					{
						damage=0.65;
					};
					class Blood
					{
						damage=0.55;
					};
					class Shock
					{
						damage=0.75;
					};
				};
				class Melee
				{
					class Health
					{
						damage=0.35;
					};
					class Blood
					{
						damage=0.40;
					};
					class Shock
					{
						damage=0.40;
					};
				};
				class Infected
				{
					class Health
					{
						damage=0.35;
					};
					class Blood
					{
						damage=0.40;
					};
					class Shock
					{
						damage=0.40;
					};
				};
			};
		};
	};
	class FOG_Helmet_Caimen_Base: Mich2001Helmet
	{
		descriptionShort = "Ballistic helmet: HG2 handgun and fragmentation protection; rifle hits overmatch.";
		class DamageSystem
		{
			class GlobalArmor
			{
				class Projectile
				{
					class Health
					{
						damage=0.65;
					};
					class Blood
					{
						damage=0.55;
					};
					class Shock
					{
						damage=0.75;
					};
				};
				class Melee
				{
					class Health
					{
						damage=0.35;
					};
					class Blood
					{
						damage=0.40;
					};
					class Shock
					{
						damage=0.40;
					};
				};
				class Infected
				{
					class Health
					{
						damage=0.35;
					};
					class Blood
					{
						damage=0.40;
					};
					class Shock
					{
						damage=0.40;
					};
				};
			};
		};
	};
	class FOG_Helmet_HGU56_ColorBase: Mich2001Helmet
	{
		descriptionShort = "Ballistic helmet: HG2 handgun and fragmentation protection; rifle hits overmatch.";
		class DamageSystem
		{
			class GlobalArmor
			{
				class Projectile
				{
					class Health
					{
						damage=0.65;
					};
					class Blood
					{
						damage=0.55;
					};
					class Shock
					{
						damage=0.75;
					};
				};
				class Melee
				{
					class Health
					{
						damage=0.35;
					};
					class Blood
					{
						damage=0.40;
					};
					class Shock
					{
						damage=0.40;
					};
				};
				class Infected
				{
					class Health
					{
						damage=0.35;
					};
					class Blood
					{
						damage=0.40;
					};
					class Shock
					{
						damage=0.40;
					};
				};
				class FragGrenade
				{
					class Health
					{
						damage=0.45;
					};
					class Blood
					{
						damage=0.35;
					};
					class Shock
					{
						damage=0.55;
					};
				};
			};
		};
	};
	class FOG_Vest_SPC_ColorBase: Clothing
	{
		descriptionShort = "AirLite SPC: lightweight structural plate carrier with good ventilation and limited coverage.";
		weight=6500;
		attachments[] = {"WalkieTalkie","FOG_big_patch","FOG_tourniquet","VestPouch","FOG_admin_small","FOG_VestSlotFR","FOG_gren_pouch","FOG_ifak_vest","FOG_vest_belly","FOG_vest_panel"};
		class DamageSystem
		{
			class GlobalArmor
			{
				class Projectile
				{
					class Health
					{
						damage=0.40;
					};
					class Blood
					{
						damage=0.35;
					};
					class Shock
					{
						damage=0.55;
					};
				};
				class Melee
				{
					class Health
					{
						damage=0.65;
					};
					class Blood
					{
						damage=0.60;
					};
					class Shock
					{
						damage=0.60;
					};
				};
				class Infected
				{
					class Health
					{
						damage=0.65;
					};
					class Blood
					{
						damage=0.60;
					};
					class Shock
					{
						damage=0.60;
					};
				};
				class FragGrenade
				{
					class Health
					{
						damage=0.55;
					};
					class Blood
					{
						damage=0.45;
					};
					class Shock
					{
						damage=0.50;
					};
				};
			};
		};
	};
	
	class FOG_Helmet_Exfil_Base: Mich2001Helmet
	{
		descriptionShort = "Ballistic helmet: HG2 handgun and fragmentation protection; rifle hits overmatch.";
		class DamageSystem
		{
			class GlobalArmor
			{
				class Projectile
				{
					class Health
					{
						damage=0.65;
					};
					class Blood
					{
						damage=0.55;
					};
					class Shock
					{
						damage=0.75;
					};
				};
				class Melee
				{
					class Health
					{
						damage=0.35;
					};
					class Blood
					{
						damage=0.40;
					};
					class Shock
					{
						damage=0.40;
					};
				};
				class Infected
				{
					class Health
					{
						damage=0.35;
					};
					class Blood
					{
						damage=0.40;
					};
					class Shock
					{
						damage=0.40;
					};
				};
				class FragGrenade
				{
					class Health
					{
						damage=0.45;
					};
					class Blood
					{
						damage=0.35;
					};
					class Shock
					{
						damage=0.55;
					};
				};
			};
		};
	};
};

