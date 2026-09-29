# DeutschZ Points – Architektur- und Codex-Auftrag v1.0

## Ziel
Baue das DeutschZ-Punktesystem als dritte, klar getrennte Wirtschaft neben Expansion-Euro (Green Mountain) und US-Dollar (fahrender Schwarzmarkt). Punkte sind die Endgame-/Aktivitätswährung. Sie dürfen Euro oder Dollar nicht ersetzen.

## Nicht verhandelbare Regeln
1. Green Mountain bleibt Expansion-Euro-Markt.
2. Der bestehende fahrende Schwarzmarkt bleibt US-Dollar-Markt.
3. DeutschZ-Punkte werden als zusätzliche Kaufart am selben fahrenden Schwarzmarkt integriert. Kein neuer stationärer Punktehändler.
4. Beschränkte Items bleiben verkaufbar.
5. Bestehende Event-Lootquellen bleiben unverändert. KOTH, Courier, RAVEN, AI Convoy und andere Events dürfen beschränkte Items weiterhin ausgeben.
6. Beschränkte Items dürfen nicht normal auf der Map/CE spawnen und nicht normal gegen Euro oder Dollar gekauft werden.
7. Helme werden in diesem Auftrag nicht verändert.
8. Waffenbeschränkung betrifft ausschließlich Morty’s Weapons (TTC_*) und Anzio. Keine anderen Waffenmods in die Restriktion einbeziehen.
9. Keine Zugangsdaten, Tokens oder Secrets in PBO, Client-Dateien, GitHub oder Website schreiben.

## Voranalyse vor jeder Änderung
Codex muss zuerst den realen IST-Stand auswerten:
- vorhandenes deutschz_serverpack und seine PBO-/Script-Struktur
- aktuelle FOG Override Mod
- Morty’s Weapons Klassen/Config (TTC_*)
- Anzio-Mod Klassen/Config
- aktuelle Expansion-/Schwarzmarkt-Traderkonfiguration
- bestehender mobiler Schwarzmarkt-LKW, Route, Stop-/Open-Logik und Warenbestand
- types.xml / CE-Dateien und etwaige Economy-Overrides
- alle Eventloot-Konfigurationen, ohne sie pauschal zu ändern
- CourierZ Geld-/Kofferlogik
- aktuelle Geldscheinklasse: Einzelwert 100, gewünschte Stackmenge 250
- vorhandene Top-Games-Konfiguration, API-/Webhook-Möglichkeiten
- aktuelle Persistenz-/Profilpfade und Server-Logs

Danach einen Audit-Bericht mit ClassName, Quelle, aktuellem Spawn-/Trader-/Eventstatus und geplanter Änderung erzeugen.

## Mod-/Client-Architektur
Die neuen Wertmarken sind echte neue Itemklassen und müssen deshalb für Clients bekannt sein. Nicht als reinen -serverMod bauen, wenn die Itemklassen sonst beim Client unbekannt wären.
Bevorzugt in die bestehende DeutschZ-Mod-/Serverpack-Struktur integrieren, statt unnötig einen weiteren Workshop-Mod zu erzeugen.
Server-only Logik (Top-Games-Abfrage, Konten, Persistenz, Bestandsrotation) darf serverseitig ausgeführt werden, aber Secrets liegen ausschließlich in Serverprofil-/Configdateien außerhalb des Client-PBO.

## Physische DeutschZ-Wertmarken
Vanilla PunchedCard als Modellbasis verwenden. Kein neues 3D-Modell, keine neue Textur erforderlich.
Eigene Klassen mit identischem Punch-Card-Modell und deutschen Stringtable-Texten:
- DeutschZ-Wertmarke – 1 Punkt
- DeutschZ-Wertmarke – 2 Punkte
- DeutschZ-Wertmarke – 3 Punkte
- DeutschZ-Wertmarke – 5 Punkte
- DeutschZ-Sondermarke – 25 Punkte

