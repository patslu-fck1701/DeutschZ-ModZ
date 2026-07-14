class CfgPatches
{
    class DeutschZ_DMarkZ_Test
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
    class DeutschZ_DMarkZ_Test
    {
        dir = "DeutschZ_DMarkZ_Test";
        name = "DeutschZ D-MarkZ Test";
        author = "DeutschZ";
        version = "0.1.0-test";
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
        descriptionShort = "DeutschZ Testwaehrung. Noch nicht fuer die Live-Oekonomie freigegeben.";
        hiddenSelectionsTextures[] = {"\DeutschZ_DMarkZ_Test\data\currency\deutschz_dmarkz_1_co.paa"};
    };

    class NoxZ_Euro2 : NoxZ_Currency_Coin_Base
    {
        displayName = "2 D-MarkZ Muenze";
        descriptionShort = "DeutschZ Testwaehrung. Noch nicht fuer die Live-Oekonomie freigegeben.";
        hiddenSelectionsTextures[] = {"\DeutschZ_DMarkZ_Test\data\currency\deutschz_dmarkz_2_co.paa"};
    };

    class NoxZ_Euro5 : NoxZ_Euro_Base
    {
        displayName = "5 D-MarkZ Schein";
        descriptionShort = "DeutschZ Testwaehrung. Noch nicht fuer die Live-Oekonomie freigegeben.";
        hiddenSelectionsTextures[] = {"\DeutschZ_DMarkZ_Test\data\currency\deutschz_dmarkz_5_co.paa"};
    };

    class NoxZ_Euro10 : NoxZ_Euro_Base
    {
        displayName = "10 D-MarkZ Schein";
        descriptionShort = "DeutschZ Testwaehrung. Noch nicht fuer die Live-Oekonomie freigegeben.";
        hiddenSelectionsTextures[] = {"\DeutschZ_DMarkZ_Test\data\currency\deutschz_dmarkz_10_co.paa"};
    };

    class NoxZ_Euro20 : NoxZ_Euro_Base
    {
        displayName = "20 D-MarkZ Schein";
        descriptionShort = "DeutschZ Testwaehrung. Noch nicht fuer die Live-Oekonomie freigegeben.";
        hiddenSelectionsTextures[] = {"\DeutschZ_DMarkZ_Test\data\currency\deutschz_dmarkz_20_co.paa"};
    };

    class NoxZ_Euro50 : NoxZ_Euro_Base
    {
        displayName = "50 D-MarkZ Schein - Halftan";
        descriptionShort = "DeutschZ Testwaehrung mit Halftan-Motiv. Noch nicht fuer die Live-Oekonomie freigegeben.";
        hiddenSelectionsTextures[] = {"\DeutschZ_DMarkZ_Test\data\currency\deutschz_dmarkz_50_co.paa"};
    };

    class NoxZ_Euro100 : NoxZ_Euro_Base
    {
        displayName = "100 D-MarkZ Schein - Patrick";
        descriptionShort = "DeutschZ Testwaehrung mit Patrick-Motiv. Noch nicht fuer die Live-Oekonomie freigegeben.";
        hiddenSelectionsTextures[] = {"\DeutschZ_DMarkZ_Test\data\currency\deutschz_dmarkz_100_co.paa"};
    };

    class NoxZ_Euro200 : NoxZ_Euro_Base
    {
        displayName = "200 D-MarkZ Schein";
        descriptionShort = "DeutschZ Testwaehrung. Noch nicht fuer die Live-Oekonomie freigegeben.";
        hiddenSelectionsTextures[] = {"\DeutschZ_DMarkZ_Test\data\currency\deutschz_dmarkz_200_co.paa"};
    };

    class NoxZ_Euro500 : NoxZ_Euro_Base
    {
        displayName = "500 D-MarkZ Schein - Patrick";
        descriptionShort = "DeutschZ Testwaehrung mit Patrick-Motiv. Noch nicht fuer die Live-Oekonomie freigegeben.";
        hiddenSelectionsTextures[] = {"\DeutschZ_DMarkZ_Test\data\currency\deutschz_dmarkz_500_co.paa"};
    };
};
