import json, sys
out = sys.argv[1]
name = sys.argv[2]
j = json.load(open(out, encoding="utf-8", errors="replace"))
for side in ("left","right"):
    for s in j[side]["sections"]:
        for sy in s.get("symbols", []):
            if sy.get("symbol",{}).get("name") == name:
                print(f"{side}: {json.dumps(sy['symbol'])}")
