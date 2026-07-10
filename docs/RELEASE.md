# Release

## Versionierung

Semantic Versioning:

```text
MAJOR.MINOR.PATCH
```

Für Entwicklungsstände sind `0.x.y`-Versionen zulässig.

## Release-Checkliste

- [ ] Zielversion festgelegt
- [ ] `CHANGELOG.md` aktualisiert
- [ ] Abhängigkeiten und Workshop-IDs geprüft
- [ ] Asset-Lizenzen vollständig
- [ ] Repository Guard erfolgreich
- [ ] alle vorgesehenen PBOs gebaut
- [ ] alle PBOs signiert
- [ ] öffentlicher Key vorhanden
- [ ] keine privaten Schlüssel im Paket
- [ ] lokaler Serverstart erfolgreich
- [ ] relevante RPT-/Script-Logs sauber
- [ ] Testmatrix abgeschlossen
- [ ] Release-Paket außerhalb des Source-Repos erstellt
- [ ] Git-Tag gesetzt

## Tag-Beispiel

```bash
git tag -a v0.2.0 -m "DeutschZ ModZ v0.2.0"
git push origin v0.2.0
```

## Release-Inhalt

Ein binäres Release kann enthalten:

```text
@DeutschZ_ModName/
├── addons/
│   ├── modname.pbo
│   └── modname.pbo.<keyname>.bisign
├── keys/
│   └── <keyname>.bikey
├── mod.cpp
└── README.md
```

Der private `*.biprivatekey` gehört niemals in das Release.
