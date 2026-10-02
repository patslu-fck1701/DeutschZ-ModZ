modded class VicinityItemManager
{
    override void RefreshVicinityItems()
    {
        super.RefreshVicinityItems();

        PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());
        if (!player) return;

        array<EntityAI> vicinityItems = GetVicinityItems();
        if (!vicinityItems) return;

        array<Object> objects = new array<Object>;
        array<CargoBase> cargos = new array<CargoBase>;
        g_Game.GetObjectsAtPosition3D(player.GetPosition(), 35.0, objects, cargos);

        foreach (Object object : objects)
        {
            DZP_PropertyDoorController controller = DZP_PropertyDoorController.Cast(object);
            if (!controller || !controller.CanDisplayCargo()) continue;
            if (vicinityItems.Find(controller) == INDEX_NOT_FOUND)
                vicinityItems.Insert(controller);
        }
    }
}
