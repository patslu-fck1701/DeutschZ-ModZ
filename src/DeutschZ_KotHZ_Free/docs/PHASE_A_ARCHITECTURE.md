# DeutschZ KotHZ Free v1 - Phase A

## Scope

Phase A stellt ausschließlich den eigenständigen Vanilla-Kern und die serverautoritative State Machine bereit. Es werden noch keine Weltobjekte, Trigger, Gegner, Rewards, Marker oder UI-Elemente erzeugt.

## Scriptmodule

- `3_Game`: Konstanten, Logging und Zustandsdefinitionen
- `4_World`: laufende Event-Session und serverautoritiver Controller
- `5_Mission`: Lebenszyklus-Hook für Initialisierung und kontrolliertes Herunterfahren

## Zustandsfolge

`IDLE -> ANNOUNCED -> ACTIVE -> CAPTURE_COMPLETE -> REWARD -> CLEANUP -> IDLE`

Ein kontrollierter Abbruch darf aus `ANNOUNCED`, `ACTIVE`, `CAPTURE_COMPLETE` oder `REWARD` direkt nach `CLEANUP` wechseln. Ein zweiter Start wird außerhalb von `IDLE` abgewiesen.

## Bewusst nicht enthalten

- Terminal und KOTHGate
- Wellen, Boss und Spezial-Infected
- Keycards, Fireworks und Storylogik
- Premium-UI, Premium-Effekte oder Premium-Musik
- Expansion AI, Expansion Groups oder andere harte Drittanbieter-Abhängigkeiten
- Admin-UI und Camo
