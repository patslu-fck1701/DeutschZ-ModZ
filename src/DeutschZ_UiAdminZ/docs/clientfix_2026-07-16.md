# UiAdminZ Clientfix 2026-07-16

## Behobene Ursachen

- Das Root-Layout wurde geladen, aber die Widget-Erzeugung brach vor `DZUIAZ_Status` ab. Die vier gelieferten PAA-Icons bleiben im PBO, sind bis zur erneuten Clientpruefung jedoch nicht als `ImageWidget` gebunden.
- Alle Pflichtbindungen werden einmalig validiert. Fehlende Widgets erzeugen eine klare Logzeile statt eines Nullpointers pro Frame.
- `UIScriptedMenu.OnShow()` sperrt Maus, Tastatur und Gamepad und zeigt den Cursor. Zusaetzlich wird der Vanilla-Input-Exclude `menu` gesetzt.
- `OnHide()`, `Cleanup()` und `OnMissionFinish()` geben den Input sicher zurueck.
- F7 und Escape schliessen das Menue.

## Erneuter Clienttest

1. Nur `@DeutschZ_UiAdminZ` laden und mit F7 oeffnen.
2. Vollstaendiges Admin-Lite-Layout pruefen.
3. Beim offenen Menue Schuss, Zielen, Kamera, Inventar und Aktionen pruefen: keine Spielaktion darf ausgeloest werden.
4. Mit F7 und Escape schliessen und danach normale Steuerung pruefen.
5. Client-RPT auf `[DeutschZ UiAdminZ] Admin Lite layout and widget bindings loaded.` pruefen.

Ein visueller Clienttest bleibt erforderlich.
