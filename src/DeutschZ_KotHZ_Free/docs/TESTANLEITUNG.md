# DeutschZ KotHZ Free - Testanleitung

1. `@DeutschZ_KotHZ_Free` auf Client und Server laden; `keys/DeutschZ.bikey` serverseitig bereitstellen.
2. Einstellungen unter `$profile:DeutschZ-System/DeutschZ_KotHZ/KotHZSettings.json` bearbeiten.
3. Log unter `$profile:DeutschZ-System/LogZ/DeutschZ_KotHZ/KotHZ.log` kontrollieren.
4. Eventstart pruefen: genau ein Mast, genau eine Fahne, weisser Rauch in der Ankuendigung und gruener Rauch in der aktiven Phase.
5. Capture mit einem Spieler pruefen. Mit Expansion und `RespectExpansionParties=true` duerfen mehrere Mitglieder derselben Expansion-Gruppe weiter capturen; eine fremde Gruppe beziehungsweise ein ungruppierter Fremder muss Rot/pausiert ausloesen.
6. Marker, Notify und neutrale Progressbar visuell pruefen; Balken und Prozent muessen dieselbe Capture-Quelle verwenden. Mit Expansion sind 3D-Marker `Skull 3` und Expansion-Notify optional aktiv, ohne Expansion greifen die Vanilla-/RPC-Fallbacks.
7. Nach Abschluss genau eine RewardCrate, Inhalt, 500 Slots, Nicht-Aufnehmbarkeit und konfigurierte Lifetime pruefen.
8. Abbruch, Abschluss, Disconnect und Serverrestart auf Marker-, UI-, Gegner-, Smoke-, Mast-, Fahnen-, Timer- und Objekt-Cleanup pruefen.

Bei Fehlern die frische RPT, das Scriptlog und `KotHZ.log` sichern. Ein bestandener Serverstart ersetzt keinen Gameplaytest.
