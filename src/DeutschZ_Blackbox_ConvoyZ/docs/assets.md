# DeutschZ Blackbox ConvoyZ Assets

Integrated from `E:\NewItems\PAA\ConvoyZ`:

- `cardreader_gpsreceiver_co.paa` -> terminal/datacore texture
- `convoyblackbox_loot_protectorcase_co.paa` -> blackbox texture
- `logogreen_sea_chest_co.paa` -> reward container texture
- `datacore_punchedcard_co.paa` -> extracted ConvoyZ data card
- `deutschz_matchbox_co.paa` -> coded signal matches
- `deutschz_9v_co.paa` -> coded 9V battery
- `deutschz_road_flare_co.paa` -> marked gas-zone road flare

Runtime notes:

- The coded battery and matches use the exact vanilla 1024 x 1024 UV and alpha
  masks. Their original non-power-of-two exports were invalid for reliable DayZ
  rendering.
- `models/dzbbc_datacore.p3d` is a vanilla PunchedCard model with only its
  diffuse texture path redirected. `d/dc.paa` is the required short-path runtime
  copy of the canonical DataCore texture; both files have identical content.

Story item classnames:

- `DZBBC_SignalMatches` (ConvoyZ end-container, cannot ignite normal fires)
- `DZBBC_Coded9VBattery` (ConvoyZ end-container)
- `DZBBC_DataCore` (temporary Blackbox transport item)

No source asset was deleted or modified.

Missing optional assets:

- custom audio files for the 30 second fragment
- custom particle files for crash smoke/fire
- dedicated HUD icon PAA files

The current implementation uses safe vanilla RPC/Print/Chat fallbacks for those effects.
