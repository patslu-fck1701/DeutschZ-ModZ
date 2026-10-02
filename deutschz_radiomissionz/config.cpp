class CfgPatches
{
	class deutschz_radiomissionz
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Scripts","DZ_Radio","DayZExpansion_Core_Scripts","DayZExpansion_Navigation_Scripts"};
	};
};

class CfgMods
{
	class deutschz_radiomissionz
	{
		dir = "deutschz_radiomissionz";
		name = "DeutschZ RadioMissionZ";
		credits = "DeutschZ";
		author = "DeutschZ";
		version = "2.1.0-final-audio";
		type = "mod";
		dependencies[] = {"Game","World","Mission"};
		class defs
		{
			class gameScriptModule { value = ""; files[] = {"deutschz_radiomissionz/scripts/3_game"}; };
			class worldScriptModule { value = ""; files[] = {"deutschz_radiomissionz/scripts/4_world"}; };
			class missionScriptModule { value = ""; files[] = {"deutschz_radiomissionz/scripts/5_mission"}; };
		};
	};
};

class CfgSoundShaders
{
 class DZRMZ_HelpTrapped_Male_SoundShader { samples[]={{"deutschz_radiomissionz\sounds\radio\help\male\Male_01_Eingeschlossen",1}}; volume=1; };
 class DZRMZ_HelpTrapped_Female_SoundShader { samples[]={{"deutschz_radiomissionz\sounds\radio\help\female\Female_01_Eingeschlossen",1}}; volume=1; };
 class DZRMZ_HelpInjured_Male_SoundShader { samples[]={{"deutschz_radiomissionz\sounds\radio\help\male\Male_02_Verletzt",1}}; volume=1; };
 class DZRMZ_HelpInjured_Female_SoundShader { samples[]={{"deutschz_radiomissionz\sounds\radio\help\female\Female_02_Verletzt",1}}; volume=1; };
 class DZRMZ_HelpBesieged_Male_SoundShader { samples[]={{"deutschz_radiomissionz\sounds\radio\help\male\Male_03_Belagert",1}}; volume=1; };
 class DZRMZ_HelpBesieged_Female_SoundShader { samples[]={{"deutschz_radiomissionz\sounds\radio\help\female\Female_03_Belagert",1}}; volume=1; };
 class DZRMZ_HelpAlone_Male_SoundShader { samples[]={{"deutschz_radiomissionz\sounds\radio\help\male\Male_04_Allein_Zurueckgeblieben",1}}; volume=1; };
 class DZRMZ_HelpAlone_Female_SoundShader { samples[]={{"deutschz_radiomissionz\sounds\radio\help\female\Female_04_Allein_Zurueckgeblieben",1}}; volume=1; };
 class DZRMZ_StoryKoth_Male_SoundShader { samples[]={{"deutschz_radiomissionz\sounds\radio\story\male\Male_05_KOTH_Auftrag",1}}; volume=1; };
 class DZRMZ_StoryKoth_Female_SoundShader { samples[]={{"deutschz_radiomissionz\sounds\radio\story\female\Female_05_KOTH_Auftrag",1}}; volume=1; };
 class DZRMZ_StoryConvoy_Male_SoundShader { samples[]={{"deutschz_radiomissionz\sounds\radio\story\male\Male_06_Transport_Sieben",1}}; volume=1; };
 class DZRMZ_StoryConvoy_Female_SoundShader { samples[]={{"deutschz_radiomissionz\sounds\radio\story\female\Female_06_Transport_Sieben",1}}; volume=1; };
 class DZRMZ_StoryDecoder_Male_SoundShader { samples[]={{"deutschz_radiomissionz\sounds\radio\story\male\Male_07_Der_Decoder",1}}; volume=1; };
 class DZRMZ_StoryDecoder_Female_SoundShader { samples[]={{"deutschz_radiomissionz\sounds\radio\story\female\Female_07_Der_Decoder",1}}; volume=1; };
 class DZRMZ_StorySignal_Male_SoundShader { samples[]={{"deutschz_radiomissionz\sounds\radio\story\male\Male_08_Signal_Aktiviert",1}}; volume=1; };
 class DZRMZ_StorySignal_Female_SoundShader { samples[]={{"deutschz_radiomissionz\sounds\radio\story\female\Female_08_Signal_Aktiviert",1}}; volume=1; };
};
class CfgSoundSets
{
 class DZRMZ_Base_SoundSet { sound3DProcessingType="character3DProcessingType"; spatial=0; doppler=0; loop=0; volumeFactor=1; };
 class DZRMZ_HelpTrapped_Male_SoundSet: DZRMZ_Base_SoundSet { soundShaders[]={"DZRMZ_HelpTrapped_Male_SoundShader"}; };
 class DZRMZ_HelpTrapped_Female_SoundSet: DZRMZ_Base_SoundSet { soundShaders[]={"DZRMZ_HelpTrapped_Female_SoundShader"}; };
 class DZRMZ_HelpInjured_Male_SoundSet: DZRMZ_Base_SoundSet { soundShaders[]={"DZRMZ_HelpInjured_Male_SoundShader"}; };
 class DZRMZ_HelpInjured_Female_SoundSet: DZRMZ_Base_SoundSet { soundShaders[]={"DZRMZ_HelpInjured_Female_SoundShader"}; };
 class DZRMZ_HelpBesieged_Male_SoundSet: DZRMZ_Base_SoundSet { soundShaders[]={"DZRMZ_HelpBesieged_Male_SoundShader"}; };
 class DZRMZ_HelpBesieged_Female_SoundSet: DZRMZ_Base_SoundSet { soundShaders[]={"DZRMZ_HelpBesieged_Female_SoundShader"}; };
 class DZRMZ_HelpAlone_Male_SoundSet: DZRMZ_Base_SoundSet { soundShaders[]={"DZRMZ_HelpAlone_Male_SoundShader"}; };
 class DZRMZ_HelpAlone_Female_SoundSet: DZRMZ_Base_SoundSet { soundShaders[]={"DZRMZ_HelpAlone_Female_SoundShader"}; };
 class DZRMZ_StoryKoth_Male_SoundSet: DZRMZ_Base_SoundSet { soundShaders[]={"DZRMZ_StoryKoth_Male_SoundShader"}; };
 class DZRMZ_StoryKoth_Female_SoundSet: DZRMZ_Base_SoundSet { soundShaders[]={"DZRMZ_StoryKoth_Female_SoundShader"}; };
 class DZRMZ_StoryConvoy_Male_SoundSet: DZRMZ_Base_SoundSet { soundShaders[]={"DZRMZ_StoryConvoy_Male_SoundShader"}; };
 class DZRMZ_StoryConvoy_Female_SoundSet: DZRMZ_Base_SoundSet { soundShaders[]={"DZRMZ_StoryConvoy_Female_SoundShader"}; };
 class DZRMZ_StoryDecoder_Male_SoundSet: DZRMZ_Base_SoundSet { soundShaders[]={"DZRMZ_StoryDecoder_Male_SoundShader"}; };
 class DZRMZ_StoryDecoder_Female_SoundSet: DZRMZ_Base_SoundSet { soundShaders[]={"DZRMZ_StoryDecoder_Female_SoundShader"}; };
 class DZRMZ_StorySignal_Male_SoundSet: DZRMZ_Base_SoundSet { soundShaders[]={"DZRMZ_StorySignal_Male_SoundShader"}; };
 class DZRMZ_StorySignal_Female_SoundSet: DZRMZ_Base_SoundSet { soundShaders[]={"DZRMZ_StorySignal_Female_SoundShader"}; };
};
