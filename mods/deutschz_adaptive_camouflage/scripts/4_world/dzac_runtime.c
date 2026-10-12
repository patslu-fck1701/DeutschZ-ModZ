class DZAC_Runtime
{
 static ref DZAC_Settings s_Settings;

 static void Start()
 {
  if (!GetGame().IsServer() || s_Settings) return;
  string root = "$profile:DeutschZ-System/deutschz_adaptive_camouflage";
  string loadError;
  DZAC_Settings loaded;
  if (FileExist(root + "/config.json"))
  {
   bool loadedConfig = JsonFileLoader<DZAC_Settings>.LoadFile(root + "/config.json", loaded, loadError);
   if (!loadedConfig || !loaded || loaded.schemaVersion != 1)
   {
    Print("[AdaptiveCamouflage] Disabled: invalid configuration preserved.");
    return;
   }
  }
  else
  {
   loaded = new DZAC_Settings;
   MakeDirectory("$profile:DeutschZ-System");
   MakeDirectory(root);
   bool savedConfig = JsonFileLoader<DZAC_Settings>.SaveFile(root + "/config.json", loaded, loadError);
   if (!savedConfig) return;
  }
  loaded.Validate();
  s_Settings = loaded;
  Print("[AdaptiveCamouflage] READY bounded detection modifier; attacks and noise unchanged.");
 }

 static float Combine(float clothing, float posture, float speed, float light, DZAC_Settings options)
 {
  return Math.Clamp(1.0 + clothing + posture + speed * options.movementPenalty - (1.0 - Math.Clamp(light, 0, 1)) * options.darknessBonus, options.minimumFactor, options.maximumFactor);
 }
}
