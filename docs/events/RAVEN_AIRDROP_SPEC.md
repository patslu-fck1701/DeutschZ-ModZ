# DeutschZ – RAVEN Airdrop

Stand: 03.10.2026  
Status: verbindlicher Soll-Ablauf; offene Source-/Assetpunkte sind ausdrücklich als Prüfung markiert.

## Schutzregel
Der bereits gut funktionierende Flugzeug-/Drop-Ablauf wird nicht unnötig umgebaut. Änderungen sollen gezielt an Vorankündigung, Landungsgefahr, Hack-Folgephase, Marker und Story-Dokument ansetzen.

## Soll-Ablauf

### 1. Vorankündigung – nur Funk
Vor jeder normalen Event-Benachrichtigung wird zuerst ausschließlich eine Funkdurchsage abgespielt.

- keine normale Notification in dieser Phase
- passende Sounddatei verwenden
- prüfen, ob die Datei bereits in den realen Source-/Asset-Files liegt
- falls nicht: alte/vorhandene Datei suchen oder neu hinterlegen
- erst nach dem Funkspruch folgt die öffentliche Eventmeldung

### 2. Öffentliche Airdrop-Ankündigung
Danach erhalten alle Spieler die Information: **„RAVEN Airdrop ist unterwegs.“**

Gleichzeitig:
- große rote Zone auf der Map
- absichtlich großer Suchradius
- exakte Position zunächst unbekannt
- Flugzeug fliegt schnell in den Bereich ein und wirft den Airdrop zügig ab
- dieser bereits funktionierende Teil soll nicht unnötig verändert werden

### 3. Landung / Event-Zombies
Nach der Landung sichern zunächst Zombies den Airdrop.

Zu prüfen/anzupassen:
- bestehendes System beibehalten
- Zombieanzahl leicht erhöhen
- Event-Zombie-HP ungefähr ×3 gegenüber normal
- nach Möglichkeit verhindern, dass Event-Zombies trivial mit Fahrzeugen überfahren werden können

### 4. Hackaktion als Phasen-Trigger
Der Airdrop ist nicht direkt zugänglich. Der Spieler startet die vorhandene Hackaktion.

**Der Beginn der Hackaktion ist der verbindliche Trigger für die nächste Phase.**

### 5. RAVEN-Bergungstrupp
Mit Beginn des Hacks wird ein bewaffneter AI-Bergungstrupp aktiviert/gespawnt.

Aufgabe:
- Airdrop zurückholen
- Hacker angreifen
- Bereich sichern

Mögliche Fraktionen: Russianz oder Americanz.

**Nicht festlegen, bevor bestehender RAVEN-Kanon und Source von RAVEN BLACK / RAVEN ECHO geprüft sind.**

### 6. Exakte Positionsmarkierung
Mit Beginn des Hacks wechselt das öffentliche Lagebild:

Vor Hack:
- große rote Suchzone
- keine exakte Position

Ab Hackbeginn:
- exakte 3D-Markierung des Airdrops für alle Spieler sichtbar

Dadurch wird öffentlich sichtbar, dass jemand am Drop arbeitet; PvP kann organisch entstehen.

### 7. Story-Dokument
Der geöffnete Airdrop enthält ein wichtiges Dokument für die weitere RAVEN-/DeutschZ-Story.

Zu prüfen:
- existiert bereits eine eigene Dokumenttextur?
- ist nur die Config-Referenz falsch?
- stimmen `hiddenSelectionsTextures` und Materialpfade?
- oder existiert noch keine eigene Grafik?

Wenn keine eigene Grafik existiert, Status ausdrücklich **„Textur noch erstellen“** und nicht „Codefehler“.

## Gesamtkette
Funkdurchsage  
→ noch keine Notification  
→ öffentliche Meldung „RAVEN Airdrop ist unterwegs.“  
→ große rote Suchzone  
→ RAVEN-Flugzeug kommt schnell herein  
→ Airdrop wird abgeworfen  
→ stärkere Event-Zombies sichern Bereich  
→ Spieler findet Airdrop  
→ Spieler startet Hack  
→ RAVEN-Bergungstrupp wird aktiviert + exakte 3D-Position wird öffentlich  
→ AI + stärkere Zombies + mögliches PvP  
→ Hack erfolgreich  
→ Airdrop öffnet  
→ Loot + Story-Dokument  
→ weiterer RAVEN-/Storyfortschritt

## Offene Source-/Asset-Prüfungen
1. Funk-Sounddatei vorhanden?
2. Welche RAVEN-Fraktion stellt den Bergungstrupp? RAVEN BLACK/ECHO zuerst prüfen.
3. Bestehende AI-Stärke und Anzahl?
4. Zombieanzahl leicht erhöhen.
5. Zombie-HP ungefähr ×3.
6. Überfahren der Event-Zombies verhindern bzw. erschweren.
7. Trigger der exakten 3D-Markierung kontrollieren: Hackbeginn.
8. Story-Dokument und zugehörige Funktion prüfen.
9. Dokumenttextur vorhanden/falsch referenziert oder **Textur noch erstellen**?
10. Funktionierenden Flugzeug-/Drop-Ablauf nicht umbauen.

## Bereits belegter Ist-Stand
Die vorhandene Live-Logauswertung vom 30.09.2026 belegt, dass RAVEN/AirdropZ grundsätzlich starten kann und mindestens ein Lauf STARTING, mehrere PHASE-Zustände, ACTIVE sowie Cleanup/Release erreicht hat. Scheduler-Restore/Provider-Staleness ist ein getrenntes Runtime-Thema und darf nicht mit diesem Eventdesign vermischt werden.

## Abnahme
- Audio kommt vor jeder sichtbaren Eventmeldung.
- Große Suchzone bleibt bis Hackbeginn ungenau.
- Bestehender schneller Flugzeug-/Drop-Pfad bleibt funktionsfähig.
- Landungsbereich ist bereits durch stärkere Zombies gefährlich.
- Hackbeginn startet Bergungstrupp und exakten öffentlichen 3D-Marker.
- Bergungstrupp-Fraktion ist aus vorhandenem Kanon/Source belegt, nicht geraten.
- Story-Dokument liegt im erfolgreichen Drop; Texturstatus ist technisch korrekt dokumentiert.
