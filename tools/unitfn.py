import json, sys
j = json.load(open(r"build\GP5E01_00\report.json", encoding="utf-8", errors="replace"))
target = sys.argv[1]
sub = sys.argv[2] if len(sys.argv) > 2 else None
u = [x for x in j["units"] if x.get("name") == target][0]
m = u["measures"]
print(f"{target}: {m.get('matched_code_percent')}%  {m.get('matched_code')}/{m.get('total_code')}")
for f in u["functions"]:
    if sub is None or sub in f.get("name", ""):
        print(f"  {f.get('name'):>24} size={f.get('size')} mp={f.get('fuzzy_match_percent')}")
