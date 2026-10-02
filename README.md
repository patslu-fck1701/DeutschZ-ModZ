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

## Snapshot 02.10.2026 · 20:00

Aktueller angelieferter Source-Stand: **Source_Stand_02.10.2026_20_00_Uhr.zip**.

- **Wichtig:** `deutschz_hudz` ist in diesem Archiv **nicht enthalten**. HUDZ wird wegen seiner Größe separat geführt.
- Enthalten: **1.489 Dateien**, davon **305 Code-/Text-/Konfigurationsdateien**.
- Vollständiges Archiv inkl. eigener Binär-/Audio-/Texture-Assets: privates Google-Drive-Backup.
- Zusätzlich liegt dort `DeutschZ_Source_Code_2026-10-02_ohne_HUDZ.zip` als kompakter Code-/Konfigurationsstand ohne große Binärassets.
- SHA-256 vollständiger Source-ZIP: `58dd14a134205457f6b53e187782e5a6a245b19e49a08b8cc23703617ea52280`
- SHA-256 Code-only-ZIP: `aeb754c359237ccc59965b59117fa6420118b465c8bdea927da3c8f57c941f42`
- Backup-Ordner: https://drive.google.com/drive/folders/1tRdKCTIK7XHR2Vqjg9AiJfDv2xTtiYun

Erfasste Source-Module:

`FogOverRide`, `deutschz_afkz`, `deutschz_aiconvoyz`, `deutschz_airdropz`, `deutschz_atmraidez`, `deutschz_battlegroundz`, `deutschz_courierz`, `deutschz_eventschedulerz`, `deutschz_fuelz`, `deutschz_king_of_the_hillz`, `deutschz_minimapz`, `deutschz_operation_deutschz`, `deutschz_pointsz`, `deutschz_propertyz`, `deutschz_propertyz_breaching`, `deutschz_radiomissionz`, `deutschz_realisticz`, `deutschz_roamingblackmarket_adapter`, `deutschz_toxicz`, `deutschz_warningz`, `deutschz_welcomez`.

Der vollständige Binärstand wird bewusst nicht als Git-Blob gespiegelt; Git bleibt für lesbaren Quellcode, Struktur, Doku und nachvollziehbare Versionsstände zuständig. Große Assets und vollständige Snapshot-ZIPs liegen im versionierten Backup.

