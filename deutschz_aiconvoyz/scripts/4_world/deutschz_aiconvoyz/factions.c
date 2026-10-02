[eAIRegisterFaction(eAIFactionAmericanZ)]
class eAIFactionAmericanZ: eAIFactionWest
{
    void eAIFactionAmericanZ() { m_Loadout = "WestLoadout"; }

    override bool IsFriendly(notnull eAIFaction other)
    {
        return other.IsInherited(eAIFactionAmericanZ);
    }

    override bool IsFriendlyEntity(EntityAI other, DayZPlayer factionMember = null)
    {
        PlayerBase player = PlayerBase.Cast(other);
        if (player && !player.IsAI())
            return false;

        return super.IsFriendlyEntity(other, factionMember);
    }
}

[eAIRegisterFaction(eAIFactionRussianZ)]
class eAIFactionRussianZ: eAIFactionEast
{
    void eAIFactionRussianZ() { m_Loadout = "EastLoadout"; }

    override bool IsFriendly(notnull eAIFaction other)
    {
        return other.IsInherited(eAIFactionRussianZ);
    }

    override bool IsFriendlyEntity(EntityAI other, DayZPlayer factionMember = null)
    {
        PlayerBase player = PlayerBase.Cast(other);
        if (player && !player.IsAI())
            return false;

        return super.IsFriendlyEntity(other, factionMember);
    }
}
