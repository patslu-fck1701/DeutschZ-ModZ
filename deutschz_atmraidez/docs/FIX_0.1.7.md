# ATM RaideZ 0.1.7 DeutschZ-ATM-Textur

## Quelle

`deutschz_expansion_atm_02_co.paa` aus dem vom Owner angegebenen CriminalZ-Bestand.

## Umsetzung

- Die vorhandene Klasse `ExpansionATM_2` bleibt erhalten.
- `EEInit()` setzt die Textur auf Slot 0 (`zbytek`) und Slot 1 (`screen`).
- Der Aufruf laeuft auch clientseitig, damit neu beitretende Spieler die Textur erhalten.

## Offener Test

- Sichttest im Client an mehreren der 16 ATMs.
- Pruefen, dass Gehaeuse und Bildschirm korrekt belegt und nicht verzerrt sind.
