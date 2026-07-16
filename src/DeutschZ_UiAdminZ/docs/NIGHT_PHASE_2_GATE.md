# Nachtauftrag Gate 2 - DeutschZ UiAdminZ

Stand: 16.07.2026, 05:54 Uhr

## Ergebnis

- Eigenstaendige optionale Mod `DeutschZ_UiAdminZ`: PASS
- KotHZ Free ohne UiAdminZ: PASS
- KotHZ Free mit UiAdminZ: PASS
- UiAdminZ ohne KotHZ Free: PASS
- Preflight / Clean Build / PBO-Inhalt / DeutschZ-Signatur: PASS
- Scriptmodule Game, World und Mission: PASS
- Missionstart: PASS
- finaler Lauf Free + UiAdminZ: 05:50 bis 05:54 Uhr stabil
- modbezogene Compile-, Nullpointer- oder Runtimefehler: 0
- Client-/Sichttest: offen, durch Patrick erforderlich

## Architektur

UiAdminZ ist eine reine optionale Client-UI und besitzt keine Eventlogik. KotHZ Free stellt den Vanilla-RPC-Endpunkt bereit und validiert jede Anfrage serverseitig. `requiredAddons[]` von KotHZ Free enthaelt keine UiAdminZ-Abhaengigkeit.

Aktive Admin-Lite-Funktionen:

- Eventstatus
- Standort und Radius
- Capture-Fortschritt
- Gegneranzahl
- Rewardstatus
- kontrollierter Start und Stop
- Diagnoseergebnis

Die Darstellung folgt dem gelieferten 1920x1080-Blueprint mit Header, Sidebar, Dashboard, FREE/PRO-Spalte und gesperrten Premiumbereichen. Es wurden nur vier benoetigte PAA-Icons aus dem unveraenderten Assetpaket uebernommen.

## Sicherheit

- Default `AdminSteamIds` ist leer und verweigert Start/Stop.
- Steam64-ID wird aus `PlayerIdentity.GetPlainId()` serverseitig geprueft.
- Pro Anfrage werden Protokollversion, Aktion, Sender, Rate-Limit und Controllerzustand validiert.
- Start/Stop werden im KotHZ-Servermod entschieden und in `KotHZ.log` protokolliert.
- Status bleibt lesbar; Start/Stop werden fuer nicht berechtigte Clients deaktiviert und serverseitig erneut abgelehnt.

## Profilpfade

- Config: `$profile:DeutschZ-System/DeutschZ_UiAdminZ/UiAdminZSettings.json`
- Log: `$profile:DeutschZ-System/LogZ/DeutschZ_UiAdminZ/UiAdminZ.log`

## Testbelege

- Free + UI Scriptlog: `script_2026-07-16_05-50-13.log`
- Free + UI RPT: `DayZServer_x64_2026-07-16_05-50-05.RPT`
- UI-only Scriptlog: `script_2026-07-16_05-42-37.log`
- UI-only RPT: `DayZServer_x64_2026-07-16_05-42-29.RPT`

Nicht als getestet behauptet: F7-Oeffnung, visuelles Layout, Client-RPC, Admin-/Nicht-Admin-Klick und Gameplay. Diese Punkte benoetigen einen echten Clienttest.
