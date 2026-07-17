# CODEX HANDOFF CURRENT

## MenuZ V2 Quick Overhaul / only_core Sync - 17.07.2026 21:48 CEST

- Letzter aktualisierter Ausgabeordner: `E:\DeutschZ\DeutschZServer\@DeutschZ_MenuMusic_V2_Test`
- Zusaetzlich atomar aktualisiert: `E:\DeutschZ\DeutschZServer\@DeutschZ_only_core`
- Source/Branch: `E:\DeutschZ\DeutschZ-Worktrees\MenuV2Quick`, `codex/menuz-v2-quick-overhaul-20260717`
- MenuZ V1 blieb unangetastet.
- UI-Basis: vorherige funktionierende MenuZ-V2-Version; Layout und Styling wurden auf das neue PAA-Pack umgestellt.
- Assetquelle: `E:\DeutschZ\PAAs\DeutschZ_MainMenu_UI_Pack_v2`; nur vorhandene PAA-Dateien verwendet, keine Konvertierung.
- Innenflaechen neutral schwarz; Giftgruen nur fuer Rahmen, Hover, Aktivstatus und kleine technische Akzente.
- Leaderboard mit Rang, Name, Kills und Punkten als Platzhalter-Schnittstelle vorbereitet.
- Musiksteuerung: Previous, Play/Pause, Next und Mute eindeutig getrennt; Zustand und Position werden erhalten.
- Clean Build: PASS, 483 PBO-Dateien.
- PBO-Inhaltspruefung: PASS.
- Signatur: PASS mit `DeutschZ.bikey`.
- DayZ-Labs-Preset `deutschz_menuz_v2_compile`: Serverstart, Script-Compile, Missionstart und mehr als zwei Minuten stabil: PASS.
- Client-/Sichttest: PATRICK_REQUIRED.
- Synchronisiert: 17.07.2026 21:48 CEST.

### MenuZ-V2-Ausgabe

- PBO: `Addons/DeutschZ_MenuMusic_V2_Test.pbo`
- BISIGN: `Addons/DeutschZ_MenuMusic_V2_Test.pbo.DeutschZ.bisign`
- BIKEY: `keys/DeutschZ.bikey`
- `mod.cpp`: JA
- `meta.cpp`: JA
- Settings: `Settings/MenuZSettings.json`
- Dokumentation: `TESTANLEITUNG.md`, `MENUZ_V2_USED_PAA.md`
- PBO SHA-256: `F1AD64ACF57B6F69345C1316907BF6A0951387D9592D9E96CEC628B070E3E660`
- BIKEY SHA-256: `429C0D163341BC3A56CF8B88A28849AEBE949026B0D4A24D79056736DCD3328D`

### only_core-Synchronisierung

`@DeutschZ_only_core` wurde nicht bereinigt oder neu zusammengesetzt. Der komplette vorhandene Bestand wurde bytegleich erhalten; ausschliesslich MenuZ V2, dessen Signatur, der DeutschZ-Key sowie `mod.cpp`, `meta.cpp` und die Testanleitung wurden eingesetzt beziehungsweise aktualisiert. Vollstaendiger aktiver Dateibestand:

