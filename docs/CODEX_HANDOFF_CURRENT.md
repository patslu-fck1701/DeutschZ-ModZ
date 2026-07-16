# CODEX HANDOFF CURRENT

Synchronisiert: 17.07.2026

## KotHZ Free 1.0.2 Gameplayfix

- Musik: Standard und serverseitiges Maximum `0.45`; Empfaenger ausschliesslich lebende Spieler im Eventradius. Stop bei Radiusverlassen, Abbruch, Ende, Cleanup und Missionende. Client ersetzt eine laufende Instanz vor jedem Start.
- Assets: neue Flaggen-, RewardChest- und FireworksLauncher-PAA bytegleich aus Patricks Quellen uebernommen. Selections: Flagge `camo`, SeaChest `camoGround`, Launcher `camo` und `placing`.
- Gewinn: genau ein Vanilla-`FireworksLauncher` als `DZKOTHF_FireworksLauncher`, lokaler Mast-Offset `[3,0,0]`; RewardChest `[-3,0,0]`. Spawn, Start und Cleanup wurden im DayZ-Labs-Test protokolliert.
- Standorte: 11 konfigurierbare Kandidaten in `KotHZLocations.json`; echte serverseitige Zufallsauswahl, direkter Repeat wird vermieden. `Persistence/location_rotation.json` speichert nur letzten Index und Namen.
- Fuenf echte Starts: `NWAF -> Pustoshka Military -> YRAP -> Krasnostav Airfield -> Tri-Kresta`; beim letzten Start wurde ein Repeat aktiv vermieden.

## Nachweise

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
