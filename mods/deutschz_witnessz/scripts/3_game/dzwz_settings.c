class DZWZ_Settings
{
 int schemaVersion = 1;
 bool enabled = true;
 float identificationRange = 35;
 float minimumFacingDot = 0.3;
 int reportDelaySeconds = 20;
 int memorySeconds = 1800;
 int maxMemories = 512;
 int maxPending = 128;
 int maxChecksPerSecond = 64;
 float threatMultiplier = 1.15;
 void Validate()
 {
  identificationRange = Math.Clamp(identificationRange, 5, 60);
  minimumFacingDot = Math.Clamp(minimumFacingDot, 0, 0.95);
  reportDelaySeconds = Math.Clamp(reportDelaySeconds, 10, 120);
  memorySeconds = Math.Clamp(memorySeconds, 60, 7200);
  maxMemories = Math.Clamp(maxMemories, 16, 1024);
  maxPending = Math.Clamp(maxPending, 8, 256);
  maxChecksPerSecond = Math.Clamp(maxChecksPerSecond, 8, 128);
  threatMultiplier = Math.Clamp(threatMultiplier, 1, 1.3);
 }
}
