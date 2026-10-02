class DZ_FuelZ_NPC : PlayerBase
{
    override void EEInit()
    {
        super.EEInit();
        if (GetGame().IsServer())
            SetAllowDamage(false);
    }

    override bool CanBeTargetedByAI(EntityAI ai)
    {
        return false;
    }

    override void SetActionsRemoteTarget(out TInputActionMap InputActionMap)
    {
        super.SetActionsRemoteTarget(InputActionMap);
        AddAction(DZFuelZActionPayNote, InputActionMap);
        AddAction(DZFuelZActionFullTank, InputActionMap);
    }
};
