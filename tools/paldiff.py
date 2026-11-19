#!/usr/bin/env python3
"""paldiff.py: which US (GP5E01) functions changed in the PAL release (GP5P01)?

For main.dol and every REL, each function of config/GP5E01_00 is looked up in the PAL binary with its address
fields masked (REL relocations exactly, DOL immediates heuristically). A function found byte-identical is shared
code; one not found changed in PAL (or moved past a changed neighbour: see the 'near' column).

    python tools/paldiff.py [module ...]        # summary per module, then the changed functions
    python tools/paldiff.py --json out.json      # machine-readable result
"""
import json
import os
import re
import struct
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
US, PAL = os.path.join(ROOT, "orig", "GP5E01_00"), os.path.join(ROOT, "orig", "GP5P01_00")
SYM = re.compile(r"^(\S+) = \.text:0x([0-9A-Fa-f]+); // type:function size:0x([0-9A-Fa-f]+)")


def functions(symbols_path):
    out = []
    for line in open(symbols_path, encoding="utf-8"):
        m = SYM.match(line)
        if m and int(m.group(3), 16) >= 4:
            out.append((m.group(1), int(m.group(2), 16), int(m.group(3), 16)))
    return out


def mask_words(text, fields):
    """fields: {word offset: mask of the bits to clear}"""
    b = bytearray(text)
    for off, m in fields.items():
        if off + 4 <= len(b):
            w = struct.unpack_from(">I", b, off)[0] & ~m & 0xFFFFFFFF
            struct.pack_into(">I", b, off, w)
    return bytes(b)


def heuristic_fields(text):
    """DOL: no relocations, so clear every branch target and D-form immediate (addresses and SDA offsets)"""
    fields = {}
    for off in range(0, len(text) - 3, 4):
        op = text[off] >> 2
        if op == 18:  # b/bl
            fields[off] = 0x03FFFFFC
        elif op in (14, 15, 24, 25) or 32 <= op <= 55:  # addi addis ori oris, loads/stores
            fields[off] = 0xFFFF
    return fields


def rel_text(path):
    """(text bytes, masked fields) of a REL's executable section 1, from its own relocation table"""
    d = open(path, "rb").read()
    nsec, secoff = struct.unpack_from(">II", d, 0x0C)
    sections = [struct.unpack_from(">II", d, secoff + i * 8) for i in range(nsec)]
    toff, tsize = sections[1]
    text = d[toff & ~3:(toff & ~3) + tsize]
    imp_off, imp_size = struct.unpack_from(">II", d, 0x28)
    fields = {}
    for i in range(imp_size // 8):
        _, roff = struct.unpack_from(">II", d, imp_off + i * 8)
        pos, sec = 0, -1
        while True:
            off, typ, s, _ = struct.unpack_from(">HBBI", d, roff)
            roff += 8
            if typ == 203:  # R_DOLPHIN_END
                break
            pos += off
            if typ == 202:  # R_DOLPHIN_SECTION
                pos, sec = 0, s
                continue
            if typ == 201 or sec != 1:  # R_DOLPHIN_NOP / not .text
                continue
            if typ in (4, 5, 6):  # ADDR16_LO/HI/HA: the halfword at pos
                fields[pos & ~3] = fields.get(pos & ~3, 0) | 0xFFFF
            elif typ == 10:  # REL24
                fields[pos] = fields.get(pos, 0) | 0x03FFFFFC
            elif typ == 1:  # ADDR32
                fields[pos] = 0xFFFFFFFF
    # intra-section branches are resolved by makerel without a relocation: clear them as well
    for off in range(0, len(text) - 3, 4):
        if text[off] >> 2 == 18:
            fields[off] = fields.get(off, 0) | 0x03FFFFFC
    return text, fields


def dol_text(path):
    d = open(path, "rb").read()
    offs = struct.unpack_from(">18I", d, 0)
    addrs = struct.unpack_from(">18I", d, 0x48)
    sizes = struct.unpack_from(">18I", d, 0x90)
    # every text section, laid out at its load address relative to the first one
    secs = [(addrs[i], d[offs[i]:offs[i] + sizes[i]]) for i in range(7) if sizes[i]]
    base = min(a for a, _ in secs)
    buf = bytearray(max(a + len(t) for a, t in secs) - base)
    for a, t in secs:
        buf[a - base:a - base + len(t)] = t
    return bytes(buf), base


def compare(name, us_text, us_fields, pal_text, pal_fields, funcs, base=0):
    us_m, pal_m = mask_words(us_text, us_fields), mask_words(pal_text, pal_fields)
    changed, shift, cursor = [], 0, 0
    for fname, addr, size in funcs:
        a = addr - base
        body = us_m[a:a + size]
        if len(body) != size:
            continue
        # look near the expected place first (functions keep their order), then anywhere
        guess = max(0, a + shift - 0x4000)
        hit = pal_m.find(body, guess, a + shift + size + 0x4000)
        if hit < 0:
            hit = pal_m.find(body)
        if hit < 0:
            changed.append({"name": fname, "us": hex(addr), "size": hex(size)})
        else:
            shift = hit - a
    return {"module": name, "functions": len(funcs), "changed": changed}


def modules(filt):
    if not filt or "main" in filt:
        us, base = dol_text(os.path.join(US, "sys", "main.dol"))
        pal, pbase = dol_text(os.path.join(PAL, "sys", "main.dol"))
        assert base == pbase
        yield "main", us, heuristic_fields(us), pal, heuristic_fields(pal), \
            functions(os.path.join(ROOT, "config", "GP5E01_00", "symbols.txt")), base
    rels = os.path.join(ROOT, "config", "GP5E01_00", "rels")
    for mod in sorted(os.listdir(rels)):
        if filt and mod not in filt:
            continue
        us_rel = os.path.join(US, "files", "dll", mod + ".rel")
        pal_rel = os.path.join(PAL, "files", "dll", mod + ".rel")
        if not (os.path.exists(us_rel) and os.path.exists(pal_rel)):
            continue
        ut, uf = rel_text(us_rel)
        pt, pf = rel_text(pal_rel)
        yield mod, ut, uf, pt, pf, functions(os.path.join(rels, mod, "symbols.txt")), 0


def main():
    args = sys.argv[1:]
    out_json = None
    if "--json" in args:
        i = args.index("--json")
        out_json = args[i + 1]
        del args[i:i + 2]
    results = [compare(*m) for m in modules(set(args))]
    results.sort(key=lambda r: -len(r["changed"]))
    for r in results:
        if r["changed"]:
            print("%-18s %4d / %4d changed" % (r["module"], len(r["changed"]), r["functions"]))
    same = [r["module"] for r in results if not r["changed"]]
    print("identical modules (%d): %s" % (len(same), " ".join(sorted(same))))
    print("total changed functions:", sum(len(r["changed"]) for r in results))
    if out_json:
        json.dump(results, open(out_json, "w"), indent=1)


if __name__ == "__main__":
    main()
