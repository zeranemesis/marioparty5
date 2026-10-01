#!/usr/bin/env python3

###
# Prints how far each unit of a module (or any unit whose name contains the
# given text) is from matching, function by function.
#
# Usage:
#   python tools/unit_status.py m524Dll
#   python tools/unit_status.py msm/ --no-build
#
# The numbers come from build/<version>/report.json, which is regenerated with
# ninja first unless --no-build is given, so they always describe the sources
# currently on disk.
###

import argparse
import json
import subprocess
import sys
from pathlib import Path

from _root import project_root

root = project_root()


def pct(value) -> str:
    return "  --  " if value is None else f"{value:6.2f}"


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("filter", help="module name (m524Dll) or unit name fragment (msm/)")
    parser.add_argument("--version", default="GP5E01_00")
    parser.add_argument("--no-build", action="store_true", help="read the existing report")
    parser.add_argument("--all", action="store_true", help="also list functions that match")
    args = parser.parse_args()

    report_path = root / "build" / args.version / "report.json"
    if not args.no_build:
        target = f"build/{args.version}/report.json"
        result = subprocess.run(["ninja", target], cwd=root)
        if result.returncode != 0:
            print("ninja failed: fix the build before reading the report", file=sys.stderr)
            return result.returncode

    report = json.loads(report_path.read_text(encoding="utf-8"))
    units = [
        u
        for u in report["units"]
        if u["name"].startswith(args.filter + "/") or args.filter in u["name"]
    ]
    if not units:
        print(f"no unit matches '{args.filter}'", file=sys.stderr)
        return 1

    total_fns = matched_fns = 0
    total_code = matched_code = 0
    for unit in units:
        m = unit.get("measures", {})
        fns = unit.get("functions", [])
        unit_total = int(m.get("total_code", 0))
        unit_matched = int(m.get("matched_code", 0))
        total_code += unit_total
        matched_code += unit_matched
        total_fns += int(m.get("total_functions", 0))
        matched_fns += int(m.get("matched_functions", 0))
        has_source = "fuzzy_match_percent" in m
        print(
            f"{unit['name']}: code {unit_matched}/{unit_total} "
            f"({pct(m.get('matched_code_percent', 0.0)).strip()}%), "
            f"functions {m.get('matched_functions', 0)}/{m.get('total_functions', 0)}, "
            f"fuzzy {pct(m.get('fuzzy_match_percent')).strip()}%"
            + ("" if has_source else "  [no compiled source]")
        )
        for fn in fns:
            fuzzy = fn.get("fuzzy_match_percent")
            if fuzzy == 100.0 and not args.all:
                continue
            state = "missing" if fuzzy is None else f"{fuzzy:6.2f}%"
            print(f"    {fn['name']:<32} size {int(fn['size']):>6}  {state}")

    print(
        f"TOTAL: functions {matched_fns}/{total_fns}, code {matched_code}/{total_code} bytes"
        + (f" ({100.0 * matched_code / total_code:.2f}%)" if total_code else "")
    )
    return 0


if __name__ == "__main__":
    sys.exit(main())
