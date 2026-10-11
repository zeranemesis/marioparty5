#!/usr/bin/env python3

###

# Locates the project root for the measurement tools.
#
# These tools are invoked from a worktree, not from the main checkout:
#
#     cd D:\decomp\mp5-wt\m521Dll
#     python D:\decomp\MP5\tools\unit_status.py m521Dll
#
# so the project root must come from the CURRENT DIRECTORY. Resolving it
# from `Path(__file__).resolve().parent.parent` instead silently points at
# D:\decomp\MP5, which has two bad consequences: the numbers describe the
# main checkout rather than the worktree being decompiled, and any `ninja`
# call rebuilds the main checkout. With fifteen agents sharing sixteen
# cores, that second effect is what saturated the machine.
#
# Fall back to the script's own location only when the current directory is
# not inside a project checkout.

import os

from pathlib import Path


def project_root() -> Path:
    """Return the checkout containing the current directory, else this repo."""
    try:
        start = Path(os.getcwd()).resolve()
    except OSError:
        start = None

    if start is not None:
        for candidate in (start, *start.parents):
            if (candidate / "configure.py").is_file() and (candidate / "tools").is_dir():
                return candidate

    return Path(__file__).resolve().parent.parent


def pool_path() -> Path:
    """Return the agent pool registry POOL.md.

    The pool directory is a sibling of the checkout, and it moves: this tree
    lived under D:\\decomp and now lives under G:\\decomp, which silently broke
    every tool that hardcoded the old absolute path. Search upward instead.
    """
    root = project_root()
    for candidate in (root, *root.parents):
        for rel in ("mp5-wt/POOL.md", "POOL.md"):
            p = candidate / rel
            if p.is_file():
                return p
    return root.parent / "mp5-wt" / "POOL.md"