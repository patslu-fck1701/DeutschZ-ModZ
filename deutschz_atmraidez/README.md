# DeutschZ ATM RaideZ

Source-Handoff fuer ein eigenstaendiges ATM-Raubsystem.

## Scope

Diese Mod ist absichtlich nur fuer:
- vorhandene DayZ Expansion ATMs (`ExpansionATMBase`)
- ATM-Raub
- zweiphasigen Ablauf
- Alarm / Smoke / Blinklicht
- Expansion Marker
- ATM- und Spieler-Cooldowns
- serverautorisierte Auszahlung
- Adminmenue

Nicht enthalten:
- Fahrzeughacks
- Garagenhacks
- Gruppen-/Clanbank
- eigenes Banking

## Expansion

Die Raid-Funktion ist absichtlich auf `ExpansionATM_2` festgelegt. Bereits vorhandene/extern platzierte
`ExpansionATM_2` werden ueber `ExpansionATMBase.SetActions()` raidbar gemacht. Zusaetzlich kann die Mod
konfigurierte `ExpansionATM_2`-Positionen selbst erzeugen und verwendet einen vorhandenen ATM im
konfigurierten Wiederverwendungsradius statt einen Doppelspawn anzulegen.

## Admin

`/atmadmin` oeffnet das vorbereitete Adminmenue.
Jede Mutation wird serverseitig anhand `AdminUIDs` autorisiert.

## Reward

`DZATM_RewardProvider` ist die einzige Stelle fuer Auszahlungen.
Standard: `PhysicalCurrency`.

Standardwaehrung ist `ExpansionBanknoteEuro` mit einem konfigurierten Einheitswert von 100.
Alternativ kann `ExpansionBanknoteUSD` verwendet werden. Die Auszahlungshoehe und die konkrete
Waehrung werden ausschliesslich serverseitig bestimmt.

Eine spaetere BankingZ-Integration soll als eigener Adapter erfolgen. ATM RaideZ darf nicht Banking-Code
quer ueber Manager/UI verteilen.

## Status

Source vorbereitet. Kein echter DayZ-Compile-/Runtime-Test in dieser Umgebung.
Codex muss Compile, Expansion-Version, Action-Dauer-Synchronisierung und Admin-UI pruefen.

## 0.1.7 DeutschZ-ATM-Textur

- Vorhandene und konfigurierte `ExpansionATM_2` erhalten die DeutschZ-Textur auf `zbytek` und `screen`.
- Klasse, Positionen, Expansion-Banking und Raidlogik bleiben unveraendert.

## 0.1.8 ATM-Textur aktualisiert

- Die bestehende ATM-PAA wurde durch die vom Owner am 05.09.2026 bereitgestellte Fassung ersetzt.
- Texturpfad, ATM-Klasse, Positionen, Banking- und Raidlogik bleiben unveraendert.

## 0.1.6 Phase-2-Rehack-Sperre

- Phase 1 bleibt als Continuous Action gueltig.
- Ab Phase 2 und waehrend ATM-Cooldown wird die Raid-Action per separatem NetSync-Zustand gesperrt.

## 0.1.5 Continuous-Action-Fix

- Die Raid-Action bricht nicht mehr durch ihre eigene NetSync-ATM-Sperre direkt nach dem Start ab.
- ATM-Cooldown, Parallelraid-Schutz und Werkzeugpruefung bleiben serverautoritativ.

## 0.1.4 Rob-Action-/Textur-Fix

- `DZATM_RobTool.SetActions()` registriert `ActionDZATM_RobATM` wie im nachweislich funktionierenden CriminalZ-Werkzeug.
- Die PAA stammt aus dem tatsaechlich verwendeten CriminalZ-Source und passt zur geerbten Crowbar-UV.

## 0.1.3 ATM-/RobTool-Fix

- 16 gewuenschte `ExpansionATM_2`-Positionen sind als robuste Defaults im Code hinterlegt.
- Fehlende/alte Profil-JSON ohne ATM-Liste wird automatisch mit diesen Positionen ergaenzt.
- ATM-Spawn nutzt `ECE_SETUP | ECE_CREATEPHYSICS | ECE_UPDATEPATHGRAPH` und wird mit Duplikatpruefung wiederholt.
- `DZATM_RobTool` verwendet die vom Owner bestaetigte funktionierende `RobTool_crowbar_co.paa` aus dem CriminalZ-Medienbestand.
- PBO-Prefix in `pbo.json` auf `deutschz_atmraidez` korrigiert.
