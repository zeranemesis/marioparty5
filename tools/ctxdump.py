import json, sys
out = sys.argv[1]
sym = sys.argv[2]
c = int(sys.argv[3]) if len(sys.argv) > 3 else 4
n0 = int(sys.argv[4]) if len(sys.argv) > 4 else 0
j = json.load(open(out, encoding="utf-8", errors="replace"))
def find(secs):
    for s in secs:
        for sy in s.get("symbols", []):
            if sy.get("symbol",{}).get("name") == sym:
                return sy
    return None
lsy = find(j["left"]["sections"]); rsy = find(j["right"]["sections"])
li = lsy.get("instructions", []); ri = rsy.get("instructions", [])
def fmt(side, k):
    it = side[k]; ins = it.get("instruction", {})
    rel = ins.get("relocation")
    extra = ""
    if rel and rel.get("target"):
        extra = f" -> {rel['target'].get('symbol',{}).get('name')}"
    ln = ins.get("line_number")
    return f"{ins.get('formatted')}{extra}  (ln {ln})"
for k in range(n0, n0+2*c+1):
    L = fmt(li,k) if k < len(li) else "?"
    R = fmt(ri,k) if k < len(ri) else "?"
    mark = " " if L==R else "*"
    print(f"{mark}[{k}] L: {L}")
    print(f"      R: {R}")
