class CfgPatches

{

    class deutschz_king_of_the_hillz

    {

        units[] =

        {

            "DZKOTH_EventFlagpole",

            "DZKOTH_FreeEventFlag",
            "DZKOTH_ProEventFlag",

            "DZKOTH_FreeRewardCrate",
            "DZKOTH_ProRewardCrate",



            "DZKOTH_FireworksLauncher",


            "DZKOTH_BattlegroundZ_SecretDocument",

            "DZKOTH_BossCorpse",

            "DZKOTH_EliteInfected",

            "DZKOTH_Infected_250",

            "DZKOTH_Infected_400",

            "DZKOTH_Infected_600",

            "DZKOTH_Infected_800",

            "DZKOTH_Infected_1000",

            "DZKOTH_BosZZombie"

        };

        weapons[] = {};

        requiredVersion = 0.1;

        requiredAddons[] =

        {

            "DZ_Data",

            "DZ_Scripts",

            "DZ_Gear_Camping",

            "DZ_Gear_Containers",

            "DZ_Gear_Tools",

            "DZ_Characters_Zombies",

            "DZ_Sounds_Effects"

        };

    };

};



class CfgSoundShaders

{

    class DZKOTHF_Music_Base_SoundShader { volume = 1; };

    class DZKOTHF_Music01_SoundShader: DZKOTHF_Music_Base_SoundShader { samples[] = {{"\deutschz_king_of_the_hillz\sounds\track01", 1}}; };

    class DZKOTHF_Music02_SoundShader: DZKOTHF_Music_Base_SoundShader { samples[] = {{"\deutschz_king_of_the_hillz\sounds\track02", 1}}; };

    class DZKOTHF_Music03_SoundShader: DZKOTHF_Music_Base_SoundShader { samples[] = {{"\deutschz_king_of_the_hillz\sounds\track03", 1}}; };

    class DZKOTHF_Music04_SoundShader: DZKOTHF_Music_Base_SoundShader { samples[] = {{"\deutschz_king_of_the_hillz\sounds\track04", 1}}; };

    class DZKOTHF_Music05_SoundShader: DZKOTHF_Music_Base_SoundShader { samples[] = {{"\deutschz_king_of_the_hillz\sounds\track05", 1}}; };

    class DZKOTHF_Music06_SoundShader: DZKOTHF_Music_Base_SoundShader { samples[] = {{"\deutschz_king_of_the_hillz\sounds\track06", 1}}; };

};



class CfgSoundSets

{

    class DZKOTHF_Music_Base_SoundSet { volumeFactor = 1; frequencyFactor = 1; spatial = 0; };

    class DZKOTHF_Music01_SoundSet: DZKOTHF_Music_Base_SoundSet { soundShaders[] = {"DZKOTHF_Music01_SoundShader"}; };

    class DZKOTHF_Music02_SoundSet: DZKOTHF_Music_Base_SoundSet { soundShaders[] = {"DZKOTHF_Music02_SoundShader"}; };

    class DZKOTHF_Music03_SoundSet: DZKOTHF_Music_Base_SoundSet { soundShaders[] = {"DZKOTHF_Music03_SoundShader"}; };

    class DZKOTHF_Music04_SoundSet: DZKOTHF_Music_Base_SoundSet { soundShaders[] = {"DZKOTHF_Music04_SoundShader"}; };

    class DZKOTHF_Music05_SoundSet: DZKOTHF_Music_Base_SoundSet { soundShaders[] = {"DZKOTHF_Music05_SoundShader"}; };

    class DZKOTHF_Music06_SoundSet: DZKOTHF_Music_Base_SoundSet { soundShaders[] = {"DZKOTHF_Music06_SoundShader"}; };

};



class CfgMods

{

    class deutschz_king_of_the_hillz

    {

        dir = "deutschz_king_of_the_hillz";

        name = "DeutschZ King of the HillZ";

        credits = "DeutschZ";

        author = "DeutschZ/fck1701";

        authorID = "";

        version = "2.0.0-pro-unified";

        type = "mod";

