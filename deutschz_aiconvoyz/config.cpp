class CfgPatches
{
    class deutschz_aiconvoyz
    {
        units[] = {"deutschz_aiconvoyz_blackbox","Toxicz_Doc_Decoder","ToxicZ_Secret_Document","ToxicZ_Signal_Marker"};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] = {"DZ_Data","AIConvoy","DayZExpansion_AI_Scripts","DayZExpansion_Navigation_Scripts","DZ_Gear_Containers","DZ_Gear_Navigation","DZ_Gear_Consumables"};
    };
};
class CfgMods
{
    class deutschz_aiconvoyz
    {
        dir = "deutschz_aiconvoyz";
        name = "deutschz aiconvoyz";
        type = "mod";
        dependencies[] = {"Game","World","Mission"};
        class defs
        {
            class gameScriptModule { value = ""; files[] = {"deutschz_aiconvoyz/scripts/3_game"}; };
            class worldScriptModule { value = ""; files[] = {"deutschz_aiconvoyz/scripts/4_world"}; };
            class missionScriptModule { value = ""; files[] = {"deutschz_aiconvoyz/scripts/5_mission"}; };
        };
    };
};
class CfgVehicles
{
    class Paper;
    class SeaChest;
    class GPSReceiver;
    class deutschz_aiconvoyz_blackbox: SeaChest
    {
        scope = 2;
        displayName = "Convoy BlackBox";
        descriptionShort = "Protected data container recovered from the convoy incident.";
        itemsCargoSize[] = {20,25};
        hiddenSelections[] = {"camoGround"};
        hiddenSelectionsTextures[] = {"\deutschz_aiconvoyz\data\aiconvoyz_blackbox_sea_chest_co.paa"};
    };
    class deutschz_aiconvoyz_decoder: GPSReceiver
    {
        scope = 2;
        displayName = "Convoy Decoder";
        descriptionShort = "Decoder recovered from the hacked Convoy BlackBox.";
        hiddenSelectionsTextures[] = {"deutschz_aiconvoyz/data/document_dekoder_gpsreceiver_co.paa"};
    };
    class Toxicz_Doc_Decoder: deutschz_aiconvoyz_decoder
    {
        scope = 2;
        displayName = "ToxicZ-Decoder";
        descriptionShort = "Mit dem geheimen ToxicZ-Dokument kombinieren, um das ToxicZ-Signalgeraet zu erhalten.";
    };
    class ToxicZ_Secret_Document: Paper
    {
        scope = 2;
        displayName = "Geheimes ToxicZ-Dokument";
        descriptionShort = "Verschluesselte ToxicZ-Koordinaten. Benoetigt einen ToxicZ-Decoder.";
        hiddenSelections[] = {"zbytek"};
        hiddenSelectionsTextures[] = {"\deutschz_aiconvoyz\data\toxicz_secret_document_co.paa"};
    };
    class ToxicZ_Signal_Marker: Toxicz_Doc_Decoder
    {
        scope = 2;
        displayName = "ToxicZ-Signalgeraet";
        descriptionShort = "Entschluesseltes Signal: Dieses Geraet startet die ToxicZ-Reise.";
        hiddenSelectionsTextures[] = {"deutschz_aiconvoyz/data/zone_marker_gpsreceiver_co.paa"};
    };
    class deutschz_aiconvoyz_cardreader: deutschz_aiconvoyz_decoder
    {
        scope = 1;
    };
    class toxicz_zone_marker: deutschz_aiconvoyz_decoder
    {
        scope = 2;
        displayName = "Toxic Zone Marker";
        descriptionShort = "Combined authorization item for the ToxicZ event.";
    };
};
