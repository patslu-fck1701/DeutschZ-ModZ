# KotHZ Free Clientfix 2026-07-16

## Behobene Ursachen

- Progress-RPCs werden waehrend `ACTIVE` nur an lebende Spieler im serverseitigen Capture-Radius gesendet.
- Verlaesst ein zuvor sichtbarer Spieler den Radius, erhaelt er genau ein Hide-RPC.
- Bei Ende, Abbruch, Cleanup und Missionende wird die Bar fuer alle sicher ausgeblendet.
- Die optionale globale Anzeige waehrend `ANNOUNCED` ist per `ShowProgressDuringAnnouncement` konfigurierbar und standardmaessig deaktiviert.
- Der Vanilla-Markerpfad besitzt jetzt den erforderlichen fuehrenden Backslash.
- `MapMenu.LoadMapMarkers()` ist der einzige Renderer fuer den KotHZ-Marker.
- Die neue Free-Flagge nutzt `camo`; die neue RewardCrate nutzt `camoGround`. Beide Assets stammen aus dem gelieferten `DZ_Gear_Camping`-UV-Set.
- Ist Expansion Navigation geladen und `UseExpansionMarkerWhenAvailable=true`, erzeugt der Server genau einen roten 3D-Servermarker mit dem Expansion-Icon `Skull 3`; ohne Expansion oder bei fehlgeschlagener Erzeugung bleibt der Vanilla-Marker aktiv.
- Ist Expansion Core geladen und `UseExpansionNotificationsWhenAvailable=true`, werden Expansion-Notifications verwendet; sonst bleibt der eigene RPC-/Chat-Fallback aktiv.
- Mit `RespectExpansionParties=true` zaehlen alle lebenden Spieler derselben Expansion-Gruppe im Radius als eine Capture-Seite und blockieren sich nicht gegenseitig. Spieler anderer Gruppen oder ungruppierte Fremde erzeugen weiterhin den umkaempften Zustand.

## Erneuter Clienttest

1. Eventstart: Marker an `EventPosition` auf der Vanilla-Karte pruefen.
2. Ausserhalb des Capture-Radius: keine Statusbar.
3. Radius betreten: Statusbar erscheint mit dem serverautoritativen Capturewert.
4. Radius verlassen: Statusbar wird sofort ausgeblendet.
5. Ende/Abbruch/Cleanup: Marker und Statusbar verschwinden.
6. Genau eine Fahne und die texturierte RewardCrate visuell pruefen.
7. Mit Expansion: 3D-Skull-Marker und Expansion-Notify pruefen; mit zwei Spielern derselben Expansion-Gruppe muss Capture weiterlaufen.
8. Ohne Expansion: sauberer Vanilla-Marker-/Notify-Fallback und weiterhin vollstaendiger Eventstart.

Gameplay- und Sichtpruefung bleiben erforderlich.
