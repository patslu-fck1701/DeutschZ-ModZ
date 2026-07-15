# DeutschZ Master Skill

Stand: 2026-07-15
Version: 2.0
Status: verbindliche Projekt- und Entwicklungsleitlinie

## 1. Mission

DeutschZ wird nicht als Sammlung einzelner DayZ-Fixes geführt, sondern als professionelle, wiederverwendbare Entwicklungsplattform.

Die Plattform muss den vollständigen Modding-Lebenszyklus abdecken:

1. Idee und Produktziel
2. technische Spezifikation
3. Architektur und Abhängigkeiten
4. Source-Code und Konfiguration
5. UI, Texturen, Audio und Event-Inszenierung
6. statische Prüfungen
7. Build, PBO und Signierung
8. lokaler Server- und Client-Test
9. Release, Changelog und Website-Ausgabe
10. Wartung, Fehleranalyse und Wiederverwendung

Zuerst wird DayZ vollständig unterstützt. Jede Struktur muss später auf weitere Spiele oder Modding-Projekte erweiterbar bleiben.

## 2. Grundsatz

KI ist eine Assistenz- und Orchestrierungsschicht, keine unkontrollierte Ersatzhandlung.

Deterministische Aufgaben müssen durch Skripte, Validatoren und reproduzierbare Pipelines erledigt werden. KI wird eingesetzt für:

- Analyse und Planung
- Auftragszerlegung
- Architekturvorschläge
- Code- und Config-Erstellung
- Reviews und Fehlerhypothesen
- Asset-Briefings
- Testplanung
- Release-Dokumentation
- Roadmap- und Investitionsempfehlungen

Nicht erlaubt ist blindes Automatisieren ohne Qualitätsgewinn, Beweise oder Rückrollmöglichkeit.

## 3. Aktuelle Arbeitsrealität

Aktuelle Hardware:

- 16 GB RAM
- 2 TB HDD
- 250 GB SSD
- NVIDIA GeForce GTX 1050 Ti
- ChatGPT Pro
- Codex
- bestehende Website: https://project23947.websitepublisher.ai/

Folgerung:

- Arbeitsmodell vorerst cloud-first und hybrid
- ChatGPT für Research, Spezifikationen und Entscheidungen
- Codex für Repo-Arbeit, Refactoring, Prüfungen und PRs
- lokaler PC für Git, DayZ Tools, Workbench, Serverstart, Logs und Sichttests
- keine große lokale LLM-Plattform als Hauptsystem auf dieser Hardware

## 4. Hardware-Roadmap

### Stufe A — sofort

Priorität:

1. Arbeitsstruktur und Git-Prozesse stabilisieren
2. SSD-Speicher freihalten
3. Domain sichern
4. CI-Prüfungen einführen
5. standardisierte Skill- und Auftragsvorlagen einführen

### Stufe B — erste Aufrüstung

Empfehlung:

1. RAM auf mindestens 32 GB
2. 1 TB SSD oder NVMe
3. saubere Trennung von Source, Build, Server und Assets

### Stufe C — lokale KI und Build-Automation

Erst danach prüfen:

- GPU mit mindestens 16 GB VRAM
- dedizierter Windows-Build-Rechner
- GitHub Self-Hosted Runner
- Ollama oder vergleichbare lokale Inferenz
- lokale Modelle nur für Datenschutz, Offline-Arbeit oder Vorprüfung

Codex darf keine GPU-Anschaffung empfehlen, bevor RAM, SSD, Arbeitsprozesse und messbarer Bedarf geprüft wurden.

## 5. Verbindliche Projektpfade

```text
Mod-Repository:
E:\DeutschZ\DeutschZ-ModZ

Server-Settings-Repository:
E:\DeutschZ\DeutschZ-Server-Settings

Ungeprüfter Ausgabe- und Deployment-Staging-Ordner:
E:\DeutschZ\DeutschZServer

Lokaler DayZ-Testserver:
C:\Program Files (x86)\Steam\steamapps\common\DayZServer

Bilder, PNG, PAA und Asset-Quellen:
E:\DeutschZ\PAAs
```

