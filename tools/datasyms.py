"""Diff the data-section symbol tables of a unit's target and base objects.

report.json tells you a unit's data is imperfect; this tells you whether the
cause is a mis-sized symbol, a missing symbol, or a mere padding tail. Pure
objdump, no objdiff, so it is fast enough to sweep every unit.
"""
import json
import subprocess
import sys

OBJDUMP = "D:/decomp/MP5/build/binutils/powerpc-eabi-objdump.exe"
OD = json.load(open("objdiff.json"))
REPORT = json.load(open("build/GP5E01_00/report.json"))
DATA = {".data", ".rodata", ".sdata", ".sdata2", ".sbss", ".bss"}


def syms(path):
    out = subprocess.run([OBJDUMP, "-t", path], capture_output=True, text=True).stdout
    table = {}
    for line in out.splitlines():
        f = line.split()
        # VALUE BIND TYPE SECTION SIZE NAME, e.g.
        # "00000060 g O .data 00000038 lbl_801D1AA8"
        if len(f) < 6 or f[1] not in ("l", "g") or f[3] not in DATA:
            continue
        if f[5] == f[3]:          # the section symbol itself, size 0
            continue
        try:
            size = int(f[4], 16)
        except ValueError:
            continue
        # objdump prints the ELF name plus any linker-appended alias, e.g.
        # ".hidden gap_07_801BF637_data" -- both matter, so keep them joined.
        table.setdefault(f[3], []).append((" ".join(f[5:]), size, f[1]))
    return table


def describe(path):
    """Section name -> size, via -h."""
    out = subprocess.run([OBJDUMP, "-h", path], capture_output=True, text=True).stdout
    sizes = {}
    lines = out.splitlines()
    for i, line in enumerate(lines):
        f = line.split()
        if len(f) > 2 and f[0].isdigit() and f[1] in DATA:
            sizes[f[1]] = int(f[2], 16)
    return sizes


def main():
    want = sys.argv[1] if len(sys.argv) > 1 else None
    paths = {u["name"]: u for u in OD["units"]}
    hits = 0
    for unit in REPORT["units"]:
        name = unit["name"]
        if want and want not in name:
            continue
        secs = {s["name"]: s for s in unit["sections"]}
        text = secs.get(".text", {}).get("fuzzy_match_percent")
        bad = {k: (v.get("fuzzy_match_percent") or 0) for k, v in secs.items()
               if k in DATA and (v.get("fuzzy_match_percent") or 0) < 100}
        if text != 100 or not bad:
            continue
        u = paths.get(name)
        if not u or "target_path" not in u or "base_path" not in u:
            continue
        try:
            ts, ms = syms(u["target_path"]), syms(u["base_path"])
            tsize, msize = describe(u["target_path"]), describe(u["base_path"])
        except Exception as e:
            print(f"{name}: objdump failed: {e}")
            continue
        print(f"\n=== {name}   data {bad}")
        for sec in sorted(set(ts) | set(ms)):
            if sec not in bad:
                continue        # objdiff already scores this 100: nothing to win
            t = {n: (sz, b) for n, sz, b in ts.get(sec, [])}
            m = {n: (sz, b) for n, sz, b in ms.get(sec, [])}
            delta = tsize.get(sec, 0) - msize.get(sec, 0)
            notes = []
            for n in sorted(set(t) | set(m)):
                if n not in m:
                    notes.append(f"+{n} (0x{t[n][0]:x}, {t[n][1]})")
                elif n not in t:
                    notes.append(f"-{n} (0x{m[n][0]:x}, {m[n][1]})")
                elif t[n] != m[n]:
                    notes.append(f"~{n} 0x{t[n][0]:x}/{t[n][1]} vs 0x{m[n][0]:x}/{m[n][1]}")
            if delta or notes:
                print(f"  {sec:<8} target 0x{tsize.get(sec, 0):<6x} mine 0x{msize.get(sec, 0):<6x} "
                      f"delta {delta:+#x}   {'  '.join(notes[:8])}")
        hits += 1
    print(f"\n{hits} units with matching .text and imperfect data")


main()
