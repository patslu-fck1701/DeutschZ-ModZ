class DZADZ_Radio
{
 static const int RPC_AUDIO=89517031;
 static bool HasRadio(PlayerBase player)
 {
  if(!player) return false;
  array<EntityAI> inventory=new array<EntityAI>;
  player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER,inventory);
  foreach(EntityAI item:inventory)
  {
   TransmitterBase radio;
   if(!Class.CastTo(radio,item)) continue;
   if(radio.IsRuined()) continue;
   ComponentEnergyManager energy=radio.GetCompEM();
   if(!energy) continue;
   if(!energy.IsWorking()) continue;
   float tunedFrequency=radio.GetTunedFrequency();
   if(tunedFrequency > 89)
   {
    if(tunedFrequency < 90) return true;
   }
  }
  return false;
 }
 static void Broadcast(string title,string text,string sound="")
 {
  if(!GetGame() || !GetGame().IsServer()) return;
  array<Man> players=new array<Man>; GetGame().GetPlayers(players);
  foreach(Man man:players)
  {
   PlayerBase player=PlayerBase.Cast(man); if(!player || !player.GetIdentity() || !player.IsAlive()) continue;
   NotificationSystem.SendNotificationToPlayerExtended(player,15,title,text,"set:dayz_gui image:icon_info");
   if(sound!="" && HasRadio(player)) player.RPCSingleParam(RPC_AUDIO,new Param1<string>(sound),true,player.GetIdentity());
  }
 }
 static void BroadcastPhase(int phase,int variant)
 {
  if(phase==DZADZ_RavenPhase.DZADZ_INTERCEPT)
   Broadcast("UNBEKANNTE LUFTFRACHT ERFASST","89,5 MHz: Raven Two-One an Leitstelle... Triebwerk haelt. Unbekannte Kraefte folgen uns. Fracht bleibt bis zur Abwurfzone an Bord. Ein altes Logistiknetz sendet wieder. Schalte deinen Funk auf 89,5 MHz.","DZADZ_RavenApproach_SoundSet");
  else if(phase==DZADZ_RavenPhase.DZADZ_TARGET_ZONE)
   Broadcast("RAVEN: SUCHGEBIET","Ein moeglicher Abwurfbereich ist auf der Karte eingegrenzt. Die Kiste liegt nicht zwingend im Zentrum. Beobachte Flugzeug, Fallschirm und Rauch.");
  else if(phase==DZADZ_RavenPhase.DZADZ_DROP_RELEASED)
   Broadcast("RAVEN TWO-ONE: FRACHT RAUS","Zielgebiet erreicht... Fracht raus! Scheisse, Bewegung am Boden! Suche den Fallschirm und den Rauch. Die Transportversiegelung laesst sich erst nach der Landung oeffnen.","DZADZ_RavenTarget_SoundSet");
  else if(phase==DZADZ_RavenPhase.DZADZ_TRANSPONDER)
   Broadcast("RAVEN-TRANSPONDER AKTIV","Unbefugter Zugriff erkannt. Die genaue Position ist jetzt fuer alle auf der Karte sichtbar. Halte beim Aufbrechen Abstand zu Schuessen: Treffer oder Bewegung unterbrechen die Arbeit. Ein Bergungsteam kann bereits unterwegs sein.");
  else if(phase==DZADZ_RavenPhase.DZADZ_OPENED)
  {
   string clue="Auf dem Frachtpapier stehen ein alter Freigabestempel und eine neue Transportnummer.";
   if(variant==2) clue="Zwischen Versorgungsguetern liegt ein beschaedigter Durchschlag: dieselbe Kennung, aber ein deutlich aelterer Sendezeitpunkt. Jemand hat diese Nachricht vorbereitet.";
   if(variant==3) clue="Das Manifest wurde geloescht. Unter einer frischen Versiegelung steckt eine alte Projektkennung. Die Fracht existiert offiziell nicht.";
   if(variant==4) clue="Q-17. Quarantaenefracht. Eine Notiz nennt Riffy; die Freigabe traegt Morozovs Kennung. Mehrere Zeitstempel wurden nachtraeglich geaendert.";
   Broadcast("RAVEN: SIEGEL GEBROCHEN",clue+" Beseitige die Bedrohung und sichere anschliessend 30 Sekunden lang den Bereich. Nur tatsaechlich anwesende Teilnehmer erhalten den Abschluss.");
  }
 }
}
modded class PlayerBase
{
 protected EffectSound m_DZADZ_RadioSound;
 override void OnRPC(PlayerIdentity sender,int rpc_type,ParamsReadContext ctx)
 {
  if (rpc_type != DZADZ_Radio.RPC_AUDIO)
  {
   super.OnRPC(sender, rpc_type, ctx);
   return;
  }
  if(!GetGame() || !GetGame().IsClient() || GetGame().GetPlayer()!=this) return;
  Param1<string> rpc_payload;
  if (!ctx.Read(rpc_payload)) return;
  if (rpc_payload.param1 != "DZADZ_RavenApproach_SoundSet" && rpc_payload.param1 != "DZADZ_RavenTarget_SoundSet") return;
  if(!DZADZ_Radio.HasRadio(this)) return;
  if(m_DZADZ_RadioSound) m_DZADZ_RadioSound.SoundStop();
  m_DZADZ_RadioSound = SEffectManager.PlaySound(rpc_payload.param1, GetPosition());
  if(m_DZADZ_RadioSound) m_DZADZ_RadioSound.SetSoundAutodestroy(true);
 }
}
