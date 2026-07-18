# CODEX HANDOFF CURRENT

Synchronisiert: 18.07.2026 11:19 CEST

## MenuZ V2

- Produkt: `DeutschZ_MenuMusic_V2_Test`
- Source: `E:\DeutschZ\DeutschZ-ModZ\src\DeutschZ_MenuMusic_V2_Test`
- Ausgabe: `E:\DeutschZ\DeutschZServer\@DeutschZ_MenuMusic_V2_Test`
- V1 `E:\DeutschZ\DeutschZServer\@DeutschZ_MenuMusic`: unveraendert, 282/282 Dateien und 0 SHA-256-Abweichungen.
- Zielreferenz: `E:\DeutschZ\DeutschZ-ModZ\docs\MenuZ_V2\menuz_v2_target_reference.png`
- 1.612 Production-Kit-Dateien inventarisiert; 138 benoetigte Assets ausgewaehlt und 138/138 als PAA konvertiert.
- Layout auf die Zielreferenz zurueckgefuehrt: kompaktes linkes Panel, freie Spielerfigur, rechte News/Statistik/Musik-Spalte, Eventkarten unten Mitte und durchgehende Navigation mit rotem DeutschZ-Z.
- Aktive Eventkarten: ConvoyZ, KotHZ, AirdropZ und HeliCrashZ. GroundZero/Operation sind nicht mehr aktiv referenziert.
- Musiksteuerung: Previous, Pause/Resume, Next und Mute sind echte Buttons; Mute wird beim Verlassen des Menues sicher zurueckgesetzt.
- 11 OGG-Tracks, Zufallsstart, Wiederholungsschutz und Lautstaerkegrenze 0.66 aktiv.
- Config: `$profile:DeutschZ-System/DeutschZ_MenuZ/MenuZSettings.json`
- Log: `$profile:DeutschZ-System/LogZ/DeutschZ_MenuZ/MenuZ.log`
- Preflight: PASS, 0 Fehler, 1 bekannte Windows-Gross-/Kleinschreibwarnung.
- Clean Build: PASS, 423 PBO-Dateien, 141.865.272 Byte, SHA-256 `414562A1E5A41A16E0D8D6F5E009DD973A38EFDF053056B61CD76302B0C6D5F6`.
- Signatur: PASS mit `DeutschZ.bikey`.
- BISIGN SHA-256: `2DC91BA6F2C2EB82EFF9A2EBE131048236C96F7CB75B9629E827AD7626A4B1AF`; `DSCheckSignatures`: PASS.
- DayZ Labs `deutschz_menuz_v2_compile`: Script-Compile, Missionstart und mehr als 2 Minuten stabil, PASS am 18.07.2026.
- Frische Script-/RPT-Pruefung: 0 relevante MenuZ-Fehler.
- DayZ Labs `deutschz_menuz_v2_with_uiadmin`: Missionstart und mehr als 2 Minuten stabil, PASS.
- Eigenstaendiges Testpaket atomar nach `E:\DeutschZ\DeutschZServer\@DeutschZ_MenuMusic_V2_Test` synchronisiert; 1/1 Menu-PBO, Quell-/Zielhash identisch.
- `@DeutschZ_only_core` enthaelt genau eine Menu-PBO: `DeutschZ_MenuMusic_V2_Test.pbo`; diese wurde hashgleich ersetzt. `mod.cpp`, `meta.cpp` und alle anderen Core-Dateien blieben unveraendert.
- Vorheriger Core-Stand: `E:\DeutschZ\_CodexBackups\@DeutschZ_only_core_before_MenuZV2_20260718_111933`.
- Client-/Sichttest des neuen Layouts: OFFEN und durch Patrick erforderlich.

## UiAdminZ Admin Lite + Pro Shell

