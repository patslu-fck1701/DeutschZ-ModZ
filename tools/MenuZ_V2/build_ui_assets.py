from __future__ import annotations

import csv
import hashlib
import json
import math
import shutil
from collections import defaultdict
from pathlib import Path

from PIL import Image


KIT = Path(r"C:\Users\patsl\Downloads\DeutschZ_UI_Production_Kit")
REPO = Path(r"E:\DeutschZ\DeutschZ-ModZ")
SOURCE = REPO / "src" / "DeutschZ_MenuMusic_V2_Test"
DOCS = REPO / "docs" / "MenuZ_V2"
OUTPUT = SOURCE / "gui" / "v2_kit"

STATES = (
    "normal", "hover", "pressed", "active", "selected", "disabled",
    "locked", "premium", "error", "warning", "success", "upcoming",
    "completed", "failed", "offline",
)

# Only the assets used by the V2 layout or its implemented interaction states
# enter the PBO. Download originals remain untouched.
SELECTED: dict[str, str] = {
    "01_PANELS/Main_Left_normal.png": "panels/main_left_normal.png",
    "01_PANELS/News_normal.png": "panels/news_normal.png",
    "01_PANELS/Character_Stats_normal.png": "panels/character_stats_normal.png",
    "01_PANELS/Events_4_Cards_normal.png": "panels/events_4_cards_normal.png",
    "01_PANELS/Bottom_Navigation_normal.png": "panels/bottom_navigation_normal.png",
    "01_PANELS/Music_Player_normal.png": "panels/music_player_normal.png",
}

for button in ("PLAY_NOW", "SERVER_SUCHEN", "CHARAKTER", "VOTE", "DISCORD", "WEBSITE"):
    for state in ("normal", "hover", "pressed", "disabled", "selected"):
        SELECTED[f"02_BUTTONS/Text/{button}_{state}.png"] = f"buttons/{button.lower()}_{state}.png"

for icon in ("home", "server", "rules", "support", "events", "shop", "settings", "profile", "power"):
    for state in ("normal", "hover", "pressed", "disabled", "active"):
        SELECTED[f"03_ICONS/Navigation/{icon}_{state}.png"] = f"icons/nav_{icon}_{state}.png"

for event in ("KOTHZ", "CONVOYZ", "GROUNDZERO", "OPERATION_DEUTSCHZ"):
    for state in ("normal", "hover", "pressed", "selected", "disabled", "locked", "active", "warning"):
        SELECTED[f"04_EVENT_CARDS/{event}_{state}.png"] = f"event_cards/{event.lower()}_{state}.png"

