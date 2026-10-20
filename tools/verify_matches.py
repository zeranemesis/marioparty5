#!/usr/bin/env python3

###
# Cross-checks every function reported as matched (fuzzy 100%) against objdiff's
# strict per-symbol diff, to catch "matches" that fndiff cannot see.
#
# Why this exists: fndiff compares instruction *text* and substitutes the
# relocation symbol name on both sides, so a `bl someFn` row looks identical
# even when `someFn` is still undefined in our object. objdiff scores that as
# DIFF_ARG_MISMATCH, so a function can be 0 `!` in fndiff yet below 100% strict.
# The report's fuzzy metric forgives unresolved relocations, so an honest audit
# must separate:
#   * DIFF_ARG_MISMATCH on a relocation  -> callee not decompiled yet (benign)
#   * anything else (DIFF_REPLACE, opcode/register/offset) -> a real mismatch
#
# Usage:
#   python tools/verify_matches.py                 # every unit that has source
#   python tools/verify_matches.py m508Dll         # only unit names containing x
#   python tools/verify_matches.py m508Dll --all   # also list the benign ones
###

import argparse
import json
import os
import subprocess
import sys
import tempfile
from pathlib import Path

from _root import project_root

root = project_root()


def objdiff_exe() -> Path:
    suffix = ".exe" if sys.platform == "win32" else ""
    exe = root / "build" / "tools" / f"objdiff-cli-2.7.1{suffix}"
    if not exe.exists():
        exe = root / "build" / "tools" / f"objdiff-cli{suffix}"
    return exe


def strict_rows(unit_name: str, exe: Path):
    """Return {symbol: [row, ...]} for the compiled (right) side of a unit."""
    out = Path(tempfile.gettempdir()) / f"verify_unit_{os.getpid()}.json"
    result = subprocess.run(
        [str(exe), "diff", "-p", str(root), "-u", unit_name, "-o", str(out), "--format", "json"],
        cwd=root,
        capture_output=True,
        text=True,
    )
    if result.returncode != 0 or not out.exists():
        return None
    data = json.loads(out.read_text(encoding="utf-8"))
    symbols = {}
    for section in data["right"].get("sections", []):
        for sym in section.get("symbols", []):
            name = sym["symbol"]["name"]
            symbols[name] = (sym.get("instructions", []), sym.get("match_percent"))
    return symbols


def reloc_names(row) -> str:
    ins = (row or {}).get("instruction") or {}
    rel = ins.get("relocation")
    if not rel:
        return ""
    return (rel.get("target") or {}).get("symbol", {}).get("name", "")


def classify(rows):
    """Split the differing rows into benign (unresolved reloc) and real ones."""
    benign, real = [], []
    for i, row in enumerate(rows or []):
        kind = (row or {}).get("diff_kind")
        if not kind:
            continue
        if kind == "DIFF_ARG_MISMATCH" and reloc_names(row):
            benign.append((i, kind, reloc_names(row)))
        else:
            real.append((i, kind, (row or {}).get("instruction", {}).get("formatted", "")))
    return benign, real


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("filter", nargs="?", default="", help="unit name fragment")
    parser.add_argument("--version", default="GP5E01_00")
    parser.add_argument("--all", action="store_true", help="also list benign-only functions")
    args = parser.parse_args()

    report = json.loads((root / "build" / args.version / "report.json").read_text(encoding="utf-8"))
    objdiff = json.loads((root / "objdiff.json").read_text(encoding="utf-8"))
    base_paths = {u["name"]: u.get("base_path") for u in objdiff["units"]}

    exe = objdiff_exe()
    checked = benign_total = real_total = 0
    bad_units = []

    for unit in report["units"]:
        name = unit["name"]
        if args.filter and args.filter not in name:
            continue
        if not base_paths.get(name):
            continue
        matched = [
            f
            for f in unit.get("functions", [])
            if f.get("fuzzy_match_percent") == 100.0
        ]
        if not matched:
            continue
        rows = strict_rows(name, exe)
        if rows is None:
            print(f"!! {name}: objdiff diff failed", file=sys.stderr)
            continue
        printed_header = False
        for fn in matched:
            checked += 1
            frows, _ = rows.get(fn["name"], ([], None))
            b, r = classify(frows)
            if not b and not r:
                continue
            if r:
                real_total += 1
                bad_units.append(name)
                if not printed_header:
                    print(f"== {name}")
                    printed_header = True
                print(f"  REAL  {fn['name']:<28} fuzzy=100  diffs={r}")
            elif b:
                benign_total += 1
                if args.all:
                    if not printed_header:
                        print(f"== {name}")
                        printed_header = True
                    names = sorted({n for _, _, n in b})
                    print(f"  benign {fn['name']:<28} unresolved callee(s): {', '.join(names)}")

    print()
    print(f"matched functions checked: {checked}")
    print(f"  benign only (unresolved callee): {benign_total}")
    print(f"  REAL mismatches: {real_total}")
    return 1 if real_total else 0


if __name__ == "__main__":
    sys.exit(main())
