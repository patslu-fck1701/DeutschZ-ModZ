# DeutschZ Codex Handoff

## 1. Kopf

- Datum und Uhrzeit: 2026-07-15 17:29:13 +02:00
- Auftragstitel: MenuZ 4K UI Fast-Track fertigstellen
- Auftragsversion: MenuZ 1.2.4-menu-ui-4k
- Codex-Arbeitsordner: `C:\Users\patsl\Documents\DayZ Projects`
- Repository: `E:\DeutschZ\DeutschZ-ModZ`
- Branch: `codex/menuz-ui-finish-20260715`
- Implementierungscommit: `7b2b5ea`
- Ergebnisstatus: CLIENT-TESTBEREIT

## 2. Aktiver DeutschZ-Fokus

- Abgeschlossene Phase: MenuZ UI-Finish, Clean Build, Signierung und zentrale Ausgabe.
- Nicht bearbeitet: KotHZ-, ConvoyZ- oder andere Eventlogik.
- Source of Truth: `E:\DeutschZ\DeutschZ-ModZ\src\DeutschZ_MenuMusic`
- Letzter aktualisierter Ausgabeordner: `E:\DeutschZ\DeutschZServer\@DeutschZ_MenuMusic`
- Zeitpunkt der Synchronisierung: 2026-07-15 17:25 +02:00

## 3. Auftrag

- Neues 4K-Menübild mit unveränderter transparenter Spielfiguren-Aussparung aktivieren.
- News- und linkes Beschreibungspanel lesbar überarbeiten.
- gelieferte Eventkarten für KotHZ, ConvoyZ, GroundZero und Operation verwenden.
- Loading-Screen und Deathscreen auf vorhandene Seitenverhältnis-Korrekturen prüfen.
- Clean Build mit dem regulären `DeutschZ`-Key erzeugen, signieren, prüfen und zentral ausgeben.
- Erst anschließend diese Übergabedatei aktualisieren.

## 4. Ergebniszusammenfassung

- Original-PNG unverändert in den Source übernommen; SHA-256 stimmt mit der gelieferten Datei überein.
- PNG: 3840 x 2160, Format32bppArgb, Alpha-Samplebereich 0 bis 255.
- DayZ-kompatible 4096 x 2048 PAA mit 16:9-Ausgabekompensation erzeugt.
- Hauptmenü referenziert die neue PAA; `00_background_main.paa` bleibt nur als inaktive Altdatei erhalten.
- Linkes Panel besitzt eine klare Überschrift, zweizeilige Beschreibung und kompakten Serverstatus.
- News-Panel besitzt kleinere Überschrift, Meta-Zeile, korrigierte Innenabstände und mehrzeiligen Inhalt.
- Eventkarten auf `14_card_kothz.paa`, `15_card_convoyz.paa`, `17_card_groundzero.paa` und `18_card_operationdeutschz.paa` umgestellt.
- Eventkarten auf 116 x 168 Pixel gesetzt, passend zum gelieferten Karten-Seitenverhältnis.
- Logo-Seitenverhältnis in MainMenu, Loading-Screen und Deathscreen aus dem vorherigen UI-Zwischenstand übernommen und gemeinsam gebaut.
- Modversion auf `1.2.4-menu-ui-4k` aktualisiert.

## 5. Fehlerliste

### Kritisch

- Keine bestätigten kritischen Build- oder Signaturfehler.

### Hoch

- Visueller Clienttest steht aus. Deshalb bleibt `MENUZ SALE READY = NEIN`.

### Mittel

- Der aktuelle DayZ-Labs-Preset führt MenuZ deaktiviert. Es wurde daher kein neuer Server-/Clientlauf behauptet.

### Niedrig

- Preflight meldet eine Großbuchstaben-Pfadwarnung für Windows-gepackte Assets. Diese ist laut verbindlicher Entscheidung kein Release-Blocker und wurde nicht durch Umbenennungen verschärft.

## 6. Geänderte Dateien