        dependencies[] = {"Game", "World", "Mission"};

        class defs

        {

            class gameScriptModule { value = ""; files[] = {"deutschz_king_of_the_hillz/scripts/3_game"}; };

            class worldScriptModule { value = ""; files[] = {"deutschz_king_of_the_hillz/scripts/4_world"}; };

            class missionScriptModule { value = ""; files[] = {"deutschz_king_of_the_hillz/scripts/5_mission"}; };

        };

    };

};



class CfgVehicles

{

    class StaticFlagPole;

    class Flag_Chernarus;

    class SeaChest;

    class FireworksLauncher;

    class Paper;

    class ZmbM_PolicemanSpecForce_Heavy;
    class ZmbM_HunterOld_Autumn;
    class ZmbM_SoldierNormal_Base;
    class ZmbM_Mummy;



    class DZKOTH_EventFlagpole: StaticFlagPole

    {

        scope = 2;

        displayName = "DeutschZ KotHZ Mast";

        descriptionShort = "Serververwalteter KotHZ-Capture-Mast.";

    };



    class DZKOTH_EventFlag: Flag_Chernarus
    {
        scope = 0;
        displayName = "DeutschZ KotHZ Fahnenbasis";
        descriptionShort = "Interne Basisklasse fuer die FREE- und PRO-Eventflagge.";
        hiddenSelections[] = {"camo"};
        hiddenSelectionsTextures[] = {"\deutschz_king_of_the_hillz\data\kothz_flag_dayz_co.paa"};
    };

    class DZKOTH_FreeEventFlag: DZKOTH_EventFlag
    {
        scope = 2;
        displayName = "DeutschZ KotHZ FREE Fahne";
        descriptionShort = "FREE-Fahne des DeutschZ KotHZ-Events.";
        hiddenSelectionsTextures[] = {"\deutschz_king_of_the_hillz\data\kothz_flag_dayz_co.paa"};
    };

    class DZKOTH_ProEventFlag: DZKOTH_EventFlag
    {
        scope = 2;
        displayName = "DeutschZ KotHZ PRO Fahne";
        descriptionShort = "PRO-Fahne des DeutschZ KotHZ-Events.";
        hiddenSelectionsTextures[] = {"\deutschz_king_of_the_hillz\data\premiun_flag_dayz_co.paa"};
    };



    class DZKOTH_RewardCrate: SeaChest

{

    scope = 0;

    displayName = "DeutschZ KotHZ Kistenbasis";

    descriptionShort = "Interne Basisklasse fuer FREE- und PRO-Belohnungskisten.";



    itemSize[] = {10, 15};

    weight = 10000;

    class Cargo
    {
        itemsCargoSize[] = {10, 50};
        openable = 0;
        allowOwnedCargoManipulation = 1;
    };

    hiddenSelections[] = {"camoGround"};
    hiddenSelectionsTextures[] = {"\deutschz_king_of_the_hillz\data\premium_sea_chest_co.paa"};

};



    class DZKOTH_FreeRewardCrate: DZKOTH_RewardCrate
    {
        scope = 2;
        displayName = "DeutschZ KotHZ FREE Belohnungskiste";
        descriptionShort = "FREE-Belohnungskiste mit 100 Inventarfeldern.";
        itemSize[] = {10, 15};
        weight = 10000;
        class Cargo
        {
            itemsCargoSize[] = {10, 10};
            openable = 0;
            allowOwnedCargoManipulation = 1;
        };
        hiddenSelections[] = {"camoGround"};
        hiddenSelectionsTextures[] = {"\deutschz_king_of_the_hillz\data\kothz_sea_chest_co.paa"};
    };

    class DZKOTH_ProRewardCrate: DZKOTH_RewardCrate
    {
        scope = 2;
        displayName = "DeutschZ KotHZ PRO Belohnungskiste";
        descriptionShort = "PRO-Belohnungskiste mit 500 Inventarfeldern.";
        itemSize[] = {10, 15};
        weight = 10000;
        class Cargo
        {
            itemsCargoSize[] = {10, 50};
            openable = 0;
            allowOwnedCargoManipulation = 1;
        };
        hiddenSelections[] = {"camoGround"};
        hiddenSelectionsTextures[] = {"\deutschz_king_of_the_hillz\data\premium_sea_chest_co.paa"};
    };


class DZKOTH_FireworksLauncher: FireworksLauncher

