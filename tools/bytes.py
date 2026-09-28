import json, re, subprocess
CWD = r"C:\Users\BEACALCADM\marioparty5-jean"
def find(secs, name):
    for s in secs:
        for sy in s.get("symbols", []):
            if sy.get("symbol",{}).get("name")==name: return sy
    return None
unit, fn = "main/game/saveload", "SLFileWrite"
out = f"scratch_{re.sub(r'[^A-Za-z0-9]+','_',unit)}_cf.diff"
subprocess.run([r"build\tools\objdiff-cli.exe","diff","-u",unit,"-o",out], cwd=CWD,
               stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
j = json.load(open(out, encoding="utf-8", errors="replace"))
lsy = find(j["left"]["sections"], fn); rsy = find(j["right"]["sections"], fn)
li = lsy.get("instructions",[]); ri = rsy.get("instructions",[])
def raw(it):
    o = it.get("instruction",{}).get("original","")
    # original is like "lfs f1, 0x0(r0)" — strip the (sym)
    return o
# print around the diff indices 25,26
for k in (24,25,26,27):
    print(f"[{k}] L:{raw(li[k])!r}  R:{raw(ri[k])!r}")
    # also show raw bytes if present
    lb = li[k].get("instruction",{}).get("raw_bytes")
    rb = ri[k].get("instruction",{}).get("raw_bytes")
    print(f"     L bytes={lb}  R bytes={rb}")