- `src/DeutschZ_MenuMusic/config.cpp` - Version aktualisiert.
- `src/DeutschZ_MenuMusic/mod.cpp` - Version und Übersicht aktualisiert.
- `src/DeutschZ_MenuMusic/gui/dzkothg_main_menu.layout` - Hintergrund, Beschreibung, News und Eventkarten überarbeitet.
- `src/DeutschZ_MenuMusic/gui/dzkothg_loading_overlay.layout` - Logo-Seitenverhältnis korrigiert.
- `src/DeutschZ_MenuMusic/gui/dzkothg_death_backdrop.layout` - Logo-Seitenverhältnis korrigiert.
- `src/DeutschZ_MenuMusic/scripts/5_Mission/DeutschZ_MenuMusic/modded_MainMenu.c` - Beschreibung gebunden und kompakter Serverstatus gesetzt.
- `src/DeutschZ_MenuMusic/gui/menu_assets/background/DeutschZ_Menu_4K_Silhouette_Transparent.png` - unverändertes geliefertes Original.
- `src/DeutschZ_MenuMusic/gui/menu_assets/background/DeutschZ_Menu_4K_Silhouette_Transparent.paa` - DayZ-PAA.
- `docs/products/MenuZ/TESTANLEITUNG.md` - konkreter visueller Testablauf.
- `docs/CODEX_HANDOFF_CURRENT.md` - diese Übergabe.

## 7. Nicht versionierte oder fremde Dateien

- Zahlreiche Änderungen anderer DeutschZ-Produkte waren bereits vorhanden und wurden weder gestaged noch verändert.
- KotHZ-, CriminalZ-, DMarkZ-, HalftanZ-, RadioMissionZ-, HUDZ- und Workshop-Dateien sind nicht Bestandteil dieses Commits.
- Keine pauschale Bereinigung und kein Reset der fremden Änderungen durchgeführt.

## 8. Technischer Zustand

### Build

- Befehl: `dzl build DeutschZ_MenuMusic --clean --no-binarize --sign --key DeutschZ --force`
- Ergebnis: PASS.
- gepackte Dateien: 281.
- PBO: `E:\DeutschZ\DeutschZServer\@DeutschZ_MenuMusic\Addons\DeutschZ_MenuMusic.pbo`
- PBO-Größe: 77.408.804 Byte.
- PBO SHA-256: `5881A2B974D4A8F95722633FCA990F799BEA40179874BE6B4331674932A51E75`

### Signatur

- BISIGN: `Addons\DeutschZ_MenuMusic.pbo.DeutschZ.bisign`
- BIKEY: `keys\DeutschZ.bikey`
- DSCheckSignatures vor und nach Deployment: PASS.
- privater Schlüssel ausgeliefert: NEIN.

### Server und Client

- Servertest möglich: JA, Paket vollständig.
- neuer Serverstart ausgeführt: NEIN.
- Clienttest erforderlich: JA.
- Sale Ready: NEIN bis zur visuellen Bestätigung durch Patrick.

## 9. Tests

| Test | Ergebnis | Beweis |
|---|---|---|
| PNG-Abmessungen und Alpha | PASS | 3840 x 2160, ARGB, Alpha 0 bis 255 |
| Original-PNG unverändert | PASS | identischer SHA-256 `7E7E64E96357E63EC5413574AB5AB3B237796D4709DE4C51D082C6891C011F6F` |
| Layoutklammern | PASS | 145 öffnende und 145 schließende Klammern |
| Layout-Bildreferenzen | PASS | 43 von 43 vorhanden |
| Preflight | PASS | 0 Fehler, 1 akzeptierte Warnung |
| Clean Build | PASS | 281 Dateien gepackt |
| PBO-Inhaltsprüfung | PASS | Hintergrund, Layout, Script und vier Karten mit BankRev bestätigt |
| BISIGN/BIKEY | PASS | DSCheckSignatures meldet `is OK` |
| atomare Ausgabe | PASS | Staging validiert, aktiver Ordner vollständig ersetzt, Hashes identisch |
| visueller Clienttest | PATRICK_REQUIRED | 1920x1080, 2560x1440, 1280x720; optional 3440x1440 |

## 10. Relevante Logs

- DayZ-Laufzeitlogs wurden für diesen UI-Build nicht neu erzeugt.
- Preflight-Bericht: `C:\Users\patsl\DayZProjects\build\@DeutschZ_MenuMusic\preflight-report.txt`
- Buildausgabe: erfolgreich, PBO und Signatur erzeugt.
- Signaturausgabe: `Signature ...DeutschZ.bisign is OK`.

## 11. Skills und Regeln

- verwendet: `dayz-modding` einschließlich vollständiger GUI-Referenz.
- eingehalten: Source zuerst, Build danach, Staging außerhalb des aktiven Ziels, keine Backup-Ordner im Ausgabeordner.
- eingehalten: keine KotHZ-/ConvoyZ-Logik, kein neues HUD und keine zusätzlichen Features.
- Übergabe wurde erst nach abgeschlossenem Build und Deployment aktualisiert.

