# KotHZ Free Phase C5 - Cleanup und Testhaertung

- `RemoveActiveEventCalls` entfernt Start, Aktivierung, Enemy-Spawn, Capture-Tick und Cleanup-Callback zentral.
- Gegner, Smoke, Mast und genau eine Fahne werden ueber `DZKOTHF_EventSession` entfernt.
- Marker und Progressbar erhalten vor World-Cleanup einen eindeutigen Hide-Zustand.
- Die RewardCrate bleibt nach Erfolg nur fuer `RewardLifetimeMinutes`; bei Missionsende wird sie entfernt.
- Vor einem neuen Event wird eine noch vorhandene RewardCrate entfernt, wodurch kein Doppelspawn moeglich ist.
- Es gibt keine Restart-Persistenz und keinen Resume-Anspruch. Jeder Prozessstart beginnt in `IDLE` und laedt nur Settings.

Echter Abbruch, Capture, Reward-Lifetime, Disconnect und Restart bleiben Patricks Gameplaytest.
