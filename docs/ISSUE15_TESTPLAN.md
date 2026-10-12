# Issue 15: operator handover and evidence

## Scope
Only TEST @Schwanz is deployed. LIVE FTP was read-only. No server start/restart, no Workshop upload, no local production @Schwanz replacement.

## Proven inputs
Live starts 2026-10-11 23:19:54 / script 23:20:27 and 2026-10-12 01:00:16 / script 01:00:39, downloaded by FTP. server_console.log is the current remote console, not a fabricated copy of the operator's renamed download. Raw logs and server settings remain outside Git.
Each RPT has 695 unable-to-create-magazine lines and 27 ignored CE entries. CONVOY keeps polling every 10s while KOTH stays ACTIVE/WAITING_FOR_PLAYER without participants. WorldMemoriez READY regions=1 describes lazy stored activity cells, not a failed world-grid loader. CarRaid ledger operator-test-001 is ABORTED; refusing its reuse is correct duplicate prevention.

## Changes
- Scheduler: KOTH expires only after 900 continuous seconds waiting with zero valid human participants. Capture/boss/reward are protected. Both installed KOTH implementations clean up before releasing their lease. New runs reset the idle clock. Fixed KOTH -> COURIER -> RAVEN -> CONVOY rotation retained.
- Restore/provider readiness no longer replaces a persisted UTC deadline with now. Reducing maximum gap configuration cannot shorten a restored deadline. Provider generation and scheduled-owner freeze remain intact.
- CONVOY deferral messages only on changed blocker or every 300s; provider retries remain every 10s.
- WorldMemoriez seeds up to 256 valid bounded cells without overwriting restored values. Actual EEHitBy hooks add combat signals; shots, explosions, infected-kill pressure proxy, player kills and committed event/supply signals remain. Quiet cells cannot delay an isolated hot-region reaction. Reactions use native NoiseSystem, bounded checks/cooldown/lifetime, no extra infected spawns. Diagnostics limited to 16 sensor lines/tick and 16 region lines/save. Infected pressure is explicitly a kill/event proxy, not a measured living population.
- CarRaid immutable per-run consumed history precedes current ledger and entity creation. Committed rewards cannot be downgraded by a failed cleanup. New operator run command validates IDs, backs up config/ledger, retains consumed history, requires stopped-server acknowledgement and explicit cancellation for active runs. At-most-once is preferred over repeating loot after a disk failure.
- Weapon compatibility: installed Morty config arrays reference 29 IDs rejected by both live starts. Twenty directly defined weapon arrays remove only these IDs with array subtraction; existing valid magazines are retained and original requiredAddons enforce loading order. No invented magazine definitions and no third-party PBO rewrite. Removal is specific to the audited installed set; re-audit if missing magazine packs are later installed.
- TEST economy already lacks 24 of LIVE's 27 rejected entries. Remove only ChristmasTree (non-public CE type), OvenIndoor and missing Ammo_Anzio20mmTracer_Green from their exact TEST types files. All other type values are preserved. No economy/state wipe and no loot-balance changes.
- Latest previous FuelZ/PropertyZ/Operation fixes are included because TEST still held older PBOs.
- WitnessZ/Camouflage source/API audit found no proven new failure: current Expansion eAI_GetTargetInformation returns the selected target, and eAI_HasLOS operates on that target. Fresh raycasts and real hit hooks are retained. Native threat bypass for combat/high threat is preserved. Radio requires an intact, powered, tuned transmitter; zero eligible receivers alone proves no defect. RBM recovery retains real waypoint arrival, stop opening, closing and marker cleanup. Their baseline source contracts and existing regression suites are tested, without claiming gameplay success.

## Automated verification
Run python -m unittest discover -s tests -v. CI uses the same complete suite. Tests cover exact Enforce scalar return expressions, source contracts, persistence fault models and the actual Python operator migration. They DO NOT execute an Enforce VM. CfgConvert checks configs, AddonBuilder packs, BankRev readback compares source byte-for-byte, DSCheckSignatures checks all shipped signatures. Neither tool proves World/Mission script compilation.

## Manual TEST cases (pending operator start)
| Case | Procedure | Required evidence |
|---|---|---|
| Compile | Start TEST manually with existing modlist | No Can't compile / SCRIPT (E) / crash; READY lines for modules |
| KOTH empty | Leave waiting area empty for 900s | TIMEOUT no_player_wait_expired, CLEANUP_DONE, SLOT_RELEASE, ROTATION_ADVANCE to COURIER |
| KOTH participant | Enter before idle expiry; capture, leave/rejoin | No idle abort during capture/boss/reward; normal completion cleanup |
| Four rotations | Complete/timeout KOTH, COURIER, RAVEN, CONVOY twice | Exact order; no simultaneous owner, duplicate run or starvation |
| Restore plan | Operator stops during scheduled cooldown, then manually starts | SCHEDULE_RESTORED remaining UTC time, RESTART_READY preserve_deadline, no early GRANTED |
| Provider reload | Register fresh provider during restored plan | PROVIDER_REACQUIRED generation logged, same selected owner/deadline, single grant |
| World grid | Inspect initial GRID/READY | 256 valid sectors at default settings; old values retained; persistenceReadOnly=0 |
| World sensors | Walk, fire, explode, damage player/infected, kill infected, complete supply event | Correct SENSOR/REGION values, regional locality and caps; no second PVP kill count |
| World reaction | Repeated noise raises severity near a player | Bounded REACTION spawns=0, native infected investigate; no permanent target or serverwide alarm |
| World quiet/restart | Wait quiet/half-life interval; operator stops/starts | Values decay, revision resumes, no discarded compatible slot |
| CarRaid | Use new issue15 run, kill guards, hold capture, claim | Stages advance, loot once, cleanup; blocked while major active |
| CarRaid restart/reuse | Operator stops/starts after reservation/claim | RUN_CONSUMED; no respawn/duplicate reward. New run only via operator tool |
| Witness | Visible unmasked crime, then masked/obscured crime and delayed witness death | Only living conscious witness with real LOS reports; same-faction TTL memory only |
| Camo | Same AI/player route standing/prone, matching/mismatched clothes, night/day | Bounded suspicion changes before combat; no invisibility, no shot/combat suppression |
| Radio | Two players: powered correct channel vs off/wrong channel | Qualified count matches; only eligible player gets radio line; normal expiry if none |
| RBM | Observe travel, three stationary windows/recoveries, two stops | Actual arrival, OPEN -> closed -> travel, correct marker lifecycle and save |
| Magazines/CE | Compare fresh RPT and test representative weapons/unload rounds | Audited invalid IDs absent; valid magazines still attach/fire; exactly corrected CE entries absent |
| Ammo | Split/merge supported loose ammunition | Stack cap 60 retained; magazine capacities unchanged |

Use python tools/check_issue15_logs.py <RPT> <script.log> <crash.log> for a read-only summary. A clean startup is not a multiplayer AI gameplay pass.

## Remaining external warnings
Third-party missing fire-mode/OpticsInfoRifle/asset/model properties and map vegetation/physics warnings are recorded as external configuration or map issues. They are not silently turned into successful tests or fixed by inventing unknown classes. Full residual counts must be compared after the operator's new TEST start.
