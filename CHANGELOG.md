# Changelog

Alle relevanten Änderungen an DeutschZ ModZ werden hier dokumentiert. Technische Builds und Serverstarts sind von einem bestätigten Client-Gameplaytest getrennt ausgewiesen.

## [Unreleased]

### Added

- `DeutschZ_HalftanZ` als Testpaket mit `SurvivorM_Halftan`, `SurvivorM_Halftan2`, `SurvivorF_DeutschZNaked` und `SurvivorF_DeutschZSexy`.
- Frauenvarianten verwenden die realen Vanilla-Selektionen `camofemale` und `personality` auf den Vanilla-Unterwäschemodellen.

### Validation

- HalftanZ: Preflight ohne Fehler oder Warnungen, PBO mit `DeutschZ` signiert, Signatur geprüft und dedizierter Serverstart erfolgreich.
- Visueller Clienttest der vier Charaktervarianten bleibt offen.

## [KotH 0.4.5-eventruntime] - 2026-07-14

### Fixed

- Eventmast und Eventflagge sind wieder zur Laufzeit erzeugbar, ohne im Editor-/Spawner-Katalog angeboten zu werden.
- Verwaiste Masten und alte sichtbare `DZEV_`-Gameplayobjekte werden bereinigt.
- Infected-Bewegung, Mumiengeschwindigkeit, Reward-Loot und Mastaktionen wurden korrigiert.
- Zwei aktuelle KOTH-Karten-/Empfängertexturen wurden aufgenommen.

### Changed

- Zufällige, persistente Rotation über elf konfigurierte Eventzonen.
- Genau eine Eventflagge pro aktivem Mast.

### Validation

- Preflight, PBO-Inhalt, `DeutschZ`-Signatur und dedizierter Serverstart bestätigt.
- Laufzeittest wählte nach Konfigurationswechsel `YRAP` aus elf Zonen und erzeugte Mast, Rauch und Marker.
- Vollständiger Client-Gameplayablauf bleibt offen.

## [CriminalZ 0.1.1] - 2026-07-14

### Fixed

- `DZCRZ_ATM` erbt vom funktionsfähigen `ExpansionATM_2` und nutzt das neue DeutschZ-Design.
- City-Store-Platzierung migriert alte ATM-Objekte ohne Duplikate.
- Store-Schild verwendet das Expansion-Schildmodell mit neuer Textur.
- Grüne Crowbar-Textur und vorzeitiger Hackabbruch wurden korrigiert.

### Validation

- 19 Stores erkannt: 11 `Land_City_Store`, 8 `Land_City_Store_WithStairs`.
- 19 ATMs und 19 Schilder erzeugt, 0 Fehler, Referenztransformtest bestanden.
- Clientseitiger ATM-/Hack-Gameplaytest bleibt offen.

## [DMarkZ 2.0.0-expansion] - 2026-07-14

### Changed

- Neun DMark-Werte verwenden jetzt die drei echten Expansion-Banknotenmodelle und die vorgegebene Wertzuordnung.
- Kompatibilitätsklassen für die bisherigen `NoxZ_Euro*`-Namen verweisen auf die neuen DMark-Klassen.
- Alte Einzeltexturen wurden durch drei aktuelle Expansion-Texturatlanten ersetzt.

### Validation

- Preflight, PBO-Inhalt, `DeutschZ`-Signatur und dedizierter Serverstart bestätigt.
- Visuelle Zuordnung aller neun Scheine im Client bleibt offen.

## [0.1.0] - 2026-07-10

### Added

- Initiales DeutschZ-ModZ-Repository.