- Produkt: `DeutschZ_UiAdminZ`
- Source: `E:\DeutschZ\DeutschZ-ModZ\src\DeutschZ_UiAdminZ`
- Ausgabe: `E:\DeutschZ\DeutschZServer\@DeutschZ_UiAdminZ`
- Assetquelle unveraendert: `C:\Users\patsl\Downloads\DeutschZ_EventAdmin_UI_Addon`
- 15 Panel-PNGs und 15 Button-State-PNGs proportional auf transparente POT-Canvas vorbereitet und 30/30 als PAA konvertiert.
- 60 bereits vorhandene Icon-PAAs selektiv uebernommen. Finale PBO enthaelt 102 Dateien und keine generierten PNG-Zwischenstufen.
- A-F-Architektur umgesetzt: Header, Sidebar, Dashboard/Eventliste, Free/Pro/Lizenz, Eventeditor, Live-Karte/Logs/Pro.
- F7-Authentifizierung ist von KotHZ getrennt. UiAdminZ laeuft ohne Eventmod und zeigt dann `EVENTMOD NICHT GELADEN`.
- Eigene RPC-ID `24873`, Protokoll v2, Steam64-Validierung, Identity-Abgleich, Rate-Limit und Auditlog serverseitig.
- Defaultadmin: `76561199819501556`; Adminliste wird nicht an Clients uebertragen.
- Eventaktionen benutzen weiterhin Adapter-RPC `24872` und werden vom Eventmod erneut serverseitig validiert.
- Input-Sperre ueber DayZ-Menue-Excludes; F7 und ESC schliessen und geben Input wieder frei.
- Gueltige Settings werden nicht ueberschrieben. Fehlende Settings werden einmalig erzeugt; defekte Settings vor Regeneration gesichert.
- Config: `$profile:DeutschZ-System/DeutschZ_UiAdminZ/UiAdminZSettings.json`
- Log: `$profile:DeutschZ-System/LogZ/DeutschZ_UiAdminZ/UiAdminZ.log`
- Preflight: PASS, 0 Fehler, 1 bekannte Windows-Gross-/Kleinschreibwarnung.
- Clean Build: PASS, PBO 2.394.016 Byte.
- PBO-Inhaltspruefung: PASS, Layout/Scripts/90 ausgewaehlte PAAs vorhanden.
- Signatur: PASS mit `DeutschZ.bikey`.
- DayZ Labs `deutschz_uiadmin_only`: Missionstart, Script-Compile und mehr als 2 Minuten stabil, PASS.
- DayZ Labs `deutschz_kothz_free_with_uiadmin`: Missionstart, Script-Compile, Eventstart bis ACTIVE und mehr als 2 Minuten stabil, PASS.
- Frische Scriptlogs: keine Compile-, Exception- oder Null-Pointer-Fehler der Produkte.
- Sichtbarer F7-/ESC-/Input-/Layouttest: OFFEN und durch Patrick erforderlich.

## Sicherheitsstatus

- ADMIN LITE SERVERAUTORITATIV: JA
- PRO SHELL TECHNISCH VORBEREITET: JA
- PRO SECURITY FINAL: NEIN
- LICENSECORE BLOCKED: JA
- PRO STANDARDMAESSIG GESPERRT: JA
- `ProDevelopmentEnabled` ist kein Lizenznachweis und erteilt keine Serverrechte.

## Uebertragene Dateien

`E:\DeutschZ\DeutschZServer\@DeutschZ_MenuMusic_V2_Test`:

- `Addons/DeutschZ_MenuMusic_V2_Test.pbo`
- `Addons/DeutschZ_MenuMusic_V2_Test.pbo.DeutschZ.bisign`
- `keys/DeutschZ.bikey`
- `mod.cpp`
- `meta.cpp`
- `Settings/MenuZSettings.json`
- `TESTANLEITUNG.md`
- `preflight-report.txt`

`E:\DeutschZ\DeutschZServer\@DeutschZ_only_core` (selektiv aktualisiert):

- `Addons/DeutschZ_MenuMusic_V2_Test.pbo`
- `Addons/DeutschZ_MenuMusic_V2_Test.pbo.DeutschZ.bisign`
- `keys/DeutschZ.bikey`

`E:\DeutschZ\DeutschZServer\@DeutschZ_UiAdminZ`:

- `Addons/DeutschZ_UiAdminZ.pbo`
- `Addons/DeutschZ_UiAdminZ.pbo.DeutschZ.bisign`
- `keys/DeutschZ.bikey`
- `mod.cpp`
- `meta.cpp`
- `Settings/UiAdminZSettings.json`
- `TESTANLEITUNG.md`
- `preflight-report.txt`

## Installation und Test

1. MenuZ V1 deaktivieren; V1 und V2 nie gleichzeitig laden.
2. `@DeutschZ_MenuMusic_V2_Test` auf Client und Server laden und `docs/MenuZ_V2/MENUZ_V2_TESTPLAN.md` abarbeiten.
3. `@DeutschZ_UiAdminZ` auf Client und Server laden; optional `@DeutschZ_KotHZ_Free` fuer den Adaptertest.
4. `docs/UiAdminZ/UIADMINZ_PRO_TESTPLAN.md` abarbeiten.
5. Erst nach Patricks Screenshots und Inputtest eine visuelle Freigabe erteilen.

## Abschlussstatus

- MENUZ V2 BUILD READY: JA
- MENUZ V2 SERVER TEST READY: JA
- MENUZ V2 VISUAL READY: NEIN
- UIADMINZ BUILD READY: JA
- UIADMINZ SERVER TEST READY: JA
- UIADMINZ VISUAL READY: NEIN
- AUSGABE AKTUELL: JA
- CLIENTTEST ERFORDERLICH: JA

## Git und Codex

- Branch: `codex/menuz-v2-paa-pipeline-20260717`
- Implementierungscommit: `8703979`
- Push: `origin/codex/menuz-v2-paa-pipeline-20260717` erfolgreich.
- Nicht zum Auftrag gehoerende lokale Aenderungen blieben ungestaged und unangetastet.
- Installierte Codex-Version: `26.707.12708.0`.
- Microsoft-Store-/winget-Updatepruefung: kein neueres Upgrade verfuegbar; installierter Stand ist aktuell laut Storequelle.
