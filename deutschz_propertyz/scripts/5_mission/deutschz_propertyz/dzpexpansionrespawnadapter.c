modded class ExpansionRespawnHandlerModule
{
    private ref map<string, ref DZPPropertyRecord> m_DZP_HouseSpawnKeys = new map<string, ref DZPPropertyRecord>;

    override bool ProcessCooldown(PlayerIdentity sender, string locKey, bool isTerritory, bool useCooldown, bool addCooldown = true)
    {
        DZPPropertyRecord property;
        if (IsMissionHost() && isTerritory && m_DZP_HouseSpawnKeys.Find(locKey, property))
        {
            DZPSettings settings = DZPSettingsService.Get();
            if (!sender || !settings || !settings.EnableExpansionHouseRespawn || !property || property.OwnerId != sender.GetId() || property.Raided) return true;
            return DZPHouseRespawnStore.Get().IsBlocked(sender.GetId(), addCooldown);
        }
        return super.ProcessCooldown(sender, locKey, isTerritory, useCooldown, addCooldown);
    }

    override array<ref ExpansionSpawnLocation> GetTerritoryList(string playerUID)
    {
        array<ref ExpansionSpawnLocation> locations = super.GetTerritoryList(playerUID);
        DZPSettings settings = DZPSettingsService.Get();
        if (!settings || !settings.EnableExpansionHouseRespawn || !IsMissionHost()) return locations;

        array<ref DZPPropertyRecord> properties = DZPPropertyStore.Get().GetAll();
        for (int i = 0; i < properties.Count(); i++)
        {
            DZPPropertyRecord record = properties[i];
            if (!record || record.OwnerId != playerUID || record.Raided) continue;
            DZPHouseTypeSetting houseSetting = DZPSettingsService.Find(record.Type);
            if (!houseSetting || houseSetting.RespawnOffset == vector.Zero) continue;

            array<Object> objects = new array<Object>;
            array<CargoBase> cargos = new array<CargoBase>;
            g_Game.GetObjectsAtPosition3D(record.Position, 3.0, objects, cargos);
            foreach (Object object : objects)
            {
                BuildingBase building = BuildingBase.Cast(object);
                if (!building || building.GetType() != record.Type) continue;
                array<vector> positions = new array<vector>;
                positions.Insert(building.ModelToWorld(houseSetting.RespawnOffset));
                ExpansionSpawnLocation location = new ExpansionSpawnLocation;
                string houseLabel = "PropertyZ - " + record.OwnerName + " (1x / 24h)";
                if (DZPHouseRespawnStore.Get().IsBlocked(playerUID, false)) houseLabel += " - gesperrt";
                location.SetLocation(houseLabel, positions, 2000000000 - i);
                // Keep list indices stable while blocked; enforce our own cooldown on the server.
                // Expansion's generic territory cooldown must not override the 24-hour rule.
                location.SetUseCooldown(false);
                m_DZP_HouseSpawnKeys.Set(location.GetKey(), record);
                locations.Insert(location);
                break;
            }
        }
        return locations;
    }
}
