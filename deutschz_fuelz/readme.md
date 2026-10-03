# DeutschZ FuelZ

Eigenstaendige Tankstellen-Mod mit serverautorisiertem Euro-Kauf.

- NPC: `DZ_FuelZ_NPC`
- Standard-/Fallbackpreis: 5 Euro pro Liter
- Optionaler Tagespreis: Mittelwert offener Super-E10-Tankstellen im 25-km-Umkreis von Berlin ueber Tankerkoenig/MTS-K.
- Fuer den Tagespreis muss der persoenliche API-Key nur serverseitig in `TankerkoenigApiKey` eingetragen werden.
- Aktualisierung beim Serverstart und danach alle 24 Stunden; bei Ausfall bleibt der letzte gueltige Cache aktiv.
- Schein in die Hand nehmen: Der Scheinwert bestimmt die maximal gekaufte Menge.
- Volltanken: Der exakte Fehlbetrag wird aus dem Inventar bezahlt.
- Fahrzeug muss sich innerhalb von 20 Metern zum NPC befinden.
- Alle 19 vanilla `Land_FuelStation_Build`-Shops auf Chernarus plus der zusaetzliche Shop bei 4394/10820 sind vorkonfiguriert (20 NPCs).
- 14 vom Betreiber gemessene NPC-Positionen werden exakt verwendet; sechs weitere werden relativ zum realen Gebaeude gesetzt.
- Zapfsaeulen geben kein kostenloses Benzin ab. Der Server fuellt das Fahrzeug erst nach erfolgreicher Bargeldentnahme.
- Der Tankwart akzeptiert `ExpansionBanknoteEuro` aus Hand und gesamtem Spielerinventar; jede Mengeneinheit des Scheins gilt als 100 Euro.
- Der Tankwart kann nicht von KI/Kreaturen als Ziel ausgewaehlt werden und bleibt unverwundbar.
- Der Literpreis wird auf Cent gerundet und mit zwei Nachkommastellen angezeigt. Bei gesetztem Tankerkoenig-API-Schluessel wird er beim Serverstart und danach taeglich aktualisiert.
- Der Tankwart traegt ein Vanilla-Arbeitsoutfit aus grauer Bomberjacke, Warnweste, schwarzer Cargohose, Arbeitshandschuhen, Arbeitsschuhen, schwarzer Baseballkappe, Pilotenbrille und schwarzer Huefttasche.
- Bezahlung ausschliesslich mit Inventar-Bargeld, niemals vom Bankkonto.
- Servereinstellungen: `$profile:DeutschZ_FuelZ/Settings.json`

Abhaengigkeiten: CF, DayZ Expansion Core und DayZ Expansion Market. Als Bargeld wird ausschliesslich `ExpansionBanknoteEuro` mit dem Wert 100 verwendet.

Die gelieferte Referenzposition ist die Tankstelle bei `6874.834473 8.880091 3094.625000`.
Der dortige NPC steht exakt bei `6872.729004 7.306866 3094.652100`, Orientierung `125.999992 0 0`.
Bei den weiteren Shops wird derselbe lokale Innenraum-Offset per `ModelToWorld` auf das reale Gebaeude angewendet.
