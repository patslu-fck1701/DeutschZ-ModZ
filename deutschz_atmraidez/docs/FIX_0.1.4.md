# ATM RaideZ 0.1.4 Fix

## Ursache

- Die Raid-Action war global registriert und am ATM eingetragen, aber nicht am aktiven `DZATM_RobTool`.
- Die zuvor gelieferte gleichnamige Mediendatei war nicht binär identisch mit der PAA des tatsaechlich verwendeten CriminalZ-Source.

## Aenderung

- `DZATM_RobTool.SetActions()` fuegt `ActionDZATM_RobATM` hinzu.
- `robtool_crowbar_co.paa` wurde aus dem nachweislich verwendeten CriminalZ-Source uebernommen.

## Nachweis vor dem Fix

- Serverlog 2026-08-26: 16 von 16 `ExpansionATM_2` erfolgreich gespawnt, 0 Fehler.
- Owner-Sichttest: normales ATM-Menue vorhanden, Raid-Action nicht angezeigt, Werkzeugtextur falsch skaliert.

## Offener Nachweis

- Neuer Client-Sichttest fuer Action und Werkzeugtextur.
- Vollstaendiger Raidablauf mit Phase 1, Phase 2 und Reward.
