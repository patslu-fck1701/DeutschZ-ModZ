# DeutschZ MenuZ V2 Test

## Installation

1. `@DeutschZ_MenuMusic` deaktivieren. V1 und V2 verwenden absichtlich dieselben Menu-Hooks und werden nicht parallel geladen.
2. `@DeutschZ_MenuMusic_V2_Test` in die Client- und Server-Modliste aufnehmen.
3. `Keys/DeutschZ.bikey` in den Server-Keyordner übernehmen.
4. Server und Client neu starten.

## Profilpfade

- Config: `$profile:DeutschZ-System/DeutschZ_MenuZ/MenuZSettings.json`
- Log: `$profile:DeutschZ-System/LogZ/DeutschZ_MenuZ/MenuZ.log`

Vorhandene gültige Settings werden nicht überschrieben. Eine fehlende Datei wird erzeugt; bei beschädigtem JSON bleibt eine Kopie als `MenuZSettings.invalid.json` erhalten.

## Clienttest

- 1920x1080, 2560x1440 und 1280x720 prüfen; optional 3440x1440.
- Hauptmenü: Hintergrund, Charakterfreistellung, linkes Panel, News, Statistiken, vier Eventkarten, Musikpanel und untere Navigation.
- NORMAL/HOVER/PRESSED der Hauptbuttons und Navigation prüfen.
- PLAY startet die Menu-Kategorie; NEXT fordert über den echten DayZ DynamicMusicPlayer einen neuen zufälligen Titel mit Fadeout und Wiederholungsschutz an.
- Loading-Screen und Deathscreen auf Stretch, Textüberlauf und Fortschrittsanzeige prüfen.
- Keine erfundenen Livewerte erwarten: Spielerzahl, Ping, Wetter und Eventcountdown werden ohne echte Quelle nicht simuliert.

## Fehlerbelege

Benötigt werden die frische Client-RPT, das Client-Scriptlog, Auflösung und ein Screenshot des betroffenen UI-Bereichs.
