# KotHZ Free – Reward-Simulation (10 Läufe)

Statische, deterministische Auswahlprüfung mit Seed `20260717`. Sie belegt die Verteilungslogik, ersetzt aber keinen DayZ-Inventar-/Cargo-Laufzeittest.

| Lauf | Garantierte Waffe | Zufallswaffen | Auswahl | Waffen gesamt | Eindeutig |
|---:|---|---:|---|---:|---|
| 1 | AKM | 1 | Mosin9130 | 2 | JA |
| 2 | M16A2 | 1 | Winchester70 | 2 | JA |
| 3 | SKS | 2 | MP5K, Mosin9130 | 3 | JA |
| 4 | M4A1 | 3 | FAL, AK74, Mosin9130 | 4 | JA |
| 5 | Mosin9130 | 1 | FAL | 2 | JA |
| 6 | SKS | 2 | MP5K, AK74 | 3 | JA |
| 7 | Mosin9130 | 1 | SKS | 2 | JA |
| 8 | M4A1 | 2 | SKS, UMP45 | 3 | JA |
| 9 | Winchester70 | 2 | AKM, AK74 | 3 | JA |
| 10 | AKM | 1 | SKS | 2 | JA |

Erwartung: je Lauf genau 1 garantierte Waffe plus 1–3 Zufallswaffen; bei `AllowDuplicateWeapons=false` keine doppelte Waffenklasse.
