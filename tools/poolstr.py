#!/usr/bin/env python3
"""
Decode a doldecomp `.string "..."` blob and slice it at the offsets the code
uses, so the individual literals of a pooled string table can be recovered.

Usage:
  python tools/poolstr.py build/GP5E01_00/asm/dolphin/os/OSCache.s 0x0 0x2c 0x44
"""

import re
import sys

BS = chr(92)


def unescape(raw: str) -> bytes:
    out = bytearray()
    i = 0
    while i < len(raw):
        c = raw[i]
        if c == BS:
            m = re.match(r"[0-7]{1,3}", raw[i + 1:])
            if m:  # \000 style octal, one byte
                out.append(int(m.group(0), 8))
                i += 1 + len(m.group(0))
                continue
            n = raw[i + 1]
            out.append({"n": 10, "t": 9, "r": 13, BS: 92, '"': 34}[n])
            i += 2
        else:
            out.append(ord(c))
            i += 1
    return bytes(out)


def main() -> int:
    path = sys.argv[1]
    offs = [int(a, 0) for a in sys.argv[2:]]
    text = open(path, errors="replace").read()
    m = re.search(r'\.string "((?:[^"\\]|\\.)*)"', text, re.S)
    if not m:
        print("no .string blob", file=sys.stderr)
        return 1
    blob = unescape(m.group(1))
    print(f"blob {len(blob)} bytes (0x{len(blob):x})")
    offs = offs + [len(blob)]
    for k, off in enumerate(offs[:-1]):
        chunk = blob[off:offs[k + 1]]
        s = chunk.split(b"\x00")[0]
        pad = chunk[len(s):]
        ok = "ok" if pad.strip(b"\x00") == b"" else "!!"
        print(f"0x{off:04x} span={offs[k+1]-off:3d} pad={len(pad):2d} {ok} {s.decode('latin1')!r}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
