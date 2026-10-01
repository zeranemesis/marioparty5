#!/usr/bin/env python3

###
# Creates an isolated worktree for decompiling one module in parallel with
# other work, ready to build:
#   - git worktree on a new branch decomp/<name> (from main by default)
#   - submodule working trees copied from the main checkout (see link_submodules)
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


def link_submodules(root: Path, path: Path) -> None:
    """Copy submodule working trees from the main checkout into the worktree.

    `git submodule update --init` cannot be used here: the superproject pins
    musyx at 39b305c7, which the remote no longer serves ("not our ref"), so
    the clone fails while the main checkout happily sits on another commit.
    Copying the tree and pointing the submodule back at the main checkout's
    module store gives the same content the main checkout builds against.
    """
    listing = run(
        ["git", "config", "--file", ".gitmodules", "--get-regexp", r"^submodule\..*\.path$"],
        cwd=root,
        capture=True,
    )
    modules_root = root / ".git" / "modules"
    for line in listing.stdout.splitlines():
        sub = line.split(None, 1)[-1].strip()
        src = root / sub
        if not src.is_dir():
            print(f"SKIP submodule {sub}: not present in {root}")
            continue
        dst = path / sub
        if dst.exists():
            shutil.rmtree(dst)
        print(f"COPY submodule {sub}")
        shutil.copytree(src, dst, ignore=shutil.ignore_patterns(".git"))
        module_dir = modules_root / sub
        if module_dir.is_dir():
            pointer = os.path.relpath(module_dir, dst)
            (dst / ".git").write_text(f"gitdir: {pointer}\n", encoding="utf-8")
        else:
            print(f"WARN submodule {sub}: no gitdir at {module_dir}")


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
    link_submodules(root, path)

    orig_src = root / "orig" / version
    orig_dst = path / "orig" / version
    orig_dst.mkdir(parents=True, exist_ok=True)
    # Walk the tree and hard-link (or copy) every regular file, skipping
    # directories we cannot read and entries like CVS metadata.
    for dirpath, dirnames, filenames in os.walk(orig_src):
        dirnames[:] = [d for d in dirnames if d not in (".gitkeep", "CVS")]
        rel = os.path.relpath(dirpath, orig_src)
        dest_dir = orig_dst if rel == "." else orig_dst / rel
        dest_dir.mkdir(parents=True, exist_ok=True)
        for name in filenames:
            if name == ".gitkeep":
                continue
            src_file = os.path.join(dirpath, name)
            dst_file = dest_dir / name
            try:
                os.link(src_file, dst_file)
                print(f"LINK {dst_file}")
            except OSError:
                try:
                    shutil.copy2(src_file, dst_file)
                    print(f"COPY {dst_file}")
                except OSError:
                    print(f"SKIP {dst_file}")

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
