#!/usr/bin/env python3
# Show only the differing instructions for one function in a given unit.
# usage: python tools/_fndiff2.py <unit> <fn>
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

def find_fn(sec, name):
    for i, s in enumerate(sec["symbols"]):
        if s["symbol"]["name"] == name:
            return i, s
    return None, None

li, _ = find_fn(lt, fn)
ri, _ = find_fn(rt, fn)
li_ins = lt["symbols"][li]["instructions"] if li is not None else []
ri_ins = rt["symbols"][ri]["instructions"] if ri is not None else []
print(f"left instrs: {len(li_ins)}  right instrs: {len(ri_ins)}")

def info(x):
    if x is None:
        return (None, None, None)
    ins = x.get("instruction") or {}
    ra = x.get("relocation") or {}
    tname = (ra.get("target") or {}).get("symbol", {}).get("name")
    return (ins.get("formatted"), tname, x.get("arg_diff"))

n = max(len(li_ins), len(ri_ins))
diffs = 0
for i in range(n):
    a = li_ins[i] if i < len(li_ins) else None
    b = ri_ins[i] if i < len(ri_ins) else None
    fa, ta, ad = info(a)
    fb, tb, bd = info(b)
    if (fa, ta, ad) != (fb, tb, bd):
        diffs += 1
        print(f"[{i}] L: {fa!r}  rel={ta}")
        print(f"     R: {fb!r}  rel={tb}")
        if ad != bd:
            print(f"     arg_diff L={ad} R={bd}")
print("total diffs:", diffs)
