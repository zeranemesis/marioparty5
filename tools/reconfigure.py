#!/usr/bin/env python3

###
# Re-runs configure.py with the arguments the current build.ninja was generated
# with (a worktree passes --dtk/--objdiff/... pointing at shared tools).
#
# Needed after creating a new source file: objdiff.json only gives a unit a
# base object once its source exists, and that is decided at configure time.
#
# Usage:
#   python tools/reconfigure.py
###

import subprocess
import sys
from pathlib import Path

root = Path(__file__).resolve().parent.parent


def stored_args() -> list:
    build_ninja = root / "build.ninja"
    if not build_ninja.exists():
        return []
    lines = build_ninja.read_text(encoding="utf-8").splitlines()
    for index, line in enumerate(lines):
        if line.startswith("configure_args ="):
            value = line[len("configure_args ="):].strip()
            # a trailing "$" continues the value on the next (indented) line
            while value.endswith("$") and not value.endswith("$$") and index + 1 < len(lines):
                index += 1
                value = value[:-1] + lines[index].strip()
            # ninja escapes: "$ " is a space inside one argument, "$$" is "$"
            args, current, i = [], "", 0
            while i < len(value):
                c = value[i]
                if c == "$" and i + 1 < len(value):
                    current += value[i + 1]
                    i += 2
                    continue
                if c == " ":
                    if current:
                        args.append(current)
                    current = ""
                else:
                    current += c
                i += 1
            if current:
                args.append(current)
            return args
    return []


def main() -> int:
    args = stored_args()
    cmd = [sys.executable, "configure.py", *args]
    print("RUN", " ".join(cmd))
    return subprocess.run(cmd, cwd=root).returncode


if __name__ == "__main__":
    sys.exit(main())
