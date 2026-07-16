# CODEX HANDOFF CURRENT

Letzte Aktualisierung: 16.07.2026, 16:27 Uhr

## Clientfix 2 - aktueller Stand

- `DeutschZ_KotHZ_Free` Version `1.0.0-clientfix-2`: neue RewardChest-PAA eingebunden; optionaler Expansion-3D-Servermarker mit Icon `Skull 3`; optionale Expansion-Notification; Vanilla-/RPC-Fallback ohne Expansion bleibt aktiv.
- Expansion-Gruppen werden serverautoritativ als eine Capture-Seite behandelt. Mehrere Mitglieder derselben Party blockieren den Fortschritt nicht; fremde Seiten bleiben umkaempft.
- `DeutschZ_UiAdminZ`: kompaktes fehlerhaftes Layout durch vollstaendig ausgeschriebenes Vanilla-Window-Layout ersetzt; Pflichtwidgets sind sichtbar angelegt und werden einzeln diagnostiziert. F7 und Escape schliessen weiterhin.
- Preflight: beide Mods 0 Fehler. PBO-Inhalt und beide DeutschZ-Signaturen geprueft.
- DayZ-Labs ohne Expansion: Script-Compile, Missionstart, `IDLE -> ANNOUNCED -> ACTIVE`, 15 Infizierte und stabiler Lauf bestanden.
- DayZ-Labs mit CF, Dabs Framework, Expansion Bundle und Expansion Licensed: Script-Compile, Missionstart und `IDLE -> ANNOUNCED -> ACTIVE` bestanden; keine KotHZ-/UiAdminZ-Compilefehler.
- Sichtbarer Admin-Layout-, Escape-, 3D-Marker-, Notify-, Gruppen- und RewardChest-Test bleibt Patrick vorbehalten.
- Synchronisierung 16.07.2026, 16:25 Uhr atomar aus geprueftem Staging. Aktive Ausgabeordner wurden gespiegelt; keine Alt-PBOs oder Alt-BISIGNs gemischt.

## Clientfix KotHZ Free + UiAdminZ

- UiAdminZ-Ursache: Das Root-Layout wurde geladen, die Widget-Erzeugung endete jedoch vor `DZUIAZ_Status`; `Update()` erzeugte danach pro Frame einen Nullpointer. Die vier gelieferten Icon-PAAs bleiben unveraendert im PBO, ihre fehlerhafte aktive `ImageWidget`-Bindung wurde fuer den erneuten Clienttest entfernt. Es wurden keine Ersatzassets erzeugt.
- UiAdminZ-Fix: Pflichtwidgets werden einmalig validiert und nullsicher verwendet. `UIScriptedMenu.OnShow()` plus Vanilla-Input-Exclude `menu` blockieren Spielsteuerung und zeigen den Cursor. `OnHide()`, `Cleanup()` und `OnMissionFinish()` geben den Input sicher zurueck. F7 und Escape schliessen.
- KotHZ-Progress-Fix: `ACTIVE`-Updates werden serverseitig mit derselben Radiuspruefung wie Capture gefiltert. Nur Spieler im Radius erhalten Show/Update; beim Verlassen folgt ein Hide-RPC. Globales Show waehrend der Ankuendigung ist optional und standardmaessig aus.
- KotHZ-Marker-Fix: Vanilla-Pfad auf `\\dz\\gear\\navigation\\data\\map_tree_ca.paa` korrigiert; genau ein Renderer ueber `MapMenu.LoadMapMarkers()`.
- Neue Free-Assets: `kothz_free_flag_dayz_co.paa` fuer Selection `camo` und `kothz_free_sea_chest_co.paa` fuer `camoGround`; `DZ_Gear_Camping` bleibt requiredAddon.
- Clientvalidierung bleibt erforderlich; serverseitiger Compile-/Missiontest ersetzt keinen sichtbaren F7-, Marker-, Radius- oder Texturtest.

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
- Status: BUILD READY = JA, SERVER TEST READY = JA, CLIENT/GAMEPLAY VALIDATED = NEIN; CLIENTFIX TESTBEREIT
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
- Status: BUILD READY = JA, SERVER TEST READY = JA, CLIENT/UI VALIDATED = NEIN; CLIENTFIX TESTBEREIT
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

## Aktuelle Clientfix-Testbelege

