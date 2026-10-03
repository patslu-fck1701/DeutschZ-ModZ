modded class Land_FuelStation_Feed
{
    override bool HasFuelToGive()
    {
        return false;
    }

    override int GetLiquidSourceType()
    {
        return LIQUID_NONE;
    }
};
