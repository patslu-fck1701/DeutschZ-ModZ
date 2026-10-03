class DZFuelZManager
{
    private static ref DZFuelZManager s_Instance;
    private ref array<Object> m_SpawnedNPCs = new array<Object>;
    private ref array<Object> m_SpawnedBuildings = new array<Object>;

    static DZFuelZManager Get()
    {
        if (!s_Instance)
            s_Instance = new DZFuelZManager;
        return s_Instance;
    }

    protected Object FindFuelShop(vector searchPosition, string className)
    {
        array<Object> objects = new array<Object>;
        array<CargoBase> cargo = new array<CargoBase>;
        GetGame().GetObjectsAtPosition3D(searchPosition, 5.0, objects, cargo);

        Object closest;
        float closestDistance = 6.0;
        foreach (Object candidate : objects)
        {
            if (!candidate || candidate.GetType() != className)
                continue;

            float distance = vector.Distance(candidate.GetPosition(), searchPosition);
            if (distance < closestDistance)
            {
                closest = candidate;
                closestDistance = distance;
            }
        }
        return closest;
    }

    protected bool ResolveNPCTransform(DZFuelZStation station, DZFuelZSettings settings, out vector position, out vector orientation)
    {
        if (station.Position != "" && station.Orientation != "")
        {
            position = station.Position.ToVector();
            orientation = station.Orientation.ToVector();
            return true;
        }

        Object fuelShop = FindFuelShop(station.BuildingPosition.ToVector(), settings.FuelShopClassname);
        if (!fuelShop)
            return false;

        position = fuelShop.ModelToWorld(settings.NPCLocalOffset.ToVector());
        orientation = fuelShop.GetOrientation();
        orientation[0] = orientation[0] + settings.NPCRelativeYaw;
        return true;
    }

    protected bool EnsureStationBuilding(DZFuelZStation station, DZFuelZSettings settings)
    {
        if (!station.PlaceBuilding)
            return true;

        vector buildingPosition = station.BuildingPosition.ToVector();
        if (FindFuelShop(buildingPosition, settings.FuelShopClassname))
            return true;

        Object building = GetGame().CreateObjectEx(settings.FuelShopClassname, buildingPosition, ECE_SETUP | ECE_KEEPHEIGHT | ECE_NOLIFETIME | ECE_NOPERSISTENCY_WORLD);
        if (!building)
            return false;

        building.SetPosition(buildingPosition);
        building.SetOrientation(station.BuildingOrientation.ToVector());
        m_SpawnedBuildings.Insert(building);
        Print(string.Format("[FuelZ] fuel shop placed position=%1 orientation=%2", buildingPosition.ToString(), station.BuildingOrientation));
        return true;
    }

    protected void EquipFuelAttendant(Object npcObject)
    {
        PlayerBase npc = PlayerBase.Cast(npcObject);
        if (!npc)
            return;

        ref array<string> outfit = {
            "BomberJacket_Grey",
            "ReflexVest",
            "CargoPants_Black",
            "WorkingGloves_Black",
            "WorkingBoots_Grey",
            "BaseballCap_Black",
            "AviatorGlasses",
            "HipPack_Black"
        };

        foreach (string className : outfit)
        {
            if (!npc.GetInventory().CreateAttachment(className))
                Print("[FuelZ] WARNING NPC outfit attachment failed: " + className);
        }
    }

    void Init()
    {
        DZFuelZSettings settings = DZFuelZSettingsService.Get();
        DZFuelZLivePriceService.Start();
        int spawned = 0;
        foreach (DZFuelZStation station : settings.Stations)
        {
            if (!EnsureStationBuilding(station, settings))
            {
                Print("[FuelZ] ERROR fuel shop could not be placed at " + station.BuildingPosition);
                continue;
            }

            vector position;
            vector orientation;
            if (!ResolveNPCTransform(station, settings, position, orientation))
            {
                Print("[FuelZ] ERROR fuel shop not found at " + station.BuildingPosition);
                continue;
            }

            Object npc = GetGame().CreateObjectEx(settings.NPCClassname, position, ECE_CREATEPHYSICS | ECE_KEEPHEIGHT | ECE_NOLIFETIME | ECE_DYNAMIC_PERSISTENCY);
            if (!npc)
            {
                Print("[FuelZ] ERROR NPC could not be created at " + position.ToString());
                continue;
            }

            npc.SetPosition(position);
            npc.SetOrientation(orientation);
            EquipFuelAttendant(npc);
            m_SpawnedNPCs.Insert(npc);
            spawned++;
            Print(string.Format("[FuelZ] NPC spawned position=%1 orientation=%2", position.ToString(), orientation.ToString()));
        }
        Print(string.Format("[FuelZ] INIT complete spawned=%1 configured=%2", spawned, settings.Stations.Count()));
    }

    void Cleanup()
    {
        foreach (Object npc : m_SpawnedNPCs)
        {
            if (npc)
                GetGame().ObjectDelete(npc);
        }
        m_SpawnedNPCs.Clear();
        foreach (Object building : m_SpawnedBuildings)
        {
            if (building)
                GetGame().ObjectDelete(building);
        }
        m_SpawnedBuildings.Clear();
    }
};

modded class MissionServer
{
    override void OnMissionStart()
    {
        super.OnMissionStart();
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DZFuelZ_Init, 1000, false);
    }

    protected void DZFuelZ_Init()
    {
        if (GetGame() && GetGame().IsServer())
            DZFuelZManager.Get().Init();
    }

    override void OnMissionFinish()
    {
        DZFuelZManager.Get().Cleanup();
        super.OnMissionFinish();
    }
};
