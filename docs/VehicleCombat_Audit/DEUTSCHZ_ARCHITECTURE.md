# DeutschZ_VehicleCombatZ – Clean-room Architecture Proposal

Status: design only; no implementation was created.

## Legal and design boundary

`DeutschZ_VehicleCombatZ` must be a clean-room project. Do not copy, extract,
translate, adapt or use any script, config, animation, asset or internal naming
from Workshop item `3738236783`. The open feature description may be treated as
a problem statement, not as implementation material.

## Recommended product scope

### MVP v1

- selected vanilla vehicles only;
- co-driver and rear passenger seats only;
- pistols and revolvers only;
- hipfire only;
- vehicle stationary or below a conservative configurable speed;
- reload/magazine actions only while the weapon FSM is idle;
- per-vehicle/per-seat aim cones and safe muzzle aperture;
- server-authoritative enable/fire eligibility;
- no custom UI beyond a minimal action/status message;
- no foreign animation assets.

### Later gated extensions

- SMGs after weapon-length and animation tests;
- shotguns/rifles/assault rifles only for explicitly profiled open seats;
- ADS only for verified seat/camera profiles;
- driver fire only as a separate experimental feature;
- modded vehicles only through explicit profiles, never inheritance alone.

## Profile paths

Config:
`$profile:DeutschZ-System/DeutschZ_VehicleCombatZ/VehicleCombatZSettings.json`

Logs:
`$profile:DeutschZ-System/LogZ/DeutschZ_VehicleCombatZ/VehicleCombatZ.log`

Both directories are created safely on the server. Paths are centralized as
constants and never replaced by absolute host paths.

## Suggested script separation

```text
scripts/
├─ 3_Game/DeutschZ/VehicleCombatZ/
│  ├─ DZVCZ_Constants.c
│  ├─ DZVCZ_Types.c
│  ├─ DZVCZ_RPCs.c
│  └─ DZVCZ_Settings.c
├─ 4_World/DeutschZ/VehicleCombatZ/
│  ├─ DZVCZ_ServerController.c
│  ├─ DZVCZ_SeatProfileResolver.c
│  ├─ DZVCZ_WeaponPolicy.c
│  ├─ DZVCZ_MuzzleValidator.c
│  ├─ DZVCZ_CombatSession.c
│  └─ DZVCZ_Log.c
└─ 5_Mission/DeutschZ/VehicleCombatZ/
   ├─ DZVCZ_ClientController.c
   ├─ DZVCZ_InputBridge.c
   └─ modded_MissionGameplay.c
```

Animation/config assets, if later required, must be original DeutschZ work and
kept in their own clearly named PBO to reduce conflicts.

## Settings model

The requested top-level settings should be retained and normalized:

- `AllowedVehicleClasses`
- `AllowedSeatIndices`
- `AllowedWeaponCategories`
- `AllowedWeaponClasses`
- `AllowDriverFire`
- `AllowPassengerFire`
- `AllowRearSeatFire`
- `AllowADS`
- `AllowReload`
- `MaxVehicleSpeedForFire`
- `MuzzleCollisionCheck`
- `FriendlyFireRules`
- `Cooldowns`
- `Logging`

Universal seat arrays are insufficient. Add explicit profiles:

```json
{
  "SchemaVersion": 1,
  "Enabled": true,
  "AllowDriverFire": false,
  "AllowPassengerFire": true,
  "AllowRearSeatFire": true,
  "AllowADS": false,
  "AllowReload": true,
  "MaxVehicleSpeedForFire": 10.0,
  "MuzzleCollisionCheck": true,
  "AllowedWeaponCategories": ["Pistol", "Revolver"],
  "AllowedWeaponClasses": [],
  "VehicleProfiles": [
    {
      "VehicleClass": "<verified vanilla class>",
      "IncludeInheritedClasses": false,
      "Seats": [
        {
          "SeatIndex": 1,
          "Role": "CoDriver",
          "AllowFire": true,
          "AllowADS": false,
          "AimYawMin": -70.0,
          "AimYawMax": 20.0,
          "AimPitchMin": -25.0,
          "AimPitchMax": 45.0,
          "AllowedWeaponCategories": ["Pistol", "Revolver"]
        }
      ]
    }
  ],
  "Cooldowns": {
    "EnableRequestMs": 500,
    "RejectedRequestLogMs": 2000
  },
  "Logging": {
    "Enabled": true,
    "RejectedRequests": true
  }
}
```

The sample values are design placeholders, not tested class/aim values.

## Server-authoritative flow

1. Client requests combat mode.
2. Server resolves current transport and authoritative crew index.
3. Server resolves an exact vehicle/seat profile.
4. Server validates player state, weapon, speed and transition state.
5. Server grants a short-lived combat session and synchronizes only the needed
   state.
6. Each relevant fire/reload transition is revalidated; a prior grant is not a
   permanent permission.
7. Any invalid state cancels the session and restores camera/input/weapon state.

No client-provided seat, speed, weapon class or muzzle position is trusted.

## Muzzle and collision policy

`MuzzleCollisionCheck` must not mean “ignore the current vehicle”. It should:

- transform verified seat aperture data with the current transport transform;
- ensure the weapon/muzzle is inside the allowed aperture volume;
- validate the direction against the configured aim cone;
- raycast to the muzzle and forward from the muzzle;
- treat closed cabin components as blockers;
- optionally allow explicitly identified window/opening components;
- fail closed when geometry/profile data is missing.

## Compatibility policy

- no hard CF or Expansion dependency;
- no dependency on another animation replacement;
- detect conflicting animation/config patches during preflight and document the
  load-order risk;
- do not claim automatic compatibility with inherited modded vehicles;
- exact class profiles are the default; inheritance is opt-in per profile;
- server and client must both load the mod because input, animation/camera and
  weapon state are client-visible.

## Test gates for a future implementation

1. script compile and dedicated server mission start;
2. one stationary passenger seat with one vanilla pistol;
3. muzzle blocked by closed door/body and allowed through verified opening;
4. server rejects wrong seat, wrong gun and excessive speed;
5. reload and magazine swap without duplication;
6. enter/exit/seat-switch cancellation;
7. death, unconsciousness, disconnect and vehicle deletion cleanup;
8. two-client moving-vehicle hit registration and desync test;
9. first-/third-person camera and aim-cone validation;
10. only then add further vehicles, seats or weapon categories.

## Architecture conclusion

A small, explicit, server-authoritative clean-room mod is preferable to trying
to generalize the Workshop product. Configuration should select verified
vehicle/seat profiles, not promise universal compatibility.
