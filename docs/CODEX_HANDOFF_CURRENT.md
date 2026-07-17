# CODEX HANDOFF CURRENT

Synchronisiert: 17.07.2026 05:36 CEST

## MenuZ V2

- Produkt: `DeutschZ_MenuMusic_V2_Test`
- Source: `E:\DeutschZ\DeutschZ-ModZ\src\DeutschZ_MenuMusic_V2_Test`
- Ausgabe: `E:\DeutschZ\DeutschZServer\@DeutschZ_MenuMusic_V2_Test`
- V1 `E:\DeutschZ\DeutschZServer\@DeutschZ_MenuMusic`: unveraendert, 282/282 Dateien und 0 SHA-256-Abweichungen.
- Zielreferenz: `E:\DeutschZ\DeutschZ-ModZ\docs\MenuZ_V2\menuz_v2_target_reference.png`
- 1.612 Production-Kit-Dateien inventarisiert; 138 benoetigte Assets ausgewaehlt und 138/138 als PAA konvertiert.
- 11 OGG-Tracks, Zufallsstart, Wiederholungsschutz und Lautstaerkegrenze 0.66 aktiv.
- Config: `$profile:DeutschZ-System/DeutschZ_MenuZ/MenuZSettings.json`
- Log: `$profile:DeutschZ-System/LogZ/DeutschZ_MenuZ/MenuZ.log`
- Clean Build: PASS, 423 PBO-Dateien, 141.959.025 Byte.
- Signatur: PASS mit `DeutschZ.bikey`.
- DayZ Labs `deutschz_menuz_v2_compile`: Missionstart und mehr als 2 Minuten stabil, PASS.
- DayZ Labs `deutschz_menuz_v2_with_uiadmin`: Missionstart und mehr als 2 Minuten stabil, PASS.
- Client-/Sichttest: OFFEN.

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
