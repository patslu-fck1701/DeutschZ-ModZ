# DeutschZ Platform Roadmap Skill

## Zweck

Dieser Skill hält Codex dauerhaft auf das eigentliche Ziel ausgerichtet: Aufbau einer professionellen, skalierbaren Entwicklungsplattform für DayZ-Modding und später weitere Spiele.

Der Skill darf nicht nur einzelne Bugs reparieren. Er muss bei jedem relevanten Auftrag prüfen, ob die Arbeit:

- den Plattformbau voranbringt,
- dokumentiert und wiederholbar ist,
- zur aktuellen DeutschZ-Priorität passt,
- sicher gebaut und getestet werden kann,
- später für weitere Projekte wiederverwendbar ist.

## Aktivierung

Diesen Skill verwenden bei:

- Architektur- und Plattformplanung
- neuen DeutschZ-Mods oder größeren Features
- Änderungen an KotHZ, ConvoyZ, Screen_Menu, KillReward oder EventItems
- Git-, CI-, Build-, Test- und Release-Arbeiten
- Asset-, UI- oder Audio-Pipelines
- Website-, Domain- oder Produkt-Roadmap-Arbeiten
- Hardware-, lokale-KI- oder Cloud-Entscheidungen
- jedem Auftrag, der mehrere Module oder den langfristigen Workflow betrifft

## Verbindliche Quellen

Vor jeder Arbeit lesen:

```text
docs/MASTER_SKILL.md
AGENTS.md, falls vorhanden
README.md
relevante Modul-Dokumentation
aktuelle Roadmap-Dateien unter docs/platform/
```

Wenn `docs/MASTER_SKILL.md` fehlt, abbrechen und als Blocker melden.

## Plattformdateien

Bei der ersten Ausführung anlegen, wenn sie fehlen:

```text
docs/platform/PLATFORM_STATUS.md
docs/platform/ROADMAP.md
docs/platform/DECISIONS.md
docs/platform/HARDWARE_PLAN.md
docs/platform/NEXT_ACTIONS.md
docs/platform/CHANGELOG_PLATFORM.md
```

Keine leeren Platzhalter erzeugen. Jede Datei muss mindestens den aktuellen bekannten Stand enthalten.

## Arbeitsablauf

### 1. Auftrag klassifizieren

Ordne den Auftrag einer oder mehreren Kategorien zu:

```text
RESEARCH
PLANNING
AUDIT
IMPLEMENTATION
ASSET
BUILD
TEST
RELEASE
WEBSITE
INFRASTRUCTURE
HARDWARE
```

### 2. Priorität prüfen

Aktuelle Reihenfolge:

```text
1. KotHZ
2. ConvoyZ
3. Screen_Menu / Loading Screen
4. KillReward
5. EventItems
6. Live-Test, Bugfixes, Release
```

Abweichungen nur mit dokumentiertem Grund.

### 3. Reifegrad bewerten

Für den betroffenen Bereich einen Status bestimmen:

```text
0 = Idee
1 = Briefing vorhanden
2 = Source identifiziert
3 = Implementierung begonnen
4 = statisch geprüft
5 = gebaut
6 = lokal getestet
7 = live-testbereit
8 = veröffentlicht
9 = stabil und wiederverwendbar
```

Status in `PLATFORM_STATUS.md` aktualisieren.

### 4. Plattformnutzen prüfen

Vor Umsetzung beantworten:

- Ist dies einmalige Handarbeit oder ein wiederkehrender Prozess?
- Lohnt sich ein Validator, Generator, Template oder Skill?
- Kann der Ablauf später für andere Mods wiederverwendet werden?
- Welche Teile müssen deterministisch bleiben?
- Welche Teile profitieren tatsächlich von KI?

Keine Automatisierung bauen, wenn der erwartete Nutzen geringer ist als Wartung und Fehlergefahr.

### 5. Quellen und Pfade prüfen

Verbindliche lokale Struktur:

```text
E:\DeutschZ\DeutschZ-ModZ
E:\DeutschZ\DeutschZ-Server-Settings
E:\DeutschZ\DeutschZServer
C:\Program Files (x86)\Steam\steamapps\common\DayZServer
E:\DeutschZ\PAAs
```

Regeln:

- Source nicht mit Build-Artefakten vermischen.
- Keine Backup-Ordner in den Hauptstrukturen erzeugen.
- Keine privaten Schlüssel oder Secrets ausgeben.
- Keine fremden PBOs öffnen oder dekompilieren.
- Lokaler DayZServer ist Testziel, nicht Source-Repo.

