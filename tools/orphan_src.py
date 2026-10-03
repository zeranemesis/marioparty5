#!/usr/bin/env python3

### Lists every unit declared Object(NonMatching, ...) in configure.py whose
### source file nevertheless exists on disk.
#
# Usage:
#   python tools/orphan_src.py
#   python tools/orphan_src.py --min-bytes 2000
#
# Why this matters. NonMatching means "incomplete or absent: out of the port".
# Normally the source file is absent, so the unit emits no compile rule and the
# build is unaffected. But a unit can be marked NonMatching while a real source
# file sits next to it, in which case that source is never compiled, never
# verified, and never counted by report.json -- yet it may already be largely
# correct. m517Dll/gamemaster is one: 16/34 functions and 1356/7240 bytes, with
# nine commits of work on it, invisible to every measurement we have.
#
# So each hit is potentially free percentage. Flip it to Equivalent, compile it,
# and measure it. Nothing else in the pool is that cheap.
#
# It is also a latent build hazard: the build only works because the original
# object occupies the dependency path, so any change to the dependency graph
# could silently start compiling a source nobody has verified.

import argparse
import re
from pathlib import Path

root = Path(__file__).resolve().parent.parent
STATUS_RE = re.compile(r'Object\((\w+),\s*"([^"]+)"\)')


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--min-bytes", type=int, default=0,
                        help="only report sources at least this large")
    args = parser.parse_args()

    text = (root / "configure.py").read_text(encoding="utf-8", errors="ignore")
    hits = []
    for status, path in STATUS_RE.findall(text):
        if status != "NonMatching":
            continue
        source = root / "src" / path
        if source.exists():
            size = source.stat().st_size
            if size >= args.min_bytes:
                hits.append((path, size))

    hits.sort(key=lambda row: -row[1])
    total = sum(size for _, size in hits)
    print(f"{len(hits)} Object(NonMatching) units have a source file on disk, "
          f"{total} bytes in total\n")
    for path, size in hits:
        print(f"  {path:<48} {size:>8}")
    print("\nFor each one: read it, set it to Object(Equivalent, ...), build the "
          "module, and measure with unit_status.py. Keep it NonMatching only if "
          "it does not compile or is worse than nothing.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())