Regeln:

- In diese vier Hauptbereiche gehören keine automatischen Backup-Ordner, Kopien mit Fantasienamen oder temporäre Codex-Ablagen.
- `E:\DeutschZ\DeutschZServer` ist Staging für ungeprüfte Ergebnisse und wird von dort kontrolliert auf den LiveServer übertragen.
- Der lokale DayZServer ist Testziel, nicht primärer Source-Arbeitsordner.
- Build-Artefakte sind keine Source.
- Private Keys, Tokens, Passwörter und Webhooks dürfen niemals in Git oder Reports landen.

## 6. Prioritäten

Aktuelle Arbeitsreihenfolge:

1. KotHZ
2. ConvoyZ
3. Screen_Menu / Loading Screen
4. KillReward
5. EventItems
6. Live-Test, Bugfixes und Release

Eine Änderung dieser Reihenfolge ist nur erlaubt, wenn ein Blocker, Sicherheitsproblem oder gemeinsamer Architekturfehler nachweisbar mehrere Bereiche betrifft.

## 7. Zielarchitektur der Plattform

Die Plattform wird modular aufgebaut.

### 7.1 Lead- und Planungsinstanz

Aufgaben:

- Nutzerwunsch in ein prüfbares Briefing übersetzen
- Umfang, Risiken und Engine-Limits erkennen
- Dateien, Abhängigkeiten und Testziele bestimmen
- klare Done-Kriterien erzeugen

### 7.2 DayZ Script Agent

Verantwortlich für:

- Enforce Script
- 3_Game, 4_World, 5_Mission
- RPCs
- Serverautorität
- Events, Zustände, Timer und Cleanup
- Expansion Notify und Marker
- HUD und UI-Anbindung

### 7.3 DayZ Config Agent

Verantwortlich für:

- config.cpp
- mod.cpp
- JSON
- XML
- Central Economy
- Load Order
- requiredAddons
- Classnames und Basisklassen

### 7.4 Asset Agent

Verantwortlich für:

- PNG/TGA/JPG zu PAA-Workflow
- UV-Vorlagen
- UI-Overlays
- Icons und Logos
- OGG/WAV/MP3-Quellen
- Funksprüche, Störsignale, Sirenen und Event-Audio
- Lizenz- und Quellenmanifest

### 7.5 QA- und Release-Agent

Verantwortlich für:

- statische Validatoren
- Build- und Signaturmatrix
- Logs
- Serverstart
- Mission- und Welttests
- Testmatrix
- Rollback
- Release Notes
- Website- und Changelog-Aktualisierung

## 8. Erlebnis statt nur Code

Jede größere Funktion wird als vollständiges Spielerlebnis bewertet.

Pflichtfragen:

- Was sieht der Spieler?
- Was hört der Spieler?
- Was muss der Spieler tun?
- Welche Rückmeldung erhält er?
- Welche Marker, Notify- oder HUD-Elemente erscheinen?
- Was passiert bei Abbruch, Tod, Restart oder Disconnect?
- Wie wird Cleanup garantiert?
- Wie wird Missbrauch verhindert?

Mögliche Erlebnisbausteine innerhalb der DayZ-Engine:

- Expansion Notify
- 2D- und 3D-Marker
- HUD-Status und Fortschrittsanzeige
- Funksprüche mit Störsignal
- Sirenen und Warnsequenzen
- Boss- oder Event-Intros
- Rauch, Licht, Sound und Spawnsequenzen
- UI-Overlays und Loading Screens
- Belohnungs- und Abschlussfeedback

Keine Funktion gilt als fertig, nur weil sie kompiliert.

## 9. Git- und Branch-Regeln

- `main` muss stabil bleiben.
- Jede Änderung erfolgt über einen klar benannten Branch oder Codex-Task.
- Pull Requests müssen Ziel, Dateiliste, Tests, Risiken und Rollback enthalten.
- Required Status Checks und Reviews sind Zielstandard.
- PBOs, BISIGNs, private Keys, Logs, Persistence und Server-Caches gehören nicht in das Source-Repo.
- Große lizenzierte Original-Assets nur bewusst über Git LFS.
- Keine fremden PBOs öffnen oder dekompilieren.

