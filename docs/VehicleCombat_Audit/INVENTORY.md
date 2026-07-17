# Vehicle Combat – Read-only Inventory

Audit date: 2026-07-17

Workshop item: `3738236783`

Audited path: `C:\Program Files (x86)\Steam\steamapps\common\DayZ\!Workshop\@Vehicle Combat`

## Scope and legal boundary

The Workshop directory was inspected read-only. It is a junction to
`C:\Program Files (x86)\Steam\steamapps\workshop\content\221100\3738236783`.
No Workshop file was changed, copied into the DeutschZ repository, extracted,
unpacked, unbinarized or decompiled.

The official Workshop description states **All rights reserved**, prohibits
reproduction, distribution and modification without explicit written
permission, and separately prohibits repacking. A later sentence permits the
unchanged mod to be downloaded and used directly on a server. This grants direct
use, not source access, modification, decompilation or redistribution. The PBO
contents were therefore not opened. This audit distinguishes between:

- **OPEN EVIDENCE**: root files delivered outside the PBOs and the official
  Workshop metadata;
- **AUTHOR CLAIM**: functionality described by the author but not verified in
  code or a runtime test;
- **NOT VERIFIED**: cannot be proven without opening protected content or
  conducting a separate runtime test.

License consequence: unchanged direct server use is permitted, while reuse,
adaptation, source inspection/decompilation or repacking is not licensed.
An independently designed clean-room DeutschZ implementation remains a separate
option, provided it uses no foreign code, assets, animations or config.

## Root metadata

`meta.cpp` declares:

- protocol: `1`
- published ID: `3738236783`
- name: `Vehicle Combat`

`mod.cpp` exposes a root patch/mod named `VehicleCombatBasic`, an empty root
`requiredAddons[]`, and the tags `Vehicles`, `Combat`, `Shooting`. This root
config is not proof that the configs inside the PBOs have no dependencies.

The official Steam metadata reports:

- title: `Vehicle Combat`
- creator Steam ID: `76561198052623689`
- creator/consumer app: `221100` (DayZ)
- public visibility
- advertised minimum: DayZ `1.29` or newer
- advertised dependencies: none
- advertised incompatibility: mods that replace/modify animation files,
  explicitly including SurvivorAnimations and ExpansionAnimations
- advertised Sgunplay compatibility for pistol fire

## Visible file inventory

| Relative path | Bytes | UTC modified | SHA-256 |
|---|---:|---|---|
| `Addons/0_VehicleCombatBasicDefines.pbo` | 1,121 | 2026-07-16 21:08:05 | `3C1406FF245369FCB1AC6F1F15C0257CB590F6181D23EBD1E80199BC4DD1BF95` |
| `Addons/0_VehicleCombatBasicDefines.pbo.ESMods.bisign` | 559 | 2026-07-16 21:08:05 | `784BF5415A65E15EB106BAD73D6EE201DDAA19D0AADED550021D21A9641980CA` |
| `Addons/Player_main.pbo` | 1,667,594 | 2026-07-16 21:08:04 | `773BCE8C81B721A6E927A6B31F315F33458D950E9FD239C62E90360694D767A7` |
| `Addons/Player_main.pbo.ESMods.bisign` | 559 | 2026-07-16 21:08:05 | `8B9829F1D188E8B85964288B7B9E3551328FBC2415E72E4899961219344CA9E2` |
| `Addons/VehicleBandage.pbo` | 61,595,083 | 2026-07-16 21:08:04 | `27908864E067620671DB3744656CADFE32ADD2E26C92274310E7E3316F25C437` |
| `Addons/VehicleBandage.pbo.ESMods.bisign` | 559 | 2026-07-16 21:08:08 | `B5EF2CE5EA6E344A9A1F07F59B1C31A7D80D10B13E506D2356B0C7AFD11874DF` |
| `Addons/VehicleCombatBasic.pbo` | 17,736,636 | 2026-07-16 21:08:04 | `72F11C56D846C999785DBF7E7D63E449505F3B3759D8B1E239C9B28D895A3B3D` |
| `Addons/VehicleCombatBasic.pbo.ESMods.bisign` | 559 | 2026-07-16 21:08:09 | `3E05CD5BA66A7709487CD6BE628FDB56D2AB17FF4D99B94EF581C9A13301D186` |
| `Keys/ESMods.bikey` | 159 | 2026-07-16 21:08:07 | `7E74B8DA2D28259DCC8DFD832D7A66AC510CD94E98242C331F7E83D33FEFBEA8` |
| `config.Jsonexample` | 3,694 | 2026-07-16 21:08:09 | `FBB2A6B92D7174C3250054626E762BE303535F6FC135DC77D52D38936CA65417` |
| `meta.cpp` | 102 | 2026-07-16 21:08:09 | `FBC08D60F33F6C220073E29D5F4B1DF0CA68D9D4024BA8114C0C99FA7FF46833` |
| `mod.cpp` | 548 | 2026-07-16 21:08:09 | `0B3BEC20BF220F148F2EBD3F8573DED8DD03D5707A68F78984C3E0EA710686D0` |
| `README_CONFIG.md` | 9,706 | 2026-07-16 21:08:07 | `33E3BD70882FACA0AA52A2FFFB049BB3FE3C866CA2A317A165D235C2FEB0C8B8` |