```text
addons/Auto_Transfer_Items_By_Category.pbo
addons/Auto_Transfer_Items_By_Category.pbo.Enclave.bisign
addons/Axiao_VPPCheck_Items.pbo
addons/Axiao_VPPCheck_Items.pbo.Axiao.bisign
addons/COT_Override.pbo
addons/COT_Override.pbo.DevCarlosKey.bisign
addons/DeutschZ_KotHZ_Free.pbo
addons/DeutschZ_KotHZ_Free.pbo.DeutschZ.bisign
addons/DeutschZ_MenuMusic_V2_Test.pbo
addons/DeutschZ_MenuMusic_V2_Test.pbo.DeutschZ.bisign
addons/DeutschZ_UiAdminZ.pbo
addons/DeutschZ_UiAdminZ.pbo.DeutschZ.bisign
addons/HB_FlipVehicle.pbo
addons/HB_FlipVehicle.pbo.TestZ.bisign
addons/InventoryInCar.pbo
addons/InventoryInCar.pbo.TestZ.bisign
addons/JGC_G63W463.pbo
addons/JGC_G63W463.pbo.TestZ.bisign
addons/MB_Batteries.pbo
addons/MB_Batteries.pbo.TestZ.bisign
addons/MBM_BMWAlpina.pbo
addons/MBM_BMWAlpina.pbo.TestZ.bisign
addons/MBM_CanAM_Maverick.pbo
addons/MBM_CanAM_Maverick.pbo.TestZ.bisign
addons/MBM_DodgeRam.pbo
addons/MBM_DodgeRam.pbo.TestZ.bisign
addons/MBM_DuneBuggy.pbo
addons/MBM_DuneBuggy.pbo.TestZ.bisign
addons/MBM_JLTV.pbo
addons/MBM_JLTV.pbo.TestZ.bisign
addons/MBM_MonteCarloSS.pbo
addons/MBM_MonteCarloSS.pbo.TestZ.bisign
addons/MBM_RamRebel.pbo
addons/MBM_RamRebel.pbo.TestZ.bisign
addons/ZenCOT.pbo
addons/ZenCOT.pbo.Zenarchist.bisign
extra/stringtable.csv
extra/types.xml
keys/5Dimencao.bikey
keys/Axiao.bikey
keys/CZCookup.bikey
keys/DaMadesTx_Public_Key.bikey
keys/DeutschZ.bikey
keys/Enclave.bikey
keys/Indra.bikey
keys/MBMv2.bikey
keys/MBMv3.bikey
keys/mebrad.bikey
keys/TestZ.bikey
keys/Zenarchist.bikey
meta.cpp
mod.cpp
TESTANLEITUNG.md
```

### Installation und Sichttest

1. Fuer den Einzeltest `E:\DeutschZ\DeutschZServer\@DeutschZ_MenuMusic_V2_Test` laden.
2. Fuer den gebuendelten Serverstand nur `@DeutschZ_only_core` laden und MenuZ V2 nicht zusaetzlich separat eintragen.
3. Hauptmenue bei 1920x1080 oeffnen und schwarze Panelinnenflaechen, Hover-/Aktivzustand, Navigation, Eventkarten, Leaderboard und Musikbuttons pruefen.
4. Bei einem sichtbaren Fehler Screenshot sowie frische Client-RPT-/Scriptlogs liefern.

## RUSForma / only_core / KotHZ Finalfix - 17.07.2026 18:57 CEST

- Letzter aktualisierter Ausgabeordner: `E:\DeutschZ\DeutschZServer\@DeutschZ_only_core`
- Zusaetzlich aktualisiert: `E:\DeutschZ\DeutschZServer\@DeutschZ_KotHZ_Free`, Server-Settings unter `E:\DeutschZ\DeutschZServer\mpmissions` und `E:\DeutschZ\DeutschZServer\profiles`
- Lokaler Spiegel: `C:\Program Files (x86)\Steam\steamapps\common\DayZServer`
- Live-Settings-Sync: 555 Dateien aus dem zentralen Ausgabeordner lokal uebernommen; `storage_1`, `Logs`/`LogZ` und Laufzeitlogs explizit ausgeschlossen; keine Mirror-/Loeschoperation.
- RUSForma bleibt separate Workshop-Mod: `@RUSForma_vehicles`, Workshop-ID `2536888090`; kein Workshop-PBO wurde veraendert oder repackt.
- RUSForma: 242 echte Fahrzeug-/Variantenklassen mit `nominal=0`; 34 nominal=1 Rad-/Tuer-/Haubenteile bewusst nicht als Fahrzeuge einsortiert.
- CE: `types_rusforma.xml`, 242 eindeutige Klassen, `nominal=0`, `min=0`, `restock=0`, `cost=100`, genau einmal in `cfgeconomycore.xml` eingebunden.
- Market: sechs Kategorien, 242 eindeutige Klassen, keine Duplikate in anderen Market-Dateien.
- Haendler: Civilian/Utility/Offroad in `Vehicles.json`; Military/Armored/Special in `Blackmarket_Vehicles.json`.
- KotHZ: neue RewardChest-PAA byteidentisch zur Downloadquelle; Gewinnrauch rot; Feuerwerk 30 Sekunden; Post-Win-Cleanup 30 Sekunden; gewichtete Rewardpools mit einer garantierten plus 1-3 zufaelligen Vanilla-Waffen und Duplikatschutz.
- KotHZ Clean Build/Preflight: PASS (0 Fehler, bekannte Windows-Pfadwarnung).
- Signatur: PASS mit `DeutschZ.bikey`.
- DayZ-Labs-Preset: `C:\Users\patsl\DayZProjects\mod-presets\deutschz_full_rusforma_only_core.json`, 22 eindeutige vorhandene Modpfade. Nicht gestartet, gemaess reduziertem Testumfang.

