import json, re, subprocess
CWD = r"C:\Users\BEACALCADM\marioparty5-jean"
def find(secs, name):
    for s in secs:
        for sy in s.get("symbols", []):
            if sy.get("symbol",{}).get("name")==name: return sy
    return None
unit, fn = "main/game/board/capmain", "MBCapsuleKettouExec"
out = f"scratch_{re.sub(r'[^A-Za-z0-9]+','_',unit)}_cf.diff"
subprocess.run([r"build\tools\objdiff-cli.exe","diff","-u",unit,"-o",out], cwd=CWD,
               stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
j = json.load(open(out, encoding="utf-8", errors="replace"))
lsy = find(j["left"]["sections"], fn); rsy = find(j["right"]["sections"], fn)
li = lsy.get("instructions",[]); ri = rsy.get("instructions",[])
def is_artifact(a,b):
    def info(it):
        ins=it.get("instruction",{}); rel=ins.get("relocation") or {}; tgt=rel.get("target") or {}
        return (ins.get("formatted"), rel.get("type_name"), tgt.get("symbol",{}).get("name"))
    lf,lt,lsn = info(a); rf,rt,rsn = info(b)
    if lf!=rf: return False
    if lt==rt:
        ln=re.match(r"^@\d+$",lsn or ""); rn=re.match(r"^@\d+$",rsn or "")
        if (ln and not rn) or (rn and not ln): return True
    return False
real=[]
for k in range(max(len(li),len(ri))):
    a=li[k].get("arg_diff") if k<len(li) else None
    b=ri[k].get("arg_diff") if k<len(ri) else None
    if not ((a and any(isinstance(x,dict) and x for x in a)) or (b and any(isinstance(x,dict) and x for x in b))): continue
    if is_artifact(li[k],ri[k]): continue
    real.append(k)
n=len(real)
print(f"total instrs: L={len(li)} R={len(ri)}  real diffs={n}")
# histogram in 100-instr buckets
import collections
buckets=collections.Counter(k//100 for k in real)
maxb=max(buckets) if buckets else 0
print("distribution (bucket of 100 instrs):")
for b in range(maxb+1):
    c=buckets.get(b,0)
    print(f"  [{b*100:5d}-{b*100+99:5d}] {'#'*c} {c}")
# print first 30 real diffs with line numbers
print("\nfirst 30 real diffs:")
for k in real[:30]:
    la=li[k].get("instruction",{}).get("formatted"); ra=ri[k].get("instruction",{}).get("formatted")
    ll=li[k].get("instruction",{}).get("line_number"); rl=ri[k].get("instruction",{}).get("line_number")
    print(f"  [{k}] L:{la} (ln{ll}) | R:{ra} (ln{rl})")
