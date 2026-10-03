# ToxicZ Verifikation 2026-09-05

- `CfgConvert`: config.cpp erfolgreich nach config.bin konvertiert.
- `AddonBuilder`: PBO erfolgreich mit Praefix `deutschz_toxicz` gebaut.
- Dedicated-Teststack: CF, Dabs Framework, Expansion Bundle, Expansion Licensed, deutschz_aiconvoyz, deutschz_toxicz.
- Frischer Dedicated-RPT: Game, World und Mission kompiliert; `deutschz_toxicz` registriert; Controller mit Zustand 0 gestartet.
- Im finalen Lauf keine ToxicZ-Scriptfehler und kein `Can't compile`.
- DZ_PRO-Signatur erstellt; `DSCheckSignatures` Exitcode 0.

Status: BEREIT FUER TESTSERVER

Kein Client-Sichttest und kein vollstaendiger Gameplay-/Auszahlungstest erfolgt.

## Korrekturpruefung 2026-09-12

- `deutschz_aiconvoyz`: Config-Konvertierung, Textur-Header und PBO-Build mit DayZ Addon Builder 1.0.240639 erfolgreich.
- `deutschz_toxicz`: Config-Konvertierung, Binarisierung und PBO-Build mit DayZ Addon Builder 1.0.240639 erfolgreich.
- Buildartefakte: `_CodexStaging/ToxicZ_fix_20260912/deutschz_aiconvoyz.pbo` und `deutschz_toxicz.pbo`.
- Noch offen: Dedicated-Scriptkompilierung sowie Client-Sicht-/Gameplaytest der Startszene, Belegt-Meldung und Offline-Uebernahme.
