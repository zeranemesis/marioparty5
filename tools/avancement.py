#!/usr/bin/env python3

###
# Writes docs/avancement.md from build/<version>/report.json and the unit
# statuses declared in configure.py, so the progress page is always measured,
# never remembered.
#
# Usage:
#   ninja                         (or: ninja build/GP5E01_00/report.json)
#   python tools/avancement.py
###

import argparse
import datetime
import json
import re
import sys
from collections import defaultdict
from pathlib import Path

root = Path(__file__).resolve().parent.parent

FAMILIES = [
    (r"^main$", "DOL (moteur et bibliothèques)"),
    (r"^m5\d\d", "Mini-jeux m5xx"),
    (r"^m4\d\d", "Mini-jeux repris de MP4 (m4xx)"),
    (r"^w0\ds", "Plateaux du mode Histoire (w0Xs)"),
    (r"^w", "Plateaux (w0X, w10, w20)"),
    (r"^(md|mg)", "Modes (md*, mg*)"),
    (r".*", "Système, menus, résultats, histoire, Super Duel, E3"),
]


def family(module: str) -> str:
    for pattern, name in FAMILIES:
        if re.match(pattern, module):
            return name
    return FAMILIES[-1][1]


def statuses() -> dict:
    text = (root / "configure.py").read_text(encoding="utf-8")
    result = defaultdict(lambda: {"Matching": 0, "Equivalent": 0, "NonMatching": 0})
    for m in re.finditer(r'Object\(\s*(Matching|NonMatching|Equivalent)\s*,\s*"([^"]+)"', text):
        path = m.group(2)
        module = path.split("/")[1] if path.startswith("REL/") and path.count("/") >= 2 else "main"
        result[module][m.group(1)] += 1
    return result


def kb(value: int) -> str:
    return f"{value / 1024:,.1f}".replace(",", " ")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--version", default="GP5E01_00")
    parser.add_argument("--output", default=str(root / "docs" / "avancement.md"))
    args = parser.parse_args()

    report = json.loads((root / "build" / args.version / "report.json").read_text(encoding="utf-8"))
    per_module = defaultdict(lambda: {"code": 0, "matched": 0, "fns": 0, "mfns": 0, "fuzzy_weighted": 0.0})
    for unit in report["units"]:
        module = unit["name"].split("/")[0]
        m = unit.get("measures", {})
        code = int(m.get("total_code", 0))
        entry = per_module[module]
        entry["code"] += code
        entry["matched"] += int(m.get("matched_code", 0))
        entry["fns"] += int(m.get("total_functions", 0))
        entry["mfns"] += int(m.get("matched_functions", 0))
        entry["fuzzy_weighted"] += code * float(m.get("fuzzy_match_percent", 0.0))

    status = statuses()
    measures = report["measures"]
    lines = []
    now = datetime.datetime.now().strftime("%Y-%m-%d %H:%M")
    lines += [
        "# Avancement de la décompilation",
        "",
        f"Généré le {now} par `tools/avancement.py` depuis `build/{args.version}/report.json`.",
        "Ne pas éditer à la main : relancer `ninja` puis le script.",
        "",
        "| mesure | valeur |",
        "|---|---|",
        f"| code identique | **{float(measures.get('matched_code_percent', 0)):.2f} %** "
        f"({kb(int(measures.get('matched_code', 0)))} / {kb(int(measures.get('total_code', 0)))} Ko) |",
        f"| code lié depuis les sources | {float(measures.get('complete_code_percent', 0)):.2f} % |",
        f"| fonctions identiques | {measures.get('matched_functions', 0)} / {measures.get('total_functions', 0)} |",
        f"| unités complètes | {measures.get('complete_units', 0)} / {measures.get('total_units', 0)} |",
        "",
        "Statuts des unités dans `configure.py` : **M** `Matching` (identique, liée), "
        "**E** `Equivalent` (complète, pas identique : entre dans le port), "
        "**N** `NonMatching` (incomplète ou absente : hors du port).",
        "",
    ]

    by_family = defaultdict(list)
    for module, entry in per_module.items():
        by_family[family(module)].append((module, entry))

    lines += ["## Par famille", "", "| famille | modules | code | identique | reste |", "|---|---|---|---|---|"]
    for _, name in FAMILIES:
        items = by_family.get(name, [])
        if not items:
            continue
        code = sum(e["code"] for _, e in items)
        matched = sum(e["matched"] for _, e in items)
        pct = 100.0 * matched / code if code else 100.0
        lines.append(f"| {name} | {len(items)} | {kb(code)} Ko | {pct:.1f} % | {kb(code - matched)} Ko |")
    lines.append("")

    lines += ["## Par module", ""]
    for _, name in FAMILIES:
        items = sorted(by_family.get(name, []), key=lambda item: item[0].lower())
        if not items:
            continue
        lines += [f"### {name}", "", "| module | code | identique | fonctions | fuzzy | unités M/E/N | état |", "|---|---|---|---|---|---|---|"]
        for module, e in items:
            pct = 100.0 * e["matched"] / e["code"] if e["code"] else 100.0
            fuzzy = e["fuzzy_weighted"] / e["code"] if e["code"] else 100.0
            s = status[module]
            if s["NonMatching"] == 0 and (s["Matching"] or s["Equivalent"]):
                state = "fait" if s["Equivalent"] == 0 else "fait (E)"
            elif e["matched"] > 0 or s["Matching"] or s["Equivalent"]:
                state = "en cours"
            else:
                state = "à faire"
            lines.append(
                f"| `{module}` | {kb(e['code'])} Ko | {pct:.1f} % | {e['mfns']}/{e['fns']} | {fuzzy:.1f} % | "
                f"{s['Matching']}/{s['Equivalent']}/{s['NonMatching']} | {state} |"
            )
        lines.append("")

    out = Path(args.output)
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text("\n".join(lines), encoding="utf-8")
    print(f"written {out}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
