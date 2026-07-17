# VEHICLE COMBAT AUDIT

## 1. Lizenzstatus

**BESTÄTIGT – proprietär / All rights reserved.** Der Autor erlaubt die
unveränderte direkte Nutzung auf einem Server, untersagt aber Reproduktion,
Verteilung, Änderung und Repacking. Eine Erlaubnis für Quellzugriff,
Dekompilierung oder abgeleitete Arbeiten fehlt. Daher wurden keine PBOs geöffnet,
extrahiert oder dekompiliert. Fremdcode und Fremdassets sind für eine
DeutschZ-Version gesperrt.

## 2. Sichtbare Struktur

Vier PBOs, vier BISIGNs, ein öffentlicher ESMods-Key, `mod.cpp`, `meta.cpp`, eine
offene Beispielkonfiguration und eine Konfigurations-README. Keine offene
Lizenzdatei und kein offener Quellcode.

Der später geprüfte Ordner
`C:\Users\patsl\Desktop\@DeutschZ_ShootingZ` enthält keine eigenständige
DeutschZ-Version: Alle vier PBOs sind per SHA-256 bytegleich mit den Workshop-
PBOs. Zwei Unterordner enthalten bereits entpackt wirkende Config-/Script- und
Animationsartefakte derselben Fremddateien. Diese Inhalte wurden wegen der
unveränderten Rechtebasis nicht gelesen oder übernommen.

## 3. Abhängigkeiten

Der Autor behauptet keine zusätzlichen Abhängigkeiten; der sichtbare Root-Patch
hat ein leeres `requiredAddons[]`. Interne PBO-Abhängigkeiten sind nicht geprüft.
Animation-Mods wie SurvivorAnimations und ExpansionAnimations werden ausdrücklich
als inkompatibel genannt. Sgunplay-Kompatibilität für Pistolen wird behauptet.

## 4. Tatsächliche Funktionen

Aus offenen Quellen **behauptet**, aber nicht runtime-/codeverifiziert:

- Pistolen-/Revolverfeuer aus Fahrer-, Beifahrer- und zwei Rücksitzen;
- Aktivieren/Deaktivieren eines Schießmodus;
- Fahrzeug-Inventarslots und Inventarzugriff;
- Magazin-, Patronen-, Reload- und Unjam-Aktionen;
- Essen, Trinken, Crafting und Selbstbehandlung im Fahrzeug;
- eigene Passagier-/Bandageanimationen;
- Fahrzeug-Vicinity-Filterung.

„Tatsächlich implementiert“ kann ohne zulässige Codeeinsicht oder separaten
Gameplaytest nicht bestätigt werden.

## 5. Aktuelle Einschränkungen

- nur Pistolen und Revolver dürfen laut Autor feuern;
- Gewehre können nur transportiert/manipuliert werden;
- Fahrzeugbewegung ist im Schießmodus blockiert, gleichzeitiges Fahren und
  Schießen funktioniert laut Autor nicht;
- nur Vanilla-Fahrzeugfamilien/Boote werden zugesichert;
- starke Konfliktgefahr mit Animation-Mods;
- ADS, Hipfire-Details, Mündung, Netzwerksynchronisierung, Tod/Bewusstlosigkeit
  und serverseitige Validierung bleiben offen.

## 6. Engine-Grenzen

Vanilla trennt `COMMANDID_VEHICLE` von normaler Waffenverarbeitung. Waffenfeuer
erfordert Raised-/WeaponManager-/FSM-Zustände; ADS benötigt passende Kamera- und
WeaponCommand-Zustände. Sitze liefern Transform und Animationstyp, aber keine
universelle sichere Schussöffnung. Bewegte Fahrzeug-, Kamera- und Muzzle-
Transforms verursachen zusätzliche Desync-Risiken. Langwaffen verschärfen
Clipping, Kollisions- und Animationsprobleme erheblich.

## 7. Risiken

- Schüsse durch Karosserieteile bei pauschalem Ignorieren des eigenen Fahrzeugs;
- Seat-/Speed-/Weapon-Spoofing ohne serverseitige Revalidierung;
- Magazine/Inventar-Duplikation bei Zustandswechseln;
- falsche Schussursprünge durch bewegte Transform-Desynchronisierung;
- festhängende Kamera, Eingabe oder Animation bei Ausstieg, Sitzwechsel, Tod oder
  Bewusstlosigkeit;
- PBO-/Animationskonflikte und unklare interne Addon-Reihenfolge.

## 8. Mögliche DeutschZ-Architektur

Eine vollständig eigenständige, serverautoritative Clean-Room-Mod
`DeutschZ_VehicleCombatZ` ist sinnvoll. Sie benötigt exakte Fahrzeug-/Sitzprofile,
serverseitige Sitz-, Waffen-, Geschwindigkeits- und Mündungsprüfung sowie strikt
getrennte Client-Input-/Animations- und Serverentscheidungslogik. Config und Logs
verwenden die verbindlichen `DeutschZ-System`-Pfade.

## 9. Empfohlener MVP

**Pistolen und Revolver, Beifahrer plus Rücksitze, wenige exakt geprüfte Vanilla-
Fahrzeuge, Hipfire, Fahrerfeuer aus, ADS aus, niedrige Maximalgeschwindigkeit,
Reload nur bei ruhiger Weapon-FSM und komponentenbewusste Mündungsprüfung.**

## 10. Realistisch unterstützbare Waffen

- realistisch zuerst: Pistolen, Revolver;
- danach mit Tests: kompakte SMGs;
- nur profilabhängig in offenen Sitzen: kurze Schrotflinten/Karabiner;
- nicht universal sinnvoll: lange Repetierer, Assault Rifles, LMGs;
- Optiken/ADS erst nach separaten Kamera- und Animationsprofilen.

## 11. Sinnvolle Fahrzeuge und Sitze zuerst

Zuerst ein geschlossenes Vanilla-Fahrzeug mit Beifahrerfenster und ein offenes
Fahrzeug/Boot, jeweils nur Beifahrer und definierte Rücksitze. Fahrer und beliebig
geerbte Modfahrzeuge bleiben außerhalb des MVP, bis eigene Profile getestet sind.

## 12. Klare Entscheidung

| Wertung | Ergebnis |
|---|---|
| A | Für einen kleinen Clean-Room-MVP technisch machbar |
| B | Der vollständige Wunschumfang ist nur mit starken Einschränkungen machbar |
| C | **Empfohlene Hauptentscheidung: zuerst nur Pistolen/Revolver** |
| D | **Langwaffen nur in bestimmten offenen Sitzen/Fahrzeugen sinnvoll** |
| E | Universell alle Waffen/Sitze/Fahrzeuge plus ADS und Fahrerfeuer ist nicht sinnvoll |
| F | **Wiederverwendung/Anpassung der Workshop-Mod ist lizenzrechtlich blockiert** |

Gesamturteil: **C + D für die Technik; F für jede Wiederverwendung des fremden
Produkts.** Eine eigene Clean-Room-Version ist besser kontrollierbar und rechtlich
sauberer, darf aber nicht aus dem geschlossenen Workshop-Code abgeleitet werden.

## Genau ein nächster Codex-Auftrag

**„Erstelle eine implementierungsfreie, testbare MVP-Spezifikation für
DeutschZ_VehicleCombatZ: Pistolen/Revolver, Beifahrer und Rücksitze, genau zwei
Vanilla-Fahrzeugprofile, Hipfire ohne ADS, Fahrerfeuer aus, serverautoritatives
Seat-/Speed-/Muzzle-Validierungsprotokoll und eine vollständige Testmatrix – ohne
fremden Code oder fremde Assets.“**
