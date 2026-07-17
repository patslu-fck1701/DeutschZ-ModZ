from __future__ import annotations

import csv
import math
import shutil
from pathlib import Path

from PIL import Image


PACK = Path(r"C:\Users\patsl\Downloads\DeutschZ_EventAdmin_UI_Addon")
REPO = Path(r"E:\DeutschZ\DeutschZ-ModZ")
SOURCE = REPO / "src" / "DeutschZ_UiAdminZ"
OUTPUT = SOURCE / "gui" / "admin_pack"
DOCS = REPO / "docs" / "UiAdminZ"

PANELS = {
    "01_PANELS/Dashboard/Admin_Header_normal.png": "panels/admin_header.png",
    "01_PANELS/Dashboard/Admin_Sidebar_normal.png": "panels/admin_sidebar.png",
    "01_PANELS/Dashboard/Dashboard_Main_normal.png": "panels/dashboard_main.png",
    "01_PANELS/Dashboard/Dashboard_Metrics_normal.png": "panels/dashboard_metrics.png",
    "01_PANELS/Dashboard/Active_Events_normal.png": "panels/active_events.png",
    "01_PANELS/Dashboard/Quick_Actions_normal.png": "panels/quick_actions.png",
    "01_PANELS/Dashboard/Scheduled_Event_normal.png": "panels/scheduled_event.png",
    "01_PANELS/License/License_Compare_normal.png": "panels/license_compare.png",
    "01_PANELS/License/Pro_Feature_List_normal.png": "panels/pro_feature_list.png",
    "01_PANELS/EventEditor/Event_Editor_Main_normal.png": "panels/event_editor_main.png",
    "01_PANELS/EventEditor/Editor_Tab_Bar_normal.png": "panels/editor_tab_bar.png",
    "01_PANELS/EventEditor/Editor_Form_Column_normal.png": "panels/editor_form_column.png",
    "01_PANELS/EventEditor/Editor_Preview_normal.png": "panels/editor_preview.png",
    "01_PANELS/MapEditor/Map_Preview_normal.png": "panels/map_preview.png",
    "01_PANELS/Logs/Live_Logs_normal.png": "panels/live_logs.png",
}

BUTTONS = {}
for name in ("EVENT_STARTEN", "EVENT_STOPPEN", "NOTFALL-STOPP"):
    for state in ("normal", "hover", "pressed", "disabled", "locked"):
        BUTTONS[f"02_BUTTONS/AdminActions/{name}_{state}.png"] = f"buttons/{name.lower()}_{state}.png"

ICONS = {
    "dashboard": "dashboard",
    "event_list": "events",
    "event_create": "event_create",
    "template": "templates",
    "schedule": "schedule",
    "zone": "zones",
    "reward": "rewards",
    "notification": "notifications",
    "live_map": "live_map",
    "logs": "logs",
    "permissions": "settings",
    "license": "license",
}


def next_pot(value: int) -> int:
    return 1 if value <= 1 else 1 << math.ceil(math.log2(value))


def prepare_png(source: Path, target: Path) -> tuple[int, int, int, int]:
    with Image.open(source) as opened:
        image = opened.convert("RGBA")
        width, height = image.size
        canvas_width, canvas_height = next_pot(width), next_pot(height)
        scale = min(canvas_width / width, canvas_height / height)
        render = image.resize((round(width * scale), round(height * scale)), Image.Resampling.LANCZOS)
        canvas = Image.new("RGBA", (canvas_width, canvas_height), (0, 0, 0, 0))
        offset = ((canvas_width - render.width) // 2, (canvas_height - render.height) // 2)
        canvas.alpha_composite(render, offset)
        target.parent.mkdir(parents=True, exist_ok=True)
        canvas.save(target, "PNG", optimize=True)
        return width, height, canvas_width, canvas_height


def find_icon(name: str, state: str) -> Path:
    candidates = sorted(PACK.glob(f"03_ICONS/**/*{name}_{state}.paa"))
    if not candidates:
        raise FileNotFoundError(f"Icon fehlt: {name}_{state}.paa")
    return candidates[0]


def main() -> None:
    if not PACK.is_dir():
        raise SystemExit(f"Assetpack fehlt: {PACK}")
    if OUTPUT.exists():
        shutil.rmtree(OUTPUT)
    OUTPUT.mkdir(parents=True)
    DOCS.mkdir(parents=True, exist_ok=True)

    rows = []
    for relative, target_relative in {**PANELS, **BUTTONS}.items():
        source = PACK / relative
        if not source.is_file():
            raise FileNotFoundError(source)
        target = OUTPUT / target_relative
        source_size = prepare_png(source, target)
        rows.append((str(source), str(target.with_suffix(".paa")), f"{source_size[0]}x{source_size[1]}", f"{source_size[2]}x{source_size[3]}", "PNG_BEREIT"))

    for icon, target_name in ICONS.items():
        for state in ("normal", "hover", "pressed", "disabled", "active"):
            source = find_icon(icon, state)
            target = OUTPUT / "icons" / f"{target_name}_{state}.paa"
            target.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(source, target)
            rows.append((str(source), str(target), "PAA", "PAA", "KOPIERT"))

    with (DOCS / "UIADMINZ_ASSET_MANIFEST.csv").open("w", newline="", encoding="utf-8-sig") as handle:
        writer = csv.writer(handle, delimiter=";")
        writer.writerow(("Quelle", "PBO-Ziel", "Original", "POT-Canvas", "Status"))
        writer.writerows(rows)
    print(f"Ausgewaehlt: {len(rows)} Assets; PNG-Konvertierung ausstehend: {len(PANELS) + len(BUTTONS)}")


if __name__ == "__main__":
    main()
