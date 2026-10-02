# Testbericht ATM RaideZ

## Ausgefuehrt

- JSON-Beispiel parsebar und UTF-8 ohne BOM.
- `types.xml` wohlgeformt.
- Fremdcode-Scan ausgefuehrt.
- Config-Syntax, PBO-Inhalt, Praefix und Signatur geprueft.
- Separater Dedicated-Compile mit Expansion 1.9.72 bis Mission bestanden.

## NICHT GETESTET

- Clientbeitritt und Sichttest des Dark-Glass-/Giftgruen-Adminmenues.
- Sichtbarkeit und Dauer der Continuous Action an allen vier ATM-Klassen.
- echter Phase-1-/Phase-2-Ablauf, Radiusabbruch, Tod und Disconnect.
- Restartverhalten der persistenten UTC-Cooldowns im Gameplay.
- Auszahlung und partieller Spawnfehler im Gameplay.
- Doppelabschluss/RPC-Lag unter realer Netzlast.
- Expansion-Menue bereits offen, waehrend der Raid startet.
- Marker, Sirene, Smoke, Blinklicht und JIP-Cleanup im Client.
- Nicht-Admin-, Rate-Limit- und Adminbutton-Test mit echtem Client.

Status: BEREIT FÜR TESTSERVER



## Nachtrag 0.1.1

Offen: Dedicated Compile sowie Client-Sichttest, dass `ATM ausrauben` an einem bereits platzierten und an einem konfiguriert gespawnten `ExpansionATM_2` mit Brechstange in der Hand erscheint.

## Nachtrag 0.1.4 - 2026-08-26

- Owner-Test 0.1.3: ATM vorhanden, normales Expansion-Banking funktioniert, Raid-Action fehlt, RobTool-Textur falsch skaliert.
- Serverlogs 07:51-09:05: 16/16 `ExpansionATM_2` gespawnt, 0 Fehler; zwei Wiederholungslaeufe verwendeten alle 16 ATMs wieder.
- Ursache Action: aktive Werkzeugklasse registrierte die Raid-Action nicht in `SetActions()`.
- Ursache Textur: gleichnamige Mediendatei war nicht identisch mit der PAA des tatsaechlich verwendeten CriminalZ-Source.
- 0.1.4 strukturell gebaut, signiert und bis Mission kompiliert.

NICHT GETESTET: Neuer Client-Sichttest fuer Actionanzeige und RobTool-Darstellung sowie kompletter Raid-/Reward-Ablauf.

Status: BEREIT FÜR TESTSERVER
