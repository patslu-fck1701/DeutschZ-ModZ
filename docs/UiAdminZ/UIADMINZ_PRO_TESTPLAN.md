# DeutschZ UiAdminZ Admin Lite + Pro Shell - Clienttest

## Installation

- `@DeutschZ_UiAdminZ` auf Client und Server laden.
- Fuer den Adaptertest zusaetzlich `@DeutschZ_KotHZ_Free` laden.
- Config: `$profile:DeutschZ-System/DeutschZ_UiAdminZ/UiAdminZSettings.json`
- Log: `$profile:DeutschZ-System/LogZ/DeutschZ_UiAdminZ/UiAdminZ.log`
- Testadmin: Steam64 `76561199819501556`.

## UiAdminZ allein

1. F7 oeffnet das vollstaendige A-F-Layout; kein schwarzer Vollbildschirm.
2. Header, Sidebar, Dashboard, Free/Pro/Lizenz, Eventeditor sowie Live-Karte/Logs sind sichtbar.
3. Ohne Eventmod steht eindeutig `EVENTMOD NICHT GELADEN`; es werden keine Livewerte erfunden.
4. Maus ist sichtbar. Schiessen, Zielen, Kamera, Inventar- und Aktionsinput bleiben blockiert.
5. ESC und F7 schliessen. Danach funktioniert der Spielinput wieder vollstaendig.
6. Eine nicht gelistete Steam64 darf das Menue nicht oeffnen.

## KotHZ Free + UiAdminZ

1. F7 oeffnet dasselbe Layout.
2. `AKTUALISIEREN` zeigt Eventzustand, Position, Radius, Capture, Gegner und Reward.
3. Start und Stop werden nur bei geladenem Eventmod aktiviert.
4. Start/Stop werden vom KotHZ-Server erneut gegen dessen Adminliste validiert.
5. Eventmarker, Progressbar, Musik und Cleanup duerfen durch das Adminfenster nicht beeinflusst werden.

## Pro-Shell

- Pro-Bereiche sind sichtbar vorbereitet, aber standardmaessig gesperrt.
- `ProDevelopmentEnabled=true` ist nur ein lokaler Entwicklungsindikator, keine Lizenz.
- Keine Clientdaten oder UI-Schalter duerfen Serverrechte erteilen.
- LicenseCore ist nicht implementiert; daher keine Verkaufs- oder Sicherheitsfreigabe.

## Aufloesungen

- 1920x1080
- 2560x1440
- 1280x720
- optional 3440x1440

Bei einem sichtbaren Fehler Screenshot, Aufloesung, frische Client-RPT und Client-Scriptlog liefern.
