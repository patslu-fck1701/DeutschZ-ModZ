class deutschz_aiconvoyz_decoder extends GPSReceiver {}
class Toxicz_Doc_Decoder extends GPSReceiver {}
class ToxicZ_Signal_Marker extends GPSReceiver {}
class ToxicZ_Secret_Document extends Paper {}
class deutschz_aiconvoyz_cardreader extends deutschz_aiconvoyz_decoder {}
class toxicz_zone_marker extends deutschz_aiconvoyz_decoder {}

class deutschz_aiconvoyz_blackbox_target
{
    static deutschz_aiconvoyz_blackbox Resolve(ActionTarget target)
    {
        if (!target)
            return null;

        deutschz_aiconvoyz_blackbox box = deutschz_aiconvoyz_blackbox.Cast(target.GetObject());
        if (box)
            return box;

        return deutschz_aiconvoyz_blackbox.Cast(target.GetParentOrObject());
    }
}

class deutschz_aiconvoyz_blackbox extends SeaChest
{
    protected bool m_deutschz_hacked;
    protected int m_deutschz_next_code_attempt;

    void deutschz_aiconvoyz_blackbox()
    {
        RegisterNetSyncVariableBool("m_deutschz_hacked");
    }

    override void EEInit()
    {
        super.EEInit();
        GameInventory inventory = GetInventory();
        if (inventory)
            inventory.LockInventory(HIDE_INV_FROM_SCRIPT);
    }
    bool IsHacked() { return m_deutschz_hacked; }

    bool CompleteHack(PlayerBase player)
    {
        if (!GetGame().IsServer() || m_deutschz_hacked || !player || !player.IsAlive()) return false;
        if (vector.Distance(player.GetPosition(), GetPosition()) > 3) return false;
        if (!g_deutschz_aiconvoyz_controller || !g_deutschz_aiconvoyz_controller.IsActiveBlackbox(this)) return false;
        GetInventory().UnlockInventory(HIDE_INV_FROM_SCRIPT);
        if (!g_deutschz_aiconvoyz_controller.FillBlackboxLoot(this, player))
        {
            GetInventory().LockInventory(HIDE_INV_FROM_SCRIPT);
            return false;
        }
        m_deutschz_hacked = true; SetSynchDirty();
        if (g_deutschz_aiconvoyz_controller) g_deutschz_aiconvoyz_controller.OnBlackboxHacked(this, player);
        return true;
    }

    override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
    {
        super.OnRPC(sender, rpc_type, ctx);
    }

    override void OnVariablesSynchronized()
    {
        super.OnVariablesSynchronized();
        GameInventory inventory = GetInventory();
        if (!inventory) return;
        if (m_deutschz_hacked) inventory.UnlockInventory(HIDE_INV_FROM_SCRIPT);
        else inventory.LockInventory(HIDE_INV_FROM_SCRIPT);
    }

    override void SetActions()
    {
        super.SetActions();
        AddAction(deutschz_action_hack_blackbox);
    }
}

modded class ActionConstructor
{
    override void RegisterActions(TTypenameArray actions)
    {
        super.RegisterActions(actions);
        actions.Insert(deutschz_action_hack_blackbox);
    }
}

class deutschz_action_hack_blackbox_cb: ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        float duration = 90;
        if (g_deutschz_aiconvoyz_controller) duration = g_deutschz_aiconvoyz_controller.GetHackSeconds();
        m_ActionData.m_ActionComponent = new CAContinuousTime(duration);
    }
}

class deutschz_action_hack_blackbox: ActionContinuousBase
{
    void deutschz_action_hack_blackbox()
    {
        m_CallbackClass = deutschz_action_hack_blackbox_cb;
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
        m_FullBody = true;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
        m_Text = "Hack BlackBox";
    }

    override void CreateConditionComponents()
    {
        m_ConditionItem = new CCINone();
        m_ConditionTarget = new CCTCursor;
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        deutschz_aiconvoyz_blackbox box = deutschz_aiconvoyz_blackbox_target.Resolve(target);
        return player && player.IsAlive() && box && !box.IsHacked();
    }

    override void OnFinishProgressServer(ActionData action_data)
    {
        if (!action_data)
            return;

        deutschz_aiconvoyz_blackbox box = deutschz_aiconvoyz_blackbox_target.Resolve(action_data.m_Target);
        if (box)
            box.CompleteHack(action_data.m_Player);
    }
}
