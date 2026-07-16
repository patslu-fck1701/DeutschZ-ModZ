# KotHZ Free Clientfix 2026-07-16

## Behobene Ursachen

- Progress-RPCs werden waehrend `ACTIVE` nur an lebende Spieler im serverseitigen Capture-Radius gesendet.
- Verlaesst ein zuvor sichtbarer Spieler den Radius, erhaelt er genau ein Hide-RPC.
- Bei Ende, Abbruch, Cleanup und Missionende wird die Bar fuer alle sicher ausgeblendet.
- Die optionale globale Anzeige waehrend `ANNOUNCED` ist per `ShowProgressDuringAnnouncement` konfigurierbar und standardmaessig deaktiviert.
- Der Vanilla-Markerpfad besitzt jetzt den erforderlichen fuehrenden Backslash.
- `MapMenu.LoadMapMarkers()` ist der einzige Renderer fuer den KotHZ-Marker.
- Die neue Free-Flagge nutzt `camo`; die neue RewardCrate nutzt `camoGround`. Beide Assets stammen aus dem gelieferten `DZ_Gear_Camping`-UV-Set.

## Erneuter Clienttest

1. Eventstart: Marker an `EventPosition` auf der Vanilla-Karte pruefen.
2. Ausserhalb des Capture-Radius: keine Statusbar.
3. Radius betreten: Statusbar erscheint mit dem serverautoritativen Capturewert.
4. Radius verlassen: Statusbar wird sofort ausgeblendet.
5. Ende/Abbruch/Cleanup: Marker und Statusbar verschwinden.
6. Genau eine Fahne und die texturierte RewardCrate visuell pruefen.

Gameplay- und Sichtpruefung bleiben erforderlich.
