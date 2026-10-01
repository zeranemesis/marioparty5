#!/usr/bin/env python3

###
# Compares the data sections of a unit's target object with the ones the current
# sources produce, word by word.
#
# Usage:
#   python tools/sectdiff.py main/game/board/capmain
#   python tools/sectdiff.py capmain --section .sdata2
#   python tools/sectdiff.py --all --offending
#
# objdiff reports a data section as a single percentage, which hides the most
# common cause of a near miss: the pool is correct but starts a few bytes late
# or early, so every relocation into it is off by the same amount. That shows up
# here as a pure shift, with no differing words at all, and the cause is usually
# a single declaration that MWCC emitted somewhere it should not have.
###

import argparse
import contextlib
import io
import json
import os
import re
import struct
import subprocess
import sys
from pathlib import Path

root = Path(__file__).resolve().parent.parent

EXE = ".exe" if sys.platform == "win32" else ""
SECTIONS = (
    ".data",
    ".rodata",
    ".sdata",
    ".sdata2",
    ".sbss",
    ".sbss2",
    ".bss",
    ".lit4",
    ".lit8",
)


def find_objdump() -> Path:
    """Locates objdump, which in a worktree lives in the checkout owning the build."""
    override = os.environ.get("OBJDUMP")
    if override:
        return Path(override)
    here = root / "build" / "binutils" / f"powerpc-eabi-objdump{EXE}"
    if here.exists():
        return here
    ninja = root / "build.ninja"
    if ninja.exists():
        text = ninja.read_text(errors="ignore")
        found = re.search(r"[A-Za-z]:.*?build[\\/]tools[\\/]", text)
        if found:
            shared = Path(found.group(0)).parent / "binutils" / f"powerpc-eabi-objdump{EXE}"
            if shared.exists():
                return shared.resolve()
    raise SystemExit("powerpc-eabi-objdump not found; set OBJDUMP to its path")


OBJDUMP = find_objdump()


def section_bytes(obj: Path, section: str) -> bytes:
    """Returns the raw contents of one section, empty if it is absent."""
    if not obj.exists():
        return b""
    result = subprocess.run(
        [str(OBJDUMP), "-s", "-j", section, str(obj)],
        capture_output=True,
        text=True,
    )
    if result.returncode != 0:
        return b""
    data = bytearray()
    for line in result.stdout.splitlines():
        # " 0010 00000000 3d800000 ...|...." -- keep the four hex groups only.
        parts = line.split()
        if len(parts) < 6 or parts[0].strip("0123456789abcdef"):
            continue
        try:
            data += bytes.fromhex("".join(parts[1:5]))
        except ValueError:
            continue
    return bytes(data)


def words(data: bytes) -> list:
    """Splits a section into 4-byte groups, zero-padding a ragged tail."""
    padded = data + b"\0" * (-len(data) % 4)
    return [padded[i : i + 4].hex() for i in range(0, len(padded), 4)]


def describe(word: str) -> str:
    """Renders a word as a float and, alongside, as characters."""
    value = struct.unpack(">f", bytes.fromhex(word))[0]
    text = "".join(chr(b) if 32 <= b < 127 else "." for b in bytes.fromhex(word))
    return f"{value!r:<22} {text}"


def report(unit: str, config: dict, only) -> bool:
    """Prints the section comparison for one unit; True if every section matches."""
    target = root / config["target_path"]
    base = root / config["base_path"]
    clean = True
    print(unit)
    if not base.exists():
        print("    no rebuilt object, run ninja first")
        return False
    for section in SECTIONS:
        if only and section != only:
            continue
        want = words(section_bytes(target, section))
        got = words(section_bytes(base, section))
        if not want and not got:
            continue
        if want == got:
            print(f"    {section:<10} identical ({len(want)} words)")
            continue
        clean = False
        # A shift shows up as the whole target appearing at some offset in ours.
        shift = next(
            (delta for delta in range(1, len(got)) if want == got[delta : delta + len(want)]),
            None,
        )
        if shift is not None:
            print(
                f"    {section:<10} shifted by {shift * 4} bytes: "
                f"target {len(want)} words, rebuilt {len(got)}"
            )
            extra = got[:shift]
            for i in range(0, len(extra), 2):
                chunk = extra[i : i + 2]
                print(f"        +0x{i * 4:04x}  {'  '.join(describe(w) for w in chunk)}")
            continue
        print(f"    {section:<10} differs ({len(want)} vs {len(got)} words)")
        for i in range(min(len(want), len(got))):
            if want[i] != got[i]:
                print(f"        0x{i * 4:04x}  target  {describe(want[i])}")
                print(f"                rebuilt {describe(got[i])}")
    return clean


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("unit", nargs="?", help="unit name, or a fragment of one")
    parser.add_argument("--section", help="only compare this section")
    parser.add_argument("--all", action="store_true", help="compare every unit")
    parser.add_argument("--offending", action="store_true", help="only list units that differ")
    args = parser.parse_args()

    config = json.loads((root / "objdiff.json").read_text(encoding="utf-8"))
    units = {unit["name"]: unit for unit in config["units"]}
    if not args.all and not args.unit:
        parser.error("give a unit name or --all")

    if args.all:
        names = [
            name
            for name, unit in units.items()
            if (root / unit["base_path"]).exists() and (root / unit["target_path"]).exists()
        ]
        if args.offending:
            names = [
                name
                for name in names
                if not quiet(name, units[name], args.section)
            ]
        for name in names:
            report(name, units[name], args.section)
        return 0

    names = [name for name in units if name == args.unit or args.unit in name]
    if not names:
        print(f"no unit matches '{args.unit}'", file=sys.stderr)
        return 1
    for name in names:
        report(name, units[name], args.section)
    return 0


def quiet(unit: str, config: dict, only) -> bool:
    """Runs report() with its output discarded; used to filter --all."""
    with contextlib.redirect_stdout(io.StringIO()):
        return report(unit, config, only)


if __name__ == "__main__":
    sys.exit(main())
