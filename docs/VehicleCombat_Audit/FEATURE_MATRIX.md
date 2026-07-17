# Vehicle Combat – Feature Matrix

Evidence levels:

- **Claimed**: explicitly stated in the official Workshop description or the
  delivered open README/example config.
- **Not stated**: no open evidence.
- **Not verified**: no PBO/code inspection and no runtime test were performed.

## Combat, seats and vehicles

| Capability | Open evidence | Audit result |
|---|---|---|
| Shoot as driver | Driver seat is configurable and advertised as supported | **Claimed**, but author also states firing mode blocks vehicle movement; simultaneous driving and firing is not supported |
| Shoot as co-driver | Co-driver seat advertised/configurable | **Claimed**, not runtime-verified |
| Shoot from rear-left/right seats | Both rear seats advertised/configurable | **Claimed**, not runtime-verified |
| Open vehicles | Zodiac and some truck/off-road variants advertised | **Claimed by vehicle list**, no per-proxy verification |
| Closed vehicles | Sedans, hatchbacks, van and bus advertised | **Claimed**, no cabin/window/muzzle validation verified |
| Modded vehicles | Any modded vehicle inheriting a vanilla vehicle class is advertised | **Claimed**; this is too broad to treat as technically guaranteed because proxies, seat counts, animations and geometry can differ |
| Vehicle inventory | Extra shoulder, pistol and back slots plus an interior category advertised for vanilla vehicles | **Claimed**, not code-verified |
| Driver movement while firing | Author states it is blocked | **Explicit limitation** |

Advertised concrete/high-level vehicle names are internally inconsistent across
the description and example config. The description names Olga, Sedan,
Hatchback, Offroad, Military Offroad, trucks and Zodiac, then lists
`CivilianSedan`, `Sedan_02`, `Hatchback_02`, `ShuttleOffHatchback`, `SUV_02`,
`Bus_01`, `Van_01`, `Truck_01_Base`, `Truck_01_Covered`, `Truck_02` and
`Zodiac`. The config instead exposes logical flags such as CIVVAN, V3S, S120,
MULTICAR, GOLF and HMMWV. Exact class-to-flag mapping is **NOT VERIFIED**.

## Weapons and weapon handling

| Capability | Open evidence | Audit result |
|---|---|---|
| Pistols | Allowed and configurable | **Claimed supported** |
| Revolvers | Allowed and configurable | **Claimed supported** |
| SMGs | No support advertised | **Not supported/not stated** |
| Shotguns | No firing support advertised | **Not supported/not stated** |
| Repeating/bolt-action rifles | Author states rifles cannot be fired, only carried/manipulated/reloaded | **Firing not supported** |
| Assault rifles | Covered by rifle restriction | **Firing not supported** |
| LMGs | No support advertised | **Not supported** |
| Hipfire | Shooting is advertised, but fire mode/camera type is not specified | **Not verified** |
| ADS/ironsights/optics | Not documented | **Not verified** |
| Reloading | Magazine and bullet operations are advertised | **Claimed supported** |
| Magazine attach/detach/swap | Explicit settings and documentation | **Claimed supported** |
| Chamber/eject/unjam | Explicit settings and documentation | **Claimed supported** |
| Weapon switching | Inventory access and weapon slots are advertised, but switching while in fire mode is not documented | **Not verified** |

## Camera, animation, ballistics and state handling

| Capability | Open evidence | Audit result |
|---|---|---|
| Passenger animations | Custom idle animation for passenger seats advertised | **Claimed** |
| Driver animation | Original driver animation retained | **Claimed** |
| Bandaging animation | New in-vehicle animation advertised | **Claimed** |
| Camera behavior | No precise first-/third-person or ADS behavior documented | **Not verified** |
| Muzzle point | No exposed implementation | **Not verified** |
| Own-vehicle obstruction | Author says obstruction belonging to the current vehicle is ignored | **Claimed; security/geometry risk**, because ignoring the entire vehicle can allow shooting through closed bodywork |
| World obstruction | No exposed implementation | **Not verified** |
| Hit registration | No exposed implementation or test evidence | **Not verified** |
| Network synchronization | No exposed RPC/state description | **Not verified** |
| Speed restriction | Movement is said to be blocked during fire mode; no numeric speed setting exists in the visible config | **Strong restriction, not configurable** |
| Seat switching | No explicit behavior | **Not verified** |
| Enter/exit during fire mode | No explicit behavior | **Not verified** |
| Death/unconsciousness | No explicit behavior | **Not verified** |
| Inventory during firing | Inventory is advertised to close automatically | **Claimed** |
| Quickbar during firing | Quickbar use advertised as blocked | **Claimed** |

## Non-combat functions advertised

- inventory access while seated;
- food and drink consumption;
- liquid transfer/pour;
- self-medical actions including bandaging, injections, saline and blood;
- crafting while seated;
- opening/unpacking selected items;
- vehicle-vicinity filtering so only the current vehicle is visible;
- prevention of dismantling vehicle parts from inside.

These functions extend far beyond vehicle combat and enlarge compatibility and
exploit surface. None was runtime-tested in this audit.

## Dependencies and restrictions

| Item | Result |
|---|---|
| Hard dependencies | Author claims none; root `requiredAddons[]` is empty; internal PBO dependencies are not verified |
| Required DayZ version | Author claims 1.29+ |
| Animation compatibility | Explicitly incompatible with SurvivorAnimations, ExpansionAnimations and other animation-file modifiers |
| Sgunplay | Compatibility with pistol shooting is claimed |
| Expansion | No hard dependency advertised; ExpansionAnimations is explicitly named as conflicting |
| Repack/modification | Prohibited by the author/license text |

## Anti-cheat and exploit assessment

Without code access the actual validation cannot be assessed. The highest-risk
areas for any implementation are:

1. client-only enabling of fire mode;
2. spoofed vehicle, seat or weapon class;
3. firing while above the allowed speed;
4. firing while entering, exiting, switching seats, dead or unconscious;
5. ignoring the complete vehicle in raycasts, enabling shots through doors,
   roof, dashboard or rear bodywork;
6. aim directions outside a seat-specific cone;
7. duplicated magazine/reload requests during inventory transitions;
8. muzzle and vehicle transforms disagreeing between client and server while
   the vehicle moves;
9. action-state conflicts caused by animation replacements.

No claim of safe server-authoritative validation can be made for the Workshop
mod from open evidence alone.