for control in ("play", "pause", "prev", "next", "volume"):
    for state in ("normal", "hover", "pressed", "disabled", "active"):
        SELECTED[f"08_CONTROLS/MusicPlayer/{control}_{state}.png"] = f"music/{control}_{state}.png"


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as handle:
        for block in iter(lambda: handle.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest().upper()


def next_power_of_two(value: int) -> int:
    if value <= 1:
        return 1
    return 1 << math.ceil(math.log2(value))


def category(relative: str) -> str:
    rel = relative.replace("\\", "/").lower()
    if "/01_panels/" in f"/{rel}":
        if "music_player" in rel:
            return "MUSIC_PLAYER"
        if "tooltip" in rel:
            return "TOOLTIP"
        if "modal" in rel:
            return "DIALOG"
        if "bottom_navigation" in rel or "main_left" in rel:
            return "NAVIGATION"
        if "character_stats" in rel:
            return "PLAYER_STATUS"
        if "social_media" in rel:
            return "SOCIAL"
        return "PANEL"
    if "/02_buttons/" in f"/{rel}":
        return "BUTTON"
    if "/03_icons/" in f"/{rel}":
        return "ICON"
    if "/04_event_cards/" in f"/{rel}":
        return "EVENT_CARD"
    if "/05_badges/" in f"/{rel}":
        return "BADGE"
    if "/06_tabs/" in f"/{rel}":
        return "NAVIGATION"
    if "/07_notifications/" in f"/{rel}":
        return "DIALOG"
    if "/08_controls/musicplayer/" in f"/{rel}":
        return "MUSIC_PLAYER"
    if "/08_controls/" in f"/{rel}":
        return "BUTTON"
    if "/09_decorations/" in f"/{rel}" or "/10_frames/" in f"/{rel}":
        return "DECORATION"
    if "/11_blank_templates/" in f"/{rel}":
        return "PANEL"
    if "background" in rel:
        return "BACKGROUND"
    if "loading" in rel:
        return "LOADING"
    if "death" in rel:
        return "DEATHSCREEN"
    return "UNKNOWN"


def state_for(name: str) -> str:
    stem = Path(name).stem.lower()
    for state in STATES:
        if stem.endswith("_" + state) or ("_" + state + "_") in stem:
            return state.upper()
    return "NORMAL"


def target_for(relative: str, cat: str) -> str:
    if relative in SELECTED:
        return SELECTED[relative].replace(".png", ".paa")
    if cat == "EVENT_CARD":
        return "Eventkartenreserve"
    if cat in ("BUTTON", "ICON", "NAVIGATION", "MUSIC_PLAYER"):
        return "UI-State-Reserve"
    if cat in ("PANEL", "PLAYER_STATUS", "SOCIAL", "TOOLTIP", "DIALOG"):
        return "Panel-Reserve"
    return "Nicht zugeordnet"


def inspect_image(path: Path) -> tuple[str, str, str]:
    if path.suffix.lower() not in (".png", ".jpg", ".jpeg"):
        return "", "", "NEIN"
    with Image.open(path) as image:
        width, height = image.size
        ratio = f"{width / height:.6f}" if height else ""
        alpha = "JA" if "A" in image.getbands() else "NEIN"
        return f"{width}x{height}", ratio, alpha


def padded_copy(source: Path, destination: Path) -> dict[str, object]:
    with Image.open(source) as opened:
        image = opened.convert("RGBA")
        width, height = image.size
        canvas_width = next_power_of_two(width)
        canvas_height = next_power_of_two(height)
        # Fill as much of the POT canvas as possible while preserving the
        # original aspect ratio. This avoids both anisotropic stretching and
        # tiny content surrounded by excessive transparent padding.
        scale = min(canvas_width / width, canvas_height / height)
        render_width = max(1, round(width * scale))
        render_height = max(1, round(height * scale))
        if render_width != width or render_height != height:
            image = image.resize((render_width, render_height), Image.Resampling.LANCZOS)
        canvas = Image.new("RGBA", (canvas_width, canvas_height), (0, 0, 0, 0))
        x = (canvas_width - render_width) // 2
        y = (canvas_height - render_height) // 2
        canvas.alpha_composite(image, (x, y))
        destination.parent.mkdir(parents=True, exist_ok=True)
        canvas.save(destination, "PNG", optimize=True)
        return {
            "source_width": width,
            "source_height": height,
            "canvas_width": canvas_width,
            "canvas_height": canvas_height,
            "render_width": render_width,
            "render_height": render_height,
            "offset_x": x,
            "offset_y": y,
        }


def main() -> None:
    if not KIT.is_dir():
        raise SystemExit(f"Production Kit missing: {KIT}")
    if not SOURCE.is_dir():
        raise SystemExit(f"V2 source missing: {SOURCE}")

    DOCS.mkdir(parents=True, exist_ok=True)
    if OUTPUT.exists():
        shutil.rmtree(OUTPUT)
    OUTPUT.mkdir(parents=True)

    files = sorted((path for path in KIT.rglob("*") if path.is_file()), key=lambda p: str(p).lower())
    hashes = {path: sha256(path) for path in files}
    groups: dict[str, list[Path]] = defaultdict(list)
    for path, digest in hashes.items():
        groups[digest].append(path)
    duplicate_ids = {digest: f"DUP-{index:04d}" for index, digest in enumerate(sorted(d for d, paths in groups.items() if len(paths) > 1), 1)}

    rows: list[dict[str, str]] = []
    for path in files:
        relative = path.relative_to(KIT).as_posix()
        dimensions, ratio, alpha = inspect_image(path)
        cat = category(relative)
        used = relative in SELECTED
        rows.append({
            "Pfad": str(path),
            "Name": path.name,
            "Dateityp": path.suffix.lower(),
            "BreiteHoehe": dimensions,
            "Seitenverhaeltnis": ratio,
            "Alpha": alpha,
            "GroesseBytes": str(path.stat().st_size),
            "SHA256": hashes[path],
            "Kategorie": cat,
            "State": state_for(path.name),
            "Zielkomponente": target_for(relative, cat),
            "Duplikatgruppe": duplicate_ids.get(hashes[path], ""),
            "InV2Verwendet": "JA" if used else "NEIN",
            "SpaeterNutzbar": "NEIN" if used else ("JA" if path.suffix.lower() == ".png" else "NEIN"),
        })

    manifest = DOCS / "UI_ASSET_MANIFEST.csv"
    with manifest.open("w", newline="", encoding="utf-8-sig") as handle:
        writer = csv.DictWriter(handle, fieldnames=list(rows[0].keys()), delimiter=";")
        writer.writeheader()
        writer.writerows(rows)

    conversion_rows: list[dict[str, str]] = []
    errors: list[str] = []
    for source_relative, target_relative in sorted(SELECTED.items()):
        source_path = KIT / Path(source_relative)
        target_path = OUTPUT / Path(target_relative)
        if not source_path.is_file():
            errors.append(f"FEHLT: {source_path}")
            continue
        try:
            info = padded_copy(source_path, target_path)
            conversion_rows.append({
                "QuellPNG": str(source_path),
                "V2PNG": str(target_path),
                "ZielPAA": str(target_path.with_suffix(".paa")),
                "Original": f"{info['source_width']}x{info['source_height']}",
                "POTCanvas": f"{info['canvas_width']}x{info['canvas_height']}",
                "Offset": f"{info['offset_x']},{info['offset_y']}",
                "Render": f"{info['render_width']}x{info['render_height']}",
                "Alpha": "JA",
                "Status": "PNG_BEREIT",
            })
        except Exception as exc:  # host-side asset pipeline, not Enforce Script
            errors.append(f"FEHLER: {source_path}: {exc}")

    with (DOCS / "UI_PAA_OUTPUT_MAP.csv").open("w", newline="", encoding="utf-8-sig") as handle:
        fields = ["QuellPNG", "V2PNG", "ZielPAA", "Original", "POTCanvas", "Render", "Offset", "Alpha", "Status"]
        writer = csv.DictWriter(handle, fieldnames=fields, delimiter=";")
        writer.writeheader()
        writer.writerows(conversion_rows)

    component_map = """# MenuZ V2 UI Component Map

| Bereich | Production-Kit-Komponente | V2-Ziel | Dynamisch |
|---|---|---|---|
| Linke Navigation | Main_Left + Textbuttons | Hauptmenü links | Text/Interaktion |
| News | News | Hauptmenü rechts oben | Titel, Kategorie, Inhalt |
| Spielerstatus | Character_Stats | Hauptmenü rechts Mitte | Vanilla MainMenuStats |
| Events | Events_4_Cards + vier Eventkarten | Hauptmenü rechts unten | Eventstatus bleibt Widget/extern |
| Untere Navigation | Bottom_Navigation + Navigation-Icons | Hauptmenü unten | Hover/Pressed/Active |
| Musik | Music_Player + MusicPlayer-Controls | vorbereiteter Playerbereich | Audio bleibt DynamicMusicPlayer |
| Loading | bestehender V1-Loading-Screen | V2-Prefix, unverzerrt | echter DayZ-Ladefortschritt |
| Deathscreen | bestehender V1-Deathscreen | V2-Prefix, unverzerrt | Respawn-/Exit-Aktionen |
| Dialoge/Tooltips | V1-Funktion erhalten; Kit-Reserve dokumentiert | spaetere visuelle QA | dynamischer Text |
"""
    (DOCS / "UI_COMPONENT_MAP.md").write_text(component_map, encoding="utf-8")

    state_map = """# MenuZ V2 UI State Map

- Textbuttons: NORMAL, HOVER und PRESSED sind im V2-Script gebunden. DISABLED und SELECTED sind als konvertierte, referenzierbare Zustände im PBO vorhanden.
- Hauptnavigation: NORMAL, HOVER, PRESSED, DISABLED und ACTIVE sind als PAA vorhanden; HOME beginnt als ACTIVE.
- Eventkarten: NORMAL, HOVER, PRESSED, SELECTED, DISABLED, LOCKED, ACTIVE und WARNING sind enthalten. Dynamischer Live-Status wird nicht vorgetäuscht.
- Musiksteuerung: NORMAL, HOVER, PRESSED, DISABLED und ACTIVE sind vorbereitet. Die Audio-Wiedergabe bleibt bei DayZ DynamicMusicPlayer; es wird kein erfundener Fortschrittswert angezeigt.
- Panelzustände: V2 verwendet NORMAL. Weitere Production-Kit-Zustände bleiben dokumentierte Reserve und werden nicht blind gepackt.
"""
    (DOCS / "UI_STATE_MAP.md").write_text(state_map, encoding="utf-8")

    duplicate_lines = ["# MenuZ V2 Asset-Duplikate", ""]
    for digest, group_id in sorted(duplicate_ids.items(), key=lambda pair: pair[1]):
        duplicate_lines.append(f"## {group_id} - SHA-256 `{digest}`")
        for path in groups[digest]:
            duplicate_lines.append(f"- `{path.relative_to(KIT).as_posix()}`")
        duplicate_lines.append("")
    (DOCS / "UI_DUPLICATES.md").write_text("\n".join(duplicate_lines), encoding="utf-8")

    unused_by_category: dict[str, int] = defaultdict(int)
    for row in rows:
        if row["InV2Verwendet"] == "NEIN":
            unused_by_category[row["Kategorie"]] += 1
    unused_lines = ["# MenuZ V2 nicht gepackte Assets", "", "Bewusst nicht blind in die Release-PBO übernommen:", ""]
    for cat, count in sorted(unused_by_category.items()):
        unused_lines.append(f"- {cat}: {count}")
    unused_lines += ["", "Die vollständige Einzelauflistung steht in `UI_ASSET_MANIFEST.csv` mit `InV2Verwendet=NEIN`."]
    (DOCS / "UI_UNUSED_ASSETS.md").write_text("\n".join(unused_lines), encoding="utf-8")

    report_lines = [
        "# MenuZ V2 PNG-to-PAA Conversion Report",
        "",
        f"- Production Kit: `{KIT}`",
        f"- Inventarisierte Dateien: {len(files)}",
        f"- Ausgewählte PNGs: {len(conversion_rows)}",
        "- Originale verändert: NEIN",
        "- Verfahren: transparente RGBA-Power-of-Two-Canvas, proportional hochskaliert und zentriert, ohne Verzerrung oder Cropping",
        "- Nächster Schritt: automatisierte ImageToPAA-Konvertierung aller ausgewählten V2-PNGs",
    ]
    (DOCS / "UI_CONVERSION_REPORT.md").write_text("\n".join(report_lines) + "\n", encoding="utf-8")
    error_lines = ["# MenuZ V2 Conversion Errors", ""] + (errors if errors else ["Keine PNG-Vorbereitungsfehler."])
    (DOCS / "UI_CONVERSION_ERRORS.md").write_text("\n".join(error_lines) + "\n", encoding="utf-8")

    summary = {
        "inventory_files": len(files),
        "selected_png": len(conversion_rows),
        "duplicates": len(duplicate_ids),
        "errors": len(errors),
        "output": str(OUTPUT),
    }
    print(json.dumps(summary, indent=2, ensure_ascii=False))
    if errors:
        raise SystemExit(2)


if __name__ == "__main__":
    main()
