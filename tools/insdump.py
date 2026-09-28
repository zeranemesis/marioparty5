import json, sys
# insdump.py <difffile> <symbol> [idx]
out = sys.argv[1]
sym = sys.argv[2]
want_idx = int(sys.argv[3]) if len(sys.argv) > 3 else None
j = json.load(open(out, encoding="utf-8", errors="replace"))
L = j["left"]["sections"]; R = j["right"]["sections"]
def find(side_secs):
    for s in side_secs:
        for sy in s.get("symbols", []):
            if sy.get("symbol",{}).get("name") == sym:
                return sy
    return None
lsy = find(L); rsy = find(R)
li = lsy.get("instructions", []); ri = rsy.get("instructions", [])
print(f"L n={len(li)}  R n={len(ri)}")
if want_idx is not None:
    for side, arr in (("L",li),("R",ri)):
        it = arr[want_idx]
        ins = it.get("instruction", {})
        print(f"{side}[{want_idx}]: {ins.get('formatted')}  line={ins.get('line_number')}")
        print(f"   arg_diff={json.dumps(it.get('arg_diff'))}")
        rel = ins.get("relocation")
        if rel:
            tgt = rel.get("target",{})
            print(f"   reloc={rel.get('type_name')} sym={tgt.get('symbol',{}).get('name')} addend={tgt.get('addend')}")
else:
    # print all mismatched
    for k in range(max(len(li),len(ri))):
        a = li[k].get("arg_diff") if k < len(li) else None
        b = ri[k].get("arg_diff") if k < len(ri) else None
        if (a and any(isinstance(x,dict) and x for x in a)) or (b and any(isinstance(x,dict) and x for x in b)):
            la = li[k].get("instruction",{}).get("formatted") if k<len(li) else "?"
            ra = ri[k].get("instruction",{}).get("formatted") if k<len(ri) else "?"
            ll = li[k].get("instruction",{}).get("line_number") if k<len(li) else "?"
            print(f"[{k}] L:{la} (ln {ll})   R:{ra}")
            for side, arr in (("L",li),("R",ri)):
                it = arr[k]
                ins = it.get("instruction",{})
                rel = ins.get("relocation")
                if rel and rel.get("target"):
                    print(f"   {side} reloc={rel.get('type_name')} sym={rel['target'].get('symbol',{}).get('name')} addend={rel['target'].get('addend')}")
