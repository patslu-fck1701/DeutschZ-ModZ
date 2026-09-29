# DeutschZ Points – Abnahmetests v1.0

Diese Testliste gehört zur verbindlichen Architektur in `docs/architecture/DEUTSCHZ_POINTS_SYSTEM.md`.
Kein Status „fertig/live“, bevor die relevanten Tests tatsächlich durchgeführt und dokumentiert wurden.

## Testmatrix
Mindestens folgende Tests dokumentieren:
1. Source/PBO kompiliert ohne Scriptfehler.
2. JSON/XML/CSV/Stringtable valide.
3. Jede Wertmarkenklasse spawnt korrekt und zeigt deutschen Namen/Wert.
4. Bunker-/Papier-/Feuer-Missbrauch der Wertmarke nicht möglich.
5. Stadt/Farm/Hunting/Police/Military/Mumie erzeugen den richtigen Wert.
6. Kill allein vergibt keine Punkte; erst Einlösen.
7. Marken sind bei Tod lootbar.
8. Vote 1/2/3 eines Tages gibt 5/10/25.
9. Vote 4+ gibt 0 direkte Punkte, zählt aber Monatsranking.
10. Tageswechsel funktioniert korrekt.
11. Monatswechsel vergibt 75/50/25 nur einmal und löscht Guthaben nicht.
12. Wiederholtes API-Polling erzeugt keine Doppelgutschrift.
13. API-Ausfall stoppt nicht den Server; Retry ohne Datenkorruption.
14. Serverrestart erhält Guthaben, Vote-Snapshot, Händlerbestand und Rotation.
15. Unzugeordnete Voter landen in Pending statt falscher Gutschrift.
16. Nur Morty’s + Anzio Waffen betroffen.
17. Waffe/Magazin/Munition/Munitionsbox konsistent eingeschränkt.
18. FOG-Pouches nach effektivem Cargo korrekt erkannt.
19. FOG-Rucksackstaffel 100-500 logisch und Varianten konsistent.
20. Nur 400/450/500-Slot-Rucksäcke Punkteware.
21. Stärkste 25 % FOG-Westen korrekt aus Override abgeleitet.
22. Helme unverändert.
23. NVGs nur per Punkte kaufbar, aber Eventloot/Verkauf bleibt.
24. Beschränkte Items spawnen nicht normal in CE.
25. Beschränkte Items sind nicht normal für Euro/Dollar kaufbar.
26. Beschränkte Items können weiter verkauft werden.
27. KOTH/Courier/RAVEN/AI Convoy und sonstige bestehende Eventlootquellen bleiben unverändert.
28. Jeder Schwarzmarkt-Stopp zeigt nur seinen eigenen Punktewarenpool.
29. Nicht jedes Pool-Item muss pro Umlauf verfügbar sein.
30. Ausverkaufte rare Ware respawnt nicht durch Restart.
31. Neuer vollständiger Umlauf erzeugt genau einmal neuen Bestand.
32. Courier: genau 1 Geld-Entity mit Quantity 250 und Gesamtwert 25.000.
33. RPT/Logs nach Test frei von neuen Crash-/Error-Spam.
34. Regressionstest des bestehenden EventSchedulers und mobilen Schwarzmarkts.

## Zusätzlich zu dokumentieren
- verwendeter Build/Commit
- getestete Server-/Mod-Versionen
- geänderte Dateien
- relevante RPT-/Script-Log-Auszüge
- festgestellte Regressionen
- Ergebnis pro Test: PASS / FAIL / BLOCKED
- Rollback-Hinweise
