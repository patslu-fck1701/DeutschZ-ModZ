# Build

## Voraussetzungen

- DayZ Tools und benötigte externe Mods lokal installiert
- P:-Drive beziehungsweise korrekter Workdrive eingerichtet
- Addon Builder oder dokumentiertes gleichwertiges Buildwerkzeug
- privater Signaturschlüssel sicher außerhalb des Repositorys
- Ausgabeverzeichnis außerhalb des Git-Arbeitsbaums

## Quellpaket prüfen

Jede PBO-Quelle benötigt mindestens:

```text
MyPboSource/
├── config.cpp
├── $PBOPREFIX$
└── scripts/ oder data/
```

Prüfen:

- `CfgPatches` eindeutig und korrekt
- `requiredAddons[]` vollständig
- Script-Modulpfade korrekt
- Groß-/Kleinschreibung der Pfade konsistent
- Textur-, Material-, Modell- und Soundpfade vorhanden
- keine absoluten lokalen Pfade
- Prefix stimmt mit den referenzierten Pfaden überein

## Lokale Repo-Prüfung

```bash
python tools/repo_guard.py
```

## Packaging

1. Quellordner als einzelne PBO bauen.
2. Ausgabe außerhalb dieses Repositorys speichern.
3. PBO mit privatem Schlüssel signieren.
4. PBO, `*.bisign` und öffentlichen `*.bikey` nur in den Release-/Serverprozess übernehmen.
5. Private Schlüssel niemals kopieren oder hochladen.

## Nach dem Build

- Client- und Server-Modordner kontrollieren
- PBO-/Signaturpaar prüfen
- öffentlichen Key auf dem Testserver prüfen
- Server starten
- RPT und Script-Logs prüfen
- Funktionstest gemäß Testmatrix durchführen
