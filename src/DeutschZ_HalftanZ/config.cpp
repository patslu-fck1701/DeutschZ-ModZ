class CfgPatches
{
    class DeutschZ_HalftanZ
    {
        units[] =
        {
            "SurvivorM_Halftan",
            "SurvivorM_Halftan2",
            "SurvivorF_DeutschZNaked",
            "SurvivorF_DeutschZSexy"
        };
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] =
        {
            "DZ_Characters",
            "DZ_Characters_Tops",
            "DZ_Characters_Pants"
        };
    };
};

class CfgMods
{
    class DeutschZ_HalftanZ
    {
        dir = "DeutschZ_HalftanZ";
        name = "DeutschZ HalftanZ";
        author = "DeutschZ";
        version = "1.0.0";
        type = "mod";
        dependencies[] = {};
    };
};

class CfgVehicles
{
    class FemaleTorso_Base;
    class FemaleLegs_Base;
    class SurvivorM_Peter;
    class SurvivorF_Eva;

    // Vanilla torso_f uses the real ODOL selection "camofemale" and the
    // f_underwear01 UV atlas. No invented hidden selection is used here.
    class DZHZ_FemaleNakedTorso : FemaleTorso_Base
    {
        scope = 1;
        model = "\DZ\characters\tops\torso_f.p3d";
        hiddenSelections[] = {"camofemale"};
        hiddenSelectionsTextures[] =
        {
            "\DeutschZ_HalftanZ\data\female_naked\deutschzx_nakedbody_f_underwear01_beige_co.paa"
        };
        hiddenSelectionsMaterials[] =
        {
            "DeutschZ_HalftanZ\data\female_naked\f_underwear01.rvmat"
        };
    };

    // Vanilla legs_f separates skin (personality) from underwear
    // (camofemale). Eva remains the skin/head base; only camofemale is custom.
    class DZHZ_FemaleNakedLegs : FemaleLegs_Base
    {
        scope = 1;
        model = "\DZ\characters\pants\legs_f.p3d";
        hiddenSelections[] = {"personality", "camofemale"};
        hiddenSelectionsTextures[] =
        {
            "\DZ\characters\heads\data\f_Eva_2\hhl_f_Eva_body_co.paa",
            "\DeutschZ_HalftanZ\data\female_naked\deutschzx_nakedbody_f_underwear01_beige_co.paa"
        };
        hiddenSelectionsMaterials[] =
        {
            "DZ\characters\heads\data\f_Eva_2\hhl_f_Eva_body.rvmat",
            "DeutschZ_HalftanZ\data\female_naked\f_underwear01.rvmat"
        };
    };

    class DZHZ_FemaleSexyTorso : FemaleTorso_Base
    {
        scope = 1;
        model = "\DZ\characters\tops\torso_f.p3d";
        hiddenSelections[] = {"camofemale"};
        hiddenSelectionsTextures[] =
        {
            "\DeutschZ_HalftanZ\data\female_sexy\deutschzx_sexybody_f_underwear01_beige_co.paa"
        };
        hiddenSelectionsMaterials[] =
        {
            "DeutschZ_HalftanZ\data\female_sexy\f_underwear01.rvmat"
        };
    };

    class DZHZ_FemaleSexyLegs : FemaleLegs_Base
    {
        scope = 1;
        model = "\DZ\characters\pants\legs_f.p3d";
        hiddenSelections[] = {"personality", "camofemale"};
        hiddenSelectionsTextures[] =
        {
            "\DZ\characters\heads\data\f_Eva_2\hhl_f_Eva_body_co.paa",
            "\DeutschZ_HalftanZ\data\female_sexy\deutschzx_sexybody_f_underwear01_beige_co.paa"
        };
        hiddenSelectionsMaterials[] =
        {
            "DZ\characters\heads\data\f_Eva_2\hhl_f_Eva_body.rvmat",
            "DeutschZ_HalftanZ\data\female_sexy\f_underwear01.rvmat"
        };
    };

    class SurvivorM_Halftan : SurvivorM_Peter
    {
        scope = 2;
        displayName = "Halftan";
        decayedTexture = "\DeutschZ_HalftanZ\data\halftan\halftan_hhl_m_peter_body_unshaved_co.paa";

        class BloodyHands
        {
            mat_normal = "DeutschZ_HalftanZ\data\halftan\body_shaved.rvmat";
            mat_blood = "DeutschZ_HalftanZ\data\halftan\body_bloodyhands.rvmat";
        };

