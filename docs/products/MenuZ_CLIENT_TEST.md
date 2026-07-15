# DeutschZ MenuZ - visueller Clienttest

## Teststand

- Build und Signatur sind technisch geprueft.
- Der visuelle Clienttest steht noch aus.
- Der Stand ist deshalb client-testbereit, aber nicht verkaufsbereit.

## Zu pruefende Aufloesungen

- 1920 x 1080
- 2560 x 1440
- 1280 x 720
- optional 3440 x 1440

## Pruefschritte

1. `@DeutschZ_MenuMusic` aus diesem Ausgabeordner als Client-Mod laden.
2. Hauptmenue oeffnen und Logo, Serverinformationen, News und Bedienelemente auf Ueberlappung, Verzerrung und abgeschnittenen Text pruefen.
3. Serverbeitritt starten und Loading-Overlay auf Seitenverhaeltnis, Logoform und Lesbarkeit pruefen.
4. Deathscreen ausloesen und Hintergrund, Logo, Maussteuerung und Bedienelemente pruefen.
5. Pro Aufloesung einen Screenshot von Hauptmenue, Loading-Overlay und Deathscreen erstellen.

## Erfolgskriterien

- oben links steht klein und rot `DeutschZ - Server #1`; `DeutschZModZ` erscheint nirgends
- Spielfigur ist leicht verkleinert, kollidiert weder mit Navigation noch Schuhbereich
- kein gruener oder unsauberer Rand an der transparenten Spieler-Aussparung
- Serverstatus-/News-Text bleibt vollstaendig im rechten oberen Panel
- genau drei vergroesserte Eventkarten sind sichtbar
- Loading-Titel ist rot, Untertitel gruen und die Trennlinie sitzt am oberen Rand des unteren Panels
- Prozentanzeige und Ladebalken verwenden denselben Min-/Max-/Ist-Wert
- keine gestreckten oder gequetschten Logos
- keine ueberlappenden Texte oder Bedienelemente
- kein abgeschnittener Inhalt
- Maus bleibt auf dem Deathscreen steuerbar
- Musik und UI laden ohne Scriptfehler

Bei Fehlern bitte Aufloesung, Screenshot sowie frische RPT- und Script-Logs bereitstellen.
