# CODEX HANDOFF CURRENT

Letzte Aktualisierung: 16.07.2026, 04:26 Uhr

## KotHZ Free Phase C

- Branch: `codex/kothz-free-v1-core-20260715`
- Status: BUILD READY = JA, SERVER TEST READY = JA, CLIENT/GAMEPLAY VALIDATED = NEIN
- Funktionen: Mast, exakt eine Fahne, Capture/Radius, White/Green/Red Smoke, 15 konfigurierbare Vanilla-Infizierte, eine JSON-RewardCrate, Vanilla Notify/Chat-Fallback, genau ein Vanilla-Marker-System, genau eine neutrale Progressbar, zentraler Cleanup.
- Nicht enthalten: Expansion-Pflicht, CF-Pflicht, Wellen, Boss, Spezial-Infected, Terminal, KOTHGate, Keycard, Premium.

## Verbindliche Profilpfade

- Config: `$profile:DeutschZ-System/DeutschZ_KotHZ/KotHZSettings.json`
- Log: `$profile:DeutschZ-System/LogZ/DeutschZ_KotHZ/KotHZ.log`
- Hoster: `/gameserver/profiles/DeutschZ-System/DeutschZ_KotHZ/` und `/gameserver/profiles/DeutschZ-System/LogZ/DeutschZ_KotHZ/`
- Altpfad wird einmalig gelesen/migriert und niemals still geloescht.

## Build und Test

- Preset: `deutschz_kothz_free_compile`
- aktive Mod: nur `P:\Mods\@DeutschZ_KotHZ_Free`
- Preflight: PASS, 0 Fehler, 1 bekannte Windows-Grossbuchstabenwarnung
- Clean Build / PBO / DeutschZ-Signatur: PASS
- Script-Compile / Missionstart / `Player connect enabled`: PASS
- Nachtauftrag Gate 1 erneut frisch getestet: 16.07.2026, 04:23 bis 04:25 Uhr, mehr als zwei Minuten stabil: PASS
- Runtime-Beleg: Mast 1, Fahne 1, Smoke WHITE -> GREEN, 15/15 Vanilla-Infizierte, Wellen/Boss/Expansion AI 0/0/0
- Settings-Migration und UTF-8 ohne BOM: PASS; beide Altdateien bleiben erhalten
- modbezogene Script-/RPT-Fehler: 0
- Sichtbarkeit, MapWidget, Popup, Progressbar, Capture, Reward und Cleanup im Gameplay: PATRICK_REQUIRED

## Ausgabe

- letzter Ausgabeordner: `E:\DeutschZ\DeutschZServer\@DeutschZ_KotHZ_Free`
- Synchronisierung: 16.07.2026, atomar ueber externes Staging
- enthalten: PBO, BISIGN, oeffentlicher BIKEY, `mod.cpp`, `meta.cpp`, `KotHZSettings.json`, Testanleitung und Phase-A-bis-C5-/Gate-1-Testdokumentation
- alte aktive PBO/BISIGN ersetzt: JA
- gemischte Altversionen: NEIN

### Uebertragene Dateien Gate 1

- `Addons/DeutschZ_KotHZ_Free.pbo`
- `Addons/DeutschZ_KotHZ_Free.pbo.DeutschZ.bisign`
- `keys/DeutschZ.bikey`
- `mod.cpp`
- `meta.cpp`
- `Settings/KotHZSettings.json`
- `docs/NIGHT_PHASE_1_GATE.md`
- `docs/PHASE_A_ARCHITECTURE.md`
- `docs/PHASE_A_TEST.md`
- `docs/PHASE_B_ARCHITECTURE.md`
- `docs/PHASE_B_TEST.md`
- `docs/PHASE_C1_DAYZ_LABS_COMPILETEST.md`
- `docs/PHASE_C1_TEST.md`
- `docs/PHASE_C2_DAYZ_LABS_COMPILETEST.md`
- `docs/PHASE_C2_TEST.md`
- `docs/PHASE_C3_DAYZ_LABS_COMPILETEST.md`
- `docs/PHASE_C3_TEST.md`
- `docs/PHASE_C4_DAYZ_LABS_COMPILETEST.md`
- `docs/PHASE_C4_TEST.md`
- `docs/PHASE_C5_DAYZ_LABS_COMPILETEST.md`
- `docs/PHASE_C5_TEST.md`
- `docs/TESTANLEITUNG.md`

## Installation und Test

1. Kompletten Ausgabeordner auf Client und Server laden und `keys/DeutschZ.bikey` serverseitig bereitstellen.
2. Settings nach dem ersten Start ausschließlich im neuen Configpfad bearbeiten.
3. Capture mit einem und umkaempften Capture mit mehreren Spielern pruefen.
4. Marker, Notify und Progressbar visuell pruefen.
5. RewardCrate-Inhalt, 500 Slots, Nicht-Aufnehmbarkeit und zehn Minuten Lifetime pruefen.
6. Abbruch, Abschluss, Disconnect und Serverrestart auf vollstaendiges Cleanup pruefen.

## DeutschZ UiAdminZ

- Assetbestand inventarisiert: 2.885 Dateien, Originale unveraendert.
- Quellmod noch nicht angelegt.
- Sicherheitsblocker: In den installierten Vanilla-Skripten wurde keine belastbare serverseitige Adminrechte-API gefunden. Eine UID-Liste wird nicht erfunden. Start/Stop muss bis zur Festlegung einer sicheren vorhandenen Adminquelle deaktiviert bleiben.
- KotHZ Free ist von UiAdminZ vollstaendig unabhaengig.