### 6. Umsetzung planen

Vor Codeänderungen kurz dokumentieren:

```text
ZIEL
BETROFFENE DATEIEN
ABHÄNGIGKEITEN
ENGINE-LIMITS
RISIKEN
TESTS
ROLLBACK
```

### 7. Erlebnisprüfung

Bei spielerrelevanten Features immer prüfen:

- Visuals
- UI/HUD
- Notify/Marker
- Audio
- Interaktion
- Fortschritt
- Abbruchfälle
- Cleanup
- Restart-Verhalten
- Belohnung
- Anti-Abuse

### 8. Qualität prüfen

Je nach Auftrag mindestens:

- JSON/XML parsebar
- CFG-Struktur plausibel
- Classnames belegt
- requiredAddons geprüft
- keine DuplicateNames
- Dateipfade und Prefixe korrekt
- Assets vorhanden
- keine Secrets
- Logs geprüft, falls ausgeführt
- Build und Test nicht behaupten, wenn sie nicht gelaufen sind

### 9. Roadmap fortschreiben

Nach jedem größeren Lauf:

- `PLATFORM_STATUS.md` aktualisieren
- `NEXT_ACTIONS.md` auf maximal sieben priorisierte Aufgaben reduzieren
- neue Entscheidungen in `DECISIONS.md` festhalten
- Hardwaretipps nur bei geändertem Bedarf in `HARDWARE_PLAN.md` ergänzen
- Plattformänderungen in `CHANGELOG_PLATFORM.md` eintragen

### 10. Empfehlungen erzeugen

Maximal drei konkrete Empfehlungen:

1. Technik
2. Prozess/Qualität
3. Infrastruktur/Hardware

Jede Empfehlung enthält:

```text
NUTZEN
AUFWAND
KOSTENKLASSE
VORAUSSETZUNG
ZEITPUNKT: JETZT / DANACH / SPÄTER
```

Keine pauschalen Aussagen.

## Hardware-Leitlinie

Aktueller Start:

```text
16 GB RAM
250 GB SSD
2 TB HDD
GTX 1050 Ti
ChatGPT Pro
Codex
```

Voreinstellung:

```text
Cloud-first, lokal für DayZ Tools, Builds, Logs und Tests.
```

Upgrade-Reihenfolge:

```text
1. 32 GB RAM
2. 1 TB SSD/NVMe
3. GPU ab 16 GB VRAM nur bei belegtem lokalem KI-Bedarf
4. optional dedizierter Windows-Build-Runner
```

Keine lokale KI-Plattform als Kern empfehlen, solange Speicher, SSD, Kontextbedarf und wirtschaftlicher Nutzen nicht belegt sind.

## Git- und Release-Leitlinie

- stabile Hauptbranch
- klare Branch-/PR-Aufgaben
- Status Checks
- Review
- Build außerhalb der Source
- Signierung mit geschütztem Private Key
- öffentliche Bikey getrennt
- Testmatrix und Rollback vor Release
- geprüfte Ausgabe nach `E:\DeutschZ\DeutschZServer`
- Live-Änderungen nur mit ausdrücklicher Freigabe

## Pflichtabschluss

Jeder Lauf endet mit:

```text
STATUS: GRÜN / GELB / ROT

REIFEGRAD VORHER: n
REIFEGRAD NACHHER: n

GEÄNDERT:
- ...

GEPRÜFT:
- ...

NICHT GEPRÜFT:
- ...

BLOCKER:
- ...

PLATTFORMDATEIEN AKTUALISIERT:
- ...

NÄCHSTE SCHRITTE:
1. ...
2. ...
3. ...

EMPFEHLUNGEN:
- Technik: ...
- Prozess/Qualität: ...
- Infrastruktur/Hardware: ...

LIVE_SERVER_TOUCHED: JA/NEIN
LIVE_DEPLOY_DONE: JA/NEIN
```

## Erfolgskriterium

Der Skill ist erfolgreich, wenn Codex nicht nur einen Auftrag erledigt, sondern gleichzeitig:

- den tatsächlichen Projektstand sauber hält,
- die nächsten Schritte priorisiert,
- Wiederverwendbarkeit erhöht,
- unnötige Kosten verhindert,
- Tests und Beweise einfordert,
- DeutschZ schrittweise zu einer professionellen Modding-Plattform entwickelt.
