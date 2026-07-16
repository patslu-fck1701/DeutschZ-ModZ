# CODEX HANDOFF CURRENT

Letzte Aktualisierung: 16.07.2026, 06:16 Uhr

## Nachtauftrag - Phasenstatus

- Phase 1 KotHZ Free Phase C: PASS
- Phase 2 DeutschZ UiAdminZ / Admin Lite: PASS
- Phase 3 DeutschZ LicenseCore: BLOCKIERT - keine sichere asymmetrische Signatur-API in Enforce Script
- Phasen 4 bis 7: gemaess Hard Gate nicht begonnen

## LicenseCore Gate 3

- Status: `BLOCKED`
- Es wurde kein LicenseCore-Quellbereich und keine Fake-Lizenzpruefung angelegt.
- Lokale Referenzpruefung: `P:\scripts\1_Core`, `3_Game`, `4_World`, `5_Mission` sowie installierte DayZ-Tools.
- Keine RSA-, ECDSA-, Ed25519-, Public-Key- oder VerifySignature-API fuer Enforce Script gefunden.
- Einziger relevanter Treffer: `P:\scripts\1_Core\DayZ\proto\EnString.c` mit `proto native int Hash();`; dieser Integer-Hash ist keine Kryptografie und keine Signatur.
- Bewusst ausgeschlossen: Hash/Checksumme, PBO-HMAC-Geheimnis, selbst erfundene Kryptografie und reine UI-Sperren.
- Empfohlener naechster Architekturpruefpunkt: separater lokaler Vorstart-Verifier mit etablierter Kryptobibliothek; noch nicht freigegeben oder implementiert.
- Vollstaendige Evidenz und Optionen: `docs/LICENSECORE_GATE_3_BLOCKER.md`.
- Wegen des verbindlichen Nachtauftrag-Gates wurden KotHZ Premium, MenuZ V2, HUDZ und RadioMissionZ nicht begonnen.

## KotHZ Free

- Source: `E:\DeutschZ\DeutschZ-ModZ\src\DeutschZ_KotHZ_Free`
- Ausgabe: `E:\DeutschZ\DeutschZServer\@DeutschZ_KotHZ_Free`
- Status: BUILD READY = JA, SERVER TEST READY = JA, CLIENT/GAMEPLAY VALIDATED = NEIN
- Funktionen: Mast, exakt eine Fahne, Capture/Radius, White/Green/Red Smoke, 15 konfigurierbare Vanilla-Infizierte, eine JSON-RewardCrate, Vanilla Notify/Chat-Fallback, genau ein Vanilla-Marker-System, genau eine neutrale Progressbar, zentraler Cleanup.
- Nicht enthalten: Expansion-Pflicht, CF-Pflicht, Wellen, Boss, Spezial-Infected, Terminal, KOTHGate, Keycard, Premium.
- Optionaler Admin-RPC-Endpunkt vorhanden; keine Abhaengigkeit auf UiAdminZ.

Profilpfade:

- Config: `$profile:DeutschZ-System/DeutschZ_KotHZ/KotHZSettings.json`
- Log: `$profile:DeutschZ-System/LogZ/DeutschZ_KotHZ/KotHZ.log`
- Altdateien werden erkannt/gemeldet und nicht still geloescht.

## DeutschZ UiAdminZ

- Source: `E:\DeutschZ\DeutschZ-ModZ\src\DeutschZ_UiAdminZ`
- Ausgabe: `E:\DeutschZ\DeutschZServer\@DeutschZ_UiAdminZ`
- Status: BUILD READY = JA, SERVER TEST READY = JA, CLIENT/UI VALIDATED = NEIN
- Oeffnen: F7
- UI: Header, linke Navigation, KotHZ-Dashboard, FREE/PRO-Spalte, Eventdetails und gesperrte Premiumbereiche gemaess geliefertem 1920x1080-Blueprint.
- Aktive Admin-Lite-Daten: Status, Position, Radius, Capture, Gegner, Reward, Diagnose.
- Aktive Aktionen: kontrollierter Start/Stop und Statusrefresh.
- Eventlogik bleibt in KotHZ Free; UiAdminZ bleibt optionale Komfortschicht.

Profilpfade:

- Config: `$profile:DeutschZ-System/DeutschZ_UiAdminZ/UiAdminZSettings.json`
- Log: `$profile:DeutschZ-System/LogZ/DeutschZ_UiAdminZ/UiAdminZ.log`

Sicherheit:

- `AdminSteamIds` in KotHZSettings ist standardmaessig leer.
- Nicht konfigurierte Spieler duerfen Status lesen, aber Start/Stop wird serverseitig verweigert.
- Steam64-ID, Protokoll, Aktion, Rate-Limit und Controllerzustand werden serverseitig geprueft.
- Start/Stop wird im KotHZ-Mod entschieden und in `KotHZ.log` auditiert.
- Kein LicenseCore-, CF- oder Expansion-Zwang.

## Gate-2-Testbelege