    {

        scope = 2;

        displayName = "DeutschZ KotHZ Feuerwerk";

        descriptionShort = "Serververwaltetes Gewinnfeuerwerk.";

        hiddenSelections[] = {"camo", "placing"};

        hiddenSelectionsTextures[] =

        {

            "\deutschz_king_of_the_hillz\data\deutschz_fireworkslauncher_co.paa",

            "\deutschz_king_of_the_hillz\data\deutschz_fireworkslauncher_co.paa"

        };

    };



class DZKOTH_BattlegroundZ_SecretDocument: Paper

    {

        scope = 2;

        displayName = "DeutschZ BattlegroundZ - Streng geheimes Dokument";

        descriptionShort = "Ein freigegebenes Einsatzdokument mit Zugangsdaten zur BattlegroundZ-Operation.";

        hiddenSelections[] = {"zbytek"};

        hiddenSelectionsTextures[] = {"\deutschz_king_of_the_hillz\data\deutschzbattlegroundzstrenggeheimdokument_loot_paper_co.paa"};

    };



class DZKOTH_BossCorpse: SeaChest

    {

        scope = 2;

        displayName = "BosZ-Ueberreste";

        descriptionShort = "Durchsuchbare Ueberreste des BosZ.";

        class Cargo

        {

            itemsCargoSize[] = {10, 50};

            openable = 0;

            allowOwnedCargoManipulation = 1;

        };

        hiddenSelections[] = {"camoGround"};

        hiddenSelectionsTextures[] = {"\deutschz_king_of_the_hillz\data\kothz_boss_remains_co.paa"};

    };



    class DZKOTH_EliteInfected: ZmbM_PolicemanSpecForce_Heavy { scope = 2; displayName = "DeutschZ KotHZ Elite-Infizierter"; };

    class DZKOTH_Infected_250: ZmbM_HunterOld_Autumn
    {
        scope = 2;
        displayName = "DeutschZ KotHZ Hunter 250";
        class DamageSystem { class GlobalHealth { class Health { hitpoints = 50000; }; }; };
    };

    class DZKOTH_Infected_400: ZmbM_PolicemanSpecForce_Heavy
    {
        scope = 2;
        displayName = "DeutschZ KotHZ Police 400";
        class DamageSystem { class GlobalHealth { class Health { hitpoints = 50000; }; }; };
    };

    class DZKOTH_Infected_600: ZmbM_SoldierNormal_Base
    {
        scope = 2;
        displayName = "DeutschZ KotHZ Military 600";
        class DamageSystem { class GlobalHealth { class Health { hitpoints = 50000; }; }; };
    };

    class DZKOTH_Infected_800: ZmbM_SoldierNormal_Base
    {
        scope = 2;
        displayName = "DeutschZ KotHZ Military 800";
        class DamageSystem { class GlobalHealth { class Health { hitpoints = 50000; }; }; };
    };

    class DZKOTH_Infected_1000: ZmbM_SoldierNormal_Base
    {
        scope = 2;
        displayName = "DeutschZ KotHZ Military 1000";
        class DamageSystem { class GlobalHealth { class Health { hitpoints = 50000; }; }; };
    };

    // Stable EventKetten.md classname; behaviour and texture remain owned by
    // the proven KotHZ document implementation.
    class DeutschZ_KotHZ_BattlegroundPapers: DZKOTH_BattlegroundZ_SecretDocument
    {
        scope = 2;
        displayName = "DeutschZ BattlegroundZ Einsatzpapiere";
    };

    class DZKOTH_BosZZombie: ZmbM_Mummy
    {
        scope = 2;
        displayName = "BosZ Zombie";
        class DamageSystem { class GlobalHealth { class Health { hitpoints = 50000; }; }; };
    };

};
