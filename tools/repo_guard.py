#!/usr/bin/env python3
"""Static safety and structure checks for the DeutschZ ModZ source repository."""

from __future__ import annotations

import json
import re
import subprocess
import sys
from pathlib import Path
import xml.etree.ElementTree as ET

ROOT = Path(__file__).resolve().parents[1]
MAX_REGULAR_FILE = 25 * 1024 * 1024

FORBIDDEN_SUFFIXES = {
    ".pbo", ".ebo", ".bisign", ".bikey", ".biprivatekey",
    ".pem", ".pfx", ".p12", ".mdmp", ".bidmp", ".dmp",
}

FORBIDDEN_PATH_PARTS = {
    "steamapps", "workshop", "!workshop", "profiles", "crashlogs",
    "dayzserver", "dayz tools", "workdrive", "buldozer", "unpacked",
    "codexchanged", "codexoutput",
}

SECRET_PATTERNS = [
    re.compile(r"discord(?:app)?\.com/api/webhooks/", re.IGNORECASE),
    re.compile(r"(?:password|passwd|api[_-]?key|secret|token)\s*[:=]\s*[\"']?(?!<|example|changeme|tbd)[^\s\"']{8,}", re.IGNORECASE),
    re.compile(r"-----BEGIN (?:RSA |EC |OPENSSH )?PRIVATE KEY-----"),
    re.compile(r"gh[pousr]_[A-Za-z0-9_]{20,}"),
]

TEXT_SUFFIXES = {
    ".c", ".cpp", ".h", ".hpp", ".json", ".xml", ".md", ".txt",
    ".yml", ".yaml", ".py", ".sh", ".bat", ".cfg", ".ini", ".rvmat",
}


def tracked_files() -> list[Path]:
    try:
        output = subprocess.check_output(
            ["git", "ls-files", "-z"], cwd=ROOT, stderr=subprocess.STDOUT
        )
        return [ROOT / p.decode("utf-8") for p in output.split(b"\0") if p]
    except (subprocess.CalledProcessError, FileNotFoundError):
        return [p for p in ROOT.rglob("*") if p.is_file() and ".git" not in p.parts]


def is_lfs_pointer(path: Path) -> bool:
    try:
        head = path.read_bytes()[:200]
    except OSError:
        return False
    return head.startswith(b"version https://git-lfs.github.com/spec/v1")


def scan() -> list[str]:
    errors: list[str] = []

    for path in tracked_files():
        if not path.exists() or not path.is_file():
            continue

        rel = path.relative_to(ROOT)
        rel_lower = rel.as_posix().lower()
        parts_lower = {part.lower() for part in rel.parts}

        if path.suffix.lower() in FORBIDDEN_SUFFIXES:
            errors.append(f"Verbotener Dateityp: {rel}")

        if parts_lower & FORBIDDEN_PATH_PARTS:
            errors.append(f"Verbotener Ordner im Repository: {rel}")

        if path.stat().st_size > MAX_REGULAR_FILE and not is_lfs_pointer(path):
            errors.append(f"Datei größer als 25 MiB und kein Git-LFS-Pointer: {rel}")

        if path.suffix.lower() == ".json":
            try:
                with path.open("r", encoding="utf-8-sig") as handle:
                    json.load(handle)
            except Exception as exc:  # noqa: BLE001
                errors.append(f"Ungültiges JSON: {rel}: {exc}")

        if path.suffix.lower() == ".xml":
            try:
                ET.parse(path)
            except Exception as exc:  # noqa: BLE001
                errors.append(f"Ungültiges XML: {rel}: {exc}")

        if path.suffix.lower() in TEXT_SUFFIXES or path.name == "$PBOPREFIX$":
            try:
                text = path.read_text(encoding="utf-8-sig", errors="strict")
            except (UnicodeDecodeError, OSError):
                continue

            if re.search(r"[A-Za-z]:\\Users\\", text, re.IGNORECASE):
                errors.append(f"Absoluter Benutzerpfad gefunden: {rel}")

            for pattern in SECRET_PATTERNS:
                if pattern.search(text):
                    errors.append(f"Mögliches Geheimnis gefunden: {rel}")
                    break

    required = [
        "README.md", "LICENSE", "CHANGELOG.md", "SECURITY.md",
        "docs/BUILD.md", "docs/TESTING.md", "docs/DEPENDENCIES.md",
    ]
    for required_path in required:
        if not (ROOT / required_path).is_file():
            errors.append(f"Pflichtdatei fehlt: {required_path}")

    return sorted(set(errors))


def main() -> int:
    errors = scan()
    if errors:
        print("Repository Guard fehlgeschlagen:\n")
        for error in errors:
            print(f"- {error}")
        return 1

    print("Repository Guard erfolgreich: keine verbotenen Dateien, offensichtlichen Geheimnisse oder ungültigen JSON/XML-Dateien gefunden.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
