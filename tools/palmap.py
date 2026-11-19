#!/usr/bin/env python3
"""palmap.py: US -> PAL entry maps of the data/*.bin archives whose layout changed in GP5P01.

The port runs the US code, which reads archive entries by their US index. For each archive that differs on the
PAL disc this finds, for every US entry, the PAL entry holding the same thing:
  1. byte-identical entries (unique match),
  2. data numbers the US and PAL code load at the same place (lis/addi pairs in the changed functions),
  3. otherwise the same index when the decompressed size and type still agree.

    python tools/palmap.py ISO               # report coverage
    python tools/palmap.py ISO --emit FILE   # write the C table (src/port/pal_datamap.inc)
"""
import difflib
import os
import re
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import paldiff  # noqa: E402

ROOT = paldiff.ROOT


def dir_names(dol):
    d = open(dol, "rb").read()
    return [m.group().decode()[5:-4] for m in re.finditer(rb"data/[A-Za-z0-9_]+\.bin", d)]


def iso_files(iso):
    f = open(iso, "rb")

    def rd(off, n):
        f.seek(off)
        return f.read(n)
    _, fst_off, fst_size = struct.unpack(">III", rd(0x420, 12))
    fst = rd(fst_off, fst_size)
    count = struct.unpack(">I", fst[8:12])[0]
    names = fst[count * 12:]
    out = {}

    def walk(i, end, path):
        while i < end:
            flags = fst[i * 12]
            o = struct.unpack(">I", fst[i * 12:i * 12 + 4])[0] & 0xFFFFFF
            n = names[o:names.index(b"\0", o)].decode()
            a, b = struct.unpack(">II", fst[i * 12 + 4:i * 12 + 12])
            if flags:
                walk(i + 1, b, path + n + "/")
                i = b
            else:
                out[path + n] = (a, b)
                i += 1
    walk(1, count, "")
    return lambda p: rd(*out[p]) if p in out else None


def entries(b):
    n = struct.unpack(">I", b[:4])[0]
    offs = list(struct.unpack(">%dI" % n, b[4:4 + 4 * n])) + [len(b)]
    return [b[offs[i]:offs[i + 1]] for i in range(n)]


def code_pairs(us_dirs, pal_dirs):
    """(us dir name, us idx) -> (pal dir name, pal idx) from data numbers loaded at matching places"""
    pairs = {}
    lisaddi = re.compile(r"lis\s+(r\d+),(-?\d+)\s*\|\s*(?:addi|ori)\s+\1,\1,(-?\d+)$")

    def num(m, dirs):
        v = (((int(m.group(2)) & 0xFFFF) << 16) + int(m.group(3))) & 0xFFFFFFFF
        d, i = v >> 16, v & 0xFFFF
        return (dirs[d], i) if d < len(dirs) and i < 0x400 else None
    import palfn
    for name, ut, uf, pt, pf, funcs, base in paldiff.modules(set()):
        um, pm = paldiff.mask_words(ut, uf), paldiff.mask_words(pt, pf)
        shift = 0
        for fname, addr, size in funcs:
            a = addr - base
            body = um[a:a + size]
            hit = pm.find(body, max(0, a + shift - 0x4000), a + shift + size + 0x4000)
            if hit >= 0:
                shift = hit - a
                continue
            us_l = [x.split("#")[0].strip() for x in palfn.disasm(ut[a:a + size], 0)]
            pal_l = [x.split("#")[0].strip() for x in palfn.disasm(pt[a + shift:a + shift + size + 0x100], 0)][:len(us_l) + 0x40]
            norm = lambda l: [re.sub(r"-?\d+", "#", x) for x in l]
            for tag, i1, i2, j1, j2 in difflib.SequenceMatcher(None, norm(us_l), norm(pal_l), autojunk=False).get_opcodes():
                if tag not in ("equal", "replace") or i2 - i1 != j2 - j1:
                    continue
                for k in range(i2 - i1 - 1):
                    u0, u1, p0, p1 = us_l[i1 + k], us_l[i1 + k + 1], pal_l[j1 + k], pal_l[j1 + k + 1]
                    mu, mp = lisaddi.match(u0 + " | " + u1), lisaddi.match(p0 + " | " + p1)
                    if not (mu and mp):
                        continue
                    u = num(mu, us_dirs)
                    v = num(mp, pal_dirs)
                    if u and v and u != v and (v[0] == u[0] or v[0].lower().startswith(u[0].lower() + "_")):
                        pairs.setdefault(u, v)
    return pairs


