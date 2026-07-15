# Phase-B-Livetest

1. Mod und Key laden; Server starten.
2. Nach `AutoStartDelaySeconds` muss genau ein Mast mit genau einer Fahne an `EventPosition` stehen.
3. Rauch ist waehrend `ANNOUNCED` weiss und waehrend `ACTIVE` gruen.
4. Genau ein lebender Spieler im `CaptureRadius` hebt die Fahne ueber `CaptureTimeSeconds`.
5. Zwei oder mehr lebende Spieler im Radius pausieren den Fortschritt und schalten den Rauch rot.
6. Abbruch und normales Ende muessen Mast, Fahne und Rauch vollstaendig entfernen.

Der Server-Compiletest beweist keine visuelle Darstellung und keinen echten Capture-Mehrspielertest.