All four visible PBOs have matching-name `.ESMods.bisign` files and one public
`ESMods.bikey` is delivered. A local signature verification utility was not
available at the checked standard paths, so cryptographic verification is
**not performed**.

## Open configuration surface

The delivered example and README advertise a generated profile file at
`$profile:\VehicleCombatBasic\config.json`. Visible settings are:

- global: `Enabled`
- weapon families: `AllowPistols`, `AllowRevolvers`
- vehicle flags: `AllowCIVVAN`, `AllowV3S`, `AllowSEDAN`,
  `AllowHATCHBACK`, `AllowBUS`, `AllowS120`, `AllowMULTICAR`, `AllowGOLF`,
  `AllowHMMWV`, `AllowZODIAC`
- seat flags: driver, co-driver, rear-left, rear-right
- firing and weapon actions: firing, attach/detach/swap magazines, load one or
  multiple bullets, eject, unjam and empty magazine
- inventory/general actions: inventory, food/drink, crafting, unpack/open,
  liquid transfer/pour and heat pack
- self-medical actions: epinephrine, morphine, generic injection, blood draw,
  blood administration and saline

The vehicle flag names are configuration labels, not verified DayZ config
class names. No RPC, input, layout, animation graph, script class or internal
`CfgPatches` inventory was produced because those would require opening PBOs.

## Visible risk indicators

- `Player_main.pbo` is a generic PBO name and may increase collision/load-order
  risk with other animation/player mods. Internal prefixes are not inspected.
- `VehicleBandage.pbo` is approximately 61.6 MB and the author advertises new
  vehicle bandage animations; the actual asset/config contents remain closed.
- The author explicitly warns that other animation-file mods are incompatible.
- Root `requiredAddons[] = {}` cannot establish internal dependency order.
- The advertised profile path does not follow the DeutschZ profile convention.

## Audit boundary result

The visible inventory is complete for files outside the PBOs. Internal layouts,
animations, inputs, RPCs, vehicle references, weapon references and seat logic
remain **NOT VERIFIED** by design and license.

## Supplementary folder check: `@DeutschZ_ShootingZ`

Patrick additionally supplied the local folder
`C:\Users\patsl\Desktop\@DeutschZ_ShootingZ` and authorized PBO inspection.
The folder itself contains no independent `mod.cpp`, `meta.cpp`, README or
license. Its four numbered subdirectories contain:

| Directory | Files | Bytes | Visible file types |
|---|---:|---:|---|
| `1` | 5 | 2,345 | PBO, config/script-looking files and extraction metadata |
| `2` | 143 | 3,328,288 | PBO, animation graph/index/state files and extraction metadata |
| `3` | 1 | 61,595,083 | PBO only |
| `4` | 1 | 17,736,636 | PBO only |

SHA-256 comparison proves that all four PBOs are byte-identical to the protected
Workshop item:

| Desktop PBO | Matching Workshop PBO | Result |
|---|---|---|
| `1/0_VehicleCombatBasicDefines.pbo` | `Addons/0_VehicleCombatBasicDefines.pbo` | identical |
| `2/Player_main.pbo` | `Addons/Player_main.pbo` | identical |
| `3/VehicleBandage.pbo` | `Addons/VehicleBandage.pbo` | identical |
| `4/VehicleCombatBasic.pbo` | `Addons/VehicleCombatBasic.pbo` | identical |

Directories `1` and `2` therefore do not establish an independent DeutschZ
source or a new license basis; they visibly contain unpacked artifacts associated
with the same foreign PBOs. Patrick's authorization permits access to his local
folder, but it cannot grant rights held by the Workshop author. The unpacked
config, script and animation contents were not read or copied. This supplementary
check does not change the audit boundary or the clean-room recommendation.

## Evidence sources

- local open files in Workshop item `3738236783`;
- official Steam Workshop metadata API response for published file
  `3738236783`;
- public Workshop page:
  `https://steamcommunity.com/sharedfiles/filedetails/?id=3738236783`;
- vanilla DayZ 1.29 script references under `P:\scripts` (read-only).
