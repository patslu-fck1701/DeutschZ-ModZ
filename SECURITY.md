# Sicherheit

## Niemals committen

- Steam-, GitHub-, Discord- oder Server-Zugangsdaten
- Passwörter, Tokens, API-Schlüssel und Webhooks
- private PBO-Signaturschlüssel (`*.biprivatekey`)
- Zertifikate und private Schlüssel
- reale `serverDZ.cfg`-Dateien
- Spieler-, Profil- oder Datenbankdaten

## Versehentlich veröffentlichte Geheimnisse

1. Zugang oder Schlüssel sofort widerrufen beziehungsweise ersetzen.
2. Betroffene Datei aus dem aktuellen Stand entfernen.
3. Git-Verlauf mit einem geeigneten Werkzeug bereinigen.
4. Remote-Historie kontrolliert aktualisieren.
5. Logs und Zugriffe prüfen.
6. Vorfall dokumentieren, ohne das Geheimnis erneut zu veröffentlichen.

Nur das Löschen in einem späteren Commit reicht nicht aus.

## Schwachstellen

Sicherheitsprobleme nicht als öffentliches Issue mit Exploitdetails melden. Nutze einen privaten Kontakt zum Repository-Eigentümer oder GitHub Private Vulnerability Reporting, sofern aktiviert.
