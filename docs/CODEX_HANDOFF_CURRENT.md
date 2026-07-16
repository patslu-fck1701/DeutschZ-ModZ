# CODEX HANDOFF CURRENT

## KotHZ Free Phase C

- Branch: `codex/kothz-free-v1-core-20260715`
- Status: BUILD READY = JA, SERVER TEST READY = JA, CLIENT/GAMEPLAY VALIDATED = NEIN
- Funktionen: Mast, exakt eine Fahne, Capture/Radius, White/Green/Red Smoke, 15 konfigurierbare Vanilla-Infizierte, eine JSON-RewardCrate, Vanilla Notify/Chat-Fallback, genau ein Vanilla-Marker-System, genau eine neutrale Progressbar, zentraler Cleanup.
- Nicht enthalten: Expansion-Pflicht, CF-Pflicht, Wellen, Boss, Spezial-Infected, Terminal, KOTHGate, Keycard, Premium.

## Verbindliche Profilpfade

- Config: `$profile:DeutschZ-System/DeutschZ_KotHZ/KotHZFreeSettings.json`
- Log: `$profile:DeutschZ-System/LogZ/DeutschZ_KotHZ/KotHZ.log`
- Hoster: `/gameserver/profiles/DeutschZ-System/DeutschZ_KotHZ/` und `/gameserver/profiles/DeutschZ-System/LogZ/DeutschZ_KotHZ/`
- Altpfad wird einmalig gelesen/migriert und niemals still geloescht.

## Build und Test

- Preset: `deutschz_kothz_free_compile`
- aktive Mod: nur `P:\Mods\@DeutschZ_KotHZ_Free`
- Preflight: PASS, 0 Fehler, 1 bekannte Windows-Grossbuchstabenwarnung
- Clean Build / PBO / DeutschZ-Signatur: PASS
- Script-Compile / Missionstart / `Player connect enabled`: PASS
- C1, C2, C3, C4 und C5 jeweils mindestens zwei Minuten stabil: PASS
- modbezogene Script-/RPT-Fehler: 0
- Sichtbarkeit, MapWidget, Popup, Progressbar, Capture, Reward und Cleanup im Gameplay: PATRICK_REQUIRED

## Ausgabe

- letzter Ausgabeordner: `E:\DeutschZ\DeutschZServer\@DeutschZ_KotHZ_Free`
- Synchronisierung: 16.07.2026, atomar ueber externes Staging
- enthalten: PBO, BISIGN, oeffentlicher BIKEY, `mod.cpp`, `meta.cpp`, Settings und Phase-A-bis-C5-Testdokumentation
- alte aktive PBO/BISIGN ersetzt: JA
- gemischte Altversionen: NEIN

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
