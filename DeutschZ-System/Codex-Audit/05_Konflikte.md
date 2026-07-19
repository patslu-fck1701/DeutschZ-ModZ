# Konflikte

## KONFLIKT-001

- Betroffene Dateien: Neuer Auditauftrag; Reale Dateisystemstruktur
- Betroffene Regeln: Entwicklung/Ausgabe/Settings-Pfade
- Problem: Auftrag nennt E:\DeutschZ\DeutschZ Mods, DeutschZ Server und DeutschZ Server Settings; diese Pfade existieren nicht.
- Auswirkung: Gefahr einer zweiten konkurrierenden Quell- und Ausgabestruktur.
- Aktueller Gewinner: Reale und bisher verbindliche Pfade: DeutschZ-ModZ, DeutschZServer, DeutschZ-Server-Settings.
- Empfohlene Bereinigung: Nach externer Freigabe Pfadstandard auf reale Verzeichnisse korrigieren.
- Risiko: KRITISCH

## KONFLIKT-002

- Betroffene Dateien: Workspace AGENTS.md; project-doc im selben Regelblock
- Betroffene Regeln: Vanilla-only kontra Expansion erlaubt
- Problem: Der obere Regelteil verbietet Expansion, der eingebettete project-doc erlaubt vorhandene Expansion-Abhaengigkeiten.
- Auswirkung: Unklare API-Auswahl bei KotHZ/Serverprojekten.
- Aktueller Gewinner: Konkreter project-doc und neuere Benutzeranweisung fuer bestehende Expansion-Projekte.
- Empfohlene Bereinigung: Regel nach Produkt aufteilen: Vanilla-Free versus Expansion-Pro.
- Risiko: HOCH

## KONFLIKT-003

- Betroffene Dateien: C:\Users\patsl\.codex\skills\_archived_old_deutschz_dayz_skills; C:\Users\patsl\.codex\skills\_skill_backups
- Betroffene Regeln: DayZ-Skilltrigger
- Problem: Gleichnamige DayZ-Skills liegen mehrfach in Archiv und zwei Backups vor.
- Auswirkung: Mehrdeutige Auswahl und veraltete Anweisungen koennen aktiviert werden.
- Aktueller Gewinner: In dieser Aufgabe explizit geladener archivierter dayz-modding Skill.
- Empfohlene Bereinigung: Nach Kontrolle genau eine aktive Skillfamilie behalten, Backups ausserhalb aktiver Skillwurzel archivieren.
- Risiko: HOCH

## KONFLIKT-004

- Betroffene Dateien: Neuer Auditauftrag; Bestehende DeutschZ-Ausgaberegel
- Betroffene Regeln: FREE/PRO-Keytrennung versus aktuell verwendeter DeutschZ-Key
- Problem: Audit-Ziel verlangt getrennte FREE/PRO-Keys, bestehende Produkte verwenden teilweise den gemeinsamen DeutschZ-Key.
- Auswirkung: Release- und Signaturstandard ist nicht einheitlich.
- Aktueller Gewinner: Tatsaechlich gebauter Produktstand; keine Schluessel werden im Audit geaendert.
- Empfohlene Bereinigung: Separate Schluesselstrategie als eigener freigegebener Migrationsauftrag.
- Risiko: HOCH

## KONFLIKT-005

- Betroffene Dateien: Neuer Auditauftrag; Verbindliche Profilpfadregel
- Betroffene Regeln: Profilordner
- Problem: Audit nennt <Modname>, aktive Regel verlangt DeutschZ_<MODNAME> unter DeutschZ-System.
- Auswirkung: Settings und Logs koennen in parallelen Ordnern landen.
- Aktueller Gewinner: Aktive AGENTS-Regel mit DeutschZ_<MODNAME> und separatem LogZ.
- Empfohlene Bereinigung: Audit-Zielstandard an aktive Namenskonvention angleichen.
- Risiko: MITTEL

## KONFLIKT-006

- Betroffene Dateien: MASTER.md-Suche
- Betroffene Regeln: Zentraler Masterstandard
- Problem: Keine eindeutige MASTER.md im echten Mod- oder Server-Settings-Repository gefunden.
- Auswirkung: Standards verteilen sich auf AGENTS, Skills, Handoffs und Auftragsdateien.
- Aktueller Gewinner: AGENTS.md plus aktuelle Benutzeranweisung.
- Empfohlene Bereinigung: Nach Audit einen einzigen kontrollierten Masterstandard bestimmen.
- Risiko: MITTEL

## KONFLIKT-007

- Betroffene Dateien: DiscordBot .env; Discord-Guild nach Neuaufbau
- Betroffene Regeln: Kanal-IDs
- Problem: Eine alte Log-Kanal-ID verwies auf einen nicht mehr existierenden Discord-Kanal.
- Auswirkung: Freigabe-/Ergebnisnachrichten konnten nicht zugestellt werden.
- Aktueller Gewinner: Am 2026-07-19 auf vorhandene server-management/bot-logs Kanal-IDs korrigiert.
- Empfohlene Bereinigung: Kanal-IDs nach jedem Guild-Rebuild automatisch neu aufloesen und validieren.
- Risiko: HOCH

