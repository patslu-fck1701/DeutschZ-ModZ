class CfgPatches
{
    class deutschz_toxicz
    {
        units[] = {"DZToxicZ_Flare","DZToxicZ_DamagedDocument","DZToxicZ_MorozovFile","DZToxicZ_AudioRecorder","DZToxicZ_Blackbox","DZToxicZ_DocumentDecoder","DZToxicZ_RewardCrate","DZToxicZ_NBCJacket","DZToxicZ_NBCPants","DZToxicZ_NBCHood","DZToxicZ_NBCGloves","DZToxicZ_NBCBoots","DZToxicZ_GP5Mask","DZToxicZ_Filter"};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] = {"DZ_Data","DZ_Scripts","DZ_Gear_Consumables","DZ_Gear_Containers","DZ_Characters_Tops","DZ_Characters_Pants","DZ_Characters_Headgear","DZ_Characters_Gloves","DZ_Characters_Shoes","DZ_Characters_Masks","DayZExpansion_AI_Scripts","DayZExpansion_Navigation_Scripts","deutschz_aiconvoyz"};
    };
};
class CfgMods
{
    class deutschz_toxicz
    {
        dir = "deutschz_toxicz";
        name = "DeutschZ ToxicZ";
        author = "DeutschZ/fck1701";
        version = "1.0.4-staging";
        type = "mod";
        dependencies[] = {"Game","World","Mission"};
        class defs
        {
            class gameScriptModule { value = ""; files[] = {"deutschz_toxicz/scripts/3_game"}; };
            class worldScriptModule { value = ""; files[] = {"deutschz_toxicz/scripts/4_world"}; };
            class missionScriptModule { value = ""; files[] = {"deutschz_toxicz/scripts/5_mission"}; };
        };
    };
};
class CfgSoundShaders
{
    class DZToxicZ_Voice_Base { volume = 1; };
    class DZToxicZ_AI_SoundShader: DZToxicZ_Voice_Base { samples[] = {{"\\deutschz_toxicz\\sounds\\toxicz_ai_voice",1}}; };
    class DZToxicZ_Reception_SoundShader: DZToxicZ_Voice_Base { samples[] = {{"\\deutschz_toxicz\\sounds\\toxicz_empfang",1}}; };
};
class CfgSoundSets
{
    class DZToxicZ_Voice_Base { volumeFactor = 1; frequencyFactor = 1; spatial = 0; };
    class DZToxicZ_AI_SoundSet: DZToxicZ_Voice_Base { soundShaders[] = {"DZToxicZ_AI_SoundShader"}; };
    class DZToxicZ_Reception_SoundSet: DZToxicZ_Voice_Base { soundShaders[] = {"DZToxicZ_Reception_SoundShader"}; };
};
class CfgVehicles
{
    class Roadflare;
    class Paper;
    class PersonalRadio;
    class SeaChest;
    class ElectronicRepairKit;
    class NBCJacketGray;
    class NBCPantsGray;
    class NBCHoodGray;
    class NBCGlovesGray;
    class NBCBootsGray;
    class GP5GasMask;
    class GasMask_Filter;
    class DZToxicZ_Flare: Roadflare { scope=2; displayName="ToxicZ-Flare"; descriptionShort="Eine auffaellige Signalflare des Protokolls T-17."; hiddenSelectionsTextures[]={"\\deutschz_toxicz\\data\\deutschz_toxicz_flare_road_flare_co.paa"}; };
    class DZToxicZ_DamagedDocument: Paper { scope=2; displayName="Beschaedigtes T-17-Dokument"; descriptionShort="Riffy nicht ohne Schutz betreten. Protokoll T-17 aktiv. Morozov weiss, was dort liegt."; hiddenSelections[]={"zbytek"}; hiddenSelectionsTextures[]={"\\deutschz_toxicz\\data\\toxicz_sec_doc_loot_paper_co.paa"}; };
    class DZToxicZ_MorozovFile: DZToxicZ_DamagedDocument { displayName="Akte Dr. Viktor Morozov"; descriptionShort="Projektleitung T-17. Status: VERSTORBEN. Todesdatum: 14 Tage vor Transportbeginn."; };
    class DZToxicZ_AudioRecorder: PersonalRadio { scope=2; displayName="Beschaedigter Audiorekorder"; descriptionShort="Transport Sieben hat Riffy erreicht. Glauben Sie nicht den offiziellen Berichten."; };
    class DZToxicZ_Blackbox: SeaChest { scope=2; displayName="T-17 Blackbox"; descriptionShort="Verschluesselte Daten von Transport Sieben."; itemsCargoSize[]={10,50}; };
    class DZToxicZ_DocumentDecoder: ElectronicRepairKit { scope=2; displayName="DeutschZ Document Decoder"; descriptionShort="Decoder fuer verschluesselte DeutschZ-Einsatzdokumente."; };
    class DZToxicZ_RewardCrate: SeaChest { scope=2; displayName="ToxicZ Belohnung"; descriptionShort="Serverautoritativ freigegebene T-17-Belohnung."; itemsCargoSize[]={10,50}; };
    class DZToxicZ_NBCJacket: NBCJacketGray
    {
        scope=2;
        displayName="ToxicZ Schutzjacke";
        hiddenSelectionsTextures[]={"\\dz\\characters\\tops\\Data\\NBC_Jacket_g_white_co.paa","\\deutschz_toxicz\\data\\deutschz_jacke_NBC_Jacket_white_co.paa","\\deutschz_toxicz\\data\\deutschz_jacke_NBC_Jacket_white_co.paa"};
    };
    class DZToxicZ_NBCPants: NBCPantsGray
    {
        scope=2;
        displayName="ToxicZ Schutzhose";
        hiddenSelectionsTextures[]={"\\deutschz_toxicz\\data\\deutschz_hose_NBC_Pants_white_co.paa","\\deutschz_toxicz\\data\\deutschz_hose_NBC_Pants_white_co.paa","\\deutschz_toxicz\\data\\deutschz_hose_NBC_Pants_white_co.paa"};
    };
    class DZToxicZ_NBCHood: NBCHoodGray
    {
        scope=2;
        displayName="ToxicZ Schutzhaube";
        hiddenSelectionsTextures[]={"\\deutschz_toxicz\\data\\deutschz_haube_NBC_Hood_white_co.paa","\\deutschz_toxicz\\data\\deutschz_haube_NBC_Hood_white_co.paa","\\deutschz_toxicz\\data\\deutschz_haube_NBC_Hood_white_co.paa"};
    };
    class DZToxicZ_NBCGloves: NBCGlovesGray
    {
        scope=2;
        displayName="ToxicZ Schutzhandschuhe";
        hiddenSelectionsTextures[]={"\\deutschz_toxicz\\data\\deutschz_handchuhe_nbc_gloves_grey_co.paa","\\deutschz_toxicz\\data\\deutschz_handchuhe_nbc_gloves_grey_co.paa","\\deutschz_toxicz\\data\\deutschz_handchuhe_nbc_gloves_grey_co.paa"};
    };
    class DZToxicZ_NBCBoots: NBCBootsGray
    {
        scope=2;
        displayName="ToxicZ Schutzstiefel";
        hiddenSelectionsTextures[]={"\\deutschz_toxicz\\data\\deutschz_schuhe_NBC_Boots_white_co.paa","\\deutschz_toxicz\\data\\deutschz_schuhe_NBC_Boots_white_co.paa","\\deutschz_toxicz\\data\\deutschz_schuhe_NBC_Boots_white_co.paa"};
    };
    class DZToxicZ_GP5Mask: GP5GasMask
    {
        scope=2;
        displayName="ToxicZ GP5-Maske";
        hiddenSelectionsTextures[]={"\\deutschz_toxicz\\data\\deutschz_maske_gp5gasmask_white_co.paa","\\deutschz_toxicz\\data\\deutschz_maske_gp5gasmask_white_co.paa","\\deutschz_toxicz\\data\\deutschz_maske_gp5gasmask_white_co.paa"};
    };
    class DZToxicZ_Filter: GasMask_Filter { scope=2; displayName="ToxicZ Filter"; hiddenSelectionsTextures[]={"\\deutschz_toxicz\\data\\deutschz_filter_gasmask_filter_co.paa"}; };
};
