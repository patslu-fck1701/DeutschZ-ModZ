# LicenseCore Gate 3 - technischer Blocker

Stand: 16.07.2026, 06:16 Uhr

## Ergebnis

Gate 3 ist **BLOCKIERT**. Es wurde bewusst kein `DeutschZ_LicenseCore`, keine
Lizenzklasse und keine scheinbare Signaturpruefung angelegt.

Die verbindliche Anforderung lautet: Eine lokale Lizenzdatei muss mit einer
starken, asymmetrischen und im Mod nur ueber den oeffentlichen Schluessel
pruefbaren Signatur validiert werden. Der private Schluessel darf niemals im
PBO liegen und eine Onlineverbindung darf nicht erforderlich sein.

## Gepruefte Quellen

Geprueft wurden die lokal installierten Vanilla-Scriptmodule unter:

- `P:\scripts\1_Core`
- `P:\scripts\3_Game`
- `P:\scripts\4_World`
- `P:\scripts\5_Mission`
- die lokal installierten DayZ-Tools-Referenzen

Gesucht wurde gezielt nach RSA, ECDSA, Ed25519, PublicKey,
VerifySignature, Asymmetric, Crypto, SHA-256/SHA-512 und vergleichbaren
Signatur- oder Kryptografie-APIs.

Ergebnis:

- Keine asymmetrische Signaturpruefung ist in der fuer Enforce Script
  verfuegbaren lokalen API vorhanden.
- `P:\scripts\1_Core\DayZ\proto\EnString.c` stellt lediglich
  `proto native int Hash();` bereit.
- Dieser Integer-Hash ist weder kollisionssichere Kryptografie noch eine
  Signaturpruefung und kann keinen nicht im PBO enthaltenen privaten
  Schluessel ersetzen.

Bewertung:

- `BESTAETIGT`: Die installierte Enforce-Script-API stellt keine belastbare
  RSA-/ECDSA-/Ed25519-Verifikation bereit.
- `WIDERLEGT`: Eine starke, rein in Enforce Script implementierte lokale
  Signaturpruefung kann mit den vorhandenen APIs sicher umgesetzt werden.
- `OFFEN`: Ein separat vertrauenswuerdiger lokaler Vorstart-Verifier kann als
  neue Architektur freigegeben werden.

## Bewusst verworfene Scheinloesungen

- `string.Hash()` oder einfache Checksummen
- HMAC mit einem im PBO gespeicherten geheimen Schluessel
- selbst erfundene RSA-, ECDSA- oder Ed25519-Implementierungen
- Verschleierung, Base64 oder String-Splitting als angebliche Kryptografie
- clientseitige oder nur per UI gesperrte Premiumfunktionen

Alle diese Varianten waeren vom Besitzer der Moddateien analysierbar oder
manipulierbar und erfuellen die Sicherheitsanforderung nicht.

## Sichere Architekturvarianten fuer eine neue Freigabe

1. **Lokaler Vorstart-Verifier (empfohlen fuer weitere Pruefung)**
   Ein separates, signiertes Windows-Werkzeug validiert die Lizenz mit einer
   etablierten Kryptobibliothek vor dem DayZ-Serverstart. Es schreibt nur ein
   kurzlebiges, servergebundenes Ergebnis in den Profilordner. Trust Boundary,
   Replay-Schutz, Prozessintegration und Manipulationsschutz muessen vor einer
   Umsetzung spezifiziert werden. Es ist kein externer Lizenzserver noetig,
   aber dies ist nicht mehr rein Enforce Script.

2. **Gepruefte native Erweiterung**
   Eine eng begrenzte native Komponente stellt nur die Signaturverifikation
   bereit. DayZ-Kompatibilitaet, Deployment, Angriffsoberflaeche und
   Updatepflege muessen zuerst technisch belegt werden.

3. **Online-Lizenzdienst**
   Kryptografisch machbar, aber durch die aktuelle Vorgabe "keine externe
   Server-/Onlinepflicht" ausgeschlossen.

4. **Kundenspezifische Builds ohne LicenseCore**
   Eine Auslieferungsstrategie, aber keine belastbare Laufzeitlizenz und daher
   kein Ersatz fuer Gate 3.

## Gate-Folge

Gemaess Nachtauftrag wurden nach diesem Blocker nicht begonnen:

- Phase 4 KotHZ Premium
- Phase 5 MenuZ V2
- Phase 6 HUDZ
- Phase 7 RadioMissionZ

Die bereits gebauten und signierten Phase-1/2-Ausgaben bleiben unveraendert.
