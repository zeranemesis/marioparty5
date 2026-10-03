#!/usr/bin/env python3

###
# Ground truth for what is actually byte-identical: compares every module's
# built .rel against config/GP5E01_00/build.sha1, which is the same check the
# `build/GP5E01_00/ok` ninja target does, one module at a time.
#
# Usage:
#   python tools/rel_hash.py            # every module with a .rel on disk
#   python tools/rel_hash.py --missing  # also list modules with no .rel yet
#
# This replaces guessing from object symbols. Whether a module links is a
# property of the finished .rel, not of any single object: m526Dll's math.o
# carries MSL's two unused sqrtf statics and m526Dll still hashes correct, so
# the symbol heuristic in promote_scan.py cannot predict link failure. Hashing
# the artefact can.
###

import argparse
import hashlib
import sys
from pathlib import Path

from _root import project_root

root = project_root()


def sha1(path: Path) -> str:
    digest = hashlib.sha1()
    with path.open("rb") as handle:
        for chunk in iter(lambda: handle.read(1 << 20), b""):
            digest.update(chunk)
    return digest.hexdigest()


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--version", default="GP5E01_00")
    parser.add_argument("--missing", action="store_true", help="list modules with no .rel")
    args = parser.parse_args()

    sha_file = root / "config" / args.version / "build.sha1"
    if not sha_file.exists():
        print(f"missing {sha_file}", file=sys.stderr)
        return 1

    want = {}
    for line in sha_file.read_text(encoding="utf-8", errors="ignore").splitlines():
        parts = line.split()
        if len(parts) >= 2 and parts[1].endswith(".rel"):
            want[Path(parts[1]).stem] = parts[0]

    ok, bad, absent, stale = [], [], [], []
    for rel, expected in sorted(want.items()):
        module = rel.split("/")[-1]
        path = root / "build" / args.version / module / f"{module}.rel"
        if not path.exists():
            absent.append(module)
            continue
        # A .rel older than the objects it was linked from proves nothing: it is
        # a leftover from before the last source change. This bit me -- a stale
        # e3setupdll.rel reported a match while the module was in fact broken.
        # Only the rebuilt objects count as inputs; a module whose units are all
        # NonMatching links the original objects under <module>/obj/, which do
        # not change, so its .rel can never go stale.
        rel_time = path.stat().st_mtime
        newest = 0.0
        for obj in (root / "build" / args.version / "src" / "REL").glob(
            f"{module}/*.o"
        ):
            newest = max(newest, obj.stat().st_mtime)
        if newest > rel_time:
            stale.append(module)
            continue
        (ok if sha1(path) == expected else bad).append(module)

    print(f"{len(ok)} of {len(want)} module .rel files match the recorded SHA-1")
    if bad:
        print(f"\nMISMATCH ({len(bad)}) -- these are the modules that are NOT byte-identical:")
        for module in bad:
            print(f"    {module}")
    if stale:
        print(
            f"\nSTALE ({len(stale)}) -- .rel older than its objects or configure.py, "
            "so the hash above says nothing. Relink before trusting them:"
        )
        for module in stale:
            print(f"    {module}")
    if args.missing and absent:
        print(f"\nnot built ({len(absent)}):")
        for module in absent:
            print(f"    {module}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
