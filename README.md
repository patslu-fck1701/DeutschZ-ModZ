# DeutschZ ModZ

Quellcode, eigene Assets, Dokumentation und Build-Struktur der selbst entwickelten DayZ-Mods für den **DeutschZ Server**.

Dieses Repository ist ein **Source-Repository**. Fertige PBOs, Signaturen, private Schlüssel, Steam-Workshop-Inhalte, DayZ-Serverdateien und lokale Laufzeitdaten werden hier nicht gespeichert.

## Projekte und Priorität

1. **KotHZ** – Hack-Terminal, Vorphase, Marker, Notify, HUD und Übergabe an das King-of-the-Hill-Event
2. **ConvoyZ** – AI-Kampfphase, Statusanzeige, Blackbox-Hack, Loot und Cleanup
3. **Screen_Menu** – Hauptmenü, Loading Screen, Logo, Layout, PAA und Audio
4. **KillReward** – serverseitige Killprüfung und konfigurierbare Belohnungen
5. **EventItems** – eigene DeutschZ-Items, Modelle, Texturen und Aktionen
6. **Live-Test, Bugfixes und Release**

## Repository-Grenzen

### Gehört in dieses Repository

- selbst geschriebene `*.c`- und `*.cpp`-Quellen
- `config.cpp`, `mod.cpp` und `$PBOPREFIX$`
- eigene oder rechtmäßig verwendete Modelle, Texturen, Materialien und Sounds
- JSON-/XML-Beispielkonfigurationen ohne Geheimnisse
- Build-, Test- und Release-Dokumentation
- Hilfsskripte für Prüfung und Packaging
- Änderungsprotokolle und Testnachweise

### Gehört nicht in dieses Repository

- `*.pbo`, `*.bisign`, private Signaturschlüssel und Build-Ausgaben
- komplette Workshop-Mods oder entpackte Dateien fremder Mods
- DayZ-, DayZ-Server- oder DayZ-Tools-Installationen
- lokale Profile, Logs, Crash-Dumps und Storage-Daten
- Zugangsdaten, Tokens, Webhooks und Passwörter
- Assets ohne geklärte Nutzungsrechte

Details: [`docs/REPOSITORY_SCOPE.md`](docs/REPOSITORY_SCOPE.md)

## Struktur

```text
DeutschZ-ModZ/
├── .github/                 GitHub-Prüfungen und Vorlagen
├── docs/                    Architektur, Build, Tests und Releases
├── src/
│   ├── KotHZ/
│   ├── ConvoyZ/
│   ├── Screen_Menu/
│   ├── KillReward/
│   └── EventItems/
├── shared/                  bewusst gemeinsam genutzte Quellen und Assets
├── templates/mod-source/    Vorlage für ein neues Mod-/PBO-Quellpaket
├── tools/                   lokale Prüf- und Build-Hilfen
├── tests/                   Testdaten und automatisierbare Prüfungen
├── CHANGELOG.md
├── CONTRIBUTING.md
├── LICENSE
└── SECURITY.md
```

Jede tatsächlich gebaute PBO-Einheit soll einen eindeutig abgegrenzten Quellordner mit eigener `config.cpp` und eigenem `$PBOPREFIX$` besitzen. Die Ordner unter `src/` dürfen daher weitere Unterpakete enthalten.

## DayZ-Script-Schichten

```text
scripts/
├── 3_Game/      Datenmodelle, Enums, RPC-IDs, Konfiguration und Hilfsklassen
├── 4_World/     Entitäten, Aktionen, Items und Weltlogik
└── 5_Mission/   Initialisierung, Manager, HUD, Marker, Notify und RPC-Empfang
```

Eventzustände, Spawns, Rewards, Validierung und Cleanup bleiben serverautoritativ. Clientcode zeigt nur UI, Marker und Rückmeldungen an.

## Abhängigkeiten

Fremde Mods werden ausschließlich als Abhängigkeit dokumentiert. Keine fremden PBOs oder entpackten Quellen committen.

Für jede Abhängigkeit sind mindestens festzuhalten:

- Modname und Workshop-ID
- benötigte Version beziehungsweise Prüfdatum
- Lade-Reihenfolge
- verwendete Klassen oder APIs
- zwingende oder optionale Abhängigkeit
- Fallback-Verhalten

Siehe [`docs/DEPENDENCIES.md`](docs/DEPENDENCIES.md).

## Assets

Eigene Source-Assets dürfen versioniert werden, darunter:

```text
.paa .png .tga .edds .rvmat .p3d .ogg .wss .wav
```

Für jedes nicht vollständig selbst erstellte Asset müssen Quelle, Urheber, Lizenz und Bearbeitungsrecht in [`docs/ASSET_LICENSES.md`](docs/ASSET_LICENSES.md) dokumentiert sein.

Große Binär-Assets gehören gegebenenfalls in Git LFS. Die automatische Repo-Prüfung blockiert normale Dateien über 25 MiB.

## Arbeitsablauf

1. Issue oder Reparaturauftrag festlegen.
2. Feature-/Fix-Branch erstellen.
3. Änderung in der passenden Mod und Script-Schicht umsetzen.
4. `python tools/repo_guard.py` ausführen.
5. PBO außerhalb des Repositorys bauen und signieren.
6. lokalen Serverstart und Logs prüfen.
7. Testmatrix dokumentieren.
8. gezielten Commit erstellen.
9. Pull Request prüfen und nach `main` übernehmen.
10. Release und Changelog erstellen.

## Commit-Beispiele

```text
feat(kothz): add terminal hack state machine
fix(convoyz): continue event after final ai death
fix(screen-menu): preserve logo aspect ratio
fix(event-items): replace invalid base classes
chore(release): prepare v0.4.0
```

Nicht verwenden: `fix`, `update`, `test`, `geht jetzt`.

## Builds und Releases

Builds werden **außerhalb** des Git-Arbeitsbaums erzeugt. Das Repository enthält keine fertigen PBOs oder privaten Schlüssel.

Ein Release ist erst bereit, wenn:

- Config und Scripts ohne relevante Fehler laden
- PBO-Prefix und Dateipfade stimmen
- PBOs gebaut und signiert sind
- Server-RPT und Script-Logs geprüft wurden
- Abhängigkeiten und Changelog aktuell sind
- die Testmatrix abgeschlossen ist
- keine Zugangsdaten oder fremden Dateien enthalten sind

Siehe [`docs/BUILD.md`](docs/BUILD.md), [`docs/TESTING.md`](docs/TESTING.md) und [`docs/RELEASE.md`](docs/RELEASE.md).

## Lizenz

Der selbst entwickelte Inhalt dieses Repositorys ist proprietär und steht unter **All Rights Reserved**. Fremde Bestandteile behalten ihre jeweilige Lizenz. Details stehen in [`LICENSE`](LICENSE).

DayZ und Bohemia Interactive sind Marken ihrer jeweiligen Rechteinhaber. Dieses Projekt ist kein offizielles Projekt von Bohemia Interactive.
