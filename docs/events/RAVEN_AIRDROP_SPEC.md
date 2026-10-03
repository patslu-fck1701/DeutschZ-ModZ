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


## Verknüpfte Loot-Basis
Verbindliche IST-Masterliste: [`docs/loot/EVENT_LOOT_MASTERLIST_2026-10-02.md`](../loot/EVENT_LOOT_MASTERLIST_2026-10-02.md), basierend auf `Server_Stand_02.10.2026_20_00_Uhr.zip`. Sie liefert reale TTC-/FOG-/CE-/KOTH-/AIConvoy-Kandidaten; diese RAVEN-Spezifikation definiert darauf aufbauend das Soll.

## Ergänzung: Hack-, Loot- & Ausrüstungssystem

### Hack-Ergebnis
Der vollständige erste Hackversuch wird immer bis zum Ende ausgeführt. Erst **nach Abschluss** wird das Ergebnis bestimmt:

- **75 %:** Hack erfolgreich; Airdrop wird freigegeben.
- **25 %:** Hack fehlgeschlagen; Airdrop bleibt verschlossen und der Spieler muss den **vollständigen Hackvorgang erneut** durchführen.

Bergungstrupp und exakter öffentlicher 3D-Marker werden bereits beim **Start des ersten Hacks** ausgelöst und bei einem Fehlschlag nicht zurückgenommen. Dadurch kostet der Fehlschlag reale Zeit unter erhöhtem AI-/Zombie-/PvP-Risiko.

Für weitere Versuche ist vor der Implementierung die bestehende Hack-Source zu prüfen; insbesondere darf nicht geraten werden, ob nur der erste Versuch probabilistisch ist oder jeder Folgeversuch erneut würfelt. Verbindlich festgelegt ist zunächst die 75/25-Entscheidung des ersten vollständig ausgeführten Versuchs und der vollständige Wiederholungshack nach Fehlschlag.

### Loot-Grundregel
RAVEN erhält **keinen frei erfundenen neuen High-End-Lootpool**. Balance-Referenz ist der bestehende **King-of-the-Hill-Loot**.

Konkrete Classnames werden ausschließlich aus realen DeutschZ-Server-/Source-Dateien, `types.xml`, vorhandenen Eventkonfigurationen und installierten Mods übernommen.

Waffen:
- primär vorhandene Mortys-Weapons-Classnames mit Präfix `TTC_`
- keine unnötige Rückkehr zu Vanilla-Waffen, wenn eine vorgesehene TTC-Variante existiert
- stärkere Sniper/Long-Range-Waffen gesondert gegen KOTH-Balance prüfen

Hochwertige taktische Ausrüstung:
- primär Forward Operator Gear / FOG
- Kleidung, Westen/Plattenträger, Helme, Ghillies/Tarnung, Taschen und taktische Ausrüstung
- FOG-Classnames niemals raten

### `types.xml` als Datenquelle
`nominal = 0` ist ein **Prüfhinweis**, kein automatisches Eventloot-Kriterium.

Für jedes Kandidatenitem sind mindestens zu erfassen:
- Classname
- Mod-Zugehörigkeit
- Kategorie
- nominal
- min
- relevante flags
- bestehende Eventkonfiguration
- KOTH-Zugehörigkeit
- bestehende Spezialloot-Pools
- tatsächlicher Spawn-/Eventstatus

Ziel ist eine zentrale Masterliste:

**Classname → Mod → Kategorie → Nominal → Event → Spawnstatus**

### RAVEN RECON
Schwerpunkt: Aufklärung, Tarnung, Long Range.

Kandidatenkategorien:
- TTC Sniper/Long-Range
- passende Magazine und Munition
- hochwertige Optik
- passende Attachments/Schalldämpfer
- FOG Ghillie/Tarnkleidung/taktische Ausrüstung
- optional NVG
- kleine medizinische Versorgung

Konkrete Waffen erst nach Source-/`types.xml`-Audit auswählen.

### RAVEN ASSAULT
Schwerpunkt: direkter militärischer Kampf.

Kandidatenkategorien:
- TTC Assault Rifle
- mehrere passende Magazine
- Munition
- Optik, Griffe, Mündungs-/Waffen-Attachments
- FOG Weste/Plattenträger
- FOG Helm
- taktische Kleidung
- optional NVG
- medizinische Grundversorgung

### RAVEN NBC / SURVIVAL / MEDICAL
Schwerpunkt: Toxic-/NBC-Schutz, Survival und Medizin; **keine außergewöhnliche High-End-Hauptwaffe**.

Kandidaten:
- vorhandene DeutschZ-/ToxicZ-ABC-Komplettausrüstung
- Maske
- Schutzanzug
- Handschuhe
- Stiefel
- Kopfschutz
- ungefähr 3 Filter
- Verbände, Blut-/Infusionsmaterial, Medikamente und Survival-Ausrüstung
- normale TTC-Sekundärwaffe, z. B. vorhandene Pistolen-/Shotgun-Kategorie
- passende Magazine/Munition

Beim Source-Audit zugleich den bekannten Darstellungsfehler des speziellen NBC-/ABC-Anzugs prüfen: aktuell offenbar weiß. Realen Classname, Textur-/Materialreferenzen und Ursache belegen, nicht raten.

### Variabilität
Recon, Assault und NBC sind **thematische Schwerpunkte, keine starren Kisten**. Passende Cross-Pool-Items sind erlaubt, solange Charakter und KOTH-basierte Gesamtbalance erhalten bleiben.

### Balance
KOTH bleibt die Referenz. RAVEN soll nicht pauschal mehr oder stärkeren Loot ausschütten. Sein Mehrwert entsteht aus:
- thematischen Dropklassen
- RAVEN-spezifischer Zusammenstellung
- Hackzeit
- 25-%-Fehlschlagrisiko beim ersten vollständigen Hack
- stärkeren Zombies
- Bergungstrupp
- öffentlichem exaktem 3D-Marker ab Hackbeginn
- daraus entstehendem PvP-Risiko

### Eventloot-Masteraudit
Mortys / `TTC_*` aufteilen in:
Pistolen, SMGs, Assault Rifles, Battle Rifles, DMRs, Sniper, Shotguns, LMGs, Spezialwaffen.

FOG aufteilen in:
Kleidung, Westen/Plattenträger, Helme, Ghillies, Tarnung, Taschen, taktische Ausrüstung.

Zusätzlich:
NVGs, Optiken, Magazine, Munition, Attachments, Medizin, ABC/NBC, Filter.

Audit-Reihenfolge:
1. Welche Items haben `nominal = 0`?
2. Welche davon sind tatsächlich Event-/Spezialloot?
3. Welche sind bereits in KOTH?
4. Welche hochwertigen TTC-/FOG-Items fehlen dort?
5. Erst daraus RAVEN Recon/Assault/NBC-Pools erzeugen.
