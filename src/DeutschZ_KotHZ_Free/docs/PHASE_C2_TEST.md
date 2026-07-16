# KotHZ Free Phase C2 - RewardCrate

- Genau eine `DZKOTHF_RewardCrate` wird unmittelbar nach erfolgreichem Capture erzeugt.
- Klasse, Inhalt, Mengen, Chancen, Spawn-Offset und Lifetime kommen aus `KotHZFreeSettings.json`.
- Default: SeaChest-Basis, 500 Slots (`10 x 50`), nicht in Haende oder Cargo aufnehmbar.
- Kein Boss, keine Keycard und kein Feuerwerk sind beteiligt.
- Mast, Fahne, Smoke und Gegner werden nach `CompletionCleanupDelaySeconds` entfernt; die Kiste bleibt bis `RewardLifetimeMinutes` bestehen.
- Vor einem neuen Event wird eine eventuell noch vorhandene Kiste entfernt, damit keine Duplikate entstehen.
- Bei Missionsende wird die Kiste serverseitig entfernt.

Profilpfade:

- `$profile:DeutschZ-System/DeutschZ_KotHZ/KotHZFreeSettings.json`
- `$profile:DeutschZ-System/LogZ/DeutschZ_KotHZ/KotHZ.log`

Ein Server-Compiletest beweist die registrierte Klasse und fehlerfreie Initialisierung. Spawn, Inhalt, Lootzugriff und Lifetime erfordern Patricks echten Capture-/Gameplaytest.
