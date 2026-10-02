modded class CodeLock
{
    override void SynchLock(string owner, array<string> guests)
    {
        super.SynchLock(owner, guests);
        DZP_PropertyDoorController controller = DZP_PropertyDoorController.Cast(GetHierarchyParent());
        PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());
        if (controller && player && player.GetIdentity())
        {
            string uid = player.GetIdentity().GetId();
            controller.DZP_SetLocalPinAuthorized(IsOwner(uid) || IsGuest(uid));
        }
    }

    override void EEHealthLevelChanged(int oldLevel, int newLevel, string zone)
    {
        super.EEHealthLevelChanged(oldLevel, newLevel, zone);
        DZP_PropertyDoorController controller = DZP_PropertyDoorController.Cast(GetHierarchyParent());
        if (controller)
        {
            if (IsRuined()) controller.DZP_SetLocalPinAuthorized(false);
            controller.DZP_UpdateControllerVisual();
        }
    }

    private bool DZP_ShowOnController()
    {
        DZP_PropertyDoorController controller = DZP_PropertyDoorController.Cast(GetHierarchyParent());
        if (!controller) return false;
        SetInvisible(false);
        OnInvisibleSet(false);
        SetScale(1.0);
        SetAnimationPhase("Combination_Lock_Item", 0);
        SetAnimationPhase("Lock_Item_1", 0);
        SetAnimationPhase("Lock_Item_2", 0);
        HideAttached();
        SetPosition(controller.GetPosition());
        SetOrientation(controller.GetOrientation());
        controller.DZP_UpdateControllerVisual();
        return true;
    }

    override void UpdateVisuals()
    {
        if (DZP_ShowOnController()) return;
        SetInvisible(false);
        OnInvisibleSet(false);
        SetScale(1.0);
        super.UpdateVisuals();
    }

    override protected void ShowItem()
    {
        DZP_PropertyDoorController controller = DZP_PropertyDoorController.Cast(GetHierarchyParent());
        if (controller)
        {
            SetAnimationPhase("Combination_Lock_Item", 0);
            SetAnimationPhase("Lock_Item_1", 0);
            SetAnimationPhase("Lock_Item_2", 0);
            return;
        }
        super.ShowItem();
    }

    override protected void ShowAttached()
    {
        DZP_PropertyDoorController controller = DZP_PropertyDoorController.Cast(GetHierarchyParent());
        if (controller)
        {
            HideAttached();
            return;
        }
        super.ShowAttached();
    }
}
