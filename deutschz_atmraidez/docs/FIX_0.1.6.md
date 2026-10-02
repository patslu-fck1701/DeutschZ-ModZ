# ATM RaideZ 0.1.6 Phase-2-Rehack-Sperre

## Ziel

Nach erfolgreichem Abschluss von Phase 1 darf am selben ATM kein neuer Hack gestartet werden.

## Umsetzung

- `m_DZATM_RaidActionBlocked` als eigener NetSync-Zustand.
- Phase 1: neue Action nicht durch den allgemeinen Banking-Lock selbst abbrechen lassen.
- Phase 2 und ATM-Cooldown: neue Raid-Action ausblenden und ablehnen.
- Serverseitiger Cooldown- und Parallelraid-Schutz bleibt unveraendert.

## Offener Test

- Phase 1 vollstaendig abschliessen.
- In Phase 2 darf `ATM ausrauben` nicht erneut erscheinen.
- Nach Ablauf/Loeschen des ATM-Cooldowns muss die Action wieder verfuegbar sein.
