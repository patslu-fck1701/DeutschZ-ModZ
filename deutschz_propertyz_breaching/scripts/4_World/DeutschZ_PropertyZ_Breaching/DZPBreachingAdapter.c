modded class HDSN_ActionDeployBreachingCharge
{
    private DZP_PropertyDoorController DZP_FindAimedController(PlayerBase player, HDSN_BreachingChargeBase charge)
    {
        if (!player || !charge || !g_Game) return null;

        vector cameraPosition = g_Game.GetCurrentCameraPosition();
        vector cameraDirection = g_Game.GetCurrentCameraDirection().Normalized();
        float maxDistance = charge.GetPlacementDistance() + 1.5;
        array<Object> objects = new array<Object>;
        array<CargoBase> cargos = new array<CargoBase>;
        g_Game.GetObjectsAtPosition3D(player.GetPosition(), maxDistance + 2.0, objects, cargos);

        DZP_PropertyDoorController best;
        float bestLineDistance = 999999.0;
        foreach (Object object : objects)
        {
            DZP_PropertyDoorController candidate = DZP_PropertyDoorController.Cast(object);
            if (!candidate || candidate.IsRuined()) continue;

            vector toCandidate = candidate.GetPosition() - cameraPosition;
            float forwardDistance = vector.Dot(toCandidate, cameraDirection);
            if (forwardDistance < 0.0 || forwardDistance > maxDistance) continue;

            vector closestPoint = cameraPosition + (cameraDirection * forwardDistance);
            float lineDistance = vector.DistanceSq(candidate.GetPosition(), closestPoint);
            if (lineDistance > 1.0 || lineDistance >= bestLineDistance) continue;
            best = candidate;
            bestLineDistance = lineDistance;
        }
        return best;
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        bool baseAllowed = super.ActionCondition(player, target, item);
        DZP_PropertyDoorController controller = DZP_PropertyDoorController.Cast(raycastTarget);
        HDSN_BreachingChargeBase charge = HDSN_BreachingChargeBase.Cast(item);
        if (!controller && g_Game.IsClient() && player && player.IsPlacingLocal())
            controller = DZP_FindAimedController(player, charge);
        if (!controller) return baseAllowed;
        if (!charge || !HDSN_DestructionManager.GetInstance().IsRaidingAllowed()) return false;

        vector snapPosition;
        vector snapOrientation;
        if (!controller.DZP_GetBreachingTransform(item.GetType(), snapPosition, snapOrientation)) return false;
        contactPos = snapPosition;
        contactDir = snapOrientation.AnglesToVector();
        raycastTarget = controller;
        charge.SetTarget(controller);
        actionText = GetCustomActionTextPlanting(controller.GetDisplayName(), item.GetDisplayName());
        return true;
    }
}

modded class HDSN_DestructionManager
{
    override bool IsChargeAllowed(Object target, Object charge, int contactComponent = 0, int level = 0, int wall = 0)
    {
        if (DZP_PropertyDoorController.Cast(target) && HDSN_BreachingChargeBase.Cast(charge))
            return true;
        return super.IsChargeAllowed(target, charge, contactComponent, level, wall);
    }

    override void DestroyTarget(Object target, PlayerBase placingPlayer, HDSN_BreachingChargeBase charge)
    {
        DZP_PropertyDoorController controller = DZP_PropertyDoorController.Cast(target);
        if (controller)
        {
            DZPPropertyControllerService.Get().OnBreached(controller);
            return;
        }
        super.DestroyTarget(target, placingPlayer, charge);
    }
}
