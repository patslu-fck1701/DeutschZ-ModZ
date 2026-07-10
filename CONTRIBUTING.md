# Mitwirken an DeutschZ ModZ

## Grundregeln

- Nur eigene oder eindeutig lizenzierte Quellen und Assets committen.
- Keine kompletten Fremdmods, PBOs, Signaturen oder privaten Schlüssel einfügen.
- Keine absoluten lokalen Benutzerpfade in produktiven Dateien fest einbauen.
- Serverentscheidungen und Rewards serverautoritativ implementieren.
- Expansion-spezifische UI darf die Kernlogik nicht blockieren, sofern ein Fallback vorgesehen ist.
- Änderungen klein, prüfbar und thematisch getrennt halten.

## Branch-Namen

```text
feature/koth-terminal
feature/convoy-blackbox
fix/eventitems-baseclasses
fix/screen-menu-scaling
hotfix/koth-start
```

## Vor dem Commit

```bash
python tools/repo_guard.py
```

Zusätzlich:

1. Config-Patches und PBO-Prefix prüfen.
2. PBO außerhalb des Repositorys bauen.
3. Serverstart durchführen.
4. RPT-, Script- und Crash-Logs prüfen.
5. betroffene Testmatrix ausfüllen.
6. `CHANGELOG.md` aktualisieren, wenn die Änderung release-relevant ist.

## Commit-Stil

```text
feat(kothz): add terminal action
fix(convoyz): remove stale marker during cleanup
fix(event-items): correct inventory sizes
refactor(shared): centralize notification adapter
chore(release): prepare v0.3.0
```

## Pull Requests

Der Pull Request muss enthalten:

- Fehler oder Ziel
- betroffene Mod/PBO
- technische Änderung
- Abhängigkeiten
- ausgeführte Prüfungen
- bekannte Restrisiken
- relevante Logauszüge ohne sensible Daten
