class DZATM_ATMTargetResolver
{
    static ExpansionATMBase Resolve(ActionTarget target)
    {
        if (!target) return null;

        ExpansionATMBase atm = ExpansionATMBase.Cast(target.GetObject());
        if (atm) return atm;

        return ExpansionATMBase.Cast(target.GetParentOrObject());
    }

    static bool IsRaidATM(ExpansionATMBase atm)
    {
        return atm && atm.GetType() == DZATM_Const.ATM_CLASS;
    }
}

modded class ExpansionATMBase
{
    protected string m_DZATM_ATMId;
    protected bool m_DZATM_RaidLocked;
    protected bool m_DZATM_RaidActionBlocked;
    protected float m_DZATM_HackDuration = 180.0;

    void ExpansionATMBase()
    {
        RegisterNetSyncVariableBool("m_DZATM_RaidLocked");
        RegisterNetSyncVariableBool("m_DZATM_RaidActionBlocked");
        RegisterNetSyncVariableFloat("m_DZATM_HackDuration", 10.0, 3600.0);
    }

    override void EEInit()
    {
        super.EEInit();
        if (GetType() == DZATM_Const.ATM_CLASS)
        {
            SetObjectTexture(0, DZATM_Const.ATM_TEXTURE);
            SetObjectTexture(1, DZATM_Const.ATM_TEXTURE);
        }
        if (GetGame() && GetGame().IsServer() && GetType() == DZATM_Const.ATM_CLASS)
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DZATM_RegisterSelf, 1000, false);
    }

    override void EEDelete(EntityAI parent)
    {
        if (GetGame() && GetGame().IsServer() && GetType() == DZATM_Const.ATM_CLASS)
            DZATM_Manager.GetInstance().UnregisterATM(this);
        super.EEDelete(parent);
    }

    protected void DZATM_RegisterSelf()
    {
        if (GetGame() && GetGame().IsServer() && GetType() == DZATM_Const.ATM_CLASS)
            DZATM_Manager.GetInstance().RegisterATM(this);
    }

    void DZATM_SetATMId(string id) { m_DZATM_ATMId = id; }
    string DZATM_GetATMId() { return m_DZATM_ATMId; }

    void DZATM_SetHackDuration(float seconds)
    {
        m_DZATM_HackDuration = Math.Clamp(seconds, 10.0, 3600.0);
        SetSynchDirty();
    }

    float DZATM_GetHackDuration()
    {
        return m_DZATM_HackDuration;
    }

    void DZATM_SetRaidLocked(bool locked)
    {
        if (m_DZATM_RaidLocked == locked) return;
        m_DZATM_RaidLocked = locked;
        SetSynchDirty();
    }

    bool DZATM_IsRaidLocked() { return m_DZATM_RaidLocked; }

    void DZATM_SetRaidActionBlocked(bool blocked)
    {
        if (m_DZATM_RaidActionBlocked == blocked) return;
        m_DZATM_RaidActionBlocked = blocked;
        SetSynchDirty();
    }

    bool DZATM_IsRaidActionBlocked() { return m_DZATM_RaidActionBlocked; }

    override void SetActions()
    {
        super.SetActions();
        if (GetType() == DZATM_Const.ATM_CLASS)
            AddAction(ActionDZATM_RobATM);
    }
}

class ActionDZATM_RobATMCB: ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        float duration = 180.0;
        if (m_ActionData && m_ActionData.m_Target)
        {
            ExpansionATMBase atm = DZATM_ATMTargetResolver.Resolve(m_ActionData.m_Target);
            if (DZATM_ATMTargetResolver.IsRaidATM(atm))
                duration = atm.DZATM_GetHackDuration();
        }
        m_ActionData.m_ActionComponent = new CAContinuousTime(Math.Max(duration, 1.0));
    }
}

class ActionDZATM_RobATM: ActionContinuousBase
{
    void ActionDZATM_RobATM()
    {
        m_CallbackClass = ActionDZATM_RobATMCB;
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
        m_FullBody = true;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
        m_Text = "ATM ausrauben";
    }

    override typename GetInputType() { return ContinuousDefaultActionInput; }

    override void CreateConditionComponents()
    {
        m_ConditionTarget = new CCTCursor(UAMaxDistances.DEFAULT);
        m_ConditionItem = new CCINonRuined;
    }

    override bool HasTarget() { return true; }
    override bool HasProgress() { return true; }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (!player || !target || !item || item.IsRuined()) return false;
        if (!item.IsKindOf("Crowbar")) return false;

        ExpansionATMBase atm = DZATM_ATMTargetResolver.Resolve(target);
        return DZATM_ATMTargetResolver.IsRaidATM(atm) && !atm.DZATM_IsRaidActionBlocked();
    }

    override void OnStartAnimationLoopServer(ActionData action_data)
    {
        super.OnStartAnimationLoopServer(action_data);
        if (!action_data || !action_data.m_Player || !action_data.m_Target) return;

        ExpansionATMBase atm = DZATM_ATMTargetResolver.Resolve(action_data.m_Target);
        if (DZATM_ATMTargetResolver.IsRaidATM(atm))
            DZATM_Manager.GetInstance().StartRaid(action_data.m_Player, atm);
    }

    override void OnEndServer(ActionData action_data)
    {
        super.OnEndServer(action_data);
        if (!action_data || !action_data.m_Player || !action_data.m_Target) return;

        ExpansionATMBase atm = DZATM_ATMTargetResolver.Resolve(action_data.m_Target);
        if (!DZATM_ATMTargetResolver.IsRaidATM(atm)) return;

        if (action_data.m_State == UA_FINISHED)
        {
            DZATM_Manager.GetInstance().CompleteHackPhase(action_data.m_Player, atm);
            return;
        }
        DZATM_Manager.GetInstance().CancelRaid(action_data.m_Player, atm, DZATM_Const.CANCEL_ACTION_ENDED);
    }
}

modded class ExpansionActionOpenATMMenu
{
    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (!super.ActionCondition(player, target, item)) return false;
        ExpansionATMBase atm = DZATM_ATMTargetResolver.Resolve(target);
        return !DZATM_ATMTargetResolver.IsRaidATM(atm) || !atm.DZATM_IsRaidLocked();
    }

    override void OnStartServer(ActionData action_data)
    {
        if (action_data && action_data.m_Target)
        {
            ExpansionATMBase atm = DZATM_ATMTargetResolver.Resolve(action_data.m_Target);
            if (DZATM_ATMTargetResolver.IsRaidATM(atm) && atm.DZATM_IsRaidLocked())
                return;
        }
        super.OnStartServer(action_data);
    }
}
