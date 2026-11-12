#!/usr/bin/env python3
"""Writes a .def file exporting the C-linkage API (GX, OS, VI, PAD, ...) of aurora's static libs.

RELs link against dol like a GameCube module links against main.dol, but
WINDOWS_EXPORT_ALL_SYMBOLS only covers dol's own objects. Without these exports
every REL pulls its own copy of aurora's code (and its own GX state), so GX
calls made from REL code never reach the real FIFO.
Usage: gen_gx_def.py <out.def> <aurora lib>...
"""
import re
import struct
import sys


def lib_symbols(path):
    d = open(path, "rb").read()
    assert d[:8] == b"!<arch>\n"
    size = int(d[8 + 48:8 + 58])
    body = d[8 + 60:8 + 60 + size]  # first linker member: big-endian offsets + names
    n = struct.unpack(">I", body[:4])[0]
    return [s.decode() for s in body[4 + 4 * n:].split(b"\0")[:n]]


# the few other aurora entry points RELs call (anything broader makes dol pull in aurora objects that shadow src/port)
EXTRA = {"OSGetTick", "OSGetTime", "OSTicksToCalendarTime"}
names = set()
for lib in sys.argv[2:]:
    names |= {s for s in lib_symbols(lib) if re.fullmatch(r"(__)?GX[A-Za-z0-9_]*", s) or s in EXTRA}
with open(sys.argv[1], "w") as f:
    f.write("EXPORTS\n" + "".join("    %s\n" % s for s in sorted(names)))
