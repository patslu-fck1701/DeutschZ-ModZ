class CfgPatches
{
    class DeutschZ_DMarkZ
    {
        units[] =
        {
            "DZDM_1",
            "DZDM_2",
            "DZDM_5",
            "DZDM_10",
            "DZDM_20",
            "DZDM_50",
            "DZDM_100",
            "DZDM_200",
            "DZDM_500",
            "NoxZ_Euro1",
            "NoxZ_Euro2",
            "NoxZ_Euro5",
            "NoxZ_Euro10",
            "NoxZ_Euro20",
            "NoxZ_Euro50",
            "NoxZ_Euro100",
            "NoxZ_Euro200",
            "NoxZ_Euro500"
        };
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] =
        {
            "DZ_Data",
            // NoxZ Phone bleibt auf dem Liveserver aktiv und definiert die
            // Legacy-Namen NoxZ_Euro*. DMarkZ muss danach geladen werden,
            // damit unsere Expansion-basierten Kompatibilitaetsklassen gelten.
            "NoxZCurrency",
            "DayZExpansion_Market_Objects"
        };
    };
};

class CfgMods
{
    class DeutschZ_DMarkZ
    {
        dir = "DeutschZ_DMarkZ";
        name = "DeutschZ D-MarkZ";
        author = "DeutschZ";
        version = "2.0.0-expansion";
        type = "mod";
        dependencies[] = {};
    };
};

class CfgVehicles
{
    class ExpansionBanknoteUSD;
    class ExpansionBanknoteEuro;
    class ExpansionBanknoteHryvnia;

    // The Expansion models share one 2048x2048 money atlas. Their named
    // selections differ between the USD/Euro and Hryvnia models.
    class DZDM_ExpansionUSD_Base : ExpansionBanknoteUSD
    {
        scope = 0;
        hiddenSelections[] = {"stack", "tape"};
    };

    class DZDM_ExpansionEuro_Base : ExpansionBanknoteEuro
    {
        scope = 0;
        hiddenSelections[] = {"stack", "tape"};
    };

    class DZDM_ExpansionHryvnia_Base : ExpansionBanknoteHryvnia
    {
        scope = 0;
        hiddenSelections[] = {"hryvnia.001", "tape.007"};
    };

    class DZDM_1 : DZDM_ExpansionHryvnia_Base
    {
        scope = 2;
        displayName = "1 D-MarkZ Schein";
        descriptionShort = "Offizieller 1-D-MarkZ-Schein der DeutschZ Oekonomie.";
        hiddenSelectionsTextures[] =
        {
            "\DeutschZ_DMarkZ\data\currency\deutschz_dmarkz_expansion_1_2_500_money_co.paa",
            "\DeutschZ_DMarkZ\data\currency\deutschz_dmarkz_expansion_1_2_500_money_co.paa"
        };
    };

    class DZDM_2 : DZDM_ExpansionUSD_Base
    {
        scope = 2;
        displayName = "2 D-MarkZ Schein";
        descriptionShort = "Offizieller 2-D-MarkZ-Schein der DeutschZ Oekonomie.";
        hiddenSelectionsTextures[] =
        {
            "\DeutschZ_DMarkZ\data\currency\deutschz_dmarkz_expansion_1_2_500_money_co.paa",
            "\DeutschZ_DMarkZ\data\currency\deutschz_dmarkz_expansion_1_2_500_money_co.paa"
        };
    };

    class DZDM_5 : DZDM_ExpansionUSD_Base
    {
        scope = 2;
        displayName = "5 D-MarkZ Schein";
        descriptionShort = "Offizieller 5-D-MarkZ-Schein der DeutschZ Oekonomie.";
        hiddenSelectionsTextures[] =
        {
            "\DeutschZ_DMarkZ\data\currency\deutschz_dmarkz_expansion_5_10_20_money_co.paa",
            "\DeutschZ_DMarkZ\data\currency\deutschz_dmarkz_expansion_5_10_20_money_co.paa"
        };
    };