Bunker-/PunchedCard-Sonderaktionen sowie ungeeignete geerbte Papier-/Feuer-/Crafting-Nutzung für diese Klassen sperren.
Normale CE-Werte der eigenen Marken auf 0 setzen; kein normaler Mapspawn.
Originale PunchedCard auf Chernarus ebenfalls prüfen und sicherstellen, dass sie nicht versehentlich als normale Wertmarke erscheint.
Stackbarkeit möglichst inventarschonend umsetzen, aber nur mit einer in der aktuellen DayZ-Version tatsächlich funktionierenden Item-/Quantity-Konfiguration. Keine nicht unterstützte Stacklogik vortäuschen.

## Zombie-Wertigkeit
Punkte werden NICHT beim Kill direkt gutgeschrieben. Der Spieler muss die Marke an der Leiche finden, aufnehmen, transportieren und beim fahrenden Schwarzmarkt einlösen.
Wert:
- Stadt/Civil = 1
- Farm = 1
- Hunting = 2
- Police = 2
- Military = 3
- Mumie = 5

Codex soll die real vorhandenen Infected-ClassNames prüfen und in einer konfigurierbaren Zuordnung ablegen. Nicht nur auf Displaynamen verlassen.
Marke beim Tod in/bei der Leiche erzeugen. Wenn das Inventar der Leiche keinen Platz hat, kontrolliert am Körper ablegen.
Die 25-Punkte-Sondermarke nicht normalen Zombies geben. Sie ist für konfigurierte Boss-/Eventbelohnungen vorgesehen.

## Digitales Punktekonto
Persistentes Konto pro eindeutiger Spieler-ID/Steam64.
Speichern:
- aktuelles Guthaben
- gesamte verdiente Punkte nach Quelle
- eingelöste physische Marken
- Vote-Gutschriften
- Monatsboni
- Käufe
- Admin-Korrekturen
- Transaktionsjournal mit eindeutiger ID/Quelle/Zeitpunkt

Neustart darf Guthaben, Bestände oder bereits verarbeitete Votes nicht duplizieren.

## Top-Games Vote-Wertung
Primäre Wertung soll serverseitig laufen und nicht vom lokalen Discord-Bot abhängen.
Top-Games regelmäßig abfragen, Standardintervall zunächst 600 Sekunden, konfigurierbar.
Tagesbelohnung:
- 1. Vote eines Tages = +5 Punkte
- 2. Vote eines Tages = +10 Punkte
- 3. Vote eines Tages = +25 Punkte
- ab 4. Vote = 0 direkte Punkte

WICHTIG: Jeder gültige Vote, auch ab dem 4., zählt weiter für das Monatsranking.
Tagesgrenze soll Deutschland/Europe-Berlin entsprechen. Nicht stillschweigend eine falsche UTC-Tagesgrenze verwenden.
Monatsbonus vorläufig konfigurierbar:
- Platz 1 = +75
- Platz 2 = +50
- Platz 3 = +25

Monatsranking startet neu; bestehendes Punkteguthaben bleibt erhalten.
API-Differenzen deduplizieren. Bei mehreren neuen Votes seit letztem Poll die Reihenfolge innerhalb des Tages korrekt nachbilden, soweit die Top-Games-Daten das zulassen. Wenn die API die notwendige Zeitauflösung nicht liefert, Verhalten dokumentieren und keine erfundenen Zeitstempel erzeugen.
Unbekannte Top-Games-Namen nicht irgendeinem Spieler gutschreiben: Pending-Credits führen, bis die Zuordnung Top-Games-Name -> Steam64 eindeutig ist.
Discord-Bot darf Rankings/Statistik anzeigen, ist aber nicht systemkritisch.

## Mobiler Schwarzmarkt: Stop-spezifischer Punktehandel
Bestehenden fahrenden Händler verwenden.
Route:
Sinistok -> Topolniki -> Novaya -> Nordroute -> Novomirovsk

Es gilt zwingend: Nicht jeder Stopp verkauft alle Punktewaren.
Jeder Stopp hat einen eigenen konfigurierbaren Warenpool; pro Händlerumlauf wird daraus nur ein Teilbestand angeboten.

