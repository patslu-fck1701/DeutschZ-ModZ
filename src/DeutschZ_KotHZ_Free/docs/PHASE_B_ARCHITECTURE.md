# DeutschZ KotHZ Free - Phase B

Phase B erweitert den serverautoritativen Kern um genau einen Vanilla-Mast, genau eine angehaengte Fahne, konfigurierbaren Capture-Radius, serverseitige Spielerpruefung, Fahnenfortschritt, synchronisierte Rauchzustaende und zentrales Cleanup.

## Laufzeit

- `DZKOTHF_EventController`: Zustandswechsel, serverseitiger Capture-Takt und Zeitsteuerung.
- `DZKOTHF_EventSession`: alleiniger Besitzer der Laufzeitreferenzen auf Mast und Fahne sowie Capture- und Rauchzustand.
- `DZKOTHF_EventFlagpole`: Vanilla-`StaticFlagPole` mit synchronisiertem Fahnenfortschritt und M18-Rauchpartikeln.
- Historischer Phase-B-Pfad (ersetzt): `$profile:DeutschZ/KotHZ_Free/KotHZFreeSettings.json`.
- Aktuell verbindlich: `$profile:DeutschZ-System/DeutschZ_KotHZ/KotHZFreeSettings.json`; Logs getrennt unter `$profile:DeutschZ-System/LogZ/DeutschZ_KotHZ/KotHZ.log`.

## Rauch

- Weiss: `ANNOUNCED`, Capture abgeschlossen/inaktiv.
- Gruen: aktive Capture-Phase, null oder genau ein Spieler im Radius.
- Rot: mehrere lebende Spieler im Radius oder Abbruch.

Bei mehr als einem Spieler pausiert der Fortschritt. Nur genau ein lebender Spieler im konfigurierten Radius erhoeht den Fahnenfortschritt.

## Nicht enthalten

Keine Gegner, Wellen, Bosse, Rewards, Marker, Notify, Progressbar, Terminals, KOTHGate oder Premiumfunktionen.
