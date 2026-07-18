# DeutschZ MenuZ V2 – Sicht- und Funktionstest

## Installation

1. `@DeutschZ_MenuMusic` deaktivieren. V1 und V2 verwenden dieselben MainMenu-Hooks und dürfen nicht parallel geladen werden.
2. `@DeutschZ_MenuMusic_V2_Test` in die Client- und Server-Modliste aufnehmen.
3. `keys/DeutschZ.bikey` in den Server-Keyordner übernehmen.
4. Server und Client vollständig neu starten.

## Profilpfade

- Config: `$profile:DeutschZ-System/DeutschZ_MenuZ/MenuZSettings.json`
- Log: `$profile:DeutschZ-System/LogZ/DeutschZ_MenuZ/MenuZ.log`

Vorhandene gültige Settings werden nicht überschrieben. Eine fehlende Datei wird erzeugt; bei beschädigtem JSON bleibt eine Sicherung als `MenuZSettings.invalid.json` erhalten.

## Sichttest

- Pflichtauflösungen: 1920×1080, 2560×1440 und 1280×720; optional 3440×1440.
- Linkes Community-Panel kompakt und vollständig sichtbar.
- Spielerfigur in der Mitte frei; Gesicht, Körpermitte und Waffe werden nicht von Panels verdeckt.
- Rechts: News, Statistiken und Musikplayer ohne Überlagerung.
- Unten Mitte: genau vier Karten in dieser Reihenfolge: ConvoyZ, KotHZ, AirdropZ, HeliCrashZ.
- Untere Navigation durchgehend, mit rotem DeutschZ-Z in der Mitte.
- NORMAL/HOVER/PRESSED der Hauptbuttons, Navigation und Musikknöpfe prüfen.

## Musiktest

- Beim Öffnen läuft genau ein zufälliger Titel; maximale Lautstärke bleibt 66 Prozent.
- Previous und Next fordern jeweils einen neuen Titel mit Fadeout und Wiederholungsschutz an.
- Play/Pause stoppt beziehungsweise startet die Menü-Kategorie, ohne mehrere Instanzen zu stapeln.
- Mute verändert nur die Musiklautstärke und stellt sie beim zweiten Klick oder beim Verlassen des Menüs wieder her.
- Discord, Website, Vote, Serverbrowser, Charakter, Einstellungen und Ende bleiben funktionsfähig.

## Weitere Oberflächen

- Loading-Screen und Deathscreen auf Stretch, Textüberlauf und Fortschrittsanzeige prüfen.
- Keine erfundenen Livewerte erwarten: Spielerzahl, Ping, Wetter und Eventcountdown werden ohne echte Datenquelle nicht simuliert.

## Fehlerbelege

Benötigt werden die frische Client-RPT, das Client-Scriptlog, die Auflösung und ein Screenshot des betroffenen UI-Bereichs.