## 12. Entscheidungen

- Die 3840x2160-PNG bleibt unverändert im Source.
- Weil ImageToPAA nur Potenz-von-zwei-Abmessungen akzeptiert, nutzt die PAA 4096x2048 mit 16:9-Ausgabekompensation.
- Die vorhandenen gelieferten PAA-Karten wurden wiederverwendet; keine Ersatzgrafiken oder fremden Assets wurden erzeugt.
- Der alte Hintergrund wurde nur aus der aktiven Layoutreferenz entfernt und nicht ungeprüft gelöscht.
- Kein Testpreset wurde eigenmächtig aktiviert und kein DayZ-Client gestartet.

## 13. Offene Blocker

- Blocker: visuelle Gesamtansicht durch Patrick noch nicht bestätigt.
- Priorität: HOCH für Sale Ready, nicht für Build Ready.
- Benötigter Beweis: Screenshots/Bestätigung bei 1920x1080, 2560x1440 und 1280x720.
- Bei Fehlern zusätzlich frische Client-RPT- und Script-Logs liefern.

## 14. Rollback

- Implementierungscommit: `7b2b5ea`.
- Ein gezielter Revert dieses Commits setzt ausschließlich die MenuZ-Änderungen zurück.
- Kein `git reset --hard` verwenden, da das Repository weitere fremde Änderungen enthält.
- Der vorherige aktive Ausgabeordner wurde nach erfolgreicher Hash- und Signaturprüfung ersetzt und nicht als Backup im Ausgabeordner behalten.

## 15. Reifegrad

- MenuZ Build Ready: 100 %.
- MenuZ Client Test Ready: 100 %.
- MenuZ visuell bestätigt: 0 % bis Patricks Test.
- MenuZ Sale Ready: NEIN.
- KotHZ/ConvoyZ: in diesem Auftrag unverändert.

## 16. Freigabestatus

- MENUZ BUILD READY: JA
- MENUZ CLIENT TEST READY: JA
- MENUZ SALE READY: NEIN
- Grund: visueller Clienttest fehlt.
- PBO erfolgreich gebaut: JA
- Signatur und Key geprüft: JA
- zentrale Ausgabe aktuell: JA

## 17. Genau ein nächster Schritt

- Patrick testet `E:\DeutschZ\DeutschZServer\@DeutschZ_MenuMusic` nach `TESTANLEITUNG.md` visuell in den drei Pflichtauflösungen und meldet Screenshots oder konkrete Abweichungen zurück.

## 18. Nachricht für ChatGPT

MenuZ ist technisch fertig gebaut, signiert und zentral ausgegeben. Keine weitere MenuZ-Funktion erfinden. Bis Patricks visueller Bestätigung bleibt Sale Ready auf NEIN. Bei Rückmeldung nur die konkret sichtbaren Abweichungen bearbeiten; danach erneut Clean Build, Signaturprüfung, atomare Ausgabe und Übergabe durchführen.

## Vollständige übertragene Dateien

- `E:\DeutschZ\DeutschZServer\@DeutschZ_MenuMusic\Addons\DeutschZ_MenuMusic.pbo`
- `E:\DeutschZ\DeutschZServer\@DeutschZ_MenuMusic\Addons\DeutschZ_MenuMusic.pbo.DeutschZ.bisign`
- `E:\DeutschZ\DeutschZServer\@DeutschZ_MenuMusic\keys\DeutschZ.bikey`
- `E:\DeutschZ\DeutschZServer\@DeutschZ_MenuMusic\mod.cpp`
- `E:\DeutschZ\DeutschZServer\@DeutschZ_MenuMusic\meta.cpp`
- `E:\DeutschZ\DeutschZServer\@DeutschZ_MenuMusic\TESTANLEITUNG.md`

## Installations- und Testschritte

1. `E:\DeutschZ\DeutschZServer\@DeutschZ_MenuMusic` als Client- und Server-Mod verwenden.
2. Den enthaltenen öffentlichen Key serverseitig in den Keys-Ordner übernehmen, falls er dort noch nicht identisch vorhanden ist.
3. Hauptmenü, Loading-Screen und Deathscreen nach `TESTANLEITUNG.md` prüfen.
4. Bei einem Fehler Auflösung, Screenshot, frische RPT und frisches Script-Log liefern.
