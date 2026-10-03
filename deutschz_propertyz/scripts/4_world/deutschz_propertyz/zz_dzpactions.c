class DZPActionOpenPropertyMenu : ActionInteractBase
{
    void DZPActionOpenPropertyMenu()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
        m_HUDCursorIcon = CursorIcons.CloseHood;
    }

    override void CreateConditionComponents()
    {
        m_ConditionItem = new CCINone;
        m_ConditionTarget = new CCTCursor;
    }

    override string GetText()
    {
        return "PropertyZ öffnen";
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (!player || !target || !IsBuilding(target))
            return false;

        BuildingBase building = ResolveBuilding(target);
        if (!building || !building.DZPIsSupported())
            return false;

        return IsInReach(player, target, DZPSettingsService.Get().InteractionDistance);
    }

    override void OnStartClient(ActionData action_data)
    {
        super.OnStartClient(action_data);
        Print("[PropertyZ] Action pressed - waiting for action cleanup");
    }

    override void OnEndClient(ActionData action_data)
    {
        super.OnEndClient(action_data);

        if (!action_data || !action_data.m_Target)
        {
            Print("[PropertyZ] ERROR: ActionData or target is null");
            return;
        }

        BuildingBase building = ResolveBuilding(action_data.m_Target);
        if (!building)
        {
            Print("[PropertyZ] ERROR: Target is not a BuildingBase");
            return;
        }

        // ActionInteractBase closes active scripted menus while finishing the
        // interaction. Queue the menu only after OnEndClient cleanup.
        GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(OpenPropertyMenu, 50, false, building);
    }

    private void OpenPropertyMenu(BuildingBase building)
    {
        if (!GetGame() || !building)
            return;

        if (DZPPropertyMenu.s_Active || GetGame().GetUIManager().GetMenu())
        {
            Print("[PropertyZ] Property menu or another menu is already active");
            return;
        }

        DZPPropertyMenu menu = DZPPropertyMenu.Cast(GetGame().GetUIManager().EnterScriptedMenu(DZP_PROPERTY_MENU_ID, null));
        if (!menu)
        {
            Print("[PropertyZ] ERROR: EnterScriptedMenu returned null");
            return;
        }

        menu.SetBuilding(building);
        Print("[PropertyZ] Property menu shown - requesting server state");
        GetGame().RPCSingleParam(building, DZ_PROPERTYZ_RPC.REQUEST_PROPERTY, null, true);
    }

    private BuildingBase ResolveBuilding(ActionTarget target)
    {
        if (!target)
            return null;

        BuildingBase building = BuildingBase.Cast(target.GetObject());
        if (!building)
            building = BuildingBase.Cast(target.GetParent());

        return building;
    }

    override bool IsLockTargetOnUse()
    {
        return false;
    }
}

class DZPActionExitLockedProperty : ActionInteractBase
{
    void DZPActionExitLockedProperty()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
        m_Text = "Tür von innen öffnen";
    }

    override void CreateConditionComponents()
    {
        m_ConditionItem = new CCINone;
        m_ConditionTarget = new CCTCursor;
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (!player || !target || !IsBuilding(target) || !IsInReach(player, target, UAMaxDistances.DEFAULT)) return false;
        BuildingBase building = BuildingBase.Cast(target.GetObject());
        if (!building || !building.DZPIsSupported()) return false;
        int doorIndex = building.GetDoorIndex(target.GetComponentIndex());
        if (doorIndex < 0 || !building.IsDoorLocked(doorIndex)) return false;
        return DZPPropertyControllerService.Get().IsPlayerInside(building, player, doorIndex);
    }

    override void OnStartServer(ActionData action_data)
    {
        super.OnStartServer(action_data);
        if (!action_data || !action_data.m_Target) return;
        BuildingBase building = BuildingBase.Cast(action_data.m_Target.GetObject());
        if (!building) return;
        int doorIndex = building.GetDoorIndex(action_data.m_Target.GetComponentIndex());
        if (doorIndex >= 0) DZPPropertyControllerService.Get().OpenInsideExit(building, action_data.m_Player, doorIndex);
    }

    override bool IsLockTargetOnUse()
    {
        return false;
    }
}

modded class ActionConstructor
{
    override void RegisterActions(TTypenameArray actions)
    {
        super.RegisterActions(actions);
        actions.Insert(DZPActionOpenPropertyMenu);
    }
}

modded class PlayerBase
{
    override void SetActions(out TInputActionMap InputActionMap)
    {
        super.SetActions(InputActionMap);
        AddAction(DZPActionOpenPropertyMenu, InputActionMap);
    }
}

modded class ActionOpenDoors
{
    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (super.ActionCondition(player, target, item)) return true;
        if (!player || !target || !IsBuilding(target) || !IsInReach(player, target, UAMaxDistances.DEFAULT)) return false;

        BuildingBase building = BuildingBase.Cast(target.GetObject());
        if (!building || !building.DZPIsSupported()) return false;
        int doorIndex = building.GetDoorIndex(target.GetComponentIndex());
        if (doorIndex < 0 || !building.IsDoorLocked(doorIndex)) return false;
        return DZPPropertyControllerService.Get().CanOpenPropertyDoor(building, player, doorIndex);
    }

    override void OnStartServer(ActionData action_data)
    {
        if (action_data && action_data.m_Target)
        {
            BuildingBase building = BuildingBase.Cast(action_data.m_Target.GetObject());
            if (building && building.DZPIsSupported())
            {
                int doorIndex = building.GetDoorIndex(action_data.m_Target.GetComponentIndex());
                if (doorIndex >= 0 && building.IsDoorLocked(doorIndex) && DZPPropertyControllerService.Get().CanOpenPropertyDoor(building, action_data.m_Player, doorIndex))
                {
                    DZPPropertyControllerService.Get().OpenInsideExit(building, action_data.m_Player, doorIndex);
                    return;
                }
            }
        }
        super.OnStartServer(action_data);
    }
}
