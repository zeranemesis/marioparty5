#!/usr/bin/env python3
# Full instruction listing for one function in a given unit (both sides).
# usage: python tools/_fndis.py <unit> <fn>
import json, subprocess, sys

fn = sys.argv[2]
unit = sys.argv[1]
cmd = [r"build\tools\objdiff-cli.exe", "diff", "-p", ".", "-u", unit,
       "-m", f"{fn}={fn}", "--format", "json", fn, "-o", "-"]
out = subprocess.run(cmd, capture_output=True, text=True).stdout
d = json.loads(out)

def text_sec(side):
    for sec in side["sections"]:
        if sec["name"] == ".text":
            return sec
    return None

L, R = d["left"], d["right"]
lt, rt = text_sec(L), text_sec(R)

def find_ins(sec, name):
    for s in sec["symbols"]:
        if s["symbol"]["name"] == name:
            return s["instructions"]
    return []

li = find_ins(lt, fn)
ri = find_ins(rt, fn)
def fmt(x):
    if x is None:
        return "            <none>"
    ins = x.get("instruction") or {}
    ra = x.get("relocation") or {}
    tname = (ra.get("target") or {}).get("symbol", {}).get("name")
    return f"{ins.get('formatted','')!r:<28} rel={tname}"

n = max(len(li), len(ri))
for i in range(n):
    a = li[i] if i < len(li) else None
    b = ri[i] if i < len(ri) else None
    print(f"[{i:3}] L {fmt(a)}")
    print(f"       R {fmt(b)}")
