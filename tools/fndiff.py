#!/usr/bin/env python3

###
# Shows, in plain text, where one function compiled from src/ differs from the
# original object: target (original) on the left, base (our build) on the
# right, with the C line that produced each base instruction.
#
# Usage:
#   python tools/fndiff.py m524Dll/REL/m524Dll/m524 fn_1_438
#   python tools/fndiff.py m524Dll/REL/m524Dll/m524 fn_1_438 --all
#
# The unit's object is rebuilt with ninja first, so the diff describes the
# source currently on disk. Only rows around differences are printed unless
# --all is given.
###

import argparse
import json
import subprocess
import sys
import tempfile
from pathlib import Path

from _root import project_root

root = project_root()


def find_unit(name: str) -> dict:
    objdiff = json.loads((root / "objdiff.json").read_text(encoding="utf-8"))
    for unit in objdiff["units"]:
        if unit["name"] == name:
            return unit
    candidates = [u["name"] for u in objdiff["units"] if name in u["name"]]
    hint = f" (did you mean: {', '.join(candidates[:8])})" if candidates else ""
    raise SystemExit(f"unit not found: {name}{hint}")


def symbol_rows(side: dict, symbol: str):
    for section in side.get("sections", []):
        for sym in section.get("symbols", []):
            if sym["symbol"]["name"] == symbol:
                return sym.get("instructions", []), sym.get("match_percent")
    return None, None


RELOC_SUFFIX = {
    "R_PPC_ADDR16_HA": "@ha",
    "R_PPC_ADDR16_HI": "@h",
    "R_PPC_ADDR16_LO": "@l",
    "R_PPC_EMB_SDA21": "@sda21",
}


def reloc_name(row: dict):
    ins = row.get("instruction") or {}
    reloc = ins.get("relocation") or {}
    return (reloc.get("target") or {}).get("symbol", {}).get("name")


def row_text(row: dict) -> str:
    # objdiff prints relocated operands as 0x0; show the symbol instead
    ins = row.get("instruction")
    if not ins:
        return ""
    text = ins.get("formatted", "")
    name = reloc_name(row)
    if name:
        suffix = RELOC_SUFFIX.get(ins["relocation"].get("type_name", ""), "")
        text = text.replace("0x0", name + suffix, 1)
    return text


def compare(left: dict, right: dict) -> str:
    """' ' same, '~' only a literal/data label differs (often harmless), '!' differs"""
    if not left.get("instruction") or not right.get("instruction"):
        return "!"
    lt, rt = row_text(left), row_text(right)
    if lt == rt:
        return " "
    ln, rn = reloc_name(left), reloc_name(right)
    if ln and rn and ln != rn:
        if lt.replace(ln, "", 1) == rt.replace(rn, "", 1) and not left["instruction"].get("mnemonic", "").startswith("b"):
            return "~"
    return "!"


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("unit", help="objdiff unit, e.g. m524Dll/REL/m524Dll/m524")
    parser.add_argument("symbol", help="function name")
    parser.add_argument("--all", action="store_true", help="print every row")
    parser.add_argument("--context", type=int, default=3, help="rows kept around a difference")
    parser.add_argument(
        "--max-diff",
        type=int,
        default=0,
        help="cap how many differing/label rows are used as anchors (0 = no cap). "
        "Large functions produce hundreds of anchors and blow the context budget; "
        "page through them with --rows instead.",
    )
    parser.add_argument(
        "--rows",
        metavar="START:END",
        default="",
        help="print an explicit row slice instead of anchoring on differences",
    )
    args = parser.parse_args()

    unit = find_unit(args.unit)
    base = unit.get("base_path")
    if not base:
        raise SystemExit(f"{args.unit} has no source file yet (no base_path)")
    build = subprocess.run(["ninja", base], cwd=root)
    if build.returncode != 0:
        return build.returncode

    # This tool passes `-o`, which objdiff-cli 3.x removed, so a 3.x build
    # answers "Unrecognized argument: -o" and prints no diff at all. Prefer a
    # 2.7.1 build sitting next to it under a distinct name; fall back to the
    # configured one so a worktree that only has 2.7.1 as objdiff-cli.exe still
    # works. Do not overwrite objdiff-cli.exe itself: configure.py needs the 3.x
    # build to generate report.json.
    suffix = ".exe" if sys.platform == "win32" else ""
    tools_dir = root / "build" / "tools"
    exe = tools_dir / f"objdiff-cli-2.7.1{suffix}"
    if not exe.exists():
        exe = tools_dir / f"objdiff-cli{suffix}"
    if not exe.exists():
        # worktrees point configure.py at the main checkout's tools
        sys.path.insert(0, str(root / "tools"))
        from reconfigure import stored_args

        stored = stored_args()
        if "--objdiff" in stored:
            exe = Path(stored[stored.index("--objdiff") + 1])
    with tempfile.TemporaryDirectory() as tmp:
        out = Path(tmp) / "diff.json"
        result = subprocess.run(
            [str(exe), "diff", "-p", str(root), "-u", args.unit, "-o", str(out), "--format", "json", args.symbol],
            cwd=root,
            capture_output=True,
            text=True,
        )
        if result.returncode != 0 or not out.exists():
            print(result.stdout + result.stderr, file=sys.stderr)
            return result.returncode or 1
        data = json.loads(out.read_text(encoding="utf-8"))

    left, left_pct = symbol_rows(data["left"], args.symbol)
    right, right_pct = symbol_rows(data["right"], args.symbol)
    if left is None:
        raise SystemExit(f"{args.symbol} is not in the original object of {args.unit}")
    if right is None:
        print(f"{args.symbol}: not present in the compiled source yet ({len(left)} instructions to write)")
        return 0

    count = max(len(left), len(right))
    marks = [
        compare(left[i] if i < len(left) else {}, right[i] if i < len(right) else {})
        for i in range(count)
    ]
    differing = [i for i in range(count) if marks[i] == "!"]
    labels = [i for i in range(count) if marks[i] == "~"]
    match = right_pct if right_pct is not None else left_pct
    print(
        f"{args.symbol}: {match:.2f}% match, {len(left)} target rows, {len(right)} base rows, "
        f"{len(differing)} differing (!), {len(labels)} label-only (~)"
    )
    if not differing and not labels and not args.all:
        return 0

    if args.rows:
        lo_text, _, hi_text = args.rows.partition(":")
        lo = int(lo_text) if lo_text else 0
        hi = int(hi_text) if hi_text else count
        keep = set(range(max(0, lo), min(count, hi)))
    else:
        shown = differing + labels
        if args.max_diff and len(shown) > args.max_diff:
            shown = shown[: args.max_diff]
            print(
                f"NOTE: capped to the first {args.max_diff} of "
                f"{len(differing) + len(labels)} anchors; page the rest with --rows START:END"
            )
        keep = set(range(count)) if args.all else {
            j for i in shown for j in range(max(0, i - args.context), min(count, i + args.context + 1))
        }
    previous = -1
    print(f"         {'target (original)':<44} | {'base (src)':<44} line")
    for i in range(count):
        if i not in keep:
            continue
        if previous != -1 and i != previous + 1:
            print("   ...")
        previous = i
        l = row_text(left[i]) if i < len(left) else ""
        r_row = right[i] if i < len(right) else {}
        r = row_text(r_row)
        line = (r_row.get("instruction") or {}).get("line_number", "") if r_row else ""
        print(f"{marks[i]} {i:4d}  {l:<44} | {r:<44} {('L' + str(line)) if line != '' else ''}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
