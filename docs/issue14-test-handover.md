# Issue 14: operator compile test

The three independent modules are a first test implementation. No gameplay PASS is claimed by the offline suite or PBO builder. The operator starts the stopped test server. Codex must not start, restart or use RCON.

## Installed API audit

The implementation was checked against the test server's installed Expansion `ai_scripts.pbo`, not only the current upstream branch. Used APIs include `eAIPlayerTargetInformation.CalculateThreat`, `eAITargetInformationState.m_SearchOnLOSLost`, `eAIBase.eAI_GetAll`, `eAI_HasLOS`, `eAI_GetTargetInformation`, faction names, daylight visibility and aggression timestamps. Native DayZ source was checked for `EEHitBy`, bullet raycasts, movement state, surface type, action registration, synchronized vehicle variables and entity lifetime. Third-party source is not included in this repository.

The main and archival branches contain roadmap references to CarRaidZ, but no binding full gameplay design. Existing ATM RaidZ, CourierZ AI spawning and EventSchedulerZ ownership APIs were audited. The proposed CarRaidZ flow below is therefore explicitly a configurable operator test, not an existing production specification.

AIConvoyZ, KOTH, CourierZ and RAVEN retain their existing scheduler ownership. CarRaidZ reads `HasActiveMajorEvent()` and yields; it never acquires a major slot or changes rotation. ToxicZ personal stories and WorldMemoriez inputs are unchanged. WitnessZ and camouflage use the installed Expansion target chain, so they also affect Expansion-controlled convoy/event AI according to normal faction and combat rules.

## Package and start arguments

For the compile-only test, the three PBOs are placed under `@Schwanz/Issue14/addons`, signed with `DZ_Neu`, and appended as `-servermod=@Schwanz/Issue14`. Existing WorldMemoriez remains `-servermod=@Schwanz/WorldMemoriez`. Do not append either twice.

CarRaidZ contains a custom interaction and a synchronized variable on CarScript. A later multiplayer gameplay test requires the **same CarRaidZ PBO on the client** as a regular mod, with the server loading it as a regular mod too. Do not join the server-only compile-test setup and interpret interaction/action-ID differences as a gameplay result. WitnessZ and camouflage are server-authoritative. AmmostackZ also needs the updated client PBO for the larger stacks.

Expected logs after the operator starts: Game, World and Mission module loaded; `[WitnessZ] READY`, `[AdaptiveCamouflage] READY`, and `[CarRaidZ] READY`. A previously reserved CarRaid run instead logs that no respawn/payout is permitted. Any `Can't compile`, new crash file or script exception is a failure requiring an operator stop before replacing PBOs.

## Proposed CarRaidZ flow

Configuration: `$profile:DeutschZ-System/deutschz_carraidez/config.json`.

One empty vanilla OffroadHatchback is placed at X/Z 7480/7480 by default, on the ground. Vehicle inventory is locked to prevent cleanup destroying player deposits. There is no automatic enemy spawn. Interacting within three metres starts two configured Expansion guards. A major event prevents initiation and aborts an active raid. After all guards die, interacting starts a 15-second capture owned by one identified player. Leaving the radius, dying, unconsciousness or disconnect cancels capture. Ten minutes ends the attempt. One small SeaChest reward is committed durably **before** spawning. The chest, vehicle and guard bodies are deleted after the configurable cleanup interval; living guards are removed on abort. Extract reward items before cleanup.

The runID is reserved before any entity spawn. A restart or crash never repeats that run, including a crash between reward commit and spawn; that case can lose the reward rather than duplicate it. Set a **new operator-selected runID** to deliberately allow another test, without deleting the ledger. Unknown/corrupt ledgers disable the module and are preserved. A process crash can leave entities until native CE lifetime cleanup; no claim of instant crash cleanup is made. Do not reset runID repeatedly without checking old test entities.

## WitnessZ behaviour and checks

Configuration: `$profile:DeutschZ-System/deutschz_witnessz/config.json`.

Only positive-damage PlayerBase hit events are evidence. The attacker must be an identified human. A covered face or any headgear prevents identification in this conservative first test. A witness must be alive, conscious, looking towards the attacker, within 35 metres, currently targeting that attacker with native LOS, and pass a fresh bullet raycast to both attacker and victim. AI victims do not report their own hit as a third-party witness. This deliberately misses some legitimate witnesses rather than guessing identity.

Reports wait 20 seconds. A dead, deleted, unconscious-at-report or faction-changed witness is discarded. Pending reports are never persisted, so unreported observations cannot survive a restart and turn into a ghost report. Reported UID/faction records expire after 30 minutes, with alternating validated state files and a maximum of 512 records. There are no names, maps or location broadcasts. Threat adjustment applies only on fresh visible re-identification and never turns native zero threat into hostility. Observation and remembered-target ray checks share one global capped budget; exhausted checks keep native behaviour.

Operator cases: hit behind a wall; hit with covered face; out-of-range/facing-away witness; attacker Alice versus Bob; east versus west faction; kill witness before delay; switch witness faction; unconscious witness; restart before delay; restart after report; expired memory; both state files damaged; future schema; full memory table; many simultaneous players. Record observed outcomes separately from offline results.

## Adaptive Camouflage behaviour and checks

Configuration: `$profile:DeutschZ-System/deutschz_adaptive_camouflage/config.json`.

Woodland clothing on woodland/grass surfaces, crouch/prone posture and darkness can reduce low initial threat. Wrong environment and motion can increase it. The factor is bounded by default to 0.65–1.2 and can never make a visible player invisible. Native zero threat stays zero. Known/searching targets, fighting, raised weapons, attack cooldown and recent aggression bypass the reduction. Native sound, bullets, damage, combat and faction checks run through `super` unchanged. Per-player clothing/surface/posture input is cached for two seconds, with no world/player inventory scan. Witnesses only identify already-targeted, freshly visible players; camouflage cannot erase an existing report.

Operator cases: same AI/distance/daylight with neutral versus matching woodland clothes; woodland versus urban surface; standing/crouching/prone; still/walking/running; daylight/night; firing immediately after changing posture; targets with prior LOS; remembered faction target; friendly/zero-threat targets; logout/rejoin; many players. Compare initial detection while keeping weapon/faction/distance constant.

## Validation boundaries

The Python suite exercises extracted scalar expressions, source contracts, bounded load models, identity/faction/expiry models and crash-point at-most-once models. It is not the DayZ virtual machine and does not prove physical LOS, native CE cleanup, client action synchronization or multiplayer gameplay. CI workflows are provided; local results and cloud-run status are reported separately. Config conversion, PBO creation, official signature checks and FTP readback are separate gates. Actual script compilation requires the operator-controlled startup; functional tests require matching clients and a separate gameplay session.
