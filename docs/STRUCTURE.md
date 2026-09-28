# Mod source layout

Use `mods/<ModName>/` as the stable top-level ownership boundary. Inside a DayZ mod, keep PBO/source package boundaries explicit and preserve DayZ script layers such as 3_Game, 4_World and 5_Mission where applicable.

Cross-mod contracts belong in `docs/architecture/`; event-specific tests belong in `docs/testing/`. Historical repository content is preserved on the legacy archive branch, not mixed into the new main baseline.
