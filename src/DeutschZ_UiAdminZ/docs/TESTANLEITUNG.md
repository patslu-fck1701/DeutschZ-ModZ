# DeutschZ UiAdminZ Admin Lite + Pro Shell

## Profilpfade

- Config: `$profile:DeutschZ-System/DeutschZ_UiAdminZ/UiAdminZSettings.json`
- Log: `$profile:DeutschZ-System/LogZ/DeutschZ_UiAdminZ/UiAdminZ.log`

Eine gueltige Settings-Datei wird bei einem Neustart nur geladen und nicht neu geschrieben. Nur eine fehlende oder beschaedigte Datei wird erzeugt. Eine beschaedigte Originaldatei wird vorher als `UiAdminZSettings.corrupt.backup.json` gesichert.

## Clienttest

1. Als Steam64 `76561199819501556` mit `@DeutschZ_UiAdminZ` verbinden.
2. F7 druecken: A-F-Layout muss sichtbar sein; kein schwarzer Vollbildschirm.
3. Maus bewegen und klicken: Schiessen, Zielen, Kamera und Spielaktionen muessen blockiert bleiben.
4. ESC und F7 muessen das Menue schliessen und die Eingabe vollstaendig zurueckgeben.
5. UiAdminZ allein: Dashboard zeigt `EVENTMOD NICHT GELADEN`, bleibt aber stabil.
6. Mit `@DeutschZ_KotHZ_Free`: Aktualisieren, Event starten und stoppen. Jede Entscheidung bleibt serverautoritativ im Eventmodul.
7. Nicht gelistete Steam64: F7 darf das Menue nicht oeffnen.
8. Pro bleibt standardmaessig sichtbar aber gesperrt. `ProDevelopmentEnabled` ist nur ein lokaler Darstellungs-Schalter und keine Lizenz.

## Sicherheitsstatus

- Adminliste wird nie an Clients uebertragen.
- UI-Authentifizierung besitzt eine eigene rate-limitierte RPC-Schnittstelle.
- Eventaktionen werden weiterhin vom jeweiligen Eventmodul erneut validiert.
- LicenseCore ist nicht implementiert; Pro-Sicherheit ist daher nicht releasefertig.
