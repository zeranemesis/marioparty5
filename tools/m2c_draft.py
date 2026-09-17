#!/usr/bin/env python3

###
# Produces a first draft of C for a unit with m2c, from the assembly dtk split
# out of the original binary. The draft is a starting point, never a result:
# it is written under build/m2c/ so it cannot be committed by accident.
#
# Usage:
#   python tools/m2c_draft.py m524Dll/REL/m524Dll/m524
#   python tools/m2c_draft.py main/msm/msmsys -f msmSysInit
#   python tools/m2c_draft.py m524Dll/REL/m524Dll/m524 --no-context
#
# m2c is looked up in $MP5_M2C, then next to the repository in
# ../mp5-tools/m2c/m2c.py (worktrees also look beside the main checkout).
#
# The context is the unit's own source when it exists; otherwise a synthetic
# file including every game/dolphin/datanum header the decompiled RELs use.
###

import argparse
import os
import re
import subprocess
import sys
from pathlib import Path

root = Path(__file__).resolve().parent.parent
version = "GP5E01_00"


def find_m2c() -> Path:
    env = os.environ.get("MP5_M2C")
    candidates = [Path(env)] if env else []
    for base in (root.parent, Path.home()):
        candidates.append(base / "mp5-tools" / "m2c" / "m2c.py")
    for candidate in candidates:
        if candidate.is_file():
            return candidate
    raise SystemExit("m2c.py not found: set MP5_M2C or clone m2c into ../mp5-tools/m2c")


def unit_paths(unit: str):
    module, _, rest = unit.partition("/")
    if not rest:
        raise SystemExit("unit must look like <module>/<path>, e.g. m524Dll/REL/m524Dll/m524")
    if module == "main":
        asm = root / "build" / version / "asm" / f"{rest}.s"
    else:
        asm = root / "build" / version / module / "asm" / f"{rest}.s"
    src = None
    for ext in (".c", ".cpp"):
        if (root / "src" / f"{rest}{ext}").exists():
            src = root / "src" / f"{rest}{ext}"
    return module, rest, asm, src


def common_includes() -> list:
    pattern = re.compile(r'^#\s*include\s*"((?:game|dolphin|datanum|msm|musyx)[^"]*|dolphin\.h|math\.h|string\.h)"')
    found = []
    for path in sorted((root / "src" / "REL").rglob("*.c")):
        for line in path.read_text(encoding="utf-8", errors="replace").splitlines():
            m = pattern.match(line.strip())
            if m and m.group(1) not in found:
                found.append(m.group(1))
    return found


def compilers_dir() -> Path:
    local = root / "build" / "compilers"
    if local.is_dir():
        return local
    sys.path.insert(0, str(root / "tools"))
    from reconfigure import stored_args

    args = stored_args()
    if "--compilers" in args:
        return Path(args[args.index("--compilers") + 1])
    raise SystemExit("compilers not found: run ninja first")


def build_context(out_dir: Path, src: Path) -> Path:
    # m2c cannot parse preprocessor directives, so the context is the unit's
    # includes run through MWCC's own preprocessor, which also sees exactly the
    # macros the real build sees.
    ctx = out_dir / "ctx.c"
    ctx_src = out_dir / "ctx_src.c"
    if src is not None:
        includes = [
            line for line in src.read_text(encoding="utf-8", errors="replace").splitlines()
            if re.match(r'^#\s*include\s*"', line.strip()) and not line.strip().endswith('.c"')
        ]
        ctx_src.write_text("\n".join(includes) + "\n", encoding="utf-8")
    else:
        ctx_src.write_text("".join(f'#include "{inc}"\n' for inc in common_includes()), encoding="utf-8")
    mwcc = compilers_dir() / "GC" / "2.6" / "mwcceppc.exe"
    cmd = [
        str(mwcc), "-EP", "-nodefaults", "-nosyspath", "-proc", "gekko", "-lang=c",
        "-DMUSY_TARGET=MUSY_TARGET_DOLPHIN", "-DVERSION=0", "-DNDEBUG=1",
        "-i", "include", "-i", "src", "-i", f"build/{version}/include", str(ctx_src),
    ]
    if sys.platform != "win32":
        cmd.insert(0, "wibo")
    result = subprocess.run(cmd, cwd=root, capture_output=True, text=True)
    if result.returncode != 0:
        print(result.stdout[-2000:] + result.stderr[-2000:], file=sys.stderr)
        raise SystemExit("preprocessing the context failed")
    lines = [line for line in result.stdout.splitlines() if not line.lstrip().startswith("#")]
    ctx.write_text(sanitize_context("\n".join(lines)) + "\n", encoding="utf-8")
    return ctx


