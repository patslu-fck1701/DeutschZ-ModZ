class DZWM_Rules
{
 static float Clamp(float value, float low, float high)
 {
  return Math.Min(high, Math.Max(low, value));
 }
 static int Axis(float coordinate, float origin, float size)
 {
  return Math.Floor((coordinate - origin) / size);
 }
 static float Decay(float value, float baseline, float elapsed, float halfLife)
 {
  return Clamp(baseline + (value - baseline) * Math.Pow(0.5, Math.Max(0, elapsed) / Math.Max(60, halfLife)), 0, 100);
 }
 static float Recovery(float noise, float violence, float pressure)
 {
  return Clamp(100 - Math.Max(noise, Math.Max(violence, pressure)), 0, 100);
 }
 static float Add(float value, float weight, float remaining)
 {
  return Clamp(value + Math.Min(Math.Max(0, weight), Math.Max(0, remaining)), 0, 100);
 }
 static float Severity(float noise, float violence, float pressure)
 {
  return Math.Max(noise, Math.Max(violence, pressure));
 }
}
