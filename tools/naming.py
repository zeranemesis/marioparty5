"""Find data symbols the retail build named and ours left anonymous.

colman was fixed by hoisting two anonymous OSReport literals onto sized
globals: the bytes were already right, only the name and the bound were
not. This sweeps every unit for that shape -- a target symbol called
lbl_* that our build calls @<offset> -- and ranks by how much padding
the name is hiding, so the cheapest fixes come first.

Unlike datasyms.py this does not require .text to be clean: the naming
mismatch is worth fixing whatever the code around it does.
"""
import json
import subprocess
import re
from concurrent.futures import ThreadPoolExecutor

OBJDUMP = "D:/decomp/MP5/build/binutils/powerpc-eabi-objdump.exe"
DATA = {".data", ".rodata", ".sdata", ".sdata2"}
LINE = re.compile(r"^([0-9a-f]+)\s+([lg])\s+\S+\s+(\S+)\s+([0-9a-f]+)\s+(.*)$")


def symbols(path):
    """section -> {offset: (name, size, bind)}"""
    out = subprocess.run([OBJDUMP, "-t", path], capture_output=True, text=True).stdout
    table = {}
    for line in out.splitlines():
        m = LINE.match(line)
        if not m:
            continue
        addr, bind, sec, size, name = m.groups()
        if sec not in DATA:
            continue
        name = name.strip()
        if name == sec:               # the section symbol itself, size 0
            continue
        table.setdefault(sec, {})[int(addr, 16)] = (name, int(size, 16), bind)
    return table


def scan(unit):
    t, b = unit["target_path"], unit["base_path"]
    try:
        ts, ms = symbols(t), symbols(b)
    except Exception:
        return None
    hits = []
    for sec in sorted(set(ts) & set(ms)):
        for off, (tname, tsize, tbind) in sorted(ts[sec].items()):
            if off not in ms[sec]:     # keyed by offset within the section
                continue
            mname, msize, mbind = ms[sec][off]
            if not tname.startswith("lbl_") or not mname.startswith("@"):
                continue
            if tbind == "l" and mbind == "l":
                pass                      # both local: still a rename candidate
            if tsize == msize and tbind == mbind:
                hits.append((0, sec, tname, mname, tsize, "rename only"))
            else:
                hits.append((tsize - msize, sec, tname, mname, tsize,
                             f"size {msize:#x} -> {tsize:#x}"))
    return hits


def main():
    units = [u for u in json.load(open("objdiff.json"))["units"]
             if "target_path" in u and "base_path" in u]
    rows = []
    with ThreadPoolExecutor(max_workers=12) as pool:
        for unit, hits in zip(units, pool.map(scan, units)):
            for delta, sec, tname, mname, tsize, note in hits or []:
                rows.append((delta, unit["name"], sec, tname, mname, note))
    rows.sort()
    print(f"{len(rows)} target lbl_* symbols that we build as anonymous literals\n")
    for delta, name, sec, tname, mname, note in rows[:40]:
        print(f"  {name:<34} {sec:<9} {tname:<14} (mine {mname:<8}) {note}")
    print("\nunits involved:", len({r[1] for r in rows}))


main()
