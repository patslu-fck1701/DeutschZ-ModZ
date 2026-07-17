# MenuZ V2 Client-Testplan

## Installation

- V1 `@DeutschZ_MenuMusic` deaktivieren.
- V2 `@DeutschZ_MenuMusic_V2_Test` auf Client und Server laden.
- Optional fuer den Kombitest `@DeutschZ_UiAdminZ` zusaetzlich laden.
- Nicht V1 und V2 gleichzeitig laden.

## Aufloesungen

- 1920x1080
- 2560x1440
- 1280x720
- optional 3440x1440

## Hauptmenue

- Zielreferenz: `E:\DeutschZ\DeutschZ-ModZ\docs\MenuZ_V2\menuz_v2_target_reference.png`.
- Hintergrund ohne Stretch, Crop-Artefakte oder Alpha-Rand.
- Spielerfigur zentral, ohne Kollision mit Panels; Camo sichtbar und nicht ueberdeckt.
- Linke Navigation und News vollstaendig lesbar.
- Rechte Server-/Eventbereiche ohne erfundene Livewerte.
- Musikplayer sichtbar; PLAY und NEXT funktionieren, kein direkter Track-Repeat, keine Audio-Ueberlagerung, maximal 66 Prozent.
- Alle aktiven Buttons: Normal, Hover und Pressed pruefen.
- Disabled/Selected-Zustaende duerfen keine falschen Aktionen ausloesen.
- Untere Navigation bleibt im Safe-Area-Bereich und ueberlappt nichts.

## Loading-/Deathscreen

- 16:9 und keine Verzerrung.
- Kein Textueberlauf.
- Keine erfundene Prozentanzeige.
- Maus und Navigation im Deathscreen pruefen.

## UiAdminZ optional

- F7 oeffnet das sichtbare Admin-Lite-Layout.
- Maus sichtbar, Spielinput gesperrt.
- F7 und ESC schliessen; danach Spielinput vollstaendig wiederhergestellt.
- Neue gruen/schwarze Panelgrafiken sichtbar; kein schwarzes Vollbild-Overlay.

## Fehlernachweis

Bei Fehlern Screenshot plus frische Client-RPT und Scriptlog liefern. Den sichtbaren Fehler mit Aufloesung und genauer Aktion beschreiben.