    class DZDM_10 : DZDM_ExpansionHryvnia_Base
    {
        scope = 2;
        displayName = "10 D-MarkZ Schein";
        descriptionShort = "Offizieller 10-D-MarkZ-Schein der DeutschZ Oekonomie.";
        hiddenSelectionsTextures[] =
        {
            "\DeutschZ_DMarkZ\data\currency\deutschz_dmarkz_expansion_5_10_20_money_co.paa",
            "\DeutschZ_DMarkZ\data\currency\deutschz_dmarkz_expansion_5_10_20_money_co.paa"
        };
    };

    class DZDM_20 : DZDM_ExpansionEuro_Base
    {
        scope = 2;
        displayName = "20 D-MarkZ Schein";
        descriptionShort = "Offizieller 20-D-MarkZ-Schein der DeutschZ Oekonomie.";
        hiddenSelectionsTextures[] =
        {
            "\DeutschZ_DMarkZ\data\currency\deutschz_dmarkz_expansion_5_10_20_money_co.paa",
            "\DeutschZ_DMarkZ\data\currency\deutschz_dmarkz_expansion_5_10_20_money_co.paa"
        };
    };

    class DZDM_50 : DZDM_ExpansionUSD_Base
    {
        scope = 2;
        displayName = "50 D-MarkZ Schein - Halftan";
        descriptionShort = "Offizieller 50-D-MarkZ-Schein der DeutschZ Oekonomie mit Halftan-Motiv.";
        hiddenSelectionsTextures[] =
        {
            "\DeutschZ_DMarkZ\data\currency\deutschz_dmarkz_expansion_50_100_200_money_co.paa",
            "\DeutschZ_DMarkZ\data\currency\deutschz_dmarkz_expansion_50_100_200_money_co.paa"
        };
    };

    class DZDM_100 : DZDM_ExpansionEuro_Base
    {
        scope = 2;
        displayName = "100 D-MarkZ Schein";
        descriptionShort = "Offizieller 100-D-MarkZ-Schein der DeutschZ Oekonomie.";
        hiddenSelectionsTextures[] =
        {
            "\DeutschZ_DMarkZ\data\currency\deutschz_dmarkz_expansion_50_100_200_money_co.paa",
            "\DeutschZ_DMarkZ\data\currency\deutschz_dmarkz_expansion_50_100_200_money_co.paa"
        };
    };

    class DZDM_200 : DZDM_ExpansionHryvnia_Base
    {
        scope = 2;
        displayName = "200 D-MarkZ Schein";
        descriptionShort = "Offizieller 200-D-MarkZ-Schein der DeutschZ Oekonomie.";
        hiddenSelectionsTextures[] =
        {
            "\DeutschZ_DMarkZ\data\currency\deutschz_dmarkz_expansion_50_100_200_money_co.paa",
            "\DeutschZ_DMarkZ\data\currency\deutschz_dmarkz_expansion_50_100_200_money_co.paa"
        };
    };

    class DZDM_500 : DZDM_ExpansionEuro_Base
    {
        scope = 2;
        displayName = "500 D-MarkZ Schein";
        descriptionShort = "Offizieller 500-D-MarkZ-Schein der DeutschZ Oekonomie.";
        hiddenSelectionsTextures[] =
        {
            "\DeutschZ_DMarkZ\data\currency\deutschz_dmarkz_expansion_1_2_500_money_co.paa",
            "\DeutschZ_DMarkZ\data\currency\deutschz_dmarkz_expansion_1_2_500_money_co.paa"
        };
    };

    // Compatibility aliases for current trader, ATM, CE and CriminalZ data.
    // They no longer inherit from a NoxZ model or require NoxZCurrency.
    class NoxZ_Euro1 : DZDM_1 {};
    class NoxZ_Euro2 : DZDM_2 {};
    class NoxZ_Euro5 : DZDM_5 {};
    class NoxZ_Euro10 : DZDM_10 {};
    class NoxZ_Euro20 : DZDM_20 {};
    class NoxZ_Euro50 : DZDM_50 {};
    class NoxZ_Euro100 : DZDM_100 {};
    class NoxZ_Euro200 : DZDM_200 {};
    class NoxZ_Euro500 : DZDM_500 {};
};
