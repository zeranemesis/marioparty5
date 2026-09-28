import json, re, subprocess, sys

CWD = r"C:\Users\BEACALCADM\marioparty5-jean"
def iv(x):
    try: return int(x)
    except Exception: return None
def rd_count(sym):
    n = 0
    for it in sym.get("instructions", []):
        ad = it.get("arg_diff")
        if ad and any(isinstance(x, dict) and x for x in ad):
            n += 1
    return n

for name in sys.argv[1:]:
    out = f"scratch_{re.sub(r'[^A-Za-z0-9]+','_',name)}_sf.diff"
    subprocess.run([r"build\tools\objdiff-cli.exe","diff","-u",name,"-o",out],
                   cwd=CWD, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    j = json.load(open(out, encoding="utf-8", errors="replace"))
    rows = []
    for s in j.get("right",{}).get("sections",[]):
        for sy in s.get("symbols",[]):
            mp = sy.get("match_percent")
            if mp is None or mp >= 100.0: continue
            nm = sy.get("symbol",{}).get("name","?")
            sz = iv(sy.get("symbol",{}).get("size")) or 0
            rows.append((rd_count(sy), mp, sz, nm))
    rows.sort()
    print(f"\n=== {name} ===")
    for rd, mp, sz, nm in rows:
        if rd <= 6:
            print(f"   rd={rd:3d}  mp={mp:6.2f}  size={sz:5d}  {nm}")
