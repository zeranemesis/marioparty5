#!/usr/bin/env python3

###
# Finds the units most likely to be one `#define _MATH_H` away from Matching.
#
# Usage:
#   python tools/promote_scan.py                 # ranked list, no build
#   python tools/promote_scan.py --report        # add exact function percentages
#   python tools/promote_scan.py --all           # include non-Equivalent units
#   python tools/promote_scan.py --unit m574Dll/REL/m574Dll/player
#
# Why this exists. MSL's math.h declares sqrtf as `extern inline`, and MWCC
# emits two local statics with it -- `_half$localstatic3$sqrtf__Ff` (0.5f)
# and `_three$localstatic4$sqrtf__Ff` (3.0f) -- for 16 bytes of .rodata. Any
# object that reaches math.h, directly or through humath.h, carries them. A
# unit whose code region is already byte-identical then fails its module link
# by exactly those 16 bytes, which reads as a data bug and is not one.
#
# So the signature of a promotion candidate is cheap and mechanical: the
# rebuilt object contains both symbol names. This tool finds them by reading
# the object bytes (the names sit in the ELF string table), which needs no
# subprocess and no nm, then uses objdump only on the candidates it kept.
#
# It never builds and never writes to the tree.
###

import argparse
import json
import re
import subprocess
import sys
from pathlib import Path

from _root import project_root

root = project_root()

TRAP_SYMBOLS = (b"_half$localstatic3$sqrtf__Ff", b"_three$localstatic4$sqrtf__Ff")

# The dispatch filter. A unit whose target object carries the string
# "singleton.h" contains at least one __assert_fail that MWCC 2.6 insists on
# inlining, and no C spelling is known to reproduce the original's out-of-line
# cold block. Since Matching requires every byte, one such function caps the
# whole unit below Matching no matter how good the rest is -- so those units
# are not worth an agent until the wall is broken.
SINGLETON = b"singleton.h"

STATUS_RE = re.compile(r'Object\((\w+),\s*"([^"]+)"\)')
MODULE_RE = re.compile(r'"([A-Za-z0-9_]+)",\s*\n\s*objects=\{(.*?)\n\s*\},', re.S)


def exe(name: str) -> str:
    return f"{name}.exe" if sys.platform == "win32" else name


def configure_status() -> dict:
    """Maps '<module>/<source path without extension>' to its configure.py status."""
    text = (root / "configure.py").read_text(encoding="utf-8", errors="ignore")
    found = {}
    for module, body in MODULE_RE.findall(text):
        for status, path in STATUS_RE.findall(body):
            unit = path.rsplit(".", 1)[0]
            found[f"{module}/{unit}"] = status
    return found


