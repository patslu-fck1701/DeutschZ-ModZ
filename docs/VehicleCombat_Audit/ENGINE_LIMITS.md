# Vehicle Combat – DayZ Engine Limits

This document analyzes vanilla DayZ 1.29 script references at `P:\scripts`.
It does not describe or reverse-engineer the Workshop PBO implementation.

## 1. Vehicle command is a distinct player command

`DayZPlayerImplement.c` handles `COMMANDID_VEHICLE` and returns immediately after
vehicle-specific handling (`P:\scripts\4_World\DayZ\Entities\DayZPlayerImplement.c`,
around lines 2418–2428). Normal movement/weapon command processing is therefore
not automatically equivalent while seated. The author’s public statement that
firing mode blocks vehicle movement is consistent with this command separation.

`HumanCommandVehicle` must be retrieved fresh. Vanilla itself warns not to store
it for later use (`DayZPlayerImplementVehicle.c`, lines 3–4). A custom system must
not cache the command across transitions.

## 2. WeaponManager prerequisites

Vanilla `WeaponManager.CanFire` requires:

- the weapon is in hands;
- the weapon is raised;
- no lift/obstruction state;
- weapon is not destroyed;
- inventory is not processing;
- weapon raise is complete;
- player is not fighting;
- weapon is not cooling down.

Reference: `P:\scripts\4_World\DayZ\Classes\Weapons\WeaponManager.c`, lines
79–88. A seated implementation must produce a valid raised-weapon state and
weapon animation while respecting inventory FSM and cooldowns; merely listening
for the fire input is insufficient.

Vanilla `PlayerBase.CanReceiveItemIntoHands` returns false while the player is in
a vehicle (`PlayerBase.c`, lines 2103–2111). Weapon switching and inventory
transfers therefore require narrowly scoped handling and robust rollback, or
they risk duplication and stuck inventory reservations.

## 3. Seats, proxies and animations

`Transport` exposes `CrewSize`, `CrewPositionIndex`, `CrewMemberIndex`,
`CrewMember`, `CrewDriver`, model/world crew transforms and per-seat animation
type. It also exposes reachability checks for seats and doors. These are
vehicle-specific contracts, not proof that every inherited modded vehicle has
compatible proxies or geometry.

Relevant reference:
`P:\scripts\3_Game\DayZ\Vehicles\Transport.c`, lines 111–149 and 475–529.

Consequences:

- seat index alone is not a safe universal profile;
- each vehicle/seat needs a verified aim origin, allowed aim cone and animation;
- a modded vehicle inheriting a vanilla class can still change crew capacity,
  proxies, doors, model origin or cabin geometry;
- open and closed seats need different collision rules;
- custom passenger animations can conflict at animation graph/config level,
  matching the Workshop author’s incompatibility warning.

## 4. Camera and ADS

Vanilla selects dedicated first- and third-person vehicle cameras through the
vehicle command and `Transport.Get3rdPersonCameraType`
(`DayZPlayerImplement.c`, lines 2849–2868).

Vanilla ADS requires a raised weapon, completed raise, non-obstructed state and
a valid `HumanCommandWeapons`; it exits sights if those conditions fail
(`DayZPlayerImplement.c`, lines 1870–1999). In-vehicle ADS is therefore not a
simple configuration flag. It requires coherent camera, animation, aim limits,
weapon obstruction and command integration. Optics increase clipping and camera
misalignment risk inside a cabin.

Recommended boundary:

- MVP: third-/first-person hipfire only, no optics;
- ADS only after per-seat camera and aim-cone tests;
- no universal ADS promise for closed cabins or rear seats.

## 5. Long weapons and geometry

Weapon length is not represented by a single reliable script-side number that
can guarantee cabin clearance. A valid muzzle can still be outside the window
while stock, hands or optics intersect the seat, roof or door. Long guns require:

- per-vehicle and per-seat whitelisting;
- an aim cone;
- a safe muzzle volume/window aperture;
- raycast from player/weapon origin to muzzle;
- raycast from muzzle forward;
- explicit handling of the current vehicle’s collision components.

Blindly ignoring the entire current vehicle is unsafe: it makes closed bodywork
non-blocking and enables shooting through vehicle parts. Conversely, treating all
vehicle geometry as blocking can make legitimate window fire impossible. A clean
implementation needs seat-specific safe apertures or component-aware checks.

## 6. Moving transforms, hit registration and desync

The seat and muzzle are attached to a moving, network-interpolated transport.
Client camera, client weapon transform and authoritative vehicle transform can
differ during latency, turning and suspension motion. Risks include:

- muzzle starting inside geometry on one peer but outside on another;
- shots originating behind/ahead of the rendered muzzle;
- aim direction diverging from camera crosshair;
- delayed seat/speed state allowing one invalid shot;
- rapid vehicle rotation magnifying positional error.

The server must validate current transport, crew index, transition state,
weapon, ammo, speed and permitted aim cone at fire time. Client requests may
express intent but cannot decide eligibility.

## 7. Speed and driver limitations

Allowing a driver to steer and fire combines two input/animation domains that
vanilla separates. A speed limit alone does not solve steering, gear changes,
camera ownership and hand animation conflicts. The safest progression is:

1. passengers only;
2. stationary vehicles;
3. low-speed passengers;
4. driver fire only after dedicated control and animation tests.

For the initial DeutschZ MVP, driver fire should be disabled. If later enabled,
the server must validate actual velocity, not a client-reported speed.

## 8. Seat change, entry/exit, death and unconsciousness

Vanilla delays unconsciousness while entering, exiting or switching seats and
caches the transport to restore the correct vehicle state
(`PlayerBase.c`, around lines 3128–3157; `DayZPlayerImplement.c`, around lines
2370–2378). A vehicle-combat state must cancel before these transitions and must
not leave a weapon modifier, camera, input lock or RPC session active.

Mandatory cancellation conditions:

- begin enter/exit/switch-seat transition;
- seat or transport changes;
- player dies or becomes unconscious;
- weapon leaves hands or changes class;
- vehicle exceeds allowed speed;
- inventory/weapon FSM becomes busy;
- vehicle is destroyed or deleted;
- disconnect or mission end.

## 9. Server-authoritative security requirements

At minimum, every enable/fire/reload request must validate:

- authenticated `PlayerIdentity` and matching controlled player;
- current `HumanCommandVehicle` and `Transport` instance;
- authoritative crew index and allowed seat profile;
- allowed vehicle and weapon class/category;
- alive, conscious and not transitioning;
- authoritative velocity below the configured limit;
- weapon in hands, valid chamber/magazine state and idle weapon FSM;
- aim direction within the seat-specific cone;
- muzzle start and forward path clear under component-aware collision rules;
- per-player cooldown/rate limit;
- friendly-fire policy where applicable.

Log rejected requests by reason without logging per-frame aim data.

## Engine conclusion

Vehicle shooting is technically possible only by deliberately bridging systems
that vanilla keeps separate. A universal “all vehicles, all seats, all guns,
ADS and driving simultaneously” implementation is not robust. Pistols/revolvers
from selected passenger seats are realistic; long weapons require individual
vehicle/seat profiles and purpose-built animations.