Vorgesehene Schwerpunkte:
- Sinistok: FOG-Pouches und kleinere Sonderausrüstung
- Topolniki: NVGs und 400-/450-Slot-Rucksäcke
- Novaya: obere Schutzwesten und .338-Waffen
- Nordroute: .338/.50 Magazine + Munition, .50 Beowulf, 500-Slot-Rucksäcke
- Novomirovsk: M82-Familie, Anzio und absolute Spitzenware

Der existierende Dollarhandel am LKW bleibt parallel bestehen.
Punkteware muss am aktiven Stopp sichtbar/kaufbar sein, Dollarware weiterhin wie bisher.
Bestand wird pro vollständigem Umlauf erzeugt und persistent geführt. Serverrestart darf den Bestand nicht neu auffüllen.
Beispiel-Limits pro Umlauf als Ausgangspunkt, konfigurierbar:
- Anzio 0-1
- M82 0-1
- andere .50 1-2
- .338 1-3
- 500-Slot-Rucksack 1
- 450-Slot-Rucksack 1-2
- Top-Schutzweste 1-2
- NVG 2-4
- Pouches mehrere

Optionales Lieferprofil pro Umlauf:
- Standard
- Militärausrüstung
- schwere Waffen
- seltene Speziallieferung
Dies darf Sortiment/Gewichtung verändern, nicht die Grundregeln umgehen.

## Waffenrestriktion – nur Morty’s + Anzio
Nur folgende Quellen untersuchen:
1. Morty’s Weapons, ClassNames TTC_*
2. Anzio-Mod

Morty’s: alle Waffen ab .338 einschließlich erfassen, insbesondere .338, .50 Beowulf, .50 BMG und größere vorhandene Kaliber.
Erwartete Kandidaten nur als Gegenprüfung: M24 .338, MRAD, F&D Defense .338, M82-Varianten, Beowulf, Gebar 43. ClassNames nicht raten, aus installierter Config ermitteln.
Anzio samt tatsächlichem Kaliber vollständig erfassen.

Für jede eingeschränkte Waffe automatisch die komplette Versorgungskette erfassen:
- Waffe
- alle passenden Magazine
- lose Munition
- vorhandene Munitionsboxen

Keine andere Waffenmod beschränken.

## FOG-Pouches
Alle echten FOG-Pouches, die nach effektivem FOG Override zusätzlichen Cargo-/Inventarplatz liefern:
- kein normaler CE-/Mapspawn
- kein normaler Euro-/Dollar-Kauf
- Punktekauf erlaubt
- bestehende Eventbelohnungen unverändert
- normaler Verkauf weiterhin erlaubt

Nicht blind jede Klasse aus einer Datei namens POUCHES sperren; echte Funktion/Cargo prüfen.

## FOG-Rucksäcke
Bestehenden FOG Override prüfen.
Wenn die Größen nicht logisch gestaffelt sind, auf folgende Stufen vereinheitlichen:
100 / 150 / 200 / 250 / 300 / 350 / 400 / 450 / 500 Slots.
Farbvarianten einer Rucksackfamilie erhalten dieselbe effektive Größe.
Modelle sinnvoll nach tatsächlicher/gedachter Rucksackgröße einordnen.
Punkteware werden ausschließlich 400, 450 und 500 Slots.
100-350 bleiben normales Equipment.

## FOG-Schutzwesten
Nur gepanzerte FOG-Westen bewerten.
Maßgeblich sind die effektiven Werte aus dem bestehenden FOG Override, nicht die Originalwerte der Fremdmod.
Farbvarianten als eine Westenfamilie behandeln.
Nach effektiver Schutzwirkung sortieren; Schussschutz primär, danach Shock/sonstige relevante Schutzwerte und Haltbarkeit.
Die stärksten 25 % der Westenfamilien werden Punkteware; bei nicht glatter Teilung aufrunden.
Alle Westen bleiben verkaufbar.
Helme ausdrücklich NICHT verändern.

## NVGs
Alle echten Nachtsichtgeräte/NVGs:
- kein normaler CE-/Mapspawn
- kein normaler Euro-/Dollar-Kauf
- Punktekauf erlaubt
- Events unverändert
- Verkauf weiterhin erlaubt

