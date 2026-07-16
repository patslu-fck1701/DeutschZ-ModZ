# KotHZ Free Phase C1 - Testanleitung

## Profilpfade

- Config: `$profile:DeutschZ-System/DeutschZ_KotHZ/KotHZFreeSettings.json`
- Log: `$profile:DeutschZ-System/LogZ/DeutschZ_KotHZ/KotHZ.log`
- Hoster-Config: `/gameserver/profiles/DeutschZ-System/DeutschZ_KotHZ/KotHZFreeSettings.json`
- Hoster-Log: `/gameserver/profiles/DeutschZ-System/LogZ/DeutschZ_KotHZ/KotHZ.log`

Der fruehere Pfad `$profile:DeutschZ/KotHZ_Free/KotHZFreeSettings.json` wird nur zur einmaligen Migration gelesen. Die alte Datei wird nicht geloescht. Existieren beide Dateien, ist der neue Pfad autoritativ.

## Servertest

1. Nur `@DeutschZ_KotHZ_Free` und den passenden oeffentlichen BIKEY laden.
2. Server bis Missionstart laufen lassen.
3. Im Scriptlog `Controller initialized`, `State IDLE -> ANNOUNCED`, `State ANNOUNCED -> ACTIVE` und `Enemy block spawned: requested=15 created=15` pruefen.
4. Im Profil die getrennt erzeugten Config- und Logdateien pruefen.
5. Bei Abbruch oder Cleanup muss `Enemy cleanup completed` erscheinen; beim naechsten Event darf kein Doppelspawn entstehen.

## Noch offener Gameplaynachweis

Der Compile-/Missionstarttest beweist Spawn und Cleanup im Serverlauf, aber keine Sichtbarkeit, AI-Navigation oder Kampfbalance mit einem echten Client.
