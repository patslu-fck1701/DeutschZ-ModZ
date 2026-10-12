// Real damage hooks; no player/world scans, one accepted hit per victim/second.
modded class PlayerBase
{
 protected int m_DZWM_LastHitMS = -1000;
 override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
 {
  super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
  if (!GetGame() || !GetGame().IsServer() || !source || source == this || !damageResult || damageResult.GetHighestDamage("Health") <= 0) return;
  int nowMS = GetGame().GetTime();
  if (nowMS - m_DZWM_LastHitMS < 1000) return;
  m_DZWM_LastHitMS = nowMS;
  DZWM_Service.Signal(GetPosition(), "combat");
 }
}
modded class ZombieBase
{
 protected int m_DZWM_LastHitMS = -1000;
 override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
 {
  super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
  if (!GetGame() || !GetGame().IsServer() || !source || !damageResult || damageResult.GetHighestDamage("Health") <= 0) return;
  int nowMS = GetGame().GetTime();
  if (nowMS - m_DZWM_LastHitMS < 1000) return;
  m_DZWM_LastHitMS = nowMS;
  DZWM_Service.Signal(GetPosition(), "combat");
 }
}
