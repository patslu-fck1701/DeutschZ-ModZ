class DZADZ_PublishedGlobal
{
 bool Enabled = true;
 int PreWarningSeconds = 420;
 int SealBreakSeconds = 45;
 int AbandonedCleanupSeconds = 1800;
 int UnopenedRecoveryDelaySeconds = 600;
 int HardlineReputationOnSecuredCompletion = 100;
 bool ShowExact3DMarkerAfterTransponder;
}
class DZADZ_PublishedFree { ref DZADZ_PublishedGlobal Global; }
class DZADZ_Settings
{
 int Version = 1;
 bool Enabled = true;
 int InitialDelaySeconds = 1800;
 int CooldownSeconds = 3600;
 int AnnouncementSeconds = 420;
 int LifetimeSeconds = 1800;
 int OpenedLifetimeSeconds = 900;
 int MinimumPlayers = 1;
 int Reputation = 100;
 int SealBreakSeconds = 45;
 int UnsealFailureChancePercent = 25;
 int RecoveryDelaySeconds = 600;
 bool Exact3DMarker;
 ref DZADZ_PublishedFree Free;
 ref array<vector> Locations = {
  "5419.168 333.422 9886.764", "4169.207 338.415 10990.305",
  "1689.941 451.739 14343.285", "8007.444 342.699 14627.948",
  "4544.529 319.462 8333.652", "2548.715 191.961 5198.598",
  "2209.268 93.762 3369.180", "4847.206 10.587 2465.876",
  "10869.070 234.548 12285.822", "2021.770 241.837 7054.070",
  "504.444 424.995 11237.219", "4832.060 475.250 15187.400"
 };
 void RestoreDefaultLocations()
 {
  Locations = {"5419.168 333.422 9886.764","4169.207 338.415 10990.305","1689.941 451.739 14343.285","8007.444 342.699 14627.948","4544.529 319.462 8333.652","2548.715 191.961 5198.598","2209.268 93.762 3369.180","4847.206 10.587 2465.876","10869.070 234.548 12285.822","2021.770 241.837 7054.070","504.444 424.995 11237.219","4832.060 475.250 15187.400"};
 }
 void ApplyPublished()
 {
  if (!Free || !Free.Global) return;
  Enabled=Free.Global.Enabled; AnnouncementSeconds=Free.Global.PreWarningSeconds;
  SealBreakSeconds=Free.Global.SealBreakSeconds; LifetimeSeconds=Free.Global.AbandonedCleanupSeconds;
  RecoveryDelaySeconds=Free.Global.UnopenedRecoveryDelaySeconds;
  Reputation=Free.Global.HardlineReputationOnSecuredCompletion; Exact3DMarker=Free.Global.ShowExact3DMarkerAfterTransponder;
 }
}
