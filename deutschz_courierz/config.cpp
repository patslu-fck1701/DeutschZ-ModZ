class CfgPatches
{
	class DeutschZ_CourierZ
	{
		units[] = {"CourierZ_ScientificCase", "CourierZ_ScientificCaseKeys", "CourierZ_ScientificCaseReward", "CourierZ_ScientificCaseKeysReward"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data", "DZ_Scripts", "DZ_Gear_Containers", "DZ_Gear_Tools"};
	};
};

class CfgMods
{
	class DeutschZ_CourierZ
	{
		dir = "deutschz_courierz";
		name = "DeutschZ CourierZ";
		author = "DeutschZ";
		version = "1.0.0";
		type = "mod";
		dependencies[] = {"Game", "World", "Mission"};
		class defs
		{
			class gameScriptModule { value = ""; files[] = {"deutschz_courierz/scripts/3_Game"}; };
			class worldScriptModule { value = ""; files[] = {"deutschz_courierz/scripts/4_World"}; };
			class missionScriptModule { value = ""; files[] = {"deutschz_courierz/scripts/5_Mission"}; };
		};
	};
};

class CfgVehicles
{
	class ScientificBriefcase;
	class ScientificBriefcaseKeys;
	class CourierZ_ScientificCase : ScientificBriefcase
	{
		scope = 2;
		displayName = "CourierZ Wissenschaftlicher Koffer";
		descriptionShort = "Ein verschlossener wissenschaftlicher Koffer fuer den geheimen CourierZ-Transportauftrag.";
		hiddenSelectionsTextures[] = {"\deutschz_courierz\data\courierz_scientificcase_co.paa"};
	};
	class CourierZ_ScientificCaseKeys : ScientificBriefcaseKeys
	{
		scope = 2;
		displayName = "CourierZ Kofferschluessel";
		descriptionShort = "Die passenden Schluessel fuer den wissenschaftlichen CourierZ-Koffer.";
		hiddenSelectionsTextures[] = {"\deutschz_courierz\data\CourierZ_ScientificCaseKeys_CO.paa"};
	};
	class CourierZ_ScientificCaseReward : CourierZ_ScientificCase
	{
		displayName = "Wissenschaftlicher Koffer";
		descriptionShort = "Deine Belohnung aus dem geheimen Transportauftrag.";
		itemsCargoSize[] = {10, 5};
	};
	class CourierZ_ScientificCaseKeysReward : CourierZ_ScientificCaseKeys
	{
		displayName = "Schluessel zum wissenschaftlichen Koffer";
		descriptionShort = "Die behaltenen Schluessel aus dem geheimen Transportauftrag.";
	};
};

class CfgSoundShaders
{
	class DZCourierZ_Voice_Base_Shader { volume = 1; };
	class DZCourierZ_Start_Shader: DZCourierZ_Voice_Base_Shader { samples[] = {{"\deutschz_courierz\sounds\start", 1}}; };
	class DZCourierZ_Keys_Shader: DZCourierZ_Voice_Base_Shader { samples[] = {{"\deutschz_courierz\sounds\keys", 1}}; };
	class DZCourierZ_Final_Shader: DZCourierZ_Voice_Base_Shader { samples[] = {{"\deutschz_courierz\sounds\final", 1}}; };
	class DZCourierZ_Idiot_Shader: DZCourierZ_Voice_Base_Shader { samples[] = {{"\deutschz_courierz\sounds\idiot", 1}}; };
};

class CfgSoundSets
{
	class DZCourierZ_Voice_Base_SoundSet { volumeFactor = 1; frequencyFactor = 1; spatial = 0; };
	class DZCourierZ_Start_SoundSet: DZCourierZ_Voice_Base_SoundSet { soundShaders[] = {"DZCourierZ_Start_Shader"}; };
	class DZCourierZ_Keys_SoundSet: DZCourierZ_Voice_Base_SoundSet { soundShaders[] = {"DZCourierZ_Keys_Shader"}; };
	class DZCourierZ_Final_SoundSet: DZCourierZ_Voice_Base_SoundSet { soundShaders[] = {"DZCourierZ_Final_Shader"}; };
	class DZCourierZ_Idiot_SoundSet: DZCourierZ_Voice_Base_SoundSet { soundShaders[] = {"DZCourierZ_Idiot_Shader"}; };
};
