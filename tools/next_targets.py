#!/usr/bin/env python3

###
# The dispatch list for one loop iteration: what is worth a session, ranked.
#
# Usage:
#   python tools/next_targets.py                 # default top 40
#   python tools/next_targets.py -n 80
#   python tools/next_targets.py --unregistered  # split objects with no objdiff entry
#   python tools/next_targets.py --unit m521Dll/REL/m521Dll/camera
#
# It never builds and never writes to the tree.
#
# Three facts decide a target, and none of them is "it is big":
#
#   1. how many code bytes are still unmatched in it (the progress metric);
#   2. whether a source file is already on disk (BRIEF section 4.3: 43 units
#      already carry 1.4 MB of m2c-style source, so "fix the worst functions"
#      is usually far cheaper than "write it from scratch");
#   3. whether its target object carries the string "singleton.h". Those units
#      contain an __assert_fail whose out-of-line cold block MWCC 1.3.2 will
#      not emit, so Matching is out of reach for them until the assert is
#      rewritten to call the shared fn_1_1D4 / __assert_fail. They are listed
#      but flagged, so nobody spends a session discovering that at the end.
#
# The pool registry keeps a module out of the list while a row exists for it.
###

import argparse
import json
import re
import sys
from collections import defaultdict
from pathlib import Path

from _root import pool_path, project_root

root = project_root()
version = "GP5E01_00"
VERSION_DIR = root / "build" / version

STATUS_RE = re.compile(r'Object\((\w+),\s*"([^"]+)"\)')
MODULE_RE = re.compile(r'"([A-Za-z0-9_]+)",\s*\n\s*objects=\{(.*?)\n\s*\},', re.S)
ROW_RE = re.compile(r'\|\s*\d+\s*\|\s*(\S+)\s*\|')


def configure_status() -> dict:
    """'<module>/<source path without extension>' -> Matching/Equivalent/NonMatching."""
    text = (root / "configure.py").read_text(encoding="utf-8", errors="ignore")
    found = {}
    for module, body in MODULE_RE.findall(text):
        for status, path in STATUS_RE.findall(body):
            found[f"{module}/{path.rsplit('.', 1)[0]}"] = status
    return found


def pooled_modules() -> set:
    """Modules that already have a row in the agent pool registry."""
    out = set()
    path = pool_path()
    if not path.exists():
        return out
    for line in path.read_text(encoding="utf-8", errors="replace").splitlines():
        m = ROW_RE.match(line)
        if m:
            out.add(m.group(1))
    return out


def report_units() -> dict:
    path = VERSION_DIR / "report.json"
    if not path.exists():
        raise SystemExit("missing %s -- run ninja first" % path)
    raw = json.loads(path.read_text(encoding="utf-8"))
    return {u["name"]: u for u in raw["units"]}


def unit_relative(target_path: str) -> str:
    """objdiff stores 'build/<version>/obj/...'; we compare against paths made
    relative to build/<version>. Normalise both sides the same way, or every unit
    looks unregistered."""
    prefix = f"build/{version}/"
    return target_path[len(prefix):] if target_path.startswith(prefix) else target_path


