#!/usr/bin/env python3

###
# Classifies every `singleton.h` assert call site in the project.
#
# Usage:
#   python tools/assert_scan.py             # per-module summary
#   python tools/assert_scan.py --module m513Dll
#
# Why this exists. For several sessions the project has carried the belief that
# MWCC 2.6 cannot place an assert block out of line, and that units containing
# one could therefore never reach Matching. That belief was based on hand-written
# source that calls `__assert_fail("ms_pointer == 0", "singleton.h", 0x35)`
# INLINE. The original objects do not contain `__assert_fail` at all: they import
# one shared assert function per module, defined once in the module's main
# object. A caller of a function that RETURNS needs no cold block, so there is
# no wall -- the source was simply decompiling a call as an inline body.
#
# This tool reports, per module, whether that is the shape on disk, so the
# difference between "compiler limitation" and "wrong decompilation" is a
# measurement rather than a belief.
#
# Reads only the original objects. Never builds.
###

import argparse
import subprocess
import sys
from collections import defaultdict
from pathlib import Path

from _root import project_root

root = project_root()

MARKER = b"singleton.h"


def exe(name: str) -> str:
    return f"{name}.exe" if sys.platform == "win32" else name


def symbols(nm: Path, obj: Path) -> tuple[set, set]:
    """Returns (defined text symbols, undefined symbols) for one object."""
    result = subprocess.run([str(nm), str(obj)], capture_output=True, text=True)
    defined, undefined = set(), set()
    for line in result.stdout.splitlines():
        parts = line.split()
        if len(parts) < 2:
            continue
        kind, name = parts[-2], parts[-1]
        if kind == "U":
            undefined.add(name)
        elif kind in "TtWwVv":
            defined.add(name)
    return defined, undefined


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--module", help="restrict to one module")
    args = parser.parse_args()

    nm = root / "build" / "binutils" / exe("powerpc-eabi-nm")
    if not nm.exists():
        print(f"missing {nm}", file=sys.stderr)
        return 1

    objdir = root / "build" / "GP5E01_00"
    modules = sorted(p for p in objdir.iterdir() if p.is_dir() and (p / "obj").is_dir())
    if args.module:
        modules = [p for p in modules if p.name == args.module]

    for moddir in modules:
        objs = sorted((moddir / "obj").rglob("*.o"))
        call_sites = [o for o in objs if MARKER in o.read_bytes()]
        if not call_sites:
            continue
        defined_in = defaultdict(set)
        undefined_in = defaultdict(set)
        for obj in objs:
            defined, undefined = symbols(nm, obj)
            for name in defined:
                defined_in[name].add(obj.stem)
            for name in undefined:
                undefined_in[name].add(obj.stem)

        if "__assert_fail" in defined_in:
            shape = "assert_fail DEFINED here (libc, inlined)"
        elif "__assert_fail" in undefined_in:
            shape = "assert_fail IMPORTED"
        else:
            shape = "assert_fail ABSENT"

        # The shared callee: undefined at a call site, defined in exactly one
        # object of this module. That is the module's own assert function.
        callee = ""
        for obj in call_sites:
            _, undefined = symbols(nm, obj)
            for name in undefined:
                if len(defined_in.get(name, ())) == 1 and name.startswith("fn_1_"):
                    callee = name
                    break
            if callee:
                break
        where = ""
        if callee:
            home = next(iter(defined_in[callee]))
            where = f", defined once in {home}"

        print(
            f"{moddir.name:<12} {len(call_sites):>2} site(s)  {shape}"
            + (f"  -> call {callee} ({where})" if callee else "")
        )
    return 0


if __name__ == "__main__":
    sys.exit(main())
