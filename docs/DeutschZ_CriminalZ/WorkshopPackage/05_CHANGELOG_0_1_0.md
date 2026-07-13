# Changelog – Version 0.1.0

## Added

- durchgehende ATM-Hackaktion mit 300–600 Sekunden Zufallsdauer
- serverautoritatives Session-, Distanz-, Werkzeug- und Reward-System
- 45.000–185.000 NoxZ-Euro-Auszahlung; Bitcoin technisch blockiert
- rote Expansion-Marker, globale Meldungen und räumliche Alarme
- persistente ATM-Cooldowns
- Expansion-Fahrzeug-Unlock über verifizierte API
- automatische Store-Erkennung für Land_City_Store und Land_City_Store_WithStairs
- ExpansionATM_2 und DZCRZ_StoreInfoSign je Store
- überarbeitete DeutschZ_a_fuelstation_sign_co.paa
- JSON-Konfiguration, Runtime-Status, Logs und Adminbefehle

## Fixed

- Placement-Fallback wird auch nach früher House.EEInit-Initialisierung zuverlässig einmal geplant
- bestehende StorePlacement-JSON wird vom Vanilla-Schild auf DZCRZ_StoreInfoSign migriert
- P3D-Material- und Diffuse-Pfade auf das CriminalZ-Schildasset gebunden

## Known Issues / Open Tests

- allgemeiner Expansion-Garage-Unlock bleibt mangels sicherer API deaktiviert
- Client-ATM-Menü, kompletter Hacklauf, Zwei-Client-Sichtbarkeit und Map-Loader-Latespawn müssen live getestet werden
