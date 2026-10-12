modded class PlayerBase
{
 protected int m_DZWZ_LastObservationMS;
 override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
 {
  super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
  if (!GetGame().IsServer() || !DZWZ_Service.Instance || !source || !damageResult || damageResult.GetHighestDamage("Health") <= 0) return;
  PlayerBase attacker = PlayerBase.Cast(source.GetHierarchyRootPlayer());
  if (!attacker) attacker = PlayerBase.Cast(source);
  if (!attacker || attacker.IsAI() || !attacker.GetIdentity()) return;
  int nowMS = GetGame().GetTime();
  if (attacker.m_DZWZ_LastObservationMS && nowMS - attacker.m_DZWZ_LastObservationMS < 1000) return;
  attacker.m_DZWZ_LastObservationMS = nowMS;
  DZWZ_Service.Instance.Observe(attacker, this);
 }
}
modded class eAIPlayerTargetInformation
{
 override float CalculateThreat(eAIBase ai = null, eAITargetInformationState state = null)
 {
  float nativeThreat = super.CalculateThreat(ai, state);
  if (!GetGame().IsServer() || !DZWZ_Service.Instance || !ai || !state || nativeThreat <= 0) return nativeThreat;
  PlayerBase subject = PlayerBase.Cast(GetEntity());
  // No friendly-to-hostile conversion; no through-wall detection or location broadcast.
  if (!subject || subject.IsAI() || !DZWZ_Service.Instance.Remembers(ai, subject)) return nativeThreat;
  return Math.Min(1.0, nativeThreat * DZWZ_Service.Instance.m_Settings.threatMultiplier);
 }
}
