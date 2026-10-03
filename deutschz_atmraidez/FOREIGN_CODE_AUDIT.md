# Foreign/Legacy Code Audit

Ausgangspunkt war `deutschz_criminalz`.

Bewusst NICHT uebernommen:
- VehicleModule / Fahrzeug-Hacks
- GarageModule / Garage-Hacks
- `HackingTool_Lockpick`
- `CriminalZ_Token`
- feste CriminalZ ATM-Positionen und eigene ATM-Spawns
- StoreInfoSign
- CriminalZ-spezifische Branding-Pfade
- alte `DZCRZ_*` RPCs und Namespaces

Bewusst als Logikvorlage uebernommen und umgebaut:
- zweiphasiger ATM-Raub
- Cooldown Manager
- Marker / Alarm / Client Effects
- Progress UI
- serverseitige Reward-Auszahlung

Pflicht fuer Codex:
- Scan auf `DZCRZ`, `CriminalZ`, `LBMaster`, `TBLib`, `TBRealEstate`, fremde APIs, alte RPC IDs.
- Jeden verbliebenen Treffer bewerten.
- Keine fremde Funktion nur kosmetisch umbenennen.
