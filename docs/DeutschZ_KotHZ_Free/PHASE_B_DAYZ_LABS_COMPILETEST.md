# DeutschZ KotHZ Free - DayZ Labs Compiletest Phase B

## Ergebnis

- Datum: 16.07.2026 Europe/Berlin
- Branch: `codex/kothz-free-v1-core-20260715`
- Preset: `deutschz_kothz_free_compile`
- Instanz: `kothz_free_compile`
- Port: `2313`
- aktive Mods: ausschliesslich `P:\Mods\@DeutschZ_KotHZ_Free`
- Preflight: PASS, 0 Fehler, 1 bekannte Windows-Grossbuchstabenwarnung
- Clean Build: PASS
- PBO-Inhaltspruefung: PASS, 17 erwartete Dateien
- Signatur: PASS, `DeutschZ.bisign is OK`
- Serverstart: PASS
- Script-Compile: PASS
- Missionstart: PASS, `Player connect enabled`
- Stabilitaet nach Missionstart: PASS, mehr als 2 Minuten
- kontrollierter Stopp: PASS

## Laufzeitbelege

- Scriptlog: `C:\Users\patsl\DayZProjects\servers\kothz_free_compile\profiles\script_2026-07-16_00-22-14.log`
- RPT: `C:\Users\patsl\DayZProjects\servers\kothz_free_compile\profiles\DayZServer_x64_2026-07-16_00-22-02.RPT`
- `World objects spawned: mast=1 flag=1 ... radius=25m.`
- `State IDLE -> ANNOUNCED.`
- `Smoke state WHITE.`
- `State ANNOUNCED -> ACTIVE.`
- `Smoke state GREEN.`
- Script-Fatalfehler: 0
- KotHZ-Free-spezifische RPT-Fehler: 0

## Bekannte nicht blockierende Engine-Meldung

Die Windows-Server-RPT meldet beim Mod-Root ebenso wie beim Vanilla-Root `sakhal` ein fehlendes `Anims/cfg/skeletons.anim.xml`. Der Mod definiert keine eigenen Animationen; Scriptcompile, Missionstart und stabiler Lauf wurden dadurch nicht beeintraechtigt. Es wurden keine hypothetischen Linux-Pfadumbauten vorgenommen.

## Noch offen

Ein echter Client-/Gameplaytest muss visuell belegen: exakt eine Fahne, Fahnenbewegung bei genau einem Spieler, Rot und Fortschrittspause bei mehreren Spielern sowie sichtbares Cleanup nach Abbruch und normalem Ende.
