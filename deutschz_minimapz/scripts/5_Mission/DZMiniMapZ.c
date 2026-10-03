class DZMiniMapZController
{
    protected static const int TILE_COUNT = 32;
    protected static const int GRID_RADIUS = 7;
    protected static const int GRID_SIDE = 15;
    protected static const float TILE_WORLD_SIZE = 480.0;
    protected static const float TILE_PIXEL_SIZE = 128.0;
    protected static const float GRID_CENTER = 960.0;
    protected static const float WORLD_SIZE = 15360.0;
    protected static const float FULL_MAP_PIXEL_SIZE = 4096.0;
    protected static const float FULLSCREEN_MAX_SIZE = 900.0;
    protected static const float MINI_ZOOM = 1.10;
    protected static const float ZOOM_MIN = 0.75;
    protected static const float ZOOM_MAX = 2.25;
    protected static const float ZOOM_STEP = 0.15;
    protected static const int EXPANSION_MARKER_LIMIT = 64;
    protected static const float EXPANSION_MARKER_SIZE = 20.0;

    protected Widget m_Root;
    protected Widget m_Frame;
    protected Widget m_Rotator;
    protected ImageWidget m_FullMap;
    protected MapWidget m_Map;
    protected MapWidget m_Map1080;
    protected MapWidget m_Map1440;
    protected CanvasWidget m_PlayerArrow;
    protected ref array<ImageWidget> m_Tiles = new array<ImageWidget>;
#ifdef EXPANSIONMODNAVIGATION
    protected ref array<ImageWidget> m_ExpansionMarkerWidgets = new array<ImageWidget>;
    protected ref array<string> m_ExpansionMarkerTextures = new array<string>;
    protected ref array<ImageWidget> m_FullscreenExpansionMarkerWidgets = new array<ImageWidget>;
    protected ref array<string> m_FullscreenExpansionMarkerTextures = new array<string>;
#endif
    protected bool m_UseTileMap;
    protected bool m_Visible = true;
    protected float m_UpdateElapsed;
    protected float m_ResolutionElapsed;
    protected float m_LastScreenWidth;
    protected float m_LastScreenHeight;
    protected int m_LastCenterTileX = -999;
    protected int m_LastCenterTileZ = -999;
    protected bool m_MapChordHeld;
    protected bool m_MapHoldHandled;
    protected bool m_Fullscreen;
    protected float m_MapChordDuration;
    protected float m_ZoomLevel = 1.0;
    protected float m_PixelsPerMeter = FULL_MAP_PIXEL_SIZE / WORLD_SIZE;

    bool Init()
    {
        if (!GetGame() || !GetGame().GetWorkspace()) return false;
        m_Root = GetGame().GetWorkspace().CreateWidgets("deutschz_minimapz/GUI/layouts/minimap.layout");
        if (!m_Root) return false;

        m_Rotator = m_Root.FindAnyWidget("DZMiniMapZRotator");
        m_Frame = m_Root.FindAnyWidget("DZMiniMapZFrame");
        m_Map1080 = MapWidget.Cast(m_Root.FindAnyWidget("DZMiniMapZMap1080"));
        m_Map1440 = MapWidget.Cast(m_Root.FindAnyWidget("DZMiniMapZMap1440"));
        m_PlayerArrow = CanvasWidget.Cast(m_Root.FindAnyWidget("DZMiniMapZPlayerArrow"));

        string worldName;
        GetGame().GetWorldName(worldName);
        worldName.ToLower();
        m_UseTileMap = worldName.Contains("chernarusplus") && CreateTileMap();
        if (m_UseTileMap)
        {
            m_Map1080.Show(false);
            m_Map1440.Show(false);
            m_Rotator.Show(true);
            Print("[DZMiniMapZ] Drehbare Chernarus-Satellitenkarte aktiv");
        }
        else
        {
            if (m_Rotator) m_Rotator.Show(false);
            ConfigureMapForResolution(true);
            if (!m_Map) { Destroy(); return false; }
            Print("[DZMiniMapZ] Vanilla-Karte als North-Up-Fallback aktiv");
        }
        DrawPlayerArrow();
#ifdef EXPANSIONMODNAVIGATION
        CreateExpansionMarkerWidgets();
#endif
        ApplyDisplayMode();
        return true;
    }

    protected bool CreateTileMap()
    {
        if (!m_Rotator) return false;
        m_FullMap = ImageWidget.Cast(m_Root.FindAnyWidget("DZMiniMapZFullMap"));
        if (!m_FullMap) return false;
        m_FullMap.SetFlags(WidgetFlags.STRETCH);
        m_FullMap.LoadImageFile(0, "deutschz_minimapz\\GUI\\fullmap.paa");
        m_FullMap.SetImage(0);
        m_FullMap.SetSize(FULL_MAP_PIXEL_SIZE, FULL_MAP_PIXEL_SIZE);
        return true;
    }

    void Destroy()
    {
        if (m_Root) delete m_Root;
        m_Tiles.Clear();
#ifdef EXPANSIONMODNAVIGATION
        m_ExpansionMarkerWidgets.Clear();
        m_ExpansionMarkerTextures.Clear();
        m_FullscreenExpansionMarkerWidgets.Clear();
        m_FullscreenExpansionMarkerTextures.Clear();
#endif
        m_PlayerArrow = null;
        m_FullMap = null;
        m_Frame = null;
        m_Map = null;
        m_Map1080 = null;
        m_Map1440 = null;
        m_Rotator = null;
        m_Root = null;
    }

    protected void ConfigureMapForResolution(bool force = false)
    {
        float screenWidth;
        float screenHeight;
        GetGame().GetWorkspace().GetScreenSize(screenWidth, screenHeight);
        if (!force && screenWidth == m_LastScreenWidth && screenHeight == m_LastScreenHeight) return;
        m_LastScreenWidth = screenWidth;
        m_LastScreenHeight = screenHeight;
        bool use1440Mode = screenHeight >= 1200;
        if (m_Map1080) m_Map1080.Show(!use1440Mode);
        if (m_Map1440) m_Map1440.Show(use1440Mode);
        if (use1440Mode) m_Map = m_Map1440;
        else m_Map = m_Map1080;
        if (m_Map) m_Map.SetScale(0.1);

        if (m_UseTileMap)
        {
            if (m_Map1080) m_Map1080.Show(false);
            if (m_Map1440) m_Map1440.Show(false);
        }
    }

    protected string PadTileIndex(int value)
    {
        if (value < 10) return "00" + value.ToString();
        if (value < 100) return "0" + value.ToString();
        return value.ToString();
    }

    protected void RefreshTileImages(int centerTileX, int centerTileZ)
    {
        if (centerTileX == m_LastCenterTileX && centerTileZ == m_LastCenterTileZ) return;
        m_LastCenterTileX = centerTileX;
        m_LastCenterTileZ = centerTileZ;
        int index;
        for (int row = 0; row < GRID_SIDE; row++)
        {
            int offsetZ = GRID_RADIUS - row;
            for (int column = 0; column < GRID_SIDE; column++)
            {
                int offsetX = column - GRID_RADIUS;
                int tileX = centerTileX + offsetX;
                int tileZ = centerTileZ + offsetZ;
                ImageWidget tile = m_Tiles[index++];
                if (tileX < 0 || tileX >= TILE_COUNT || tileZ < 0 || tileZ >= TILE_COUNT)
                {
                    tile.Show(false);
                    continue;
                }
                string texturePath = string.Format("deutschz_minimapz\\GUI\\maptiles\\T_%1_%2.paa", PadTileIndex(tileX), PadTileIndex(tileZ));
                tile.LoadImageFile(0, texturePath);
                tile.SetImage(0);
                tile.Show(true);
            }
        }
    }

    protected void UpdateTileMap(vector playerPosition, float heading)
    {
        if (!m_FullMap) return;
        float effectiveZoom = MINI_ZOOM;
        if (m_Fullscreen) effectiveZoom = m_ZoomLevel;
        float mapPixelSize = FULL_MAP_PIXEL_SIZE * effectiveZoom;
        m_FullMap.SetSize(mapPixelSize, mapPixelSize);
        m_FullMap.Update();
        // The image widget uses exact pixel sizing. Reading its transformed
        // size after a fullscreen/rotation change can return stale geometry
        // and offsets both the map and every marker in the mini view.
        m_PixelsPerMeter = mapPixelSize / WORLD_SIZE;
        float pixelsPerMeterZ = mapPixelSize / WORLD_SIZE;
        float fullMapLeft = GRID_CENTER - playerPosition[0] * m_PixelsPerMeter;
        float fullMapTop = GRID_CENTER - (WORLD_SIZE - playerPosition[2]) * pixelsPerMeterZ;
        m_FullMap.SetPos(fullMapLeft, fullMapTop);
        if (m_Fullscreen) m_Rotator.SetRotation(0, 0, 0, true);
        else m_Rotator.SetRotation(0, 0, -heading, true);
    }

    protected void DrawPlayerArrow()
    {
        if (!m_PlayerArrow) return;
        int color = ARGB(255, 57, 255, 20);
        m_PlayerArrow.Clear();
        m_PlayerArrow.DrawLine(10, 1, 3, 22, 3, color);
        m_PlayerArrow.DrawLine(10, 1, 17, 22, 3, color);
        m_PlayerArrow.DrawLine(3, 22, 10, 17, 3, color);
        m_PlayerArrow.DrawLine(17, 22, 10, 17, 3, color);
    }

#ifdef EXPANSIONMODNAVIGATION
    protected void CreateExpansionMarkerWidgets()
    {
        if (!m_Frame) return;
        for (int i = 0; i < EXPANSION_MARKER_LIMIT; i++)
        {
            ImageWidget markerWidget = ImageWidget.Cast(GetGame().GetWorkspace().CreateWidgets("deutschz_minimapz\\GUI\\layouts\\expansion_marker.layout", m_Frame));
            if (!markerWidget) break;
            markerWidget.SetSize(EXPANSION_MARKER_SIZE, EXPANSION_MARKER_SIZE);
            markerWidget.Show(false);
            m_ExpansionMarkerWidgets.Insert(markerWidget);
            m_ExpansionMarkerTextures.Insert("");

            ImageWidget fullscreenMarkerWidget = ImageWidget.Cast(GetGame().GetWorkspace().CreateWidgets("deutschz_minimapz\\GUI\\layouts\\expansion_marker.layout", m_Root));
            if (!fullscreenMarkerWidget) break;
            fullscreenMarkerWidget.SetSize(EXPANSION_MARKER_SIZE, EXPANSION_MARKER_SIZE);
            fullscreenMarkerWidget.SetSort(200);
            fullscreenMarkerWidget.Show(false);
            m_FullscreenExpansionMarkerWidgets.Insert(fullscreenMarkerWidget);
            m_FullscreenExpansionMarkerTextures.Insert("");
        }
    }

    protected void AddExpansionMarker(array<ExpansionMarkerData> markers, ExpansionMarkerData markerData, ExpansionMarkerModule markerModule)
    {
        if (!markerData || markers.Count() >= m_ExpansionMarkerWidgets.Count()) return;
        if (!markerData.IsMapVisible() || !markerModule.IsMapVisible(markerData.GetType())) return;
        markers.Insert(markerData);
    }

    protected void CollectExpansionMarkers(array<ExpansionMarkerData> markers, ExpansionMarkerModule markerModule)
    {
        ExpansionMarkerClientData markerData = markerModule.GetData();
        if (!markerData) return;

        int i;
        for (i = 0; i < markerData.PersonalCount(); i++)
            AddExpansionMarker(markers, markerData.PersonalGet(i), markerModule);
        for (i = 0; i < markerData.ServerCount(); i++)
            AddExpansionMarker(markers, markerData.ServerGet(i), markerModule);

#ifdef EXPANSIONMODGROUPS
        for (i = 0; i < markerData.PartyCount(); i++)
            AddExpansionMarker(markers, markerData.PartyGet(i), markerModule);
        for (i = 0; i < markerData.PartyPlayerCount(); i++)
            AddExpansionMarker(markers, markerData.PartyPlayerGet(i), markerModule);
        for (i = 0; i < markerData.PartyQuickCount(); i++)
            AddExpansionMarker(markers, markerData.PartyQuickGet(i), markerModule);
#endif
    }

    protected void HideExpansionMarkers(int firstUnused)
    {
        for (int i = firstUnused; i < m_ExpansionMarkerWidgets.Count(); i++)
            m_ExpansionMarkerWidgets[i].Show(false);
    }

    protected void HideFullscreenExpansionMarkers(int firstUnused)
    {
        for (int i = firstUnused; i < m_FullscreenExpansionMarkerWidgets.Count(); i++)
            m_FullscreenExpansionMarkerWidgets[i].Show(false);
    }

    protected void UpdateExpansionMarkers(vector playerPosition, float heading)
    {
        ExpansionMarkerModule markerModule = ExpansionMarkerModule.GetModuleInstance();
        if (!markerModule)
        {
            HideExpansionMarkers(0);
            HideFullscreenExpansionMarkers(0);
            return;
        }

        if (m_Fullscreen) HideExpansionMarkers(0);
        else HideFullscreenExpansionMarkers(0);

        array<ExpansionMarkerData> markers = new array<ExpansionMarkerData>;
        CollectExpansionMarkers(markers, markerModule);

        float frameWidth;
        float frameHeight;
        m_Frame.GetSize(frameWidth, frameHeight);
        float centerX = frameWidth * 0.5;
        float centerY = frameHeight * 0.5;
        float pixelsPerMeter = m_PixelsPerMeter;
        float radians = -heading * Math.DEG2RAD;
        float cosHeading = Math.Cos(radians);
        float sinHeading = Math.Sin(radians);
        float halfMarker = EXPANSION_MARKER_SIZE * 0.5;
        float frameScreenX;
        float frameScreenY;
        if (m_Fullscreen) m_Frame.GetScreenPos(frameScreenX, frameScreenY);
        int used = 0;

        int activeMarkerLimit = m_ExpansionMarkerWidgets.Count();
        if (m_Fullscreen) activeMarkerLimit = m_FullscreenExpansionMarkerWidgets.Count();
        for (int i = 0; i < markers.Count() && used < activeMarkerLimit; i++)
        {
            ExpansionMarkerData data = markers[i];
            vector markerPosition = data.GetPosition();
            float screenX;
            float screenY;
            if (m_Fullscreen)
            {
                screenX = frameScreenX + centerX + (markerPosition[0] - playerPosition[0]) * pixelsPerMeter;
                screenY = frameScreenY + centerY - (markerPosition[2] - playerPosition[2]) * pixelsPerMeter;
            }
            else
            {
                float dx = (markerPosition[0] - playerPosition[0]) * pixelsPerMeter;
                float dy = -(markerPosition[2] - playerPosition[2]) * pixelsPerMeter;
                float rotatedX = dx * cosHeading - dy * sinHeading;
                float rotatedY = dx * sinHeading + dy * cosHeading;
                screenX = centerX + rotatedX;
                screenY = centerY + rotatedY;
            }

            if (m_Fullscreen)
            {
                if (screenX < frameScreenX + halfMarker || screenX > frameScreenX + frameWidth - halfMarker || screenY < frameScreenY + halfMarker || screenY > frameScreenY + frameHeight - halfMarker)
                    continue;
            }
            else if (screenX < halfMarker || screenX > frameWidth - halfMarker || screenY < halfMarker || screenY > frameHeight - halfMarker)
                continue;

            string texturePath = ExpansionIcons.GetPath(data.GetIconName());
            if (texturePath == "") texturePath = data.GetIconName();
            if (texturePath == "") continue;

            ImageWidget markerWidget;
            string loadedTexture;
            if (m_Fullscreen)
            {
                markerWidget = m_FullscreenExpansionMarkerWidgets[used];
                loadedTexture = m_FullscreenExpansionMarkerTextures[used];
            }
            else
            {
                markerWidget = m_ExpansionMarkerWidgets[used];
                loadedTexture = m_ExpansionMarkerTextures[used];
            }
            if (loadedTexture != texturePath)
            {
                markerWidget.LoadImageFile(0, texturePath);
                markerWidget.SetImage(0);
                if (m_Fullscreen) m_FullscreenExpansionMarkerTextures[used] = texturePath;
                else m_ExpansionMarkerTextures[used] = texturePath;
            }
            markerWidget.SetColor(data.GetColor());
            if (m_Fullscreen) markerWidget.SetScreenPos(screenX - halfMarker, screenY - halfMarker);
            else markerWidget.SetPos(screenX - halfMarker, screenY - halfMarker);
            markerWidget.Show(true);
            used++;
        }

        if (m_Fullscreen) HideFullscreenExpansionMarkers(used);
        else HideExpansionMarkers(used);
    }
#endif

    protected void ApplyDisplayMode()
    {
        if (!m_Frame || !m_Rotator || !m_PlayerArrow) return;
        if (m_Fullscreen)
        {
            float screenWidth;
            float screenHeight;
            GetGame().GetWorkspace().GetScreenSize(screenWidth, screenHeight);
            float mapSize = screenHeight * 0.86;
            if (mapSize > screenWidth * 0.86) mapSize = screenWidth * 0.86;
            if (mapSize > FULLSCREEN_MAX_SIZE) mapSize = FULLSCREEN_MAX_SIZE;
            m_Frame.SetSize(mapSize, mapSize);
            m_Rotator.SetPos(mapSize * 0.5 - GRID_CENTER, mapSize * 0.5 - GRID_CENTER);
            m_PlayerArrow.SetPos(mapSize * 0.5 - 10, mapSize * 0.5 - 14);
            ConfigureMapForResolution(true);
            if (m_UseTileMap)
            {
                if (m_Map1080) m_Map1080.Show(false);
                if (m_Map1440) m_Map1440.Show(false);
            }
        }
        else
        {
            m_Frame.SetSize(260, 260);
            m_Rotator.SetPos(130 - GRID_CENTER, 130 - GRID_CENTER);
            m_PlayerArrow.SetPos(120, 114);
            if (m_UseTileMap)
            {
                if (m_Map1080) m_Map1080.Show(false);
                if (m_Map1440) m_Map1440.Show(false);
            }
        }
    }

    protected void HandleFullscreenZoom()
    {
        if (!m_Fullscreen || !m_UseTileMap) return;
        Input input = GetGame().GetInput();
        if (!input) return;
        if (input.LocalPress("UADZMiniMapZZoomIn", false))
        {
            m_ZoomLevel += ZOOM_STEP;
            if (m_ZoomLevel > ZOOM_MAX) m_ZoomLevel = ZOOM_MAX;
        }
        if (input.LocalPress("UADZMiniMapZZoomOut", false))
        {
            m_ZoomLevel -= ZOOM_STEP;
            if (m_ZoomLevel < ZOOM_MIN) m_ZoomLevel = ZOOM_MIN;
        }
    }

    void Update(float timeslice)
    {
        if (!m_Root || !GetGame()) return;
        bool controlHeld = KeyState(KeyCode.KC_LCONTROL) == 1 || KeyState(KeyCode.KC_RCONTROL) == 1;
        bool mapChordDown = controlHeld && KeyState(KeyCode.KC_N) == 1;
        if (mapChordDown)
        {
            if (!m_MapChordHeld)
            {
                m_MapChordDuration = 0;
                m_MapHoldHandled = false;
            }
            m_MapChordDuration += timeslice;
            if (m_MapChordDuration >= 0.65 && !m_MapHoldHandled)
            {
                m_MapHoldHandled = true;
                m_Visible = !m_Visible;
            }
        }
        else if (m_MapChordHeld)
        {
            if (!m_MapHoldHandled)
            {
                m_Fullscreen = !m_Fullscreen;
                if (!m_Fullscreen) m_ZoomLevel = 1.0;
                m_Visible = true;
                ApplyDisplayMode();
            }
        }
        m_MapChordHeld = mapChordDown;
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        bool shouldShow = m_Visible && player && player.IsAlive() && !GetGame().GetUIManager().GetMenu();
        m_Root.Show(shouldShow);
        if (!shouldShow) return;
        HandleFullscreenZoom();

        m_ResolutionElapsed += timeslice;
        if (m_ResolutionElapsed >= 1.0) { m_ResolutionElapsed = 0; ConfigureMapForResolution(false); }
        m_UpdateElapsed += timeslice;
        if (m_UpdateElapsed < 0.05) return;
        m_UpdateElapsed = 0;

        vector playerPosition = player.GetWorldPosition();
        float heading = GetGame().GetCurrentCameraDirection().VectorToAngles()[0];
        if (m_UseTileMap)
        {
            UpdateTileMap(playerPosition, heading);
#ifdef EXPANSIONMODNAVIGATION
            if (m_Fullscreen) UpdateExpansionMarkers(playerPosition, 0);
            else UpdateExpansionMarkers(playerPosition, heading);
#endif
        }
        else if (m_Map) { m_Map.SetMapPos(playerPosition); m_Map.SetScale(0.1); }
    }
}

modded class MissionGameplay
{
    protected ref DZMiniMapZController m_DZMiniMapZ;
    protected float m_DZMiniMapZInitDelay;

    override void OnMissionStart()
    {
        super.OnMissionStart();
        m_DZMiniMapZInitDelay = 1.0;
    }

    override void OnUpdate(float timeslice)
    {
        super.OnUpdate(timeslice);
        if (!m_DZMiniMapZ)
        {
            m_DZMiniMapZInitDelay -= timeslice;
            if (m_DZMiniMapZInitDelay <= 0 && GetGame().GetPlayer())
            {
                m_DZMiniMapZ = new DZMiniMapZController;
                if (!m_DZMiniMapZ.Init()) m_DZMiniMapZ = null;
            }
        }
        if (m_DZMiniMapZ) m_DZMiniMapZ.Update(timeslice);
    }

    override void OnMissionFinish()
    {
        if (m_DZMiniMapZ) m_DZMiniMapZ.Destroy();
        m_DZMiniMapZ = null;
        super.OnMissionFinish();
    }
}
