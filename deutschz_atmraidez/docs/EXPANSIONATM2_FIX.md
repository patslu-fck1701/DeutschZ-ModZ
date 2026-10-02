# ExpansionATM_2 Raid-/Spawn-Fix

Stand: 2026-08-25

- Raid-Ziel ist fest `ExpansionATM_2`.
- Action-Aufloesung versucht zuerst `ActionTarget.GetObject()` und danach `GetParentOrObject()`.
- Bereits vorhandene oder per Admin-/Mapping-Tool gesetzte `ExpansionATM_2` erhalten die Raid-Action ueber `SetActions()`.
- 16 konfigurierte ATM-Positionen koennen serverseitig gespawnt werden.
- Befindet sich innerhalb `ReuseRadiusMeters` bereits ein `ExpansionATM_2`, wird dieser registriert und nicht doppelt gespawnt.
- `DZCRZ_ATM` wird nicht verwendet; die gelieferten DZCRZ-Zeilen dienten nur als Positions-/Orientierungsquelle.
- Der fruehere Compile-Bericht gilt nur fuer 0.1.0. Die Aenderung 0.1.1 wurde in dieser Arbeitsumgebung nicht mit dem DayZ-Compiler gebaut oder live getestet.