### Uebertragene Dateien

`E:\DeutschZ\DeutschZServer\@DeutschZ_only_core`:

- `Addons/DeutschZ_MenuMusic_V2_Test.pbo` + `.DeutschZ.bisign`
- `Addons/DeutschZ_KotHZ_Free.pbo` + `.DeutschZ.bisign`
- `Addons/DeutschZ_UiAdminZ.pbo` + `.DeutschZ.bisign`
- `keys/DeutschZ.bikey`
- `mod.cpp`
- `meta.cpp`
- `TESTANLEITUNG.md`

Settings-Overlay in Ausgabe und lokalem Server:

- `modlist.txt`
- `DeutschZ_Start_local.bat`
- `mpmissions/dayzOffline.chernarusplus/cfgeconomycore.xml`
- `mpmissions/dayzOffline.chernarusplus/dz_mod_ce/types_rusforma.xml`
- sechs `profiles/ExpansionMod/Market/DeutschZ_RUSForma_*.json`
- `profiles/ExpansionMod/Market/DeutschZ_RUSForma_Civilian.json`
- `profiles/ExpansionMod/Market/DeutschZ_RUSForma_Utility.json`
- `profiles/ExpansionMod/Market/DeutschZ_RUSForma_Offroad.json`
- `profiles/ExpansionMod/Market/DeutschZ_RUSForma_Military.json`
- `profiles/ExpansionMod/Market/DeutschZ_RUSForma_Armored.json`
- `profiles/ExpansionMod/Market/DeutschZ_RUSForma_Special.json`
- `profiles/ExpansionMod/Traders/Vehicles.json`
- `profiles/ExpansionMod/Traders/Blackmarket_Vehicles.json`
- `keys/RUS67pak.bikey`

### Hashes und Status

- KotHZ PBO: `7E73A10B836A798C2F004AF73E4330937F313C4AB61534822D561578D637557F`
- MenuMusic V2 Test PBO: `067583AEAF7DFA7B89467BDB9B462E680E624D61C9BBAB47657197C5EC43960F`
- UiAdminZ PBO: `658C5AEFC1DB28F8906C9C3EDA58003C296624426D5BC9D4CD20B2DF67609EA0`
- DeutschZ BIKEY: `429C0D163341BC3A56CF8B88A28849AEBE949026B0D4A24D79056736DCD3328D`
- RUSForma CE XML: `8A7736B1DCBF7E291C928A8449E07E0F4D3D5A3F9A2AA1C10CB69DEDD610C819`
- Build/Signatur/JSON/XML/Duplikate/Pfade: PASS
- Lokaler Gameplaytest: PATRICK_REQUIRED
- Markettest: PATRICK_REQUIRED
- Economytest: PATRICK_REQUIRED

### Installation und Test

1. Preset `deutschz_full_rusforma_only_core` laden oder die identische `modlist.txt` verwenden.
2. Keine separaten MenuMusic-/KotHZ-/UiAdmin-Modzeilen zusaetzlich laden.
3. `C:\Users\patsl\Desktop\CodexChanged\RUSFORMA_LOCAL_TESTPLAN.md`, `ONLY_CORE_TESTPLAN.md` und `KOTHZ_FINAL_RETEST.md` abarbeiten.
4. Vollstaendigen Server-/Gameplay-/Market-/Economy-Test durch Patrick ausfuehren und frische Fehlerlogs zurueckgeben.

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

## Git und Codex

- Branch: `codex/menuz-v2-paa-pipeline-20260717`
- Implementierungscommit: `8703979`
- Push: `origin/codex/menuz-v2-paa-pipeline-20260717` erfolgreich.
- Nicht zum Auftrag gehoerende lokale Aenderungen blieben ungestaged und unangetastet.
- Installierte Codex-Version: `26.707.12708.0`.
- Microsoft-Store-/winget-Updatepruefung: kein neueres Upgrade verfuegbar; installierter Stand ist aktuell laut Storequelle.
