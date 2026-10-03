# DeutschZ ToxicZ

Eigenstaendige, serverautoritativ gesteuerte Eventkette:

`toxicz_zone_marker` -> Fundort/Flare -> Hospital I -> Hospital II -> kompletter Schutzanzug -> Riffy -> Blackbox -> Decoder -> T-17 -> Cliffhanger.

## Betrieb

- Einstellungen: `$profile:DeutschZ/ToxicZ/ToxicZSettings.json`
- Zustand: `$profile:DeutschZ/ToxicZ/ToxicZState.json`
- Voraussetzung: CF, Dabs Framework, DayZ Expansion Bundle und Licensed.
- Startvertrag: Ein Spieler mit `toxicz_zone_marker` startet die Kette; das Item wird serverseitig genau einmal verbraucht.
- Auszahlung: 1.000.000 D-MarkZ als `ExpansionBanknoteEuro`, Wert 100 je Einheit. Stackgroessen werden zur Laufzeit aus der Itemkonfiguration gelesen.

## Persistenz und Sicherheit

- Der Starter und alle spaeter beitretenden Parteien werden ueber `PlayerIdentity.GetPlainId()` gespeichert.
- Weitere Spieler mit einem Signalgeraet treten der laufenden Operation bei. Ihr Geraet wird dabei verbraucht und ihre Spur auf die gemeinsamen Ziele gefuehrt.
- Zustandswechsel, Zielmarker, Abschluss und Cliffhanger gelten fuer alle beteiligten Parteien; die Belohnung wird trotzdem nur einmal erzeugt.
- Phasen werden nach jedem Uebergang gespeichert und nach Restart rekonstruiert.
- Die Belohnung wird erst nach erfolgreicher Kisten- und Waehrungserstellung als vergeben gespeichert.
- Fehlgeschlagene Waehrungsstacks werden zurueckgerollt.
- Laufzeitobjekte, AI-Gruppen und Marker werden beim Missionsende bereinigt.

## Offene Livepruefung

Hospital- und Decoderpositionen, AI-Loadout `BanditLoadout`, Expansion-Markersymbol `Biohazard`, Textursichtbarkeit, beide Audiodateien, komplette Spielerroute, Tod/Reconnect/Restart und die tatsaechliche Market-Bewertung der Banknoten muessen im passenden Client-/Testserverlauf geprueft werden.

## Testkorrektur 2026-09-12

- Startszene nutzt die konkret spawnbare Klasse `ZmbM_SoldierNormal` statt der Basisklasse.
- Das geheime Kombinationsdokument definiert die Paper-Selection `zbytek`, damit seine PAA-Textur sichtbar angewendet wird.
- Mehrere Parteien koennen derselben laufenden T-17-Operation beitreten und werden an den gemeinsamen Zielpunkten zusammengefuehrt.
- Persistente Start-, Hospital-, Riffy-, Blackbox- und Decoderphasen stellen ihren Zielmarker nach einem Serverneustart wieder her; auch ein spaeterer Beitritt repariert einen fehlenden Marker.
