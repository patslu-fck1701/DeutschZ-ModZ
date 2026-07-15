# CODEX HANDOFF CURRENT

## Aktueller Auftrag

- Produkt: DeutschZ MenuZ / `@DeutschZ_MenuMusic`
- Phase: gezielter Clientfix nach Screenshots vom 15.07.2026
- KotHZ Auftrag 02: pausiert, keine KotHZ-Datei geaendert
- Camo: keine Datei geaendert
- Status: BUILD READY = JA, CLIENT TEST READY = JA, SALE READY = NEIN
- Grund fuer SALE READY NEIN: erneuter visueller Clienttest durch Patrick erforderlich

## Umgesetzte MenuZ-Korrekturen

- `DeutschZModZ` als riesiger Debug-/Modded-Text entfernt.
- kleine rote Serverkennung `DeutschZ - Server #1` nach dem Vanilla-`Refresh()` stabil gesetzt.
- Intro-Kamera um 8 Prozent zurueckgesetzt; Figur dadurch proportional kleiner.
- transparente Spieler-Aussparung im unveraenderten 4K-Motiv um 24 Source-Pixel erweitert.
- rechte News-/Statusschriften auf exakte Pixelgroessen begrenzt; kein abgeschnittener Platzhaltertext mehr vorgesehen.
- rechtes oberes Panel als `SERVERSTATUS` stabilisiert; es zeigt nur bestaetigte Serveradresse und vorhandene MenuZ-Meldungen, keine erfundenen Livewerte.
- Eventbereich vergroessert; maximal drei Karten gleichzeitig sichtbar.
- untere Navigation in die Reihenfolge HOME, SERVER, EVENTS, REGELN, SHOP, SUPPORT, PROFIL, OPTIONEN, ENDE gebracht.
- Loading-Trennlinie an den oberen Rand des unteren Panels verschoben.
- Loading-Titel rot, Untertitel gruen und groesser.
- Prozentwert wird aus `GetMin()`, `GetMax()` und `GetCurrent()` derselben Progressbar berechnet.

## Geaenderte Source-Dateien

- `src/DeutschZ_MenuMusic/data/ui/loading/loading_content.json`
- `src/DeutschZ_MenuMusic/config.cpp`
- `src/DeutschZ_MenuMusic/gui/dzkothg_loading_overlay.layout`
- `src/DeutschZ_MenuMusic/gui/dzkothg_main_menu.layout`
- `src/DeutschZ_MenuMusic/gui/menu_assets/background/DeutschZ_Menu_4K_Silhouette_Transparent.png`
- `src/DeutschZ_MenuMusic/gui/menu_assets/background/DeutschZ_Menu_4K_Silhouette_Transparent.paa`
- `src/DeutschZ_MenuMusic/scripts/3_Game/DeutschZ_MenuMusic/modded_LoadingScreen.c`
- `src/DeutschZ_MenuMusic/scripts/5_Mission/DeutschZ_MenuMusic/modded_MainMenu.c`
- `src/DeutschZ_MenuMusic/mod.cpp`
- `docs/products/MenuZ_CLIENT_TEST.md`
- `docs/CODEX_HANDOFF_CURRENT.md`

## Build- und Signaturstatus

- Preflight: BESTANDEN, 0 Fehler, 1 bekannte Grossbuchstabenwarnung
- Build: `dzl build DeutschZ_MenuMusic --clean --no-binarize --sign --key DeutschZ --force`
- Build: BESTANDEN
- PBO-Inhalt mit `BankRev -lf` geprueft: JA
- Signatur mit `DSCheckSignatures.exe` geprueft: JA
- Ergebnis: `Signature ...DeutschZ.bisign is OK`
- Gameplay-/Clientbeweis: OFFEN

## Letzter aktualisierter Ausgabeordner

- Zeitpunkt der Synchronisierung: 15.07.2026, 19 Uhr Europe/Berlin
- Vollstaendiger Ausgabeordner: `E:\DeutschZ\DeutschZServer\@DeutschZ_MenuMusic`
- Synchronisierung: atomar ueber externes Staging
- alte aktive PBO/BISIGN ersetzt: JA
- gemischte Altversion im aktiven Modordner: NEIN

### Uebertragene Dateien

- `E:\DeutschZ\DeutschZServer\@DeutschZ_MenuMusic\Addons\DeutschZ_MenuMusic.pbo`
- `E:\DeutschZ\DeutschZServer\@DeutschZ_MenuMusic\Addons\DeutschZ_MenuMusic.pbo.DeutschZ.bisign`
- `E:\DeutschZ\DeutschZServer\@DeutschZ_MenuMusic\keys\DeutschZ.bikey`
- `E:\DeutschZ\DeutschZServer\@DeutschZ_MenuMusic\mod.cpp`
- `E:\DeutschZ\DeutschZServer\@DeutschZ_MenuMusic\meta.cpp`
- `E:\DeutschZ\DeutschZServer\@DeutschZ_MenuMusic\MenuZ_CLIENT_TEST.md`

## Installation und Test

1. Den kompletten Ordner `E:\DeutschZ\DeutschZServer\@DeutschZ_MenuMusic` als aktuelle Client-/Servermod verwenden.
2. Sicherstellen, dass nur diese MenuZ-PBO aktiv ist.
3. Hauptmenue bei 1920x1080 testen; danach 2560x1440, 1280x720 und optional 3440x1440.
4. Serverbeitritt starten und Loading-Titel, Untertitel, Linie, Balken und Prozent vergleichen.
5. Screenshots sowie bei Scriptfehlern frische Client-RPT und Scriptlogs liefern.
6. Vollstaendige Kriterien stehen in `MenuZ_CLIENT_TEST.md` im Ausgabeordner.

## Bekannte Grenzen

- Spielerzahl, Ping, Wetter, Restart und Eventcountdown haben im bestehenden MenuZ keine bestaetigte Live-Datenquelle. Sie wurden deshalb nicht erfunden.
- Camo ist kein MenuZ-Widget und wurde wegen der ausdruecklichen Scopegrenze nicht veraendert.
- Die weitergehende Komplett-Neugestaltung aus der spaeteren Fehlerliste ist nur teilweise als sichere Layoutkorrektur umgesetzt; neue Live-Datenfunktionen bleiben ausserhalb dieses Clientfixes.
- Erneuter visueller Clienttest ist zwingend.

## Naechster Schritt

Patrick testet exakt den Stand aus `E:\DeutschZ\DeutschZServer\@DeutschZ_MenuMusic` und liefert Hauptmenue- sowie Loading-Screenshot bei 1920x1080.
