# DeutschZ ATM RaideZ Integration

## Expansion 1.9.72

Die Mod erweitert `ExpansionATMBase`. In der lokal installierten Expansion-Version erben
`ExpansionATM_1`, `ExpansionATM_2`, `ExpansionATM_3` und `ExpansionATMLocker` davon. Es werden
keine eigenen ATM-Objekte oder Positionen erzeugt.

`ExpansionActionOpenATMMenu` wird waehrend eines aktiven Raids und waehrend des ATM-Cooldowns
clientseitig ausgeblendet und serverseitig vor `SendPlayerATMData` erneut abgefangen. Ein bereits
vor Raidstart geoeffnetes Expansion-Menue kann mit der oeffentlichen Action-API nicht zwangsweise
geschlossen werden; ein clientseitiger Sichttest dieser Race Condition bleibt Pflicht.

## Reward

Alle Auszahlungen laufen durch `DZATM_RewardProvider`. Standard ist `ExpansionBanknoteEuro` mit
`PayoutUnitValue = 100`; `ExpansionBanknoteUSD` kann per Config eingestellt werden. Bei einem
partiellen Spawnfehler werden die in diesem Grant bereits erzeugten Objekte geloescht und der Grant
als fehlgeschlagen behandelt.

Eine BankingZ-Anbindung ist absichtlich nicht enthalten. Sie wird spaeter als deklarierter Adapter
gegen die oeffentliche BankingZ-API gebaut, nicht per Reflection oder String-Hack.

## Persistence

Cooldown-Dateien Version 2 speichern `ExpiresAtUTC`. Alte Version-1-Eintraege mit
`RemainingSeconds` werden einmalig zu einem UTC-Ablaufzeitpunkt migriert. Neustarts pausieren den
Cooldown damit nicht mehr.

ATM-IDs bestehen aus Map, ATM-Klasse, auf 0,1 Meter normalisierter XYZ-Position und auf 0,1 Grad
normalisierter Yaw-Orientierung. Runtime-Object-IDs werden nicht verwendet.

