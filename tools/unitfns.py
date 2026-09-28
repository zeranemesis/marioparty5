import json, sys
j = json.load(open(r"build\GP5E01_00\report.json", encoding="utf-8", errors="replace"))
def iv(x):
    try: return int(x)
    except Exception: return 0
target = sys.argv[1]
u = [x for x in j["units"] if x.get("name","")==target][0]
fns = u.get("functions", [])
rows = []
for f in fns:
    size = iv(f.get("size", 0))
    mp = iv(f.get("fuzzy_match_percent", 0))
    if size > 0 and mp < 100:
        rows.append((size, f.get("name"), mp))
rows.sort(reverse=True)
print(f"{target}: {len(fns)} fns, {len(rows)} incomplete")
print(f"{'size':>8} {'mp%':>7}  fn")
for size, name, mp in rows[:60]:
    print(f"{size:8d} {mp:7.2f}  {name}")
print(f"\nincomplete sum size={sum(r[0] for r in rows)}")