def unregistered(measured: set, limit: int) -> int:
    """Split objects on disk that objdiff.json does not describe.

    These cannot be scored by objdiff at all, so they silently count as
    unmatched code forever. Each one needs an Object(...) line in
    configure.py before a session can be spent on it.

    Two layouts exist: the DOL lives in build/<v>/obj/**, each REL module in
    build/<v>/<module>/obj/**. Both must be walked.
    """
    found = []
    for obj_dir in [VERSION_DIR / "obj"] + sorted(VERSION_DIR.glob("*/obj")):
        if not obj_dir.is_dir():
            continue
        for path in obj_dir.rglob("*.o"):
            rel = path.relative_to(VERSION_DIR).as_posix()
            if rel not in measured:
                found.append((path.stat().st_size, rel))
    found.sort()
    per_module = defaultdict(lambda: [0, 0])
    for size, rel in found:
        key = rel.split("/")[1] if rel.startswith("REL/") else "main"
        per_module[key][0] += 1
        per_module[key][1] += size
    print("%d split objects have no objdiff entry and cannot be scored yet" % len(found))
    print("%-18s %6s %10s" % ("module", "units", "bytes"))
    for module, (count, size) in sorted(per_module.items(), key=lambda kv: -kv[1][1]):
        print("%-18s %6d %10d" % (module, count, size))
    if found:
        print("\nsmallest first, they are the cheapest to register and to write:")
        for size, rel in found[:limit]:
            print("    %8d  %s" % (size, rel))
    return 0


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("-n", "--count", type=int, default=40)
    parser.add_argument("--unregistered", action="store_true")
    parser.add_argument("--unit", action="append")
    parser.add_argument("--all", action="store_true", help="ignore the pool registry")
    parser.add_argument(
        "--sort",
        choices=("volume", "cheap", "finish"),
        default="volume",
        help="volume: biggest gap first (what the progress metric rewards). "
        "cheap: smallest gap first (one `#define _MATH_H` away). "
        "finish: highest percentage first, whatever the size -- the band where "
        "a session converts into Matching.",
    )
    args = parser.parse_args()

    config = json.loads((root / "objdiff.json").read_text(encoding="utf-8"))
    measured = {
        unit_relative(u["target_path"]) for u in config["units"] if "target_path" in u
    }
    if args.unregistered:
        return unregistered(measured, args.count)

    status = configure_status()
    pooled = set() if args.all else pooled_modules()
    report = report_units()

    rows = []
    for unit in config["units"]:
        name = unit["name"]
        if args.unit and name not in args.unit:
            continue
        measures = report.get(name, {}).get("measures", {})
        total = int(measures.get("total_code", 0) or 0)
        matched = int(measures.get("matched_code", 0) or 0)
        if not total or matched >= total:
            continue
        target = root / unit.get("target_path", "")
        singleton = bool(target.exists() and b"singleton.h" in target.read_bytes())
        base = root / unit.get("base_path", "") if unit.get("base_path") else None
        rows.append(
            {
                "name": name,
                "module": name.split("/")[0],
                "status": status.get(name, "-")[:4],
                "missing": total - matched,
                "total": total,
                "pct": 100.0 * matched / total,
                "fns": "%s/%s"
                % (
                    measures.get("matched_functions", 0),
                    measures.get("total_functions", 0),
                ),
                "source": bool(base and base.exists()),
                "singleton": singleton,
                "pooled": name.split("/")[0] in pooled,
            }
        )

    rows.sort(key=lambda r: (-r["missing"], r["total"]))
    free = [r for r in rows if not r["pooled"]]
    if args.sort == "cheap":
        free.sort(key=lambda r: (r["missing"], r["total"]))
    elif args.sort == "finish":
        free.sort(key=lambda r: (-r["pct"], r["missing"]))

    print(
        "%d units with unmatched code, %d on pooled modules, %d free"
        % (len(rows), len(rows) - len(free), len(free))
    )
    print("%d of the free ones carry the singleton.h assert wall\n" % len([r for r in free if r["singleton"]]))
    print("%-46s %-4s %8s %6s %7s %3s %3s" % ("unit", "stat", "missing", "total", "pct", "src", "asrt"))
    for row in free[: args.count]:
        print(
            "%-46s %-4s %8d %6d %6.1f%% %3s %3s"
            % (
                row["name"],
                row["status"],
                row["missing"],
                row["total"],
                row["pct"],
                "yes" if row["source"] else "-",
                "YES" if row["singleton"] else "-",
            )
        )
    print("\nsrc=yes means a source file is already on disk: finish it, do not rewrite it.")
    return 0


if __name__ == "__main__":
    sys.exit(main())