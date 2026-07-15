# Phase-A-Testanleitung

## Zweck

Dieser Stand prüft nur Packaging, Signatur, Script-Compile und den Lebenszyklus des Free-Controllers. Er startet noch kein spielbares KotH-Event.

## Servertest

1. `@DeutschZ_KotHZ_Free` als Client- und Servermod laden.
2. Server starten und bis zum vollständigen Missionstart warten.
3. Frisches Script-Log und RPT auf `SCRIPT (E)` sowie `Cannot compile` prüfen.
4. Im Script-Log folgende Zeile erwarten:
   `[DeutschZ KotHZ Free] Controller initialized. Version 1.0.0-phase-a. State IDLE.`
5. Server kontrolliert beenden und prüfen, dass keine Null-Pointer- oder Cleanup-Fehler protokolliert werden.

## Erfolgskriterien

- Server kompiliert die drei Scriptmodule ohne Fehler.
- Der Controller initialisiert genau einmal in `IDLE`.
- Es erscheinen keine Terminal-, KOTHGate-, Boss-, Wellen- oder Expansion-Abhängigkeiten.
- Ein Gameplaytest ist erst nach den folgenden Kernphasen sinnvoll.
