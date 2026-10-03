class DZADZ_KothLootEntry
{
 string ClassName;
 float Chance;
 int Min;
 int Max;
 float MinQuantity;
 float MaxQuantity;
 bool Enabled;
 float MinHealthPercent;
 float MaxHealthPercent;
 ref array<string> Attachments;
 ref array<string> ExtraItems;
 ref array<string> Alternatives;
}

class DZADZ_KothLootConfig
{
 ref array<ref DZADZ_KothLootEntry> RewardCrateLoot;
 ref array<ref DZADZ_KothLootEntry> BossCorpseLoot;

 static DZADZ_KothLootConfig Load()
 {
  string path="$profile:DeutschZ-System/DeutschZ_KotHZ/Config/KotHZLoot.json";
  if (!FileExist(path)) return null;
  ref DZADZ_KothLootConfig config=new DZADZ_KothLootConfig;
  string error;
  if (!JsonFileLoader<ref DZADZ_KothLootConfig>.LoadFile(path,config,error))
  {
   Print("[AirdropZ] KOTH loot load failed: "+error);
   return null;
  }
  return config;
 }
}
