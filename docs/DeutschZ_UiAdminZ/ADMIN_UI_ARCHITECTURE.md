# DeutschZ UiAdminZ - Architekturentscheidung

## Produktname

Falls die gemeinsame Event-Admin-Oberfläche als eigenständige Mod umgesetzt wird, lautet der verbindliche Name:

`DeutschZ_UiAdminZ`

Andere Namensvarianten werden nicht verwendet.

## Abhängigkeitsregel

`DeutschZ_UiAdminZ` ist immer optional. KotHZ Free, ConvoyZ Free und spätere Eventmods müssen ohne diese UI-Mod vollständig kompilieren, starten, laufen und per JSON beziehungsweise vorhandenen sicheren Adminwegen konfigurierbar bleiben.

Eventmods erhalten deshalb keine `requiredAddons[]`-Pflicht auf `DeutschZ_UiAdminZ`.

## Verantwortlichkeiten

### DeutschZ_UiAdminZ

- gemeinsame Client-Layouts
- Panels, Buttons und Icons
- Fenster, Tabs und Navigation
- gemeinsames Farb- und Designsystem
- allgemeine Adminrechte-Basis auf Clientseite
- optionale Adapter für stabile Eventmod-Schnittstellen
- Erkennung geladener Eventmodule und Ausblenden nicht verfügbarer Tabs

### DeutschZ_KotHZ_Free

- vollständige KotHZ-spezifische Eventlogik
- serverautoritiver Status und Zustandswechsel
- Eventstart und kontrollierter Stop
- Radius, Fortschritt, Gegner und Rewards
- Settings, Marker, Notify und Cleanup
- serverseitige Rechte-, Distanz-, Zustands- und Payloadprüfung
- optionale, stabile RPC-/API-Schnittstellen für eine UI

### DeutschZ_KotHZ_Premium

- spätere Pro-Funktionen wie Wellen, AI, Gruppen und erweiterte Einstellungen
- spätere Lizenzprüfung ausschließlich in der Premium-Schicht

## Laufzeit ohne UI-Mod

Ohne `DeutschZ_UiAdminZ` müssen weiterhin funktionieren:

- automatischer oder bestehender administrativer Eventstart
- JSON-Settings
- Marker und Notify
- Capture, Rewards und Cleanup
- vollständiger Serverstart ohne fehlende Klassen oder Nullzugriffe

## Laufzeit mit UI-Mod

Die UI ist ausschließlich eine Komfortschicht für:

- Statusanzeige
- Start/Stop-Schaltflächen
- Diagnosewerte
- später freigegebene Editor- und Premiumfunktionen

Jede UI-Anforderung wird serverseitig vom jeweiligen Eventmod validiert. Die UI trifft keine finale Event-, Reward- oder Berechtigungsentscheidung.

## Free-/Pro-Grenze

Admin Lite wird als `FREE_OPTIONAL` geplant:

- Eventstatus
- Standort
- Radius
- Fortschritt
- Gegneranzahl
- Rewardstatus
- Start
- kontrollierter Stop
- Diagnose

Admin Pro wird als `PREMIUM_CORE` geplant:

- Standort-, Wellen- und Rewardeditor
- Smoke-, Marker-, Notify- und Progressbar-Editor
- Expansion AI und Groups
- Zeitpläne, Profile, Import und Export
- späterer Area-/MapEditor

## Aktueller Status

- Assets inventarisiert: JA
- Name auf Kollision geprüft: JA, keine Kollision gefunden
- Quellmod angelegt: NEIN
- Layout implementiert: NEIN
- RPCs implementiert: NEIN
- LicenseCore implementiert: NEIN
- Freigabe zur Implementierung: erst nach bestandenem KotHZ-Free-Compiletest und separater Beauftragung
