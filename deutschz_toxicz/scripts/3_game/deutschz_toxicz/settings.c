enum DZToxicZState { WAITING, START_SCENE, HOSPITAL_ONE, HOSPITAL_TWO, RIFFY_APPROACH, RIFFY_COMBAT, BLACKBOX, DECODER, T17, COMPLETE, COOLDOWN }

class DZToxicZLocation
{
    string Name;
    vector Position;
    bool Enabled;
    void DZToxicZLocation(string name = "", vector position = "0 0 0", bool enabled = true) { Name=name; Position=position; Enabled=enabled; }
}
class DZToxicZLoot { string ClassName; int Count; float Chance; }
class DZToxicZSettings
{
    int Version = 1;
    bool Enabled = true;
    string RequiredChainItem = "ToxicZ_Signal_Marker";
    vector RiffyPosition = "13725 0 14025";
    vector DecoderPosition = "12014 0 12617";
    float ObjectiveRadius = 18;
    float RiffyRevealDistance = 2500;
    float CombatActivationDistance = 450;
    int FirstSquadCount = 4;
    int SecondSquadCount = 6;
    int RiffySquadCount = 10;
    int CliffhangerDelaySeconds = 15;
    int CooldownMinutes = 180;
    string AIFaction = "Raiders";
    string AILoadout = "BanditLoadout";
    int RewardValueDMarkZ = 1000000;
    string RewardCurrencyClass = "ExpansionBanknoteEuro";
    int RewardCurrencyUnitValue = 100;
    ref array<ref DZToxicZLocation> Hospitals;
    ref array<ref DZToxicZLoot> RewardLoot;
    void DZToxicZSettings() { Hospitals=new array<ref DZToxicZLocation>; RewardLoot=new array<ref DZToxicZLoot>; }
    static DZToxicZSettings Defaults()
    {
        DZToxicZSettings s=new DZToxicZSettings;
        s.Hospitals.Insert(new DZToxicZLocation("Elektrozavodsk Hospital","10355 0 2265"));
        s.Hospitals.Insert(new DZToxicZLocation("Berezino Hospital","12010 0 9110"));
        s.Hospitals.Insert(new DZToxicZLocation("Krasnostav Medical","11220 0 12220"));
        s.Hospitals.Insert(new DZToxicZLocation("Svetlojarsk Medical","13800 0 13270"));
        DZToxicZLoot loot;
        loot=new DZToxicZLoot; loot.ClassName="TTC_GEVAR43"; loot.Count=1; loot.Chance=35; s.RewardLoot.Insert(loot);
        loot=new DZToxicZLoot; loot.ClassName="TTC_GEVAR43_Magazine_10rnd"; loot.Count=3; loot.Chance=80; s.RewardLoot.Insert(loot);
        loot=new DZToxicZLoot; loot.ClassName="TTC_Ammo_338"; loot.Count=2; loot.Chance=35; s.RewardLoot.Insert(loot);
        loot=new DZToxicZLoot; loot.ClassName="SNAFU_HNightforce"; loot.Count=1; loot.Chance=30; s.RewardLoot.Insert(loot);
        loot=new DZToxicZLoot; loot.ClassName="SNAFU_AtlasBipod"; loot.Count=1; loot.Chance=25; s.RewardLoot.Insert(loot);
        loot=new DZToxicZLoot; loot.ClassName="TTC_Universal_Suppressor_TAVOR"; loot.Count=1; loot.Chance=25; s.RewardLoot.Insert(loot);
        return s;
    }
}
class DZToxicZSave
{
    int Version = 1;
    int State = DZToxicZState.WAITING;
    string OwnerId;
    ref array<string> ParticipantIds;
    vector StartPosition;
    vector HospitalOne;
    vector HospitalTwo;
    bool FirstSquadDefeated;
    bool SecondSquadDefeated;
    bool RiffySquadDefeated;
    bool RewardGranted;
    int CliffhangerAt;
    int CooldownUntil;
    void DZToxicZSave() { ParticipantIds=new array<string>; }
}
