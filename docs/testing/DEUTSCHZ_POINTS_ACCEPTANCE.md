# DeutschZ Points – Compile- und Übergabeprüfung v1.2

Diese Datei definiert ausschließlich den Prüf- und Übergabeschritt von Codex.
Live-Server- und Gameplay-Tests gehören ausdrücklich nicht zu diesem Codex-Auftrag.

## Compile-Gate
1. Geänderte DayZ-Sourcen vollständig kompilieren.
2. Alle für diese Änderung benötigten PBOs erfolgreich bauen.
3. Geänderte JSON/XML/CSV/Stringtable-Dateien auf Syntax-/Buildfehler prüfen, soweit relevant.
4. Keine relevanten Script-/Compilefehler akzeptieren.
5. Bei FAIL: Fehler beheben und erneut kompilieren.
6. Bei FAIL: **keine** neuen Artefakte in den Workshop-Arbeitsordner übernehmen.
7. Erst bei vollständigem PASS weiter zur Signierung.

## Signierung
1. Den bereits bekannten bestehenden DeutschZ-Signierschlüssel aus der vorhandenen lokalen Projektumgebung verwenden.
2. Den privaten Key niemals in Repository, Website, Dokumentation, Logs oder Ergebnisdateien kopieren.
3. Die finalen Build-Artefakte signieren.
4. Prüfen, dass zu den betroffenen PBOs die erwarteten Signaturartefakte erzeugt wurden.

## Austausch im bekannten Workshop-Arbeitsordner
1. Nur bei Compile PASS.
2. Vorhandenen, Codex bereits bekannten Workshop-Arbeitsordner benutzen.
3. Nur die durch diesen Auftrag betroffenen Build-Dateien gezielt ersetzen.
4. Keine fremden oder unveränderten Dateien pauschal löschen.
5. Sofern im bestehenden Workflow vorgesehen, vor Austausch lokale Rollback-Kopie der alten Build-Artefakte anlegen.
6. Nach Austausch Dateinamen, Größe/Hash bzw. Zeitstempel und Signaturdateien gegen den erfolgreichen Build prüfen.

## Danach STOP
Codex darf in diesem Auftrag nicht:
- auf Steam Workshop hochladen oder publishen;
- den Server starten oder neu starten;
- RCON für einen Restart verwenden;
- einen Live-Server- oder Gameplay-Test ausführen;
- das System als live/fertig bestätigen.

## Manueller Schritt des Serverbetreibers
Nach Codex:
1. Steam-Workshop-Upload manuell durchführen.
2. Serverrestart manuell durchführen.
3. Der bestehende automatische Steam-Update-Ablauf des Servers übernimmt anschließend die aktualisierte Workshop-Version.
4. Live-/Ingame-Prüfung erfolgt erst danach.

## Abschlussmeldung von Codex
Codex meldet kompakt:
- Compile PASS/FAIL;
- erzeugte PBOs;
- Signierstatus;
- im bekannten Workshop-Arbeitsordner ersetzte Dateien;
- Rollback-Status;
- alle Punkte, die erst nach Upload/Restart live geprüft werden können.
