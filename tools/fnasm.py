#!/usr/bin/env python3

###
# Prints the disassembly of a single function, followed by the values of
# only the rodata/data constants that function references.
#
# Reading a whole .s file is what exhausts a decompilation session's context,
# so this is the intended way to work: one function per call, nothing else.
#
# Usage:
#   python tools/fnasm.py m524Dll/REL/m524Dll/m524 fn_1_20A0
#   python tools/fnasm.py --list m524Dll/REL/m524Dll/m524
###

import re
import sys
from pathlib import Path

from _root import project_root

root = project_root()


def asm_path(unit: str) -> Path:
    module = unit.split("/")[0]
    name = unit.split("/")[-1].rsplit(".", 1)[0]
    relative = Path("build") / "GP5E01_00" / module / "asm" / "REL" / module / f"{name}.s"
    # The script may be run from another checkout (agents call it by absolute
    # path); fall back to the current directory, which is the worktree that
    # actually has the disassembly built.
    for base in (root, Path.cwd()):
        candidate = base / relative
        if candidate.exists():
            return candidate
    return root / relative


def functions(lines: list[str]) -> list[tuple[str, int, int]]:
    found = []
    for index, line in enumerate(lines):
        match = re.match(r"^\.fn (\S+?),", line)
        if match:
            found.append([match.group(1), index, None])
    for entry in found:
        for index in range(entry[1], len(lines)):
            if lines[index].startswith(f".endfn {entry[0]}"):
                entry[2] = index
                break
    return [tuple(entry) for entry in found if entry[2] is not None]


def constants(lines: list[str], wanted: list[str]) -> None:
    if not wanted:
        return
    blocks = {}
    index = 0
    while index < len(lines):
        header = re.match(r"^# \.(rodata|data):(0x[0-9A-Fa-f]+) \|", lines[index])
        if header:
            label = None
            cursor = index + 1
            while cursor < len(lines) and not lines[cursor].startswith("# ."):
                match = re.match(r"^\.obj (\S+?),", lines[cursor])
                if match:
                    label = match.group(1)
                elif label and lines[cursor].strip() and not lines[cursor].startswith(".endobj"):
                    blocks.setdefault(label, []).append(lines[cursor].strip())
                cursor += 1
            index = cursor
        else:
            index += 1
    print("\n--- referenced constants ---")
    for label in wanted:
        if label in blocks:
            print(f"{label}: {' '.join(blocks[label])}")
        else:
            print(f"{label}: <not in dump>")


def main() -> int:
    args = sys.argv[1:]
    if not args:
        print(__doc__)
        return 1
    if args[0] == "--list":
        if len(args) < 2:
            print(__doc__)
            return 1
        args = [args[1], "--list"]
    unit = args[0]
    path = asm_path(unit)
    if not path.exists():
        print(f"no disassembly at {path}; run ninja first")
        return 1
    lines = path.read_text(encoding="utf-8", errors="replace").splitlines()
    table = functions(lines)

    if args[1] == "--list":
        print(f"{len(table)} functions in {unit}")
        for name, start, end in table:
            print(f"  {name:<24} lines {start + 1}..{end + 1}")
        return 0

    name = args[1]
    entry = next((item for item in table if item[0] == name), None)
    if entry is None:
        print(f"{name} not found in {unit}")
        return 1
    _, start, end = entry
    body = lines[start:end + 1]
    print(f"--- {name} ({len(body)} lines) ---")
    for line in body:
        print(line)
    referenced = []
    for line in body:
        for label in re.findall(r"\b(lbl_1_(?:rodata|data)_[0-9A-Fa-f]+)\b", line):
            if label not in referenced:
                referenced.append(label)
    constants(lines, referenced)
    return 0


if __name__ == "__main__":
    sys.exit(main())