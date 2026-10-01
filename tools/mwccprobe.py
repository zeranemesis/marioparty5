#!/usr/bin/env python3

###
# Recreated scratch tooling: compile a single .c with the project's real MWCC
# flags, then objdump it. Authoritative for iterating on dolphin/os/* (the
# "asShapes" experiments) - do NOT score with it, use tools/fndiff.py.
#
# Usage:
#   python tools/mwccprobe.py src/dolphin/os/OSContext.c [fn ...]
###

import os
import re
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent

CFLAGS = (
    "-nodefaults -proc gekko -align powerpc -enum int -fp hardware "
    "-Cpp_exceptions off -O4,p -inline auto -pragma \"cats off\" "
    "-pragma \"warn_notinlined off\" -maxerrors 30 -nosyspath -RTTI off "
    "-fp_contract on -str reuse -multibyte "
    "-i include -i libc -i libs/dolphin/include -i extern/musyx/include -i src "
    "-DMUSY_TARGET=MUSY_TARGET_DOLPHIN -i build/GP5E01_00/include "
    "-DVERSION=0 -DNDEBUG=1 -lang=c"
)

CC = os.path.abspath("build/compilers/GC/1.2.5n/mwcceppc.exe")
OBJDUMP = os.path.abspath("build/binutils/powerpc-eabi-objdump.exe")


def compile_one(src: str, out_obj: str) -> None:
    # built as one string: mwcceppc mis-parses list2cmdline quoting of -pragma "..."
    cmd = f'"{CC}" {CFLAGS} -c "{src}" -o "{out_obj}"'
    result = subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True, shell=True)
    if result.returncode != 0:
        print(result.stdout + result.stderr, file=sys.stderr)
        raise SystemExit(result.returncode)
    if result.stdout.strip():
        print(result.stdout, file=sys.stderr)


def dump(obj: str) -> str:
    result = subprocess.run([OBJDUMP, "-d", "-r", "-j", ".text", obj],
                            cwd=ROOT, capture_output=True, text=True)
    return result.stdout


def main() -> int:
    if len(sys.argv) < 2:
        print(__doc__, file=sys.stderr)
        return 2
    src = sys.argv[1]
    wanted = sys.argv[2:]
    tmpdir = tempfile.mkdtemp(dir=str(ROOT / "build"))
    obj = os.path.join(tmpdir, "probe.o")
    compile_one(src, obj)
    text = dump(obj)
    # split per function
    chunks = re.split(r"\n(?=[0-9a-f]{8} <)", text)
    for chunk in chunks:
        m = re.match(r"[0-9a-f]{8} <([^>]+)>:", chunk)
        if not m:
            continue
        name = m.group(1)
        if wanted and name not in wanted:
            continue
        print(chunk.rstrip())
        print()
    Path(obj).unlink(missing_ok=True)
    os.rmdir(tmpdir)
    return 0


if __name__ == "__main__":
    sys.exit(main())