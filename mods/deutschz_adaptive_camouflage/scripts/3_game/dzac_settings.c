class DZAC_Settings
{
 int schemaVersion = 1;
 bool enabled = true;
 float minimumFactor = 0.65;
 float maximumFactor = 1.2;
 float woodlandClothingBonus = 0.12;
 float wrongEnvironmentPenalty = 0.08;
 float proneBonus = 0.10;
 float crouchBonus = 0.05;
 float movementPenalty = 0.10;
 float darknessBonus = 0.08;
 int cacheMilliseconds = 2000;
 int attackBypassSeconds = 20;
 ref array<string> woodlandClothes = {"GhillieSuit_Woodland", "GhillieTop_Woodland", "TTsKOJacket_Camo", "TTSKOPants"};
 ref array<string> woodlandSurfaces = {"grass", "forest", "wood"};

 void Validate()
 {
  minimumFactor = Math.Clamp(minimumFactor, 0.5, 1.0);
  maximumFactor = Math.Clamp(maximumFactor, 1.0, 1.3);
  woodlandClothingBonus = Math.Clamp(woodlandClothingBonus, 0, 0.2);
  wrongEnvironmentPenalty = Math.Clamp(wrongEnvironmentPenalty, 0, 0.15);
  proneBonus = Math.Clamp(proneBonus, 0, 0.15);
  crouchBonus = Math.Clamp(crouchBonus, 0, 0.1);
  movementPenalty = Math.Clamp(movementPenalty, 0, 0.2);
  darknessBonus = Math.Clamp(darknessBonus, 0, 0.1);
  cacheMilliseconds = Math.Clamp(cacheMilliseconds, 1000, 10000);
  attackBypassSeconds = Math.Clamp(attackBypassSeconds, 15, 60);
  if (!woodlandClothes) woodlandClothes = new array<string>;
  if (!woodlandSurfaces) woodlandSurfaces = new array<string>;
  while (woodlandClothes.Count() > 32) woodlandClothes.Remove(woodlandClothes.Count() - 1);
  while (woodlandSurfaces.Count() > 16) woodlandSurfaces.Remove(woodlandSurfaces.Count() - 1);
 }
}
