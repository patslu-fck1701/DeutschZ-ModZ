# CODEX HANDOFF CURRENT

Synchronisiert: 17.07.2026 02:05 CEST

## KotHZ Free 1.0.3 Settings Preserve

- Restartschutz: gueltige `KotHZSettings.json` und `KotHZLocations.json` werden nur geladen und nicht mehr bei jedem Serverstart neu geschrieben.
- Neuerzeugung erfolgt nur, wenn die jeweilige Datei fehlt oder nicht lesbar ist. Vor der Neuerzeugung einer defekten Datei bleibt eine unveraenderte `*.corrupt.backup.json` erhalten.
- Standard-Admin: Steam-ID `76561199819501556` ist im Code-Default und in der ausgelieferten Beispielkonfiguration eingetragen.
- Restart-Nachweis: zwei echte Serverstarts mit identischem SHA-256 `D8FE894865A623A4A5BB0D4DB96438EC7FC386EF533F8E86F100547EF95B1302`, identischem Zeitstempel und erhaltener Admin-ID.

## KotHZ Free 1.0.2 Gameplayfix

- Musik: Standard und serverseitiges Maximum `0.45`; Empfaenger ausschliesslich lebende Spieler im Eventradius. Stop bei Radiusverlassen, Abbruch, Ende, Cleanup und Missionende. Client ersetzt eine laufende Instanz vor jedem Start.
- Assets: neue Flaggen-, RewardChest- und FireworksLauncher-PAA bytegleich aus Patricks Quellen uebernommen. Selections: Flagge `camo`, SeaChest `camoGround`, Launcher `camo` und `placing`.
- Gewinn: genau ein Vanilla-`FireworksLauncher` als `DZKOTHF_FireworksLauncher`, lokaler Mast-Offset `[3,0,0]`; RewardChest `[-3,0,0]`. Spawn, Start und Cleanup wurden im DayZ-Labs-Test protokolliert.
- Standorte: 11 konfigurierbare Kandidaten in `KotHZLocations.json`; echte serverseitige Zufallsauswahl, direkter Repeat wird vermieden. `Persistence/location_rotation.json` speichert nur letzten Index und Namen.
- Fuenf echte Starts: `NWAF -> Pustoshka Military -> YRAP -> Krasnostav Airfield -> Tri-Kresta`; beim letzten Start wurde ein Repeat aktiv vermieden.

## Nachweise

- Settings-Restarttest: zwei echte Starts im Preset `kothz_free_compile`; SHA-256 und Zeitstempel der gueltigen Config blieben bytegenau unveraendert, die Standard-Admin-ID blieb enthalten.
- Frische Logs: `script_2026-07-17_02-01-20.log` und `DayZServer_x64_2026-07-17_02-01-09.RPT`; Script-Compile und Missionstart ohne KotHZ-Fehler.
- Preflight und Clean Build: PASS
- PBO-Inhalt: drei PAAs, Locations-Script und acht Musiktracks vorhanden
- Signatur: `DeutschZ.bikey` / `.pbo.DeutschZ.bisign` PASS
- Script-Compile und Missionstart: PASS
- Gewinn-Testhook nur im lokalen Testpreset: `ACTIVE -> CAPTURE_COMPLETE -> REWARD -> CLEANUP -> IDLE`, RewardChest sowie `FIREWORK SPAWNED`, `FIREWORK STARTED`, `FIREWORK CLEANED` PASS. Testhook anschliessend aus Mission entfernt.
- Sichtbarer Clienttest fuer Texturen, Lautstaerke und Feuerwerk bleibt erforderlich.

## Ausgabe

- `E:\DeutschZ\DeutschZServer\@DeutschZ_KotHZ_Free`
- Config: `$profile:DeutschZ-System/DeutschZ_KotHZ/KotHZSettings.json`
- Locations: `$profile:DeutschZ-System/DeutschZ_KotHZ/KotHZLocations.json`
- History: `$profile:DeutschZ-System/DeutschZ_KotHZ/Persistence/location_rotation.json`
- Log: `$profile:DeutschZ-System/LogZ/DeutschZ_KotHZ/KotHZ.log`

Uebertragene Dateien:

- `Addons/DeutschZ_KotHZ_Free.pbo`
- `Addons/DeutschZ_KotHZ_Free.pbo.DeutschZ.bisign`
- `keys/DeutschZ.bikey`
- `mod.cpp`
- `meta.cpp`
- `Settings/KotHZSettings.json`
- `Settings/KotHZLocations.json`
- `TESTANLEITUNG.md`

Build: PASS. Signatur: PASS. Servertest: PASS. Gameplaytest fuer den Settings-Restartschutz nicht erforderlich; sichtbare Gameplay-Resttests aus 1.0.2 bleiben bestehen.