## 10. Standardpipeline

### Phase 1 — Briefing

Pflichtausgabe:

- Ziel
- Nicht-Ziele
- betroffene Module
- Engine-Limits
- Abhängigkeiten
- Risiken
- Done-Kriterien

### Phase 2 — Source-Audit

Prüfen:

- führende Source
- Duplikate und Altstände
- Classnames
- Abhängigkeiten
- Config- und Script-Grenzen
- vorhandene Tests

### Phase 3 — Umsetzung

- kleine, nachvollziehbare Änderungen
- keine Fantasie-Classnames
- keine unnötigen Fremdmod-Abhängigkeiten
- keine hardcodierten lokalen Windows-Pfade in Runtime-Code

### Phase 4 — statische Prüfung

Mindestens:

- JSON parsebar
- XML parsebar
- CFG-Struktur plausibel
- keine DuplicateNames
- Prefixe und Pfade korrekt
- requiredAddons geprüft
- Asset-Dateien vorhanden
- keine Secrets

### Phase 5 — Build

- Build außerhalb des Source-Repositories
- PBO erzeugen
- BISIGN erzeugen
- Public Bikey prüfen
- Private Key geschützt halten

### Phase 6 — lokaler Test

- DayZ Labs / lokaler Server
- Mission lädt
- Scripts laden
- Events starten
- Items spawnen
- Logs ohne kritische Fehler
- Client-/Sichttest durch Patrick

### Phase 7 — Release

- geprüfte Dateien nach `E:\DeutschZ\DeutschZServer`
- Release Notes
- Versionsnummer
- Rollback-Paket
- Website- und Changelog-Aktualisierung
- erst danach kontrollierter Live-Test oder Live-Deploy

## 11. Fortschritts- und Roadmap-Dateien

Dieser Master Skill verlangt eine dauerhafte Projektsteuerung.

Codex muss folgende Dateien verwenden oder anlegen:

```text
docs/platform/PLATFORM_STATUS.md
docs/platform/ROADMAP.md
docs/platform/DECISIONS.md
docs/platform/HARDWARE_PLAN.md
docs/platform/NEXT_ACTIONS.md
docs/platform/CHANGELOG_PLATFORM.md
```

### PLATFORM_STATUS.md

Enthält:

- aktueller Reifegrad
- aktive Module
- Blocker
- letzte überprüfte Commits
- CI-Status
- Build-Status
- Teststatus
- Website-/Domain-Status

### ROADMAP.md

Enthält:

- Jetzt
- Danach
- Später
- messbare Ziele
- Abhängigkeiten
- Budgetstufe

### DECISIONS.md

Jede wichtige Architektur- oder Investitionsentscheidung wird dokumentiert mit:

- Datum
- Entscheidung
- Begründung
- Alternativen
- Folgen
- Prüftermin

### HARDWARE_PLAN.md

Enthält:

- aktuelle Hardware
- Engpässe
- nächste sinnvolle Aufrüstung
- erwarteter Nutzen
- geschätzte Budgetklasse
- Entscheidung: jetzt kaufen / später kaufen / nicht nötig

### NEXT_ACTIONS.md

Enthält maximal sieben konkrete nächste Aufgaben, nach Wirkung und Abhängigkeit sortiert.

## 12. Selbstprüfung bei jedem Codex-Auftrag

Vor Beginn muss Codex prüfen:

1. Ist `docs/MASTER_SKILL.md` vorhanden und gelesen?
2. Ist der Auftrag mit der aktuellen Priorität vereinbar?
3. Welche Roadmap-Dateien sind betroffen?
4. Ist die führende Source eindeutig?
5. Ist der Auftrag Analyse, Umsetzung, Build, Test oder Release?
6. Welche Freigaben fehlen?
7. Gibt es einen billigeren oder sichereren Weg mit gleichem Ergebnis?

Nach Abschluss muss Codex prüfen:

