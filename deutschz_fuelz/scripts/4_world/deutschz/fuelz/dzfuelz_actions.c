class DZFuelZActionCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(0.5);
    }
};

class DZFuelZActionPayNote : ActionContinuousBase
{
    void DZFuelZActionPayNote()
    {
        m_CallbackClass = DZFuelZActionCB;
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
        m_FullBody = true;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
        m_Text = "Tankwart: gehaltenen 100-Euro-Schein verwenden";
    }

    override void CreateConditionComponents()
    {
        m_ConditionItem = new CCINonRuined;
        m_ConditionTarget = new CCTMan(3.0);
    }

    override typename GetInputType()
    {
        return ContinuousInteractActionInput;
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (!player || !item || !DZFuelZService.IsFuelNPC(target))
            return false;
        return DZFuelZSettingsService.Get().GetCurrencyValue(item.GetType()) > 0;
    }

    override bool UseMainItem()
    {
        return true;
    }

    override bool MainItemAlwaysInHands()
    {
        return true;
    }

    override void OnFinishProgressServer(ActionData action_data)
    {
        if (!action_data || !action_data.m_Player || !action_data.m_Target || !action_data.m_MainItem)
            return;

        int value = DZFuelZSettingsService.Get().GetCurrencyValue(action_data.m_MainItem.GetType());
        if (value > 0)
            DZFuelZService.RefuelForPayment(action_data.m_Player, DZFuelZService.ResolveFuelNPC(action_data.m_Target), value);
    }
};

class DZFuelZActionFullTank : ActionContinuousBase
{
    void DZFuelZActionFullTank()
    {
        m_CallbackClass = DZFuelZActionCB;
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
        m_FullBody = true;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
        m_Text = "Tankwart ansprechen / mit Bargeld aus Hand oder Inventar volltanken";
    }

    override void CreateConditionComponents()
    {
        m_ConditionItem = new CCINone;
        m_ConditionTarget = new CCTMan(3.0);
    }

    override typename GetInputType()
    {
        return ContinuousInteractActionInput;
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (!player || !DZFuelZService.IsFuelNPC(target))
            return false;
        return true;
    }

    override void OnFinishProgressServer(ActionData action_data)
    {
        if (action_data && action_data.m_Player && action_data.m_Target)
            DZFuelZService.RefuelFull(action_data.m_Player, DZFuelZService.ResolveFuelNPC(action_data.m_Target));
    }
};

class DZFuelZActionFillContainer : ActionContinuousBase
{
    void DZFuelZActionFillContainer()
    {
        m_CallbackClass = DZFuelZActionCB;
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_FILLBOTTLEWELL;
        m_FullBody = true;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
        m_Text = "Kraftstoffbehälter kostenpflichtig befüllen";
    }

    override void CreateConditionComponents()
    {
        m_ConditionItem = new CCINonRuined;
        m_ConditionTarget = new CCTCursor;
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (!player || !target || !item) return false;
        Object pump = target.GetObject();
        if (!DZFuelZService.IsManagedPump(pump)) return false;
        if (!Liquid.CanFillContainer(item, LIQUID_GASOLINE)) return false;
        return item.GetQuantity() < item.GetQuantityMax();
    }

    override void OnFinishProgressServer(ActionData action_data)
    {
        if (!action_data || !action_data.m_Player || !action_data.m_Target || !action_data.m_MainItem) return;
        DZFuelZService.FillContainerForPayment(action_data.m_Player, action_data.m_Target.GetObject(), action_data.m_MainItem);
    }
};

modded class ActionConstructor
{
    override void RegisterActions(TTypenameArray actions)
    {
        super.RegisterActions(actions);
        actions.Insert(DZFuelZActionPayNote);
        actions.Insert(DZFuelZActionFullTank);
        actions.Insert(DZFuelZActionFillContainer);
    }
};
