import json, sys
j = json.load(open(r"build\GP5E01_00\report.json", encoding="utf-8", errors="replace"))
def iv(x):
    try: return int(x)
    except Exception: return 0
rows = []
for u in j["units"]:
    m = u.get("measures", {})
    total = iv(m.get("total_code", 0))
    matched = iv(m.get("matched_code", 0))
    mp = iv(m.get("matched_code_percent", 0))
    rows.append((total, u.get("name",""), mp, matched))
# only incomplete units, biggest first
inc = [r for r in rows if r[3] < r[0]]
inc.sort(reverse=True)
topn = int(sys.argv[1]) if len(sys.argv) > 1 else 40
print(f"{'total':>8} {'matched':>8} {'mp%':>7}  unit  (incomplete, biggest first)")
for total, n, mp, matched in inc[:topn]:
    print(f"{total:8d} {matched:8d} {mp:7.2f}  {n}")
print(f"\nincomplete units: {len(inc)}  sum total={sum(r[0] for r in inc)} sum matched={sum(r[3] for r in inc)}")