## Allgemeine Bezugsweg-Regel für beschränkte Items
- normaler Map-/CE-Loot: NEIN
- normaler Kauf mit Expansion-Euro: NEIN
- normaler Kauf mit US-Dollar: NEIN
- Kauf mit DeutschZ-Punkten: JA
- bestehende Events: JA, unverändert
- PvP-/Spielerbeute: JA
- Verkauf an vorhandene Händler: JA
- Verkauf gibt kein Punkteguthaben zurück

Wichtig: Nicht durch globales Löschen eines ClassNames versehentlich Eventloot oder Verkauf entfernen. Bezugswege getrennt behandeln.

## Preisrahmen v1
Preise konfigurierbar halten; dies sind Startwerte:
- kleine FOG-Pouch: 75
- mittlere FOG-Pouch: 100
- große FOG-Pouch: 125
- extrem große FOG-Pouch: 150
- NVG: 200-250
- 400-Slot-Rucksack: 225
- 450-Slot-Rucksack: 275
- 500-Slot-Rucksack: 350
- Top-25%-Schutzweste: 275-375
- .338 Munition: 35-50
- .338 Magazin: 50
- .338 Waffe: 425-475
- .50 Beowulf Munition: 50
- .50 Beowulf Magazin: 60
- .50 Beowulf Waffe: 500
- .50 BMG Munition: 70-80
- .50 BMG Magazin: 85-100
- M82: 600-650
- Anzio-Munition: 90-110
- Anzio-Magazin: 110-130
- Anzio: 800-850

Balancing-Ziel:
Ein typischer Spieler mit etwa 2 Stunden Spielzeit pro Tag, ca. 30 gelooteten Zombies und nebenbei einigen Votes soll ungefähr eine Woche für eine hochwertige Anschaffung benötigen. Sehr aktive Spieler dürfen schneller sein; harte Begrenzung entsteht zusätzlich durch Route, Stopp, zufälliges Sortiment und begrenzte Stückzahlen.

## CourierZ Geld-Fix
Im Courier-Koffer soll genau EIN Geldstack liegen:
- 250 Scheine
- vorhandener Einzelwert je Schein = 100
- Gesamtwert = 25.000
- genau 1 Geld-Entity, Quantity/Stack = 250

Nicht 250 einzelne Geld-Entities erzeugen.
Die vorhandene Geldscheinklasse und deren aktuellen Wert nicht unnötig ändern.
Falls ihre Stackobergrenze unter 250 liegt, gezielt auf mindestens 250 anheben und prüfen, dass dies keine unerwünschten Nebeneffekte an anderen Geldquellen erzeugt.
Courier-Log ergänzen:
- verwendete Geldklasse
- Entity-Anzahl
- Quantity
- Einzelwert
- Gesamtwert

## Konfiguration
Alle Balancewerte auslagerbar, mindestens:
- Enabled
- TopGamesEnabled
- PollIntervalSeconds
- VoteRewards [5,10,25]
- MonthlyBonuses [75,50,25]
- Token-ClassNames/Werte
- Infected-Zuordnungen
- RestrictedWeapons/Ammo/Magazines
- RestrictedPouches
- RestrictedBackpacks
- RestrictedArmor
- RestrictedNVGs
- AllowEventLoot
- AllowSell
- DisableCESpawn
- RouteStops und Stop-Warenpools
- Rotation-/Stocklimits
- Preise und Item-Overrides

Top-Games-Token nur in serverseitiger Profil-/Secret-Konfiguration, niemals im Repository.

## Repository-/Build-Regeln
- Vorhandene Struktur respektieren.
- Fremde Moddateien nicht ins Repository kopieren.
- Keine PBO/P3D/PAA/Keys fremder Mods committen.
- Eigene Source nach bestehender DeutschZ-Mod-Struktur ablegen.
- docs/architecture für Systemvertrag, docs/testing für Tests.
- Bestehende Source aus dem vorbereiteten DeutschZ-Points-Paket nur als Ausgangspunkt behandeln und gegen den aktuellen Serverstand prüfen; nicht blind übernehmen.

