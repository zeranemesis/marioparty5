import json, re, subprocess, sys
CWD = r\"G:\\decomp\\MP5\"
UNIT = "w04sDll/REL/w04sDll/world04"
def run_diff(fn):
    out = f"tools/diff_{fn}.json"
    subprocess.run([r"build\tools\objdiff-cli.exe","diff",fn,"-u",UNIT,"-o",out],
                   cwd=CWD, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    return json.load(open(out, encoding="utf-8", errors="replace"))
def find(secs, name):
    for s in secs:
        for sy in s.get("symbols", []):
            if sy.get("symbol",{}).get("name")==name: return sy
    return None
def info(it):
    ins=it.get("instruction",{}); rel=ins.get("relocation") or {}; tgt=rel.get("target") or {}
    return (ins.get("formatted"), rel.get("type_name"), tgt.get("symbol",{}).get("name"), ins.get("line_number"))
def is_artifact(a,b):
    lf,lt,lsn,ll = info(a); rf,rt,rsn,rl = info(b)
    if lf!=rf: return False
    if lt==rt:
        ln=re.match(r"^@\d+$",lsn or ""); rn=re.match(r"^@\d+$",rsn or "")
        if (ln and not rn) or (rn and not ln): return True
    return False
def show(fn):
    j=run_diff(fn)
    lsy=find(j["left"]["sections"],fn); rsy=find(j["right"]["sections"],fn)
    li=lsy.get("instructions",[]); ri=rsy.get("instructions",[])
    real=[]
    for k in range(max(len(li),len(ri))):
        a=li[k].get("arg_diff") if k<len(li) else None
        b=ri[k].get("arg_diff") if k<len(ri) else None
        if not ((a and any(isinstance(x,dict) and x for x in a)) or (b and any(isinstance(x,dict) and x for x in b))): continue
        if is_artifact(li[k],ri[k]): continue
        real.append(k)
    print(f"### {fn}: L={len(li)} R={len(ri)}  real_diffs={len(real)} {real[:40]}")
    for k in real[:40]:
        la=li[k].get("instruction",{}).get("formatted") if k<len(li) else "?"
        ra=ri[k].get("instruction",{}).get("formatted") if k<len(ri) else "?"
        ll=li[k].get("instruction",{}).get("line_number") if k<len(li) else "?"
        rl=ri[k].get("instruction",{}).get("line_number") if k<len(ri) else "?"
        print(f"  [{k}] L:{la} (ln{ll}) | R:{ra} (ln{rl})")
if __name__=="__main__":
    fns=sys.argv[1:] or ["MB4Ev_MasuHatena","MB4_SakanaClose","MB4_CoinInit","MB4_SakanaCreate","MB4_CoinDispUpdate"]
    for f in fns: show(f)
