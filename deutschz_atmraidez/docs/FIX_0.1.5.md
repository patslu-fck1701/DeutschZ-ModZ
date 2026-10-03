# ATM RaideZ 0.1.5 Continuous-Action-Fix

## Symptom

`ATM ausrauben` war sichtbar und startete, brach aber unmittelbar wieder ab.

## Ursache

`StartRaid()` sperrte den ATM per NetSync. `ActionCondition()` verlangte gleichzeitig einen nicht gesperrten ATM und invalidierte dadurch die eigene laufende Continuous Action.

## Fix

- Raid-Lock aus der clientseitigen Raid-`ActionCondition()` entfernt.
- Serverseitige Validierung in `StartRaid()` bleibt unveraendert.
- Cursorbedingung auf den im funktionierenden Vorgänger belegten `CCTCursor(UAMaxDistances.DEFAULT)`-Aufruf gesetzt.

## Offener Test

- Phase 1 muss ueber die konfigurierte Dauer durchlaufen.
- Danach muss Phase 2 beginnen und der ATM weiterhin fuer normales Banking gesperrt bleiben.
