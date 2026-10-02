# Compile-Bericht ATM RaideZ

Stand: 2026-08-25

- `CfgConvert -test config.cpp`: Exit 0.
- PBO-Build mit DayZ Labs/Addon Builder: erfolgreich.
- PBO-Praefix: `deutschz_atmraidez`; erwartete Config, Scripts, Layouts und PAA vorhanden.
- Signatur: `DZ_PRO`; `DSCheckSignatures`: OK, Exit 0.
- Dedicated-Stack: CF, Dabs Framework, Expansion Licensed, Expansion Bundle, ATM RaideZ.
- Prozesszeile vor Auswertung geprueft; `-mod` war ein zusammenhaengendes Argument.
- Expansion-Version im frischen Log: 1.9.72.
- Game-, World-, Mission-Modul und Mission-`init.c`: geladen.
- `DeutschZ_ATMRaideZ`: im geladenen CfgMods-Bestand vorhanden.
- Kein `Can't compile`, `unknown type`, `unknown function`, `duplicate class` oder `failed to load`.
- Offene Config-Warnung: `DZATM_RobTool.EnfAnimSys` fehlt; sie brach den Compile nicht ab.

Status: LOKAL TECHNISCH BESTANDEN



## Nachtrag 0.1.1

Die ExpansionATM_2-Action-/Spawn-Aenderungen wurden nach dem oben dokumentierten 0.1.0-Compile vorgenommen. Fuer 0.1.1 liegt in dieser Arbeitsumgebung kein neuer DayZ-Compile-/Signaturbeweis vor.

## Nachtrag 0.1.4 - 2026-08-26

- `DZATM_RobTool.SetActions()` mit `ActionDZATM_RobATM` ergaenzt.
- Addon Builder: Exit 0.
- PBO-Inhalt mit BankRev geprueft; neue Scriptklasse und korrekte PAA enthalten.
- PAA-SHA256 im PBO stimmt mit dem tatsaechlich verwendeten CriminalZ-Source ueberein.
- `DSCheckSignatures`: ATM-RaideZ Signatur OK, Exit 0.
- Isolierter Dedicated-Compile: Game, World, Mission und Mission-`init.c` geladen.
- Kein ATM-RaideZ-Compilefehler, unknown type, unknown function oder duplicate class.

Status: LOKAL TECHNISCH BESTANDEN
