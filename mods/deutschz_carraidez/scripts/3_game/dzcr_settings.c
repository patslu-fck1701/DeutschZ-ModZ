class DZCR_Settings
{
 int schemaVersion = 1;
 bool enabled = true;
 string runID = "operator-test-001";
 vector position = "7480 0 7480";
 string vehicleClass = "OffroadHatchback";
 string guardClass = "eAI_SurvivorM_Boris";
 int guardCount = 2;
 int captureSeconds = 15;
 int timeoutSeconds = 600;
 int cleanupSeconds = 120;
 float interactionRadius = 3;
 bool yieldToMajorEvents = true;
 ref array<string> rewards = {"BandageDressing", "AmmoBox_762x39_20Rnd"};
 void Validate()
 {
  guardCount = Math.Clamp(guardCount, 1, 4);
  captureSeconds = Math.Clamp(captureSeconds, 10, 120);
  timeoutSeconds = Math.Clamp(timeoutSeconds, 120, 1800);
  cleanupSeconds = Math.Clamp(cleanupSeconds, 60, 600);
  interactionRadius = Math.Clamp(interactionRadius, 1.5, 4);
  if (!rewards) rewards = new array<string>;
  while (rewards.Count() > 8) rewards.Remove(rewards.Count() - 1);
 }
}
