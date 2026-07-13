# DeutschZ_CriminalZ

Version 0.1.0 implements the CriminalZ MVP and automatic City Store placements.

Runtime configuration:

- `$profile:DeutschZ-System/DeutschZ_CriminalZ/Config/DeutschZ_CriminalZ.json`
- `$profile:DeutschZ-System/DeutschZ_CriminalZ/Config/StorePlacementSettings.json`
- `$profile:DeutschZ-System/DeutschZ_CriminalZ/Data/atm_positions.json`
- `$profile:DeutschZ-System/DeutschZ_CriminalZ/Persistence/cooldowns.json`
- `$profile:DeutschZ-System/DeutschZ_CriminalZ/Runtime/runtime.json`
- `$profile:DeutschZ-System/LogZ/DeutschZ_CriminalZ/criminalz.log`

The automatic placement uses `House.EEInit`, delayed processing and one fallback world scan. It does not maintain a coordinate list for City Stores.

`DZCRZ_StoreInfoSign` is the active default. It uses the supplied `DeutschZ_a_fuelstation_sign_co.paa` on the fuel-station sign model; the original normal/specular textures remain in use.
