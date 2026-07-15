# DeutschZ MenuZ - visueller Clienttest

## Teststand

- Clean Build und Signatur sind technisch geprueft.
- Der visuelle Clienttest steht noch aus.
- Der Stand ist deshalb client-testbereit, aber nicht verkaufsbereit.

## Zu pruefende Aufloesungen

- 1920 x 1080
- 2560 x 1440
- 1280 x 720
- optional 3440 x 1440

## Pruefschritte

1. `@DeutschZ_MenuMusic` aus `E:\DeutschZ\DeutschZServer` als Client-Mod laden.
2. Hauptmenue oeffnen und die unveraenderte transparente Spielfiguren-Aussparung kontrollieren.
3. Hintergrund, Logo, linke Beschreibung, News und Eventkarten auf Ueberlappung, Verzerrung und abgeschnittenen Text pruefen.
4. Serverbeitritt starten und Loading-Overlay auf Seitenverhaeltnis, Logoform und Lesbarkeit pruefen.
5. Deathscreen ausloesen und Hintergrund, Logo, Maussteuerung und Bedienelemente pruefen.
6. Pro Aufloesung einen Screenshot von Hauptmenue, Loading-Overlay und Deathscreen erstellen.

## Erfolgskriterien

- Hintergrund erscheint im 16:9-Menue unverzerrt.
- Die transparente Aussparung bleibt frei und die DayZ-Spielfigur wird dort sichtbar.
- KotHZ-, ConvoyZ-, GroundZero- und Operation-Eventkarten sind korrekt zugeordnet.
- News-Titel, Meta-Zeile und News-Text bleiben innerhalb des rechten Panels.
- Beschreibung und Serverstatus bleiben innerhalb des linken Panels.
- keine ueberlappenden Texte oder Bedienelemente
- kein abgeschnittener Inhalt
- Maus bleibt auf dem Deathscreen steuerbar
- Musik und UI laden ohne Scriptfehler

Bei Fehlern bitte Aufloesung, Screenshot sowie frische RPT- und Script-Logs bereitstellen.
