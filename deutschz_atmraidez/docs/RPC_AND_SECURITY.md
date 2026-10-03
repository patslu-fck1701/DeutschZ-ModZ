# RPC- und Security-Liste

- `1701180100` Client -> Server: Admin command. Identity/Player-ID-Abgleich, AdminUID, Whitelist
  und 750-ms-Rate-Limit werden serverseitig validiert.
- `1701180101` Server -> Client: Progress.
- `1701180102` Server -> Client: Nachricht.
- `1701180103` Server -> Client: Effekt Start/Stop.
- `1701180104` Server -> Client: Adminstatus.

Admin-Whitelist: `status`, `reload`, `cancelall`, `cancelnearest`, `clearnearest`, `locknearest`,
`unlocknearest`, `debug_on`, `debug_off`. Es existiert kein generischer Command-Executor.

Raidstart, Spielerzustand, Distanz, Toolbesitz, Toolzustand, ATM-/Player-Cooldown, aktive
Spieler-/ATM-Session und Auszahlung werden serverseitig bestimmt.