def main():
    iso = sys.argv[1]
    emit = sys.argv[sys.argv.index("--emit") + 1] if "--emit" in sys.argv else None
    us_dirs = dir_names(os.path.join(ROOT, "orig", "GP5E01_00", "sys", "main.dol"))
    pal_dirs = dir_names(os.path.join(ROOT, "orig", "GP5P01_00", "sys", "main.dol"))
    pal = iso_files(iso)
    pairs = code_pairs(us_dirs, pal_dirs)
    tables = []
    for d in us_dirs:
        us_b = open(os.path.join(ROOT, "orig", "GP5E01_00", "files", "data", d + ".bin"), "rb").read()
        pal_b = pal("data/%s.bin" % d)
        if pal_b is None or us_b == pal_b:
            continue
        ue = entries(us_b)
        # candidate PAL archives: the same name, then its English variant (capsule_en...)
        cands = [d] + [n for n in pal_dirs if n.lower() == d.lower() + "_en"]
        pes = {c: entries(pal("data/%s.bin" % c)) for c in cands}
        pe = pes[d]
        m = [None] * len(ue)
        how = [""] * len(ue)
        used = set()
        for c in cands:
            by_bytes = {}
            for j, e in enumerate(pes[c]):
                by_bytes.setdefault(e, []).append(j)
            for i, e in enumerate(ue):
                if m[i] is not None:
                    continue
                js = by_bytes.get(e, [])
                if len(js) == 1 or (js and c == d and i in js):
                    j = i if (c == d and i in js) else js[0]
                    m[i], how[i] = (c, j), "bytes"
                    used.add((c, j))
        for (dn, i), (pd, j) in pairs.items():
            if dn == d and i < len(m) and m[i] is None and pd in pes and j < len(pes[pd]):
                m[i], how[i] = (pd, j), "code"
                used.add((pd, j))
        for i in range(len(ue)):
            if m[i] is None and i < len(pe) and ue[i][:8] == pe[i][:8] and (d, i) not in used:
                m[i], how[i] = (d, i), "size"
                used.add((d, i))
        # a unique unused PAL entry with the same decompressed size and compression
        for i in range(len(ue)):
            if m[i] is not None:
                continue
            hits = [(c, j) for c in cands for j, e in enumerate(pes[c]) if e[:8] == ue[i][:8] and (c, j) not in used]
            if len(hits) == 1:
                m[i], how[i] = hits[0], "header"
                used.add(hits[0])
        same_layout = len(ue) == len(pe)
        if all(m[i] in (None, (d, i)) for i in range(len(ue))) and same_layout:
            continue
        tables.append((d, m, how, len(pe)))
        unresolved = [i for i in range(len(m)) if m[i] is None]
        moved = sum(1 for i in range(len(m)) if m[i] is not None and m[i] != (d, i))
        other = sum(1 for i in range(len(m)) if m[i] is not None and m[i][0] != d)
        print("%-14s us %3d pal %3d  moved %3d (other archive %2d)  unresolved %3d %s" % (
            d, len(ue), len(pe), moved, other, len(unresolved), unresolved[:14]))
    if emit:
        names = sorted({v[0] for _, m, _, _ in tables for v in m if v is not None} | {d for d, _, _, _ in tables})
        lines = ["/* Generated by tools/palmap.py: US -> PAL entries of the data archives whose layout changed on the",
                 " * GP5P01 disc, as {PAL archive (PalArchives index), PAL entry}. {-1, -1}: no counterpart found, the",
                 " * US index is kept. Do not edit. */",
                 "static const char *const PalArchives[] = {%s};" % ", ".join('"data/%s.bin"' % n for n in names)]
        for d, m, _, _ in tables:
            lines.append("static const PalDataEntry PalMap_%s[%d] = {%s};" % (d, len(m), ", ".join(
                "{-1, -1}" if v is None else "{%d, %d}" % (names.index(v[0]), v[1]) for v in m)))
        lines.append("static const PalDataMap PalDataMaps[] = {")
        lines += ['    {"data/%s.bin", PalMap_%s, %d},' % (d, d, len(m)) for d, m, _, _ in tables]
        lines.append("};")
        open(emit, "w", newline="").write("".join(l + chr(10) for l in lines))

if __name__ == "__main__":
    main()
