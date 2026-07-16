# DeutschZ UiAdminZ Admin Lite - Test

- F7 oeffnet die optionale Admin-Lite-Oberflaeche.
- Erwartetes Layout: breiter DeutschZ-Header, linke Navigation, zentrales KotHZ-Dashboard, rechte FREE/PRO-Spalte und unterer Eventdetailbereich gemaess geliefertem 1920x1080-Blueprint.
- Ohne KotHZ Free muss das Fenster fehlerfrei `KEINE ANTWORT` anzeigen.
- Mit KotHZ Free werden Status, Position, Radius, Capture, Gegner und Reward serverseitig geliefert.
- Steam64-IDs werden ausschliesslich in KotHZ `KotHZSettings.json` unter `AdminSteamIds` gepflegt. Die Defaultliste ist leer und verweigert Start/Stop.
- Nicht-Admins sehen Status, koennen Start/Stop jedoch nicht ausfuehren.
- Jede Start-/Stop-Anfrage wird erneut im KotHZ-Servermod validiert und in `KotHZ.log` auditiert.
- UiAdminZ Config: `$profile:DeutschZ-System/DeutschZ_UiAdminZ/UiAdminZSettings.json`
- UiAdminZ Log: `$profile:DeutschZ-System/LogZ/DeutschZ_UiAdminZ/UiAdminZ.log`
- Clienttest mindestens in 1920x1080 und 2560x1440; zusaetzlich 1280x720 auf Ueberlauf pruefen.
- Die vier Icons `dashboard`, `start_event`, `stop_event` und `refresh` muessen sichtbar und ohne schwarzen/weißen Ersatzhintergrund gerendert werden.