- Preset A `deutschz_kothz_free_only`: KotHZ Free ohne UiAdminZ PASS.
- Preset B `deutschz_kothz_free_with_uiadmin`: finaler Lauf 05:50 bis 05:54 Uhr, mehr als zwei Minuten stabil, PASS.
- Preset C `deutschz_uiadmin_only`: UiAdminZ ohne KotHZ, Compile und Missionstart PASS.
- Game-, World- und Mission-Scriptmodule: PASS.
- modbezogene Compile-, Nullpointer- und Runtimefehler: 0.
- Free+UI Scriptlog: `script_2026-07-16_05-50-13.log`.
- Free+UI RPT: `DayZServer_x64_2026-07-16_05-50-05.RPT`.
- UI-only Scriptlog: `script_2026-07-16_05-42-37.log`.
- PBO-Inhalt mit BankRev geprueft: PASS.
- Beide PBO-Signaturen mit DSCheckSignatures geprueft: PASS.
- Kontrollierter Serverstopp: PASS.
- F7, sichtbares Layout, RPC-Klicks und Gameplay: PATRICK_REQUIRED.

## Ausgabe und Synchronisierung

Synchronisierung: 16.07.2026, 05:55 Uhr, atomar ueber `E:\DeutschZ\_codex_phase2_staging`; Staging danach entfernt. Alte aktive PBO/BISIGN wurden ersetzt, keine gemischten Altversionen und keine Backupordner im Ausgabeordner.

Phase 3 erzeugte wegen des Sicherheitsblockers keinen Build und keine neue Produktausgabe. Die unten dokumentierten, geprueften Phase-1/2-Ausgaben bleiben der aktuelle Teststand.

Uebertragen nach `E:\DeutschZ\DeutschZServer\@DeutschZ_UiAdminZ`:

- `Addons/DeutschZ_UiAdminZ.pbo`
- `Addons/DeutschZ_UiAdminZ.pbo.DeutschZ.bisign`
- `keys/DeutschZ.bikey`
- `mod.cpp`
- `meta.cpp`
- `Settings/UiAdminZSettings.json`
- `docs/NIGHT_PHASE_2_GATE.md`
- `docs/TESTANLEITUNG.md`

Aktualisiert in `E:\DeutschZ\DeutschZServer\@DeutschZ_KotHZ_Free`:

- `Addons/DeutschZ_KotHZ_Free.pbo`
- `Addons/DeutschZ_KotHZ_Free.pbo.DeutschZ.bisign`
- `keys/DeutschZ.bikey`
- `mod.cpp`
- `meta.cpp`
- `Settings/KotHZSettings.json`
- alle vorhandenen Phase-A-bis-C- und Testdokumente

## Assets und PAA-Regel

- Admin-Assetquelle: `C:\Users\patsl\Downloads\DeutschZ_EventAdmin_UI_Addon`; Originale unveraendert.
- Nur vier benoetigte PAA-Icons uebernommen und im Layout referenziert: dashboard, start, stop, refresh.
- MenuZ-V2-Quelle: `C:\Users\patsl\Downloads\DeutschZ_UI_Production_Kit`; erst in Phase 5 verwenden.
- Neue Loading-Screen-Kandidaten bleiben bis Phase 5 unveraendert: `DeutschZ_LoadingScreenZ.png` und `DeutschZ_LoadingScreenZ2.png`.
- Jede spaeter tatsaechlich verwendete PNG/TGA-Textur muss einzeln ueber das installierte DayZ-Tools-`ImageToPAA.exe` in PAA erzeugt und danach im PBO geprueft werden. Keine blinde Komplettkonvertierung.

## Installation und Clienttest

1. `@DeutschZ_KotHZ_Free` allein laden und den bisherigen Eventtest wiederholen.
2. Danach `@DeutschZ_UiAdminZ` zusaetzlich laden; gleichen `DeutschZ.bikey` serverseitig bereitstellen.
3. F7 in 1920x1080 und 2560x1440 pruefen; 1280x720 auf Ueberlauf pruefen.
4. Ohne Eintrag in `AdminSteamIds` muss Start/Stop verweigert sein.
5. Eigene Steam64-ID in `KotHZSettings.json` eintragen, Server neu starten und Start/Stop pruefen.
6. `KotHZ.log` auf AdminRPC, Steam64-ID, Aktion und Ergebnis pruefen.
7. UiAdminZ allein laden: F7 darf keine Nullfehler verursachen und soll `KEINE ANTWORT` anzeigen.

## Bekannte Restpunkte

- Visueller Clienttest fehlt.
- Admin- und Nicht-Admin-Klicktest fehlt.
- LicenseCore benoetigt eine neue, ausdruecklich freigegebene Architektur ausserhalb einer rein in Enforce Script erfundenen Signaturpruefung.
- Windows-DayZ-Server akzeptiert die bekannten Grossbuchstabenpfade; sie sind fuer diesen Auftrag kein Releaseblocker.
