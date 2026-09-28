import json, re, subprocess, sys
CWD = r"C:\Users\BEACALCADM\marioparty5-jean"
def iv(x):
    try: return int(x)
    except Exception: return None
def find(secs, name):
    for s in secs:
        for sy in s.get("symbols", []):
            if sy.get("symbol",{}).get("name")==name: return sy
    return None
def insinfo(it):
    ins = it.get("instruction",{})
    rel = ins.get("relocation") or {}
    tgt = rel.get("target") or {}
    return (ins.get("formatted"), rel.get("type_name"), tgt.get("symbol",{}).get("name"), tgt.get("addend"))
def is_artifact(li_k, ri_k):
    lf, lt, lsn, la = insinfo(li_k)
    rf, rt, rsn, ra = insinfo(ri_k)
    if lf != rf: return False
    # same formatted; artifact if same reloc type and one side has a numeric auto-label (@NNN)
    if lt == rt:
        lnum = re.match(r"^@\d+$", lsn or "")
        rnum = re.match(r"^@\d+$", rsn or "")
        if (lnum and not rnum) or (rnum and not lnum):
            return True
    return False
def classify(unit, fn):
    out = f"scratch_{re.sub(r'[^A-Za-z0-9]+','_',unit)}_cf.diff"
    subprocess.run([r"build\tools\objdiff-cli.exe","diff","-u",unit,"-o",out],
                   cwd=CWD, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    j = json.load(open(out, encoding="utf-8", errors="replace"))
    lsy = find(j["left"]["sections"], fn); rsy = find(j["right"]["sections"], fn)
    li = lsy.get("instructions",[]); ri = rsy.get("instructions",[])
    real=[]; phantom=0
    for k in range(max(len(li),len(ri))):
        a = li[k].get("arg_diff") if k<len(li) else None
        b = ri[k].get("arg_diff") if k<len(ri) else None
        if not ((a and any(isinstance(x,dict) and x for x in a)) or (b and any(isinstance(x,dict) and x for x in b))):
            continue
        if is_artifact(li[k], ri[k]):
            phantom+=1
        else:
            real.append(f"[{k}] L:{insinfo(li[k])[0]}({insinfo(li[k])[2]}) | R:{insinfo(ri[k])[0]}({insinfo(ri[k])[2]})")
    return real, phantom

groups = {
  "main/game/board/capmain": ["MBCapsuleKettouExec"],
}
print(f"{'fn':<20} {'REAL':>4} {'PHANT':>5}   real diffs")
for unit, fns in groups.items():
    for f in fns:
        real, phantom = classify(unit, f)
        print(f"{f:<20} {len(real):4d} {phantom:5d}   {unit.split('/')[-1]}")
        for r in real[:4]:
            print(f"      {r}")
