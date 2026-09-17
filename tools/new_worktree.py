#!/usr/bin/env python3

###
# Creates an isolated worktree for decompiling one module in parallel with
# other work, ready to build:
#   - git worktree on a new branch decomp/<name> (from main by default)
#   - the disc image hard-linked into orig/ (no copy, no second 900 MB file)
#   - configure.py pointed at the tools already downloaded by this checkout
#   - a first ninja run, which must end with every file OK
#
# Usage (from the main checkout):
#   python tools/new_worktree.py resultDll
#   python tools/new_worktree.py msm --root C:\Users\BEACALCADM\mp5-wt
#
# Remove it afterwards with:
#   git worktree remove <path>   and   git branch -d decomp/<name>
###

import argparse
import os
import shutil
import subprocess
import sys
from pathlib import Path

root = Path(__file__).resolve().parent.parent
version = "GP5E01_00"


def run(cmd, cwd, check=True, capture=False):
    print("RUN", " ".join(str(c) for c in cmd))
    result = subprocess.run([str(c) for c in cmd], cwd=cwd, text=True, capture_output=capture)
    if check and result.returncode != 0:
        if capture:
            print(result.stdout[-4000:] + result.stderr[-4000:])
        raise SystemExit(f"failed ({result.returncode}): {' '.join(str(c) for c in cmd)}")
    return result


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("name", help="module or task name, e.g. resultDll")
    parser.add_argument("--root", default=str(root.parent / "mp5-wt"), help="directory holding worktrees")
    parser.add_argument("--base", default="main", help="commit or branch to start from")
    parser.add_argument("--branch", help="branch name (default decomp/<name>)")
    parser.add_argument("--no-build", action="store_true", help="skip the first ninja run")
    args = parser.parse_args()

    tools = root / "build" / "tools"
    exe = ".exe" if sys.platform == "win32" else ""
    needed = {
        "--dtk": tools / f"dtk{exe}",
        "--objdiff": tools / f"objdiff-cli{exe}",
        "--sjiswrap": tools / "sjiswrap.exe",
        "--binutils": root / "build" / "binutils",
        "--compilers": root / "build" / "compilers",
    }
    missing = [str(p) for p in needed.values() if not p.exists()]
    if missing:
        raise SystemExit("run ninja in the main checkout first; missing: " + ", ".join(missing))

    path = Path(args.root) / args.name
    branch = args.branch or f"decomp/{args.name}"
    if path.exists():
        raise SystemExit(f"{path} already exists")
    path.parent.mkdir(parents=True, exist_ok=True)
    run(["git", "worktree", "add", path, "-b", branch, args.base], cwd=root)

    orig_src = root / "orig" / version
    orig_dst = path / "orig" / version
    orig_dst.mkdir(parents=True, exist_ok=True)
    for item in orig_src.iterdir():
        if item.name == ".gitkeep" or not item.is_file():
            continue
        target = orig_dst / item.name
        try:
            os.link(item, target)
            print(f"LINK {target}")
        except OSError:
            shutil.copy2(item, target)
            print(f"COPY {target}")

    configure = [sys.executable, "configure.py"]
    for flag, value in needed.items():
        configure += [flag, value]
    run(configure, cwd=path)

    if not args.no_build:
        result = run(["ninja"], cwd=path, check=False, capture=True)
        tail = "\n".join((result.stdout + result.stderr).splitlines()[-14:])
        print(tail)
        if result.returncode != 0 or "files OK" not in result.stdout:
            raise SystemExit("the first build of the worktree did not end OK")

    print(f"\nready: {path} on branch {branch}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
