modded class PlayerBase
{
 protected int m_DZAC_CacheUntil;
 protected float m_DZAC_Clothing;
 protected float m_DZAC_Posture;
 protected float m_DZAC_Speed;

 float DZAC_Factor(float daylight)
 {
  DZAC_Settings options = DZAC_Runtime.s_Settings;
  if (!options || !options.enabled) return 1.0;
  int nowMilliseconds = GetGame().GetTime();
  if (nowMilliseconds >= m_DZAC_CacheUntil || m_DZAC_CacheUntil - nowMilliseconds > options.cacheMilliseconds)
  {
   m_DZAC_CacheUntil = nowMilliseconds + options.cacheMilliseconds;
   vector position = GetPosition();
   string surface;
   GetGame().SurfaceGetType(position[0], position[2], surface);
   surface.ToLower();
   bool woodland;
   foreach (string surfaceWord: options.woodlandSurfaces)
   {
    if (surfaceWord != "" && surface.Contains(surfaceWord)) woodland = true;
   }
   ref array<string> slots = {"Body", "Legs", "Headgear", "Back"};
   int matchedSlots;
   foreach (string slotName: slots)
   {
    EntityAI clothing = FindAttachmentBySlotName(slotName);
    if (!clothing) continue;
    foreach (string clothingClass: options.woodlandClothes)
    {
     if (clothingClass != "" && clothing.IsKindOf(clothingClass))
     {
      matchedSlots++;
      break;
     }
    }
   }
   m_DZAC_Clothing = 0;
   if (matchedSlots > 0)
   {
    if (woodland) m_DZAC_Clothing = -options.woodlandClothingBonus;
    else m_DZAC_Clothing = options.wrongEnvironmentPenalty;
   }
   HumanMovementState movement = new HumanMovementState;
   GetMovementState(movement);
   m_DZAC_Posture = 0;
   if (movement.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_PRONE) m_DZAC_Posture = -options.proneBonus;
   else if (movement.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_CROUCH) m_DZAC_Posture = -options.crouchBonus;
   m_DZAC_Speed = Math.Clamp(Expansion_GetMovementSpeed() / 3.0, 0, 1);
  }
  return DZAC_Runtime.Combine(m_DZAC_Clothing, m_DZAC_Posture, m_DZAC_Speed, daylight, options);
 }
}
