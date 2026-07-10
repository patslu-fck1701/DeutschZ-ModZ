# Architektur

## Trennung der Verantwortlichkeiten

### 3_Game

- Konstanten und Enums
- RPC-IDs und Nachrichtenmodelle
- JSON-Konfigurationsmodelle
- gemeinsam nutzbare Datenklassen

### 4_World

- Items und Entitäten
- Aktionen
- Eventobjekte
- Welt- und Interaktionslogik

### 5_Mission

- Servermanager und Lifecycle
- Missionsinitialisierung
- Client-HUD
- Marker und Notifications
- RPC-Senden und -Empfangen

## Autorität

Der Server entscheidet über:

- Eventstart und Zustandswechsel
- Berechtigung von Aktionen
- Spawn und Löschung von Objekten
- AI-, Zombie- und Lootzustände
- Rewards und Cooldowns
- Erfolg, Abbruch und Cleanup

Der Client darf Zustände anzeigen, aber keine Rewards oder Eventfortschritte verbindlich festlegen.

## Event-State-Machine

Events sollen klar definierte Zustände nutzen, zum Beispiel:

```text
IDLE -> ANNOUNCED -> PREPARING -> ACTIVE -> REWARD -> CLEANUP -> COOLDOWN
```

Jeder Übergang benötigt Logging, Validierung und ein definiertes Fehler-/Timeout-Verhalten.

## Optionale Integrationen

Expansion Notify und Marker werden über Adapter gekapselt. Fehlt eine optionale Integration, läuft die Eventlogik mit einem dokumentierten Fallback weiter.
