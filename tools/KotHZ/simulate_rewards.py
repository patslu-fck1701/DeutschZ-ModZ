#!/usr/bin/env python3
"""Deterministic static simulation of the KotHZ Free reward selection rules."""

from __future__ import annotations

import json
import random
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
SETTINGS = ROOT / "src" / "DeutschZ_KotHZ_Free" / "example-settings" / "KotHZSettings.json"
REPORT = ROOT / "docs" / "DeutschZ_KotHZ_Free" / "REWARD_SIMULATION_10_RUNS.md"


def weighted_choice(rng: random.Random, pool: list[dict], excluded: set[str] | None = None) -> dict:
    candidates = [entry for entry in pool if entry["Chance"] > 0 and (not excluded or entry["Type"] not in excluded)]
    return rng.choices(candidates, weights=[entry["Chance"] for entry in candidates], k=1)[0]


def main() -> None:
    settings = json.loads(SETTINGS.read_text(encoding="utf-8"))
    rng = random.Random(20260717)
    rows: list[str] = []

    for run in range(1, 11):
        selected: list[str] = []
        guaranteed = weighted_choice(rng, settings["GuaranteedWeaponPool"])["Type"]
        selected.append(guaranteed)
        extra_count = rng.randint(settings["RandomWeaponMin"], settings["RandomWeaponMax"])
        for _ in range(extra_count):
            excluded = set(selected) if not settings["AllowDuplicateWeapons"] else None
            selected.append(weighted_choice(rng, settings["RandomWeaponPool"], excluded)["Type"])
        rows.append(f"| {run} | {guaranteed} | {extra_count} | {', '.join(selected[1:])} | {len(selected)} | {'JA' if len(selected) == len(set(selected)) else 'NEIN'} |")

    report = "\n".join(
        [
            "# KotHZ Free – Reward-Simulation (10 Läufe)",
            "",
            "Statische, deterministische Auswahlprüfung mit Seed `20260717`. Sie belegt die Verteilungslogik, ersetzt aber keinen DayZ-Inventar-/Cargo-Laufzeittest.",
            "",
            "| Lauf | Garantierte Waffe | Zufallswaffen | Auswahl | Waffen gesamt | Eindeutig |",
            "|---:|---|---:|---|---:|---|",
            *rows,
            "",
            "Erwartung: je Lauf genau 1 garantierte Waffe plus 1–3 Zufallswaffen; bei `AllowDuplicateWeapons=false` keine doppelte Waffenklasse.",
            "",
        ]
    )
    REPORT.parent.mkdir(parents=True, exist_ok=True)
    REPORT.write_text(report, encoding="utf-8", newline="\n")
    print(REPORT)


if __name__ == "__main__":
    main()
