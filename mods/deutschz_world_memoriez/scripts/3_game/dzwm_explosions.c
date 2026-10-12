modded class DayZGame
{
 override void ExplosionEffectsEx(Object source, Object directHit, int componentIndex, float energyFactor, float explosionFactor, HitInfo hitInfo)
 {
  super.ExplosionEffectsEx(source, directHit, componentIndex, energyFactor, explosionFactor, hitInfo);
  if (IsServer() && hitInfo && DZWM_ExplosionBus.Events) DZWM_ExplosionBus.Events.Invoke(hitInfo.GetPosition());
 }
}
class DZWM_ExplosionBus
{
 static ref ScriptInvoker Events = new ScriptInvoker;
}
