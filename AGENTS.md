# AGENTS.md — DeutschZ Codex Entry Rules

## Pflichtstart

Bei jedem Codex-Auftrag in diesem Repository zuerst lesen:

```text
docs/MASTER_SKILL.md
.agents/skills/deutschz-platform-roadmap/SKILL.md
README.md
```

Danach relevante Modul- und Plattformdokumentation lesen.

Der Plattform-Skill ist verpflichtend bei allen größeren Änderungen, Audits, Features, Builds, Tests, Releases, Assets, Website-, Infrastruktur- und Hardwarethemen.

## Projektziel

DeutschZ ist das Referenzprojekt für eine professionelle, wiederverwendbare Modding-Entwicklungsplattform.

Nicht nur Code liefern. Immer den vollständigen Ablauf beachten:

```text
Idee -> Spezifikation -> Source -> Config -> Assets -> Prüfung -> Build -> Test -> Release -> Website/Wartung
```

## Aktuelle Priorität

```text
1. KotHZ
2. ConvoyZ
3. Screen_Menu / Loading Screen
4. KillReward
5. EventItems
6. Live-Test, Bugfixes, Release
```

Abweichungen begründen und in der Roadmap dokumentieren.

## Verbindliche Pfade

```text
E:\DeutschZ\DeutschZ-ModZ
E:\DeutschZ\DeutschZ-Server-Settings
E:\DeutschZ\DeutschZServer
C:\Program Files (x86)\Steam\steamapps\common\DayZServer
E:\DeutschZ\PAAs
```

`E:\DeutschZ\DeutschZServer` ist Staging für ungeprüfte Ergebnisse. Der lokale DayZServer ist Testziel, nicht primäre Source.

## Harte Regeln

- Keine automatischen Backup- oder Kopie-Ordner in den Hauptstrukturen.
- Keine fremden PBOs öffnen oder dekompilieren.
- Keine Secrets, Tokens, Webhooks, Passwörter oder Private Keys ausgeben oder committen.
- Build-Artefakte nicht als Source behandeln.
- Keine Fantasie-Classnames.
- Keine lokalen Windows-Pfade in DayZ-Runtime-Code hardcoden.
- Keine Build-, Test- oder Live-Erfolge behaupten, wenn sie nicht tatsächlich ausgeführt wurden.
- Keine Live-Änderung ohne ausdrückliche Freigabe und Rollback.

## Plattformsteuerung

Bei relevanten Aufträgen folgende Dateien prüfen und aktualisieren:

```text
docs/platform/PLATFORM_STATUS.md
docs/platform/ROADMAP.md
docs/platform/DECISIONS.md
docs/platform/HARDWARE_PLAN.md
docs/platform/NEXT_ACTIONS.md
docs/platform/CHANGELOG_PLATFORM.md
```

Fehlende Dateien nach den Regeln des Roadmap-Skills mit echtem Inhalt anlegen.

## Arbeitsweise

Vor Änderungen:

```text
ZIEL
BETROFFENE DATEIEN
ABHÄNGIGKEITEN
RISIKEN
TESTS
ROLLBACK
```

Nach Änderungen:

```text
STATUS
GEÄNDERT
GEPRÜFT
NICHT GEPRÜFT
BLOCKER
ROADMAP-AKTUALISIERUNG
NÄCHSTE SCHRITTE
MAXIMAL DREI KONKRETE EMPFEHLUNGEN
LIVE-STATUS
```

## Erlebnisprüfung

Spielerrelevante Features müssen als vollständiges Erlebnis geprüft werden:

- Visuals
- UI/HUD
- Expansion Notify/Marker
- Audio
- Interaktion
- Fortschritt
- Abbruch/Restart/Disconnect
- Cleanup
- Belohnung
- Anti-Abuse

Kompilieren allein ist keine Fertigstellung.

## Hardware- und KI-Leitlinie

Aktuell cloud-first arbeiten:

- ChatGPT Pro für Research, Spezifikation und Entscheidungen
- Codex für Repo-Arbeit, Reviews und längere Coding-Aufgaben
- lokaler PC für DayZ Tools, Workbench, Builds, Logs und Tests

Upgrade-Reihenfolge:

```text
1. 32 GB RAM
2. 1 TB SSD/NVMe
3. GPU ab 16 GB VRAM nur bei belegtem Bedarf
4. optional Windows Self-Hosted Build Runner
```

Keine teure Hardware oder lokale KI empfehlen, bevor Nutzen, Engpass und Kosten nachvollziehbar dokumentiert sind.

## Pflichtabschluss

Jeder größere Lauf endet im Format aus:

```text
.agents/skills/deutschz-platform-roadmap/SKILL.md
```

Zusätzlich immer angeben:

```text
LIVE_SERVER_TOUCHED: JA/NEIN
LIVE_DEPLOY_DONE: JA/NEIN
```
