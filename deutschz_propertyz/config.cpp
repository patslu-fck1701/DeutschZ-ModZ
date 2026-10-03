class CfgPatches {
    class deutschz_propertyz {
        units[] = {"DZP_PropertyDoorController"};
        weapons[] = {};
        requiredVersion = 1.0;
        requiredAddons[] = {
            "DZ_Data",
            "CodeLock",
            "GDZ_Codelock",
            "DZ_Scripts",
            "DZ_Structures_Residential",
            "JM_CF_Scripts",
            "DayZExpansion_Groups_Scripts",
            "DayZExpansion_Market_Scripts",
            "DayZExpansion_SpawnSelection_Scripts"
        };
    };
};

class CfgVehicles {
    class Fence;
    class DZP_PropertyDoorController : Fence {
        scope = 2;
        displayName = "PropertyZ Haussteuerung";
        descriptionShort = "Hauszugang und Hausinventar";
        // Visible entrance body. The real attached CodeLock remains the
        // authoritative PIN/owner state and is hidden by script to avoid a
        // second, rotated visual instance.
        model = "\Codelock\codelock\GDZ_codelock.p3d";
        hiddenSelections[] = {"combination_lock_item"};
        class AnimationSources {
            class DZPHidden { source = "user"; animPeriod = 0.01; initPhase = 1; };
            class Combination_Lock_Item : DZPHidden { initPhase = 0; };
            class Lock_Item_1 : DZPHidden { initPhase = 0; };
            class Lock_Item_2 : DZPHidden { initPhase = 0; };
            class Combination_Lock_Attached : DZPHidden {};
            class Lock_Attached_1 : DZPHidden {};
            class Lock_Attached_2 : DZPHidden {};
        };
        itemsCargoSize[] = {10,200};
        attachments[] = {"Att_CombinationLock"};
        weight = 10000;
        lifetime = 3888000;
        itemSize[] = {10,10};
        carveNavmesh = 0;
        canBeDigged = 0;
        physLayer = "item_large";
        class GUIInventoryAttachmentsProps {
            class PropertyLock {
                name = "CodeLock";
                description = "";
                attachmentSlots[] = {"Att_CombinationLock"};
                icon = "cat_bb_attachments";
            };
        };
    };
};

class CfgMods {
    class deutschz_propertyz {
        dir = "deutschz_propertyz";
        hideName = 0;
        name = "DeutschZ PropertyZ";
        credits = "DeutschZ; behavioral reference: TheBuster, Sense";
        author = "DeutschZ";
        version = "1.0.0-test";
        extra = 0;
        type = "mod";
        dependencies[] = {"Game", "World", "Mission"};
        inputs = "deutschz_propertyz/inputs.xml";

        class defs {
            class gameScriptModule {
                value = "";
                files[] = {"deutschz_propertyz/scripts/3_Game"};
            };
            class worldScriptModule {
                value = "";
                files[] = {"deutschz_propertyz/scripts/4_World"};
            };
            class missionScriptModule {
                value = "";
                files[] = {"deutschz_propertyz/scripts/5_Mission"};
            };
        };
    };
};

class cfgSoundSets {
    class DZP_Alarm_SoundSet {
        soundShaders[] = {"DZP_Alarm_SoundShader"};
        sound3DProcessingType = "character3DProcessingType";
        volumeCurve = "characterAttenuationCurve";
        spatial = 1;
        doppler = 0;
        loop = 1;
    };
};
class cfgSoundShaders {
    class DZP_Alarm_SoundShader {
        samples[] = {{"deutschz_propertyz\\Data\\sound\\alarm",1}};
        volume = 5;
        range = 200;
        limitation = 0;
    };
};
