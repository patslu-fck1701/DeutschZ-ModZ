modded class eAIPlayerTargetInformation
{
 override float CalculateThreat(eAIBase ai = null, eAITargetInformationState state = null)
 {
  float nativeThreat = super.CalculateThreat(ai, state);
  DZAC_Settings options = DZAC_Runtime.s_Settings;
  if (!GetGame().IsServer() || !options || !options.enabled || !ai || !state || nativeThreat <= 0 || nativeThreat >= 0.4) return nativeThreat;
  PlayerBase player = PlayerBase.Cast(GetEntity());
  if (!player || player.IsAI() || !player.IsAlive() || player.IsRaised() || player.IsFighting()) return nativeThreat;
  // Previously identified targets keep native combat/search behaviour.
  if (state.m_SearchOnLOSLost || player.m_eAI_AttackCooldown > 0) return nativeThreat;
  int nowUTC = ExpansionStatic.GetTimestamp(true);
  if (player.m_eAI_LastAggressionTimeout > nowUTC) return nativeThreat;
  if (player.m_eAI_LastAggressionTime > 0 && nowUTC - player.m_eAI_LastAggressionTime < options.attackBypassSeconds) return nativeThreat;
  return nativeThreat * player.DZAC_Factor(ai.m_Expansion_DaylightVisibility);
 }
}
