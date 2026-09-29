# DeutschZ Points – Abnahmetests v1.1

Diese Testliste gehört zur verbindlichen Architektur in `docs/architecture/DEUTSCHZ_POINTS_SYSTEM.md`.
Kein Status „fertig/live“, bevor die relevanten Tests tatsächlich durchgeführt und dokumentiert wurden.

## Tests / Abnahme
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
10. Tageswechsel funktioniert mit Europe/Berlin korrekt, inklusive Sommer-/Winterzeit-Grenze.
11. Monatswechsel vergibt 75/50/25 nur einmal und löscht Guthaben nicht.
12. Wiederholtes API-Polling erzeugt keine Doppelgutschrift.
13. API-Ausfall stoppt nicht den Server; Retry ohne Datenkorruption.
14. Serverrestart erhält Guthaben, Vote-Snapshot, Händlerbestand und Rotation.
15. Unzugeordnete Voter landen in Pending statt falscher Gutschrift.
16. Nur Morty’s + Anzio Waffen betroffen.
17. Waffe/Magazin/Munition/Munitionsbox konsistent eingeschränkt.
18. FOG-Pouches werden nach Familien gruppiert; Farbvarianten zählen nicht als eigene Familie.
19. Originale FOG-Cargogröße und aktueller Override werden pro Pouch-Familie dokumentiert; eine vorhandene 100-Slot-Pauschalisierung wird eindeutig sichtbar.
20. Nach Rebalance existieren nur die Pouch-Cargostufen 20/30/48/64/80/100.
21. Alle Varianten einer Pouch-Familie erhalten exakt dieselbe Cargostufe und denselben Punktepreis.
22. `itemSize[]`, Attachment-Slots und Kompatibilität bleiben unverändert, sofern keine separat dokumentierte Korrektur nötig ist.
23. Ein legitimer voll ausgerüsteter Pouch-Aufbau liegt ideal bei 120-180 und überschreitet im ersten Entwurf nicht 200 zusätzliche Pouch-Slots.
24. Gefüllte Pouches können nicht als rekursiver Inventar-Multiplikations-Exploit genutzt werden; Fix verursacht keinen Item-/Inhaltsverlust.
25. Pouch-Tier, Punktepreis, Stop-Pool und Bestandslimit stimmen für jede Familie überein.
26. 100-Slot-Pouches erscheinen nur Nordroute/Novomirovsk und höchstens 0-1 pro vollständigem Händlerumlauf.
27. FOG-Rucksackstaffel 100-500 ist logisch und Varianten sind konsistent.
28. Nur 400/450/500-Slot-Rucksäcke sind Punkteware.
29. Stärkste 25 % FOG-Westen werden korrekt aus dem effektiven Override abgeleitet.
30. Helme bleiben unverändert.
31. NVGs sind nur per Punkte kaufbar, aber Eventloot/Verkauf bleibt.
32. Beschränkte Items spawnen nicht normal in CE.
33. Beschränkte Items sind nicht normal für Euro/Dollar kaufbar.
34. Beschränkte Items können weiter verkauft werden.
35. KOTH/Courier/RAVEN/AI Convoy und sonstige bestehende Eventlootquellen bleiben unverändert.
36. Jeder Schwarzmarkt-Stopp zeigt nur seinen eigenen Punktewarenpool, einschließlich korrekter Pouch-Tiers.
37. Nicht jedes Pool-Item muss pro Umlauf verfügbar sein.
38. Ausverkaufte rare Ware respawnt nicht durch Restart.
39. Neuer vollständiger Umlauf erzeugt genau einmal neuen Bestand.
40. Courier: genau 1 Geld-Entity mit Quantity 250 und Gesamtwert 25.000.
41. RPT/Logs nach Test sind frei von neuem Crash-/Error-Spam.
42. Regressionstest des bestehenden EventSchedulers und mobilen Schwarzmarkts besteht.

## Zusätzlich zu dokumentieren
- verwendeter Build/Commit
- getestete Server-/Mod-Versionen
- geänderte Dateien
- relevante RPT-/Script-Log-Auszüge
- festgestellte Regressionen
- Ergebnis pro Test: PASS / FAIL / BLOCKED
- Rollback-Hinweise
