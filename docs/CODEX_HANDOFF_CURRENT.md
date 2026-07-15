# CODEX HANDOFF CURRENT

## Aktueller Auftrag

- Produkt: DeutschZ KotHZ Free / `@DeutschZ_KotHZ_Free`
- Phase: B - Mast, eine Fahne, Capture-Radius, Capture-Fortschritt, Rauch und Cleanup
- Branch: `codex/kothz-free-v1-core-20260715`
- Status: BUILD READY = JA, SERVER TEST READY = JA, GAMEPLAY VALIDATED = NEIN
- MenuZ bleibt unveraendert in `E:\DeutschZ\DeutschZServer\@DeutschZ_MenuMusic` und wird von Patrick parallel getestet.

## Umgesetzter Phase-B-Umfang

- genau ein serververwalteter Vanilla-Mast `DZKOTHF_EventFlagpole`
- genau eine angehaengte Fahne `DZKOTHF_EventFlag`
- synchronisierter Fahnenfortschritt von 0 bis 100 Prozent
- konfigurierbarer Capture-Radius, Standard 25 Meter
- serverseitige Pruefung lebender Spieler im Radius
- Weiss in `ANNOUNCED`, Gruen in `ACTIVE`, Rot bei mehreren Spielern oder Abbruch
- Fortschritt nur bei genau einem lebenden Spieler; Pause bei null oder mehreren Spielern
- zentrale Objekt- und Zustandsverwaltung in `DZKOTHF_EventSession`
- vollstaendiges Cleanup bei Abbruch, normalem Ende und Mission-Shutdown
- Profilkonfiguration: `$profile:DeutschZ/KotHZ_Free/KotHZFreeSettings.json`

Nicht enthalten: Gegner, Rewards, Marker, Notify, Progressbar, Boss, Wellen, Terminal, KOTHGate oder Premiumfunktionen.

## Geaenderte und neue Source-Dateien

- `src/DeutschZ_KotHZ_Free/config.cpp`
- `src/DeutschZ_KotHZ_Free/mod.cpp`
- `src/DeutschZ_KotHZ_Free/scripts/3_Game/DeutschZ_KotHZ_Free/DZKOTHF_Constants.c`
- `src/DeutschZ_KotHZ_Free/scripts/3_Game/DeutschZ_KotHZ_Free/DZKOTHF_Settings.c`
- `src/DeutschZ_KotHZ_Free/scripts/3_Game/DeutschZ_KotHZ_Free/DZKOTHF_SmokeState.c`
- `src/DeutschZ_KotHZ_Free/scripts/4_World/DeutschZ_KotHZ_Free/DZKOTHF_EventController.c`
- `src/DeutschZ_KotHZ_Free/scripts/4_World/DeutschZ_KotHZ_Free/DZKOTHF_EventFlagpole.c`
- `src/DeutschZ_KotHZ_Free/scripts/4_World/DeutschZ_KotHZ_Free/DZKOTHF_EventSession.c`
- `src/DeutschZ_KotHZ_Free/example-settings/KotHZFreeSettings.json`
- `src/DeutschZ_KotHZ_Free/docs/PHASE_B_ARCHITECTURE.md`
- `src/DeutschZ_KotHZ_Free/docs/PHASE_B_TEST.md`
- `docs/DeutschZ_KotHZ_Free/PHASE_B_DAYZ_LABS_COMPILETEST.md`
- `docs/CODEX_HANDOFF_CURRENT.md`

## Build- und Teststatus

- Preflight: PASS, 0 Fehler, 1 bekannte Windows-Grossbuchstabenwarnung
- Clean Build: PASS
- PBO-Inhalt: PASS, 17 erwartete Dateien
- Signatur: PASS, `DeutschZ.bisign is OK`
- DayZ-Labs-Preset: `deutschz_kothz_free_compile`
- aktive Testmod: ausschliesslich `P:\Mods\@DeutschZ_KotHZ_Free`
- Script-Compile: PASS
- Missionstart / `Player connect enabled`: PASS
- stabil mindestens 2 Minuten nach Missionstart: PASS
- kontrollierter Stopp: PASS
- Script-Fatalfehler: 0
- KotHZ-Free-spezifische RPT-Fehler: 0
- Gameplay-/Clientbeweis: OFFEN

## Letzter aktualisierter Ausgabeordner

- Vollstaendiger Ausgabeordner: `E:\DeutschZ\DeutschZServer\@DeutschZ_KotHZ_Free`
- Zeitpunkt der Synchronisierung: 16.07.2026 00:33 Europe/Berlin
- Synchronisierung: atomar ueber externes Staging
- Build-/Signaturstatus: PASS / PASS
- alte PBO/BISIGN ersetzt: JA
- gemischte Altversionen: NEIN

### Uebertragene Dateien

- `Addons/DeutschZ_KotHZ_Free.pbo`
- `Addons/DeutschZ_KotHZ_Free.pbo.DeutschZ.bisign`
- `keys/DeutschZ.bikey`
- `mod.cpp`
- `meta.cpp`
- `Settings/KotHZFreeSettings.json`
- `docs/PHASE_A_ARCHITECTURE.md`
- `docs/PHASE_A_TEST.md`
- `docs/PHASE_B_ARCHITECTURE.md`
- `docs/PHASE_B_TEST.md`
- `docs/PHASE_B_DAYZ_LABS_COMPILETEST.md`

## Installation und Test

1. Den kompletten Ausgabeordner als Client- und Servermod laden.
2. Den oeffentlichen `DeutschZ.bikey` im Server-Keys-Ordner bereitstellen.
3. Server starten und an der konfigurierten `EventPosition` exakt einen Mast und eine Fahne pruefen.
4. Weiss waehrend Ankuendigung und Gruen nach Aktivierung pruefen.
5. Mit genau einem Spieler im Radius die Fahnenbewegung pruefen.
6. Mit zwei Spielern Rot und pausierenden Fortschritt pruefen.
7. Abbruch und normalen Abschluss getrennt auf vollstaendiges Cleanup pruefen.
8. Bei Fehlern frische RPT- und Scriptlogs sowie einen Screenshot liefern.

## Bekannte Grenzen

- Visuelle Fahnen-, Rauch- und Mehrspieler-Capture-Pruefung ist ohne Client nicht bewiesen.
- Das kontrollierte DayZ-Labs-Stoppen beendet den Prozess; ein Gameplay-Abbruch wurde nicht simuliert.
- Die bekannte Animation-Rootmeldung betrifft auch Vanilla `sakhal` und blockiert den Windows-Serverstart nicht.
