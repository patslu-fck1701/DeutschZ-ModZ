# DeutschZ Blackbox ConvoyZ Assets

Integrated from `E:\NewItems\PAA\ConvoyZ`:

- `cardreader_gpsreceiver_co.paa` -> terminal/datacore texture
- `convoyblackbox_loot_protectorcase_co.paa` -> blackbox texture
- `logogreen_sea_chest_co.paa` -> reward container texture
- `datacore_punchedcard_co.paa` -> extracted ConvoyZ data card
- `deutschz_matchbox_co.paa` -> coded signal matches
- `deutschz_9v_co.paa` -> coded 9V battery

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
