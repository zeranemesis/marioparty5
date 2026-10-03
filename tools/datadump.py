#!/usr/bin/env python3
"""Prints only the .data/.rodata/.ctors label blocks of one unit's asm.

Companion to fnasm.py: reading a whole .s file exhausts a decompilation
session's context, so data has to be reachable without opening it.

    python tools/datadump.py m522Dll/REL/m522Dll/gamemaster
"""
import sys
from pathlib import Path


def asm_path(unit: str) -> Path:
    module = unit.split("/")[0]
    name = unit.split("/")[-1].rsplit(".", 1)[0]
    relative = Path("build") / "GP5E01_00" / module / "asm" / "REL" / module / f"{name}.s"
    return Path.cwd() / relative


def main() -> int:
    wanted = sys.argv[1] if len(sys.argv) > 1 else ""
    lines = asm_path(sys.argv[1]).read_text(errors="replace").splitlines()
    keep = False
    for line in lines:
        if line.startswith(".section") or line.startswith("# ."):
            keep = bool(wanted and wanted in line)
        elif line.startswith(".include") or line.startswith(".file"):
            keep = False
        if keep:
            print(line)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())