        class Lifespan
        {
            class Beard
            {
                mat[] =
                {
                    "\DeutschZ_HalftanZ\data\halftan\halftan_hhl_m_peter_body_unshaved_co.paa",
                    "DeutschZ_HalftanZ\data\halftan\body_shaved.rvmat",
                    "\DeutschZ_HalftanZ\data\halftan\halftan_hhl_m_peter_body_unshaved_co.paa",
                    "DeutschZ_HalftanZ\data\halftan\body_unshaved.rvmat",
                    "\DeutschZ_HalftanZ\data\halftan\halftan_hhl_m_peter_body_unshaved_co.paa",
                    "DeutschZ_HalftanZ\data\halftan\body_bearded.rvmat",
                    "\DZ\characters\heads\data\m_peter\hhl_m_peter_beard_ca.paa",
                    "DeutschZ_HalftanZ\data\halftan\beard.rvmat"
                };
            };
        };

        class Wounds
        {
            tex[] = {};
            mat[] =
            {
                "DeutschZ_HalftanZ\data\halftan\body_shaved.rvmat",
                "DeutschZ_HalftanZ\data\halftan\body_shaved_injury.rvmat",
                "DeutschZ_HalftanZ\data\halftan\body_shaved_injury2.rvmat",
                "DeutschZ_HalftanZ\data\halftan\body_unshaved.rvmat",
                "DeutschZ_HalftanZ\data\halftan\body_unshaved_injury.rvmat",
                "DeutschZ_HalftanZ\data\halftan\body_unshaved_injury2.rvmat",
                "DeutschZ_HalftanZ\data\halftan\body_bearded.rvmat",
                "DeutschZ_HalftanZ\data\halftan\body_bearded_injury.rvmat",
                "DeutschZ_HalftanZ\data\halftan\body_bearded_injury2.rvmat"
            };
        };
    };

    class SurvivorM_Halftan2 : SurvivorM_Peter
    {
        scope = 2;
        displayName = "Halftan 2";
        decayedTexture = "\DeutschZ_HalftanZ\data\halftan2\halftan2_hhl_m_peter_body_unshaved_co.paa";

        class BloodyHands
        {
            mat_normal = "DeutschZ_HalftanZ\data\halftan2\body_shaved.rvmat";
            mat_blood = "DeutschZ_HalftanZ\data\halftan2\body_bloodyhands.rvmat";
        };

        class Lifespan
        {
            class Beard
            {
                mat[] =
                {
                    "\DeutschZ_HalftanZ\data\halftan2\halftan2_hhl_m_peter_body_unshaved_co.paa",
                    "DeutschZ_HalftanZ\data\halftan2\body_shaved.rvmat",
                    "\DeutschZ_HalftanZ\data\halftan2\halftan2_hhl_m_peter_body_unshaved_co.paa",
                    "DeutschZ_HalftanZ\data\halftan2\body_unshaved.rvmat",
                    "\DeutschZ_HalftanZ\data\halftan2\halftan2_hhl_m_peter_body_unshaved_co.paa",
                    "DeutschZ_HalftanZ\data\halftan2\body_bearded.rvmat",
                    "\DZ\characters\heads\data\m_peter\hhl_m_peter_beard_ca.paa",
                    "DeutschZ_HalftanZ\data\halftan2\beard.rvmat"
                };
            };
        };

        class Wounds
        {
            tex[] = {};
            mat[] =
            {
                "DeutschZ_HalftanZ\data\halftan2\body_shaved.rvmat",
                "DeutschZ_HalftanZ\data\halftan2\body_shaved_injury.rvmat",
                "DeutschZ_HalftanZ\data\halftan2\body_shaved_injury2.rvmat",
                "DeutschZ_HalftanZ\data\halftan2\body_unshaved.rvmat",
                "DeutschZ_HalftanZ\data\halftan2\body_unshaved_injury.rvmat",
                "DeutschZ_HalftanZ\data\halftan2\body_unshaved_injury2.rvmat",
                "DeutschZ_HalftanZ\data\halftan2\body_bearded.rvmat",
                "DeutschZ_HalftanZ\data\halftan2\body_bearded_injury.rvmat",
                "DeutschZ_HalftanZ\data\halftan2\body_bearded_injury2.rvmat"
            };
        };
    };

    class SurvivorF_DeutschZNaked : SurvivorF_Eva
    {
        scope = 2;
        displayName = "DeutschZ Naked";
        emptyHead = "FemaleEvaHead";
        emptyBody = "DZHZ_FemaleNakedTorso";
        emptyLegs = "DZHZ_FemaleNakedLegs";
        emptyFeet = "FemaleEvaFeet";
        emptyGloves = "FemaleEvaHands";
        faceType = "FemaleEvaHead";
    };

    class SurvivorF_DeutschZSexy : SurvivorF_Eva
    {
        scope = 2;
        displayName = "DeutschZ Sexy";
        emptyHead = "FemaleEvaHead";
        emptyBody = "DZHZ_FemaleSexyTorso";
        emptyLegs = "DZHZ_FemaleSexyLegs";
        emptyFeet = "FemaleEvaFeet";
        emptyGloves = "FemaleEvaHands";
        faceType = "FemaleEvaHead";
    };
};
