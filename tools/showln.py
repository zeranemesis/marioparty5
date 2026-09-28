import sys
fn = sys.argv[1]
ranges = sys.argv[2:]
lines = open(fn, encoding="cp932", errors="replace").readlines()
for r in ranges:
    a, b = [int(x) for x in r.split("-")]
    print(f"--- {fn} lines {a}-{b} ---")
    for i in range(a - 1, min(b, len(lines))):
        print(f"{i+1}: {lines[i].rstrip()}")
