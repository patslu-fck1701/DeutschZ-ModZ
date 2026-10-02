class CfgPatches
{
    class deutschz_minimapz
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] = {"DZ_Data"};
    };
};

class RscMapControl
{
    scaleMin = 0.05;
    scaleMax = 0.85;
    scaleDefault = 0.1;
    ptsPerSquareSea = 8;
    ptsPerSquareTxt = 10;
    ptsPerSquareCLn = 10;
    ptsPerSquareExp = 10;
    ptsPerSquareCost = 10;
    ptsPerSquareFor = 99;
    ptsPerSquareForEdge = 99;
    ptsPerSquareRoad = 4;
    ptsPerSquareObj = 15;
    maxSatelliteAlpha = 1;
    alphaFadeStartScale = 1;
    alphaFadeEndScale = 1;
    userMapPath = "dz\gear\navigation\data\usermap";
    maxUserMapAlpha = 0.2;
    alphaUserMapFadeStartScale = 0.5;
    alphaUserMapFadeEndScale = 0.8;
    showCountourInterval = 1;
    colorLevels[] = {0.65,0.6,0.45,0.3};
    colorSea[] = {0.2,0.5,0.7,1};
    colorForest[] = {0.36,0.78,0.08,0};
    colorRocks[] = {0.5,0.5,0.5,0.2};
    colorCountlines[] = {0.85,0.8,0.65,0.1};
    colorMainCountlines[] = {0.45,0.4,0.25,0};
    colorCountlinesWater[] = {0.25,0.4,0.5,0.3};
    colorMainCountlinesWater[] = {0.25,0.4,0.5,0.9};
    colorPowerLines[] = {0.1,0.1,0.1,1};
    colorRailWay[] = {0.8,0.2,0,1};
    colorForestBorder[] = {0.4,0.8,0,0};
    colorRocksBorder[] = {0.5,0.5,0.5,0};
    colorOutside[] = {1,1,1,1};
    colorTracks[] = {0.78,0.66,0.34,1};
    colorRoads[] = {0.69,0.43,0.23,1};
    colorMainRoads[] = {0.53,0.35,0,1};
    colorTracksFill[] = {0.96,0.91,0.6,1};
    colorRoadsFill[] = {0.92,0.73,0.41,1};
    colorMainRoadsFill[] = {0.84,0.61,0.21,1};
    colorGrid[] = {0,0,0,0};
    colorGridMap[] = {0,0,0,0};
    fontNames = "gui/fonts/sdf_MetronBook24";
    sizeExNames = 0.03;
    colorNames[] = {1,1,1,1};
    fontGrid = "gui/fonts/sdf_MetronBook24";
    sizeExGrid = 0.02;
    fontLevel = "gui/fonts/sdf_MetronBook24";
    sizeExLevel = 0.01;
    colorMountPoint[] = {0.45,0.4,0.25,0};
    mapPointDensity = 0.12;
    text = "";
    fontLabel = "gui/fonts/sdf_MetronBook24";
    fontInfo = "gui/fonts/sdf_MetronBook24";

    class Legend
    {
        x = 0.05;
        y = 0.85;
        w = 0.4;
        h = 0.1;
        font = "gui/fonts/sdf_MetronBook24";
        sizeEx = 0.02;
        colorBackground[] = {1,1,1,0};
        color[] = {0,0,0,0};
    };
};

class CfgMods
{
    class deutschz_minimapz
    {
        dir = "deutschz_minimapz";
        name = "DeutschZ MiniMapZ";
        credits = "TH, Rauuuul; DeutschZ maintenance";
        author = "DeutschZ";
        version = "2.0.0";
        type = "mod";
        inputs = "deutschz_minimapz\inputs\inputs.xml";
        dependencies[] = {"Mission"};

        class defs
        {
            class missionScriptModule
            {
                value = "";
                files[] = {"deutschz_minimapz\scripts\5_Mission"};
            };
        };
    };
};
