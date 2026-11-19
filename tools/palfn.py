#!/usr/bin/env python3
"""palfn.py MODULE FUNC [MAXLINES]: side-by-side disassembly of a US function and its PAL counterpart.

The PAL copy is located from the nearest unchanged neighbour (see paldiff.py); only differing lines are shown,
with a few lines of context.
"""
import difflib
import os
import subprocess
import sys
import tempfile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import paldiff  # noqa: E402

OBJDUMP = os.environ.get("PPC_OBJDUMP", "C:/SysGCC/powerpc-eabi/bin/powerpc-eabi-objdump")


def disasm(code, vma):
    with tempfile.NamedTemporaryFile(delete=False, suffix=".bin") as f:
        f.write(code)
    try:
        out = subprocess.check_output([OBJDUMP, "-D", "-b", "binary", "-m", "powerpc:750", "-EB",
                                       "--adjust-vma=0x%x" % vma, f.name], text=True)
    finally:
        os.unlink(f.name)
    lines = []
    for line in out.splitlines():
        parts = line.split("\t")
        if len(parts) >= 3 and parts[0].strip().endswith(":"):
            lines.append(" ".join(p.strip() for p in parts[2:]))
    return lines


def locate(mod, func):
    for name, ut, uf, pt, pf, funcs, base in paldiff.modules({mod}):
        um, pm = paldiff.mask_words(ut, uf), paldiff.mask_words(pt, pf)
        shift, target = 0, None
        for fname, addr, size in funcs:
            a = addr - base
            if fname == func:
                target = (a, size)
                break
            hit = pm.find(um[a:a + size], max(0, a + shift - 0x4000), a + shift + size + 0x4000)
            if hit >= 0:
                shift = hit - a
        if target is None:
            sys.exit("no function %s in %s" % (func, mod))
        a, size = target
        # the PAL body ends where the next shared function starts: take a generous window and let the diff trim it
        return ut[a:a + size], pt[a + shift:a + shift + size + 0x200], a + base, a + shift + base
    sys.exit("no module " + mod)


def main():
    mod, func = sys.argv[1], sys.argv[2]
    us, pal, ua, pa = locate(mod, func)
    ul, pl = disasm(us, ua), disasm(pal, pa)
    # cut the PAL window at its function end: the first blr after the US length minus slack
    end = len(pl)
    for i in range(max(0, len(ul) - 40), len(pl)):
        if pl[i].startswith("blr"):
            end = i + 1
            if i + 1 >= len(ul) - 2:
                break
    pl = pl[:end]
    print("US 0x%x (%d insns)  PAL 0x%x (%d insns)" % (ua, len(ul), pa, len(pl)))
    strip = lambda l: [x.split("#")[0].strip() for x in l]
    for line in difflib.unified_diff(strip(ul), strip(pl), "US", "PAL", n=2, lineterm=""):
        print(line)


if __name__ == "__main__":
    main()