1. Was wurde tatsächlich geändert?
2. Welche Beweise existieren?
3. Welche Tests liefen wirklich?
4. Was blieb ungeprüft?
5. Welche Roadmap-Datei muss aktualisiert werden?
6. Was ist der nächste sinnvollste Schritt?
7. Gibt es einen Hardware-, Prozess- oder Tool-Tipp mit messbarem Nutzen?

## 13. Kontinuierliche Beratung

Codex soll nicht nur Aufgaben ausführen, sondern den Weg zur Plattform aktiv absichern.

Nach jedem größeren Lauf muss Codex maximal drei Empfehlungen liefern:

- nächster technischer Schritt
- nächster Prozess-/Qualitätsschritt
- nächster Infrastruktur-/Hardware-Schritt

Empfehlungen müssen konkret sein und enthalten:

- Nutzen
- Aufwand
- Kostenklasse: kostenlos / niedrig / mittel / hoch
- Voraussetzung
- warum jetzt oder warum später

Keine generischen Tipps wie „mehr automatisieren“ oder „bessere Hardware kaufen“.

## 14. Website und Domain

Die Website ist das Produkt-, Release- und Kommunikations-Frontend der Plattform.

Aktueller Stand:

```text
https://project23947.websitepublisher.ai/
```

Ziel:

```text
https://www.deutschz.de/
```

Website-Aufgaben:

- aktuelle Mod- und Eventstände zeigen
- keine entfernten oder nie geplanten Events bewerben
- Changelogs und Releases darstellen
- AI Studio klar als Roadmap und späteres Produkt zeigen
- GitHub, Discord und Serverinformationen konsistent halten
- Creator- und Auftragsprozess verständlich darstellen

Domain-Status und Migration werden in `docs/platform/PLATFORM_STATUS.md` und `docs/platform/ROADMAP.md` geführt.

## 15. Qualitätsgates

Eine Aufgabe ist nur abgeschlossen, wenn die für ihre Phase notwendigen Gates erfüllt sind.

### Analyse-Gate

- Pfade und Source geprüft
- Risiken dokumentiert
- keine unbelegten Behauptungen

### Umsetzungs-Gate

- Änderungen nachvollziehbar
- Syntax- und Strukturprüfungen bestanden
- Tests definiert

### Build-Gate

- PBO gebaut
- Signaturen vollständig
- Artefakte getrennt

### Test-Gate

- Serverstart geprüft
- relevante Logs geprüft
- Mission/Welt/Skript geladen
- Sichttest offen oder abgeschlossen klar markiert

### Release-Gate

- Rollback vorhanden
- Release Notes vorhanden
- Dateien im richtigen Staging-Ordner
- Live-Freigabe ausdrücklich vorhanden

## 16. Pflichtausgabe jedes Codex-Laufs

```text
STATUS: GRÜN / GELB / ROT

GEÄNDERT:
- ...

GEPRÜFT:
- ...

NICHT GEPRÜFT:
- ...

BLOCKER:
- ...

ROADMAP AKTUALISIERT:
- JA/NEIN
- Dateien: ...

NÄCHSTE SCHRITTE:
1. ...
2. ...
3. ...

EMPFEHLUNG:
- technischer Tipp
- Prozess-/Qualitätstipp
- Infrastruktur-/Hardwaretipp

LIVE-STATUS:
LIVE_SERVER_TOUCHED: JA/NEIN
LIVE_DEPLOY_DONE: JA/NEIN
```

## 17. Abschlussziel

Das Endprodukt ist eine professionelle, wiederverwendbare Entwicklungsplattform, die aus einem Nutzerwunsch kontrolliert erzeugen kann:

- Spezifikation
- Repo- und Dateistruktur
- Code und Config
- UI- und Asset-Briefings
- Audio- und Event-Cue-Sheets
- Validatoren
- Testmatrix
- Build- und Release-Paket
- Changelog und Website-Inhalte

DeutschZ ist das erste Referenzprojekt. Die Plattform muss so dokumentiert und modularisiert werden, dass später weitere Server, Modder und Spiele unterstützt werden können.
