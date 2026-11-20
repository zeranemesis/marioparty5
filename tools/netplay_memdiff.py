#!/usr/bin/env python3
"""Compare the game memory two machines wrote at the same frame of a netplay session (MP5_NET_DUMPAT=<frame>,
src/port/netplay.cpp): MEM1 and the five HuMem heaps (host allocations on PC).

usage: netplay_memdiff.py <dir A> <dir B> <frame> [max regions]
  reads <dir>/netplay_{mem1,heap0..4}_<frame>_<role>_<base>.bin

The game's data is laid out identically on every machine; only pointers differ (each process maps its memory and
code elsewhere). An 8-byte word counts as equal when both point to the same offset of the same region, or both look
like host pointers outside those regions (code, host heap). Prints each differing region with its values as floats.
"""
import glob
import os
import re
import struct
import sys


def load(directory, frame):
    regions = {}
    for path in glob.glob(os.path.join(directory, f"netplay_*_{frame}_*_*.bin")):
        m = re.search(r"netplay_(mem1|heap\d)_\d+_\w+?_([0-9a-f]+)\.bin$", os.path.basename(path))
        if m:
            regions[m.group(1)] = (int(m.group(2), 16), open(path, "rb").read())
    return regions


def locate(regions, v):
    for name, (base, data) in regions.items():
        if base <= v < base + len(data):
            return name, v - base
    return None


def is_host_ptr(v):
    # Windows x64: code and heaps above 0x50_0000_0000; Android arm64: tagged heap pointers (0xb4...) and
    # mappings at 0x5x..0x7f_xxxx_xxxx. A float next to a small int (8 bytes) stays below these: still compared.
    return (v >> 56) == 0xB4 or (0x50_0000_0000 <= v < 0x8000_0000_0000)


def main():
    a, b = load(sys.argv[1], sys.argv[3]), load(sys.argv[2], sys.argv[3])
    limit = int(sys.argv[4]) if len(sys.argv) > 4 else 40
    for name in sorted(set(a) & set(b)):
        (base_a, da), (base_b, db) = a[name], b[name]
        size = min(len(da), len(db))
        # pointers sit on 4-byte boundaries in GameCube-shaped structs: try both 8-byte windows over a word
        def same_ptr(w):
            if w < 0 or w + 8 > size:
                return False
            va, vb = struct.unpack_from("<Q", da, w)[0], struct.unpack_from("<Q", db, w)[0]
            la, lb = locate(a, va), locate(b, vb)
            if la is not None and lb is not None and la[0] == lb[0]:
                return True  # into the same region: absolute alignments differ between hosts, so offsets may too
            # both pointer-shaped (into a region or the host heap/code): a pointer, whatever it points to
            return (la is not None or is_host_ptr(va)) and (lb is not None or is_host_ptr(vb))
        diffs = []
        for off in range(0, size - 3, 4):
            if da[off:off + 4] == db[off:off + 4]:
                continue
            if same_ptr(off) or same_ptr(off - 4):
                continue
            diffs.append(off)
        print(f"=== {name}: {len(diffs)} differing words (size {size:#x})")
        groups = []
        for off in diffs:
            if groups and off - groups[-1][1] <= 32:
                groups[-1][1] = off
            else:
                groups.append([off, off])
        for start, end in groups[:limit]:
            print(f"--- {name}+{start:#x} .. {end + 8:#x}")
            for off in range(max(0, (start & ~0xF) - 16), min(end + 24, size), 4):
                ia, ib = struct.unpack_from("<I", da, off)[0], struct.unpack_from("<I", db, off)[0]
                fa, fb = struct.unpack_from("<f", da, off)[0], struct.unpack_from("<f", db, off)[0]
                print(f"{'!!' if ia != ib else '  '} +{off:#09x}  {ia:08x} {ib:08x}   {fa:>14.7g} {fb:>14.7g}")


if __name__ == "__main__":
    main()
