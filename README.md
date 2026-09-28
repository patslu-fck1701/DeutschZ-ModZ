# DeutschZ ModZ

Canonical source repository for the currently developed DeutschZ-owned DayZ mods.

## Structure
- `mods/<ModName>/` — one owned mod/source unit per folder.
- `shared/` — intentionally shared source/assets only.
- `docs/architecture/` — cross-mod contracts and architecture.
- `docs/testing/` — event/mod test definitions and evidence references.
- `docs/dependencies/` — external mod/API dependencies; never vendor foreign mods.
- `tools/` — build/validation helpers.
- `templates/` — clean source templates.

Each mod should keep its own README, dependencies, build notes and test scope close to the source.

## Rules
Source-first. No built PBOs/bisigns, private signing keys, Workshop dumps, DayZ installations, runtime profiles/logs/storage, secrets, or assets without clear rights.

The existing DeutschZ Testzentrale remains structurally unchanged for now. Precise test possibilities are expanded event by event.

**WerkZ semantic baseline:** KB-v1.19  
**Legacy snapshot:** branch `archive/legacy-before-modz-rebuild-2026-09-28`

The current local ModZ source is imported onto this clean baseline deliberately; the previous repository contents are historical only.
