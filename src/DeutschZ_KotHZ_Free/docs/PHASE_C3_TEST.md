# KotHZ Free Phase C3 - Notify und Marker

- Ein einziges Vanilla-`ScriptRPC` (`24871`, Protokollversion 1) transportiert Notify und Marker.
- Notifications werden serverseitig ausgelöst; optional kann `NotifyUseChatFallback` den Vanilla-Chat statt Popup-RPC verwenden.
- Keine CF- oder Expansion-Abhängigkeit.
- Genau ein clientseitiger Marker-Store und ein `MapMenu`-Renderer.
- Marker erscheint bei `ANNOUNCED`/`ACTIVE` und wird bei Erfolg, Cleanup und Missionende entfernt.
- Neu verbundene Spieler erhalten den aktuellen serverautoritativen Markerzustand.

Der Server-Compiletest beweist RPC-Klassen und Missionstart. Popupdarstellung und MapWidget-Marker bleiben Patricks Clienttest.