## Tests / Abnahme
Mindestens folgende Tests dokumentieren:
1. Source/PBO kompiliert ohne Scriptfehler.
2. JSON/XML/CSV/Stringtable valide.
3. Jede Wertmarkenklasse spawnt korrekt und zeigt deutschen Namen/Wert.
4. Bunker-/Papier-/Feuer-Missbrauch der Wertmarke nicht möglich.
5. Stadt/Farm/Hunting/Police/Military/Mumie erzeugen den richtigen Wert.
6. Kill allein vergibt keine Punkte; erst Einlösen.
7. Marken sind bei Tod lootbar.
8. Vote 1/2/3 eines Tages gibt 5/10/25.
9. Vote 4+ gibt 0 direkte Punkte, zählt aber Monatsranking.
10. Tageswechsel funktioniert korrekt.
11. Monatswechsel vergibt 75/50/25 nur einmal und löscht Guthaben nicht.
12. Wiederholtes API-Polling erzeugt keine Doppelgutschrift.
13. API-Ausfall stoppt nicht den Server; Retry ohne Datenkorruption.
14. Serverrestart erhält Guthaben, Vote-Snapshot, Händlerbestand und Rotation.
15. Unzugeordnete Voter landen in Pending statt falscher Gutschrift.
16. Nur Morty’s + Anzio Waffen betroffen.
17. Waffe/Magazin/Munition/Munitionsbox konsistent eingeschränkt.
18. FOG-Pouches nach effektivem Cargo korrekt erkannt.
19. FOG-Rucksackstaffel 100-500 logisch und Varianten konsistent.
20. Nur 400/450/500-Slot-Rucksäcke Punkteware.
21. Stärkste 25 % FOG-Westen korrekt aus Override abgeleitet.
22. Helme unverändert.
23. NVGs nur per Punkte kaufbar, aber Eventloot/Verkauf bleibt.
24. Beschränkte Items spawnen nicht normal in CE.
25. Beschränkte Items sind nicht normal für Euro/Dollar kaufbar.
26. Beschränkte Items können weiter verkauft werden.
27. KOTH/Courier/RAVEN/AI Convoy und sonstige bestehende Eventlootquellen bleiben unverändert.
28. Jeder Schwarzmarkt-Stopp zeigt nur seinen eigenen Punktewarenpool.
29. Nicht jedes Pool-Item muss pro Umlauf verfügbar sein.
30. Ausverkaufte rare Ware respawnt nicht durch Restart.
31. Neuer vollständiger Umlauf erzeugt genau einmal neuen Bestand.
32. Courier: genau 1 Geld-Entity mit Quantity 250 und Gesamtwert 25.000.
33. RPT/Logs nach Test frei von neuen Crash-/Error-Spam.
34. Regressionstest des bestehenden EventSchedulers und mobilen Schwarzmarkts.

## Ergebnislieferung
Codex liefert:
- geänderte Source
- genaue Dateiliste
- Item-Audit-Tabelle mit ClassNames und finaler Zuordnung
- Morty’s/Anzio Kaliber-/Magazin-/Ammo-Matrix
- FOG-Rucksackliste mit alten/neuen effektiven Slots
- FOG-Westenranking und markierte Top 25 %
- NVG-/Pouch-Liste
- Stop-/Warenpool-Matrix für Sinistok, Topolniki, Novaya, Nordroute, Novomirovsk
- finale Preis-/Bestandsconfig
- Courier-Fix
- Testprotokoll
- relevante RPT-Auszüge
- Rollback-Hinweise
- keine Aussage "fertig/live", bevor die Tests tatsächlich bestanden sind.

## Abnahmekriterium
Das System ist erst abnahmebereit, wenn Punkte aus Zombies und Votes persistent funktionieren, der mobile Schwarzmarkt je Stopp unterschiedliche begrenzte Punkteware anbietet, Euro/Dollar-Handel erhalten bleibt, Eventloot nicht beschädigt wurde, beschränkte Items nicht normal kaufbar/spawnbar sind, Verkauf weiterhin funktioniert und der Courier-Koffer nachweislich nur einen Stack mit 250 Scheinen enthält.