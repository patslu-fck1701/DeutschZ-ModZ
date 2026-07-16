# CODEX HANDOFF CURRENT

Synchronisiert: 16.07.2026, 22:19 Uhr

## Aktueller Teststand

- KotHZ Free: Build/Signatur PASS, gemeinsamer DayZ-Labs-Serverlauf mit UiAdminZ ueber zwei Minuten PASS, Script-Compile und Missionstart PASS.
- UiAdminZ: Build/Signatur PASS, Server-RPCs laden fehlerfrei. Sichtbarer F7-/Escape-/Input-Test bleibt Clienttest.
- MenuMusic: Build/Signatur PASS. Elf Titel aus `Restart_Menue_Tracks`, zufaellige Auswahl mit Wiederholungsschutz, maximale Lautstaerke 66 Prozent. Sicht- und Hoertest bleibt Clienttest.
- KotHZ-Musik: acht Titel, zufaellige Auswahl ohne direkte Wiederholung, maximale Lautstaerke 33 Prozent, Wiedergabe nur fuer Spieler im Eventradius.
- KotHZ Reward: genau eine RewardChest mit aktueller Flaggen-/Kisten-PAA; M4A1, ein volles STANAG-Magazin und 5.56-Munition werden serverseitig garantiert, weitere `RewardItems` bleiben JSON-konfigurierbar.
- Erweiterte Settings-Klassen wurden nicht beibehalten: Sie verursachten reproduzierbar einen nativen DayZ-1.29-Compilerabsturz. Das stabile Settings-Schema und die serverseitige Musik-/Reward-Implementierung sind aktiv.

## Ausgabeordner

- `E:\DeutschZ\DeutschZServer\@DeutschZ_KotHZ_Free`
- `E:\DeutschZ\DeutschZServer\@DeutschZ_UiAdminZ`
- `E:\DeutschZ\DeutschZServer\@DeutschZ_MenuMusic`

## Profilpfade

- KotHZ Config: `$profile:DeutschZ-System/DeutschZ_KotHZ/KotHZSettings.json`
- KotHZ Log: `$profile:DeutschZ-System/LogZ/DeutschZ_KotHZ/KotHZ.log`
- UiAdminZ Config: `$profile:DeutschZ-System/DeutschZ_UiAdminZ/UiAdminZSettings.json`
- UiAdminZ Log: `$profile:DeutschZ-System/LogZ/DeutschZ_UiAdminZ/UiAdminZ.log`

## Uebertragene Dateien

- je Produkt: `Addons/<Produkt>.pbo`, passende `.pbo.DeutschZ.bisign`, `keys/DeutschZ.bikey`, `mod.cpp`, `meta.cpp`
- KotHZ zusaetzlich: `Settings/KotHZSettings.json`, `TESTANLEITUNG.md`
- UiAdminZ zusaetzlich: `TESTANLEITUNG.md`
- MenuMusic zusaetzlich: `TESTANLEITUNG.md`

## Test

Preset: `deutschz_kothz_free_compile`. Frische Logs: `script_2026-07-16_22-15-39.log` und `DayZServer_x64_2026-07-16_22-15-29.RPT`. Keine Script-/Compilefehler. RPT enthaelt nur bekannte Vanilla-Weltassetwarnungen. Fuer Gameplay noch clientseitig pruefen: F7-Layout, Escape/Input-Rueckgabe, Marker/Notify, Gruppen-Capture, Radius-Musik, Flagge/RewardChest und zufaellige MenuMusic-Wiedergabe.