def section_size(objdump: Path, obj: Path, section: str) -> int | None:
    """One section's size in bytes, or None when the section is absent."""
    result = subprocess.run(
        [str(objdump), "-h", str(obj)], capture_output=True, text=True
    )
    if result.returncode != 0:
        return None
    for line in result.stdout.splitlines():
        parts = line.split()
        # "  1 .rodata       0000004c  00000000 ..." -- column 0 is the index.
        if len(parts) > 2 and parts[1] == section:
            try:
                return int(parts[2], 16)
            except ValueError:
                return None
    return None


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--version", default="GP5E01_00")
    parser.add_argument("--all", action="store_true", help="do not filter on Equivalent")
    parser.add_argument("--unit", action="append", help="only this unit (repeatable)")
    parser.add_argument("--report", action="store_true", help="add function percentages")
    parser.add_argument(
        "--new",
        action="store_true",
        help="list unwritten (NonMatching) units smallest first, with the "
        "singleton.h dispatch filter applied",
    )
    parser.add_argument(
        "--modules",
        action="store_true",
        help="group the sqrtf trap by REL module: the 16 bytes are a "
        "module-level property, not a per-unit one",
    )
    args = parser.parse_args()

    objdump = root / "build" / "binutils" / exe("powerpc-eabi-objdump")
    if not objdump.exists():
        print(f"missing {objdump}", file=sys.stderr)
        return 1

    config = json.loads((root / "objdiff.json").read_text(encoding="utf-8"))
    status = configure_status()

    report = None
    if args.report:
        path = root / "build" / args.version / "report.json"
        if path.exists():
            report = {
                u["name"]: u.get("measures", {})
                for u in json.loads(path.read_text(encoding="utf-8"))["units"]
            }

    ANY = object()
    wanted = ANY if args.modules else ("NonMatching" if args.new else "Equivalent")
    rows = []
    missing = 0
    skipped = []
    for unit in config["units"]:
        name = unit["name"]
        if args.unit and name not in args.unit:
            continue
        state = status.get(name)
        if not args.all and wanted is not ANY and state != wanted:
            continue
        # A NonMatching unit has no base_path by construction: it emits no
        # compile rule, which is exactly why it has no source file on disk.
        if "base_path" not in unit and wanted is not ANY:
            continue  # DOL unit with no source; nothing to compare
        base = root / unit.get("base_path", "")
        target = root / unit["target_path"]
        if not target.exists():
            continue
        if not base.exists():
            if wanted is ANY or wanted == "NonMatching":
                pass  # expected: NonMatching means no source file on disk
            else:
                missing += 1
                skipped.append(name)
                continue
        data = base.read_bytes() if unit.get("base_path") and base.exists() else b""
        hits = [s.decode() for s in TRAP_SYMBOLS if s in data]
        row = {
            "name": name,
            "status": state,
            "trap": len(hits),
            "code": section_size(objdump, target, ".text") or 0,
            "fns": "",
            "singleton": SINGLETON in target.read_bytes(),
        }
        if hits and unit.get("base_path") and base.exists():
            row["rodata"] = (
                section_size(objdump, target, ".rodata"),
                section_size(objdump, base, ".rodata"),
            )
            row["text"] = (
                section_size(objdump, target, ".text"),
                section_size(objdump, base, ".text"),
            )
        rows.append(row)

    if args.modules:
        # The REL is one link unit: the original .rel carries exactly one copy
        # of the two floats, so a module only links when NO rebuilt object in it
        # emits a second copy. A Matching unit that carries the trap therefore
        # breaks the module link today, and an Equivalent one blocks promotion
        # for its whole module until the trap is cleared everywhere.
        mods = {}
        for row in rows:
            if not row["trap"]:
                continue
            mods.setdefault(row["name"].split("/")[0], []).append(row)
        broken, blocked = [], []
        for mod, units in sorted(mods.items()):
            if any(u["status"] == "Matching" for u in units):
                broken.append((mod, units))
            else:
                blocked.append((mod, units))
        for title, group in (("LINK IS BROKEN NOW", broken), ("PROMOTION BLOCKED", blocked)):
            print(f"\n=== {title}: {len(group)} modules ===")
            for mod, units in group:
                names = ", ".join(
                    f"{u['name'].split('/')[-1]}[{(u['status'] or '?')[0]}]" for u in units
                )
                print(f"  {mod:<16} {names}")
        print(
            "\nEvery module in the two lists above needs `#define _MATH_H` as the "
            "first line of each unit named, not just the unit being promoted."
        )
        return 0

    if args.new:
        rows.sort(key=lambda r: r["code"])
        print(f"{'unit':<52} {'text':>7}  singleton")
        for row in rows:
            print(
                f"{row['name']:<52} {row['code']:>7}  "
                f"{'BLOCKED' if row['singleton'] else 'free'}"
            )
        free = [r for r in rows if not r["singleton"]]
        print(f"\n{len(free)} of {len(rows)} unwritten units have no singleton.h assert")
        for row in free[:40]:
            print(f"    {row['name']:<52} {row['code']:>6}")
        return 0

    rows.sort(key=lambda r: (-r["trap"], r["code"]))

    print(
        f"{'unit':<52} {'status':<12} {'trap':>4} "
        f"{'rodata tgt/new':>15} {'d':>5} {'text tgt/new':>14} {'d':>6}"
    )
    for row in rows:
        if row["trap"]:
            tgt, new = row["rodata"]
            ttgt, tnew = row["text"]
            rodata = f"{tgt}/{new}"
            rdelta = "" if tgt is None or new is None else f"{new - tgt:+d}"
            text = f"{ttgt}/{tnew}"
            tdelta = "" if ttgt is None or tnew is None else f"{tnew - ttgt:+d}"
        else:
            rodata = text = rdelta = tdelta = "-"
        print(
            f"{row['name']:<52} {str(row['status']):<12} {row['trap']:>4} "
            f"{rodata:>15} {rdelta:>5} {text:>14} {tdelta:>6}"
        )

    near = [r for r in rows if r["trap"] and r["rodata"][1] - r["rodata"][0] == 16 and r["text"][0] == r["text"][1]]
    print(
        f"\n{len([r for r in rows if r['trap']])} of {len(rows)} Equivalent units carry the "
        f"16-byte sqrtf pair"
    )
    print(
        f"{len(near)} of those have .rodata exactly +16 and .text the right size: "
        "these are the near-certain promotions"
    )
    for row in near:
        print(f"    {row['name']}")
    if missing:
        print(f"\n{missing} units skipped, target or rebuilt object not on disk:")
        for name in skipped:
            print(f"    {name}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