def strip_balanced(text: str, keyword: str) -> str:
    # removes `keyword(...)` including nested parentheses
    out = []
    i = 0
    while True:
        j = text.find(keyword, i)
        if j < 0:
            out.append(text[i:])
            return "".join(out)
        out.append(text[i:j])
        k = j + len(keyword)
        while k < len(text) and text[k].isspace():
            k += 1
        if k >= len(text) or text[k] != "(":
            out.append(keyword)
            i = j + len(keyword)
            continue
        depth = 0
        while k < len(text):
            if text[k] == "(":
                depth += 1
            elif text[k] == ")":
                depth -= 1
                if depth == 0:
                    k += 1
                    break
            k += 1
        i = k


def sanitize_context(text: str) -> str:
    # m2c only needs types and prototypes. Function bodies in headers (inline
    # helpers, MWCC `asm` blocks) are what its C parser chokes on, so every
    # top-level body becomes a plain prototype.
    text = strip_balanced(text, "__attribute__")
    text = strip_balanced(text, "__declspec")
    text = re.sub(r"\)\s*:\s*0x[0-9A-Fa-f]+\s*;", ");", text)
    text = re.sub(r"(\w)\s*:\s*0x[0-9A-Fa-f]+\s*;", r"\1;", text)
    out = []
    depth = 0
    i = 0
    n = len(text)
    while i < n:
        c = text[i]
        if depth == 0 and c == "{":
            prev = "".join(out).rstrip()
            if prev.endswith(")"):
                # skip the whole body
                body_depth = 1
                i += 1
                while i < n and body_depth:
                    if text[i] == "{":
                        body_depth += 1
                    elif text[i] == "}":
                        body_depth -= 1
                    i += 1
                out.append(";")
                continue
        if c == "{":
            depth += 1
        elif c == "}":
            depth = max(0, depth - 1)
        out.append(c)
        i += 1
    text = "".join(out)
    text = re.sub(r"\basm\b", "", text)
    return text


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("unit", help="objdiff unit name, e.g. m524Dll/REL/m524Dll/m524")
    parser.add_argument("-f", "--function", action="append", default=[], help="only these functions")
    parser.add_argument("--no-context", action="store_true")
    parser.add_argument("--stdout", action="store_true", help="print instead of writing build/m2c/...")
    args = parser.parse_args()

    module, rest, asm, src = unit_paths(args.unit)
    if not asm.exists():
        raise SystemExit(f"no assembly at {asm}: run ninja first")
    out_dir = root / "build" / "m2c" / module / Path(rest).parent
    out_dir.mkdir(parents=True, exist_ok=True)

    cmd = [sys.executable, str(find_m2c()), "-t", "ppc-mwcc-c"]
    if not args.no_context:
        cmd += ["--context", str(build_context(out_dir, src))]
    for fn in args.function:
        cmd += ["-f", fn]
    cmd.append(str(asm))

    result = subprocess.run(cmd, cwd=root, capture_output=True, text=True)
    if result.returncode != 0 and not args.no_context:
        print("m2c failed with the context, retrying without it:", file=sys.stderr)
        print((result.stderr or result.stdout)[-2000:], file=sys.stderr)
        cmd = [c for c in cmd if not c.endswith("ctx.c") and c != "--context"]
        result = subprocess.run(cmd, cwd=root, capture_output=True, text=True)
    if result.returncode != 0:
        print(result.stdout + result.stderr, file=sys.stderr)
        return result.returncode

    if args.stdout:
        print(result.stdout)
    else:
        out = out_dir / (Path(rest).name + ("." + "_".join(args.function) if args.function else "") + ".m2c.c")
        out.write_text(result.stdout, encoding="utf-8")
        print(f"draft written to {out.relative_to(root)} ({len(result.stdout.splitlines())} lines)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
