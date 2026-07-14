class CfgPatches
{
    class DeutschZ_DMarkZ
    {
        units[] =
        {
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
            "NoxZCurrency"
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
        version = "1.0.0";
        type = "mod";
        dependencies[] = {};
    };
};

class CfgVehicles
{
    class NoxZ_Currency_Coin_Base;
    class NoxZ_Euro_Base;

    class NoxZ_Euro1 : NoxZ_Currency_Coin_Base
    {
        displayName = "1 D-MarkZ Muenze";
        descriptionShort = "Offizielle 1-D-MarkZ-Muenze der DeutschZ Oekonomie.";
        hiddenSelectionsTextures[] = {"\DeutschZ_DMarkZ\data\currency\deutschz_dmarkz_1_co.paa"};
    };

    class NoxZ_Euro2 : NoxZ_Currency_Coin_Base
    {
        displayName = "2 D-MarkZ Muenze";
        descriptionShort = "Offizielle 2-D-MarkZ-Muenze der DeutschZ Oekonomie.";
        hiddenSelectionsTextures[] = {"\DeutschZ_DMarkZ\data\currency\deutschz_dmarkz_2_co.paa"};
    };

    class NoxZ_Euro5 : NoxZ_Euro_Base
    {
        displayName = "5 D-MarkZ Schein";
        descriptionShort = "Offizieller 5-D-MarkZ-Schein der DeutschZ Oekonomie.";
        hiddenSelectionsTextures[] = {"\DeutschZ_DMarkZ\data\currency\deutschz_dmarkz_5_co.paa"};
    };

    class NoxZ_Euro10 : NoxZ_Euro_Base
    {
        displayName = "10 D-MarkZ Schein";
        descriptionShort = "Offizieller 10-D-MarkZ-Schein der DeutschZ Oekonomie.";
        hiddenSelectionsTextures[] = {"\DeutschZ_DMarkZ\data\currency\deutschz_dmarkz_10_co.paa"};
    };

    class NoxZ_Euro20 : NoxZ_Euro_Base
    {
        displayName = "20 D-MarkZ Schein";
        descriptionShort = "Offizieller 20-D-MarkZ-Schein der DeutschZ Oekonomie.";
        hiddenSelectionsTextures[] = {"\DeutschZ_DMarkZ\data\currency\deutschz_dmarkz_20_co.paa"};
    };

    class NoxZ_Euro50 : NoxZ_Euro_Base
    {
        displayName = "50 D-MarkZ Schein - Halftan";
        descriptionShort = "Offizieller 50-D-MarkZ-Schein der DeutschZ Oekonomie mit Halftan-Motiv.";
        hiddenSelectionsTextures[] = {"\DeutschZ_DMarkZ\data\currency\deutschz_dmarkz_50_co.paa"};
    };

    class NoxZ_Euro100 : NoxZ_Euro_Base
    {
        displayName = "100 D-MarkZ Schein - Patrick";
        descriptionShort = "Offizieller 100-D-MarkZ-Schein der DeutschZ Oekonomie mit Patrick-Motiv.";
        hiddenSelectionsTextures[] = {"\DeutschZ_DMarkZ\data\currency\deutschz_dmarkz_100_co.paa"};
    };

    class NoxZ_Euro200 : NoxZ_Euro_Base
    {
        displayName = "200 D-MarkZ Schein";
        descriptionShort = "Offizieller 200-D-MarkZ-Schein der DeutschZ Oekonomie.";
        hiddenSelectionsTextures[] = {"\DeutschZ_DMarkZ\data\currency\deutschz_dmarkz_200_co.paa"};
    };

    class NoxZ_Euro500 : NoxZ_Euro_Base
    {
        displayName = "500 D-MarkZ Schein - Patrick";
        descriptionShort = "Offizieller 500-D-MarkZ-Schein der DeutschZ Oekonomie mit Patrick-Motiv.";
        hiddenSelectionsTextures[] = {"\DeutschZ_DMarkZ\data\currency\deutschz_dmarkz_500_co.paa"};
    };
};
