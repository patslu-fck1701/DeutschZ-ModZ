# Codex-Auftrag – DeutschZ ModZ

## Ziel

Beschreibe das konkrete, messbare Endergebnis.

## Fehlerliste

1. Fehler mit beobachtetem Verhalten
2. erwartetes Verhalten
3. relevante Klassen, Dateien und Logs

## Priorität

1. blockierender Fehler
2. Funktionsfehler
3. UI/Asset-Fehler
4. Cleanup/Logging

## Arbeitsauftrag

- vorhandene Struktur vollständig prüfen
- Ursache im Quellcode und in den Config-Patches belegen
- kleinstmögliche saubere Reparatur umsetzen
- Server-/Client-Verantwortung korrekt trennen
- optionale Abhängigkeiten absichern
- Build außerhalb des Repositorys durchführen
- erreichbare statische Prüfungen und Tests ausführen
- RPT-/Script-Logs auswerten
- geänderte Dateien und Ergebnisse dokumentieren

## Testmatrix

- [ ] Build erfolgreich
- [ ] Server startet
- [ ] keine neuen relevanten Fehler
- [ ] Hauptfunktion geprüft
- [ ] Abbruch-/Fehlerfall geprüft
- [ ] Restart/Cleanup geprüft

## Abschlussmeldung

- Ursache
- geänderte Dateien
- Build-Ausgabe
- Tests
- offene Restrisiken
- Status: bereit oder nicht bereit für Live-Test