- Preset `deutschz_uiadmin_only`: Game-, World- und Missionmodule kompiliert, Missionstart PASS, keine modbezogenen Scriptfehler. Scriptlog `script_2026-07-16_14-21-10.log`.
- Preset `deutschz_kothz_free_only`: Game-, World- und Missionmodule kompiliert, Missionstart PASS, keine modbezogenen Scriptfehler. Scriptlog `script_2026-07-16_14-24-40.log`.
- Preset `deutschz_kothz_free_with_uiadmin`: finaler Lauf ab 14:40 Uhr ueber zwei Minuten stabil, Missionstart PASS, Mast=1, Fahne=1, 15 Infizierte, keine modbezogenen Compile-, Nullpointer- oder Runtimefehler.
- Nach Ergaenzung des expliziten Escape-Handlings wurde UiAdminZ erneut sauber gebaut und mit beiden Mods bis Missionstart kompiliert. Scriptlog `script_2026-07-16_15-03-27.log`, RPT `DayZServer_x64_2026-07-16_15-03-19.RPT`; keine modbezogenen Fehler.
- Finales Scriptlog: `C:\Users\patsl\DayZProjects\servers\kothz_free_compile\profiles\script_2026-07-16_14-40-23.log`.
- Finale RPT: `C:\Users\patsl\DayZProjects\servers\kothz_free_compile\profiles\DayZServer_x64_2026-07-16_14-40-12.RPT`.
- Preflight: beide Mods 0 Fehler; bekannte Grossbuchstabenwarnung ist auf dem aktuellen Windows-Server kein Blocker.
- PBO-Inhalt mit BankRev geprueft; beide neuen KotHZ-PAAs enthalten.
- Signaturen mit DSCheckSignatures geprueft: PASS.
- Kontrollierter Serverstopp: PASS.

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

Synchronisierung: KotHZ Free 16.07.2026, 14:45 Uhr atomar ueber `E:\DeutschZ\.codex-staging-clientfix-20260716-1445`; finaler UiAdminZ-Build 15:05 Uhr atomar ueber `E:\DeutschZ\.codex-staging-uiadmin-clientfix-20260716-1505`. Beide Staging-Verzeichnisse wurden entfernt. Alte aktive PBO/BISIGN wurden ersetzt, keine gemischten Altversionen und keine Backupordner im Ausgabeordner.

Phase 3 erzeugte wegen des Sicherheitsblockers keinen Build und keine neue Produktausgabe. Die unten dokumentierten, geprueften Phase-1/2-Ausgaben bleiben der aktuelle Teststand.

Uebertragen nach `E:\DeutschZ\DeutschZServer\@DeutschZ_UiAdminZ`:

- `Addons/DeutschZ_UiAdminZ.pbo`
- `Addons/DeutschZ_UiAdminZ.pbo.DeutschZ.bisign`
- `keys/DeutschZ.bikey`
- `mod.cpp`
- `meta.cpp`
- `Settings/UiAdminZSettings.json`
- `docs/TESTANLEITUNG_CLIENTFIX.md`

Aktualisiert in `E:\DeutschZ\DeutschZServer\@DeutschZ_KotHZ_Free`:

- `Addons/DeutschZ_KotHZ_Free.pbo`
- `Addons/DeutschZ_KotHZ_Free.pbo.DeutschZ.bisign`
- `keys/DeutschZ.bikey`
- `mod.cpp`
- `meta.cpp`
- `Settings/KotHZSettings.json`
- `docs/TESTANLEITUNG_CLIENTFIX.md`

## Assets und PAA-Regel

- Admin-Assetquelle: `C:\Users\patsl\Downloads\DeutschZ_EventAdmin_UI_Addon`; Originale unveraendert.
- Vier gelieferte PAA-Icons bleiben im UiAdminZ-PBO, sind im Clientfix jedoch bis zur erneuten Sichtpruefung nicht aktiv als `ImageWidget` gebunden.
- Gelieferte KotHZ-Free-Flaggen- und Sea-Chest-PAA sind in den aktiven Vanilla-Hidden-Selections eingebunden.
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
8. Mit offenem F7-Menue pruefen: kein Schuss, kein Zielen, keine Kamera-/Inventar-/Aktionssteuerung; F7 und Escape schliessen, danach Input wieder frei.
9. KotHZ ausserhalb Radius: keine Bar; Radius betreten: Bar sichtbar; Radius verlassen: sofort verborgen.
10. Vanilla-Karte pruefen: genau ein Marker an `EventPosition`; bei Ende/Abbruch/Cleanup entfernt.
11. Flagge und RewardCrate auf die gelieferten Free-Texturen pruefen.

## Bekannte Restpunkte

- Erneuter visueller Clienttest fuer Layout, Inputsperre, Marker, Radiusbar und neue Texturen fehlt.
- Admin- und Nicht-Admin-Klicktest fehlt.
- LicenseCore benoetigt eine neue, ausdruecklich freigegebene Architektur ausserhalb einer rein in Enforce Script erfundenen Signaturpruefung.
- Windows-DayZ-Server akzeptiert die bekannten Grossbuchstabenpfade; sie sind fuer diesen Auftrag kein Releaseblocker.
